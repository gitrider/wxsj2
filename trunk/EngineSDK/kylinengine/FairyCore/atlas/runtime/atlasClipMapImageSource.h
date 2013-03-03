//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ATLAS_RUNTIME_ATLASCLIPMAPIMAGESOURCE_H_
#define _ATLAS_RUNTIME_ATLASCLIPMAPIMAGESOURCE_H_

#include "clipmap/clipMapImageSource.h"
#include "atlas/runtime/atlasInstanceTexTOC.h"

class AtlasClipMapImageSource : public IClipMapImageSource
{
   AtlasInstanceTexTOC *mTOC;
   Point2I mCacheOrigin;
   bool mUnique;

   /// Given a texel region, convert to the TOC chunks that contain the
   /// specified data.
   inline void convertToTOCRect(const U32 mipLevel, const RectI &region, RectI &outStubs) const
   {
      const F32 chunkSize = (F32)mTOC->getTextureChunkSize();
      outStubs.point.x = (S32)mFloor(F32(region.point.x) / chunkSize);
      outStubs.point.y = (S32)mFloor(F32(region.point.y) / chunkSize);

      outStubs.extent.x = (S32)mCeil(F32(region.point.x + region.extent.x) / chunkSize) - outStubs.point.x;
      outStubs.extent.y = (S32)mCeil(F32(region.point.y + region.extent.y) / chunkSize) - outStubs.point.y;
   }

public:

   AtlasClipMapImageSource();
   ~AtlasClipMapImageSource();

   void setTOC(AtlasResourceTexTOC *toc);

   void setUnique(bool unique) { mUnique = unique; };
   bool isUnique() { return mUnique; };

   virtual const U32 getMipLevelCount() const;
   virtual void setInterestCenter(const Point2I& origin, const U32 radius);
   virtual bool isDataAvailable(const U32 mipLevel, const RectI& region) const;
   virtual void copyBits(const U32 mipLevel, const ClipMap::ClipStackEntry *cse, 
      const RectI& srcRegion, U8 *bits, const U32 pitch, const U32 expandonecomponent);
   virtual bool isTiling() const;

   S32 getChunkSize()
   {
      return mTOC->getTextureChunkSize();
   }

   virtual F32 texelOffset() const;
};

#endif
