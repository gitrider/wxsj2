//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _CLIPMAP_CLIPMAPIMAGESOURCE_H_
#define _CLIPMAP_CLIPMAPIMAGESOURCE_H_

#include "terrain/clipMap.h"

/// Interface to fetch bits from an image source.
///
/// This abstracts the clipmap system from the source of its image bits -
/// might be an Atlas TOC or just a GBitmap, depending on the user of the
/// clipmap.
///
/// @note Please note that the clip mapping code uses different mipmap
///   level numbering than the GBitmap code.  Whereas for GBitmaps miplevel 0
///   is the original image, it is the 1x1 mipmap for the clip map code.
class IClipMapImageSource
{
public:
   virtual ~IClipMapImageSource() {}

   /// Number of mip levels. Used to compensate for varied image source depths.
   virtual const U32 getMipLevelCount() const = 0;

   /// Set a new interest center for this image source. This gives the source
   /// important cues for paging of content.
   virtual void setInterestCenter(const Point2I &origin, const U32 radius) = 0;

   /// True if the data for the specified mip level and region is available.
   virtual bool isDataAvailable(const U32 mipLevel, const RectI &region) const = 0;

   /// If true, this image source tiles itself infinitely; if the image source
   /// is tiling then the clipmap can also be tiling. Specifically this means
   /// that the source can update a rectangle from any location in image-space
   /// as long as its extents are no bigger than the reported size of the
   /// image source.
   virtual bool isTiling() const = 0;

   /// Copy bits from source into a locked texture buffer.
   ///
   /// @param mipLevel  What mip level of the texture?
   /// @param cse       Information about the clip stack layer we're uploading to. Can be NULL.
   /// @param srcRegion Region in pixels on this mip level to copy from.
   /// @param bits      Pointer to beginning of locked buffer.
   /// @param pitch     Stride in bytes from start of one row to start of next.
   virtual void copyBits(const U32 mipLevel, const ClipMap::ClipStackEntry *cse,
      const RectI& srcRegion, U8 *bits, const U32 pitch, const U32 expandonecomponent)=0;

   /// This is used to deal with geometry that wants texels to be at vertices vs. geometry that wants texels to
   /// be in the center of the quad/triangle.  Return 0 for centering, and 0.5 for vertex based.
   virtual F32 texelOffset() const = 0;
};

#endif
