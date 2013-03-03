//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "atlas/resource/atlasResourceGeomTOC.h"
#include "atlas/runtime/atlasClipMapBatcher.h"
#include "materials/shaderData.h"
#include "sceneGraph/sceneGraph.h"
#include "sceneGraph/sceneState.h"
#include "math/mathUtils.h"
#include "materials/sceneData.h"
#include "materials/materialManager.h"
#include "atlas/runtime/atlasInstance2.h"
#include "gfx/gfxCardProfile.h"

Vector<AtlasRenderPlugin*> AtlasClipMapBatcher::mRenderPlugins;

AtlasClipMapBatcher::AtlasClipMapBatcher()
{
   mObject = 0;
   mClipMap = NULL;
   mState = 0;
   
   mDetailMapFadeStartDistance = 30.f;
   mDetailMapFadeEndDistance = 40.f;

   // And have the detail map repeat 128 times across the terrain.
   mDetailMapTextureMultiply = 128.f;

   // Set up our render lists.
   mRenderList[0] = NULL;
   for(S32 i=1; i<4; i++)
      mRenderList[i].clear();

   for (U32 i = 0; i < mRenderPlugins.size(); i++)
      mRenderPlugins[i]->init();

   mIsDetailMappingEnabled = true;

   mDetailMat = 0;
   mFogMat = 0;

   mBrightnessScaleSC = 0;
   mDetDataSC = 0;
}

AtlasClipMapBatcher::~AtlasClipMapBatcher()
{
   if( mDetailMat )
      SAFE_DELETE( mDetailMat );
   if( mFogMat )
      SAFE_DELETE( mFogMat );
}

void AtlasClipMapBatcher::init(ClipMap *acm, SceneState *state, AtlasInstance *renderer)
{
   PROFILE_START(AtlasClipMapBatcher_init);

   // Note our render state.
   mClipMap = acm;
   mState = state;
   mObject = renderer;
   
   // Empty the render lists...
   for(S32 i=1; i<4; i++)
      mRenderList[i].clear();
   mDetailList.clear();
   mLightList.clear();
   mFogList.clear();

   // And clear the render notes.
   mRenderNoteAlloc.freeBlocks();

   PROFILE_END();
}

void AtlasClipMapBatcher::initMaterials()
{
   // [rene - 20/10/08: shamelessly adapted from terrain render stuff.
   //    All this will soon go the way of all code and be superseded anyway.
   //    The entire rendering stuff shared between legacy and Atlas has
   //    become a mess. ]

   mFixedFunction = GFX->getPixelShaderVersion() == 0.0f;

   GFXAtlasVert2* vert = NULL;

   // Grab the material for the detail pass
   if( !mFixedFunction )
      mDetailMat = MaterialManager::get()->createMatInstance("TerrainDetailMaterial", (GFXVertexFlags)getGFXVertFlags(vert));
   else
   {
      if( GFX->getCardProfiler()->queryProfile( "lerpDetailBlend" ) )
         mDetailMat = MaterialManager::get()->createMatInstance("TerrainDetailFFLerpOpMaterial", (GFXVertexFlags)getGFXVertFlags(vert));
      else if( GFX->getCardProfiler()->queryProfile( "fourStageDetailBlend" ) )
         mDetailMat = MaterialManager::get()->createMatInstance("TerrainDetailFF4StageMaterial", (GFXVertexFlags)getGFXVertFlags(vert));
   }

   if (!mDetailMat)
      Con::errorf( "AtlasClipMapBatcher::initMaterials - Unable to find supported terrain detail material!");    
   else
   {
      mBrightnessScaleSC = mDetailMat->getMaterialParameterHandle(ClipMap::smBrightnessScale);
      mDetDataSC = mDetailMat->getMaterialParameterHandle(ClipMap::smDetData);         
   }

   mFogMat = MaterialManager::get()->createMatInstance("TerrainFogMaterial", (GFXVertexFlags)getGFXVertFlags(vert));
   if (!mFogMat)
      Con::errorf("AtlasClipMapBatcher::initMaterials - Unable to find TerrainFogMaterial!");         
}

void AtlasClipMapBatcher::queue(const Point3F &camPos, AtlasResourceGeomStub *args)
{
   PROFILE_START(AtlasClipMapBatcher_queue);

   AtlasGeomChunk *agc = args->mChunk;

   Point3F nearPos, farPos;
   Point2F nearTC, farTC;
   agc->calculatePoints(camPos, nearPos, farPos, nearTC, farTC );
   const F32 nearDistance = (camPos - nearPos).len();
   const F32 farDistance = (camPos - farPos).len();

   const RectF texBounds(nearTC, farTC - nearTC);

   // Now, calculate and store levels into a new RenderNote.
   S32 startLevel, endLevel;
   mClipMap->calculateClipMapLevels(nearDistance, farDistance, texBounds, startLevel, endLevel);

   // Allocate a render note.
   RenderNote *rn = mRenderNoteAlloc.alloc();

   // Check if this chunk will get fogged - consider furthest point, and if
   // it'll be fogged then draw a fog pass.
   if(mState->getHazeAndFog(farDistance, farPos.z - camPos.z) > (1.0/256.0)
      || mState->getHazeAndFog(farDistance, -(farPos.z - camPos.z)) > (1.0/256.0))
      mFogList.push_back(rn);

   // Is this chunk within the detail map range?
   if( isDetailMappingEnabled() && nearDistance < mDetailMapFadeEndDistance )
		   mDetailList.push_back(rn);

   // Let our plugins have a shot at the render note.
   for (U32 i = 0; i < mRenderPlugins.size(); i++)
   {
      mRenderPlugins[i]->queue(rn, mObject, args);
   }

   rn->levelStart = startLevel;
   rn->levelEnd   = endLevel;
   rn->levelCount = endLevel - startLevel + 1;
   rn->chunk      = agc;
   rn->nearDist   = nearDistance;

   // Stuff into right list based on shader.
   switch(rn->levelCount)
   {
   case 2:
   case 3:
   case 4:
      mRenderList[rn->levelCount-1].push_back(rn);
      break;

   default:
      Con::errorf("AtlasClipMapBatcher::queue - got unexpected level count of %d", rn->levelCount);
      break;
   }

   AssertFatal(rn->levelCount >= 2 && rn->levelCount <= 4, 
      "AtlasClipMapBatcher::queue - bad level count!");

   PROFILE_END();
}

S32 AtlasClipMapBatcher::cmpRenderNote(const void *a, const void *b)
{
   RenderNote *fa = *((RenderNote **)a);
   RenderNote *fb = *((RenderNote **)b);

   // Sort by distance.
   if(fa->nearDist > fb->nearDist)
      return 1;
   else if(fa->nearDist < fb->nearDist)
      return -1;

   // So this is what equality is like...
   return 0;
}

void AtlasClipMapBatcher::sort()
{
   PROFILE_START(AtlasClipMapBatcher_sort);

   // Sort our elements. The other lists are already good to go, and there's
   // no benefit to drawing them in a specific order as this pass (the first
   // pass) will already set up the Z buffer.
   for(S32 i=1; i<4; i++)
      dQsort(mRenderList[i].address(),mRenderList[i].size(),sizeof(RenderNote*),cmpRenderNote);

   PROFILE_END();
}

void AtlasClipMapBatcher::renderClipMap( SceneGraphData& sgData, BaseMatInstance* overrideMat )
{
   PROFILE_START(AtlasClipMapBatcher_renderClipMap);

   for(S32 curBin=1; curBin<4; curBin++)
   {
      // If bin is empty, skip.
      if(mRenderList[curBin].size() == 0)
         continue;

      for(S32 i=0; i<mRenderList[curBin].size(); i++)
      {
         // Grab the render note.
         const RenderNote *rn = mRenderList[curBin][i];

         // Set up clipmap levels.
         if( !mFixedFunction )
         {
            BaseMatInstance* material = overrideMat;
            if( !material )
               switch( rn->levelCount )
               {
               case 2:     material = mClipMap->getMaterialAndTextures( rn->levelEnd, rn->levelStart, -1, -1, false ); break;
               case 3:     material = mClipMap->getMaterialAndTextures( rn->levelEnd, rn->levelStart + 1, rn->levelStart, -1, false ); break;
               case 4:     material = mClipMap->getMaterialAndTextures( rn->levelEnd, rn->levelStart + 2, rn->levelStart + 1, rn->levelStart, false ); break;
               default:    material = MaterialManager::get()->getWarningMatInstance();
               }

            while( material->setupPass( mState, sgData ) )
               rn->chunk->render();
         }
         else
         {
            Point4F clipmapMapping;
            for( U32 curLayer = rn->levelEnd; curLayer >= rn->levelStart; -- curLayer )
            {
               BaseMatInstance* material = overrideMat;
               if( !material )
                  material = mClipMap->bindTexturesFF( curLayer, clipmapMapping, curLayer == rn->levelEnd, false );

               while( material->setupPass( mState, sgData ) )
                  rn->chunk->render();
            }
         }
      }
   }

   PROFILE_END();
}

void AtlasClipMapBatcher::renderFog( SceneGraphData& sgData )
{
   PROFILE_SCOPE(AtlasClipMapBatcher_renderFog);

   if( !mFogMat )
      return;

   if( mFixedFunction )
      GFX->setTexture( 0, mState->getSceneManager()->getFogTexture() );

   // And render all the fogged chunks - all for now.
   for(S32 i=0; i<mFogList.size(); i++)
   {
      // Grab the render note.
      const RenderNote *rn = mFogList[i];

      // And draw it.
      while( mFogMat->setupPass( mState, sgData ) )
         rn->chunk->render();
   }
}

void AtlasClipMapBatcher::renderDetail( SceneGraphData& sgData )
{
   PROFILE_SCOPE(AtlasClipMapBatcher_renderDetail);

   if( !mDetailMat )
      return;

   if( mFixedFunction )
   {
      MatrixF detailTexMatrix( true );
      detailTexMatrix.scale( Point3F( mDetailMapTextureMultiply, 
                                      mDetailMapTextureMultiply, 0.0f ) );

      GFX->setTextureMatrix( 0, detailTexMatrix );
   }
   else
   {
      Point3F detailConst(
         mDetailMapFadeStartDistance,
         1.f / (mDetailMapFadeEndDistance - mDetailMapFadeStartDistance), 
         mDetailMapTextureMultiply);
      F32 brightnessScale = 1.0;

      MaterialParameters* params = mDetailMat->getMaterialParameters();
      params->set( mDetDataSC, detailConst );
      params->set( mBrightnessScaleSC, brightnessScale );
   }

   GFX->setTexture( 0, mObject->mDetailTex );

   // And render all the detailed chunks - all for now.
   for(S32 i=0; i<mDetailList.size(); i++)
   {
      // Grab the render note.
      const RenderNote *rn = mDetailList[i];

      // And draw it.
      while( mDetailMat->setupPass( mState, sgData ) )
         rn->chunk->render();
   }
}

void AtlasClipMapBatcher::render( BaseMatInstance* overrideMat )
{
   PROFILE_SCOPE(AtlasClipMapBatcher_render);

   SceneGraphData sgData;
   sgData.setFogParams( mState->getSceneManager() );
   sgData.camPos = mState->getCameraPosition();
   sgData.objTrans = mObject->getRenderTransform();
   sgData.reflectPass = mState->mFlipCull;

//   LightManager* lightManager = mState->getLightManager();
//   if( lightManager )
//      lightManager->setupLights( mObject, mState->getCameraPosition(), Point3F::Zero, mState->getVisibleDistance(), 31 );

   renderClipMap( sgData, overrideMat );

   // Render detail, lighting, and fog, if not using override material.

   if( !overrideMat )
   {
      if( isDetailMappingEnabled() )
         renderDetail( sgData );

      // Let our plugins have a shot at rendering
      for (U32 i = 0; i < mRenderPlugins.size(); i++)
         mRenderPlugins[i]->render( mState, sgData, mObject); 

      renderFog( sgData );
   }

//   if( lightManager )
//      lightManager->resetLights();
}

void AtlasClipMapBatcher::registerRenderPlugin( AtlasRenderPlugin* arp )
{
   mRenderPlugins.push_back(arp);
}

void AtlasClipMapBatcher::unregisterRenderPlugin( AtlasRenderPlugin* arp )
{   
   for (S32 i = mRenderPlugins.size() - 1; i >= 0; i--)
   {
      if (mRenderPlugins[i] == arp)
         mRenderPlugins.erase(i);
   }
}
