//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _ATLASRESOURCEGEOMTOC_H_
#define _ATLASRESOURCEGEOMTOC_H_

#include "atlas/core/atlasResourceTOC.h"
#include "atlas/resource/atlasGeomChunk.h"
#include "Core/FairyPrerequisites.h"
struct RayInfo;
class Convex;
class AbstractPolyList;

/// @defgroup AtlasResource Atlas Resource System
///
/// Although the core background loading system is implemented in AtlasCore, 
/// the specific data types that are loaded, the rules for regenerating them,
/// and so forth all live in this group.
///
/// @ingroup Atlas

/// Atlas resource stub subclass for geometry.
///
/// @ingroup AtlasResource
class AtlasResourceGeomStub : public AtlasResourceStub<AtlasGeomChunk>
{
public:
   AtlasResourceGeomStub()
   {
      mBounds.minExtents.set(-100, -100, -100);
      mBounds.maxExtents.set( 100,  100,  100);
   }

   /// Geometry bounds.
   Box3F mBounds;

   typedef AtlasResourceStub<AtlasGeomChunk> Parent;
   virtual void read(Stream *s, const U32 version);
   virtual void write(Stream *s);
};

/// Atlas resource TOC subclass for geometry.
///
/// @ingroup AtlasResource
class AtlasResourceGeomTOC : public AtlasResourceTOC<AtlasResourceGeomStub>
{
public:

   typedef AtlasResourceTOC<AtlasResourceGeomStub> Parent;

   U32 mGoalBatchSize;
   F32 mDistance_LODMax;

   void initializeTOC(U32 depth)
   {
      helpInitializeTOC(depth);
   }

   AtlasResourceGeomTOC();
   ~AtlasResourceGeomTOC();

   virtual bool write(Stream *s);
   virtual bool read(Stream *s);

   /// Override so we can update stub bounds when a new chunk is stored.
   virtual void instateLoadedChunk(StubType *stub, ChunkType *newChunk);
}; 

class IGetSurfaceHelper;

/// Allow traversal over atlas geometry using UV coordinates. Performs
/// normal interpolation.
class AtlasSurfaceQueryHelper
{
   IGetSurfaceHelper* mGetSurfaceHelper;
   bool internalLookup(const Point2F &inUv, bool bFindNeighbor, U16 inTriIdx, U16 &outTriIdx, Point3F &outPos, Point3F &outNorm);
   void findNeighborNormal(const Point2F& coord, Point3F& result);
   void getAdjustedNormal(U16 inTriIdx, bool bFindNeighbor, Point3F& result);
public:
   AtlasGeomChunk *mChunk;

   /// Texture bounds of the chunk.
   RectF mTexBounds;

   /// Number of bins in the grid on each axis.
   U32 mGridCount;

   /// List of lists of triangles in bins. Indexed by mTriangleOffsets.
   ///
   /// Lists are 0 terminated.
   Vector<U16> mTriangleBuffer;

   /// For each bin, index into mTriangleBuffers for that bin's triangles.
   Vector<U16> mTriangleOffsets;

   /// Last triangle we hit. Used to optimize queries.
   S32 mLastHitTriangle;

   /// List of vertex normals, used for interpolation.
   Vector<Point3F> mVertexNormals;

   AtlasSurfaceQueryHelper(IGetSurfaceHelper* gsh);
   ~AtlasSurfaceQueryHelper();
   void generateLookupTables();
   bool pointInTri(const Point2F &pt, const Point2F &a, const Point2F &b, 
      const Point2F &c, Point2F &baryCoords);
   /// Perform a lookup in UV space. Takes a UV parameter, and returns the
   /// index of the triangle it's on, the position in 3space at which that UV
   /// is located, and a smoothed normal for that location.
   bool lookup(const Point2F &inUv, bool bFindNeighbor, U16 &outTriIdx, Point3F &outPos, Point3F &outNorm);
};

// This is an interface which returns a surface helper for a give UV coordinate.  Used
// to get neighbor surface helpers
class IGetSurfaceHelper
{
public:
   virtual AtlasSurfaceQueryHelper* getSurfaceHelper(const Point2F& tc) = 0;
};

#endif
