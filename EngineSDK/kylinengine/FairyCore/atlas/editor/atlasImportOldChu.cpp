//-----------------------------------------------------------------------------
// Torque Game Engine Advanced Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "core/stream/fileStream.h"
#include "core/util/fourcc.h"
#include "core/util/safeDelete.h"

#include "atlas/core/atlasFile.h"
#include "atlas/resource/atlasResourceGeomTOC.h"

#include "atlas/editor/atlasDiscreteMesh.h"
#include "atlas/editor/atlasOldGenerator.h"
#include "atlas/editor/atlasImportOldChu.h"
#include "atlas/editor/atlasExport.h"

AtlasImportOldCHUHelper::AtlasImportOldCHUHelper(const char *chuFile, AtlasFile *outFile)
{
   mChuFileName = chuFile;
   mChuFile     = NULL;
   mAtlasFile   = outFile;
   dMemset(&mHeader, 0, sizeof(mHeader));
}

AtlasImportOldCHUHelper::~AtlasImportOldCHUHelper()
{
   // Close our file stream, if we have one.
   if(mChuFile)
   {
      mChuFile->close();
      delete mChuFile;
      mChuFile = NULL;
   }
}

bool AtlasImportOldCHUHelper::registerTOC()
{
   // Open the CHU file.
   mChuFile = new FileStream();
   if(!mChuFile->open(mChuFileName, Torque::FS::File::Read))
   {
      Con::errorf("AtlasImportOldCHUHelper::registerTOC - could not open chu file '%s' for read.", mChuFileName);
      return false;
   }

   mChuFile->read(&mHeader.fourCC);
   if(mHeader.fourCC != MakeFourCC('C', 'H', 'U', '3'))
   {
      Con::errorf("AtlasImportOldCHUHelper::registerTOC - invalid fourCC on CHU.");
      return false;
   }

   mChuFile->read(&mHeader.version);
   if(mHeader.version != 400)
   {
      Con::errorf("AtlasImportOldCHUHelper::registerTOC - expected CHU version 400, encountered version %d", mHeader.version);
      return false;
   }

   mChuFile->read(&mHeader.treeDepth);
   mChuFile->read(&mHeader.baseMaxError);
   mChuFile->read(&mHeader.verticalScale);
   mChuFile->read(&mHeader.leafDimensions);
   mChuFile->read(&mHeader.chunkCount);
   mChuFile->read(&mHeader.colTreeDepth);

   // Ok, header all read, so let's register the TOC.
   mTOC = new AtlasResourceGeomTOC();
   mTOC->initializeTOC(mHeader.treeDepth);
   mAtlasFile->registerTOC(mTOC);

   return true;
}

bool AtlasImportOldCHUHelper::populateChunks()
{
   // Now, copy chunks from the CHU in. First read all the stubs.
   Vector<OldCHUStub> chuStubs;
   chuStubs.reserve(mHeader.chunkCount);

   U32 totalWork = mHeader.chunkCount * 2;
   AtlasExport::updateProgress(0, totalWork, NULL);

   for(U32 i=0; i<mHeader.chunkCount; i++)
   {
      chuStubs.increment();
      OldCHUStub &ocs = chuStubs.last();

      mChuFile->read(&ocs.sentinel);

      if(ocs.sentinel != 0xDEADBEEF)
      {
         Con::errorf("importOldAtlasCHU - invalid chunk sentinel in CHU. (chunk #%d)", i);
         return false;
      }

      mChuFile->read(&ocs.label);

      for(S32 i=0; i<4; i++)
         mChuFile->read(&ocs.neighbors[i]);

      mChuFile->read(&ocs.level);
      mChuFile->read(&ocs.x);
      mChuFile->read(&ocs.y);
      mChuFile->read(&ocs.min);
      mChuFile->read(&ocs.max);
      mChuFile->read(&ocs.chunkOffset);

      AtlasExport::updateProgress(i, totalWork, NULL);
   }

   Con::printf("importOldAtlasCHU - Headers read, remapping chunks...");

   // We have to do a remap step as the headers get written to the CHU in any
   // order.
   Vector<OldCHUStub*> chuStubMap;
   chuStubMap.setSize(chuStubs.size());

   for(S32 i=0; i<chuStubs.size(); i++)
      chuStubMap[chuStubs[i].label] = &chuStubs[i];

   // Ok, remapped, now we go through and instate each chunk. We do this by
   // walking the whole quadtree, getting the corresponding old, generating
   // a chunk, and instating it into the appropriate new.

   Con::printf("importOldAtlasCHU - Importing geometry chunks...");

   Point2F terrainSize;
   terrainSize.x = terrainSize.y = mHeader.leafDimensions * F32(BIT(mHeader.treeDepth - 1));

   // Actually, they're the same index schemes which simplifies things a bit.
   U32 workCountChunk = mHeader.chunkCount;
   for(S32 level=mHeader.treeDepth-1; level>=0; level--)
   {
      Con::printf("importOldAtlasCHU -    level %d chunks...", level);
      for(S32 x=0; x<BIT(level); x++)
      {
         for(S32 y=0; y<BIT(level); y++)
         {
            U32 ni = mTOC->getNodeIndex(level, Point2I(y,x)); // Note x/y flip! -- BJG
            AtlasResourceGeomStub *args = mTOC->getStub(level, Point2I(x,y));
            OldCHUStub *oldStub = chuStubMap[ni];

            workCountChunk++;
            AtlasExport::updateProgress(workCountChunk, totalWork, NULL);

            // Ok, we have our stubs, so let's do some load and process.
            mChuFile->setPosition(oldStub->chunkOffset);
            OldCHUChunk curChunk;
            if(!curChunk.read(*mChuFile, mTOC->getNodeCount(mHeader.colTreeDepth), 
               BIT(mHeader.colTreeDepth-1) * BIT(mHeader.colTreeDepth-1)))
            {
               Con::errorf("importOldAtlasCHU - error reading chunk #%d (%d,%d@%d)!", ni, level, x, y);
               return false;
            }

            // We have a loaded chunk, so let's convert it to our new chunk format.

            // We generate our own collision data, so we can skip the existing
            // data in the .chu.

            // Grab the geometry, stuff it into an ADM, and install the ADM
            // in a chunk.
            AtlasDiscreteMesh adm;
            adm.mOwnsData     = false;
            adm.mHasMorphData = true;
            adm.mIndexCount   = curChunk.indexCount;
            adm.mIndex        = curChunk.index;
            adm.mVertexCount  = curChunk.vertCount;

            // Generate our vertex data.
            F32 chunkSize = mHeader.leafDimensions * BIT((mHeader.treeDepth - 1) - oldStub->level);

            curChunk.generateVertexArrays(
               mHeader.verticalScale, chunkSize,
               Point2F(chunkSize * oldStub->x, chunkSize * oldStub->y), 
               terrainSize);

            adm.mPos            = curChunk.pos;
            adm.mPosMorphOffset = curChunk.morph;
            adm.mTex            = curChunk.texCoord;

            // Set some defaults for morph and normal data.
            adm.mTexMorphOffset = new Point2F[curChunk.vertCount];
            adm.mNormal         = new Point3F[curChunk.vertCount];
            for(S32 i=0; i<curChunk.vertCount; i++)
            {
               adm.mTexMorphOffset[i].set(0,0);
               adm.mNormal[i].set(0, 0, 1);
            }

            // Then instate the chunk and potentially update the stub.
            AtlasGeomChunk *agc = new AtlasGeomChunk;
            agc->copyFromDiscreteMesh(&adm);

            // Update bounds from children as well, if appropriate.
            args->mBounds = adm.calcBounds();
            if(level < mHeader.treeDepth - 1)
            {
               const U8 offsets[4][2] =
               {
                  {0,0},
                  {0,1},
                  {1,1},
                  {1,0},
               };

               for(S32 j=0; j<4; j++)
               {
                  AtlasResourceGeomStub *childArgs = 
                     mTOC->getStub(level + 1,
                     Point2I((x * 2) + offsets[j][0], (y * 2) + offsets[j][1]));

                  args->mBounds.minExtents.setMin( childArgs->mBounds.minExtents );
                  args->mBounds.maxExtents.setMax( childArgs->mBounds.maxExtents );

                  AssertFatal(args->mBounds.isContained(childArgs->mBounds), 
                     "importOldAtlasCHU - child not contained by parent!");

               }
            }

            // Make sure we match stub and chunk bounds.
            agc->mBounds = args->mBounds;

            // Update collision if needed. We MUST do this after other bound
            // update operations, as collision lookup info is encoded based 
            // on the bounds.
            if(level == mHeader.treeDepth - 1)
               agc->generateCollision();

            // Instate the chunk
            mTOC->instateNewChunk(args, agc, true);

            // And clean up once we're done, minimize memory consumption.
            args->purge();
         }
      }
   }

   // Wait for everything to finish serializing, then return.
   mAtlasFile->waitForPendingWrites();

   return true;
}

ConsoleFunction(importOldAtlasCHU, void, 3, 3, "(oldCHU, newAtlasFile)")
{
   // Kick up an AtlasFile.
   AtlasFile af;

   // And create our import helper..
   AtlasImportOldCHUHelper aioCh(argv[1], &af);

   // Let it setup the TOCs...
   if(!aioCh.registerTOC())
   {
      Con::errorf("importOldAtlasCHU - TOC registration failed, aborting import...");
      return;
   }

   // Ok, now let's create the Atlas File.
   if(!af.createNew(argv[2]))
   {
      Con::errorf("importOldAtlasCHU - could not create new Atlas file '%s'", argv[2]);
      return;
   }

   Con::printf("importOldAtlasCHU - created new Atlas file '%s'", argv[2]);

   // Start loader threads..
   af.startLoaderThreads();

   Con::printf("importOldAtlasCHU - Atlas file initialized, converting...");

   //  And let the helper import our chunks.
   if(!aioCh.populateChunks())
   {
      Con::errorf("importOldAtlasCHU - chunk population failed, aborting import...");
      return;
   }

   //  All done!
   Con::printf("importOldAtlasCHU - import OK!");

}

//-----------------------------------------------------------------------------

AtlasImportOldCHUHelper::OldCHUChunk::OldCHUChunk()
{
   pos = NULL;
   morph = NULL;
   texCoord = NULL;
   verts = NULL;
   index = NULL;
   colIndicesOffsets = NULL;
   colIndexBuffer = NULL;
}

AtlasImportOldCHUHelper::OldCHUChunk::~OldCHUChunk()
{
   SAFE_DELETE_ARRAY(verts);
   SAFE_DELETE_ARRAY(index);
   SAFE_DELETE_ARRAY(colIndicesOffsets);
   SAFE_DELETE_ARRAY(colIndexBuffer);
   SAFE_DELETE_ARRAY(pos);
   SAFE_DELETE_ARRAY(morph);
   SAFE_DELETE_ARRAY(texCoord);
}

bool AtlasImportOldCHUHelper::OldCHUChunk::read( Stream &chuFs, U32 colTreeCount, U32 colTreeGridCount )
{
   chuFs.read(&sentinel);
   if(sentinel != 0xbeef1234)
   {
      Con::errorf("OldCHUChunk::read - invalid sentinel(1)!");
      return false;
   }

   chuFs.read(&vertCount);
   verts = new Vert[vertCount];
   for(S32 i=0; i<vertCount; i++)
   {
      chuFs.read(&verts[i].x);
      chuFs.read(&verts[i].y);
      chuFs.read(&verts[i].z);
      chuFs.read(&verts[i].morph);
   }

   chuFs.read(&indexCount);
   index = new U16[indexCount];
   for(S32 i=0; i<indexCount; i++)
      chuFs.read(index+i);

   chuFs.read(&triCount);

   chuFs.read(&colDataFlag);

   if(colDataFlag==1)
   {
      // Got collision data, read it in.

      colTreeNodes = new ColTreeNode[colTreeCount];
      for(S32 i=0; i<colTreeCount; i++)
      {
         chuFs.read(&colTreeNodes[i].min);
         chuFs.read(&colTreeNodes[i].max);
      }

      chuFs.read(&sent2);
      if(sent2 != 0xb33fd34d)
      {
         Con::errorf("OldCHUChunk::read - invalid sentinel(2, collision)!");
         return false;
      }

      colIndicesOffsets = new U16[colTreeGridCount];
      for(S32 i=0; i<colTreeGridCount; i++)
         chuFs.read(&colIndicesOffsets[i]);

      chuFs.read(&bufferSize);
      colIndexBuffer = new U16[bufferSize];
      for(S32 i=0; i<bufferSize; i++)
         chuFs.read(&colIndexBuffer[i]);
   }
   else if(colDataFlag == 0)
   {
      // Do nothing...
   }
   else
   {
      Con::errorf("OldCHUChunk::read - unexpected value for colDataFlag (%d, not 0 or 1)!", colDataFlag);
      return false;
   }

   // Check footer sentinel.
   chuFs.read(&post); 
   if(post != 0xb1e2e3f4)
   {
      Con::errorf("OldCHUChunk::read - invalid sentinel(3)!");
      return false;
   }

   return true;
}

void AtlasImportOldCHUHelper::OldCHUChunk::generateVertexArrays( F32 verticalScale, F32 chunkSize, Point2F chunkOffset, Point2F terrainSize )
{
   SAFE_DELETE_ARRAY(pos);
   SAFE_DELETE_ARRAY(morph);
   SAFE_DELETE_ARRAY(texCoord);

   pos      = new Point3F[vertCount];
   morph    = new Point3F[vertCount];
   texCoord = new Point2F[vertCount];

   for(S32 i=0; i<vertCount; i++)
   {
      // Decompress pos.
      pos[i].set(
         ((F32(verts[i].x) / F32(1 << 14)) * 0.5 + 0.5) * chunkSize + chunkOffset.x,
         ((F32(verts[i].y) / F32(1 << 14)) * 0.5 + 0.5) * chunkSize + chunkOffset.y,
         F32(verts[i].z) * verticalScale
         );

      // We need to generate our texcoords in context of the whole terrain.
      // 
      // Take our corrected position (above) and cook into "our" section
      // of the terrain, based on the terrain bounds.
      texCoord[i].set(pos[i].y / terrainSize.y, pos[i].x / terrainSize.x);

      // Decompress and expand morph.
      morph[i].set(0, 0, F32(verts[i].morph) * verticalScale);
   }
}
