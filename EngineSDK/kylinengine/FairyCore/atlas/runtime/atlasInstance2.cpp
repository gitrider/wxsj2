//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "console/consoleTypes.h"
#include "atlas/runtime/atlasInstance2.h"
#include "atlas/resource/atlasResourceConfigTOC.h"
#include "sceneGraph/sceneState.h"
#include "sceneGraph/sceneGraph.h"
#include "math/mMath.h"
#include "math/mathIO.h"
#include "app/game.h"
#include "core/stream/bitStream.h"
#include "core/util/path.h"
#include "gfx/sim/debugDraw.h"
#include "renderInstance/renderPassManager.h"
#include "math/mathUtils.h"
#include "clipmap/clipMapUniqueCache.h"
#include "clipmap/clipMapBlenderCache.h"
#include "clipmap/clipMap.h"
#include "atlas/runtime/atlasClipMapBatcher.h"
#include "atlas/runtime/atlasClipMapImageSource.h"

IMPLEMENT_CO_NETOBJECT_V1(AtlasInstance);
S32 AtlasInstance::smRayCollisionDebugLevel = AtlasInstance::RayCollisionDebugToTriangles;
S32 AtlasInstance::smRenderChunkBoundsDebugLevel = AtlasInstance::ChunkBoundsDebugNone;
bool AtlasInstance::smLockFrustrum = false;
bool AtlasInstance::smRenderWireframe = false;
bool AtlasInstance::smNoUpdate = false;

void AtlasInstance::consoleInit()
{
   Con::addVariable("AtlasInstance2::rayCollisionDebugLevel", TypeS32, &smRayCollisionDebugLevel);
   Con::addVariable("AtlasInstance2::renderChunkBoundsDebugLevel", TypeS32, &smRenderChunkBoundsDebugLevel);
   Con::addVariable("AtlasInstance2::lockFrustrum", TypeBool, &smLockFrustrum);
   Con::addVariable("AtlasInstance2::renderWireframe", TypeBool, &smRenderWireframe);
   Con::addVariable("AtlasInstance2::renderDebugTextures", TypeBool, &ClipMap::smDebugTextures);
   Con::addVariable( "AtlasInstance2::noUpdate", TypeBool, &smNoUpdate );
}

void AtlasInstance::initPersistFields()
{
   Parent::initPersistFields();

   addField("detailTex", TypeFilename, Offset(mDetailTexFileName, AtlasInstance));
   addField("atlasFile", TypeFilename, Offset(mAtlasFileName, AtlasInstance));
   addField("lightmapDimension", TypeS32, Offset(mLightmapDimension, AtlasInstance));   
}

//-----------------------------------------------------------------------------

AtlasInstance::AtlasInstance()
{
   mTypeMask = AtlasObjectType | StaticObjectType |
	   StaticRenderedObjectType | ShadowCasterObjectType;

   mAtlasFile = NULL;
   mAtlasMissionFile = NULL;
   mLightMapFile = NULL;
   mGeomTOC   = NULL;
   mObjectTOC = NULL;
   mDetailTexFileName = NULL;
   mAtlasFileName = StringTable->insert("terrain_water_demo/clean.atlas");
   mAtlasMissionFileName = StringTable->insert("terrain_water_demo/clean_mission.atlas");
   mClipMap = NULL;
   mIsBlended = false;
   mLightmapDimension = 256;
   mLMChunkSize = 0;
   mNumBlendLayers = 0;
   mOpacityToc = NULL;
}

AtlasInstance::~AtlasInstance()
{
	if(mAtlasMissionFile)
		delete mAtlasMissionFile;
}

bool AtlasInstance::onAdd()
{
#ifdef TORQUE_OS_MAC
   String warning("As of OS X 10.5.6 Atlas causes a hard lock in the video drivers when trying to use \
                     glTexSubImage2D() in GFXGLTextureObject::unlock() so it has been disabled on Macs");

   Con::errorf(warning.c_str());
   AssertFatal(false, warning.c_str());

   return false;
#endif

   if(!Parent::onAdd())
      return false;

   mAtlasFile = AtlasFile::load(mAtlasFileName);

   if(mAtlasFile.isNull())
   {
      Con::errorf("AtlasInstance::onAdd - cannot open atlas file.");
      return false;
   }

   mAtlasFile->startLoaderThreads();

   // Bind the TOCs.
   AtlasResourceGeomTOC   *argtoc=NULL;
   AtlasResourceConfigTOC *arctoc=NULL;
   mAtlasFile->getTocBySlot(0, argtoc);
   mAtlasFile->getTocBySlot(0, arctoc);

   mGeomTOC = new AtlasInstanceGeomTOC;
   mGeomTOC->initializeTOC(argtoc);

   // Do configuration based initialization.
   if(arctoc)
   {
     
     AtlasConfigChunk *acc = NULL;
     if(arctoc->getConfig("schema", acc))
     {
        // Is this a unique or a blender TOC?
        const char *schemaType = NULL;
        if(acc->getEntry("type", schemaType))
        {
           if(!dStricmp(schemaType, "unique"))
           {
              // Grab the TOC - first texture TOC is what we want.
              AtlasResourceTexTOC *arttoc;
              if(mAtlasFile->getTocBySlot(0, arttoc))
              {
                 // Set up a clipmap with appropriate settings.
                 mClipMap = new ClipMap();
                 mClipMap->mClipMapSize = Con::getIntVariable("Pref::Atlas::clipMapSize", 512);

                 // Do unique texture setup - this also initializes the texture size on clipmap.
                 AtlasClipMapImageSource *acmis = new AtlasClipMapImageSource();
                 acmis->setTOC(arttoc);
                 acmis->setUnique(true);
                 ClipMapUniqueCache *cmuc = new ClipMapUniqueCache(acmis);
                 mClipMap->setCache(cmuc);

                 // Don't forget to set the texture size!
                 mClipMap->mTextureSize = BIT(acmis->getMipLevelCount()-1);
              }
              else
              {
                 Con::errorf("AtlasInstance2::onAdd - no texture TOC present, cannot initialize unique texturing!");
              }

           }
           else if(!dStricmp(schemaType, "blender"))
           {
              mIsBlended = true;
              // Get what slots our TOCs are in.
              const char *opacityTocSlot, *shadowTocSlot;
              bool gotTocSlotsOk = true;
              gotTocSlotsOk &= acc->getEntry("opacityMapSlot", opacityTocSlot);
              gotTocSlotsOk &= acc->getEntry("shadowMapSlot", shadowTocSlot);

              if(!gotTocSlotsOk)
              {
                 Con::errorf("AtlasInstance2::onAdd - unable to get opacity or shadow TOC"
                             " slot information from config block.");
                 goto endOfBlenderInitBlock;
              }

              // Grab TOCs.
              bool tocInitOk;
              AtlasResourceTexTOC *shadowToc;
              AtlasResourceTexTOC *opacityToc;
              tocInitOk = true;
              shadowToc = NULL;                  
              tocInitOk &= mAtlasFile->getTocBySlot(dAtoi(shadowTocSlot), shadowToc);
              tocInitOk &= mAtlasFile->getTocBySlot(dAtoi(opacityTocSlot), opacityToc);                  

              if(!tocInitOk)
              {
                 Con::errorf("AtlasInstance2::onAdd - unable to grab opacity or shadow TOC.");
                 goto endOfBlenderInitBlock;
              }

              // Initialize the clipmap.
              mClipMap = new ClipMap();
              mClipMap->mClipMapSize = Con::getIntVariable("Pref::Atlas::clipMapSize", 512);

              // What size should the virtual texture be?
              const char *virtualTexSize;
              if(!acc->getEntry("virtualTexSize", virtualTexSize) || dAtoi(virtualTexSize) == 0)
              {
                 Con::errorf("AtlasInstance2::onAdd - no virtual texture Size specified in config block or got zero size.");
                 goto endOfBlenderInitBlock;
              }
              else
              {
                 mClipMap->mTextureSize = dAtoi(virtualTexSize);
              }
              AtlasClipMapImageSource* acmis_opacity;
              acmis_opacity = new AtlasClipMapImageSource();
              acmis_opacity->setTOC(opacityToc);
              acmis_opacity->setUnique(false);
              AtlasClipMapImageSource *acmis_lightmap = new AtlasClipMapImageSource();
              acmis_lightmap->setTOC(shadowToc);
              acmis_opacity->setUnique(false);
              mAcmic_b = new ClipMapBlenderCache(acmis_opacity, acmis_lightmap, true);
              mClipMap->setCache(mAcmic_b);

              mLMChunkSize = acmis_lightmap->getChunkSize();

              // And grab source images.
              const char *srcImageCount;
              if(!acc->getEntry("sourceImageCount", srcImageCount))
              {
                 Con::errorf("AtlasInstance2::onAdd - no source image count specified.");
                 goto endOfBlenderInitBlock;
              }

              S32 srcCount = dAtoi(srcImageCount);
              mNumBlendLayers = srcCount;
              for(S32 i=0; i<srcCount; i++)
              {
                 const char *srcImage;
                 if(!acc->getEntry(avar("sourceImage%d", i), srcImage))
                 {
                    Con::errorf("AtlasInstance2::onAdd - no source image specified at index %d!", i);
                    mMaterialList.push_back( String() );
                    continue;
                 }

                 char buff[1024];
                 Platform::makeFullPathName(srcImage, buff, 1024, mAtlasFile.getFilePath());
                 String relPath = Platform::makeRelativePathName(buff, Platform::getCurrentDirectory());
                 mAcmic_b->registerSourceImage(relPath);

                 // Add to material list for mapping.

                 Torque::Path path( srcImage );
                 mMaterialList.push_back( path.getFileName() );
              }

              for(S32 i=srcCount; i<4; i++)
                 mAcmic_b->registerSourceImage("");

              mMaterialList.mapMaterials();

endOfBlenderInitBlock: ;

           }
           else
           {
              Con::errorf("AtlasInstance2::onAdd - unknown texture schema type '%s'", schemaType);
           }
        }
        else
        {
           Con::errorf("AtlasInstance2::onAdd - no texture schema type specified", schemaType);
        }
     }
     else
     {
        Con::errorf("AtlasInstance2::onAdd - no texture schema present.");
     }		   

     if(mClipMap)
     {
        // We need to initialize the clipmap before we can do stuff to it!
        mClipMap->initClipStack();

        // If there is a clipmap, make sure it's all loaded and ready to go.
        U32 time = Platform::getRealMilliseconds();
        while(!mClipMap->fillWithTextureData())
        {
           syncThreads();               
           Platform::sleep(10);
        }
        U32 postTime = Platform::getRealMilliseconds();
        Con::printf("AtlasInstance2::onAdd - took %d ms to fill clipmap with texture data.", postTime - time);
     }
     else
     {
        Con::errorf("AtlasInstance2::onAdd - failed to initialize clipmap!");
     }

     if( mDetailTexFileName )
     {
        if( !mDetailTex.set( mDetailTexFileName, &GFXDefaultStaticDiffuseProfile, avar( "%s - detailTex", __FUNCTION__ ) ) )
        {
           Con::warnf( "AtlasInstance2::onAdd - could not load detail map; disabling detail mapping" );
           mBatcher.isDetailMappingEnabled( false );
        }
     }
     else
      mBatcher.isDetailMappingEnabled( false );

     mBatcher.initMaterials();

   }

   // Root node contains all children, so we can just grab its bounds and go from there.
   AtlasResourceGeomStub *args = mGeomTOC->getResourceStub(mGeomTOC->getStub(0));
   mObjBox = args->mBounds;

   // Get our collision info as well.
   mGeomTOC->loadCollisionLeafChunks();
   

   
   char baseAtlas[256];
   dSprintf(baseAtlas,sizeof(baseAtlas),"%s",mAtlasFileName);
   char * dot = dStrstr((const char*)baseAtlas, ".atlas");
   if(dot)
	   *dot = '\0';
   char missionName [256];
   dSprintf(missionName,sizeof(missionName),"%s_mission.atlas",baseAtlas);
   mAtlasMissionFileName = missionName;
   if(!Platform::isFile(mAtlasMissionFileName))
   {
	   AtlasFile af;
	   
	   AtlasResourceObjectTOC* aro_toc = new AtlasResourceObjectTOC;
	   aro_toc->initializeTOC(mGeomTOC->getTreeDepth());

	   //AtlasInstanceObjectTOC * aio_toc = new AtlasInstanceObjectTOC;
	   //aio_toc->initializeTOC(aro_toc);
	   
	   af.registerTOC(aro_toc);
	   af.createNew(mAtlasMissionFileName);
	   af.startLoaderThreads();
	   AtlasResourceGeomTOC* arg_toc = mGeomTOC->getResourceTOC();
	   
	   for(S32 i=0; i<aro_toc->getStubCount(); i++)
	   {
		   // Get our original stub, and issue the load.
		   AtlasResourceGeomStub *origArgs = arg_toc->getStub(i);
		   //arg_toc->immediateLoad(origArgs, AtlasTOC::EditorLoad);

		   // Make a chunk copy and instate it.
		   AtlasResourceObjectStub *aros = aro_toc->getStub(i);
		 
		   AtlasObjectChunk* aoc = new AtlasObjectChunk();
           aoc->tsObjectCount = 0;
		   aoc->bitmapCount = 0;
		   aoc->materialCount = 0;
		   aoc->mBounds = origArgs->mBounds;
		   //aros->mChunk = aoc;
		   //origArgs->purge();
           aro_toc->instateNewChunk(aros, aoc);
		   aros->purge();
		   
	   }
	   af.waitForPendingWrites();
	  
   }
   mAtlasMissionFile = new AtlasFile();
   if(!mAtlasMissionFile->openForWrite(mAtlasMissionFileName))
   {
	   Con::warnf( "AtlasInstance2::onAdd - could not load mission atlasFile %s",mAtlasMissionFileName);
	   return false;
   }
   mAtlasMissionFile->startLoaderThreads();
   AtlasResourceObjectTOC *arotoc = NULL;
   mAtlasMissionFile->getTocBySlot(0,arotoc);
   if(!arotoc)
   {
	   Con::warnf( "AtlasInstance2::onAdd - could not get mission toc ");
	   return false;
   }
   mObjectTOC = new AtlasInstanceObjectTOC;
   mObjectTOC->initializeTOC(arotoc);
   mObjectTOC->loadCollisionLeafChunks();
   /*if(!mObjectTOC)
   {
	   mObjectTOC = new AtlasInstanceObjectTOC;
	   AtlasResourceObjectTOC* aro_toc = new AtlasResourceObjectTOC;
	   aro_toc->initializeTOC(mGeomTOC->getTreeDepth());
	   //mAtlasMissionFile->getTocBySlot(0,aro_toc);
	   mObjectTOC->initializeTOC(aro_toc);
	   mAtlasFile->registerTOC(aro_toc);
       //syncThreads();

	   AtlasResourceGeomTOC* arg_toc = mGeomTOC->getResourceTOC();
      
	   for(S32 i=0; i<aro_toc->getStubCount(); i++)
	   {
		   // Get our original stub, and issue the load.
		   AtlasResourceGeomStub *origArgs = arg_toc->getStub(i);
		   // Make a chunk copy and instate it.
		   AtlasResourceObjectStub *aros = aro_toc->getStub(i);

		   AtlasObjectChunk* aoc = new AtlasObjectChunk();
		   aoc->objectCount = 0;
		   aoc->bitmapCount = 0;
		   aoc->materialCount = 0;
		   aoc->shapeCount = 0;
		   aoc->mBounds = origArgs->mBounds;
		   //aros->mChunk = aoc;
		   aro_toc->instateNewChunk(aros,aoc);
		   
	   }
	   U32 len = mAtlasFile->getDeferredFile().lockStream(true)->getStreamSize();
	   mAtlasFile->getDeferredFile().unlockStream();
	   mAtlasFile->waitForPendingWrites();

   }
   */
   
   // Do general render initialization stuff.
   setRenderTransform(mObjToWorld);
   resetWorldBox();
   addToScene();
   Sim::getAtlasSet()->addObject(this);

   // Ok, all done.
   return true;
}

void AtlasInstance::loadAlternateLightMap(const StringTableEntry sLightmapFile)
{   
   if (!mIsBlended)
   {
      Con::errorf("Alternate lightmaps are only supported for blended terrains!");
      return;
   }
   // Cleanup if we had a lightmap loaded already
   mLightMapFile = NULL;

   mLightMapFile = AtlasFile::load(sLightmapFile);
   mLightMapFile->startLoaderThreads();

   // Bind the TOCs.
   AtlasResourceConfigTOC *lmArctoc=NULL;

   mLightMapFile->getTocBySlot(0, lmArctoc);

   AtlasResourceTexTOC *shadowToc = NULL;

   AtlasConfigChunk *lmacc = NULL;
   if(lmArctoc->getConfig("schema", lmacc))
   {
      // Is this a unique or a blender TOC?
      const char *schemaType = NULL;
      if(lmacc->getEntry("type", schemaType))
      {
         if(!dStricmp(schemaType, "blender"))
         {
            // Get what slots our TOCs are in.
            const char *shadowTocSlot;
            bool gotTocSlotsOk = true;                           
            gotTocSlotsOk = lmacc->getEntry("shadowMapSlot", shadowTocSlot);                           
            if (!gotTocSlotsOk)
            {
               Con::errorf("Unable to find shadow map slot!");               
               return;
            }             

            if (!mLightMapFile->getTocBySlot(dAtoi(shadowTocSlot), shadowToc))
            {
               Con::errorf("Unable to load light map!");
               return;
            }
            AtlasClipMapImageSource* newLightmapImageSource = new AtlasClipMapImageSource();
            newLightmapImageSource->setTOC(shadowToc);
            mLMChunkSize = newLightmapImageSource->getChunkSize();
            mAcmic_b->setNewLightmapSource(newLightmapImageSource);            
            while(!mClipMap->fillWithTextureData())
            {
               syncThreads();
               Platform::sleep(10);
            }
         }
      }
   }   
}

void AtlasInstance::releaseLightmap()
{
   if (!mIsBlended)
   {
      Con::errorf("Alternate lightmaps are only supported for blended terrains!");
      return;
   }
   if (mLightMapFile.isNull())
      return;
   // Cleanup
   mLightMapFile.purge();
   mLightMapFile = NULL;   

   // Load the default lightmap   
   AtlasResourceConfigTOC *lmArctoc=NULL;
   mAtlasFile->getTocBySlot(0, lmArctoc);
   AtlasResourceTexTOC *shadowToc = NULL;
   AtlasConfigChunk *lmacc = NULL;
   if(lmArctoc->getConfig("schema", lmacc))
   {
      // Is this a unique or a blender TOC?
      const char *schemaType = NULL;
      if(lmacc->getEntry("type", schemaType))
      {
         if(!dStricmp(schemaType, "blender"))
         {
            // Get what slots our TOCs are in.
            const char *shadowTocSlot;
            bool gotTocSlotsOk = true;                           
            gotTocSlotsOk = lmacc->getEntry("shadowMapSlot", shadowTocSlot);                           
            if (!gotTocSlotsOk)
            {
               Con::errorf("Unable to find shadow map slot!");               
               return;
            }             

            if (!mAtlasFile->getTocBySlot(dAtoi(shadowTocSlot), shadowToc))
            {
               Con::errorf("Unable to load light map!");
               return;
            }
            AtlasClipMapImageSource* newLightmapImageSource = new AtlasClipMapImageSource();
            newLightmapImageSource->setTOC(shadowToc);
            mAcmic_b->setNewLightmapSource(newLightmapImageSource);            
            while(!mClipMap->fillWithTextureData())
            {
               syncThreads();
               Platform::sleep(10);
            }
         }
      }
   }   
}

S32 AtlasInstance::getLightMapChunkSize()
{
   if (mIsBlended)
   {
      return mLMChunkSize;
   } else {
      return -1;
   }
}


void AtlasInstance::onRemove()
{
   SAFE_DELETE(mGeomTOC);   
   SAFE_DELETE(mClipMap);

   mLightMapFile = NULL;
   mAtlasFile = NULL;
   Sim::getAtlasSet()->removeObject(this);
   // And let the rest of cleanup happen.
   removeFromScene();
   Parent::onRemove();
}

bool AtlasInstance::prepRenderImage(SceneState *state, const U32 stateKey,
                                     const U32 startZone, const bool modifyBaseZoneState)
{
   if (isLastState(state, stateKey))
      return false;
   setLastState(state, stateKey);

   // Make sure we play nice if we should be drawn no matter what.
   bool render = false;
   if(state->isTerrainOverridden())
      render = true;
   else
      render = state->isObjectRendered(this);

   if(render)
   {
      ObjectRenderInst *ri = gRenderInstManager->allocInst<ObjectRenderInst>();
      ri->state = state;
      ri->type = RenderPassManager::RIT_Object;
      ri->mRenderDelegate.bind( this, &AtlasInstance::renderObject );
      gRenderInstManager->addInst( ri );
   }

   return render;
}

void AtlasInstance::renderObject( ObjectRenderInst* ri, BaseMatInstance* overrideMat )
{
   PROFILE_START(AtlasInstance2_renderObject);

   //// Set up projection and world transform info.
   GFX->pushWorldMatrix();
   GFX->multWorld(getRenderTransform());
   MatrixF scaleMat(1);
   scaleMat.scale(getScale());
   GFX->multWorld(scaleMat);

   SceneState* state = ri->state;
   mGeomTOC->initCuller(state);

   //// Synch up with the atlas file.
   if(!gClientSceneGraph->isReflectPass() && !smNoUpdate)
   {
      PROFILE_START(reflectPass);

      // Set focus point.
      Point3F pos = state->getCameraPosition();
      mWorldToObj.mulP(pos);

      // Clamp focus point to the terrain bounds.
      Point3F focusPos;
      focusPos = mObjBox.getClosestPoint(pos);

      // Now, cast a ray to figure our texcoords at this location and recenter the clipmap.
      Point3F endPos;
      RayInfo ri;

      // First cast down...
      endPos = focusPos - Point3F(0,0,2.0*mWorldBox.len_z());

      if(mGeomTOC->castRay(focusPos, endPos, &ri, false))
      {
         mClipMap->recenter(ri.texCoord);
      }
      else
      {
         // Cast up...
         endPos = focusPos + Point3F(0,0,2.0*mWorldBox.len_z());
         if(mGeomTOC->castRay(focusPos, endPos, &ri, false))
            mClipMap->recenter(ri.texCoord);
      }

      // Sync up with threads once a frame - don't bother checking more than that.
      // Internally the file will filter down to one every 10ms anyway.
      syncThreads();

      // This clear time logic is actually kind of broken but it's ok as we
      // don't do anything in the clear() that uses dT. It would probably be
      // better to do heat dissipation from the processTick or somewhere else.
      static U32 lastTime = Sim::getCurrentTime();
      mGeomTOC->clear( F32(Sim::getCurrentTime() - lastTime) / 1000.f );
      lastTime = Sim::getCurrentTime();

      // And do our LOD processing.
      if(!smLockFrustrum)
         mGeomTOC->processLOD(state);

      PROFILE_END();
   }

   PROFILE_START(preBatch);

   //// First draw the bounds, if desired.
   if(smRenderChunkBoundsDebugLevel)
      mGeomTOC->renderBounds(smRenderChunkBoundsDebugLevel);

   //if(smRenderWireframe)
   //   GFX->setFillMode(GFXFillWireframe);

   PROFILE_END();

   // Let our batcher draw everything.
   if( !smNoUpdate )
   {
      mBatcher.init(mClipMap, state, this);
      mGeomTOC->batchGeometry(&mBatcher);
      mBatcher.sort(); 
   }
   mBatcher.render( overrideMat );

   PROFILE_START(postBatch);
   //if(smRenderWireframe)
   //   GFX->setFillMode(GFXFillSolid);

   GFX->popWorldMatrix();
   GFX->disableShaders();

   PROFILE_END();

   PROFILE_END();
}

bool AtlasInstance::castRay(const Point3F &start, const Point3F &end, RayInfo* info)
{
   // Check to see if we're doing object box debug collision.
   if(smRayCollisionDebugLevel == RayCollisionDebugToObjectBox)
   {
      F32 t;
      Point3F n;

      if(mObjBox.collideLine(start, end, &t, &n))
      {
         info->object = this;
         info->t = t;
         info->normal = n;
         return true;
      }

      return false;
   }

   // As we get our castray coords already in object space, we don't have to
   // do any transformation, just pass it on to the geometry TOC.
   bool ret = mGeomTOC->castRay(start, end, info, false);
   if( ret )
   {
      info->object = this;
      info->setContactPoint( start, end );

      // Set material (blended only).

      if( mIsBlended && mOpacityToc && mMaterialList.getMaterialCount() > 0 )
      {
         // Find out which material has the highest opacity at the contact
         // point (we're using the texcoords to find that out).

         S32 maxOpacity = -1;
         U32 matIndex = 0;

         for( U32 i = 0; i < mNumBlendLayers; i += 4 )
         {
            ColorI color;
            mOpacityToc->getTexel( info->texCoord, i / 4, color );

            for( U32 n = 0; n < 4; ++ n )
               if( color[ n ] > maxOpacity )
               {
                  maxOpacity = color[ n ];
                  matIndex = i + n;
               }
         }

         // Look up the material instance in the mapping list.

         info->material = mMaterialList.getMaterialInst( matIndex );
      }
      else
         info->material = NULL;
   }

   return ret;
}

void AtlasInstance::buildConvex(const Box3F& box, Convex* convex)
{

   // Get the box into local space and pass it off to the TOC.
   Box3F localBox = box;
   mWorldToObj.mul(localBox);

   mGeomTOC->buildCollisionInfo(localBox, convex, NULL, this);
}

bool AtlasInstance::buildPolyList(AbstractPolyList* polyList, const Box3F &box, const SphereF &sphere)
{
   // Get the box into local space and pass it off to the TOC.
   Box3F localBox = box;
   mWorldToObj.mul(localBox);

   polyList->setObject(this);
   polyList->setTransform(&getTransform(), getScale());

   // Since the polylist transforms for us we don't have to deal with
   // the transform stuff that convexes do.
   return mGeomTOC->buildCollisionInfo(localBox, NULL, polyList, this);
}

void AtlasInstance::findDeepestStubs(Vector<AtlasResourceGeomTOC::StubType *> &stubs)
{
   if(!mGeomTOC)
      return;

   AtlasResourceGeomTOC *toc = mGeomTOC->getResourceTOC();
   U32 treeDepth = toc->getTreeDepth();

   for(S32 x=0; x<BIT(treeDepth-1); x++)
   {
      for(S32 y=0; y<BIT(treeDepth-1); y++)
      {
         stubs.push_back(toc->getStub(treeDepth-1, Point2I(x,y)));
      }
   }
}

void AtlasInstance::refillClipMap()
{
   if(!mClipMap)
   {
      Con::errorf("AtlasInstance::refillClipMap - no clipmap on this object, maybe you want the client instance?");
      return;
   }

   mClipMap->fillWithTextureData();
}

void AtlasInstance::syncThreads()
{
   mAtlasFile->syncThreads();
   if (mLightMapFile)
      mLightMapFile->syncThreads();
}

//------------------------------------------------------------------------------

ConsoleMethod(AtlasInstance, purgeClipmap, void, 2, 2, "() - Refill clipmap from image cache.")
{
   U32 time = Platform::getRealMilliseconds();
   object->refillClipMap();
   U32 postTime = Platform::getRealMilliseconds();
   Con::printf("AtlasInstance2::purgeClipmap - took %d ms to refill clipmap.", postTime - time);
  
}

ConsoleMethod(AtlasInstance, purgeClipmapTimed, void, 2, 2, "() - Refill clipmap from image cache, time test.")
{
   const U32 time = Platform::getRealMilliseconds();
   
   for(S32 i=0; i<4; i++)
      object->refillClipMap();
   
   const U32 postTime = Platform::getRealMilliseconds();
   Con::printf("AtlasInstance2::purgeClipmap - took %d avg ms to refill clipmap (4 trials).", 
                  (postTime - time) / 4);
}

#ifdef TORQUE_DEBUG

ConsoleFunction(atlasEmitCastRayTest, void, 2, 2, "(pos) - draw a bunch of debug raycasts from the specified point.")
{
   Point3F pos;

   dSscanf(argv[1], "%f %f %f", &pos.x, &pos.y, &pos.z);

   for(S32 i=-2; i<3; i++)
   {
      for(S32 j=-2; j<3; j++)
      {
         const F32 stepFactor = 10;
         const F32 probeDepth = -10;

         Point3F start = pos;
         Point3F end = start + Point3F(F32(i) * stepFactor, F32(j) * stepFactor, probeDepth);

         RayInfo r;
         bool ret;
         DebugDrawer* debugDraw = DebugDrawer::get();

         // Do Client...
         ret = gClientContainer.castRay(start, end, AtlasObjectType, &r);
         if(ret)
            debugDraw->drawLine(start, r.point, ColorF(0, 1, 1));
         else
            debugDraw->drawLine(start, end, ColorF(1, 0, 1));

         // Do server...
         ret = gClientContainer.castRay(start, end, AtlasObjectType, &r);
         if(ret)
            debugDraw->drawLine(start, r.point, ColorF(0, 1, 0));
         else
            debugDraw->drawLine(start, end, ColorF(1, 0, 0));

         debugDraw->setLastTTL(5000);
      }
   }
}

ConsoleFunction(testPNGCompression, void, 1, 1, "")
{
   GBitmap *gb = GBitmap::load("terrain_water_demo/alpha");

   // Seperate pixels, apply delta encoding and serialized.
   FileStream fs;
   fs.open("terrain_water_demo/alphaOut.raw",  Torque::FS::File::Write);

   for(S32 channel=0; channel<4; channel++)
   {
      S32 pixelCount = gb->getWidth() * gb->getHeight();
      const U8 *bits = gb->getBits();

      U8 lastPix = 0;
      for(S32 i=0; i<pixelCount; i++)
      {
         // Encode...
         const U8 pixel = bits[i * gb->getBytesPerPixel() + channel];
         U8 encPixel = pixel; // - lastPix;
         lastPix = pixel;

         // Serialize...
         fs.write(encPixel);
      }
   }

   fs.close();
}

#endif
