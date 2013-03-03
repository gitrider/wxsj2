//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "atlas/editor/atlasOldMesher.h"
#include "atlas/editor/atlasOldGenerator.h"
#include "core/resManager.h"
#include "core/stream/fileStream.h"
#include "math/mRect.h"
#include "util/triBoxCheck.h"
#include "util/quadTreeTracer.h"
#include "core/frameAllocator.h"

//------------------------------------------------------------------------

AtlasOldMesher::AtlasOldMesher(AtlasOldActivationHeightfield *hf, S8 level)
{
   mHeight = hf;
   mLevel = level;

   AssertFatal(mLevel > 0,
      "AtlasOldMesher::AtlasOldMesher - Chunks must be level 1 or greater!");

   // Kick out the hash bins.
   mVertHash.increment(HashBinCount);
}

AtlasOldMesher::~AtlasOldMesher()
{
}

AtlasOldMesher::VertIndex AtlasOldMesher::getVertIndex(Point2I pos)
{
   // Calculate hash for this position.
   S32 bin = generatePointHash(pos) % HashBinCount;

   // First, check to see if it exists already.
   for(VertHashBin::const_iterator i=mVertHash[bin].begin(); i!=mVertHash[bin].end(); i++)
      if(mVerts[*i].isEqual(pos))
         return *i;

   // No hit, so let's add it to the vert store.
   mVerts.push_back(Vert(pos.x, pos.y));
   VertIndex vi = mVerts.size() - 1;

   // Add it to the hash, too.
   mVertHash[bin].push_back(vi);

   // And return...
   return vi;
}

AtlasOldMesher::VertIndex AtlasOldMesher::addSpecialVert(Point2I pos, S16 z)
{
   // We optimize a bit by checking against just the special lists.

   // We check special verts, too, it's fun.
   for(S32 i=0; i<mSpecialVerts.size(); i++)
      if(mVerts[mSpecialVerts[i]].isSpecialEqual(pos, z))
         return mSpecialVerts[i];

   // No hit, add it.
   mVerts.push_back(Vert(pos.x, pos.y, z));
   mSpecialVerts.push_back(mVerts.size() - 1);
   return mVerts.size() - 1;
}

Point3F AtlasOldMesher::getVertPos(const Vert &v)
{
   Point3F res;

   const Point2I &vertPos = v.pos;
   Point3F center;
   mBounds.getCenter(&center);

   if (v.special)
      res.z = v.z;
   else
      res.z = mHeight->sampleRead(vertPos);

   res.z *= mHeight->mVerticalScale;

   res.x = (vertPos.x * mHeight->mSampleSpacing) - mBounds.minExtents.x;
   res.y = (vertPos.y * mHeight->mSampleSpacing) - mBounds.minExtents.y;

   return res;
}

void AtlasOldMesher::writeVertex(Stream *s, Vert *vert, const S8 level)
{
   S16 x, y, z;

   const Point2I &vertPos = vert->pos;
   Point3F center;
   mBounds.getCenter(&center);

   if (vert->special)
      z = vert->z;
   else
      z = mHeight->sampleRead(vertPos);

   S32 xTmp, yTmp;
   xTmp = (S32)mFloor(((vertPos.x * mHeight->mSampleSpacing - center.x) * mCompressionFactor.x) + 0.5);
   yTmp = (S32)mFloor(((vertPos.y * mHeight->mSampleSpacing - center.y) * mCompressionFactor.y) + 0.5);

   AssertFatal(S16(xTmp) == xTmp,
      "AtlasOldMesher::writeVertex - Overflow writing x-coordinate!");
   AssertFatal(S16(yTmp) == yTmp,
      "AtlasOldMesher::writeVertex - Overflow writing y-coordinate!");

   x = xTmp;
   y = yTmp;

   s->write(x);
   s->write(y);
   s->write(z);

   // Morph info.  Calculate the difference between the
   // vert height, and the height of the same spot in the
   // next lower-LOD mesh.
   S16 morphHeight;
   if (vert->special)
      morphHeight = z;	// special verts don't morph.
   else
   {
      morphHeight = mHeight->getHeightAtLOD(vertPos, level + 1);
   }

   S32 morphDelta = (S32(morphHeight) - S32(z));
   s->write(S16(morphDelta));

   if(morphDelta != S16(morphDelta))
      Con::warnf("AtlasOldMesher::writeVertex - overflow in lerpedHeight!");
}

void AtlasOldMesher::updateBounds()
{
   // Reset our bounds!
   mBounds.minExtents.set(F32_MAX, F32_MAX, F32_MAX);
   mBounds.maxExtents.set(-F32_MAX, -F32_MAX, -F32_MAX);

   // First, iterate over our verts and our bounds.
   for(S32 i=0; i<mVerts.size(); i++)
   {
      S16 z;
      if(mVerts[i].special)
         z = mVerts[i].z;
      else
         z = mHeight->sampleRead(mVerts[i].pos);

      Point3F v(  mVerts[i].pos.x * mHeight->mSampleSpacing,
                  mVerts[i].pos.y * mHeight->mSampleSpacing,
                  z * mHeight->mVerticalScale);

      if(i)
      {
         mBounds.maxExtents.setMax(v);
         mBounds.minExtents.setMin(v);

         if(z < mMinZ) mMinZ = z;
         if(z > mMaxZ) mMaxZ = z;
      }
      else
      {
         mMaxZ = mMinZ = z;
         mBounds.maxExtents = mBounds.minExtents = v;
      }
   }

   // Calculate quantization for compression.
   Point3F boxExtents(mBounds.len_x() / 2.f, mBounds.len_y() / 2.f, mBounds.len_z() / 2.f);
   boxExtents.setMax(Point3F(1, 1, 1)); // Make sure we don't bottom out...

   // Use (1 << 14) values in both positive and negative
   // directions.  Wastes just under 1 bit, but the total range
   // is [-2^14, 2^14], which is 2^15+1 values.  This is good --
   // fits nicely w/ 2^N+1 dimensions of binary-triangle-tree
   // vertex locations.
   mCompressionFactor.x = F32(1<<14) / boxExtents.x;
   mCompressionFactor.y = F32(1<<14) / boxExtents.y;
   mCompressionFactor.z = F32(1<<14) / boxExtents.z;
}

void AtlasOldMesher::optimize()
{
   AssertISV(false, "This is probably broken right now - BJG");
#if 0
   // Shove everything through nvTriStrip
   SetCacheSize(24);

   // first, stripify our geometry...
   PrimitiveGroup *pg;
   U16 pgCount;

   GenerateStrips(mIndices.address(), mIndices.size(), &pg, &pgCount, AtlasOldActivationHeightfield::smDoChecks);

   // We're lazy.
   AssertISV(pgCount == 1,
      "AtlasOldMesher::optimize - Got unexpectedly complex geometry from NVTriStrip! (a)");
   AssertISV(pg->type == PT_STRIP,
      "AtlasOldMesher::optimize - Got unexpectedly complex geometry from NVTriStrip! (b)");

   // Remap indices! BJGTODO - how am I supposed to interpet the results?
   /*   PrimitiveGroup *pgRemapped;
   RemapIndices(pg, pgCount, mVerts.size(), &pgRemapped); */

   // Ok, let's suck this stuff back in.
   mIndices.clear();
   mIndices.reserve(pg->numIndices);

   for(S32 i=0; i<pg->numIndices; i++)
      mIndices[i] = pg->indices[i];

   // And clean up the memory from the stripper.
   delete[] pg;

#endif
}

void AtlasOldMesher::writeCollision(Stream *s)
{
   // First, do the binning. This is a bit gross but, hey, what can you do...
   const U32 gridSize = BIT(gAtlasColTreeDepth-1);
   const U32 gridCount = gridSize * gridSize;

   Vector<U16> bins[gridCount];

   // Track the min/max for the bins.
   S16 binsMax[gridCount];
   S16 binsMin[gridCount];

   // Clear bins.
   for(S32 i=0; i<gridCount; i++)
   {
      binsMax[i] = S16_MIN;
      binsMin[i] = S16_MAX;
   }

   // Get the size of bins (we step in x/y, not in Z).
   Point3F binSize( mBounds.len_x() / F32(gridSize), mBounds.len_y() / F32(gridSize), mBounds.len_z());

   for(S32 i=0; i<gridSize; i++)
   {
      for(S32 j=0; j<gridSize; j++)
      {
         // Figure the bounds for this bin...
         Box3F binBox;

         binBox.minExtents.x = binSize.x * i;
         binBox.minExtents.y = binSize.y * j;
         binBox.minExtents.z = mBounds.minExtents.z - 1.f;

         binBox.maxExtents.x = binSize.x * (i+1);
         binBox.maxExtents.y = binSize.y * (j+1);
         binBox.maxExtents.z = mBounds.maxExtents.z + 1.f;

         Vector<U16> &binList = bins[i * gridSize + j];

         S16 &binMin = binsMin[i * gridSize + j];
         S16 &binMax = binsMax[i * gridSize + j];

         // Now, consider all the triangles in the mesh. Note: we assume a trilist.
         for(S32 v=0; v<mIndices.size(); v+=3)
         {
            // Get the verts.
            const Vert &a = mVerts[mIndices[v+0]];
            const Vert &b = mVerts[mIndices[v+1]];
            const Vert &c = mVerts[mIndices[v+2]];

            // If it's a special, skip it, we don't want to collide with skirts.
            if(a.special ||
               b.special ||
               c.special)
               continue; // I can't stand skirts!

            // Reject anything degenerate...
            if(mIndices[v+0] == mIndices[v+1])
               continue;
            if(mIndices[v+1] == mIndices[v+2])
               continue;
            if(mIndices[v+2] == mIndices[v+0])
               continue;

            // Otherwise, we're good, so consider it for the current bin.
            const Point3F aPos = getVertPos(a);
            const Point3F bPos = getVertPos(b);
            const Point3F cPos = getVertPos(c);

            if(triBoxOverlap(binBox, aPos, bPos, cPos))
            {
               // Got a hit, add it to the list!
               binList.push_back(v);

               // Update the min/max info. This will be TOO BIG if we have a
               // very large triangle! An optimal implementation will do a clip,
               // then update the bin. This is probably ok for the moment.
               S16 hA = mHeight->sample(a.pos);
               S16 hB = mHeight->sample(b.pos);
               S16 hC = mHeight->sample(c.pos);

               if(hA > binMax) binMax = hA;
               if(hB > binMax) binMax = hB;
               if(hC > binMax) binMax = hC;

               if(hA < binMin) binMin = hA;
               if(hB < binMin) binMin = hB;
               if(hC < binMin) binMin = hC;
            }
         }

		 // Limit the triangle count to 16bits.  While primary meshes support more
		 // than that, collision meshes don't.  If we don't catch that here, we'll
		 // see raycasting issues later in Atlas.
		 AssertISV( binList.size() <= 65536,
			 "AtlasOldMesher::writeCollision - too many triangles! (>65536)  Try again with a deeper tree" );

         // Ok, we're all set for this bin...
         AssertFatal(binMin <= binMax,
            "AtlasOldMesher::writeCollision - empty bin, crap!");
      }
   }

   // Next, generate the quadtree.
   FrameAllocatorMarker qtPool;
   const U32 nodeCount = QuadTreeTracer::getNodeCount(gAtlasColTreeDepth);
   S16 *qtMin = (S16*)qtPool.alloc(sizeof(S16) * nodeCount);
   S16 *qtMax = (S16*)qtPool.alloc(sizeof(S16) * nodeCount);

   // We have to recursively generate this from the bins on up. First we copy
   // the bins from earlier, then we do our recursomatic thingummy. (It's
   // actually not recursive.)
   for(S32 i=0; i<gridSize; i++)
   {
      for(S32 j=0; j<gridSize; j++)
      {
         const U32 qtIdx = QuadTreeTracer::getNodeIndex(gAtlasColTreeDepth-1, Point2I(i,j));

         qtMin[qtIdx] = binsMin[i * gridSize + j];
         qtMax[qtIdx] = binsMax[i * gridSize + j];

         AssertFatal(qtMin[qtIdx] <= qtMax[qtIdx],
            "AtlasOldMesher::writeCollision - bad child quadtree node min/max! (negative a)");

      }
   }

   // Alright, now we go up the bins, generating from the four children of each,
   // till we hit the root.

   // For each empty level from bottom to top...
   for(S32 depth = gAtlasColTreeDepth - 2; depth >= 0; depth--)
   {
      // For each square...
      for(S32 i=0; i<BIT(depth); i++)
      for(S32 j=0; j<BIT(depth); j++)
      {
         const U32 curIdx = QuadTreeTracer::getNodeIndex(depth, Point2I(i,j));

         // For each of this square's 4 children...
         for(S32 subI=0; subI<2; subI++)
         for(S32 subJ=0; subJ<2; subJ++)
         {
            const U32 subIdx =
               QuadTreeTracer::getNodeIndex(depth+1, Point2I(i*2+subI,j*2+subJ));

            // As is the child.
            AssertFatal(qtMin[subIdx] <= qtMax[subIdx],
               "AtlasOldMesher::writeCollision - bad child quadtree node min/max! (a)");

            // Update the min and max of the parent.
            if(qtMin[subIdx] < qtMin[curIdx]) qtMin[curIdx] = qtMin[subIdx];
            if(qtMax[subIdx] > qtMax[curIdx]) qtMax[curIdx] = qtMax[subIdx];

            // Make sure we actually contain the child.
            AssertFatal(qtMin[subIdx] >= qtMin[curIdx],
               "AtlasOldMesher::writeCollision - bad quadtree child min during coltree generation!");
            AssertFatal(qtMax[subIdx] <= qtMax[curIdx],
               "AtlasOldMesher::writeCollision - bad quadtree child max during coltree generation!");

            // And that the parent is still valid.
            AssertFatal(qtMin[curIdx] <= qtMax[curIdx],
               "AtlasOldMesher::writeCollision - bad parent quadtree node min/max!");

            // As is the child.
            AssertFatal(qtMin[subIdx] <= qtMax[subIdx],
               "AtlasOldMesher::writeCollision - bad child quadtree node min/max! (b)");

         }
      }
   }

   // Wasn't that fun? Now we have a ready-to-go quadtree.

   // Now write the quadtree, in proper order
   for(S32 i=0; i<nodeCount; i++)
   {
      AssertFatal(qtMin[i] <= qtMax[i], "AtlasOldMesher::writeCollision - invalid quadtree min/max.");
      s->write(qtMin[i]);
      s->write(qtMax[i]);
   }

   s->write(U32(0xb33fd34d));

   // We have to generate...
   // ... the list of triangle offsets for each bin. (Done above!)
   // ... the triangle buffer which stores the offsets for each bin.
   ChunkTriangleBufferGenerator ctbg(gridSize);

   for(S32 i=0; i<gridSize; i++)
      for(S32 j=0; j<gridSize; j++)
         ctbg.insertBinList(Point2I(i,j), bins[i * gridSize + j]);

   // Finally, write the data out.
   ctbg.write(s);
}

void AtlasOldMesher::write(Stream *s, const S8 level, bool doWriteCollision)
{
   AssertISV(mIndices.size(), "AtlasOldMesher::write - no indices to write?!?");

   // First, figure our bounds.
   updateBounds();

   // Then, run the tristripper, get our geometry a bit more optimal.
   ///optimize();

   // We start at the appropriate position in the TOC. It's great!

   // Write min & max y values.  This can be used to reconstruct
   // the bounding box.
   s->write(mMinZ);
   s->write(mMaxZ);

   // Write a placeholder for the mesh data file pos.
   U32 currentPos = s->getPosition();
   s->write(U32(0));

   // write out the vertex data at the *end* of the file.
   s->setPosition(s->getStreamSize());
   U32 meshPos = s->getPosition();

   // Write a sentinel.
   s->write(U32(0xbeef1234));

   // Make sure the vertex buffer is not too big.
   if(mVerts.size() > 0xFFFF)
   {
      AssertISV(false,
         "AtlasOldMesher::write - too many vertices! (> 65535) Try making a deeper tree!");
   }

   // Write vertices.  All verts contain morph info.
   s->write(U16(mVerts.size()));
   for (int i = 0; i < mVerts.size(); i++)
      writeVertex(s, &mVerts[i], level);

   // Write the index buffer.
   s->write(S32(mIndices.size()));
   for(S32 i=0; i<mIndices.size(); i++)
      s->write(mIndices[i]);

   // Write the triangle count.
   s->write(U32(mIndices.size()) / 3);

   gOldChunkGenStats.outputRealTriangles += mIndices.size() / 3;

   // Now, we have to make our collision info...
   if(doWriteCollision)
   {
      s->write(U8(1));
      writeCollision(s);
   }
   else
   {
      s->write(U8(0));
   }

   // write the postscript.
   s->write(U32(0xb1e2e3f4));

   // Rewind, and fill in the mesh data file pos.
   s->setPosition(currentPos);
   s->write(meshPos);

   // Con::printf("    - %d indices, %d verts, meshOffset=%d", mIndices.size(), mVerts.size(), meshPos);
}
