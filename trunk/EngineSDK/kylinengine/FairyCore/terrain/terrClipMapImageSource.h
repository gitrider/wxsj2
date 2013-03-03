//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _TERRCLIPMAPIMAGESOURCE_H_
#define _TERRCLIPMAPIMAGESOURCE_H_

#include "terrain/clipMapImageSource.h"

class TerrClipMapImageSource : public IClipMapImageSource
{
public:
   GBitmap *mBitmap;

   TerrClipMapImageSource(GBitmap *bmp, F32 offset);
   virtual ~TerrClipMapImageSource();

   virtual const U32 getMipLevelCount() const;
   virtual void setInterestCenter(const Point2I &origin, const U32 radius);
   virtual bool isDataAvailable(const U32 mipLevel, const RectI &region) const;
   virtual void copyBits(const U32 mipLevel, const ClipMap::ClipStackEntry *cse,
      const RectI& srcRegion, U8 *bits, const U32 pitch, const U32 expandonecomponent);
   virtual bool isTiling() const;
   virtual F32 texelOffset() const;
private:
   F32 texOffset;
};

#endif // _TERRCLIPMAPIMAGESOURCE_H_
