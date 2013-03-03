//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "atlas/resource/atlasResourceTexTOC.h"
#include "atlas/resource/atlasTexChunk.h"
#include "core/util/safeDelete.h"
#include "gfx/gfxDevice.h"

AtlasTexChunk::AtlasTexChunk()
{
   mFormat = FormatPNG;
   bitmap = NULL;
   dds = NULL;   
   layerCount = 0;
}

AtlasTexChunk::~AtlasTexChunk()
{
   SAFE_DELETE_ARRAY(bitmap);
   SAFE_DELETE_ARRAY(dds);
}

void AtlasTexChunk::read(Stream *s)
{
   AssertISV(s, "AtlasTexChunk::read - no stream present!");

   // Read the format.
   U8 f;
   s->read(&f);
   mFormat = (TexFormat)f;

   AssertFatal(!(bitmap || dds), "AtlasTexChunk::read - already have a bitmap/dds loaded!");

   // Let's check for the layerCount, or default to 1 if we're on a TOC
   // with version prior to 150.
   if(((AtlasResourceTexTOC*)mOwningTOC)->getFormatVersion() < 150)
      layerCount = 1;
   else
      s->read(&layerCount);

   // Set up arrays, if needed.
   switch(f)
   {
   case FormatJPEG:
   case FormatPNG:
      bitmap = new GBitmap[layerCount];
      break;

   case FormatDDS:
      dds = new DDSFile[layerCount];
      break;

   default:
      AssertISV(false, "AtlasTexChunk::read - unknown format type!");
      break;
   }

   for(S32 layer=0; layer<layerCount; layer++)
   {
      bool readSuccess = false;

      switch(f)
      {
      case FormatJPEG:
         readSuccess = bitmap[layer].readBitmap( "jpeg", *s );
         break;

      case FormatPNG:
         readSuccess = bitmap[layer].readBitmap( "png", *s );
         break;

      case FormatDDS:
         readSuccess = dds[layer].read(*s);
         break;

      default:
         AssertISV(false, "AtlasTexChunk::read - unknown format!");
         break;
      }

      // For runtime we have to convert endianness.
      if(GFXDevice::getDeviceVector()->size())
      {
         if(bitmap[layer].getBytesPerPixel() == 4)
            bitmap[layer].swizzle(GFX->getDeviceSwizzle32());
         else if(bitmap[layer].getBytesPerPixel() == 3)
            bitmap[layer].swizzle(GFX->getDeviceSwizzle24());
      }

      AssertISV(readSuccess, "AtlasTexChunk::read - failed to read an image layer!");
   }
}

void AtlasTexChunk::write(Stream *s)
{
   AssertISV(s, "AtlasTexChunk::write - no stream present!");

   // Write our format, then a texture-specific encoding.
   U8 f = mFormat;
   s->write(f);

   // Layer count!
   s->write(layerCount);

   bool writeSuccess = false;

   for(S32 i=0; i<layerCount; i++)
   {
      switch(f)
      {
      case FormatJPEG:
         AssertISV(bitmap, "AtlasTexChunk::write - (JPEG) no bitmap is present!");
         writeSuccess = bitmap[i].writeBitmap( "jpeg", *s );
         break;

      case FormatPNG:
         AssertISV(bitmap, "AtlasTexChunk::write - (PNG) no bitmap is present!");
         writeSuccess = bitmap[i].writeBitmap( "png", *s );
         break;

      case FormatDDS:
         AssertISV(dds, "AtlasTexChunk::write - (DDS) no DDS is present!");
#ifndef TORQUE_OS_MAC
         writeDDS(s);
         writeSuccess = true;
#else
         AssertISV(false, "AtlasTexChunk::write - (DDS) DDS writing is not available on the Mac!");
         writeSuccess = false;
#endif
         break;

      default:
         AssertISV(false, "AtlasTexChunk::write - unknown format!");
         break;
      }

      AssertISV(writeSuccess, "AtlasTexChunk::write - failed writing bitmap!");
   }
}

void AtlasTexChunk::process()
{
}

void AtlasTexChunk::generate(AtlasChunk *genericChildren[4])
{
   // Cast things so they're convenient.
   AtlasTexChunk *children[4];
   for(S32 i=0; i<4; i++)
      children[i] = (AtlasTexChunk*)genericChildren[i];

   // Combine the four children, downsampling them as needed.
   mFormat = children[0]->mFormat;

   if(isBitmapTexFormat(mFormat))
   {
      // Do it in the manner most befitting a GBitmap.

      // Prepare our own GBitmaps to write into.
      U32 tileSize = children[0]->bitmap->getWidth();

      layerCount = children[0]->layerCount;
      bitmap = new GBitmap[children[0]->layerCount];
      
      for(S32 i=0; i<layerCount; i++)
         bitmap[i].allocateBitmap(tileSize, tileSize, true, children[0]->bitmap->getFormat());

      // Now, bitblt the children into it. Start at topleft, CW. We cheat and use one of their mips.
      for(S32 i=0; i<4; i++)
      {
         for(S32 j=0; j<layerCount; j++)
         {
            // Make sure we've got mips, then copy from the second one.
            GBitmap *childBitmap = &children[i]->bitmap[j];

            // Swap back before we muck with it. We'll have to do some
            // cleanup when we have in-engine editing.
            if(GFXDevice::getDeviceVector()->size())
            {
               if(childBitmap->getBytesPerPixel() == 4)
                  childBitmap->swizzle(GFX->getDeviceSwizzle32());
               else if(childBitmap->getBytesPerPixel() == 3)
                  childBitmap->swizzle(GFX->getDeviceSwizzle24());
            }

            childBitmap->extrudeMipLevels();

            const U8 offsets[4][2] =
            {
               {0,0},
               {1,0},
               {0,1},
               {1,1},
            };

            const S32 xOffset = offsets[i][0];
            const S32 yOffset = offsets[i][1];

            U32 halfTile = (tileSize >> 1);
            AssertFatal(childBitmap->getHeight(1) == halfTile, 
               "AtlasTexChunk::generate - tile size mismatch!");

            for(S32 y=0; y<halfTile; y++)
            {
               // Copy a row at a time.
               dMemcpy(
                  bitmap[j].getAddress(xOffset * halfTile, yOffset * halfTile + y),
                  childBitmap->getAddress(0, y, 1), 
                  childBitmap->getBytesPerPixel() * halfTile);
            }

            // And we're done.
         }
      }

      // Yay, all done!
   }
   else
   {
      // Do it in the manner most befitting a DDS. (ie, make D3D do it for now.)
      AssertISV(false, "AtlasTexChunk::generate - DDS LOD generation not currently supported.");
   }
}

AtlasTexChunk *AtlasTexChunk::generateCopy(S32 reformat)
{
   AtlasTexChunk *atc = new AtlasTexChunk;
   
   // Support changing the format as we go.
   if(reformat > -1)
      atc->mFormat = (TexFormat)reformat;
   else
      atc->mFormat = mFormat;

   atc->layerCount = layerCount;

   // We have to copy our layers...
   if(bitmap)
   {
      atc->bitmap = new GBitmap[layerCount];
      for(S32 layer=0; layer<layerCount; layer++)
      {
         if(GFXDevice::getDeviceVector()->size())
         {
            if(bitmap[layer].getBytesPerPixel() == 4)
               bitmap[layer].swizzle(GFX->getDeviceSwizzle32());
            else if(bitmap[layer].getBytesPerPixel() == 3)
               bitmap[layer].swizzle(GFX->getDeviceSwizzle24());
         }

         constructInPlace< GBitmap >( &atc->bitmap[layer], (const GBitmap*)&bitmap[ layer ] );
      }
   }

   if(dds)
   {
      atc->dds = new DDSFile[layerCount];
      for(S32 layer=0; layer<layerCount; layer++)
         atc->dds[layer] = dds[layer];
   }

   return atc;
}

U32 AtlasTexChunk::getHeadSentinel()
{
   return MakeFourCC('T', 'e', 'x', 'H');
}

U32 AtlasTexChunk::getTailSentinel()
{
   return MakeFourCC('T', 'e', 'x', 'T');
}
