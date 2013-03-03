//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "core/stream/fileStream.h"
#include "core/strings/stringFunctions.h"

#include "atlas/editor/atlasOldGenerator.h"
#include "math/mRect.h"
#include "atlas/resource/atlasResourceConfigTOC.h"
#include "atlas/resource/atlasResourceTexTOC.h"
#include "atlas/resource/atlasResourceGeomTOC.h"

#include "atlas/editor/atlasExport.h"
#include "atlas/editor/atlasExportInterfaces.h"
#include "atlas/editor/atlasImportOldCHU.h"
#include "atlas/editor/atlasImageImport.h"

//-----------------------------------------------------------------------------

void (*AtlasExport::smProgressCallback)(S32 current, S32 total, const char *newTitle) = 0;

void AtlasExport::updateProgress( S32 current, S32 total, const char *newTitle )
{
   if(!smProgressCallback)
      return;

   smProgressCallback(current, total, newTitle);
}

//-----------------------------------------------------------------------------
// Assorted status callback glue code.

static ITerrainInfo *gCallbackTI = NULL;
static void consoleCallbackTI(ConsoleLogEntry::Level level, const char *consoleLine)
{
   gCallbackTI->handleLog(consoleLine);
}

static void progressCallbackTI(S32 current, S32 total, const char *newTitle)
{
   gCallbackTI->updateProgress(current, total, newTitle);
}

//-----------------------------------------------------------------------------

bool generateChuFile(  ITerrainInfo * info, char * chuOutBuff, F32 error, S32 treeDepth  )
{
   // Figure out what our terrain scale will need to be, given we're crunching
   // into S16's.
   const F32 terrMin = info->getMinHeight();
   const F32 terrMax = info->getMaxHeight();

   // We want terrMin to be S16_MIN and terrMax to be S16_MAX. So we calculate
   // an offset to bump our float values to, then divide by range.

   // f(x) = S16((x + terrOffset) * terrScale);

   // Specifically, f(avg(terrMin,terrMax)) = 0. So terrOffset = -avg(terrMin,terrMax).
   const F32 terrOffset = -(terrMin + terrMax) / 2.0f;

   // And f(terrMax) = S16_MAX. So terrScale = ...
   const F32 terrScale = F32(S16_MAX) / F32(terrMax + terrOffset);

   // Now we can export the .chu.
   AtlasOldActivationHeightfield cahf(getBinLog2(info->getWidth()), info->getSampleSpacing(), 1.0 / terrScale);

   // Now, populate it with real info.
   const S32 h = info->getHeight(), w = info->getWidth();

   info->updateProgress(0, h*w, "[GEOM] 2. Converting heightfield...");

   for(S32 i=0; i<h; i++)
   {
      for(S32 j=0; j<w; j++)
      {
         // Convert and store.
         cahf.sample(Point2I(i,j)) = (S32)mFloor((info->getAltitude(i,j) + terrOffset) * terrScale);

         // Horrible hack to make sure our borders are good.
         cahf.sample(Point2I(i+1,j+1)) = cahf.sample(Point2I(i,j));
         cahf.sample(Point2I(i+1,j+0)) = cahf.sample(Point2I(i,j));
         cahf.sample(Point2I(i+0,j+1)) = cahf.sample(Point2I(i,j));
      }
      info->updateProgress(i*w, h*w, NULL);
   }

   // Now write out to some crazy file.
   {
      FileStream fs;

      if(!fs.open(chuOutBuff, Torque::FS::File::ReadWrite))
      {
         Con::errorf("atlasGenerateOldChunkFileFromAtlasInterface - failed to open output file!");
         return false;
      }

      info->updateProgress(0, 0, "[GEOM] 3. Generating intermediate geometry data...");
      cahf.generateChunkFile(&fs, treeDepth, error);

      fs.close();
   }

   return true;
}

bool atlasGenerateFromAtlasInterface(const char *outFile, ITerrainInfo *info, IAtlasImageImportSource *texSource, IAtlasImageImportSource *opacitySource)
{
   // Given we launch from within a DLL... hack some init for things we just have to use.
   if(StringTable == NULL)
   {
      // Initialize some utilities that we'll need.
      FrameAllocator::init(5 * 1024 * 1024);
      _StringTable::create();

      // Hook in logging...
      gCallbackTI = info;
      Con::addConsumer(consoleCallbackTI);

      // Set the progress callback, too.
      AtlasExport::smProgressCallback = progressCallbackTI;
   }

   // Check some assumptions.

   // Is it a square terrain?
   if(info->getHeight() != info->getWidth())
   {
      Con::errorf("atlasGenerateOldChunkFileFromAtlasInterface - non-square terrain.");
      return false;
   }

   // Let's do a little precalculation and ensure we have TOC depths as well
   // as a reasonable error metric.

   // Let's try to get a 256px tile size for the texture TOC(s).
   Point2I texSize;
   texSource->getExtents(texSize.x, texSize.y);
   S32 texTocDepth = getBinLog2(texSize.x / 256) + 1;

   // Determine a tile size & a TOC depth to achieve it, for geometry. If it's
   // a mondo terrain try to do slightly bigger chunks.
   S32 geomTileSize = (info->getWidth() > 1024 ? 128 : 64);
   S32 geomTocDepth = getBinLog2(info->getWidth() / geomTileSize) + 1;

   // Let's also assume a clipmap size of 512 and make sure that our
   // geometry TOC is at most 2 less than the size of the clipmap so we
   // won't have any issues fitting the clipmap in for rendering.
   const S32 clipMapSize = 512;
   S32 clipMapHeight = getBinLog2(texSize.x) - getBinLog2(clipMapSize) + 1; // Subtract the mips.

   // Sanity check our clipmap height. :)
   if(texSize.x < clipMapSize)
      clipMapHeight = 1;

   // Don't even bother if we have some tiny clipmap...
   if(clipMapHeight > 2)
   {
      if(geomTocDepth < (clipMapHeight - 2))
      {
         // Ooh, we aren't deep enough.
         geomTocDepth = clipMapHeight - 2;
      }
   }

   // Ok, now we need to figure our geometry error threshold.
   F32 geomErrorThreshold = 2.0; // Random value that might be good.

   Con::printf("AUTOGEN'ed params: Geom TOC depth = %d, Tex TOC depth = %d, geom err = %f", 
      geomTocDepth, texTocDepth, geomErrorThreshold);

   // Using the provided terrain info interface, generate a chunk file.
   info->startProgress("[GEOM] 1. Allocating memory...");

   // We have to generate a working file...
   char chuOutBuff[4096];
   dStrcpy(chuOutBuff, outFile);
   dStrcat(chuOutBuff, ".temp.chu");

   // First set up our heightfield, on which all our calculations reside.
   if(!generateChuFile(info, chuOutBuff, geomErrorThreshold, geomTocDepth))
      return false;

   // ------------
   //  Begin Atlas file generation...
   info->updateProgress(0, 0, "[ATLAS] 1. Creating Atlas file...");
   AtlasFile af;

   // Initialize our TOCs...
   //    - Config TOC
   AtlasResourceConfigTOC *arctoc = new AtlasResourceConfigTOC();
   arctoc->initialize(2);
   af.registerTOC(arctoc);

   //    - Texture TOC (just one, diffuse map)
   AtlasImageImporter aii(&af, texSource, texTocDepth);
   aii.registerTOC();

   AtlasImageImporter *opacityImporter = NULL;
   if(opacitySource)
   {
      //    - Opacity map in slot 1, with depth of 1 for now.
      opacityImporter = new AtlasImageImporter(&af, opacitySource, 1);
      opacityImporter->registerTOC();
   }

   // Init the CHU conversion...
   AtlasImportOldCHUHelper aioCh(chuOutBuff, &af);
   aioCh.registerTOC();

   // Create atlas file and kick off the threads...
   af.createNew(outFile);
   af.startLoaderThreads();

   // Now we need to populate the TOCs...

   // First, create a config TOC for a straight up diffuse mapped terrain.
   AtlasConfigChunk *acc = new AtlasConfigChunk;

   if(!opacitySource)
   {
      acc->setEntry("type", "unique");
      acc->setEntry("textureSlot", "0");
   }
   else
   {
      acc->setEntry("type", "blender");
      acc->setEntry("opacityMapSlot", "1");
      acc->setEntry("shadowMapSlot", "0");
      acc->setEntry("virtualTexSize", "32768");

      // Fudge up a source image list.
      acc->setEntry("sourceImageCount", avar("%d", opacitySource->getNumberChannels()));

      for(S32 i=0; i<opacitySource->getNumberChannels(); i++)
      {
         char buff[2][256];
         dSprintf(buff[0], 256, "sourceImage%d", i);

         if(info->hasMaterialFilenames() && info->getMaterialName(i))
            dSprintf(buff[1], 256, info->getMaterialName(i));
         else
            dSprintf(buff[1], 256, "source%d", i);

         acc->setEntry(buff[0], buff[1]);
      }
   }

   arctoc->addConfig("schema", acc);

   // Second, populate the texture TOC by snarfing data from the info interface.
   info->updateProgress(1, 3, "[ATLAS] 3. Importing texture chunks...");
   aii.populateChunks();

   // Do opacity maps, if present.
   if(opacityImporter)
      opacityImporter->populateChunks();

   // Third, import the .chu into a geometry TOC.
   info->updateProgress(2, 3, "[ATLAS] 4. Importing geometry chunks...");
   aioCh.populateChunks();

   info->endProgress();
   return true;
}
