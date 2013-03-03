//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "atlas/runtime/atlasClipMapImageSource.h"

AtlasClipMapImageSource::AtlasClipMapImageSource()
{
   mTOC = NULL;
   mCacheOrigin.set(0,0);
   mUnique = false;
}

AtlasClipMapImageSource::~AtlasClipMapImageSource()
{
   SAFE_DELETE(mTOC);
}

void AtlasClipMapImageSource::setTOC( AtlasResourceTexTOC *toc )
{
   mTOC = new AtlasInstanceTexTOC();
   mTOC->initializeTOC(toc);

   // Request the root chunk, so we can fulfill chunk size queries.

   // Immediately load and process.
   mTOC->getResourceTOC()->immediateLoad(mTOC->getResourceStub(mTOC->getStub(0)), AtlasTOC::RootLoad);

   // Note the request so that we won't unload later.
   mTOC->requestLoad(mTOC->getStub(0), AtlasTOC::RootLoad, 1.f);
}

const U32 AtlasClipMapImageSource::getMipLevelCount() const
{
   // We need to return log2(total toc texture size).
   //[rene, 08/06/2008] Plus one! as this is the count, not the max index
   return getBinLog2(mTOC->getTextureChunkSize()) + mTOC->getTreeDepth();
}

void AtlasClipMapImageSource::setInterestCenter( const Point2I& origin, const U32 radius )
{
   // Walk the quadtree, topmost unloaded stuff has highest priority.
   // We also need to issue unloads for stuff around the old origin...
   // So let's iterate over everything within the bounds of our radius of BOTH
   // origins. If it's within the originRect, keep it, otherwise cancel it.

   RectI oldOriginTexelRect, newOriginTexelRect;
   RectI oldOriginTOCRect, newOriginTOCRect;
   RectI unionRect;

   for(S32 i=mTOC->getTreeDepth()-1; i>=0; i--)
   {
      const U32 shift = mTOC->getTreeDepth() - i - 1;

      oldOriginTexelRect.point = mCacheOrigin;
      oldOriginTexelRect.point.x >>= shift;
      oldOriginTexelRect.point.y >>= shift;
      oldOriginTexelRect.extent.set(0,0);
      S32 rad = (S32)radius;
      oldOriginTexelRect.inset(-rad, -rad);

      newOriginTexelRect.point = origin;
      newOriginTexelRect.point.x >>= shift;
      newOriginTexelRect.point.y >>= shift;
      newOriginTexelRect.extent.set(0,0);
      newOriginTexelRect.inset(-rad, -rad);

      convertToTOCRect(i, oldOriginTexelRect, oldOriginTOCRect);
      convertToTOCRect(i, newOriginTexelRect, newOriginTOCRect);

      unionRect = oldOriginTOCRect;
      unionRect.unionRects(newOriginTOCRect);

      // Clamp our update region so we're not walking stuff that is out of range.
      const S32 xStart = mClamp(unionRect.point.x, 0, BIT(i));
      const S32 xEnd   = mClamp(unionRect.point.x + unionRect.extent.x, 0, BIT(i));

      const S32 yStart = mClamp(unionRect.point.y, 0, BIT(i));
      const S32 yEnd   = mClamp(unionRect.point.y + unionRect.extent.y, 0, BIT(i));

      for(S32 x=xStart; x<xEnd; x++)
      {
         for(S32 y=yStart; y<yEnd; y++)
         {
            const Point2I pos(x, y);
            AtlasInstanceTexStub *aits = mTOC->getStub(i, pos);

            // Note we weight our loads by depth - the closest to the root
            // goes first.
            if(newOriginTOCRect.pointInRect(pos))
               mTOC->requestLoad(aits, AtlasTOC::NormalPriority, 
                  F32(shift + 1) / F32(mTOC->getTreeDepth() + 1));
            else
            {
               mTOC->cancelLoadRequest(aits, AtlasTOC::NormalPriority);
            }
         }
      }
   }

   // Now we can update the cache origin.
   mCacheOrigin = origin;
}


bool AtlasClipMapImageSource::isDataAvailable( const U32 mipLevel, const RectI& inRegion ) const
{
   // We need to convert from a mip level to a level in our TOC, potentially scaling.
   U32 baseMips = getBinLog2(mTOC->getTextureChunkSize());
   U32 tocLevel = -1;
   if(mipLevel >= baseMips)
   {
      // In an inner or leaf tile.
      tocLevel = mipLevel - baseMips;
   }
   else
   {
      // It's in our base tile.
      tocLevel = 0;
   }
   
   // We don't scale beyond the depth of the TOC, so simply assert in that
   // case.
   AssertFatal(tocLevel < mTOC->getTreeDepth(), "AtlasClipMapImageSource::isDataAvailable - went beyond depth of tree.");

   // Check all the chunks of all the stubs in the appropriate region...
   RectI r;
   convertToTOCRect(tocLevel, inRegion, r);

   const S32 xStart = mClamp(r.point.x,               0, BIT(tocLevel));
   const S32 xEnd   = mClamp(r.point.x + r.extent.x,  0, BIT(tocLevel));

   const S32 yStart = mClamp(r.point.y,               0, BIT(tocLevel));
   const S32 yEnd   = mClamp(r.point.y + r.extent.y,  0, BIT(tocLevel));

   for(S32 x=xStart; x<xEnd; x++)
   {
      for(S32 y=yStart; y<yEnd; y++)
      {
         AtlasResourceTexStub *arts = mTOC->getResourceStub(mTOC->getStub(tocLevel, Point2I(x,y)));

         if(!arts->hasResource())
            return false;
      }
   }

   return true;
}

struct ColumnNote
{
   S32 chunkColStart, chunkColEnd, colStart, colEnd;
   AtlasTexChunk *atc;
};

void AtlasClipMapImageSource::copyBits( const U32 inMipLevel, 
                                       const ClipMap::ClipStackEntry *cse, 
                                       const RectI& inSrcRegion, U8 *bits, const U32 pitch, const U32 expandonecomponent )
{
   //    Con::printf("           + copying data from level %d rect (%d,%d ext=%d,%d)", 
   //       inMipLevel, 
   //    srcRegion.point.x, srcRegion.point.y, 
   //    srcRegion.extent.x, srcRegion.extent.y); 

   // We want to write everything out in linear order, in order to have
   // (hopefully) have good memory characteristics, esp. if we're writing onto
   // the bus.
   //
   // So we need to walk across the relevant sections of the texture chunks,
   // so our IO looks something like this:
   //      Reads                    Writes
   //   Across Chunks             To Clip Map
   // |----| |----| |----|      |----------------|
   // |  12| |3456| |78  |      |    12345678    |
   // |  12| |3456| |78  |      |    12345678    |
   // |----| |----| |----|      |    12345678    |
   //                           |                |
   // |----| |----| |----|      |----------------|
   // |  12| |3456| |78  |
   // |    | |    | |    |
   // |----| |----| |----|
   //
   // Neat! Naturally we need to determine this efficiently as we're in a 
   // texture lock when we do it. (Ick!)

   AssertFatal(expandonecomponent == 0, "AtlasClipMapImageCache_TextureTOC::bitblit - expandonecomponent is not supported nor planned!");

   RectI srcRegion = inSrcRegion;

   // We need to convert from a mip level to a level in our TOC, potentially scaling.
   U32 mipLevel = inMipLevel;

   // In cases where the clip map is actually smaller than our base tile, we'll have to work
   // off some miplevel of that tile.
   bool isBaseTileMip = false;
   U32 baseTileMip = 0; // This is a GBitmap miplevel index, not a clip map miplevel index!!! (Oh lovely confusion...)

   RectI srcTOCs( 0, 0, 1, 1 );
   U32 baseMips = getBinLog2(mTOC->getTextureChunkSize());
   U32 tocLevel = -1;
   if(mipLevel >= baseMips)
   {
      // In an inner or leaf tile.
      tocLevel = mipLevel - baseMips;
      convertToTOCRect(tocLevel, srcRegion, srcTOCs);
   }
   else
   {
      if( !srcRegion.overlaps( RectI( 0, 0, BIT( mipLevel ) - 1, BIT( mipLevel ) - 1 ) ) )
         return;

      // It's in some miplevel that's actually lower-res than our base tile,
      // so set up to work from an extruded miplevel of the base tile GBitmap.

      tocLevel = 0;
      isBaseTileMip = true;
      baseTileMip = baseMips - mipLevel;

      // Adjust the source region to work in base tile map dimensions.
      // This is all a bit hairy since from now on we have to be on our
      // toes whether we're speaking base tile or mipmap coordinates.

      srcRegion.point.x *= BIT( baseTileMip );
      srcRegion.point.y *= BIT( baseTileMip );

      srcRegion.extent.x <<= baseTileMip;
      srcRegion.extent.y <<= baseTileMip;
   }

   // Make sure we CAN bitblt this... must be all 
   const RectI safeTOCRegion(0, 0, BIT(tocLevel), BIT(tocLevel));
   if(!safeTOCRegion.contains(srcTOCs))
      return;

   U32 texelsCopied = 0;

   const S32 left   = mClamp(srcTOCs.point.x, 0, BIT(tocLevel));
   const S32 top    = mClamp(srcTOCs.point.y, 0, BIT(tocLevel));
   const S32 right  = mClamp(srcTOCs.point.x + srcTOCs.extent.x, 0, BIT(tocLevel));
   const S32 bottom = mClamp(srcTOCs.point.y + srcTOCs.extent.y, 0, BIT(tocLevel));

   // Memoize column info - will save us a TON of work if we have a vertical
   // stripe. (With 512px clipmap, if we move one pixel "against the grain"
   // it will let us save 512 times calculating this stuff, if we're
   // copying from one tile...)

   FrameTemp<ColumnNote> cnotes(bottom-top);

   for(S32 i=0; i<bottom-top; i++)
   {
      const S32 y = i + top;

      // Now we're walking along the columns, so figure a min/max column
      // for this chunk's row.
      cnotes[i].chunkColStart = S32(y * mTOC->getTextureChunkSize());
      cnotes[i].chunkColEnd   = S32((y+1) * mTOC->getTextureChunkSize());

      AssertFatal(srcRegion.point.y < cnotes[i].chunkColEnd,
         "AtlasClipMapImageCache_TextureTOC::bitblit - considering chunk outside source region (end)!");

      AssertFatal(srcRegion.point.y + srcRegion.extent.y >= cnotes[i].chunkColStart,
         "AtlasClipMapImageCache_TextureTOC::bitblit - considering chunk outside source region (start)!");

      cnotes[i].colStart = mClamp(srcRegion.point.y, 
         cnotes[i].chunkColStart, cnotes[i].chunkColEnd );
      cnotes[i].colEnd   = mClamp(srcRegion.point.y + srcRegion.extent.y, 
         cnotes[i].chunkColStart, cnotes[i].chunkColEnd );

      AssertFatal(cnotes[i].chunkColStart < cnotes[i].chunkColEnd, 
         "AtlasClipMapImageCache_TextureTOC::bitblit - invalid cnotes[i].chunkCol start and end.");

      AssertFatal(cnotes[i].colStart < cnotes[i].colEnd, 
         "AtlasClipMapImageCache_TextureTOC::bitblit - invalid cnotes[i].col start and end.");
   }

   for(S32 x=left; x<right; x++ )
   {
      // So, in reference to above diagram, we're walking across some
      // chunks that contain image data we care about. For each row of
      // pixels we need to copy we have to walk across the chunks in this row.

      // Update chunk bitmap pointers.
      for(S32 i=0; i<(bottom-top); i++)
      { 
         const S32 y = i + top;
         cnotes[i].atc = mTOC->getResourceStub(mTOC->getStub(tocLevel, Point2I(x,y)))->mChunk;
         AssertWarn(cnotes[i].atc, avar("AtlasClipMapImageCache_TextureTOC::bitblt - missing chunk (%d,%d@%d)!", x,y,tocLevel));
      }

      // Let's figure what rows those are, then we can do some more copying.
      S32 chunkRowStart = S32(x * mTOC->getTextureChunkSize());
      S32 chunkRowEnd   = S32((x+1) * mTOC->getTextureChunkSize());

      S32 rowStart = getMax(chunkRowStart, srcRegion.point.x);
      S32 rowEnd   = getMin(chunkRowEnd, srcRegion.point.x + srcRegion.extent.x);

      if( baseTileMip > 0 )
      {
         // Translate to mipmap space.

         chunkRowStart  >>= baseTileMip;
         chunkRowEnd    >>= baseTileMip;
         rowStart       >>= baseTileMip;
         rowEnd         >>= baseTileMip;
      }

      for(S32 row=rowStart; row<rowEnd; row++)
      {
         for(S32 y=top; y<bottom; y++ )
         {
            const S32 i = y - top;

            // Now correct into chunkspace and copy some image data out.

            const U32 offsetX = ( row - srcRegion.point.x / BIT( baseTileMip ) ) * pitch;
            const U32 offsetY = ( 4 * (cnotes[i].colStart - srcRegion.point.y) ) / BIT( baseTileMip );

            U8 *outBits = bits + offsetX + offsetY;
            const U32 numPixels = ( cnotes[ i ].colEnd - cnotes[ i ].colStart ) >> baseTileMip;
            texelsCopied += numPixels;

            if(!cnotes[i].atc)
            {
               // Do some magenta!
               for(S32 j=0; j < numPixels; j++)
               {
                  outBits[j*4+0] = 0xFF;
                  outBits[j*4+1] = 0x00;
                  outBits[j*4+2] = 0xFF;
                  outBits[j*4+3] = 0x00;
               }

               continue;
            }

            GBitmap* bitmap = cnotes[ i ].atc->bitmap;

            // If we're copying from a base tile mip, make sure its miplevel data is in place.

            if( isBaseTileMip && bitmap->getNumMipLevels() == 1 )
               bitmap->extrudeMipLevels();

            const U8 *bmpBits = bitmap->getAddress(
               ( cnotes[i].colStart - cnotes[i].chunkColStart ) >> baseTileMip,
               row - chunkRowStart, baseTileMip );

            if( bitmap->getBytesPerPixel() == 4 )
            {
               dMemcpy( outBits, bmpBits, numPixels * 4 );
            }
            else if( bitmap->getBytesPerPixel() == 3 )
            {
               // Do a fast little copy
               const U8 *curIn = bmpBits;
               const U8 *endIn = (bmpBits + numPixels * 3);
               U32 *curOut = (U32*)outBits;

               while(curIn < endIn)
               {
                  *curOut = (curIn[2] << 16) | (curIn[1] << 8) | curIn[0];
                  curIn+=3;
                  curOut++;
               }
            }
            else
            {
               for(S32 z=0; z< numPixels*2; z++)
               {
                  outBits[z*2+0] = row % 137;
                  outBits[z*2+1] = ( cnotes[i].colStart / BIT( baseTileMip ) + z) % 137;
               }
            }
         }
      }
   }

   AssertFatal(texelsCopied == ( srcRegion.extent.x >> baseTileMip ) * ( srcRegion.extent.y >> baseTileMip ),
      "AtlasClipMapImageCache_TextureTOC - Copied fewer texels than expected!");
}

bool AtlasClipMapImageSource::isTiling() const
{
   return false;
}

F32 AtlasClipMapImageSource::texelOffset() const
{
   return 0.0f;
}
