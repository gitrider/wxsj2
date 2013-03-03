//-----------------------------------------------------------------------------
// Torque Game Engine Advanced Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "core/stream/fileStream.h"
#include "core/util/fourcc.h"
#include "core/util/safeDelete.h"

#include "atlas/core/atlasFile.h"
#include "atlas/resource/atlasResourceTexTOC.h"
#include "atlas/editor/atlasImportOldTqt.h"

AtlasImportOldTQTHelper::AtlasImportOldTQTHelper( const char *tqtFile, AtlasFile *outFile )
{
   mTQTFileName = tqtFile;
   mTQTFile     = NULL;
   mAtlasFile   = outFile;
   dMemset(&mHeader, 0, sizeof(mHeader));
}

AtlasImportOldTQTHelper::~AtlasImportOldTQTHelper()
{
   // Close our file stream, if we have one.
   if(mTQTFile)
   {
      mTQTFile->close();
      delete mTQTFile;
      mTQTFile = NULL;
   }
}

void AtlasImportOldTQTHelper::registerTOC()
{
   // Let's open our input files.
   mTQTFile = new FileStream();
   if(!mTQTFile->open(mTQTFileName, Torque::FS::File::Read))
   {
      Con::errorf("AtlasImportOldTQTHelper::registerTOC - could not open TQT file '%s' for read.", mTQTFileName);
      return;
   }

   Con::printf("AtlasImportOldTQTHelper::registerTOC - opened '%s', processing...", mTQTFileName);

   // Now do the TQT header.
   mTQTFile->read(&mHeader.fourCC);
   if(mHeader.fourCC != MakeFourCC('t', 'q', 't', 0))
   {
      Con::warnf("AtlasImportOldTQTHelper::registerTOC - encountered an invalid TQT fourcc.");
   }

   mTQTFile->read(&mHeader.version);
   mTQTFile->read(&mHeader.treeDepth);
   mTQTFile->read(&mHeader.tileSize);

   // Setup the TOCs.
   mTOC = new AtlasResourceTexTOC();
   mTOC->initializeTOC(mHeader.treeDepth);
   mAtlasFile->registerTOC(mTOC);
}

void AtlasImportOldTQTHelper::populateChunks()
{
#ifndef TORQUE_OS_MAC
   // Ok, time for some importing!
   // TQT is easy, as we have a list of offsets, and each chunk has length + a DDS.
   Vector<U32> tqtOffsets;
   U32 tqtCount = mTOC->getNodeCount(mHeader.treeDepth);
   tqtOffsets.setSize(tqtCount);

   for(S32 i=0; i<tqtCount; i++)
      mTQTFile->read(&tqtOffsets[i]);

   Con::printf("AtlasImportOldTQTHelper::populateChunks - instating textures...");

   // Same deal as before - walk the quadtree, instate new data.
   for(S32 i=0; i<mHeader.treeDepth; i++)
   {
      for(S32 x=0; x<BIT(i); x++)
      {
         for(S32 y=0; y<BIT(i); y++)
         {
            U32 ni = mTOC->getNodeIndex(i, Point2I(y,x));
            AtlasResourceTexStub *arts = mTOC->getStub(i, Point2I(x,y));
            U32 offset = tqtOffsets[ni];

            // Ok, we have our stubs, so let's do some load and process.
            mTQTFile->setPosition(offset);

            // Read size, then data into a buffer.
            U32 chunkSize;
            mTQTFile->read(&chunkSize);

            U8 *texBuffer = new U8[chunkSize];
            mTQTFile->read(chunkSize, texBuffer);

            // We have a loaded chunk, so let's convert it to our new chunk format.
            AtlasTexChunk *agt = new AtlasTexChunk;
            agt->mFormat = AtlasTexChunk::FormatPNG;
            agt->bitmap = AtlasTexChunk::loadDDSIntoGBitmap( texBuffer, chunkSize);
            agt->layerCount = 1;

            delete[] texBuffer;

            mTOC->instateNewChunk(arts, agt, true);

            // And kill it so we save memory.
            arts->purge();
         }
      }

      Con::printf("  - wrote all of level %d (%dx%d)", i, BIT(i), BIT(i));
   }

   // Wait for everything to finish serializing, then return.
   mAtlasFile->waitForPendingWrites();
#endif
}

#ifndef TORQUE_OS_MAC
ConsoleFunction(importOldAtlasTQT, void, 3, 3, "(oldTQT, newAtlasFile)")
{
   // Allocate a new AtlasFile and create TOCs with the settings from our
   // source headers.
   AtlasFile af;

   AtlasImportOldTQTHelper aioTh(argv[1], &af);

   aioTh.registerTOC();

   if(!af.createNew(argv[2]))
   {
      Con::errorf("importOldAtlasTQT - could not create new Atlas file '%s'", argv[2]);
      return;
   }

   Con::printf("importOldAtlasTQT - created new Atlas file '%s'...", argv[2]);

   af.startLoaderThreads();

   Con::printf("importOldAtlasTQT - initialized Atlas file, converting...");

   aioTh.populateChunks();

   Con::printf("importOldAtlasTQT - Done");
}
#endif
