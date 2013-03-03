//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ATLASINSTANCE2_H_
#define _ATLASINSTANCE2_H_

#include "platform/threads/thread.h"
#include "platform/threads/mutex.h"
#include "core/stream/stream.h"
#include "core/resManager.h"
#include "core/util/safeDelete.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxStructs.h"
#include "sceneGraph/sceneObject.h"
#include "materials/matInstance.h"
#include "materials/materialList.h"
#include "atlas/core/atlasFile.h"
#include "atlas/runtime/atlasInstanceGeomTOC.h"
#include "atlas/runtime/atlasInstanceTexTOC.h"
#include "atlas/runtime/atlasInstanceObjectTOC.h"
#include "atlas/runtime/atlasClipMapBatcher.h"

class ClipMap;
class ClipMapBlenderCache;
class AtlasClipMapImageSource;

/// @defgroup AtlasRuntime Atlas Runtime
///
/// Functionality that relates to rendering and interacting with an Atlas
/// terrain in a running game instance.
///
/// @ingroup Atlas

/// An instance of an Atlas terrain.
///
/// Provides glue code to bring an Atlas terrain into the game world!
///
/// @ingroup AtlasRuntime
class AtlasInstance : public SceneObject
{
   friend class AtlasClipMapBatcher;
   typedef SceneObject Parent;

   ResourceOld<AtlasFile> mAtlasFile;
   AtlasFile* mAtlasMissionFile;
   AtlasInstanceGeomTOC *mGeomTOC;
   AtlasInstanceObjectTOC *mObjectTOC;
   ResourceOld<AtlasFile> mLightMapFile;

   GFXTexHandle mDetailTex;

   StringTableEntry mDetailTexFileName;
   StringTableEntry mAtlasFileName;
   StringTableEntry mAtlasMissionFileName;

   ClipMap *mClipMap;
   AtlasClipMapBatcher mBatcher;

   // Blended atlas specific stuff
   bool mIsBlended;
   U32 mNumBlendLayers;
   ClipMapBlenderCache* mAcmic_b;
   AtlasResourceTexTOC* mOpacityToc;
   MaterialList mMaterialList;
   U32 mLightmapDimension;
   S32 mLMChunkSize;

   void syncThreads();
public:
   AtlasInstance();
   ~AtlasInstance();
   DECLARE_CONOBJECT(AtlasInstance);

   struct atlasMaterial 
   {
	   enum Flags 
	   {
		   Plain          = 0,
		   Rotate         = 1,
		   FlipX          = 2,
		   FlipXRotate    = 3,
		   FlipY          = 4,
		   FlipYRotate    = 5,
		   FlipXY         = 6,
		   FlipXYRotate   = 7,
		   RotateMask     = 7,
		   Empty          = 8,
		   Modified       = BIT(7),

		   // must not clobber TerrainFile::MATERIAL_GROUP_MASK bits!
		   PersistMask       = BIT(7)
	   };
	   U8 flags;
	   U8 index;
   };

   AtlasInstanceGeomTOC *getGeomTOC()
   {
      return mGeomTOC;
   }
   AtlasInstanceObjectTOC *getObjectTOC()
   {
	   return mObjectTOC;
   }

   AtlasFile* getAtlasFile()
   {
	   return mAtlasFile;
   }
   AtlasFile* getLightMapFile()
   {
	   return mLightMapFile;
   }

   void findDeepestStubs(Vector<AtlasResourceGeomTOC::StubType *> &stubs);

   bool onAdd();
   void onRemove();

   static void consoleInit();
   static void initPersistFields();

   bool prepRenderImage(SceneState *state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState);
   void renderObject( ObjectRenderInst* ri, BaseMatInstance* overrideMat );

   bool castRay(const Point3F &start, const Point3F &end, RayInfo* info);
   void buildConvex(const Box3F& box, Convex* convex);
   bool buildPolyList(AbstractPolyList* polyList, const Box3F &box, const SphereF &sphere);


   void refillClipMap();

   enum RayCollisionDebugLevel
   {
      RayCollisionDebugToTriangles = 0,
      RayCollisionDebugToColTree   = 1,
      RayCollisionDebugToChunkBox  = 2,
      RayCollisionDebugToObjectBox = 3,
      RayCollisionDebugToMesh      = 4, ///< Let raycasting test against raw geometry data rather than the collision mesh.
   };

   enum ChunkBoundsDebugMode
   {
      ChunkBoundsDebugNone          = 0,
      ChunkBoundsDebugLODThreshold  = 1,
      ChunkBoundsDebugLODHolistic   = 2,
      ChunkBoundsDebugHeat          = 3,
   };

   static S32  smRayCollisionDebugLevel;
   static S32  smRenderChunkBoundsDebugLevel;
   static bool smLockFrustrum;
   static bool smRenderWireframe;
   static bool smNoUpdate;

   // Lightmap support
   void loadAlternateLightMap(const StringTableEntry sLightmapFile);
   void releaseLightmap();
   S32 getLightMapChunkSize();
   const StringTableEntry getAtlasFilename() { return mAtlasFileName; }
   bool isBlended() { return mIsBlended; }
   U32 getLightMapDimension() { return mLightmapDimension; }
};

#endif
