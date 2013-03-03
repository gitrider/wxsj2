//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _CLIPMAP_CLIPMAPBLENDERCACHE_H_
#define _CLIPMAP_CLIPMAPBLENDERCACHE_H_

#include "graphic/gDevice.h"
#include "terrain/clipMapImageSource.h"
#include "terrain/clipMapCache.h"

class ClipMapBlenderCache : public IClipMapImageCache
{
public:
   ClipMapBlenderCache(IClipMapImageSource *opacitySource, IClipMapImageSource *lightmapSource, bool LM1 = false);
   ClipMapBlenderCache(IClipMapImageSource *lightmapSource);
   virtual ~ClipMapBlenderCache();

   virtual void initialize(ClipMap *cm);
   virtual void setInterestCenter(const Point2I &origin);
   virtual bool beginRectUpdates(ClipMap::ClipStackEntry &cse);
   virtual void doRectUpdate(U32 mipLevel, ClipMap::ClipStackEntry &cse, const RectI &srcRegion, const RectI &dstRegion);
   virtual void finishRectUpdates(ClipMap::ClipStackEntry &cse);
   virtual bool isDataAvailable(U32 mipLevel, const RectI &region) const;
   virtual bool isRenderToTargetCache();

   void clearSourceImages();
   void registerSourceImage(const String &imgPath);

   void clearOpacitySources();
   void registerOpacitySource(IClipMapImageSource *opacitySource);

   void createOpacityScratchTextures();

   // Lightmap support
   void setNewLightmapSource(IClipMapImageSource *lightmapSource);

private:
   /// References to image source for opacity and lightmap information.
   VectorPtr<IClipMapImageSource*> mOpacitySources;
   VectorPtr<IClipMapImageSource*> mLightmapSources;

   Point4F     mTempScaleFactors;
   Vector<F32> mScales;
   Vector<F32> mSourceWidths;

   /// Used for streaming video data to card for blend operations.
   Vector<GTexHandle> mOpacityScratchTextures;
   Vector<GTexHandle> mLightmapScratchTextures;

   /// Whether LM1 (lightmap blend factor 1) shaders should be used.
   bool mLM1;

   /// Whether mOpacityScratchTexturesFF should be used, amongst other things
   bool mFixedfunction;

   /// Shared shader constant blocks
   GShaderConstBufferRef mShaderConsts;
   GShaderConstHandle* mModelViewProjSC;
   GShaderConstHandle* mOpacityMapSC;
   GShaderConstHandle* mLightMapSC;
   GShaderConstHandle* mTex1SC;
   GShaderConstHandle* mTex2SC;
   GShaderConstHandle* mTex3SC;
   GShaderConstHandle* mTex4SC;
   GShaderConstHandle* mSourceTexScalesSC;

   /// SM2.0 shader used for one pass blending.
   GShader *mOnePass;
   GStateBlockRef mOnePassSB;

   /// SM1.0 shaders used for two pass blending.
   GShader *mTwoPass[2];
   GStateBlockRef mTwoPassSB;

   /// FF
   GStateBlockRef mFFBaseLayerSB;
   GStateBlockRef mFFAdditionalLayersSB;
   GStateBlockRef mFFLightmapSB;

   /// Images that we're blending; in channel-usage order.
   Vector<GTexHandle> mSourceImages;

   U32 mClipMapSize;
   ClipMap *mOwningClipMap;

   /// Generate appropriate volatile geometry and set it as the current VB
   void setupGeometry( const RectF &srcRect0, const RectF &srcRect1,
                       const RectF &masterCoords, const RectI &dstRect);
   void setupGeometryFF( const RectF &srcRect0, const RectF &masterCoords,
                       const F32 scaleFactor, const RectI &dstRect);
};

#endif
