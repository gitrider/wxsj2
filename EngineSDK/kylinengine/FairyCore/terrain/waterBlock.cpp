//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "terrain/waterBlock.h"
#include "core/util/safeDelete.h"
#include "sceneManager/sceneState.h"
#include "sceneManager/sceneManager.h"
#include "sceneManager/lightInfo.h"
#include "materials/matInstance.h"
#include "materials/materialManager.h"
#include "core/stream/bitStream.h"
#include "math/mathIO.h"
#include "console/consoleTypes.h"
#include "gui/3d/guiTSControl.h"
#include "graphic/primBuilder.h"
#include "graphic/gTransformSaver.h"
#include "renderManager/renderPassManager.h"

#define BLEND_TEX_SIZE 256
#define V_SHADER_PARAM_OFFSET 50

IMPLEMENT_CO_NETOBJECT_V1(WaterBlock);

void WaterBlock::WaterMatParams::init(BaseMatInstance* matInst)
{
   mWaveDataSC = matInst->getMaterialParameterHandle("$waveData");
   mTimeDataSC = matInst->getMaterialParameterHandle("$timeData");
   mWaveTexScaleSC = matInst->getMaterialParameterHandle("$waveTexScale");
   mReflectTexSizeSC = matInst->getMaterialParameterHandle("$reflectTexSize");
   mBaseColorSC = matInst->getMaterialParameterHandle("$baseColor");
   mMiscParamsSC = matInst->getMaterialParameterHandle("$miscParams");
   mTimeDataSC = matInst->getMaterialParameterHandle("$timeData");
}

//*****************************************************************************
// WaterBlock
//*****************************************************************************
WaterBlock::WaterBlock()
{
   mGridElementSize = 100.0f;
   mObjScale.set( 100.0f, 100.0f, 10.0f );

   mFullReflect = true;
   mTypeMask = WaterObjectType;

   mObjBox.minExtents.set( -0.5f, -0.5f, -0.5f );
   mObjBox.maxExtents.set(  0.5f,  0.5f,  0.5f );

   mElapsedTime = 0.0f;
   mReflectTexSize = 512;
   mBaseColor.set( 11, 26, 44, 255 );
   mUnderwaterColor.set( 25, 51, 76, 153 );
   
   for( U32 i=0; i<MAX_WAVES; i++ )
   {
      mWaveDir[i].set( 0.0f, 0.0f );
      mWaveSpeed[i] = 0.0f;
      mWaveTexScale[i].set( 0.0f, 0.0f );
   } 

   dMemset( mSurfMatName, 0, sizeof(mSurfMatName) );
   dMemset( mMatInstances, 0, sizeof(mMatInstances) );

   mClarity = 0.15f;
   mFresnelBias = 0.12f;
   mFresnelPower = 6.0f;
   mVisibilityDepth = 10.0;

   mRenderUpdateCount = 0;
   mReflectUpdateCount = 0;
   mReflectUpdateTicks = 0;

   mRenderFogMesh = true;

   mRenderDelegate.bind(this, &WaterBlock::renderObject);
}

WaterBlock::~WaterBlock()
{
   cleanupMaterials();
}

//-----------------------------------------------------------------------------
// onAdd
//-----------------------------------------------------------------------------
bool WaterBlock::onAdd()
{
   if( !Parent::onAdd() )
      return false;

   mPrevScale = mObjScale;

  initMaterials();

  if( GRAPHIC->getPixelShaderVersion() >= 1.4 && mFullReflect )
  {
     // add to reflective set of objects (sets up updateReflection() callback)
     SimSet *reflectSet = dynamic_cast<SimSet*>( Sim::findObject( "reflectiveSet" ) );
     reflectSet->addObject( (SimObject*)this );
     mReflectPlane.setupTex( mReflectTexSize );
  }
  else
  {
     setupRadialVBIB();

     // Create render target for watery bump on 1.1 card implementation
     mBumpTex.set( BLEND_TEX_SIZE, BLEND_TEX_SIZE, GFormatR8G8B8A8, &GDefaultRenderTargetProfile, avar("%s() - mBumpTex (line %d)", __FUNCTION__, __LINE__), 1 );
  }
  setupVBIB();
  // Create our state blocks
  GStateBlockDesc sb;
  sb.blendDefined = true;
  sb.blendEnable = true;
  sb.blendSrc = GBlendSrcAlpha;
  sb.blendDest = GBlendInvSrcAlpha;
  sb.zDefined = true;
  sb.zEnable = false;
  sb.cullDefined = true;
  sb.cullMode = GCullNone;
  mUnderwaterSB = GRAPHIC->createStateBlock(sb);
   
   resetWorldBox();
   addToScene();

   return true;
}

//-----------------------------------------------------------------------------
// onRemove
//-----------------------------------------------------------------------------
void WaterBlock::onRemove()
{
   clearVertBuffers();
   cleanupMaterials();

   removeFromScene();
   Parent::onRemove();
}

// Setup vertex and index buffers
//-----------------------------------------------------------------------------
void WaterBlock::setupVBIB()
{
   clearVertBuffers();

   const U32 maxIndexedVerts = 65536; // max number of indexed verts with U16 size indices

   if( mObjScale.x < mGridElementSize ||
       mObjScale.y < mGridElementSize )
   {
      AssertISV( false, "WaterBlock: Scale must be larger than gridElementSize" );
   }

   mWidth   = (U32)mFloor( mObjScale.x / mGridElementSize ) + 1;
   mHeight  = (U32)mFloor( mObjScale.y / mGridElementSize ) + 1;

   // figure out how many blocks are needed and their size
   U32 maxBlockRows = maxIndexedVerts / mWidth;
   U32 rowOffset = 0;
   
   while( (rowOffset+1) < mHeight )
   {
      U32 numRows = mHeight - rowOffset;
      if( numRows == 1 ) numRows++;
      if( numRows > maxBlockRows )
      {
         numRows = maxBlockRows;
      }

      setupVertexBlock( mWidth, numRows, rowOffset );
      setupPrimitiveBlock( mWidth, numRows );

      rowOffset += numRows - 1;
   }

}

//-----------------------------------------------------------------------------
// Set up a block of vertices - the width is always the width of the entire
// waterBlock, so this is a block of full rows.
//-----------------------------------------------------------------------------
void WaterBlock::setupVertexBlock( U32 width, U32 height, U32 rowOffset )
{
   Point3F pos = getPosition();
   RayInfo rInfo;
   VectorF sunVector(-0.61f, 0.354f, 0.707f);

   LightManager* lm = gSceneManager->getLightManager();
   if (lm)
   {
      LightInfo* linfo = lm->getSpecialLight(LightManager::slTranslatedSunlightType);

      if (linfo)
         sunVector = linfo->mDirection;
   }

   sunVector.normalize();

   U32 numVerts = width * height;

   GVertexPCT *verts = new GVertexPCT[ numVerts ];
   ColorI waterColor(31, 56, 64, 127);
   GVertexColor vertCol(waterColor);

   U32 index = 0;
   for( U32 i=0; i<height; i++ )
   {
      for( U32 j=0; j<width; j++, index++ )
      {
         GVertexPCT *vert = &verts[index];
         vert->point.x = (-mObjScale.x / 2.0) + mGridElementSize * j;
         vert->point.y = (-mObjScale.y / 2.0) + mGridElementSize * (i + rowOffset);
         vert->point.z = 0.0;
         vert->color = vertCol;
         // Calculate the water depth

         Point3F start, end;
         Point3F worldPoint = vert->point + pos;

         start.x = end.x = worldPoint.x;
         start.y = end.y = worldPoint.y;
         start.z = -2000; // Really high, might be over kill
         end.z = 2000; // really low, might be overkill

         // Cast a ray to see how deep the water is. We are
         // currently just testing for terrain and atlas
         // objects, but potentially any object that responds
         // to a ray cast could detected.
         if(gClientContainer.castRay(start, end, 
            //StaticObjectType | 
            InteriorObjectType | 
            //ShapeBaseObjectType | 
            //StaticShapeObjectType | 
            //ItemObjectType |
            //StaticTSObjectType |
            TerrainObjectType |
            AtlasObjectType
            , &rInfo))
         {
            F32 depth = -(rInfo.point.z - pos.z);

            if(depth <= 0.0f)
            {
               depth = 1.0f;
            }
            else
            {
               depth = depth / mVisibilityDepth;
               if(depth > 1.0f)
               {
                  depth = 1.0f;
               }

               depth = 1.0f - depth;
            }

            vert->texCoord.x = depth;
         }
         else
         {
            vert->texCoord.x = 0.0f;
         }

         // Cast a ray to do some AO-style shadowing.
         F32 &shadow = vert->texCoord.y;

         if(gClientContainer.castRay(worldPoint, worldPoint + sunVector * 9000.f, 
            //StaticObjectType | 
            InteriorObjectType | 
            //ShapeBaseObjectType | 
            //StaticShapeObjectType | 
            //ItemObjectType |
            //StaticTSObjectType |
            TerrainObjectType |
            AtlasObjectType
            , &rInfo))
         {
            shadow = 0.f;
         }
         else
         {
            shadow = 1.f;
         }
      }
   }

   // copy to vertex buffer
   GVertexBufferHandle <GVertexPCT> * vertBuff = new GVertexBufferHandle <GVertexPCT>;

   vertBuff->set( GRAPHIC, numVerts, GBufferTypeStatic );
   GVertexPCT *vbVerts = vertBuff->lock();
   dMemcpy( vbVerts, verts, sizeof(GVertexPCT) * numVerts );
   vertBuff->unlock();
   mVertBuffList.push_back( vertBuff );
   

   delete [] verts;

}

//-----------------------------------------------------------------------------
// Set up a block of indices to match the block of vertices. The width is 
// always the width of the entire waterBlock, so this is a block of full rows.
//-----------------------------------------------------------------------------
void WaterBlock::setupPrimitiveBlock( U32 width, U32 height )
{
   // setup vertex / primitive buffers
   U32 numIndices = (width-1) * (height-1) * 6;
   U16 *indices = new U16[ numIndices ];
   U32 numVerts = width * height;

   // This uses indexed triangle lists instead of strips, but it shouldn't be
   // significantly slower if the indices cache well.
   
   // Rough diagram of the index order
   //   0----2----+ ...
   //   |  / |    |
   //   |/   |    |
   //   1----3----+ ...
   //   |    |    |
   //   |    |    |
   //   +----+----+ ...

   U32 index = 0;
   for( U32 i=0; i<(height-1); i++ )
   {
      for( U32 j=0; j<(width-1); j++, index+=6 )
      {
         // Process one quad at a time.  Note it will re-use the same indices from
         // previous quad, thus optimizing vert cache.  Cache will run out at
         // end of each row with this implementation however.
         indices[index+0] = (i) * mWidth + j;         // 0
         indices[index+1] = (i+1) * mWidth + j;       // 1
         indices[index+2] =  i * mWidth + j+1;        // 2
         indices[index+3] = (i+1) * mWidth + j;       // 1
         indices[index+4] = (i+1) * mWidth + j+1;     // 3
         indices[index+5] =  i * mWidth + j+1;        // 2
      }

   }

   GPrimitiveBufferHandle *indexBuff = new GPrimitiveBufferHandle;
   
   GPrimitive pInfo;
   pInfo.type = GTriangleList;
   pInfo.numPrimitives = numIndices / 3;
   pInfo.startIndex = 0;
   pInfo.minIndex = 0;
   pInfo.numVertices = numVerts;

   U16 *ibIndices;
   GPrimitive *piInput;
   indexBuff->set( GRAPHIC, numIndices, 1, GBufferTypeStatic );
   indexBuff->lock( &ibIndices, &piInput );
   dMemcpy( ibIndices, indices, numIndices * sizeof(U16) );
   dMemcpy( piInput, &pInfo, sizeof(GPrimitive) );
   indexBuff->unlock();
   mPrimBuffList.push_back( indexBuff );


   delete [] indices;
}


//-----------------------------------------------------------------------------
// prepRenderImage
//-----------------------------------------------------------------------------
bool WaterBlock::prepRenderImage( SceneState* state, 
                                 const U32   stateKey,
                                 const U32, 
                                 const bool )
{
   PROFILE_SCOPE(WaterBlock_prepRenderImage);

   if( isLastState(state, stateKey) )
      return false;

   setLastState(state, stateKey);

   // This should be sufficient for most objects that don't manage zones, and
   //  don't need to return a specialized RenderImage...
   if( state->isObjectRendered(this) )
   {
      ObjectRenderInst *ri = gRenderInstManager->allocInst<ObjectRenderInst>();
      ri->mRenderDelegate = mRenderDelegate;
      ri->state = state;
      ri->type = RenderPassManager::RIT_Water;
      gRenderInstManager->addInst( ri );

      mRenderUpdateCount++;
   }

   return false;
}

//------------------------------------------------------------------------------
// Setup scenegraph data structure for materials
//------------------------------------------------------------------------------
SceneGraphData WaterBlock::setupSceneGraphInfo( SceneState *state )
{
   SceneGraphData sgData;

   LightManager* lm = gSceneManager->getLightManager();
   sgData.light = lm->getSpecialLight(LightManager::slTranslatedSunlightType);
   sgData.lightSecondary = lm->getDefaultLight();

   // fill in camera position relative to water
   sgData.camPos = state->getCameraPosition();

   // fill in water's transform
   sgData.objTrans = getRenderTransform();

   // fog
   sgData.setFogParams(gSceneManager);

   // misc
   sgData.backBuffTex = GRAPHIC->getSfxBackBuffer();
   sgData.reflectTex = mReflectPlane.getTex();
   sgData.miscTex = mBumpTex;


   return sgData;
}

void WaterBlock::setShaderXForms(BaseMatInstance* mat)
{
   // setup proj/world transform
   GRAPHIC->pushWorldMatrix();   
   MatrixF world = GRAPHIC->getWorldMatrix();
   world.mul( getRenderTransform() );
   GRAPHIC->setWorldMatrix( world );
}

//-----------------------------------------------------------------------------
// set shader parameters
//-----------------------------------------------------------------------------
void WaterBlock::setShaderParams(BaseMatInstance* mat, const WaterMatParams& paramHandles)
{
   mElapsedTime = (F32)Platform::getVirtualMilliseconds()/1000.0f; // uggh, should multiply by timescale (it's in main.cpp)

   MaterialParameters* matParams = mat->getMaterialParameters();
   
   // set vertex shader constants
   //-----------------------------------
   for( U32 i=0; i<MAX_WAVES; i++ )
   {
      mConstArray[i].set(mWaveDir[i].x, mWaveDir[i].y);
   }
   matParams->set(paramHandles.mWaveDataSC, mConstArray);

   Point4F timeData;
   for( U32 i=0; i<MAX_WAVES; i++ )
   {
      timeData[i] = mWaveSpeed[i] * mElapsedTime;
   }      
   matParams->set(paramHandles.mTimeDataSC, timeData);
   
   for( U32 i=0; i<MAX_WAVES; i++ )
   {
      Point2F texScale = mWaveTexScale[i];
      if( texScale.x > 0.0 )
      {
         texScale.x = 1.0 / texScale.x;
      }
      if( texScale.y > 0.0 )
      {
         texScale.y = 1.0 / texScale.y;
      }

      mConstArray[i].set(texScale.x, texScale.y);
   }
   matParams->set(paramHandles.mWaveTexScaleSC, mConstArray);
   matParams->set(paramHandles.mReflectTexSizeSC, (F32)mReflectTexSize);

   // set pixel shader constants
   //-----------------------------------

   ColorF c(mBaseColor);
   matParams->set(paramHandles.mBaseColorSC, c);

   Point3F miscParams(mClarity, mFresnelBias, mFresnelPower);
   matParams->set(paramHandles.mMiscParamsSC, miscParams);   
}

//-----------------------------------------------------------------------------
// renderObject
//-----------------------------------------------------------------------------
void WaterBlock::renderObject(ObjectRenderInst *ri, BaseMatInstance* overrideMat)
{
   if (overrideMat)
      return;

   if( ri->state->getSceneManager()->isReflectPass() )
      return;

   //-------------------------------------
   // copy  backbuffer
   //-------------------------------------
   GRAPHIC->copyBBToSfxBuff();

   SceneGraphData sgData = setupSceneGraphInfo( ri->state );
   const Point3F &camPos = ri->state->getCameraPosition();

   if( mRender11 )
   {
      if (GRAPHIC->getPixelShaderVersion() > 0.0)
         animBumpTex( ri->state );
      render1_1( ri->state, sgData, camPos );
   }
   else
   {
      render2_0( ri->state, sgData, camPos );
   }
}

//-----------------------------------------------------------------------------
// render water for 1.1 pixel shader cards
//-----------------------------------------------------------------------------
void WaterBlock::render1_1( SceneState * state, SceneGraphData &sgData, const Point3F &camPosition )
{
   // update plane
   PlaneF plane;
   Point3F norm;
   getRenderTransform().getColumn( 2, &norm );
   norm.normalize();
   plane.set( getRenderPosition(), norm );
   mReflectPlane.setPlane( plane );

   // set the material
   BaseMatInstance *mat = NULL;
   if( !mFullReflect )
   {
      // This is here because the 1_1 pass is run by 2.0 cards when full reflect is off
      mat = mMatInstances[ NO_REFLECT ];
   }
   else
   {
      mat = mMatInstances[ BASE_PASS ];
   }

   bool underWater = false;
   if( isUnderwater( camPosition ) )
   {
      underWater = true;
      mat = mMatInstances[UNDERWATER_PASS];
 
      if( GRAPHIC->getPixelShaderVersion() >= 2.0 )
      {         
         setShaderParams(mat, mMatParamHandles[UNDERWATER_PASS]);
      }
   }

   // render the grid geometry
   if( mat )
   {
      setShaderXForms(mat);
      while( mat->setupPass( state, sgData ) )
      {         
         for( U32 i=0; i<mVertBuffList.size(); i++ )
         {
            // set vert/prim buffer
            GRAPHIC->setVertexBuffer( *mVertBuffList[i] );
            GPrimitiveBuffer *primBuff = *mPrimBuffList[i];
            GRAPHIC->setPrimitiveBuffer( primBuff );
            GRAPHIC->drawPrimitives();
         }
      }
      GRAPHIC->popWorldMatrix();
   }

   if( mRenderFogMesh )
   {
      // render radial geometry
      if( !underWater )
      {
         GRAPHIC->popWorldMatrix();
         GRAPHIC->pushWorldMatrix();
         MatrixF world = GRAPHIC->getWorldMatrix();

         MatrixF matTrans = getRenderTransform();
         Point3F pos = plane.project( camPosition );
         matTrans.setPosition( pos );
         world.mul( matTrans );
         GRAPHIC->setWorldMatrix( world );
         sgData.objTrans = matTrans;

         GRAPHIC->setVertexBuffer( mRadialVertBuff );
         GPrimitiveBuffer *primBuff = mRadialPrimBuff;
         GRAPHIC->setPrimitiveBuffer( primBuff );

         mat = mMatInstances[FOG_PASS];

         if( mat )
         {
            setShaderXForms(mat);
            while( mat->setupPass(state, sgData ) )
            {
               GRAPHIC->drawPrimitives();
            }
            GRAPHIC->popWorldMatrix();
         }
      }
   }

   if( underWater )
   {
      drawUnderwaterFilter();
   }
}

//-----------------------------------------------------------------------------
// render water for 2.0+ pixel shader cards
//-----------------------------------------------------------------------------
void WaterBlock::render2_0( SceneState * state, SceneGraphData &sgData, const Point3F &camPosition )
{
   // set the material
   BaseMatInstance *mat = mMatInstances[ BASE_PASS ];
   WaterMatParams matParams = mMatParamHandles[BASE_PASS];

   bool underwater = false;
   if( isUnderwater( camPosition ) )
   {
      underwater = true;
      mat = mMatInstances[UNDERWATER_PASS];
      matParams = mMatParamHandles[UNDERWATER_PASS];
   }

   setShaderXForms(mat);
   setShaderParams(mat, matParams);

   // render the geometry
   if( mat )
   {
      while( mat->setupPass( state, sgData ) )
      {                 
         for( U32 i=0; i<mVertBuffList.size(); i++ )
         {
            // set vert/prim buffer
            GRAPHIC->setVertexBuffer( *mVertBuffList[i] );
            GPrimitiveBuffer *primBuff = *mPrimBuffList[i];
            GRAPHIC->setPrimitiveBuffer( primBuff );
            GRAPHIC->drawPrimitives();
         }
      }
   }

   GRAPHIC->popWorldMatrix();

   if( underwater )
   {
      drawUnderwaterFilter();
   }
}

//-----------------------------------------------------------------------------
// initPersistFields
//-----------------------------------------------------------------------------
void WaterBlock::initPersistFields()
{
   Parent::initPersistFields();

   addGroup("WaveData");
   addField( "waveDir",       TypePoint2F,  Offset( mWaveDir, WaterBlock ), MAX_WAVES );
   addField( "waveSpeed",     TypeF32,  Offset( mWaveSpeed, WaterBlock ), MAX_WAVES );
   addField( "waveTexScale",  TypePoint2F,  Offset( mWaveTexScale, WaterBlock ), MAX_WAVES );
   endGroup("WaveData");

   addGroup("Misc");
   addField( "reflectTexSize", TypeS32,  Offset( mReflectTexSize, WaterBlock ) );
   addField( "baseColor", TypeColorI,  Offset( mBaseColor, WaterBlock ) );
   addField( "underwaterColor", TypeColorI,  Offset( mUnderwaterColor, WaterBlock ) );
   addField( "gridSize", TypeF32,  Offset( mGridElementSize, WaterBlock ) );
   endGroup("Misc");

   addField( "surfMaterial", TypeString,  Offset( mSurfMatName, WaterBlock ), NUM_MAT_TYPES );
   addField( "fullReflect",  TypeBool,  Offset( mFullReflect, WaterBlock ) );
   addField( "clarity",  TypeF32,  Offset( mClarity, WaterBlock ) );
   addField( "fresnelBias",  TypeF32,  Offset( mFresnelBias, WaterBlock ) );
   addField( "fresnelPower",  TypeF32,  Offset( mFresnelPower, WaterBlock ) );
   addField( "visibilityDepth", TypeF32, Offset( mVisibilityDepth, WaterBlock ) );

   addField( "renderFogMesh",  TypeBool,  Offset( mRenderFogMesh, WaterBlock ) );
}     


//-----------------------------------------------------------------------------
// Update planar reflection
//-----------------------------------------------------------------------------
void WaterBlock::updateReflection()
{
   PROFILE_SCOPE(WaterBlock_updateReflection);

   // Simple method of insuring reflections aren't rendered unless
   // the waterBlock is actually visible
   if( mRenderUpdateCount > mReflectUpdateCount )
      mReflectUpdateCount = mRenderUpdateCount;
   else
      return;

   // Update water reflection no more than specified interval - causes some
   // artifacts if you whip the camera around a lot, but definitely can
   // improve speed.
   U32 curTime = Platform::getVirtualMilliseconds();
   if( curTime - mReflectUpdateTicks < 16 )
      return;

   // grab camera transform from tsCtrl
   GuiTSCtrl *tsCtrl;
   tsCtrl = dynamic_cast<GuiTSCtrl*>( Sim::findObject("PlayGui") );
   CameraQuery query;
   if(!tsCtrl->processCameraQuery( &query ))
      return;

   mReflectUpdateTicks = curTime;

   // store current matrices
   GRAPHIC->pushWorldMatrix();
   MatrixF proj = GRAPHIC->getProjectionMatrix();

   // set up projection - must match that of main camera
   Point2I resolution = gSceneManager->getDisplayTargetResolution();
   GRAPHIC->setFrustum( mRadToDeg(query.fov),
      F32(resolution.x) / F32(resolution.y ),
      query.nearPlane, query.farPlane );

   // store "normal" camera position before changing over to reflected position
   MatrixF camTrans = query.cameraMatrix;
   gSceneManager->mNormCamPos = camTrans.getPosition();

   // update plane
   PlaneF plane;
   Point3F norm;
   getRenderTransform().getColumn( 2, &norm );
   norm.normalize();
   plane.set( getRenderPosition(), norm );
   mReflectPlane.setPlane ( plane );

   // set world mat from new camera view
   MatrixF camReflectTrans = mReflectPlane.getCameraReflection( camTrans );
   camReflectTrans.inverse();
   GRAPHIC->setWorldMatrix( camReflectTrans );

   // set new projection matrix
   gSceneManager->setNonClipProjection( (MatrixF&) GRAPHIC->getProjectionMatrix() );
   MatrixF clipProj = mReflectPlane.getFrustumClipProj( camReflectTrans );
   GRAPHIC->setProjectionMatrix( clipProj );

   // render a frame
   gSceneManager->setReflectPass( true );

   GTextureTargetRef myTarg = GRAPHIC->allocRenderToTextureTarget();
   myTarg->attachTexture(GTextureTarget::Color0, mReflectPlane.getTex() );
   myTarg->attachTexture(GTextureTarget::DepthStencil, mReflectPlane.getDepth() );
   GRAPHIC->pushActiveRenderTarget();
   GRAPHIC->setActiveRenderTarget( myTarg );

   GRAPHIC->clear( GClearZBuffer | GClearStencil | GClearTarget, ColorI( 64, 64, 64 ), 1.0f, 0 );
   U32 objTypeFlag = -1;
   gSceneManager->renderScene( objTypeFlag );
   GRAPHIC->popActiveRenderTarget();

   // cleanup
   GRAPHIC->clear( GClearZBuffer | GClearStencil, ColorI( 255, 0, 255 ), 1.0f, 0 );
   gSceneManager->setReflectPass( false );
   GRAPHIC->popWorldMatrix();
   GRAPHIC->setProjectionMatrix( proj );
}

//-----------------------------------------------------------------------------
// Draw translucent filter over screen when underwater
//-----------------------------------------------------------------------------
void WaterBlock::drawUnderwaterFilter()
{
   // set up camera transforms
   MatrixF proj = GRAPHIC->getProjectionMatrix();
   MatrixF newMat(true);
   GRAPHIC->setProjectionMatrix( newMat );
   GRAPHIC->pushWorldMatrix();
   GRAPHIC->setWorldMatrix( newMat );   

   // set up render states
   GRAPHIC->disableShaders();

   GRAPHIC->setStateBlock(mUnderwaterSB);

   // draw quad
   Point2I resolution = GRAPHIC->getActiveRenderTarget()->getSize();
   F32 copyOffsetX = 1.0 / resolution.x;
   F32 copyOffsetY = 1.0 / resolution.y;

   PrimBuild::color( mUnderwaterColor );
   PrimBuild::begin( GTriangleFan, 4 );
      PrimBuild::texCoord2f( 0.0, 1.0 );
      PrimBuild::vertex3f( -1.0 - copyOffsetX, -1.0 + copyOffsetY, 0.0 );
   
      PrimBuild::texCoord2f( 0.0, 0.0 );
      PrimBuild::vertex3f( -1.0 - copyOffsetX,  1.0 + copyOffsetY, 0.0 );
   
      PrimBuild::texCoord2f( 1.0, 0.0 );
      PrimBuild::vertex3f(  1.0 - copyOffsetX,  1.0 + copyOffsetY, 0.0 );
   
      PrimBuild::texCoord2f( 1.0, 1.0 );
      PrimBuild::vertex3f(  1.0 - copyOffsetX, -1.0 + copyOffsetY, 0.0 );
   PrimBuild::end();

   // reset states / transforms
   GRAPHIC->setProjectionMatrix( proj );
   GRAPHIC->popWorldMatrix();
}

//-----------------------------------------------------------------------------
// Animate the bump texture - for 1.1 cards
//-----------------------------------------------------------------------------
void WaterBlock::animBumpTex( SceneState *state )
{
   GTransformSaver saver;   // mostly for viewport.

   // set up blend shader - pass in wave params
   BaseMatInstance *mat = mMatInstances[BLEND];
   const WaterMatParams& matParam = mMatParamHandles[BLEND];
   
   // set ortho projection matrix
   MatrixF proj = GRAPHIC->getProjectionMatrix();
   MatrixF newMat(true);
   GRAPHIC->setProjectionMatrix( newMat );
   GRAPHIC->pushWorldMatrix();
   GRAPHIC->setWorldMatrix( newMat );

   // send time info to shader
   mElapsedTime = (F32)Platform::getVirtualMilliseconds()/1000.0f; // uggh, should multiply by timescale (it's in main.cpp)
   F32 timeScale = mElapsedTime * 0.15f;
   mat->getMaterialParameters()->set(matParam.mTimeDataSC, timeScale);
   
   GRAPHIC->pushActiveRenderTarget();
   GTextureTargetRef myTarg = GRAPHIC->allocRenderToTextureTarget();
   myTarg->attachTexture(GTextureTarget::Color0, mBumpTex);
   //myTarg->attachTexture(GTextureTarget::DepthStencil, GTextureTarget::sDefaultDepthStencil );
   GRAPHIC->setActiveRenderTarget( myTarg );
   
   SceneGraphData sgData = setupSceneGraphInfo( state ); // hrmm, eliminate this?
   F32 copyOffsetX = 1.0f / BLEND_TEX_SIZE;
   F32 copyOffsetY = 1.0f / BLEND_TEX_SIZE;

   // render out new bump texture
   while( mat->setupPass( state, sgData ) )
   {
      PrimBuild::begin( GTriangleFan, 4 );
         PrimBuild::texCoord2f( 0.0f, 1.0f );
         PrimBuild::vertex3f( -1.0f - copyOffsetX, -1.0f + copyOffsetY, 0.0f );
      
         PrimBuild::texCoord2f( 0.0f, 0.0f );
         PrimBuild::vertex3f( -1.0f - copyOffsetX,  1.0f + copyOffsetY, 0.0f );
      
         PrimBuild::texCoord2f( 1.0f, 0.0f );
         PrimBuild::vertex3f(  1.0f - copyOffsetX,  1.0f + copyOffsetY, 0.0f );
      
         PrimBuild::texCoord2f( 1.0f, 1.0f );
         PrimBuild::vertex3f(  1.0f - copyOffsetX, -1.0f + copyOffsetY, 0.0f );
      PrimBuild::end();
   }
   
   GRAPHIC->popActiveRenderTarget();

   // done
   GRAPHIC->popWorldMatrix();
   GRAPHIC->setProjectionMatrix( proj );
}

void WaterBlock::initMaterials()
{
   GMaterialFeatureData::FeatureListHandle features = MaterialManager::get()->getDefaultFeatureData().codify();
   // Figure out which set of shaders we're using.
   mRender11 = GRAPHIC->getPixelShaderVersion() < 2.0 || !mFullReflect && mRadialVertBuff;

   // Load in various Material definitions
   for( U32 i=0; i<NUM_MAT_TYPES; i++ )
   {
      GStateBlockDesc desc;
      desc.cullDefined = true;
      desc.cullMode = GCullNone;

      if (mRenderFogMesh && mRender11)
      {
         desc.stencilDefined = true;
         desc.stencilEnable = true;
         desc.stencilRef = 2;
         desc.stencilFunc = GCmpAlways;
         desc.stencilPassOp = GStencilOpReplace;
      }

      GVertexPC* vert = NULL;
      GVertexFlags flags = (GVertexFlags)getGVertFlags(vert);
      if( mSurfMatName[i] && mSurfMatName[i][0] )
      {
         delete mMatInstances[i];
         mMatInstances[i] = MaterialManager::get()->createMatInstance(mSurfMatName[i]);
         if ((mRenderFogMesh) && (i != FOG_PASS) && (i != BLEND))
         {
            mMatInstances[i]->addStateBlockDesc(desc);
         }            
         mMatInstances[i]->init(features, features, flags);
         mMatParamHandles[i].init(mMatInstances[i]);
         if (mConstArray.getElementSize() == 0)
            mConstArray.setCapacity(MAX_WAVES, mMatInstances[i]->getMaterialParameters()->getAlignmentValue(GSCT_Float2));
      }
      if( !mMatInstances[i] )
      {
         Con::warnf( "Invalid Material name: %s: for WaterBlock ps2.0+ surface", mSurfMatName[i] );
      }
   }
}

void WaterBlock::cleanupMaterials()
{
   for (U32 i = 0; i < NUM_MAT_TYPES; i++)
   {
      SAFE_DELETE(mMatInstances[i]);
   }
}

//-----------------------------------------------------------------------------
// Set up the vertex/index buffers for a radial ( concentric rings ) mesh
//-----------------------------------------------------------------------------
void WaterBlock::setupRadialVBIB()
{
   Vector< GVertexPC > verts;
   Vector< U16 > indices;

   // create verts
   U32 numRadPoints = 15;
   U32 numSections = 30;

   GVertexPC vert;
   vert.point.set( 0.0, 0.0, 0.0 );
   ColorI waterColor(31, 56, 64, 127);
   vert.color = GVertexColor(waterColor);
   verts.push_back( vert );

   F32 radius = 1.0;

   // set up rings
   for( U32 i=0; i<numSections; i++ )
   {
      for( U32 j=0; j<numRadPoints; j++ )
      {
         F32 angle = F32(j) / F32(numRadPoints) * M_2PI;
         vert.point.set( radius * mSin(angle), radius * mCos(angle), 0.0 );
         verts.push_back( vert );
      }
      radius *= 1.3f;
   }

   // set up indices for innermost circle
   for( U32 i=0; i<numRadPoints; i++ )
   {
      U16 index = 0;
      indices.push_back( index );
      index = i+1;
      indices.push_back( index );
      index = ((i+1) % numRadPoints) + 1;
      indices.push_back( index );
   }

   // set up indices for concentric rings around the center
   for( U32 i=0; i<numSections-1; i++ )
   {
      for( U32 j=0; j<numRadPoints; j++ )
      {
         U16 pts[4];
         pts[0] = 1 + i * numRadPoints + j;
         pts[1] = 1 + (i+1) * numRadPoints + j;
         pts[2] = 1 + i * numRadPoints + (j+1) % numRadPoints;
         pts[3] = 1 + (i+1) * numRadPoints + (j+1) % numRadPoints;

         indices.push_back( pts[0] );
         indices.push_back( pts[1] );
         indices.push_back( pts[2] );

         indices.push_back( pts[1] );
         indices.push_back( pts[3] );
         indices.push_back( pts[2] );
      }
   }


   // copy to vertex buffer
   mRadialVertBuff.set( GRAPHIC, verts.size(), GBufferTypeStatic );
   GVertexPC *vbVerts = mRadialVertBuff.lock();
   dMemcpy( vbVerts, verts.address(), sizeof(GVertexPC) * verts.size() );
   mRadialVertBuff.unlock();

   // copy to index buffer
   GPrimitive pInfo;
   pInfo.type = GTriangleList;
   pInfo.numPrimitives = indices.size() / 3;
   pInfo.startIndex = 0;
   pInfo.minIndex = 0;
   pInfo.numVertices = verts.size();

   U16 *ibIndices;
   GPrimitive *piInput;
   mRadialPrimBuff.set( GRAPHIC, indices.size(), 1, GBufferTypeStatic );
   mRadialPrimBuff.lock( &ibIndices, &piInput );
   dMemcpy( ibIndices, indices.address(), indices.size() * sizeof(U16) );
   dMemcpy( piInput, &pInfo, sizeof(GPrimitive) );
   mRadialPrimBuff.unlock();
}

//-----------------------------------------------------------------------------
// Returns true if specified point is under the water plane and contained in
// the water's bounding box.
//-----------------------------------------------------------------------------
bool WaterBlock::isUnderwater( const Point3F &pnt )
{

  PlaneF plane;
  Point3F norm;
  getTransform().getColumn( 2, &norm );
  norm.normalize();
  plane.set( getPosition(), norm );
  mReflectPlane.setPlane( plane );

   if( mReflectPlane.getPlane().distToPlane( pnt ) < -0.005 )
   {
      if( mWorldBox.isContained( pnt ) )
      {
         return true;
      }
   }

   return false;
}

//-----------------------------------------------------------------------------
// Clear vertex and primitive buffers
//-----------------------------------------------------------------------------
void WaterBlock::clearVertBuffers()
{
   for( U32 i=0; i<mVertBuffList.size(); i++ )
      delete mVertBuffList[i];
   mVertBuffList.clear();

   for( U32 i=0; i<mPrimBuffList.size(); i++ )
      delete mPrimBuffList[i];
   mPrimBuffList.clear();
}

//-----------------------------------------------------------------------------
// Set up projection matrix for multipass technique with different geometry.
// It basically just pushes the near plane out.  This should work across 
// fixed-function and shader geometry.
//-----------------------------------------------------------------------------
/*
void WaterBlock::setMultiPassProjection()
{
   F32 nearPlane, farPlane;
   F32 left, right, bottom, top;
   bool ortho = false;
   GRAPHIC->getFrustum( &left, &right, &bottom, &top, &nearPlane, &farPlane, &ortho );

   F32 FOV = GameGetCameraFov();
   Point2I size = GRAPHIC->getVideoMode().resolution;

//   GRAPHIC->setFrustum( FOV, size.x/F32(size.y), nearPlane + 0.010, farPlane + 10.0 );

// note - will have to re-calc left, right, top, bottom if the above technique
// doesn't work through portals
//   GRAPHIC->setFrustum( left, right, bottom, top, nearPlane + 0.001, farPlane );


}
*/

bool WaterBlock::castRay(const Point3F& start, const Point3F& end, RayInfo* info )
{
   // Simply look for the hit on the water plane
   // and ignore any future issues with waves, etc.
   const Point3F norm(0,0,1);
   PlaneF plane( Point3F::Zero, norm );

   F32 hit = plane.intersect( start, end );
   if ( hit < 0.0f || hit > 1.0f )
      return false;
   
   info->t = hit;
   info->object = this;
   info->point = start + ( ( end - start ) * hit );
   info->normal = norm;
   info->material = mMatInstances[ BASE_PASS ];

   return true;
}
