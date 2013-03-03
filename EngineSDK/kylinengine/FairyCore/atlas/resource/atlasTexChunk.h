//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _ATLASTEXCHUNK_H_
#define _ATLASTEXCHUNK_H_

#include "gfx/bitmap/ddsFile.h"
#include "gfx/bitmap/gBitmap.h"
#include "gfx/gfxTextureHandle.h"
#include "atlas/core/atlasChunk.h"
#include "Core/FairyPrerequisites.h"
/// Atlas chunk subclass for textures.
///
/// This is where the real meat for textures lives; texture chunks may be in
/// a variety of formats, and include several textures worth of opacity
/// map data if they are used in that role.
///
/// @ingroup AtlasResource
class AtlasTexChunk : public AtlasChunk
{
   void writeDDS(Stream *s);
public:
   AtlasTexChunk();
   ~AtlasTexChunk();

   typedef AtlasChunk Parent;
   
   virtual void read(Stream *s);
   virtual void write(Stream *s);
   virtual U32 getHeadSentinel();
   virtual U32 getTailSentinel();

   virtual void process();

   void generate(AtlasChunk *children[4]);

   enum TexFormat
   {
      FormatJPEG, ///< Use (lossy) JPEG compression.
      FormatPNG,  ///< Use (lossless) PNG compression.
      FormatDDS,  ///< Use (fast-to-load, big, lossy) DDS with DXT compression.
   };

   TexFormat mFormat;

   /// If we're storing opacity map data there may be more than one
   /// layer worth of image data. (e.g. we have 10 channels of opacity
   /// data and PNG only supports 4 channels/image, so we'd have to have 
   /// 3 layers).
   ///
   /// TOC versions prior than 150 only support a layerCount of 1.
   U8 layerCount;
   GBitmap *bitmap;
   DDSFile *dds;

   inline const bool isBitmapTexFormat(const TexFormat f) const
   {
      switch(f)
      {
      case FormatJPEG:
      case FormatPNG:
         return true;
      }

      return false;
   }

   AtlasTexChunk *generateCopy(S32 reformat = -1);

   static GBitmap *loadDDSIntoGBitmap(const U8 *ddsBuffer, U32 ddsBufferSize);
};

#endif