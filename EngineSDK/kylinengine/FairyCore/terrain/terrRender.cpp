//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "console/consoleTypes.h"
#include "core/frameAllocator.h"
#include "platform/profiler.h"
#include "math/mMath.h"
#include "sceneManager/sceneState.h"
#include "sceneManager/sceneManager.h"
#include "sceneManager/sgUtil.h"
#include "graphic/gDevice.h"
#include "graphic/bitmap/gBitmap.h"
#include "graphic/gCardProfile.h"
#include "materials/matInstance.h"
#include "materials/materialManager.h"
#include "terrain/terrData.h"
#include "terrain/sky.h"
#include "terrain/terrRender.h"
#include "terrain/terrBatch.h"
#include "terrain/waterBlock.h"
#include "core/util/safeDelete.h"
#include "renderManager/renderPassManager.h"

MatrixF     TerrainRender::mCameraToObject;
SceneState* TerrainRender::mSceneState;

S32 TerrainRender::mSquareSize;
F32 TerrainRender::mScreenSize;
F32 TerrainRender::mPixelError;
F32 TerrainRender::mScreenError;
F32 TerrainRender::mMinSquareSize;
F32 TerrainRender::mFarDistance;

TerrainBlock* TerrainRender::mCurrentBlock = NULL;
Point2F       TerrainRender::mBlockPos;
Point2I       TerrainRender::mBlockOffset;
Point2I       TerrainRender::mTerrainOffset;
PlaneF        TerrainRender::mClipPlane[MaxClipPlanes];
U32           TerrainRender::mNumClipPlanes = 4;
Point3F       TerrainRender::mCamPos;

TerrainLightingPlugin* TerrainRender::mTerrainLighting = NULL;
U32           TerrainRender::mDynamicLightCount;
bool          TerrainRender::mEnableTerrainDynLights     = true;
TerrLightInfo TerrainRender::mTerrainLights[MaxTerrainLights];

bool TerrainRender::mRenderingCommander = false;

bool TerrainRender::mMaterialsInit = false;
BaseMatInstance* TerrainRender::mOverrideMat = NULL;
BaseMatInstance* TerrainRender::mDetailMat = NULL;
BaseMatInstance* TerrainRender::mFogMat = NULL;
bool TerrainRender::mFixedFunction = false;

MaterialParameterHandle* TerrainRender::mBrightnessScaleSC;
MaterialParameterHandle* TerrainRender::mDetDataSC;
MaterialParameterHandle* TerrainRender::mMorphTSC;

void TerrainRender::init()
{
   mScreenError   = 4.0f;
   mScreenSize    = 45.0f;
   mMinSquareSize = 4.0f;
   mFarDistance   = 500.0f;

   Con::addVariable("screenSize", TypeF32, &mScreenSize);
   Con::addVariable("farDistance", TypeF32, &mFarDistance);

   Con::addVariable("inCommanderMap", TypeBool, &mRenderingCommander);

   Con::addVariable("pref::Terrain::dynamicLights", TypeBool, &mEnableTerrainDynLights);
   Con::addVariable("pref::Terrain::screenError", TypeF32, &mScreenError);

   TerrBatch::init();
}

void TerrainRender::shutdown()
{   
   SAFE_DELETE(mDetailMat);
   SAFE_DELETE(mFogMat);
   mMaterialsInit = false;
   TerrBatch::destroy();
}

void TerrainRender::initMaterials(SceneGraphData& sgData)
{
   if (!mMaterialsInit)
   {
      mFixedFunction = GRAPHIC->getPixelShaderVersion() == 0.0f;

      GAtlasVert2* vert = NULL;

      // Grab the material for the detail pass
      if( !mFixedFunction )
      {
         mDetailMat = MaterialManager::get()->createMatInstance("TerrainDetailMaterial", (GVertexFlags)getGVertFlags(vert));
      }
      else
      {
         if( GRAPHIC->getCardProfiler()->queryProfile( "lerpDetailBlend" ) )
            mDetailMat = MaterialManager::get()->createMatInstance("TerrainDetailFFLerpOpMaterial", (GVertexFlags)getGVertFlags(vert));
         else if( GRAPHIC->getCardProfiler()->queryProfile( "fourStageDetailBlend" ) )
            mDetailMat = MaterialManager::get()->createMatInstance("TerrainDetailFF4StageMaterial", (GVertexFlags)getGVertFlags(vert));
      }

      if (!mDetailMat)
      {
         Con::errorf("Unable to find supported terrain detail material!");    
      }
      else
      {
         mBrightnessScaleSC = mDetailMat->getMaterialParameterHandle(ClipMap::smBrightnessScale);
         mDetDataSC = mDetailMat->getMaterialParameterHandle(ClipMap::smDetData);         
      }

      mFogMat = MaterialManager::get()->createMatInstance("TerrainFogMaterial", (GVertexFlags)getGVertFlags(vert));
      if (!mFogMat)
      {
         Con::errorf("Unable to find TerrainFogMaterial!");         
      }
      else
      {
         mMorphTSC = mFogMat->getMaterialParameterHandle(ClipMap::smMorphT);
      }
      mMaterialsInit = true;
   }   
}

void TerrainRender::buildClippingPlanes(bool flipClipPlanes)
{
   PROFILE_START(TerrainRender_buildClippingPlanes);

   F32 frustumParam[6];
   F64 realfrustumParam[6];
   bool isOrtho;
   
   GRAPHIC->getFrustum(&frustumParam[0], &frustumParam[1],
      &frustumParam[2], &frustumParam[3],
      &frustumParam[4], &frustumParam[5], &isOrtho);


   // Cast se can put them in the sg call.
   for(U32 i=0; i<6; i++)
      realfrustumParam[i] = frustumParam[i];

   Point3F osCamPoint = Point3F::Zero;
   mCameraToObject.mulP(osCamPoint);
   sgComputeOSFrustumPlanes(realfrustumParam,
      mCameraToObject,
      osCamPoint,
      mClipPlane[4],
      mClipPlane[0],
      mClipPlane[1],
      mClipPlane[2],
      mClipPlane[3]);

   // no need
   mNumClipPlanes = 4;

   // near plane is needed as well...
   //PlaneF p(0, 1, 0, -frustumParam[4]);
   //mTransformPlane(mCameraToObject, Point3F(1,1,1), p, &mClipPlane[0]);

   if (flipClipPlanes) {
      mClipPlane[0].neg();
      mClipPlane[1].neg();
      mClipPlane[2].neg();
      mClipPlane[3].neg();
      mClipPlane[4].neg();
      mClipPlane[5].neg();
   }

   PROFILE_END();
}

S32 TerrainRender::testSquareVisibility(const Point3F &minPt, const Point3F &maxPt, S32 mask, F32 expand)
{
   PROFILE_SCOPE(TerrainRender_testSquareVisibility);
   S32 retMask = 0;
   Point3F minPoint;
   Point3F maxPoint;
   for(U32 i = 0; i < mNumClipPlanes; i++)
   {
      if(mask & (1 << i))
      {
         if(mClipPlane[i].x > 0)
         {
            maxPoint.x = maxPt.x;
            minPoint.x = minPt.x;
         }
         else
         {
            maxPoint.x = minPt.x;
            minPoint.x = maxPt.x;
         }
         if(mClipPlane[i].y > 0)
         {
            maxPoint.y = maxPt.y;
            minPoint.y = minPt.y;
         }
         else
         {
            maxPoint.y = minPt.y;
            minPoint.y = maxPt.y;
         }
         if(mClipPlane[i].z > 0)
         {
            maxPoint.z = maxPt.z;
            minPoint.z = minPt.z;
         }
         else
         {
            maxPoint.z = minPt.z;
            minPoint.z = maxPt.z;
         }

         F32 maxDot = mDot(maxPoint, mClipPlane[i]);
         F32 planeD = mClipPlane[i].d;
         if(maxDot <= -(planeD + expand))
         {
            return -1;
         }

         F32 minDot = mDot(minPoint, mClipPlane[i]);
         if(minDot <= -planeD)
            retMask |= (1 << i);
      }
   }

   return retMask;
}

ChunkCornerPoint *TerrainRender::allocInitialPoint(const Point3F &pos)
{
   ChunkCornerPoint *ret = (ChunkCornerPoint *) FrameAllocator::alloc(sizeof(ChunkCornerPoint));
   ret->x = pos.x;
   ret->y = pos.y;
   ret->z = fixedToFloat(mCurrentBlock->getHeight(pos.x/mCurrentBlock->getSquareSize(), pos.y/mCurrentBlock->getSquareSize()));
   ret->xfIndex = 0;
   
   return ret;
}

ChunkCornerPoint *TerrainRender::allocPoint(const Point2I &pos)
{
   ChunkCornerPoint *ret = (ChunkCornerPoint *) FrameAllocator::alloc(sizeof(ChunkCornerPoint));
   ret->x = pos.x * mSquareSize + mBlockPos.x;
   ret->y = pos.y * mSquareSize + mBlockPos.y;
   ret->z = fixedToFloat(mCurrentBlock->getHeight(pos.x, pos.y));
   ret->xfIndex = 0;

   return ret;
}

void TerrainRender::allocRenderEdges(U32 edgeCount, EdgeParent **dest, bool renderEdge)
{
   if(renderEdge)
   {
      for(U32 i = 0; i < edgeCount; i++)
      {
         ChunkEdge *edge = (ChunkEdge *) FrameAllocator::alloc(sizeof(ChunkEdge));
         edge->c1 = NULL;
         edge->c2 = NULL;
         edge->xfIndex = 0;
         edge->pointCount = 0;
         edge->pointIndex = 0;
         dest[i] = edge;
      }
   }
   else
   {
      for(U32 i = 0; i < edgeCount; i++)
      {
         ChunkScanEdge *edge = (ChunkScanEdge *) FrameAllocator::alloc(sizeof(ChunkScanEdge));
         edge->mp = NULL;
         dest[i] = edge;
      }
   }
}

void TerrainRender::subdivideChunkEdge(ChunkScanEdge *e, const Point2I &pos, bool chunkEdge)
{
   if(!e->mp)
   {
      allocRenderEdges(2, &e->e1, chunkEdge);
      e->mp = allocPoint(pos);

      e->e1->p1 = e->p1;
      e->e1->p2 = e->mp;
      e->e2->p1 = e->mp;
      e->e2->p2 = e->p2;
   }
}

F32 TerrainRender::getSquareDistance(const Point3F& minPoint, const Point3F& maxPoint, F32* zDiff)
{
   Point3F vec;

   if(mCamPos.z < minPoint.z)
      vec.z = minPoint.z - mCamPos.z;
   else if(mCamPos.z > maxPoint.z)
      vec.z = maxPoint.z - mCamPos.z;
   else
      vec.z = 0;

   if(mCamPos.x < minPoint.x)
      vec.x = minPoint.x - mCamPos.x;
   else if(mCamPos.x > maxPoint.x)
      vec.x = mCamPos.x - maxPoint.x;
   else
      vec.x = 0;

   if(mCamPos.y < minPoint.y)
      vec.y = minPoint.y - mCamPos.y;
   else if(mCamPos.y > maxPoint.y)
      vec.y = mCamPos.y - maxPoint.y;
   else
      vec.y = 0;

   if (mCamPos.z <= minPoint.z)
   {
      *zDiff = maxPoint.z - mCamPos.z;
   }
   else
   {
      if (mCamPos.z > maxPoint.z)
      {
         *zDiff = minPoint.z - mCamPos.z;
      }
      else
      {
         F32 zCenter = (maxPoint.z + minPoint.z) / 2.0f;
         if (mCamPos.z > zCenter)
            *zDiff = minPoint.z - mCamPos.z;
         else
            *zDiff = maxPoint.z - mCamPos.z;
      }
   }

   return vec.len();
}

struct BatchHelper
{
   typedef Vector<EmitChunk*> ChunkList;
   ChunkList mLightPasses[MaxTerrainLights][16];
   ChunkList mDiffusePasses[256], mFog, mDetail;

   void reset()
   {
      for(S32 i=0; i<MaxTerrainLights; i++)
      {
         for(S32 j=0; j<16; j++)
            mLightPasses[i][j].clear();
      }

      for(S32 i=0; i<256; i++)
         mDiffusePasses[i].clear();

      mFog.clear();
      mDetail.clear();
   }

   BatchHelper()
   {
      reset();
   }

   ~BatchHelper()
   {
   }

   void batchChunk(EmitChunk *ec, const F32 squareDistance, F32 zDelta)
   {
      // Figure out what level range is needed for this chunk, and put it
      // on the right list.

      const F32 blockSize = TerrainRender::mSquareSize * F32(TerrainBlock::BlockSize);

      // What will be the bounds of this chunk?
      RectF texBounds;
      texBounds.point.x = (F32(ec->x) * TerrainRender::mSquareSize) / blockSize;
      texBounds.point.y = (F32(ec->y) * TerrainRender::mSquareSize) / blockSize;
      texBounds.extent.x = (4.0f * TerrainRender::mSquareSize) / blockSize;
      texBounds.extent.y = (4.0f * TerrainRender::mSquareSize) / blockSize;

      // Get the near/far distances for this chunk.
      F32 near = squareDistance;
      F32 far = squareDistance + (4.0f * TerrainRender::mSquareSize);

      S32 startLevel, endLevel;
      TerrainRender::mCurrentBlock->mClipMap->calculateClipMapLevels
                              (near, far, texBounds, startLevel, endLevel);

      // Great - now encode levels and store into appropriate bin.
      U8 binCode = ((endLevel & 0xF)<<4) | (startLevel & 0xF);
      mDiffusePasses[binCode].push_back(ec);

      // Check if this chunk will get fogged - consider furthest point, and if
      // it'll be fogged then draw a fog pass.
      if(TerrainRender::mSceneState->getHazeAndFog(far, zDelta) > (1.f/256.f) 
         || TerrainRender::mSceneState->getHazeAndFog(far, -zDelta) > (1.f/256.f))
         mFog.push_back(ec);

      // Also, check for detail.
      if(near < TerrainRender::mCurrentBlock->mDetailDistance)
         mDetail.push_back(ec);

      // Add this to the right light lists if it has lighting.
      if(ec->lightMask)
      {
         for(S32 i=0; i<MaxTerrainLights; i++)
         {
            if(ec->lightMask & BIT(i))
               mLightPasses[i][endLevel & 15].push_back(ec);
         }
      }
   }

   void renderChunkList(SceneState * state, ChunkList &cl, BaseMatInstance *m, SceneGraphData &sgData, TerrBatch::vertexType vtype, Point4F *clipmapMapping, const MatrixF &blockTransform, const Point3F &cameraPosition, LightInfo *light)
   {
      AssertFatal(m, "NULL material!");

      while(m->setupPass(state, sgData))
      {         
         for(S32 i=0; i<cl.size(); i++)
         {
            EmitChunk *sq = cl[i];
            TerrBatch::begin();

            // Emit the appropriate geometry for our rendering mode...
            if(TerrainRender::mRenderingCommander)
               TerrainRender::renderChunkCommander(sq);
            else
               TerrainRender::renderChunkNormal(sq);
            
            TerrBatch::end(false, vtype, clipmapMapping, blockTransform, cameraPosition, light, state);
         }

         // force batch to flush
         TerrBatch::end(true, vtype, clipmapMapping, blockTransform, cameraPosition, light, state);
      }
   }
} gChunkBatcher;

void TerrainRender::emitTerrChunk(SquareStackNode *n, F32 squareDistance, U32 lightMask, F32 zDelta, F32 squareDistanceLOD)
{
   GridChunk *gc = mCurrentBlock->findChunk(n->pos);

   EmitChunk *chunk = (EmitChunk *) FrameAllocator::alloc(sizeof(EmitChunk));
   chunk->x = n->pos.x + mBlockOffset.x + mTerrainOffset.x;
   chunk->y = n->pos.y + mBlockOffset.y + mTerrainOffset.y;
   chunk->lightMask = lightMask;

   if (!mOverrideMat)
   {
      gChunkBatcher.batchChunk(chunk, squareDistance, zDelta);
   } else {
      gChunkBatcher.mDiffusePasses[0].push_back(chunk);
   }

   if(mRenderingCommander)
      return;

   chunk->edge[0] = (ChunkEdge *) n->top;
   chunk->edge[1] = (ChunkEdge *) n->right;
   chunk->edge[2] = (ChunkEdge *) n->bottom;
   chunk->edge[3] = (ChunkEdge *) n->left;

   chunk->edge[0]->c2 = chunk;
   chunk->edge[1]->c1 = chunk;
   chunk->edge[2]->c1 = chunk;
   chunk->edge[3]->c2 = chunk;

   // holes only in the primary terrain block
   if (gc->emptyFlags && mBlockPos.x == 0 && mBlockPos.y == 0)
      chunk->emptyFlags = gc->emptyFlags;
   else
      chunk->emptyFlags = 0;

   S32 subDivLevel;
   F32 growFactor = 0.0f;

   F32 minSubdivideDistance = 1000000.0f;

   if(squareDistance < 1.0f || squareDistanceLOD < 1.0f)
      subDivLevel = -1;
   else
   {
      for(subDivLevel = 2; subDivLevel >= 0; subDivLevel--)
      {
         F32 subdivideDistance = fixedToFloat(gc->heightDeviance[subDivLevel]) / mPixelError;
         if(subdivideDistance > minSubdivideDistance)
            subdivideDistance = minSubdivideDistance;

         if(squareDistance >= subdivideDistance)
            break;

         F32 clampDistance = subdivideDistance * 0.75f;

         if(squareDistance > clampDistance)
         {
            growFactor = (squareDistance - clampDistance) / (0.25f * subdivideDistance);
            subDivLevel--;
            break;
         }

         minSubdivideDistance = clampDistance;
      }
   }
   chunk->subDivLevel = subDivLevel;
   chunk->growFactor = growFactor;
}

void TerrainRender::processBlock(SceneState*, EdgeParent *topEdge, EdgeParent *rightEdge, EdgeParent *bottomEdge, EdgeParent *leftEdge)
{
   SquareStackNode stack[TerrainBlock::BlockShift*4];
   Point3F minPoint, maxPoint;

   // Set up the root node of the stack...
   stack[0].level        = TerrainBlock::BlockShift;
   stack[0].clipFlags    = (BIT(mNumClipPlanes) - 1) | FarSphereMask;  // test all the planes
   stack[0].pos.set(0,0);
   stack[0].top          = topEdge;
   stack[0].right        = rightEdge;
   stack[0].bottom       = bottomEdge;
   stack[0].left         = leftEdge;
   stack[0].lightMask    = BIT(mDynamicLightCount) - 1; // test all the lights

   // Set up fog...
   Vector<SceneState::FogBand> *posFog = mSceneState->getPosFogBands();
   Vector<SceneState::FogBand> *negFog = mSceneState->getNegFogBands();
   bool clipAbove = posFog->size() > 0 && (*posFog)[0].isFog == false;
   bool clipBelow = negFog->size() > 0 && (*negFog)[0].isFog == false;
   bool clipOn    = posFog->size() > 0 && (*posFog)[0].isFog == true;

   if(posFog->size() != 0 || negFog->size() != 0)
      stack[0].clipFlags |= FogPlaneBoxMask;

   // Initialize the stack
   S32 curStackSize = 1;
   F32 squareDistance;

   while(curStackSize)
   {
      SquareStackNode *n = stack + curStackSize - 1;

      // see if it's visible
      GridSquare *sq = mCurrentBlock->findSquare(n->level, n->pos);

      // Calculate bounding points...
      minPoint.set(mSquareSize * n->pos.x + mBlockPos.x,
                   mSquareSize * n->pos.y + mBlockPos.y,
                   fixedToFloat(sq->minHeight));
      maxPoint.set(minPoint.x + (mSquareSize << n->level),
                   minPoint.y + (mSquareSize << n->level),
                   fixedToFloat(sq->maxHeight));

      // holes only in the primary terrain block
      if ((sq->flags & GridSquare::Empty) && mBlockPos.x == 0.0f && mBlockPos.y == 0.0f)
      {
         curStackSize--;
         continue;
      }

      F32 zDiff;
      squareDistance = getSquareDistance(minPoint, maxPoint, &zDiff);

      S32 nextClipFlags = 0;

      if(n->clipFlags)
      {
         if(n->clipFlags & FogPlaneBoxMask)
         {
            F32 camZ = mCamPos.z;
            bool boxBelow = camZ > maxPoint.z;
            bool boxAbove = camZ < minPoint.z;
            bool boxOn = !(boxAbove || boxBelow);
            if( clipOn ||
               (clipAbove && boxAbove && (maxPoint.z - camZ > (*posFog)[0].cap)) ||
               (clipBelow && boxBelow && (camZ - minPoint.z > (*negFog)[0].cap)) ||
               (boxOn && (( clipAbove && maxPoint.z - camZ > (*posFog)[0].cap ) ||
               ( clipBelow && camZ - minPoint.z > (*negFog)[0].cap ))))
            {
               // Using the fxSunLight can cause the "sky" to extend down below the camera.
               // To avoid the sun showing through, the terrain must always be rendered.
               // If the fxSunLight is not being used, the following optimization can be
               // uncommented.
#if 0
               if(boxBelow && !mSceneState->isBoxFogVisible(squareDistance, maxPoint.z, minPoint.z))
               {
                  // Totally fogged terrain tiles can be thrown out as long as they are
                  // below the camera. If they are above, the sky will show through the
                  // fog.
                  curStackSize--;
                  continue;
               }
#endif
               nextClipFlags |= FogPlaneBoxMask;
            }
         }

         if(n->clipFlags & FarSphereMask)
         {
            // Reject stuff outside our visible range...
            if(squareDistance >= mFarDistance)
            {
               curStackSize--;
               continue;
            }

            // Otherwise clip against the bounding sphere...
            S32 dblSquareSz = mSquareSize << (n->level+1);

            // We add a "fudge factor" to get the furthest possible point of the square...
            if(squareDistance + (maxPoint.z - minPoint.z) + dblSquareSz > mFarDistance)
               nextClipFlags |= FarSphereMask;
         }

         
         // zDelta for screen error height deviance.
         F32 zDelta = squareDistance * mPixelError;
         minPoint.z -= zDelta;
         maxPoint.z += zDelta;

         nextClipFlags |= testSquareVisibility(minPoint, maxPoint, n->clipFlags, F32(mSquareSize));
         if(nextClipFlags == -1)
         {
            // trivially rejected, so pop it off the stack
            curStackSize--;
            continue;
         }
      }
      // If we have been marked as lit we need to see if that still holds true...
      if(n->lightMask)
         n->lightMask = testSquareLights(sq, n->level, n->pos, n->lightMask);

      if(n->level == 2)
      {
         // We've tessellated far enough, stop here.

         F32 squareDistanceLOD = squareDistance;

         if (!mCurrentBlock->isTiling())
         {
            // Lock border LOD so no gaps
            if (n->pos.x == 0 || n->pos.x >= (TerrainBlock::BlockSize-4))
               squareDistanceLOD = 0.5f;
       
            if (n->pos.y == 0 || n->pos.y >= (TerrainBlock::BlockSize-4))
               squareDistanceLOD = 0.5f;         
         }

         emitTerrChunk(n, squareDistance, n->lightMask, zDiff, squareDistanceLOD);
         curStackSize--;
         continue;
      }

      // Ok, we have to tessellate further down...
      bool allocChunkEdges = (n->level == 3);

      Point2I pos = n->pos;

      ChunkScanEdge *top    = (ChunkScanEdge *) n->top;
      ChunkScanEdge *right  = (ChunkScanEdge *) n->right;
      ChunkScanEdge *bottom = (ChunkScanEdge *) n->bottom;
      ChunkScanEdge *left   = (ChunkScanEdge *) n->left;

      // subdivide this square and throw it on the stack
      S32 squareOneSize  = 1 << n->level;
      S32 squareHalfSize = squareOneSize >> 1;

      ChunkCornerPoint *midPoint = allocPoint(Point2I(pos.x + squareHalfSize, pos.y + squareHalfSize));
      S32 nextLevel = n->level - 1;

      subdivideChunkEdge(top,    Point2I(pos.x + squareHalfSize, pos.y + squareOneSize),  allocChunkEdges);
      subdivideChunkEdge(right,  Point2I(pos.x + squareOneSize,  pos.y + squareHalfSize), allocChunkEdges);
      subdivideChunkEdge(bottom, Point2I(pos.x + squareHalfSize, pos.y),                  allocChunkEdges);
      subdivideChunkEdge(left,   Point2I(pos.x,                  pos.y + squareHalfSize), allocChunkEdges);

      // cross edges go top, right, bottom, left
      EdgeParent *crossEdges[4];
      allocRenderEdges(4, crossEdges, allocChunkEdges);
      crossEdges[0]->p1 = top->mp;
      crossEdges[0]->p2 = midPoint;
      crossEdges[1]->p1 = midPoint;
      crossEdges[1]->p2 = right->mp;
      crossEdges[2]->p1 = midPoint;
      crossEdges[2]->p2 = bottom->mp;
      crossEdges[3]->p1 = left->mp;
      crossEdges[3]->p2 = midPoint;

      n->level     = nextLevel;
      n->clipFlags = nextClipFlags;

      // Set up data for our sub-squares
      for(S32 i = 1; i < 4; i++)
      {
         n[i].level        = nextLevel;
         n[i].clipFlags    = nextClipFlags;
         n[i].lightMask    = n->lightMask;
      }

      // push in reverse order of processing.
      n[3].pos = pos;
      n[3].top = crossEdges[3];
      n[3].right = crossEdges[2];
      n[3].bottom = bottom->e1;
      n[3].left = left->e2;

      n[2].pos.set(pos.x + squareHalfSize, pos.y);
      n[2].top = crossEdges[1];
      n[2].right = right->e2;
      n[2].bottom = bottom->e2;
      n[2].left = crossEdges[2];

      n[1].pos.set(pos.x, pos.y + squareHalfSize);
      n[1].top = top->e1;
      n[1].right = crossEdges[0];
      n[1].bottom = crossEdges[3];
      n[1].left = left->e1;

      n[0].pos.set(pos.x + squareHalfSize, pos.y + squareHalfSize);
      n[0].top = top->e2;
      n[0].right = right->e1;
      n[0].bottom = crossEdges[1];
      n[0].left = crossEdges[0];

      curStackSize += 3;
   }
}

//---------------------------------------------------------------
// Root block render function
//---------------------------------------------------------------
void TerrainRender::fixEdge(ChunkEdge *edge, S32 x, S32 y, S32 dx, S32 dy)
{
   PROFILE_SCOPE(TR_fixEdge);

   S32 minLevel;

   if(edge->c1)
   {
      minLevel = edge->c1->subDivLevel;

      if(edge->c2 && (edge->c2->subDivLevel < minLevel))
         minLevel = edge->c2->subDivLevel;
   }
   else
   {
      minLevel = edge->c2->subDivLevel;
   }

   if(minLevel == 2)
   {
      edge->pointCount = 0;
      return;
   }

   S32 maxLevel = minLevel;
   F32 growFactor;

   if(edge->c1)
   {
      maxLevel = edge->c1->subDivLevel;
      growFactor = edge->c1->growFactor;

      if(edge->c2 && (edge->c2->subDivLevel >= minLevel))
      {
         if(edge->c2->subDivLevel > maxLevel)
         {
            maxLevel = edge->c2->subDivLevel;
            growFactor = edge->c2->growFactor;
         }
         else if(edge->c2->growFactor > growFactor)
         {
            growFactor = edge->c2->growFactor;
         }
      }
   }
   else
   {
      growFactor = edge->c2->growFactor;
   }

   // get the mid heights
   EdgePoint *pmid = &edge->pt[1];
   ChunkCornerPoint *p1 = edge->p1;
   ChunkCornerPoint *p2 = edge->p2;

   pmid->x = (p1->x + p2->x) * 0.5f;
   pmid->y = (p1->y + p2->y) * 0.5f;

   if(maxLevel == 2)
   {
      // pure interp
      pmid->z = (p1->z + p2->z) * 0.5f;

      if(minLevel >= 0)
      {
         edge->pointCount = 1;
         return;
      }
   }
   else
   {
      pmid->z = fixedToFloat(mCurrentBlock->getHeight(x + dx + dx, y + dy + dy));
      if(maxLevel == 1)
         pmid->z = pmid->z + growFactor * (((p1->z + p2->z) * 0.5f) - pmid->z);

      if(minLevel >= 0)
      {
         edge->pointCount = 1;
         return;
      }
   }

   // last case - minLevel == -1, midPoint calc'd
   edge->pointCount = 3;
   EdgePoint *pm1 = &edge->pt[0];
   EdgePoint *pm2 = &edge->pt[2];

   pm1->x = (p1->x + pmid->x) * 0.5f;
   pm1->y = (p1->y + pmid->y) * 0.5f;
   pm2->x = (p2->x + pmid->x) * 0.5f;
   pm2->y = (p2->y + pmid->y) * 0.5f;

   if(maxLevel != -1)
   {
      pm1->z = (p1->z + pmid->z) * 0.5f;
      pm2->z = (p2->z + pmid->z) * 0.5f;
      return;
   }

   // compute the real deals:
   pm1->z = fixedToFloat(mCurrentBlock->getHeight(x + dx, y + dy));
   pm2->z = fixedToFloat(mCurrentBlock->getHeight(x + dx + dx + dx, y + dy + dy + dy));

   if(growFactor)
   {
      pm1->z = pm1->z + growFactor * (((p1->z + pmid->z) * 0.5f) - pm1->z);
      pm2->z = pm2->z + growFactor * (((p2->z + pmid->z) * 0.5f) - pm2->z);
   }
}

void TerrainRender::renderChunkCommander(EmitChunk *chunk)
{
   PROFILE_START(TerrainRender_renderChunkCommander);

   // Emit our points
   U32 ll = TerrBatch::mCurVertex;

   for(U32 y = 0; y <= 64; y += 4)                   // 16 steps
   {
      for(U32 x = (y & 4) ? 4 : 0; x <= 64; x += 8)  // 8 steps
         TerrBatch::emit(chunk->x + x,chunk->y + y); // 128 vertices
   }

   // Emit a mesh overlaying 'em (768 indices)
   for(U32 y = 0; y < 8; y++)
   {
      for(U32 x = 0; x < 8; x++)
      {
         TerrBatch::emitTriangle(ll + 9, ll,      ll + 17);
         TerrBatch::emitTriangle(ll + 9, ll + 17, ll + 18);
         TerrBatch::emitTriangle(ll + 9, ll + 18, ll + 1);
         TerrBatch::emitTriangle(ll + 9, ll + 1,  ll);
         ll++;
      }
      ll += 9;
   }

   PROFILE_END();
}

void TerrainRender::renderChunkNormal(EmitChunk *chunk)
{
   PROFILE_START(TerrainRender_renderChunkNormal);

   // Emits a max of 96 indices
   // Emits a max of 25 vertices
   ChunkEdge *e0 = chunk->edge[0];
   ChunkEdge *e1 = chunk->edge[1];
   ChunkEdge *e2 = chunk->edge[2];
   ChunkEdge *e3 = chunk->edge[3];

   if(e0->xfIndex != TerrBatch::mCurXF)
   {
      if(!e0->xfIndex)
         fixEdge(e0, chunk->x, chunk->y + 4, 1, 0);
      TerrBatch::emit( *e0 );
   }

   if(e1->xfIndex != TerrBatch::mCurXF)
   {
      if(!e1->xfIndex)
         fixEdge(e1, chunk->x + 4, chunk->y + 4, 0, -1);
      TerrBatch::emit( *e1 );
   }

   if(e2->xfIndex != TerrBatch::mCurXF)
   {
      if(!e2->xfIndex)
         fixEdge(e2, chunk->x, chunk->y, 1, 0);
      TerrBatch::emit( *e2 );
   }

   if(e3->xfIndex != TerrBatch::mCurXF)
   {
      if(!e3->xfIndex)
         fixEdge(e3, chunk->x, chunk->y + 4, 0, -1);
      TerrBatch::emit( *e3 );
   }

   // Edges above, give us 12 verts
   // Corners below are 4 more
   U16 p0 = TerrBatch::emit(e0->p1);
   U16 p1 = TerrBatch::emit(e0->p2);
   U16 p2 = TerrBatch::emit(e2->p2);
   U16 p3 = TerrBatch::emit(e2->p1);
 
   // build the interior points (one more vert):
   U16 ip0 = TerrBatch::emit(chunk->x + 2, chunk->y + 2);
   F32 growFactor = chunk->growFactor;

   // So now we have a total of 17 verts
   if(chunk->subDivLevel >= 1)
   {
      // just emit the fan for the whole square:
      S32 i;

      if(e0->pointCount)
      {
         TerrBatch::emitTriangle(ip0, p0, e0->pointIndex);  // 3 indices
         for(i = 1; i < e0->pointCount; i++)
            TerrBatch::emitFanStep(e0->pointIndex + i);     // 9 i
         TerrBatch::emitFanStep(p1);                        // 3 i
      }
      else 
      {
         TerrBatch::emitTriangle(ip0, p0, p1);
      }

      for(i = 0; i < e1->pointCount; i++)
         TerrBatch::emitFanStep(e1->pointIndex + i);        // 9 indices

      TerrBatch::emitFanStep(p2);                           // 3 indices
      for(i = e2->pointCount - 1; i >= 0; i--)
         TerrBatch::emitFanStep(e2->pointIndex + i);        // 6 i

      TerrBatch::emitFanStep(p3);                           // 3 i
      for(i = e3->pointCount - 1; i >= 0; i--)
         TerrBatch::emitFanStep(e3->pointIndex + i);        // 6 i

      TerrBatch::emitFanStep(p0);                           // 3 i

      // Total indices for this path:                          42 indices
   }
   else
   {
      if(chunk->subDivLevel == 0)
      {
         // Add yet four verts more to the list! (now we're at 21)
         U32 ip1 = TerrBatch::emitInterp(p0, ip0, chunk->x + 1, chunk->y + 3, growFactor);
         U32 ip2 = TerrBatch::emitInterp(p1, ip0, chunk->x + 3, chunk->y + 3, growFactor);
         U32 ip3 = TerrBatch::emitInterp(p2, ip0, chunk->x + 3, chunk->y + 1, growFactor);
         U32 ip4 = TerrBatch::emitInterp(p3, ip0, chunk->x + 1, chunk->y + 1, growFactor);

         // emit the 4 fans:
         if((chunk->emptyFlags & CornerEmpty_0_1) != CornerEmpty_0_1)
         {
            TerrBatch::emitTriangle(ip1, p0, e0->pointIndex);        // 3

            if(e0->pointCount == 3)
               TerrBatch::emitFanStep (e0->pointIndex + 1);          // 3

            TerrBatch::emitFanStep (ip0);                            // 3

            if(e3->pointCount == 1)
               TerrBatch::emitFanStep ( e3->pointIndex );            
            else
            {
               TerrBatch::emitFanStep ( e3->pointIndex + 1 );        // 3
               TerrBatch::emitFanStep ( e3->pointIndex );            // 3
            }

            TerrBatch::emitFanStep ( p0 );                           // 3

            // Total emitted indices                                    18
         }

         if((chunk->emptyFlags & CornerEmpty_1_1) != CornerEmpty_1_1)
         {
            TerrBatch::emitTriangle( ip2, p1, e1->pointIndex );

            if(e1->pointCount == 3)
               TerrBatch::emitFanStep ( e1->pointIndex + 1 );

            TerrBatch::emitFanStep ( ip0 );

            if(e0->pointCount == 1)
               TerrBatch::emitFanStep( e0->pointIndex );
            else
            {
               TerrBatch::emitFanStep( e0->pointIndex + 1 );
               TerrBatch::emitFanStep( e0->pointIndex + 2 );
            }
            TerrBatch::emitFanStep( p1 );
         }

         if((chunk->emptyFlags & CornerEmpty_1_0) != CornerEmpty_1_0)
         {

            if(e2->pointCount == 1)
               TerrBatch::emitTriangle( ip3, p2, e2->pointIndex );
            else
            {
               TerrBatch::emitTriangle( ip3, p2, e2->pointIndex + 2 );
               TerrBatch::emitFanStep( e2->pointIndex + 1 );
            }

            TerrBatch::emitFanStep( ip0 );

            if(e1->pointCount == 1)
               TerrBatch::emitFanStep( e1->pointIndex );
            else
            {
               TerrBatch::emitFanStep( e1->pointIndex + 1 );
               TerrBatch::emitFanStep( e1->pointIndex + 2 );
            }
            TerrBatch::emitFanStep( p2 );
         }

         if((chunk->emptyFlags & CornerEmpty_0_0) != CornerEmpty_0_0)
         {

            if(e3->pointCount == 1)
               TerrBatch::emitTriangle( ip4, p3, e3->pointIndex);
            else
            {
               TerrBatch::emitTriangle( ip4, p3, e3->pointIndex + 2);
               TerrBatch::emitFanStep( e3->pointIndex + 1 );
            }
            TerrBatch::emitFanStep( ip0 );

            if(e2->pointCount == 1)
               TerrBatch::emitFanStep(  e2->pointIndex );
            else
            {
               TerrBatch::emitFanStep(  e2->pointIndex + 1 );
               TerrBatch::emitFanStep(  e2->pointIndex );
            }
            TerrBatch::emitFanStep( p3 );
         }

         // Four fans of 18 indices =                                  72 indices
      }
      else
      {
         // subDiv == -1
         U32 ip1 = TerrBatch::emit(chunk->x + 1, chunk->y + 3);
         U32 ip2 = TerrBatch::emit(chunk->x + 3, chunk->y + 3);
         U32 ip3 = TerrBatch::emit(chunk->x + 3, chunk->y + 1);
         U32 ip4 = TerrBatch::emit(chunk->x + 1, chunk->y + 1);
         U32 ip5 = TerrBatch::emitInterp(e0->pointIndex + 1, ip0, chunk->x + 2, chunk->y + 3, growFactor);
         U32 ip6 = TerrBatch::emitInterp(e1->pointIndex + 1, ip0, chunk->x + 3, chunk->y + 2, growFactor);
         U32 ip7 = TerrBatch::emitInterp(e2->pointIndex + 1, ip0, chunk->x + 2, chunk->y + 1, growFactor);
         U32 ip8 = TerrBatch::emitInterp(e3->pointIndex + 1, ip0, chunk->x + 1, chunk->y + 2, growFactor);

         // Or eight more points, in which case we hit 25 verts total..

         // now do the squares:
         if(chunk->emptyFlags & CornerEmpty_0_1)
         {
            if((chunk->emptyFlags & CornerEmpty_0_1) != CornerEmpty_0_1)
            {
               if(!(chunk->emptyFlags & SquareEmpty_0_3))
               {
                  TerrBatch::emitTriangle(ip1, e3->pointIndex, p0);
                  TerrBatch::emitTriangle(ip1, p0, e0->pointIndex);
               }
               if(!(chunk->emptyFlags & SquareEmpty_1_3))
               {
                  TerrBatch::emitTriangle(ip1, e0->pointIndex, e0->pointIndex + 1);
                  TerrBatch::emitTriangle(ip1, e0->pointIndex + 1, ip5);
               }
               if(!(chunk->emptyFlags & SquareEmpty_1_2))
               {
                  TerrBatch::emitTriangle(ip1, ip5, ip0);
                  TerrBatch::emitTriangle(ip1, ip0, ip8);
               }
               if(!(chunk->emptyFlags & SquareEmpty_0_2))
               {
                  TerrBatch::emitTriangle(ip1, ip8, e3->pointIndex + 1);
                  TerrBatch::emitTriangle(ip1, e3->pointIndex + 1, e3->pointIndex);
               }
            }
            // 24 indices on this path ^^^
         }
         else
         {
            TerrBatch::emitTriangle( ip1, p0, e0->pointIndex );
            TerrBatch::emitFanStep( e0->pointIndex + 1 );
            TerrBatch::emitFanStep( ip5 );
            TerrBatch::emitFanStep( ip0 );
            TerrBatch::emitFanStep( ip8 );
            TerrBatch::emitFanStep( e3->pointIndex + 1 );
            TerrBatch::emitFanStep( e3->pointIndex );
            TerrBatch::emitFanStep( p0 );
            // And 24 here.
         }

         if(chunk->emptyFlags & CornerEmpty_1_1)
         {
            if((chunk->emptyFlags & CornerEmpty_1_1) != CornerEmpty_1_1)
            {

               if(!(chunk->emptyFlags & SquareEmpty_3_3))
               {
                  TerrBatch::emitTriangle(ip2, e0->pointIndex + 2, p1);
                  TerrBatch::emitTriangle(ip2, p1, e1->pointIndex);
               }
               if(!(chunk->emptyFlags & SquareEmpty_3_2))
               {
                  TerrBatch::emitTriangle(ip2, e1->pointIndex, e1->pointIndex + 1);
                  TerrBatch::emitTriangle(ip2, e1->pointIndex + 1, ip6);
               }
               if(!(chunk->emptyFlags & SquareEmpty_2_2))
               {
                  TerrBatch::emitTriangle(ip2, ip6, ip0);
                  TerrBatch::emitTriangle(ip2, ip0, ip5);
               }
               if(!(chunk->emptyFlags & SquareEmpty_2_3))
               {
                  TerrBatch::emitTriangle(ip2, ip5, e0->pointIndex + 1);
                  TerrBatch::emitTriangle(ip2, e0->pointIndex + 1, e0->pointIndex + 2);
               }
            }
         }
         else
         {
            TerrBatch::emitTriangle( ip2, p1, e1->pointIndex );
            TerrBatch::emitFanStep ( e1->pointIndex + 1 );
            TerrBatch::emitFanStep ( ip6 );
            TerrBatch::emitFanStep ( ip0 );
            TerrBatch::emitFanStep ( ip5 );
            TerrBatch::emitFanStep ( e0->pointIndex + 1 );
            TerrBatch::emitFanStep ( e0->pointIndex + 2 );
            TerrBatch::emitFanStep ( p1 );
         }

         if(chunk->emptyFlags & CornerEmpty_1_0)
         {
            if((chunk->emptyFlags & CornerEmpty_1_0) != CornerEmpty_1_0)
            {

               if(!(chunk->emptyFlags & SquareEmpty_3_0))
               {
                  TerrBatch::emitTriangle (ip3, e1->pointIndex + 2, p2);
                  TerrBatch::emitTriangle (ip3, p2, e2->pointIndex + 2);
               }
               if(!(chunk->emptyFlags & SquareEmpty_2_0))
               {
                  TerrBatch::emitTriangle (ip3, e2->pointIndex + 2, e2->pointIndex + 1);
                  TerrBatch::emitTriangle (ip3, e2->pointIndex + 1, ip7);
               }
               if(!(chunk->emptyFlags & SquareEmpty_2_1))
               {
                  TerrBatch::emitTriangle (ip3, ip7, ip0);
                  TerrBatch::emitTriangle (ip3, ip0, ip6);
               }
               if(!(chunk->emptyFlags & SquareEmpty_3_1))
               {
                  TerrBatch::emitTriangle (ip3, ip6, e1->pointIndex + 1);
                  TerrBatch::emitTriangle (ip3, e1->pointIndex + 1, e1->pointIndex + 2);
               }
            }
         }
         else
         {
            TerrBatch::emitTriangle(ip3, p2, e2->pointIndex + 2 );
            TerrBatch::emitFanStep( e2->pointIndex + 1 );
            TerrBatch::emitFanStep( ip7 );
            TerrBatch::emitFanStep( ip0 );
            TerrBatch::emitFanStep( ip6 );
            TerrBatch::emitFanStep( e1->pointIndex + 1 );
            TerrBatch::emitFanStep( e1->pointIndex + 2 );
            TerrBatch::emitFanStep( p2 );

         }

         if(chunk->emptyFlags & CornerEmpty_0_0)
         {
            if((chunk->emptyFlags & CornerEmpty_0_0) != CornerEmpty_0_0)
            {
               if(!(chunk->emptyFlags & SquareEmpty_0_0))
               {
                  TerrBatch::emitTriangle(ip4, e2->pointIndex, p3);
                  TerrBatch::emitTriangle(ip4, p3, e3->pointIndex + 2);
               }
               if(!(chunk->emptyFlags & SquareEmpty_0_1))
               {
                  TerrBatch::emitTriangle(ip4, e3->pointIndex + 2, e3->pointIndex + 1);
                  TerrBatch::emitTriangle(ip4, e3->pointIndex + 1, ip8);
               }
               if(!(chunk->emptyFlags & SquareEmpty_1_1))
               {
                  TerrBatch::emitTriangle(ip4, ip8, ip0);
                  TerrBatch::emitTriangle(ip4, ip0, ip7);
               }
               if(!(chunk->emptyFlags & SquareEmpty_1_0))
               {
                  TerrBatch::emitTriangle(ip4, ip7, e2->pointIndex + 1);
                  TerrBatch::emitTriangle(ip4, e2->pointIndex + 1, e2->pointIndex);
               }
            }
         }
         else
         {
            TerrBatch::emitTriangle( ip4, p3, e3->pointIndex + 2 );
            TerrBatch::emitFanStep( e3->pointIndex + 1 );
            TerrBatch::emitFanStep( ip8 );
            TerrBatch::emitFanStep( ip0 );
            TerrBatch::emitFanStep( ip7 );
            TerrBatch::emitFanStep( e2->pointIndex + 1 );
            TerrBatch::emitFanStep( e2->pointIndex );
            TerrBatch::emitFanStep( p3 );
         }
      }

      // 4 chunks with 24 indices each:                        96 indices
   }
   PROFILE_END();
}

bool TerrainRender::renderOverride(SceneState *state, SceneGraphData& sgData, TerrainBlock* block)
{
   if (TerrainRender::mOverrideMat)
   {
      const MatrixF blockTransform = block->getTransform();
      const Point3F &cameraPosition = gRenderInstManager->getCamPos();
      gChunkBatcher.renderChunkList(state,gChunkBatcher.mDiffusePasses[0], mOverrideMat, sgData, TerrBatch::vertexTypeFullClipMapping, NULL, blockTransform, cameraPosition, NULL);
      return true;
   } 
   else
   {
      return false;
   }
}

void TerrainRender::renderDiffuse(SceneState *state, SceneGraphData& sgData, TerrainBlock* block)
{      
   const MatrixF blockTransform = block->getTransform();
   const Point3F &cameraPosition = gRenderInstManager->getCamPos();
   // Run through all the bins and draw each range.
   for(S32 i=0; i<256; i++)
   {
      BatchHelper::ChunkList &cl = gChunkBatcher.mDiffusePasses[i];
      if(!cl.size())
         continue;

      PROFILE_START(TerrainRender_DrawDiffusePass);
      // There's stuff in this bin, so set up the clipmap and draw.
      U8 start = i&0xF;
      U8 end   = (i>>4)&0xF;
      S8 levelDelta = (end-start);         
      
      BaseMatInstance* mat = NULL;      
      if (!mFixedFunction)      
      {
         switch(levelDelta)
         {
         case 0:
         case 1:
            mat = block->mClipMap->getMaterialAndTextures(end, start);
            break;
         case 2:
            mat = block->mClipMap->getMaterialAndTextures(end, start+1, start);
            break;
         case 3:
            mat = block->mClipMap->getMaterialAndTextures(end, start+2, start+1, start);
            break;
         default:
            //AssertFatal(false, "TerrainRender::renderBlock - invalid level delta in clipmap setup!");
            mat = MaterialManager::get()->getWarningMatInstance();
            break;
         }
         gChunkBatcher.renderChunkList(state, cl, mat, sgData, TerrBatch::vertexTypeFullClipMapping, NULL, blockTransform, cameraPosition, NULL);
      }
      else
      {
         // multi-pass fixed function clipmapping
         Point4F clipmapMapping;
         // render the layers in order of least detail to most detail, each
         // with a vertex color controlling alpha blend transitions
         for (S32 curLayer=end; curLayer>=start;curLayer--)
         {
            mat = block->mClipMap->bindTexturesFF(curLayer, clipmapMapping, curLayer==end);
            gChunkBatcher.renderChunkList(state, cl, mat, sgData, TerrBatch::vertexTypeSingleTextureClipMapping, &clipmapMapping, blockTransform, cameraPosition, NULL);
         }
      }

      // megaterrain seams, yargh
      GRAPHIC->clearSamplerOverrides();
      GRAPHIC->updateStates(true);

      PROFILE_END();
   }
}

void TerrainRender::renderDetail(SceneState *state, SceneGraphData& sgData, TerrainBlock *block)
{
   if (!mDetailMat || block->mDetailTextureHandle.isNull() )
      return;
   
   PROFILE_SCOPE(TerrainRender_DrawDetailPass);
   
   if( mFixedFunction )
   {
      PROFILE_SCOPE(TerrainRender_DrawDetailPass_FixedFunction);

      MatrixF detailTexMatrix( true );
      detailTexMatrix.scale( Point3F( TerrainRender::mCurrentBlock->mDetailScale, 
                                      TerrainRender::mCurrentBlock->mDetailScale, 0.0f ) );

      GRAPHIC->setTextureMatrix( 0, detailTexMatrix );
   }
   else
   {
      PROFILE_SCOPE(TerrainRender_DrawDetailPass_Shader);
      Point3F detailConst(
         TerrainRender::mCurrentBlock->mDetailDistance / 2.0f,
         1.0f / (TerrainRender::mCurrentBlock->mDetailDistance / 2.0f), 
         TerrainRender::mCurrentBlock->mDetailScale);      

      MaterialParameters* matParams = mDetailMat->getMaterialParameters();      
      matParams->set(mDetDataSC, detailConst);         
      matParams->set(mBrightnessScaleSC, TerrainRender::mCurrentBlock->mDetailBrightness);
   }
   
   // Ok - next up, draw the detail chunks.   
   GRAPHIC->setTexture(0, TerrainRender::mCurrentBlock->mDetailTextureHandle);

   // Draw detail.
   const MatrixF blockTransform = block->getTransform();
   const Point3F &cameraPosition = gRenderInstManager->getCamPos();

   gChunkBatcher.renderChunkList(state, gChunkBatcher.mDetail, mDetailMat, sgData, mFixedFunction ? TerrBatch::vertexTypeDetail : TerrBatch::vertexTypeFullClipMapping, 
      NULL, blockTransform, cameraPosition, NULL);   
}

void TerrainRender::renderDynamicLights(SceneState *state, SceneGraphData& sgData, TerrainBlock *block, BaseMatInstance* m)
{
   // texture units for dlight materials are:
   // 0 : diffuseMap  (diffuse texture of base material)
   // 1 : blackfogMap (fog attenuation - optional)
   // 2 : dlightMap   (distance attenuation)
   // 3 : dlightMask  (directional color filter - optional)
   if(m && (!mRenderingCommander))
   {            
      PROFILE_SCOPE(TerrainRender_DrawDynamicLightsPass);

      const MatrixF blockTransform = block->getTransform();
      const Point3F &cameraPosition = gRenderInstManager->getCamPos();

      for(U32 i=0; i<mDynamicLightCount; i++)
      {
         AssertFatal(mTerrainLighting, "Terrain lighting should be non-null if dynamic light count > 0, see TerrRender::buildLightArray");
         // Do we have any chunks for this light? Skip if not.
         U32 curLayer = 0;
         for (; curLayer<16; curLayer++)
         {
            if(gChunkBatcher.mLightPasses[i][curLayer].size() > 0)
               break;
         }
         if (curLayer == 16)
            continue; // nothing lit

         // Ok - there is real work to do here, so set up the light info
         // and batch up our geometry.         
         LightInfo *light      = mTerrainLights[i].light;
         sgData.light = light;
         BaseMatInstance* dmat = NULL;
         TerrainRender::mTerrainLighting->setupLightStage(state->getLightManager(), light, sgData, m, &dmat);
         if (!dmat)
            continue;

         if (mFixedFunction)
            GRAPHIC->setTexture(1, sgData.blackfogTex);

         for (; curLayer<16; curLayer++)
         {
            if(gChunkBatcher.mLightPasses[i][curLayer].size())
            {
               // yes this is used on both pixel shader and fixed function path to set the diffuse pass.
               Point4F clipmapMapping;
               block->mClipMap->bindTexturesFF(curLayer, clipmapMapping, false);
               gChunkBatcher.renderChunkList(state, gChunkBatcher.mLightPasses[i][curLayer], dmat, sgData, mFixedFunction ? TerrBatch::vertexTypeDLight : TerrBatch::vertexTypeSingleTextureClipMapping, 
                  &clipmapMapping, blockTransform, cameraPosition, light);
            }
         }
         TerrainRender::mTerrainLighting->cleanupLights(state->getLightManager());
      }
   }      
}

void TerrainRender::renderFog(SceneState *state, SceneGraphData& sgData, TerrainBlock *block)
{   
   if (!mFogMat)
      return;

   const MatrixF blockTransform = block->getTransform();
   const Point3F &cameraPosition = gRenderInstManager->getCamPos();   

   PROFILE_SCOPE(TerrainRender_DrawFogPass);
      
   mFogMat->getMaterialParameters()->set(mMorphTSC, 0.0f);

   // D'oh, for FF the ProcessedFFMaterial doesn't read the texture info from script currently. 
   if (mFixedFunction)
      GRAPHIC->setTexture(0, state->getSceneManager()->getFogTexture());

   gChunkBatcher.renderChunkList(state, gChunkBatcher.mFog, mFogMat, sgData, 
      mFixedFunction ? TerrBatch::vertexTypeFog : TerrBatch::vertexTypeFullClipMapping, NULL,
      blockTransform, cameraPosition, NULL);               
}

void TerrainRender::renderBlock(TerrainBlock *block, SceneState *state, BaseMatInstance * m, SceneGraphData& sgData)
{
   PROFILE_START(TerrainRender);

   PROFILE_START(TerrainRenderSetup);
   U32 storedWaterMark = FrameAllocator::getWaterMark();

   mSceneState  = state;
   mFarDistance = state->getVisibleDistance();

   mCameraToObject = GRAPHIC->getWorldMatrix();
   mCameraToObject.inverse();
   mCameraToObject.getColumn(3, &mCamPos);

   mCurrentBlock = block;
   mSquareSize   = block->getSquareSize();

   const F32 blockSize = F32(mSquareSize * TerrainBlock::BlockSize);

   // Don't update the clipmap if we're reflecting, or if we are overriding the clipmap.
   if(!state->mFlipCull && !mOverrideMat)
   {   
      // Set up the clipmap -  have to update now so we put the right textures on
      // the right chunks.
      block->mClipMap->recenter(Point2F(mCamPos.x / blockSize, mCamPos.y / blockSize));
   }

   MatrixF proj = GRAPHIC->getProjectionMatrix();

   // compute pixelError
   if(mScreenError >= 0.001f)
      mPixelError = 1.0f / GRAPHIC->projectRadius(mScreenError, 1.f);
   else
      mPixelError = 0.000001f;

   buildClippingPlanes(state->mFlipCull);
   buildLightArray(state);

   // Calculate the potentially viewable area...
   S32 xStart = (S32)mFloor( (mCamPos.x - mFarDistance) / blockSize );
   S32 xEnd   = (S32)mCeil ( (mCamPos.x + mFarDistance) / blockSize );
   S32 yStart = (S32)mFloor( (mCamPos.y - mFarDistance) / blockSize );
   S32 yEnd   = (S32)mCeil ( (mCamPos.y + mFarDistance) / blockSize );

   // If we disable repeat, then start/end are always one.
   if(!mCurrentBlock->isTiling())
   {
      xStart = yStart = 0;
      xEnd = yEnd = 1;
   }

   S32 xExt   = (S32)(xEnd - xStart);
   S32 yExt   = (S32)(yEnd - yStart);

   PROFILE_END();
   PROFILE_START(TerrainRenderRecurse);

   // Come up with a dummy height to work with...
   F32 height = fixedToFloat(block->getHeight(0,0));

   EdgeParent **bottomEdges = (EdgeParent **) FrameAllocator::alloc(sizeof(ChunkScanEdge *) * xExt);

   TerrainRender::allocRenderEdges(xExt, bottomEdges, false);
   
   ChunkCornerPoint *prevCorner = TerrainRender::allocInitialPoint(
                                    Point3F(
                                       xStart * blockSize, 
                                       yStart * blockSize, 
                                       height
                                     )
                                   );

   mTerrainOffset.set(xStart * TerrainBlock::BlockSquareWidth, 
                      yStart * TerrainBlock::BlockSquareWidth);

   for(S32 x = 0; x < xExt; x++)
   {
      bottomEdges[x]->p1 = prevCorner;
      
      prevCorner = TerrainRender::allocInitialPoint(
                     Point3F(
                        (xStart + x + 1) * blockSize,
                        (yStart    ) * blockSize, 
                        height
                     )
                   );

      bottomEdges[x]->p2 = prevCorner;
   }

   for(S32 y = 0; y < yExt; y++)
   {
      // allocate the left edge:
      EdgeParent *left;
      TerrainRender::allocRenderEdges(1, &left, false);
      left->p1 = TerrainRender::allocInitialPoint(Point3F(xStart * blockSize, (yStart + y + 1) * blockSize, height));
      left->p2 = bottomEdges[0]->p1;

      for(S32 x = 0; x < xExt; x++)
      {
         // Allocate the right,...
         EdgeParent *right;
         TerrainRender::allocRenderEdges(1, &right, false);
         right->p1 = TerrainRender::allocInitialPoint(
                        Point3F(
                           (xStart + x + 1) * blockSize, 
                           (yStart + y + 1) * blockSize, 
                           height
                        )
                     );
         right->p2 = bottomEdges[x]->p2;

         // Allocate the top...
         EdgeParent *top;
         TerrainRender::allocRenderEdges(1, &top, false);
         top->p1 = left->p1;
         top->p2 = right->p1;

         // Process this block...

         //    -  set up for processing the block...
         mBlockOffset.set(x << TerrainBlock::BlockShift,
                          y << TerrainBlock::BlockShift);

         mBlockPos.set((xStart + x) * blockSize,
                       (yStart + y) * blockSize);

         //    - Do it...
         TerrainRender::processBlock(state, top, right, bottomEdges[x], left);

         // Clean up...
         left = right;
         bottomEdges[x] = top;
      }
   }
   
   GRAPHIC->setProjectionMatrix(proj);

   PROFILE_END();

   PROFILE_START(TerrainRenderEmit);

   // Set up for the render...
   initMaterials(sgData);
   if (!renderOverride(state, sgData, block))
   {
      renderDiffuse(state, sgData, block);
      renderDetail(state, sgData, block);
      renderDynamicLights(state, sgData, block, m);
      renderFog(state, sgData, block);
   }

   // Cleanup
   gChunkBatcher.reset();
      
   FrameAllocator::setWaterMark(storedWaterMark);
   
   GRAPHIC->disableShaders();
   
   GRAPHIC->clearSamplerOverrides();
   GRAPHIC->updateStates(true);

   PROFILE_END();
   PROFILE_END_NAMED(TerrainRender);   
   mCurrentBlock = NULL;
}
