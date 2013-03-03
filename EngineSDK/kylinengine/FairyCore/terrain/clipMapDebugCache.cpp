//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "terrain/clipMapDebugCache.h"
#include "platform/platform.h"
#include "core/frameAllocator.h"

ClipMapImageCache_Debug::ClipMapImageCache_Debug()
{
   mDoSquares = true;
   mCurDebugMode = 0;
}

bool ClipMapImageCache_Debug::isDataAvailable(U32 mipLevel, const RectI &region) const
{
   // We generate on the fly, so it's always available!
   return true;
}

void ClipMapImageCache_Debug::doRectUpdate(U32 mipLevel, 
                                                ClipMap::ClipStackEntry &cse, 
                                                const RectI &srcRegion, const RectI &dstRegion)
{
   // NOTE: The GTextureHandle::lock() call was broken at one time and
   // used a reversed axis standard.  It has since been fixed, but the Atlas
   // clipmap logic has not be fully corrected for this.  So we're swapping
   // the lock axis here until it is fixed properly.
   RectI swappedLock( dstRegion.point.y, dstRegion.point.x, dstRegion.extent.y, dstRegion.extent.x );

   // Acquire a lock on our texture.
   GLockedRect *lockRect = cse.mTex->lock(0, &swappedLock);
   U32 pitch = lockRect->pitch;
   U8 *bits = lockRect->bits;

   static bool updateToggle = false;

   updateToggle = !updateToggle;

   // Let's do a simple gradient + colored checkers test pattern.
   for(S32 x=0; x<srcRegion.extent.x; x++)
   {
      for(S32 y=0; y<srcRegion.extent.y; y++)
      {
         U8 *pixel = bits + (x * pitch) + 4 * y;

         S32 realX = x + srcRegion.point.x;
         S32 realY = y + srcRegion.point.y;

         S32 xFlag = realX & 4;
         S32 yFlag = realY & 4;

         if(xFlag ^ yFlag)
            pixel[0] = 0xFF;
         else
            pixel[0] = 0;

         pixel[1] = U8(F32(realX) / (512.0f * cse.mScale) * 255.0f);
         pixel[2] = updateToggle ? 0xFF : 0;
         pixel[3] = 0; //F32(F32(realY) / F32(F32(mClipMapSize) * cse.mScale) * 255.0);
      }
   }

   // Don't forget to unlock.
   cse.mTex.unlock();
}
