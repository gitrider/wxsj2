//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "core/stream/fileStream.h"
#include "core/resManager.h"
#include "core/strings/stringFunctions.h"

#include "math/mRect.h"
#include "core/util/fourcc.h"
#include "atlas/editor/atlasExport.h"
#include "atlas/editor/atlasOldGenerator.h"


bool AtlasOldActivationHeightfield::smDoChecks = false;
bool AtlasOldActivationHeightfield::smGenerateDebugData = false;

AtlasGenStats gOldChunkGenStats;

//------------------------------------------------------------------------

AtlasOldActivationHeightfield::AtlasOldActivationHeightfield(const U32 sizeLog2, const F32 sampleSpacing, const F32 heightScale)
   :  AtlasOldHeightfield(sizeLog2, sampleSpacing, heightScale)
{
   // Allocate the activation array and null it out.
   mActivationLevels = new S8[realSize() * realSize()];
   //dMemset(mActivationLevels, -1, sizeof(S8) * realSize() * realSize());
   for(S32 i=0; i<realSize(); i++)
      for(S32 j=0; j<realSize(); j++)
         mActivationLevels[i*realSize() + j] = -1;
}

AtlasOldActivationHeightfield::~AtlasOldActivationHeightfield()
{
   delete[] mActivationLevels ;
}

S8 AtlasOldActivationHeightfield::getLevel(Point2I pos)
{
   return mActivationLevels[indexOf(pos)];
}

void AtlasOldActivationHeightfield::setLevel(Point2I pos, S8 level)
{
   mActivationLevels[indexOf(pos)] = level;
}

void AtlasOldActivationHeightfield::activate(Point2I pos, S8 level)
{
   S8 currentLevel = getLevel(pos);
   if (level > currentLevel) 
   {
      if (currentLevel == -1) gOldChunkGenStats.outputVertices++;
      setLevel(pos, level);
   }
}

void AtlasOldActivationHeightfield::update(F32 baseMaxError, Point2I a, Point2I r, Point2I l)
{
   // Compute the coordinates of this triangle's base vertex.
   const Point2I d = l - r;

   if (mAbs(d.x) <= 1 && mAbs(d.y) <= 1) 
   {
      // We've reached the base level.  There's no base
      // vertex to update, and no child triangles to
      // recurse to.
      return;
   }

   // base vert is midway between left and right verts.
   const Point2I b = r + d/2;

   F32 error = mFabs((sampleRead(b) - (sampleRead(l) + sampleRead(r)) / 2) * mVerticalScale);
   AssertISV(error >= 0, "AtlasOldActivationHeightfield::update - encountered invalid error metric!");
   if (error >= baseMaxError) 
   {
      // Compute the mesh level above which this vertex
      // needs to be included in LOD meshes.
      const S32 activationLevel = mClamp((S32) mFloor((error / baseMaxError) + 0.5f), 0, 0xFF);

      // Force the base vert to at least this activation level.
      activate(b, activationLevel);
   }

   // Recurse to child triangles.
   update(baseMaxError, b, a, r);   // base, apex, right
   update(baseMaxError, b, l, a);   // base, left, apex
}

AtlasOldActivationHeightfield::HeightType AtlasOldActivationHeightfield::heightQuery(S8 level, Point2I pos, Point2I a, Point2I r, Point2I l)
{
   // If the query is on one of our verts, return that vert's height.
   if (pos == a
      || pos == r
      || pos == l)
   {
      return sampleRead(pos);
   }

   // a = apex, r = right, l = left

   // Make sure we're inside the bounding box of this tri.
   RectI rect(a, a);
   rect.unionRects(RectI(r, r));
   rect.unionRects(RectI(l, l));

   AssertFatal(rect.pointInRect(pos), "Point outside of tri bounds!");

   // Compute the coordinates of this triangle's base vertex.
   Point2I d = l - r;
   if (mAbs(d.x) <= 1 && mAbs(d.y) <= 1) 
   {
      // We've reached the base level.  This is an error condition; we should
      // have gotten a successful test earlier.

      // assert(0);
      AssertFatal(false, "AtlasOldActivationHeightfield::heightQuery - hit base of heightfield.");

      return sampleRead(a);
   }

   // base vert is midway between left and right verts.
   Point2I b = (r + l) / 2;

   // compute the length of a side edge.
   const F32 edgeLengthSquared = d.lenSquared() / 2.f;

   // calculate barycentric co-ordinates w/r/t the right and left edges.
   Point2F s;

   s.x = ((pos.x - a.x) * (r.x - a.x) + (pos.y - a.y) * (r.y - a.y)) / edgeLengthSquared;
   s.y = ((pos.x - a.x) * (l.x - a.x) + (pos.y - a.y) * (l.y - a.y)) / edgeLengthSquared;

   AssertFatal(s.x >= 0.f && s.x <= 1.f, "Bad s.x");
   AssertFatal(s.y >= 0.f && s.y <= 1.f, "Bad s.y");
   //AssertFatal( (s.x + s.y > 0.99f) && (s.x + s.y < 1.1f), "Bad s sum!");


   //  Check to see if we need to recurse.
   if (getLevel(b) >= level)
   {
      // The mesh is more tesselated at the desired LOD.  Recurse.
      if (s.x >= s.y) 
      {
         // Query is in right child triangle.
         return heightQuery(level, pos, b, a, r);   // base, apex, right
      } 
      else 
      {
         // Query is in left child triangle.
         return heightQuery(level, pos, b, l, a);   // base, left, apex
      }
   }

   // Get height data so we can interpolate as needed.
   F64 ay     = F64(sampleRead(a));
   F64 dRight = F64(sampleRead(r)) - ay;
   F64 dLeft  = F64(sampleRead(l)) - ay;

   // This triangle is as far as the desired LOD goes.  Compute the
   // query's height on the triangle.
   return (HeightType) (mFloorD(ay + s.x * dRight + s.y * dLeft + 0.5));
}

AtlasOldActivationHeightfield::HeightType AtlasOldActivationHeightfield::getHeightAtLOD(Point2I pos, S8 level)
{
   if (pos.y > pos.x) 
   {
      // Query in SW quadrant.
      return heightQuery(level, 
                              pos,
                              Point2I(0,      size()), 
                              Point2I(size(), size()),
                              Point2I(0,      0)
                         );
   }
   else 
   {
      // Query in NW quadrant
      return heightQuery(level, 
                              pos, 
                              Point2I(size(),   0), 
                              Point2I(0,        0),
                              Point2I(size(),   size())
                         );
   }
}

void AtlasOldActivationHeightfield::propagateActivationLevel(Point2I c, S8 level, S8 targetLevel)
{
   AssertFatal(c.x >= 0 && c.x < size(), "bad x");
   AssertFatal(c.y >= 0 && c.y < size(), "bad y");

   const S32 halfSize    = 1 << level;
   const S32 quarterSize = halfSize >> 1;

   // First, check to see if we're at the appropriate level...
   if (level > targetLevel) 
   {
      for (int j = 0; j < 2; j++)
      {
         for (int i = 0; i < 2; i++)
         {
            propagateActivationLevel(
               c + Point2I(
                     -quarterSize + halfSize * i, 
                     -quarterSize + halfSize * j),
               level - 1, targetLevel
               );
         }
      }

      // Only process at the right level, so let's bail.
      return;
   }

   // We're at the target level.  Do the propagation on this square.

   if (level > 0)
   {
      // Propagate corner verts to edge verts.
      S8 lev = getLevel(c + Point2I( quarterSize, -quarterSize)); // ne
      activate(c + Point2I(halfSize,  0), lev);
      activate(c + Point2I(0, -halfSize), lev);

      lev    = getLevel(c + Point2I(-quarterSize, -quarterSize)); // nw
      activate(c + Point2I(0, -halfSize), lev);
      activate(c + Point2I(-halfSize, 0), lev);

      lev    = getLevel(c + Point2I(-quarterSize,  quarterSize)); // sw
      activate(c + Point2I(-halfSize, 0), lev);
      activate(c + Point2I(0, halfSize), lev);

      lev    = getLevel(c + Point2I( quarterSize,  quarterSize)); // se
      activate(c + Point2I(0, halfSize), lev);
      activate(c + Point2I(halfSize, 0), lev);
   }

   // Propagate edge verts to center.
   activate(c, getLevel(c + Point2I(halfSize,  0)));
   activate(c, getLevel(c + Point2I(0, -halfSize)));
   activate(c, getLevel(c + Point2I(0,  halfSize)));
   activate(c, getLevel(c + Point2I(-halfSize, 0)));
}

S8 AtlasOldActivationHeightfield::checkPropagation(Point2I c, S8 level)
{
   const S32 halfSize    = 1 << level;
   const S32 quarterSize = halfSize >> 1;

   S32 max_act = -1;

   // cne = ne child, cnw = nw child, etc.
   S32 cne, cnw, csw, cse;
   cne = cnw = csw = cse = -1;

   if (level > 0) 
   {
      // Recurse to children.
      cne = checkPropagation(c + Point2I( quarterSize, -quarterSize), level - 1);
      cnw = checkPropagation(c + Point2I(-quarterSize, -quarterSize), level - 1);
      csw = checkPropagation(c + Point2I(-quarterSize,  quarterSize), level - 1);
      cse = checkPropagation(c + Point2I( quarterSize,  quarterSize), level - 1);
   }

   // ee == east edge, en = north edge, etc
   S32 ee = getLevel(c + Point2I(halfSize,  0));
   S32 en = getLevel(c + Point2I(0, -halfSize));
   S32 ew = getLevel(c + Point2I(-halfSize, 0));
   S32 es = getLevel(c + Point2I(0,  halfSize));

   if (level > 0) 
   {
      // Check child verts against edge verts.
      if (cne > ee || cse > ee) 
         Con::warnf("AtlasOldActivationHeightfield::checkPropagation - error! ee! lev = %d, cx = %d, cz = %d, alev = %d\n", level, c.x, c.y, ee);	//xxxxx

      if (cne > en || cnw > en) 
         Con::warnf("AtlasOldActivationHeightfield::checkPropagation - error! en! lev = %d, cx = %d, cz = %d, alev = %d\n", level, c.x, c.y, en);	//xxxxx

      if (cnw > ew || csw > ew)
         Con::warnf("AtlasOldActivationHeightfield::checkPropagation - error! ew! lev = %d, cx = %d, cz = %d, alev = %d\n", level, c.x, c.y, ew);	//xxxxx

      if (csw > es || cse > es)
         Con::warnf("AtlasOldActivationHeightfield::checkPropagation - error! es! lev = %d, cx = %d, cz = %d, alev = %d\n", level, c.x, c.y, es);	//xxxxx
   }

   // Check level of edge verts against center.
   S8 cLvl = getLevel(c);
   max_act = getMax(max_act, ee);
   max_act = getMax(max_act, en);
   max_act = getMax(max_act, es);
   max_act = getMax(max_act, ew);

   if (max_act > cLvl) 
      Con::warnf("AtlasOldActivationHeightfield::checkPropagation - error! center! lev = %d, cx = %d, cz = %d, alev = %d, max_act = %d ee = %d en = %d ew = %d es = %d\n", level, c.x, c.y, cLvl, max_act, ee, en, ew, es);

   return (S32)getMax((S32)max_act, (S32)cLvl);
}

bool AtlasOldActivationHeightfield::dumpActivationLevels(const char *filename)
{
   FileStream fs;

   if(!gResourceManager->openFileForWrite(fs, filename, Torque::FS::File::ReadWrite))
   {
      Con::errorf("AtlasOldActivationHeightfield::dumpActivationLevels - failed to open output file!");
      return false;
   }

   for(S32 i=0; i<size(); i++)
      for(S32 j=0; j<size(); j++)
         fs.write(getLevel(Point2I(i, j)));

   fs.close();

   return true;
}

bool AtlasOldActivationHeightfield::dumpHeightAtLOD(S8 level, const char *filename)
{
   FileStream fs;

   if(!gResourceManager->openFileForWrite(fs, filename, Torque::FS::File::ReadWrite))
   {
      Con::errorf("AtlasOldActivationHeightfield::dumpHeightAtLOD - failed to open output file!");
      return false;
   }

   // Write out heights at desired LOD.
   for(S32 i=0; i<size(); i++)
   {
      for(S32 j=0; j<size(); j++)
      {
         const Point2I pos(i,j);

         HeightType h = getHeightAtLOD(pos, level);
         fs.write(h);
      }
   }

   fs.close();

   return true;
}

void AtlasOldActivationHeightfield::generateChunkFile(Stream *s, S8 treeDepth, F32 baseMaxError)
{
   Con::printf("Generating chunked geometry (depth=%d, baseMaxError=%f)", treeDepth, baseMaxError);

   // Set up some global properties.
   mRootChunkLevel = treeDepth - 1;
   
   // Reset stats.
   gOldChunkGenStats.clear();
   gOldChunkGenStats.inputVertices = size() * size();

   Con::printf("   o Calculating activation levels...");

   AtlasExport::updateProgress(0, 2, "[GEOM] 4. Calculating activation levels...");

   // Run a view-independent L-K style BTT update on the heightfield, to generate
   // error and activation_level values for each element.
   update(baseMaxError,
            Point2I(0,          size()),
            Point2I(size(),     size()),
            Point2I(0,          0)); // sw half of the square

   AtlasExport::updateProgress(1, 2, NULL);

   update(baseMaxError,
            Point2I(size(), 0),
            Point2I(0,      0), 
            Point2I(size(), size()));	// ne half of the square

   AtlasExport::updateProgress(2, 2, NULL);

   Con::printf("      - done.");

   Con::printf("   o Propagating activation levels...");

   // Propagate the activation_level values of verts to their
   // parent verts, quadtree LOD style.  Gives same result as
   // L-K.
   AtlasExport::updateProgress(0, mFieldShift*2, "[GEOM] 5. Propagating activation levels...");
   for (int i = 0; i < mFieldShift; i++) 
   {
      AtlasExport::updateProgress(i*2, mFieldShift*2, NULL);

      propagateActivationLevel(Point2I(size() >> 1, size() >> 1), mFieldShift - 1, i);

      AtlasExport::updateProgress(i*2+1, mFieldShift*2, NULL);

      propagateActivationLevel(Point2I(size() >> 1, size() >> 1), mFieldShift - 1, i);
   }

   Con::printf("      - done.");

   // Make sure we're awesome.
   if(AtlasOldActivationHeightfield::smDoChecks)
   {
      Con::printf("   o [debug] Checking activation levels...");
      checkPropagation(Point2I(size() >> 1, size() >> 1), mFieldShiftMinusOne);
      Con::printf("      - done.");

      // Also, dump raw heightfields at different LODs.
      Con::printf("   o [debug] Dumping internal representations...");

      for (int i = 0; i < mFieldShift; i++) 
      {
         const char  *fn = avar("demo/heightDumpLOD%d.raw", i);
         Con::printf("      - Dumping heights at level %d (%s)...", i, fn);
         dumpHeightAtLOD(i, fn);
      }

      Con::printf("      - Dumping activation levels (demo/activationDump.raw)");
      dumpActivationLevels("demo/activationDump.raw");

      Con::printf("      - done!");
   }

   Con::printf("   o Writing file header...");

   AtlasExport::updateProgress(0, 3, "[GEOM] 6. Serializing headers...");

   // Write a .chu header for the output file.
   const U32 fourCC = MakeFourCC('C', 'H', 'U', '3');
   s->write(fourCC);

   const S16 version = 400; // File format version.
   s->write(version);

   s->write(S16(treeDepth)); // Depth of the chunk quadtree.

   s->write(baseMaxError);
   s->write(mVerticalScale);

   // x/z dimension, in meters, of highest LOD chunks.
   s->write((1 << (mFieldShift - (treeDepth - 1))) * mSampleSpacing);

   // Chunk count.  Fully populated quadtree.
   const U32 chunkCount = 0x55555555 & ((1 << (treeDepth*2)) - 1);
   s->write(chunkCount);

   // Finally, what's our coltree depth?
   s->write(U32(gAtlasColTreeDepth));

   Con::printf("      - done.");

   Con::printf("   o Generating empty TOC...");

   AtlasExport::updateProgress(1, 2, NULL);

   // Make space for our chunk table-of-contents.  Fixed-size
   // chunk headers will go in this space; the vertex/index data
   // for chunks gets appended to the end of the stream.
   generateEmptyTOC(s, treeDepth);

   Con::printf("      - done.");

   Con::printf("   o Generating meshes...");

   AtlasExport::updateProgress(0, chunkCount, "[GEOM] 7. Serializing chunks...");

   // Write out the node data for the entire chunk tree.
   generateNodeData(s, Point2I(0, 0), mFieldShift, mRootChunkLevel);

   Con::printf("      - done.");

   // Generate a little statistics report on the generated data.
   gOldChunkGenStats.outputSize = s->getPosition();

   F32 vertsPerChunk = F32(gOldChunkGenStats.outputVertices) / F32(gOldChunkGenStats.outputChunks);

   gOldChunkGenStats.printChunkStats();
   Con::printf("========================================");
   Con::printf("                chunks: %10d", gOldChunkGenStats.outputChunks);
   Con::printf("           input verts: %10d", gOldChunkGenStats.inputVertices);
   Con::printf("          output verts: %10d", gOldChunkGenStats.outputVertices);
   Con::printf("");
   Con::printf("       avg verts/chunk: %10.0f", vertsPerChunk);

   if (vertsPerChunk < 1000) 
   {
      Con::printf("NOTE: verts/chunk is low; for higher poly throughput consider setting the tree depth to %d and reprocessing.",
         treeDepth - 1);
   }
   else if (vertsPerChunk > 5000) 
   {
      Con::printf("NOTE: verts/chunk is high; for smoother framerate consider setting the tree depth to %d and reprocessing.",
         treeDepth + 1);
   }

   Con::printf("          output bytes: %10d", gOldChunkGenStats.outputSize);
   Con::printf("      bytes/input vert: %10.2f", gOldChunkGenStats.outputSize / (F32) gOldChunkGenStats.inputVertices);
   Con::printf("     bytes/output vert: %10.2f", gOldChunkGenStats.outputSize / (F32) gOldChunkGenStats.outputVertices);

   Con::printf("        real triangles: %10d", gOldChunkGenStats.outputRealTriangles);

}

// Manually synced!!!  (@@ should use a fixed-size struct, to be
// safer, although that ruins endian safety.)  If you change the chunk
// header contents, you must keep this constant in sync.  In DEBUG
// builds, there's an assert that should catch discrepancies, but be
// careful.
const U32 CHUNK_HEADER_BYTES = 4 + 4*4 + 1 + 2 + 2 + 2*2 + 4 + 4;

void AtlasOldActivationHeightfield::generateEmptyTOC(Stream* s, S32 treeDepth)
{
   // We're gonna be cheesy here, let's assume a fixed header size of N bytes.
   U8	buff[CHUNK_HEADER_BYTES];	// dummy chunk header.
   dMemset(buff, 0xFF, sizeof(buff));

   U32 startPos = s->getPosition();

   // BJGTODO - put this in a helper function. OR SUFFER.
   const U32 chunkCount = 0x55555555 & ((1 << (treeDepth*2)) - 1);

   for (S32 i=0; i < chunkCount; i++)
      s->write(sizeof(buff), buff);

   // Rewind, so caller can start writing real TOC data.
   s->setPosition(startPos);
}

void AtlasOldActivationHeightfield::generateNodeData(Stream* s, Point2I pos, S32 logSize, S32 level, HeightType min, HeightType max)
{
   // Note our starting position.
   U32 startPos = s->getPosition();

   const S32 size     = (1 << logSize);
   const S32 halfSize = size >> 1;
   const Point2I c(pos.x + halfSize, pos.y + halfSize);

   gOldChunkGenStats.outputChunks++;
   const U32 chunkCount = 0x55555555 & ((1 << ((mRootChunkLevel+1)*2)) - 1);
   AtlasExport::updateProgress(gOldChunkGenStats.outputChunks, chunkCount, NULL);

   // Determine our label, so that we can generate references... Label is based
   // off of our origin point.
   const S32 chunkLabel = nodeIndex(c);

   // Write a sentinel.
   s->write(U32(0xDEADBEEF));

   // Ok, write our header information.
   s->write(chunkLabel);

   //Con::printf("   + chunk (%d, %d) @ %d, label=%d, offset=%d", c.x, c.y, level, chunkLabel, startPos);

   // Write the labels of our neighbors.
   s->write(S32(nodeIndex(c + Point2I(size,  0)    ))); // EAST
   s->write(S32(nodeIndex(c + Point2I(0,     -size)))); // NORTH
   s->write(S32(nodeIndex(c + Point2I(-size, 0)    ))); // WEST
   s->write(S32(nodeIndex(c + Point2I(0,     size) ))); // SOUTH

   // Write this chunk's co-ordinates.
   const S32 lodLevel = mRootChunkLevel - level;

   AssertISV(lodLevel >= 0 && lodLevel < 0xFF, "AtlasOldActivationHeightfield::generateNodeData - out of bound LOD level for chunk!");

   s->write(U8(lodLevel));
   s->write(S16(pos.x >> logSize));
   s->write(S16(pos.y >> logSize));

   // Alright, onto the mesh generation!
   AtlasOldMesher *cm = new AtlasOldMesher(this, logSize);

   // Make sure our corner verts are activated on this level.
   activate(pos + Point2I(0,    0),    level);
   activate(pos + Point2I(size, 0),    level);
   activate(pos + Point2I(0,    size), level);
   activate(pos + Point2I(size, size), level);

   // Since we're gonna run this all through a tristripper when we're done, we
   // don't give a fig about optimizing the mesh as long as it's normalized.

   // This makes our life MUCH easier than Ulrich's. :)

   // Generate data for our edge skirts.  Go counterclockwise around
   // the outside (ensures correct winding).
   generateSkirt(cm, c + Point2I( halfSize,  halfSize), c + Point2I( halfSize, -halfSize), level); // east
   generateSkirt(cm, c + Point2I( halfSize, -halfSize), c + Point2I(-halfSize, -halfSize), level); // north
   generateSkirt(cm, c + Point2I(-halfSize, -halfSize), c + Point2I(-halfSize,  halfSize), level); // west
   generateSkirt(cm, c + Point2I(-halfSize,  halfSize), c + Point2I( halfSize,  halfSize), level); // south 

   // Generate the mesh.
   generateBlock(cm, level, logSize, c);

   // <fig> <-- we keep this one.

   // Tristrip, quantize, and write the mesh data... Only  write collision for
   // leaf nodes.
   cm->write(s, level, (level==0) );

   // While we're at it, collect some more statistics...
   gOldChunkGenStats.noteChunkStats(level, cm->getTriCount());

   HeightType b=cm->mMaxZ, a=cm->mMinZ;

   // Check min and max bounds.
   if(cm->mMaxZ > max)
      Con::errorf("AtlasOldActivationHeightfield::generateNodeData - Max exceeded! May have paging issues!");

   if(cm->mMinZ < min)
      Con::errorf("AtlasOldActivationHeightfield::generateNodeData - Min exceeded! May have paging issues!");

   delete cm;

   // Check we're ok on header size.
   AssertISV(
      (s->getPosition() - startPos) == CHUNK_HEADER_BYTES,
      "AtlasOldActivationHeightfield::generateNodeData - Invalid header size!");

   // Finally, recurse to our child chunks!
   if(level)
   {
      generateNodeData(s, pos,                                 logSize-1, level-1, a, b); // nw
      generateNodeData(s, pos + Point2I(halfSize,  0),         logSize-1, level-1, a, b); // ne
      generateNodeData(s, pos + Point2I(0,         halfSize),  logSize-1, level-1, a, b); // sw
      generateNodeData(s, pos + Point2I(halfSize,  halfSize),  logSize-1, level-1, a, b); // se
   }

   // A figgy saved is a figgy earned. -- Benjamin Figgylin.
}


struct AtlasOldGenState
{
   Point2I mBuffer[2];	// coords of the last two vertices emitted by the generate_ functions.
   S8 activation_level;	// for determining whether a vertex is enabled in the block we're working on
   S8 ptr;              // indexes my_buffer.
   S8 previous_level;	// for keeping track of level changes during recursion.

   AtlasOldGenState(S8 activationLevel)
   {
      ptr = 0;
      previous_level = 0;
      activation_level = activationLevel;

      for (S32 i = 0; i < 2; i++)
         mBuffer[i>>1].set(S32_MIN, S32_MAX);
   }

   bool check(Point2I pos)
      // Returns true if the specified vertex is in my_buffer.
   {
      return (mBuffer[0] == pos || mBuffer[1] == pos);
   }

   void set(Point2I pos)
      // Sets the current my_buffer entry to pos
   {
      mBuffer[ptr] = pos;
   }
};

Vector<U16> gOldTriStrip;

void AtlasOldActivationHeightfield::generateBlock(AtlasOldMesher *cm, const S8 activationLevel, S32 logSize, Point2I c)
// Generate the mesh for the specified square with the given center.
// This is paraphrased directly out of Lindstrom et al, SIGGRAPH '96.
// It generates a square mesh by walking counterclockwise around four
// triangular quadrants.  The resulting mesh is composed of a single
// continuous triangle strip, with a few corners turned via degenerate
// tris where necessary.
{
   // quadrant corner coordinates.
   const S32 halfSize = 1 << (logSize - 1);
   const S32 q[4][2] = {
      { c.x + halfSize, c.y + halfSize },	// se
      { c.x + halfSize, c.y - halfSize },	// ne
      { c.x - halfSize, c.y - halfSize },	// nw
      { c.x - halfSize, c.y + halfSize },	// sw
   };

   // Init state for generating mesh.
   AtlasOldGenState	state(activationLevel);

   // Kick off with the first vert!
   gOldTriStrip.clear();
   gOldTriStrip.push_back(cm->getVertIndex(Point2I(q[0][0], q[0][1])));
   state.set(Point2I(q[0][0], q[0][1]));

   {for (int i = 0; i < 4; i++) 
   {
      if ((state.previous_level & 1) == 0) 
      {
         // tulrich: turn a corner?
         state.ptr ^= 1;
      } 
      else 
      {
         // tulrich: jump via degenerate?
         gOldTriStrip.push_back(cm->getVertIndex(state.mBuffer[1 - state.ptr]));	// or, emit vertex(last - 1);
      }

      // Initial vertex of quadrant.
      gOldTriStrip.push_back(cm->getVertIndex(Point2I(q[i][0], q[i][1])));
      state.set(Point2I(q[i][0], q[i][1]));
      state.previous_level = 2 * logSize + 1;

      generateQuadrant(
         cm,
         state,
         Point2I(q[i][0], q[i][1]),
         c,
         Point2I(q[(i+1)&3][0], q[(i+1)&3][1]),	// q[i][r]
         2 * logSize
         );
   }}

   if (!state.check(Point2I(q[0][0], q[0][1]))) 
      // finish off the strip.  @@ may not be necessary?
      gOldTriStrip.push_back(cm->getVertIndex(Point2I(q[0][0], q[0][1])));

   // Now, we have to convert the tristrip into a list. (No, this does not
   // make a lot of sense at this point.)
   bool flippyThing = false;
   for(S32 i=0; i<gOldTriStrip.size() - 2; i++)
   {
      if(flippyThing)
         cm->emitTri(gOldTriStrip[i+2], gOldTriStrip[i+1], gOldTriStrip[i+0]);
      else
         cm->emitTri(gOldTriStrip[i+0], gOldTriStrip[i+1], gOldTriStrip[i+2]);

      flippyThing = !flippyThing;
   }
}

void	AtlasOldActivationHeightfield::generateQuadrant(AtlasOldMesher *cm, AtlasOldGenState &s, Point2I l, Point2I t, Point2I r, S32 recursionLevel)
// Auxiliary function for generate_block().  Generates a mesh from a
// triangular quadrant of a square heightfield block.  Paraphrased
// directly out of Lindstrom et al, SIGGRAPH '96.
{
   if (recursionLevel <= 0) return;

   if (getLevel(t) >= s.activation_level) 
   {
      // Find base vertex.
      Point2I b = (l + r) / 2;

      generateQuadrant(cm, s, l, b, t, recursionLevel - 1); // left half of quadrant

      if (!s.check(t)) 
      {
         if ((recursionLevel + s.previous_level) & 1) 
         {
            s.ptr ^= 1;
         } 
         else 
         {
            gOldTriStrip.push_back(cm->getVertIndex(s.mBuffer[1 - s.ptr]));
         }

         gOldTriStrip.push_back(cm->getVertIndex(t));
         s.set(t);
         s.previous_level = recursionLevel;
      }

      generateQuadrant(cm, s, t, b, r, recursionLevel - 1); // left half of quadrant
   }
}

void AtlasOldActivationHeightfield::generateSkirt(AtlasOldMesher *cm, const Point2I a, const Point2I b, const S8 level)
{

   // We're going to write a list of vertices comprising the
   // edge.
   //
   // We're also going to write the index (in this list) of the
   // midpoint vertex of the edge, so the renderer can join
   // t-junctions.

   // Scan the edge, looking for the minimum height at each vert,
   // taking into account the full LOD mesh, plus all meshes up to
   // two levels above our own.

   Vector<HeightType> minVerts;

   // We're going to be stepping, figure out delta and magnitude.
   Point2I delta;
   S32 steps = -1;

   if (a.x < b.x) 
   {
      AssertISV(a.y == b.y, "AtlasOldActivationHeightfield::generateSkirt - got a crooked line! (A)");

      delta.set(1, 0);
      steps = b.x - a.x + 1;
   }
   else if (a.x > b.x) 
   {
      AssertISV(a.y == b.y, "AtlasOldActivationHeightfield::generateSkirt - got a crooked line! (B)");

      delta.set(-1, 0);
      steps = a.x - b.x + 1;
   } 
   else if (a.y < b.y) 
   {
      AssertISV(a.x == b.x, "AtlasOldActivationHeightfield::generateSkirt - got a crooked line! (C)");

      delta.set(0, 1);
      steps = b.y - a.y + 1;
   }
   else if (a.y > b.y) 
   {
      AssertISV(a.x == b.x, "AtlasOldActivationHeightfield::generateSkirt - got a crooked line! (D)");

      delta.set(0, -1);
      steps = a.y - b.y + 1;
   } 
   else 
   {
      AssertISV(false, "AtlasOldActivationHeightfield::generateSkirt - got a bad line!");
   }

   AssertISV(steps > 0, "Bad step count!");

   HeightType currentMin = sampleRead(a);

   for (S32 i = 0, x = a.x, y = a.y; i < steps; i++, x += delta.x, y += delta.y)
   {
      const Point2I curPos(x,y);

      HeightType h = sampleRead(curPos);
      if(h < currentMin)
         currentMin = h;

      if (getLevel(curPos) >= level) 
      {
         // This is an active vert at this level of detail.

         // TODO: activation level & chunk level are consistent
         // with each other, but they go in the unintuitive
         // direction: level 0 is the *highest* (i.e. most
         // detailed) "level of detail".  Should reverse this to be
         // less confusing.

         // Check height of lower LODs.  The rule is, the renderer
         // allows a certain level of difference in LOD between
         // neighboring chunks, so we want to ensure we can cover
         // cracks that encompass gaps between our mesh and the
         // minimum LOD of our neighbor.
         //
         // The more levels of difference allowed, the less
         // constrained the renderer can be about paging.  On the
         // other hand, the more levels of difference allowed, the
         // more conservative we have to be with our skirts.
         //
         // Also, every edge is *interior* to some minimum LOD
         // chunk.  So we don't have to look any higher than that
         // LOD.

         S32 majorCoord = a.x;
         if (!delta.y) 
            majorCoord = a.y;

         // lod of the least-detailed chunk bordering this edge.
         const S32 minEdgeLod = minimumEdgeLod(majorCoord);

         // A parameter in the renderer as well -- keep in sync.
         const S32 MAXIMUM_ALLOWED_NEIGHBOR_DIFFERENCE = 2;	

         S32 levelDiff = getMin(S32(minEdgeLod + 1), S32(mRootChunkLevel)) - level;
         levelDiff = getMin(S32(levelDiff), MAXIMUM_ALLOWED_NEIGHBOR_DIFFERENCE);

         for (S32 lod = level; lod <= level + levelDiff; lod++)
         {
            HeightType lodHeight = getHeightAtLOD(curPos, lod);
            if(lodHeight < currentMin) currentMin = lodHeight;
         }

         // Remember the minimum height of the edge for this
         // segment.

         if (currentMin > -32768)
            currentMin -= 1;	// be slightly conservative here.

         minVerts.push_back(currentMin);
         currentMin = sampleRead(curPos);
      }
   }

   // Generate a "skirt" which runs around our outside edges and
   // makes sure we don't leave any visible cracks between our
   // (simplified) edges and the true shape of the mesh along our
   // edges.

   AtlasOldMesher::VertIndex stripA=0, stripB=0;

   S32 vIdx = 0;
   for (S32 i = 0, x = a.x, y = a.y; i < steps; i++, x += delta.x, y += delta.y)
   {
      const Point2I curPos(x,y);

      if (getLevel(curPos) >= level)
      {
         // Put down a vertex which is at the minimum of our
         // previous and next segments.
         HeightType minHeight = minVerts[vIdx];
         if (minVerts.size() > vIdx + 1)
            minHeight = getMin(minHeight, minVerts[vIdx + 1]);

         // Ok, now we have to emit some tris for our skirt. This is a bit hairy
         // as this code originally did a tristrip; we converted to be a little
         // friendlier to NVTriStrip. (Woot.)
         
         AtlasOldMesher::VertIndex newA = cm->getVertIndex(curPos);
         AtlasOldMesher::VertIndex newB = cm->addSpecialVert(curPos, minHeight);

         // If stripA and stripB are 0, we are the first one, so let's just chill..
         if(stripA != 0 && stripB != 0)
         {
            // Nope, we can link this stuff up!
            cm->emitTri(stripA, stripB, newA);
            cm->emitTri(stripB, newB, newA);
         }

         // Update stripA and stripB
         stripA = newA;
         stripB = newB;

         vIdx++;
      }
   }

   // Ok, we're all done!
}

ConsoleFunction(atlasOldGenerateChunkFileFromRaw16, void, 8, 8, "(srcFile, size, squareSize, vertScale, "
                "destFile, error, treeDepth) Create a chunk file from a raw 16 bit heightfield of the "
                "given square size and vertical scale.")
{
   // Parse parameters.
   const char *srcFile     = argv[1];
   S32 size          = dAtoi(argv[2]);
   F32 squareSize    = dAtof(argv[3]);
   F32 vertScale     = dAtof(argv[4]);
   const char *destFile    = argv[5];
   F32 error         = dAtof(argv[6]);
   S32 treeDepth     = dAtoi(argv[7]);

   AtlasOldActivationHeightfield cahf(getBinLog2(size), squareSize, vertScale);

   // Load it!
   Stream *s = gResourceManager->openStream(srcFile);
   if(!s)
   {
      Con::errorf("generateChunkFileFromRaw16 - failed to load '%s'!", srcFile);
      return;
   }

   cahf.loadRawU16(*s);
   gResourceManager->closeStream(s);

   // Cool, it's loaded, try generating something.
   FileStream fs;

   // Wipe it first.
   if(!gResourceManager->openFileForWrite(fs, destFile, Torque::FS::File::Write))
   {
      Con::errorf("generateChunkFileFromRaw16 - failed to open output file!");
      return;
   }

   fs.close();

   // Now do it for reals.
   if(!gResourceManager->openFileForWrite(fs, destFile, Torque::FS::File::ReadWrite))
   {
      Con::errorf("generateChunkFileFromRaw16 - failed to open output file!");
      return;
   }

   cahf.generateChunkFile(&fs, treeDepth, error);

   fs.close();
}
