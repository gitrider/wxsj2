//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _TERRRENDER_H_
#define _TERRRENDER_H_

#ifndef _TERRDATA_H_
#include "terrain/terrData.h"
#endif

#include "materials/matInstance.h"

struct EmitChunk;

struct EdgePoint : public Point3F
{
   Point3F vn;
};

struct ChunkCornerPoint : public EdgePoint
{
   U32 pointIndex;
   U32 xfIndex;
};

struct EdgeParent
{
   ChunkCornerPoint *p1, *p2;
};

struct ChunkScanEdge : public EdgeParent
{
   ChunkCornerPoint *mp;
   EdgeParent *e1, *e2;
};

struct ChunkEdge : public EdgeParent
{
   U32 xfIndex;
   U16 pointIndex;
   U16 pointCount;

   EdgePoint pt[3];
   EmitChunk *c1, *c2;
};

struct EmitChunk
{
   ChunkEdge *edge[4];
   F32 growFactor;
   S32 x, y;
   U32 emptyFlags;
   S16 subDivLevel;
   U16 lightMask;
};

struct SquareStackNode
{
   U32     clipFlags;
   U32     lightMask;
   Point2I pos;
   U32     level;
   EdgeParent *top, *right, *bottom, *left;
};

struct TerrLightInfo
{
   Point3F pos;       ///< world position
   F32 radius;        ///< radius of the light
   LightInfo *light;
};

enum EmptyFlags {
   SquareEmpty_0_0 = BIT(0),
   SquareEmpty_1_0 = BIT(1),
   SquareEmpty_2_0 = BIT(2),
   SquareEmpty_3_0 = BIT(3),
   SquareEmpty_0_1 = BIT(4),
   SquareEmpty_1_1 = BIT(5),
   SquareEmpty_2_1 = BIT(6),
   SquareEmpty_3_1 = BIT(7),
   SquareEmpty_0_2 = BIT(8),
   SquareEmpty_1_2 = BIT(9),
   SquareEmpty_2_2 = BIT(10),
   SquareEmpty_3_2 = BIT(11),
   SquareEmpty_0_3 = BIT(12),
   SquareEmpty_1_3 = BIT(13),
   SquareEmpty_2_3 = BIT(14),
   SquareEmpty_3_3 = BIT(15),
   CornerEmpty_0_0 = SquareEmpty_0_0 | SquareEmpty_1_0 | SquareEmpty_0_1 | SquareEmpty_1_1,
   CornerEmpty_1_0 = SquareEmpty_2_0 | SquareEmpty_3_0 | SquareEmpty_2_1 | SquareEmpty_3_1,
   CornerEmpty_0_1 = SquareEmpty_0_2 | SquareEmpty_1_2 | SquareEmpty_0_3 | SquareEmpty_1_3,
   CornerEmpty_1_1 = SquareEmpty_2_2 | SquareEmpty_3_2 | SquareEmpty_2_3 | SquareEmpty_3_3,
};

struct RenderPoint : public Point3F
{
};

enum TerrConstants {
   MaxClipPlanes       = 8, ///< left, right, top, bottom - don't need far tho...
   MaxTerrainMaterials = 256,

   MaxTerrainLights = 64,
   MaxVisibleLights = 31,
   ClipPlaneMask    = (1 << MaxClipPlanes) - 1,
   FarSphereMask    = 0x80000000,
   FogPlaneBoxMask  = 0x40000000,
};

class SceneState;

inline U32 getPower(U32 x)
{
   F32 floatValue = F32(x);
   return (*((U32*) &floatValue) >> 23) - 127;
}

// Allows a lighting system to plug into terrain rendering
class TerrainLightingPlugin
{
public:
   virtual ~TerrainLightingPlugin() {}
   
   virtual void setupLightStage(LightManager * lm, LightInfo* light, SceneGraphData& sgData, BaseMatInstance* basemat, BaseMatInstance** dmat) = 0;
   virtual void cleanupLights(LightManager * lm) {}
};

struct TerrainRender
{
   static MatrixF mCameraToObject;

   static SceneState *mSceneState;

   static TerrainBlock *mCurrentBlock;
   static S32 mSquareSize;
   static F32 mScreenSize;
   static U32 mNumClipPlanes;

   static Point2F mBlockPos;
   static Point2I mBlockOffset;
   static Point2I mTerrainOffset;

   static PlaneF mClipPlane[MaxClipPlanes];
   static Point3F mCamPos;

   static TerrainLightingPlugin* mTerrainLighting;
   static TerrLightInfo mTerrainLights[MaxTerrainLights];
   static U32  mDynamicLightCount;
   static bool mEnableTerrainDynLights;

   static F32 mPixelError;
   static F32 mScreenError;
   static F32 mMinSquareSize;
   static F32 mFarDistance;
   static F32 mInvFarDistance;
   static F32 mInvHeightRange;
   static bool mRenderingCommander;

   static bool mMaterialsInit;
   // Used to render the terrain with a different material.
   static BaseMatInstance* mOverrideMat;
   static BaseMatInstance* mDetailMat;
   static MaterialParameterHandle* TerrainRender::mBrightnessScaleSC;
   static MaterialParameterHandle* TerrainRender::mDetDataSC;
   static BaseMatInstance* mFogMat;
   static MaterialParameterHandle* TerrainRender::mMorphTSC;   

   static bool mFixedFunction;

   // Control...
   static void init();
   static void shutdown();
   static void initMaterials(SceneGraphData& sgData);

   // Points...
   static ChunkCornerPoint *allocInitialPoint(const Point3F &pos);
   static ChunkCornerPoint *allocPoint(const Point2I &pos);

   // Edges...
   static void allocRenderEdges(U32 edgeCount, EdgeParent **dest, bool renderEdge);
   static void fixEdge(ChunkEdge *edge, S32 x, S32 y, S32 dx, S32 dy);
   static void subdivideChunkEdge(ChunkScanEdge *e, const Point2I &pos, bool chunkEdge);

   // Core functionality...
   static void processBlock(SceneState* state, EdgeParent *topEdge, EdgeParent *rightEdge, EdgeParent *bottomEdge, EdgeParent *leftEdge);
   static void emitTerrChunk(SquareStackNode *n, F32 squareDistance, U32 lightMask, F32 zDelta, F32 squareDistanceLOD);
   static void renderBlock(TerrainBlock *, SceneState *state, BaseMatInstance * m, SceneGraphData& sgData);

   static bool TerrainRender::renderOverride(SceneState *state, SceneGraphData& sgData, TerrainBlock* block);
   static void renderDiffuse(SceneState *state, SceneGraphData& sgData, TerrainBlock* block);
   static void renderDetail(SceneState *state, SceneGraphData& sgData, TerrainBlock *block);
   static void renderDynamicLights(SceneState *state, SceneGraphData& sgData, TerrainBlock *block, BaseMatInstance* m);
   static void renderFog(SceneState *state, SceneGraphData& sgData, TerrainBlock *block);

   static void renderChunkNormal(EmitChunk *chunk);
   static void renderChunkCommander(EmitChunk *chunk);

   // Helper functions...
   static U32 testSquareLights(GridSquare *sq, S32 level, const Point2I &pos, U32 lightMask);
   static S32 testSquareVisibility(const Point3F &minPt, const Point3F &maxPt, S32 clipMask, F32 expand);

   static F32 getSquareDistance(const Point3F& minPoint, const Point3F& maxPoint,
                                F32* zDiff);

   static void buildLightArray(SceneState *);
   static void buildClippingPlanes(bool flipClipPlanes);

};

#endif
