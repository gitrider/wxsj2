//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "console/consoleTypes.h"
#include "math/mMath.h"
#include "core/frameAllocator.h"
#include "graphic/gDevice.h"
#include "graphic/bitmap/gBitmap.h"
#include "sceneManager/sceneState.h"
#include "sceneManager/sceneManager.h"
#include "sceneManager/sgUtil.h"
#include "sceneManager/lightInfo.h"
#include "platform/profiler.h"
#include "materials/matInstance.h"
#include "terrain/sky.h"
#include "terrain/terrRender.h"
#include "terrain/terrBatch.h"
#include "terrain/terrRender.h"
#include "terrain/waterBlock.h"

namespace TerrBatch
{
   // We buffer everything here then send it off to the GPU
   GAtlasVert2*          mVertexStore;
   GVertexPCNT*          mVertexStorePCNT;
   GVertexPCNTT*         mVertexStorePCNTT;
   U16                     mCurVertex;
   U16*                    mIndexStore;
   U16                     mCurIndex;
   GPrimitive*           mPrimitiveStore;
   U16                     mCurPrim;
   U32                     mCurXF;

   // Store previous counts for batching
   U16                     mLastIndex;
   U16                     mLastVertex;

   // Statistics
   S32                     mBiggestVertexBatch;
   S32                     mBiggestIndexBatch;

   const U32               mVertexStoreSize    = 16535;
   const U32               mIndexStoreSize     = 16535;
   const U32               mPrimitiveStoreSize = 4096;

   void init()
   {
      // Allocate space...
      mVertexStore    = new GAtlasVert2[mVertexStoreSize];
      mVertexStorePCNT = new GVertexPCNT[mVertexStoreSize];
      mVertexStorePCNTT = new GVertexPCNTT[mVertexStoreSize];
      mIndexStore     = new U16[mIndexStoreSize];
      mPrimitiveStore = new GPrimitive[mPrimitiveStoreSize];

/*      // Gratuitous mem usage spew
      Con::printf("Allocated terrain buffers...");
      Con::printf("    - Vertices      (bytes): %d", mVertexStore   .memSize());
      Con::printf("    - VerticesPCNT  (bytes): %d", mVertexStorePCNT.memSize());
      Con::printf("    - VerticesPCNTT (bytes): %d", mVertexStorePCNTT.memSize());
      Con::printf("    - Indices       (bytes): %d", mIndexStore    .memSize());
      Con::printf("    - Prims      (bytes): %d", mPrimitiveStore.memSize());      
      Con::printf("  === Total      (bytes): %d", mVertexStore.memSize() + mIndexStore.memSize() + mPrimitiveStore.memSize() + mTextureStore.memSize());
*/
      // Initialize
      mCurVertex  = 0;
      mCurIndex   = 0;
      mCurXF      = 1;

      mLastIndex  = 0;
      mLastVertex = 0;

      // Init statistics
      mBiggestIndexBatch = 0;
      mBiggestVertexBatch = 0;

      Con::addVariable("TRender::mBiggestIndexBatch", TypeS32, &mBiggestIndexBatch);
      Con::addVariable("TRender::mBiggestVertexBatch", TypeS32, &mBiggestVertexBatch);
   }

   void destroy()
   {
      delete [] mVertexStore;
      delete [] mVertexStorePCNT;
      delete [] mVertexStorePCNTT;
      delete [] mIndexStore;
      delete [] mPrimitiveStore;
   }
   
   void begin()
   {
      // Do nothing for now...
      mLastVertex = mCurVertex;
   }

   void terrTexGen( vertexType vtype, Point4F *clipmapMapping, const MatrixF &blockTransform, const Point3F &cameraPosition, LightInfo *light, SceneState * state)
   {
      PROFILE_SCOPE(Terrain_TexGen);
      SceneManager * sceneManager = state->getSceneManager();
      Point3F relative;
      const F32 blockTexCoordScale = 1.0f / (TerrainRender::mCurrentBlock->getSquareSize() * TerrainBlock::BlockSize);
      // Apply texgen to the new verts...
      if (vtype == vertexTypeFullClipMapping)
      {
         for(U32 i=0; i<mCurVertex; i++)
         {
            mVertexStore[i].texCoord.x = mVertexStore[i].point.x * blockTexCoordScale;
            mVertexStore[i].texCoord.y = mVertexStore[i].point.y * blockTexCoordScale;
         }
      }
      else if (vtype == vertexTypeSingleTextureClipMapping)
      {
         // Compute the fixedfunction vert stuff now
         AssertFatal(clipmapMapping != NULL, "TerrBatch::end - vertexTypeSingleTextureClipMapping requires clipmapMapping variable!");
         const F32 fadeConstant = 3.0f;
         const F32 blockTexCoordScale2 = blockTexCoordScale * clipmapMapping->z;
         for(U32 i=0; i<mCurVertex; i++)
         {
            mVertexStorePCNT[i].point = mVertexStore[i].point;
            mVertexStorePCNT[i].normal = mVertexStore[i].normal;
            mVertexStorePCNT[i].texCoord.x = mVertexStore[i].point.x * blockTexCoordScale2;
            mVertexStorePCNT[i].texCoord.y = mVertexStore[i].point.y * blockTexCoordScale2;
            relative.x = mVertexStorePCNT[i].texCoord.x - clipmapMapping->x;
            relative.y = mVertexStorePCNT[i].texCoord.y - clipmapMapping->y;
            relative.z = 0;
            // note: this uses 128.0f - instead of 255.0f - to hide some
            // transition artifacts at the edges (which are not visible
            // in the shader path due to its use of /2 in the vertex
            // shader and saturate(fade*2) in the pixel shader, which
            // allows sharp transitions to be interpolated more cleanly)
            mVertexStorePCNT[i].color.set(255, 255, 255, (U8)mClampF(128.0f - (relative.len() * (2.0f * fadeConstant) - (fadeConstant - 1.0f)) * 255.0f, 0.0f, 255.0f));
         }
      }
      else if (vtype == vertexTypeDLight)
      {
         // Compute the fixedfunction vert stuff now
         AssertFatal(clipmapMapping != NULL, "TerrBatch::end - vertexTypeDLight requires clipmapMapping variable!");
         AssertFatal(light != NULL, "TerrBatch::end - vertexTypeDLight requires light variable!");
         AssertFatal(light->mRadius > 0, "TerrBatch::end - vertexTypeDLight requires light->mRadius > 0!");
         const F32 blockTexCoordScale2 = blockTexCoordScale * clipmapMapping->z;
         const F32 heightOffset = sceneManager->getFogHeightOffset();
         const F32 inverseHeightRange = sceneManager->getFogInvHeightRange();
         const F32 inverseVisibleDistanceMod = 1.0f / sceneManager->getVisibleDistanceMod();
         Point3F worldPoint;
         const F32 lightRadius = light->mRadius;
         const Point3F lightPosition = light->mPos;
         F32 intensity;
         const F32 inverseLightRadius = 1.0f / lightRadius;
         // note: this imitates sgLightingModel only very loosely for
         // performance reasons, it does look very similar to the shader path
         for(U32 i=0; i<mCurVertex; i++)
         {
            mVertexStorePCNTT[i].point = mVertexStore[i].point;
            mVertexStorePCNTT[i].normal = mVertexStore[i].normal;
            mVertexStorePCNTT[i].texCoord[0].x = mVertexStore[i].point.x * blockTexCoordScale2;
            mVertexStorePCNTT[i].texCoord[0].y = mVertexStore[i].point.y * blockTexCoordScale2;
            blockTransform.mulP(mVertexStore[i].point, &worldPoint);
            relative = worldPoint - cameraPosition;
            mVertexStorePCNTT[i].texCoord[1].x = 1.0 - (relative.len() * inverseVisibleDistanceMod);
            mVertexStorePCNTT[i].texCoord[1].y = (worldPoint.z - heightOffset) * inverseHeightRange;
            relative = worldPoint - lightPosition;
            intensity = getMax(1.0f - relative.len() * inverseLightRadius, 0.0f);
            intensity = 512.0f * intensity;
            if (intensity > 0)
               mVertexStorePCNTT[i].color.set((U8)getMin(light->mColor.red * intensity, 255.0f), (U8)getMin(light->mColor.green * intensity, 255.0f), (U8)getMin(light->mColor.blue * intensity, 255.0f), 255);
            else
               mVertexStorePCNTT[i].color.set(0, 0, 0, 255);
         }
      }
      else if (vtype == vertexTypeFog)
      {
         const F32 heightOffset = sceneManager->getFogHeightOffset();
         const F32 inverseHeightRange = sceneManager->getFogInvHeightRange();
         const F32 inverseVisibleDistanceMod = 1.0f / sceneManager->getVisibleDistanceMod();
         Point3F worldPoint;
         for(U32 i=0; i<mCurVertex; i++)
         {
            mVertexStorePCNT[i].point = mVertexStore[i].point;
            mVertexStorePCNT[i].normal = mVertexStore[i].normal;
            blockTransform.mulP(mVertexStore[i].point, &worldPoint);
            relative = worldPoint - cameraPosition;
            mVertexStorePCNT[i].texCoord.x = 1.0 - (relative.len() * inverseVisibleDistanceMod);
            mVertexStorePCNT[i].texCoord.y = (worldPoint.z - heightOffset) * inverseHeightRange;
            mVertexStorePCNT[i].color.set(255, 255, 255, 255);
         }
      }
      // The only time 'vertexTypeDetail' is used is during a fixed-function detail pass.
      else if( vtype == vertexTypeDetail )
      {
         // Get detail distance squared to save us from sqrt
         const F32 detailDistanceSquared = TerrainRender::mCurrentBlock->mDetailDistance * TerrainRender::mCurrentBlock->mDetailDistance;

         // Detail Brightness done via assignment of color values
         const U8 colorByte = mClamp( 255 * TerrainRender::mCurrentBlock->mDetailBrightness, 0, 255 );

         Point3F worldPoint;
         for( U32 i = 0; i < mCurVertex; i++ )
         {
            mVertexStorePCNT[i].point = mVertexStore[i].point;
            mVertexStorePCNT[i].normal = mVertexStore[i].normal;
            mVertexStorePCNT[i].texCoord.x = mVertexStore[i].point.x * blockTexCoordScale;
            mVertexStorePCNT[i].texCoord.y = mVertexStore[i].point.y * blockTexCoordScale;

            // Transform vertex into world space
            blockTransform.mulP( mVertexStore[i].point, &worldPoint );
            relative = worldPoint - cameraPosition;

            // Alpha
            const F32 alpha = TerrainRender::mCurrentBlock->mDetailBrightness * ( 1.0f - ( relative.lenSquared() / detailDistanceSquared ) );

            // Assign alpha value to vert so the detail texture blend fades
            mVertexStorePCNT[i].color.set( colorByte, colorByte, colorByte, mClamp( alpha * 255, 0, 255 ) );
         }
      }
      else
      {
         for(U32 i=0; i<mCurVertex; i++)
         {
            mVertexStorePCNT[i].point = mVertexStore[i].point;
            mVertexStorePCNT[i].normal = mVertexStore[i].normal;
            mVertexStorePCNT[i].texCoord.x = mVertexStore[i].point.x * blockTexCoordScale;
            mVertexStorePCNT[i].texCoord.y = mVertexStore[i].point.y * blockTexCoordScale;
            mVertexStorePCNT[i].color.set(255, 255, 255, 255);
         }
      }
   }

   void vertexBufferCopy(vertexType vtype)
   {
      PROFILE_SCOPE(Terrain_vbufferCopy);

      // Do vertexes
      if (vtype == vertexTypeFullClipMapping)
      {
         GVertexBufferHandle<GAtlasVert2>  v(GRAPHIC, mCurVertex, GBufferTypeVolatile);
         PROFILE_START(Terrain_bufferCopy_lockV);
         v.lock();
         PROFILE_END();

         dMemcpy(&v[0], &mVertexStore[0], sizeof(GAtlasVert2) * mCurVertex);

         PROFILE_START(Terrain_bufferCopy_unlockV);
         v.unlock();
         PROFILE_END();
         GRAPHIC->setVertexBuffer(v);
      }
      else if (vtype == vertexTypeDLight)
      {
         GVertexBufferHandle<GVertexPCNTT> vPCNTT(GRAPHIC, mCurVertex, GBufferTypeVolatile);
         PROFILE_START(Terrain_bufferCopy_lockVPCNTT);
         vPCNTT.lock();
         PROFILE_END();

         dMemcpy(&vPCNTT[0], &mVertexStorePCNTT[0], sizeof(GVertexPCNTT) * mCurVertex);

         PROFILE_START(Terrain_bufferCopy_unlockVPCNTT);
         vPCNTT.unlock();
         PROFILE_END();
         GRAPHIC->setVertexBuffer(vPCNTT);
      }
      else
      {
         GVertexBufferHandle<GVertexPCNT> vPCNT(GRAPHIC, mCurVertex, GBufferTypeVolatile);
         PROFILE_START(Terrain_bufferCopy_lockVPCNT);
         vPCNT.lock();
         PROFILE_END();

         dMemcpy(&vPCNT[0], &mVertexStorePCNT[0], sizeof(GVertexPCNT) * mCurVertex);

         PROFILE_START(Terrain_bufferCopy_unlockVPCNT);
         vPCNT.unlock();
         PROFILE_END();
         GRAPHIC->setVertexBuffer(vPCNT);
      }
   }

   void end(bool final, vertexType vtype, Point4F *clipmapMapping, const MatrixF &blockTransform, const Point3F &cameraPosition, LightInfo *light, SceneState * state)
   {
      PROFILE_START(Terrain_batchEnd);

      // Advance the cache - we can't keep old verts because of texture co-ordinates.
      mCurXF++;

      // If we have nothing to draw, quick-out
/*      if( (mCurVertex== 0 || (mCurIndex-mLastIndex) == 0) && !final)
         return;
*/
      AssertFatal(mLastVertex < U16_MAX && mCurVertex < U16_MAX, "TerrBatch::end - too many verts!");
      if(!final)
      {
         // If we're not up to the threshold don't draw anything, just store the primitive...

         // Store the primitive
         // GTriangleList, 0, mCurVertex, 0, mCurIndex / 3
         GPrimitive &tempPrim = mPrimitiveStore[mCurPrim++];
         tempPrim.type          = GTriangleList;
         tempPrim.minIndex      = 0;
         tempPrim.numVertices   = mCurVertex;
         tempPrim.startIndex    = mLastIndex;
         tempPrim.numPrimitives = (mCurIndex-mLastIndex) / 3;

         if(mCurIndex-mLastIndex > mBiggestIndexBatch)
            mBiggestIndexBatch = mCurIndex-mLastIndex;

         if(mCurVertex-mLastVertex > mBiggestVertexBatch)
            mBiggestVertexBatch = mCurVertex-mLastVertex;

         // Update our last-state
         mLastIndex  = mCurIndex;
         mLastVertex = mCurVertex;

         // NOTE: These thresholds were set by careful readthroughs of the 
         //       terrRender code.
         //       If you change the terrain code these need to be updated.
         //
         // Some reference values...
         //    - Most indices  commander chunk can generate - 768
         //    - Most vertices commander chunk can generate - 128
         //    - Most indices  normal    chunk can generate - 96
         //    - Most vertices normal    chunk can generate - 25
         
         // Default, normal chunks...
         U32 idxThresh  = 96;
         U32 vertThresh = 25;

         if(TerrainRender::mRenderingCommander)
         {
            // Special case for commander map
            idxThresh  = 768;
            vertThresh = 128;
         }

         if(mCurVertex < mVertexStoreSize-vertThresh && mCurIndex < (mIndexStoreSize-idxThresh))
         {
            // And bail if we don't HAVE to draw now...
            PROFILE_END();
            return;
         }
      }

      // Blast everything out to buffers and draw it.

      // If there's nothing to draw... draw nothing
      if(mCurVertex == 0 || mCurIndex  == 0)
      {
         PROFILE_END();
         return;
      }

      terrTexGen(vtype, clipmapMapping, blockTransform, cameraPosition, light, state);

      PROFILE_START(Terrain_bufferCopy);

      vertexBufferCopy(vtype);

      // Do primitives/indices
      U16          *idxBuff;
      GPrimitive *primBuff;
      GPrimitiveBufferHandle              p(GRAPHIC, mCurIndex, 1, GBufferTypeVolatile);

      PROFILE_START(Terrain_bufferCopy_lockP);
      p.lock(&idxBuff, &primBuff);
      PROFILE_END();

      dMemcpy(idxBuff, &mIndexStore[0], sizeof(U16) * mCurIndex);

      PROFILE_START(Terrain_bufferCopy_unlockP);
      p.unlock();
      PROFILE_END();

      PROFILE_END();
      
      PROFILE_START(TRender_DIP);

      // Now... RENDER!!!      
      GRAPHIC->setPrimitiveBuffer(p);

      U32 dipCall=0;

      U32 curPrim = 0;

      do
      {
         // This is all triangle list geometry, so we can batch adjacent stuff. Yay!
         // We do a fixup when we submit so all indices are in "global" number-space.

         // Figure out what span we can do
         U32 startIndex = mPrimitiveStore[curPrim].startIndex;
         U32 numPrims   = mPrimitiveStore[curPrim].numPrimitives;
         U32 minIndex   = mPrimitiveStore[curPrim].minIndex;
         U32 numVerts   = mPrimitiveStore[curPrim].numVertices;

         // Advance right off.
         curPrim++;

         while(curPrim < mCurPrim)
         {
            // Update our range. All of this is consecutive in the IB...
            AssertFatal(startIndex + numPrims * 3 == mPrimitiveStore[curPrim].startIndex,
               "TerrBatch::end - non-contiguous IB ranges!");

            // Increase our prim count.
            numPrims += mPrimitiveStore[curPrim].numPrimitives;

            // See if we need to update our index range.
            minIndex = getMin(mPrimitiveStore[curPrim].minIndex, minIndex);

            if(mPrimitiveStore[curPrim].minIndex + mPrimitiveStore[curPrim].numVertices > minIndex + numVerts)
               numVerts = mPrimitiveStore[curPrim].minIndex + mPrimitiveStore[curPrim].numVertices - minIndex;

            curPrim++;
         }

         // Now issue an aggregate draw.
         if(numPrims)
            GRAPHIC->drawIndexedPrimitive(GTriangleList, minIndex, numVerts, startIndex, numPrims);

         dipCall++;
      }
      while(curPrim < mCurPrim);

      //Con::printf("---------- terrain draw (%d prim, %d dips)", mCurPrim, dipCall);

      // Clear all our buffers
      mCurIndex   = 0;
      mLastIndex  = 0;
      mCurVertex  = 0;
      mLastVertex = 0;
      mCurPrim    = 0;
      mCurXF++;

      PROFILE_END();
      // All done!

      PROFILE_END();
   }
};


