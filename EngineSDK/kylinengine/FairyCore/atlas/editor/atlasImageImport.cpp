//-----------------------------------------------------------------------------
// Torque Game Engine Advanced Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "console/console.h"
#include "gfx/bitmap/gBitmap.h"

#include "atlas/core/atlasFile.h"
#include "atlas/resource/atlasResourceTexTOC.h"

#include "atlas/editor/atlasImageImport.h"
#include "atlas/editor/atlasOldGenerator.h"
#include "atlas/editor/atlasExport.h"

AtlasImageImporter::AtlasImageImporter(AtlasFile *atlasFile, IAtlasImageImportSource *source, U32 treeDepth)
{
   mTreeDepth = treeDepth;
   mSource    = source;
   mAtlasFile = atlasFile;
   mTOC       = NULL;
}

void AtlasImageImporter::registerTOC()
{
   mTOC = new AtlasResourceTexTOC;
   mTOC->initializeTOC(mTreeDepth);
   mAtlasFile->registerTOC(mTOC);
}

void AtlasImageImporter::populateChunks()
{
   // Figure out what size each chunk is gonna be.
   Point2I extents;
   mSource->getExtents(extents.x, extents.y);

   U32 leafCount = BIT(mTreeDepth - 1);
   U32 tileSize = extents.x / leafCount;

   for(S32 x=0; x<leafCount; x++)
   {
      for(S32 y=0; y<leafCount; y++)
      {
         AtlasExport::updateProgress(x*leafCount+y, leafCount*leafCount, NULL);

         AtlasTexChunk *atc = new AtlasTexChunk;

         if(mSource->getNumberChannels() == 0)
         {
            atc->mFormat = AtlasTexChunk::FormatJPEG;
            atc->layerCount = 1;

            // Construct the bitmap for this tile.
            GBitmap *gb = new GBitmap[1];
            gb->allocateBitmap(tileSize, tileSize);

            ColorI col;

            for(S32 leafX=0; leafX<tileSize; leafX++)
            {
               for(S32 leafY=0; leafY<tileSize; leafY++)
               {
                  mSource->getColor(leafX + x*tileSize, leafY + y*tileSize, col.red, col.green, col.blue, col.alpha);
                  gb->setColor(leafY, leafX, col);
               }
            }

            atc->bitmap = gb;
         }
         else
         {
            // It's a multilayer image, so set things up a bit differently
            atc->mFormat = AtlasTexChunk::FormatPNG;

            const S32 channelCount = mSource->getNumberChannels();
            atc->layerCount = (channelCount + 3) / 4;

            // Set up the chunk's bitmaps.
            atc->bitmap = new GBitmap[atc->layerCount];
            for(S32 i=0; i<atc->layerCount; i++)
               atc->bitmap[i].allocateBitmap(tileSize, tileSize, false, GFXFormatR8G8B8A8);

            // Ok, now sample each pixel & write out into the bitmaps.
            U8 *alphas = new U8[channelCount];
            ColorI c;

            for(S32 leafX=0; leafX<tileSize; leafX++)
            {
               for(S32 leafY=0; leafY<tileSize; leafY++)
               {
                  mSource->sampleChannels(leafX + x*tileSize, leafY + y*tileSize, alphas);

                  // Now store the alphas into their respective bitmaps.
                  for(S32 alphaStep=0; alphaStep < channelCount; alphaStep+=4)
                  {
                     c.red   = alphas[alphaStep+0];
                     c.green = (alphaStep + 1 < channelCount) ? alphas[alphaStep+1] : 0;
                     c.blue  = (alphaStep + 2 < channelCount) ? alphas[alphaStep+2] : 0;
                     c.alpha = (alphaStep + 3 < channelCount) ? alphas[alphaStep+3] : 0;

                     atc->bitmap[alphaStep/4].setColor(leafX, leafY, c);
                  }            
               }
            }
         }

         // Get the stub.
         AtlasResourceTexStub *tStub = mTOC->getStub(mTreeDepth-1, Point2I(x,y));

         // Store, then purge, the chunk.
         mTOC->instateNewChunk(tStub, atc, true);
         tStub->purge();
      }
   }

   // Generate all the intermediate levels...
   mTOC->generate(RectI(0, 0, leafCount, leafCount));

   // And block on write before we return.
   mAtlasFile->waitForPendingWrites();
}
