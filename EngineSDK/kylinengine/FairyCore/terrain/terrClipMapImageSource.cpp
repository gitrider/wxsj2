//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "terrain/terrClipMapImageSource.h"
#include "core/util/safeDelete.h"
#include "graphic/bitmap/gBitmap.h"

TerrClipMapImageSource::TerrClipMapImageSource( GBitmap *bmp, F32 offset )
{
   mBitmap = bmp;
   texOffset = offset;
}

TerrClipMapImageSource::~TerrClipMapImageSource()
{
   SAFE_DELETE(mBitmap);
}

const U32 TerrClipMapImageSource::getMipLevelCount() const
{
   return getBinLog2(mBitmap->getWidth()) + 1;
}

void TerrClipMapImageSource::setInterestCenter( const Point2I &origin, const U32 radius )
{
   // Do nothing, we don't page.
}

bool TerrClipMapImageSource::isDataAvailable( const U32 mipLevel, const RectI &region ) const
{
   // Data always available, as we don't page.
   return true;
}

inline S32 safeModulo(S32 v, S32 m)
{
   while(v >= m)
      v -= m;
   while(v < 0)
      v += m;

   return v;
}

void clipAgainstGrid(const S32 gridSpacing, const RectI &rect, S32 &outCount, RectI *&outBuffer)
{
   // Check against X grids...
   const S32 startX = rect.point.x;
   const S32 endX   = rect.point.x + rect.extent.x;

   const S32 startGridX = mFloor(F32(startX) / F32(gridSpacing));
   const S32 endGridX   = mCeil(F32(endX) / F32(gridSpacing));

   // Check against Y grids...
   const S32 startY = rect.point.y;
   const S32 endY   = rect.point.y + rect.extent.y;

   const S32 startGridY = mFloor(F32(startY) / F32(gridSpacing));
   const S32 endGridY   = mCeil(F32(endY) / F32(gridSpacing));

   // Now we know the region of grid squares we overlap; so we can
   // allocate an array of RectI's to store our results in.
   outCount  = (endGridY - startGridY) * (endGridX - startGridX);
   outBuffer = new RectI[outCount];

   // Track where we are in the outBuffer.
   S32 emittedRects = 0;

   // All that's left is to run through all the grid squares our rect overlaps
   // and clip against each one in turn.
   for(S32 gridX=startGridX; gridX<endGridX; gridX++)
   {
      for(S32 gridY=startGridY; gridY<endGridY; gridY++)
      {
         // Ok - clip our rect into this grid square and store it out.
         RectI &outRect = outBuffer[emittedRects++];

         // Calculate extents of this grid square.
         const S32 minX = gridX * gridSpacing;
         const S32 maxX = minX + gridSpacing;
         const S32 minY = gridY * gridSpacing;
         const S32 maxY = minY + gridSpacing;

         // Now let's do some clamping!
         outRect.point.x = mClamp(startX, minX, maxX);
         outRect.point.y = mClamp(startY, minY, maxY);
         
         // This original version of this was adding 1, giving invalid extents (1025x1025 on 1024x1024 clipmap)
         outRect.extent.x = mClamp(endX, minX, maxX) - outRect.point.x;
         outRect.extent.y = mClamp(endY, minY, maxY) - outRect.point.y;
      }
   }
}

void TerrClipMapImageSource::copyBits( const U32 mipLevel, const ClipMap::ClipStackEntry *cse,
                                       const RectI& srcRegion, U8 *bits, const U32 pitch, const U32 expandonecomponent )
{
   const S32 requestedBitmapMip = getMipLevelCount() - 1 - mipLevel;

   if(requestedBitmapMip != 0 && mBitmap->getNumMipLevels() == 1)
   {
      mBitmap->extrudeMipLevels(false);

      // Debug code to visualize image sources.
      // static int debugCnt=0;
      // debugCnt++;
      // mBitmap->writePNGDebug((char*)avar("demo/tcmis_debug%d", debugCnt));
   }

   AssertFatal(requestedBitmapMip < mBitmap->getNumMipLevels(), "TerrClipMapImageSource::copyBits - too-high mip level!");
   AssertFatal(mBitmap->getWidth() == mBitmap->getHeight(), "TerrClipMapImageSource::copyBits - can't use non-square source images.");
   //AssertFatal(srcRegion.extent.x <= mBitmap->getWidth(), "TerrClipMapImageSource::copyBits - invalid request! (x)");
   //AssertFatal(srcRegion.extent.y <= mBitmap->getWidth(), "TerrClipMapImageSource::copyBits - invalid request! (y)");

   // Tricky thing here, we have to flip the rectangle - so y becomes x and
   // vice versa. This is to conform to existing ClipMap convention.

   // Clip our input level against our size at this mip level...
   RectI *clippedRects = NULL;
   S32 clippedRectCount = 0;
   clipAgainstGrid(mBitmap->getWidth(requestedBitmapMip), srcRegion, clippedRectCount, clippedRects);
   
   // Ensure that we don't try to copy beyond the bounds of the bitmap
   for(U32 i = 0; i < clippedRectCount; i++)
   {
      if(safeModulo(clippedRects[i].point.y, mBitmap->getHeight(requestedBitmapMip)) + clippedRects[i].extent.y > (S32)mBitmap->getHeight(requestedBitmapMip))
         clippedRects[i].extent.y = mBitmap->getHeight(requestedBitmapMip) - safeModulo(clippedRects[i].point.y, mBitmap->getHeight(requestedBitmapMip));
      
      if(safeModulo(clippedRects[i].point.x, mBitmap->getWidth(requestedBitmapMip)) + clippedRects[i].extent.x > (S32)mBitmap->getWidth(requestedBitmapMip))
         clippedRects[i].extent.x = mBitmap->getWidth(requestedBitmapMip) - safeModulo(clippedRects[i].point.x, mBitmap->getWidth(requestedBitmapMip));
   }

   // Now copy all the rows.
   if(mBitmap->getBytesPerPixel() == 4 && expandonecomponent == 0)
   {
      // Do memcpy route.
      for(S32 rectIdx=0; rectIdx<clippedRectCount; rectIdx++)
      {
         for(S32 i=0; i<clippedRects[rectIdx].extent.x; i++)
         {
            U8 *bitOffset = bits +
                            pitch * (i + (clippedRects[rectIdx].point.x - srcRegion.point.x)) +
                            (clippedRects[rectIdx].point.y - srcRegion.point.y) * 4;
            dMemcpy(bitOffset,
               mBitmap->getAddress(
                 safeModulo(clippedRects[rectIdx].point.y, mBitmap->getHeight(requestedBitmapMip)),
                 safeModulo(clippedRects[rectIdx].point.x+i, mBitmap->getWidth(requestedBitmapMip)),
                 requestedBitmapMip),
                 4 * clippedRects[rectIdx].extent.y);
         }
      }
   }
   else if(mBitmap->getBytesPerPixel() == 4 && expandonecomponent > 0)
   {
      // Expand R or G or B or A to a full color.
      AssertFatal(expandonecomponent < 5, "TerrClipMapImageSource::copyBits - expandonecomponent can only be 0, 1, 2 or 3 on a 4 component source");
      U32 component = expandonecomponent - 1;
      for(S32 rectIdx=0; rectIdx<clippedRectCount; rectIdx++)
      {
         for(S32 i=0; i<clippedRects[rectIdx].extent.x; i++)
         {
            const U8 *bmpBits = mBitmap->getAddress(
               safeModulo(clippedRects[rectIdx].point.y, mBitmap->getHeight(requestedBitmapMip)),
               safeModulo(clippedRects[rectIdx].point.x+i, mBitmap->getWidth(requestedBitmapMip)),
               requestedBitmapMip);
            const U8 *curIn = bmpBits;
            const U8 *endIn = bmpBits + (clippedRects[rectIdx].extent.y * 4);
            U32 *curOut = (U32 *)(bits + pitch*(i + (clippedRects[rectIdx].point.x - srcRegion.point.x)) + (clippedRects[rectIdx].point.y - srcRegion.point.y) * 4);

            while(curIn < endIn)
            {
               *curOut = curIn[component] * 0x01010101;
               curIn+=4;
               curOut++;
            }
         }
      }
   }
   else if(mBitmap->getBytesPerPixel() == 3 && expandonecomponent == 0)
   {
      // We have to pad up to 4 bytes.
      for(S32 rectIdx=0; rectIdx<clippedRectCount; rectIdx++)
      {
         for(S32 i=0; i<clippedRects[rectIdx].extent.x; i++)
         {
            if (i >= 1024)
               continue;

            const U8 *bmpBits = mBitmap->getAddress(
               safeModulo(clippedRects[rectIdx].point.y, mBitmap->getHeight(requestedBitmapMip)),
               safeModulo(clippedRects[rectIdx].point.x+i, mBitmap->getWidth(requestedBitmapMip)),
               requestedBitmapMip);
            const U8 *curIn = bmpBits;
            const U8 *endIn = bmpBits + (clippedRects[rectIdx].extent.y * 3);
            U32 *curOut = (U32*)(bits + pitch*(i + (clippedRects[rectIdx].point.x - srcRegion.point.x)) + (clippedRects[rectIdx].point.y - srcRegion.point.y) * 4);

            while(curIn < endIn)
            {
               *curOut = (curIn[2] << 16) | (curIn[1] << 8) | curIn[0];
               curIn+=3;
               curOut++;
            }
         }
      }
   }
   else if(mBitmap->getBytesPerPixel() == 3 && expandonecomponent > 0)
   {
      // Expand R or G or B to a full color.
      AssertFatal(expandonecomponent < 4, "TerrClipMapImageSource::copyBits - expandonecomponent can only be 0, 1 or 2 on a 3 component source");
      U32 component = expandonecomponent - 1;
      for(S32 rectIdx=0; rectIdx<clippedRectCount; rectIdx++)
      {
         for(S32 i=0; i<clippedRects[rectIdx].extent.x; i++)
         {
            const U8 *bmpBits = mBitmap->getAddress(
               safeModulo(clippedRects[rectIdx].point.y, mBitmap->getHeight(requestedBitmapMip)),
               safeModulo(clippedRects[rectIdx].point.x+i, mBitmap->getWidth(requestedBitmapMip)),
               requestedBitmapMip);
            const U8 *curIn = bmpBits;
            const U8 *endIn = bmpBits + (clippedRects[rectIdx].extent.y * 3);
            U8 *curOut = (bits + pitch*(i + (clippedRects[rectIdx].point.x - srcRegion.point.x)) + (clippedRects[rectIdx].point.y - srcRegion.point.y) * 4);

            while(curIn < endIn)
            {
               *curOut = curIn[component] * 0x01010101;
               curIn+=3;
               curOut++;
            }
         }
      }
   }
   else
   {
      AssertFatal(false, "16 bit sources not supported!");
   }

   // Don't forget to nuke our rect list.
   delete[] clippedRects;
}

bool TerrClipMapImageSource::isTiling() const
{
   return true;
}

F32 TerrClipMapImageSource::texelOffset() const
{
   return texOffset;
}
