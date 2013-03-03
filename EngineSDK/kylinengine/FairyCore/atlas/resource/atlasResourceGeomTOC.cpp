//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "atlas/resource/atlasResourceObjectTOC.h"
#include "atlas/resource/atlasGeomCollision.h"
#include "core/frameAllocator.h"
#include "math/mathIO.h"

void AtlasResourceGeomStub::read(Stream *s, const U32 version)
{
   Parent::read(s, version);

   // Will there be a bounding box or just min/max?
   if(version < 101)
   {
      // Nuts, no box.
      F32 tmp;
      s->read(&tmp);
      s->read(&tmp);

      // Upconvert? No, not yet.

      Con::errorf("AtlasResourceGeomStub::read - pre v101 file format, no bounding box data present!");
   }
   else
   {
      // Ok, load that box.
      mathRead(*s, &mBounds);

      // Check sentinel.
      U8 sentinel;
      s->read(&sentinel);

      AssertISV(sentinel == 0xd0, "AtlasResourceGeomStub::read - did not encounter valid sentinel.")
   }
}

void AtlasResourceGeomStub::write(Stream *s)
{
   Parent::write(s);
   mathWrite(*s, mBounds);

   // Add a sentinel
   s->write(U8(0xd0));
}


//-------------------------------------------------------------------------

AtlasResourceGeomTOC::AtlasResourceGeomTOC()
{
   mGoalBatchSize = 1500;
   mDistance_LODMax = 50.f;
   mIsQuadtree = true;
}

AtlasResourceGeomTOC::~AtlasResourceGeomTOC()
{
}

bool AtlasResourceGeomTOC::write(Stream *s)
{
   s->write(mGoalBatchSize);

   return Parent::write(s);
}

bool AtlasResourceGeomTOC::read(Stream *s)
{
   s->read(&mGoalBatchSize);

   return Parent::read(s);
}

void AtlasResourceGeomTOC::instateLoadedChunk(StubType *stub, ChunkType *newChunk )
{
   // Got a new chunk, so let's update the stub's bounding box.
   stub->mBounds = newChunk->getBounds();

   // And pass control on up.
   Parent::instateLoadedChunk(stub, newChunk);
}

//
// AtlasSurfaceQueryHelper
//
AtlasSurfaceQueryHelper::AtlasSurfaceQueryHelper(IGetSurfaceHelper* gsh)
{
   mGridCount = 8;
   mTexBounds.point.set(0,0);
   mTexBounds.extent.set(0,0);
   mLastHitTriangle = -1;
   mGetSurfaceHelper = gsh;
}

AtlasSurfaceQueryHelper::~AtlasSurfaceQueryHelper()
{
}

void AtlasSurfaceQueryHelper::generateLookupTables()
{
   // Walk all of the bins and suck triangles into them. Not very
   // efficient but could be worse.
   Point2F binSize;
   binSize.x = mTexBounds.extent.x / F32(mGridCount);
   binSize.y = mTexBounds.extent.y / F32(mGridCount);

   // Initialize our arrays.
   mTriangleOffsets.setSize(mGridCount * mGridCount);
   mTriangleBuffer.reserve(mTriangleOffsets.size() * 8);
   mTriangleBuffer.clear();
   mLastHitTriangle = -1;

   for(S32 x=0; x<mGridCount; x++)
   {
      for(S32 y=0; y<mGridCount; y++)
      {
         // Generate the bin TC bounds.
         RectF binBounds;
         binBounds.point.x = F32(x) * binSize.x;
         binBounds.point.y = F32(y) * binSize.y;
         binBounds.extent = binSize;

         // Note bin start.
         mTriangleOffsets[x*mGridCount + y] = mTriangleBuffer.size();

         //Con::printf("for bin %dx%d", x, y);

         // Now, find all triangles that overlap this TC range.
         for(S32 i=0; i<mChunk->mIndexCount-2; i+=3)
         {
            // Snag texcoords!
            const Point2F &a = mChunk->mVert[mChunk->mIndex[i+0]].texCoord - mTexBounds.point;
            const Point2F &b = mChunk->mVert[mChunk->mIndex[i+1]].texCoord - mTexBounds.point;
            const Point2F &c = mChunk->mVert[mChunk->mIndex[i+2]].texCoord - mTexBounds.point;

            // If the TCs are a degenerate quad, skip it...
            if(a==b || b ==c || c==a)
               continue;

            // Do overlap test.
            if(!binBounds.intersectTriangle(a,b,c))
               continue;

            mTriangleBuffer.push_back(i);

            // We overlap, so add to list.
            //Con::printf("     idx %d", i);
         }

         // Finally, null-terminate the list.
         mTriangleBuffer.push_back(0xFFFF);
      }
   }

   // Generate face normals.
   Vector<Point3F> faceNormals;
   faceNormals.reserve((mChunk->mIndexCount)/3);

   for(S32 i=0; i<mChunk->mIndexCount-2; i+=3)
   {
      // Snag verts!
      const GFXAtlasVert2 &a = mChunk->mVert[mChunk->mIndex[i+0]];
      const GFXAtlasVert2 &b = mChunk->mVert[mChunk->mIndex[i+1]];
      const GFXAtlasVert2 &c = mChunk->mVert[mChunk->mIndex[i+2]];

      Point3F e1 = b.point - a.point;
      Point3F e2 = c.point - a.point;
      faceNormals.increment();

      e1.normalize();
      e2.normalize();
      mCross(e2, e1, &faceNormals.last());
   }

   // Now - for every vert, generate a normal by averaging the faces it
   // is present on.
   mVertexNormals.setSize(mChunk->mVertCount);

   Vector<U16> adjacentFaces(128);

   for(S32 i=0; i<mChunk->mVertCount; i++)
   {
      // Find adjacent faces - not fast but we can optimize later.
      adjacentFaces.clear();
      for(S32 j=0; j<mChunk->mIndexCount-2; j+=3)
      {
         // If it's degenerate UV wise, skip it.
         const GFXAtlasVert2 &a = mChunk->mVert[mChunk->mIndex[j+0]];
         const GFXAtlasVert2 &b = mChunk->mVert[mChunk->mIndex[j+1]];
         const GFXAtlasVert2 &c = mChunk->mVert[mChunk->mIndex[j+2]];

         if(a.texCoord == b.texCoord || b.texCoord == c.texCoord || c.texCoord == a.texCoord)
            continue;

         // Kick out skirts
         if (mFabs(faceNormals[j/3].z) < 0.001)
            continue;

         if(mChunk->mIndex[j+0] == i || mChunk->mIndex[j+1] == i || mChunk->mIndex[j+2] == i)
            adjacentFaces.push_back(j/3);
      }

      if(!adjacentFaces.size())
         continue;

      // Ok, now accumulate the normals.
      Point3F normAcc(0,0,0);
      //AssertFatal(adjacentFaces.size(), "AtlasLightingQueryHelper::generateLookupTables - orphan vertex!");
      for(S32 j=0; j<adjacentFaces.size(); j++)
         normAcc = normAcc + faceNormals[adjacentFaces[j]];

      normAcc /= F32(adjacentFaces.size());

      // Normalize the result.
      normAcc.normalize();

      // Store!
      mVertexNormals[i] = normAcc;
   }
}

bool AtlasSurfaceQueryHelper::pointInTri(const Point2F &pt, const Point2F &a, const Point2F &b, 
                                         const Point2F &c, Point2F &baryCoords)
{
   const Point2F e1 = b - a;
   const Point2F e2 = c - a;
   const Point2F h = pt - a;

   baryCoords.x = (e2.x * h.y - e2.y * h.x) / (e2.x * e1.y - e2.y * e1.x);
   baryCoords.y = (h.x * e1.y - h.y * e1.x) / (e2.x * e1.y - e2.y * e1.x);

   if(baryCoords.x < 0.f)
      return false;

   if(baryCoords.y < 0.f)
      return false;

   if((baryCoords.x + baryCoords.y) > 1.f)
      return false;

   // Ok we got a hit!
   return true;
}

void AtlasSurfaceQueryHelper::findNeighborNormal(const Point2F& coord, Point3F& result)
{
   AtlasSurfaceQueryHelper* sqh = mGetSurfaceHelper->getSurfaceHelper(coord); // yes, department of redundancy department ;)
   if (sqh != this)
   {
      U16 outTriIdx;
      Point3F outPos;
      Point3F outNorm;
      if (sqh->lookup(coord, false, outTriIdx, outPos, outNorm))
      {
         result += outNorm;
      }
   }
}

void AtlasSurfaceQueryHelper::getAdjustedNormal(U16 inTriIdx, bool bFindNeighbor, Point3F& result)
{
   result = mVertexNormals[mChunk->mIndex[inTriIdx]];

   if (bFindNeighbor)
   {
      const GFXAtlasVert2 &a = mChunk->mVert[mChunk->mIndex[inTriIdx]];

      // We want just enough to push us over into another bin, so no scaling by
      // interval or anything fancy.
      F32 offset = 0.001f;  

      findNeighborNormal(Point2F(a.texCoord.x - offset, a.texCoord.y), result);
      findNeighborNormal(Point2F(a.texCoord.x + offset, a.texCoord.y), result);
      findNeighborNormal(Point2F(a.texCoord.x, a.texCoord.y - offset), result);
      findNeighborNormal(Point2F(a.texCoord.x, a.texCoord.y + offset), result);

      result.normalize();
   }
}

bool AtlasSurfaceQueryHelper::internalLookup(const Point2F &inUv, bool bFindNeighbor, U16 inTriIdx, U16 &outTriIdx, Point3F &outPos, Point3F &outNorm)
{
   // Is the UV within the texcoords for this triangle?
   const GFXAtlasVert2 &a = mChunk->mVert[mChunk->mIndex[inTriIdx+0]];
   const GFXAtlasVert2 &b = mChunk->mVert[mChunk->mIndex[inTriIdx+1]];
   const GFXAtlasVert2 &c = mChunk->mVert[mChunk->mIndex[inTriIdx+2]];

   Point2F bary;
   if(pointInTri(inUv, a.texCoord, b.texCoord, c.texCoord, bary))
   {
      // Interpolate position...
      Point3F posEdge1 = b.point - a.point;
      Point3F posEdge2 = c.point - a.point;
      outPos = a.point + posEdge1 * bary.x + posEdge2 * bary.y;

      // Interpolate normal...
      Point3F nA, nB, nC;
      getAdjustedNormal(inTriIdx+0, bFindNeighbor, nA);
      getAdjustedNormal(inTriIdx+1, bFindNeighbor, nB);
      getAdjustedNormal(inTriIdx+2, bFindNeighbor, nC);

      outNorm = nA + (nB - nA) * bary.x + (nC - nA) * bary.y;

      // Note the triangle we hit!
      outTriIdx = inTriIdx;
      return true;
   }
   return false;
}
/// Perform a lookup in UV space. Takes a UV parameter, and returns the
/// index of the triangle it's on, the position in 3space at which that UV
/// is located, and a smoothed normal for that location.
bool AtlasSurfaceQueryHelper::lookup(const Point2F &inUv, bool bFindNeighbor, U16 &outTriIdx, Point3F &outPos, Point3F &outNorm)
{
   // First, see if we are within the last hit triangle - if so we can
   // save some work!
   if ((mLastHitTriangle != -1) && internalLookup(inUv, bFindNeighbor, mLastHitTriangle, outTriIdx, outPos, outNorm))
   {
      return true;
   }

   // Otherwise, search the bin.
   Point2F binSize;
   binSize.x = mTexBounds.extent.x / F32(mGridCount);
   binSize.y = mTexBounds.extent.y / F32(mGridCount);

   const Point2F localUV = inUv - mTexBounds.point;
   S32 binX = mFloor(localUV.x / binSize.x);
   S32 binY = mFloor(localUV.y / binSize.y);

   // Get the bin list.
   U32 index = binX * mGridCount + binY;
   if (index >= mTriangleOffsets.size())
      index = mTriangleOffsets.size() - 1;
   S32 binOffset = mTriangleOffsets[index];      

   // Walk it.
   U16 curIdx = 0;
   while((curIdx = mTriangleBuffer[binOffset++]) != 0xFFFF)
   {
      // Is the UV within the texcoords for this triangle?
      AssertFatal(binOffset < mTriangleBuffer.size(), 
         "AtlasLightingQueryHelper::lookup - out of rang binOffset!");
      AssertFatal(curIdx+2 < mChunk->mIndexCount, 
         "AtlasLightingQueryHelper::lookup - out of range index!");

      if (internalLookup(inUv, bFindNeighbor, curIdx, outTriIdx, outPos, outNorm))
      {
         mLastHitTriangle = outTriIdx;
         return true;
      }
   }

   return false;
}
