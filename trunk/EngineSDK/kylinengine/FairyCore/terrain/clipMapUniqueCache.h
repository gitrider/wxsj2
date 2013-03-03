//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _CLIPMAP_CLIPMAPUNIQUECACHE_H_
#define _CLIPMAP_CLIPMAPUNIQUECACHE_H_

#include "terrain/clipMapCache.h"
#include "terrain/clipMapImageSource.h"

/// Fill clipmap with data from a single large image source.
class ClipMapUniqueCache : public IClipMapImageCache
{
   IClipMapImageSource *mUniqueSource;
   U32 mClipMapSize;

public:
   ClipMapUniqueCache(IClipMapImageSource *uniqueSource);
   virtual ~ClipMapUniqueCache();
   virtual void initialize(ClipMap *cm);
   virtual void setInterestCenter(const Point2I &origin);
   virtual bool beginRectUpdates(ClipMap::ClipStackEntry &cse);
   virtual void doRectUpdate(U32 mipLevel, ClipMap::ClipStackEntry &cse, const RectI &srcRegion, const RectI &dstRegion);
   virtual void finishRectUpdates(ClipMap::ClipStackEntry &cse);
   virtual bool isDataAvailable(U32 mipLevel, const RectI &region) const;
   virtual bool isRenderToTargetCache();
};

#endif