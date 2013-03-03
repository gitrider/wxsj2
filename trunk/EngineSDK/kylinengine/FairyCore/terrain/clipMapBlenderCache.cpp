//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "terrain/clipMapBlenderCache.h"
#include "materials/shaderData.h"
#include "core/util/safeDelete.h"
#include "shader/shaderVars.h"
#include "graphic/bitmap/gBitmap.h"

ClipMapBlenderCache::ClipMapBlenderCache( IClipMapImageSource *opacitySource, IClipMapImageSource *lightmapSource, bool LM1 )
{
   mOpacitySources.push_back(opacitySource);
   mLightmapSources.push_back(lightmapSource);
   mOwningClipMap = NULL;
   mClipMapSize = -1;
   mTempScaleFactors.set(128.0f, 128.0f, 128.0f, 128.0f);
   mLM1 = LM1;
}

ClipMapBlenderCache::ClipMapBlenderCache(IClipMapImageSource *lightmapSource)
{
   mLightmapSources.push_back(lightmapSource);
   mOwningClipMap = NULL;
   mClipMapSize = -1;
   mTempScaleFactors.set(128.0f, 128.0f, 128.0f, 128.0f);
   mLM1 = false;
}

ClipMapBlenderCache::~ClipMapBlenderCache()
{
   clearSourceImages();
   clearOpacitySources();

   mOpacityScratchTextures.clear();
   mLightmapScratchTextures.clear();

   for (U32 i = 0; i < mLightmapSources.size(); i++)
      SAFE_DELETE(mLightmapSources[i]);

   mLightmapSources.clear();
}

void ClipMapBlenderCache::createOpacityScratchTextures()
{
   U32 numScratchTextures = 1;
   if (mFixedfunction)
      numScratchTextures = 4;

   if (mOpacitySources.size() > 0)
      numScratchTextures *= mOpacitySources.size();

   for (U32 i = mOpacityScratchTextures.size(); i < numScratchTextures; i++)
   {
      mOpacityScratchTextures.increment();
      mOpacityScratchTextures.last().set( mClipMapSize, mClipMapSize, GFormatR8G8B8A8, &ClipMapTextureProfile, avar("%s() - mOpacityScratchTextures (line %d)", __FUNCTION__, __LINE__), 1);
   }
}

void ClipMapBlenderCache::initialize( ClipMap *cm )
{
   mOwningClipMap = cm;
   mClipMapSize = cm->mClipMapSize;

   mLightmapScratchTextures.push_back(GTexHandle( mClipMapSize, mClipMapSize, GFormatR8G8B8X8, &ClipMapTextureProfile, avar("%s() - mLightmapScratchTextures (line %d)", __FUNCTION__, __LINE__), 1 ));

   if (GRAPHIC->getPixelShaderVersion() == 0.0f)
   {
      mFixedfunction = true;

      // Fixed function stateblock
      GStateBlockDesc sbFF;
      sbFF.samplersDefined = true;
      sbFF.samplers[0] = GSamplerStateDesc::getClampLinear();
      sbFF.samplers[0].textureColorOp = GTOPSelectARG1;
      sbFF.samplers[0].colorArg1 = GTATexture;
      sbFF.samplers[1] = GSamplerStateDesc::getWrapLinear();      
      mFFBaseLayerSB = GRAPHIC->createStateBlock(sbFF);
      sbFF.setBlend(true, GBlendOne, GBlendOne);
      mFFAdditionalLayersSB = GRAPHIC->createStateBlock(sbFF);
      sbFF.setBlend(true, GBlendDestColor, GBlendSrcColor);
      sbFF.samplers[1].textureColorOp = GTOPDisable;
      mFFLightmapSB = GRAPHIC->createStateBlock(sbFF);
   }
   else
   {
      mFixedfunction = false;

      // Find and init shaders.
      ShaderData *sd = NULL;      
      if(GRAPHIC->getPixelShaderVersion() >= 2.0f)
      {
         if(!Sim::findObject( ( mLM1 ? "AtlasBlender20ShaderLM1" : "AtlasBlender20Shader" ), sd) || (sd->mShader == NULL))
         {
            Con::errorf("ClipMapBlenderCache::initialize - "
               "Couldn't find shader 'AtlasBlender20Shader'! Terrain will not blend properly on SM2.0 cards!");
         }
         else
         {
            mOnePass = sd->mShader;

            if (mOnePass)
               mShaderConsts = mOnePass->allocConstBuffer();

            if (mShaderConsts)
               sd->mapSamplerNames(mShaderConsts);
         }
      }
      else
      {
         if(!Sim::findObject( ( mLM1 ? "AtlasBlender11AShaderLM1" : "AtlasBlendeer11AShader" ), sd) || (sd->mShader == NULL))
         {
            Con::errorf("ClipMapBlenderCache::initialize - "
               "Couldn't find shader 'AtlasBlender11AShader'! Terrain will not blend properly on SM1.0 cards!");
         }
         else
         {
            mTwoPass[0] = sd->mShader;

            if (mTwoPass[0])
               mShaderConsts = mTwoPass[0]->allocConstBuffer();

            if (mShaderConsts)
               sd->mapSamplerNames(mShaderConsts);
         }

         if(!Sim::findObject( ( mLM1 ? "AtlasBlender11BShaderLM1" : "AtlasBlender11BShader" ), sd) || (sd->mShader == NULL))
         {
            Con::errorf("ClipMapBlenderCache::initialize - "
               "Couldn't find shader 'AtlasBlender11BShader'! Terrain will not blend properly on SM1.0 cards!");
         }
         else
         {
            mTwoPass[1] = sd->mShader;
         }
      }
      if (mShaderConsts)
      {
         mModelViewProjSC = mShaderConsts->getShader()->getShaderConstHandle(ShaderVars::modelview);
         mOpacityMapSC = mShaderConsts->getShader()->getShaderConstHandle("$opacity");
         mLightMapSC = mShaderConsts->getShader()->getShaderConstHandle("$lightMap");
         mTex1SC = mShaderConsts->getShader()->getShaderConstHandle("$tex1");
         mTex2SC = mShaderConsts->getShader()->getShaderConstHandle("$tex2");
         mTex3SC = mShaderConsts->getShader()->getShaderConstHandle("$tex3");
         mTex4SC = mShaderConsts->getShader()->getShaderConstHandle("$tex4");
         mSourceTexScalesSC = mShaderConsts->getShader()->getShaderConstHandle("$sourceTexScales");
      }
      
      // Init state blocks
      GStateBlockDesc sbd;
      sbd.setCullMode(GCullNone);
      sbd.setZEnable(false);
      sbd.zWriteEnable = false;
      sbd.samplersDefined = true;
      sbd.samplers[0] = GSamplerStateDesc::getClampLinear();
      sbd.samplers[1] = GSamplerStateDesc::getClampLinear();
      sbd.samplers[2] = GSamplerStateDesc::getWrapLinear();
      sbd.samplers[3] = GSamplerStateDesc::getWrapLinear();
      sbd.samplers[4] = GSamplerStateDesc::getWrapLinear();
      sbd.samplers[5] = GSamplerStateDesc::getWrapLinear();

      mOnePassSB = GRAPHIC->createStateBlock(sbd);

      sbd.setBlend(true, GBlendOne, GBlendOne);

      mTwoPassSB = GRAPHIC->createStateBlock(sbd);   
   }

   createOpacityScratchTextures();
}

void ClipMapBlenderCache::setInterestCenter( const Point2I &origin )
{
   AssertFatal(mClipMapSize != -1, "ClipMapBlenderCache::setInterestCenter - no one set mClipMapSize!");
   
   if (mOpacitySources.size() == 0)
   {
      Con::errorf("ClipMapBlenderCache::setInterestCenter() - tried to set the interest center without any opacity sources");
      return;
   }

   // For each source, scale origin, which is in texels at the
   // most detailed clipmap level, to be in texels at the most detailed
   // level of each source.
   for (U32 i = 0; i < mOpacitySources.size(); i++)
   {
      Point2I opacityOrigin = origin;
      if( mOpacitySources[i]->getMipLevelCount() != ( getBinLog2( mOwningClipMap->mTextureSize ) + 1 ) )
      {
         S32 scaleDelta = mOpacitySources[i]->getMipLevelCount() - 1 - getBinLog2(mOwningClipMap->mTextureSize);
         if(scaleDelta > 0)
         {
            opacityOrigin.x <<= scaleDelta;
            opacityOrigin.y <<= scaleDelta;
         }
         else
         {
            opacityOrigin.x >>= -scaleDelta;
            opacityOrigin.y >>= -scaleDelta;
         }
      }

      mOpacitySources[i]->setInterestCenter(opacityOrigin, mClipMapSize * 2);
   }

   for (U32 i = 0; i < mLightmapSources.size(); i++)
   {
      Point2I lightmapOrigin = origin;

      if(mLightmapSources[i]->getMipLevelCount() != ( getBinLog2(mOwningClipMap->mTextureSize) + 1 ) )
      {
         S32 scaleDelta = mLightmapSources[i]->getMipLevelCount() - 1 - getBinLog2(mOwningClipMap->mTextureSize);
         if(scaleDelta > 0.0f)
         {
            lightmapOrigin.x <<= scaleDelta;
            lightmapOrigin.y <<= scaleDelta;
         }
         else
         {
            lightmapOrigin.x >>= -scaleDelta;
            lightmapOrigin.y >>= -scaleDelta;
         }
      }

      mLightmapSources[i]->setInterestCenter(lightmapOrigin, mClipMapSize * 2);
   }
}

bool ClipMapBlenderCache::isDataAvailable( U32 mipLevel, const RectI &region ) const
{
   // Scale the region to the available depths of the sources.
   for (U32 i = 0; i < mOpacitySources.size(); i ++)
   {
      RectI opacityRegion = region; 
      S32 opacityLevel = mipLevel;
      if(mipLevel >= mOpacitySources[i]->getMipLevelCount())
      {
         opacityLevel = mOpacitySources[i]->getMipLevelCount()-1;
         S32 scaleDelta = mipLevel - mOpacitySources[i]->getMipLevelCount() + 1;
         opacityRegion.point.x = region.point.x >> scaleDelta;
         opacityRegion.point.y = region.point.y >> scaleDelta;
         opacityRegion.extent.x = region.extent.x >> scaleDelta;
         opacityRegion.extent.y = region.extent.y >> scaleDelta;
      }

      if(!mOpacitySources[i]->isDataAvailable(opacityLevel, opacityRegion))
         return false;
   }

   for (U32 i = 0; i < mLightmapSources.size(); i++)
   {
      RectI lightmapRegion = region; 
      S32 lightmapLevel = mipLevel;
      if(mipLevel >= mLightmapSources[i]->getMipLevelCount())
      {
         lightmapLevel = mLightmapSources[i]->getMipLevelCount()-1;
         S32 scaleDelta = mipLevel - mLightmapSources[i]->getMipLevelCount() + 1;
         lightmapRegion.point.x = region.point.x >> scaleDelta;
         lightmapRegion.point.y = region.point.y >> scaleDelta;
         lightmapRegion.extent.x = region.extent.x >> scaleDelta;
         lightmapRegion.extent.y = region.extent.y >> scaleDelta;
      }

      if(!mLightmapSources[i]->isDataAvailable(lightmapLevel, lightmapRegion))
         return false;
   }

   return true;
}

bool ClipMapBlenderCache::isRenderToTargetCache()
{
   // We do render-target operations so must have a render-target allocated.
   return true;
}

// Ugly globals.
static MatrixF proj;
static RectI viewPort;
static bool mustEndScene = false;
static S32 pushDepth = 0;

bool ClipMapBlenderCache::beginRectUpdates( ClipMap::ClipStackEntry &cse )
{
   pushDepth++;
   AssertFatal(pushDepth == 1, "BAD1");

   if(!GRAPHIC->canCurrentlyRender())
   {
      mustEndScene = GRAPHIC->beginScene();
      if (!mustEndScene)
         return false;      
   }
   else
   {
      mustEndScene = false;
   }

   proj = GRAPHIC->getProjectionMatrix();
   viewPort = GRAPHIC->getViewport();
   GRAPHIC->pushWorldMatrix();

   // For sanity, let's just purge all our texture states.
   for(S32 i=0; i<GRAPHIC->getNumSamplers(); i++)
      GRAPHIC->setTexture(i, NULL);

   // Set a render target...
   GRAPHIC->pushActiveRenderTarget();
   GTextureTargetRef myTarg = GRAPHIC->allocRenderToTextureTarget();
   myTarg->attachTexture(GTextureTarget::Color0, cse.mTex );
   GRAPHIC->setActiveRenderTarget( myTarg );

   // And some render states.
   GRAPHIC->setWorldMatrix(MatrixF(true));
   GRAPHIC->setProjectionMatrix(MatrixF(true));
   GRAPHIC->disableShaders();

   GRAPHIC->setClipRect(RectI(0,0, mClipMapSize, mClipMapSize));

   // And pass it what it needs modelview wise...
   if (!mFixedfunction)
   {   
      if (!mShaderConsts.isNull())
      {
         MatrixF proj = GRAPHIC->getProjectionMatrix();
         proj.mul(GRAPHIC->getWorldMatrix());   
         mShaderConsts->set(mModelViewProjSC, proj);
         mShaderConsts->set(mOpacityMapSC, (S32)0);
         mShaderConsts->set(mLightMapSC, (S32)1);
         mShaderConsts->set(mTex1SC, (S32)2);
         mShaderConsts->set(mTex2SC, (S32)3);
         mShaderConsts->set(mTex3SC, (S32)4);
         mShaderConsts->set(mTex4SC, (S32)5);
         GRAPHIC->setShaderConstBuffer(mShaderConsts);
      }

      if (GRAPHIC->getPixelShaderVersion() >= 2.0f)
      {   
         for(S32 i=0; i<mClamp(mSourceImages.size(), 0, GRAPHIC->getNumSamplers()-2); i++)
         {
            GRAPHIC->setTexture(i+2,mSourceImages[i]);
         }
      }
   }

   // Scale the blended textures appropriately.
   U32 totalSize = mOwningClipMap->mTextureSize;

   for(S32 i=0; i< mSourceImages.size(); i++)
   {
      if (i >= mScales.size())
         mScales.push_back(128.0f);

      F32 scale = totalSize / mSourceWidths[i];
      mScales[i] = scale;
   }

   U32 padScalesTo4 = mScales.size() % 4;
   for(U32 i = 0; i < padScalesTo4; i++)
      mScales.push_back(128.0f);
      

   if (mShaderConsts)
   {
      mTempScaleFactors.set(mScales[0], mScales[1], mScales[2], mScales[3]);
      mShaderConsts->set(mSourceTexScalesSC, mTempScaleFactors);
   }

   return true;
}

void calcScaleRegionStuff( VectorPtr<IClipMapImageSource*>& sources, U32 levelSize, RectI srcRegion,
                          Vector<GTexHandle>& scratchTexs, RectF &outTexCoord, bool fixedFunction)
{
   // Quick pre-sanity check to make sure that there are enough scratch textures
   if (sources.size() == 0)
      return;
   if (scratchTexs.size() == 0)
      return;
   else if (fixedFunction && scratchTexs.size() < sources.size() * 4)
      return;
   else if (!fixedFunction && scratchTexs.size() < sources.size())
      return;

   // Beware! This function assumes that all of the sources and
   // scratchTexs are of the same size and formats
   //
   // source - the image source we're getting our data from
   // levelSize - size of this clipmap level in texels
   // srcRegion - region of this layer of the clipmap we're updating.
   // scratchTexs - textures we're uploading our data to.
   // tc - texture coordinates for the requested source data.
   Point2I scratchSize(1, 1);

   if (scratchTexs.size() > 0)
      scratchSize.set(scratchTexs[0].getWidth(), scratchTexs[0].getHeight());

   // What's the maximum resolution available for our source?
   S32 maxSourceSize = BIT(sources[0]->getMipLevelCount()-1);

   // Our upload will be noted here.
   RectI uploadRegion;
   U32 uploadLevel = -1;

   // Is this a non-scale situation?
   if(levelSize <= maxSourceSize)
   {
      // simple - just blast the right level up.
      uploadLevel = getBinLog2(levelSize);
      uploadRegion = srcRegion;

      outTexCoord.point.x = 0.0f;
      outTexCoord.point.y = 0.0f;
      outTexCoord.extent.x = F32(srcRegion.extent.x) / F32(scratchSize.x);
      outTexCoord.extent.y = F32(srcRegion.extent.y) / F32(scratchSize.y);
   }
   else
   {
      // harder - have to fudge it out by a texel in all directions, and
      //          generate TCs

      // First, figure scale factor
      S32 scaleDelta = getBinLog2(levelSize) - getBinLog2(maxSourceSize);
      F32 scaleFactor = F32(BIT(scaleDelta));

      // From this we know what level we're uploading.
      uploadLevel = getBinLog2(maxSourceSize);

      // Scale the rectangle.
      uploadRegion.point.x = (S32)mFloor(F32(srcRegion.point.x) / scaleFactor);
      uploadRegion.point.y = (S32)mFloor(F32(srcRegion.point.y) / scaleFactor);

      uploadRegion.extent.x = (S32)mCeil(F32(srcRegion.point.x + srcRegion.extent.x) / scaleFactor) - uploadRegion.point.x;
      uploadRegion.extent.y = (S32)mCeil(F32(srcRegion.point.y + srcRegion.extent.y) / scaleFactor) - uploadRegion.point.y;

      // Now, if possible we want a 1tx border around the region we're using.
      // This allows us to avoid nasty seam issues.

      // Nudging logic varies based on whether this is a tiling source or not.
      if(sources[0]->isTiling())
      {
         // We can always nudge since tiling sources support arbitrary size
         // uploads (since they tile).

         // We can nudge x, so do it.
         uploadRegion.point.x--;
         uploadRegion.extent.x++;
         uploadRegion.extent.x++;

         // We can nudge y, so do it.
         uploadRegion.point.y--;
         uploadRegion.extent.y++;
         uploadRegion.extent.y++;
      }
      else
      {
         // Nudge out left...
         if(uploadRegion.point.x > 0)
         {
            uploadRegion.point.x--;
            uploadRegion.extent.x++;
         }

         // Nudge out right..
         if((uploadRegion.extent.x + uploadRegion.point.x) < (maxSourceSize-1))
         uploadRegion.extent.x++;

         // Nudge top...
         if(uploadRegion.point.y > 0)
         {
            uploadRegion.point.y--;
            uploadRegion.extent.y++;
         }

         // Nudge bottom...
         if((uploadRegion.extent.y + uploadRegion.point.y) < (maxSourceSize-1))
            uploadRegion.extent.y++;
      }


      // Calculate the texture coordinates.
      outTexCoord.point.x = (F32(srcRegion.point.x) / scaleFactor - F32(uploadRegion.point.x)) / scratchSize.x;
      outTexCoord.point.y = (F32(srcRegion.point.y) / scaleFactor - F32(uploadRegion.point.y)) / scratchSize.y;
      outTexCoord.extent.x = (F32(srcRegion.extent.x) / scaleFactor) / scratchSize.x;
      outTexCoord.extent.y = (F32(srcRegion.extent.y) / scaleFactor) / scratchSize.y;

      outTexCoord.point.x += (sources[0]->texelOffset() / scratchSize.x);
      outTexCoord.point.y += (sources[0]->texelOffset() / scratchSize.y);
   }

//    Con::printf(" %x srcRegion=(%d, %d, %d, %d) outTexCoord=(%f, %f, %f, %f)",
//       source,
//       srcRegion.point.x, srcRegion.point.y, srcRegion.extent.x, srcRegion.extent.y,
//       outTexCoord.point.x, outTexCoord.point.y, outTexCoord.extent.x, outTexCoord.extent.y);


   AssertFatal(uploadLevel != -1, "calcScaleRegionStuff - didn't set uploadLevel, somehow skipped all our upload setup logic.");

   // Lock the texture and upload data from the source.

   // NOTE: The GTextureHandle::lock() call was broken at one time and
   // used a reversed axis standard.  It has since been fixed, but the Atlas
   // clip map logic has not be fully corrected for this.  So we're swapping
   // the lock axis here until it is fixed properly.
   RectI swappedLock(0, 0, uploadRegion.extent.y, uploadRegion.extent.x);

   for (U32 i = 0; i < sources.size(); i++)
   {
      U32 scratchPerSource = 1;

      if (fixedFunction)
         scratchPerSource = 4;

      for (U32 component = 0; component < scratchPerSource; component++)
      {
         U32 expand = 0;

         if (fixedFunction)
            expand = component < 3 ? (3 - component) : 4;

         GLockedRect *glr = scratchTexs[i * scratchPerSource + component].lock(0, &swappedLock);
            AssertFatal(glr, "ClipMapBlenderCache::calcScaleRegionStuff - couldn't lock scratch texture");
         sources[i]->copyBits(uploadLevel, NULL, uploadRegion, glr->bits, glr->pitch, expand );
         scratchTexs[i * scratchPerSource + component].unlock();
      }
   }
}

void ClipMapBlenderCache::doRectUpdate( U32 mipLevel, ClipMap::ClipStackEntry &cse, const RectI &srcRegion, const RectI &dstRegion )
{
   AssertFatal(pushDepth==1, "ClipMapBlenderCache::doRectUpdate - "
                                             "someone pushed more than once.");

   // Hopefully we never hit these but it is a pretty quick check just in case
   if (mFixedfunction && mOpacityScratchTextures.size() < mOpacitySources.size() * 4)
      createOpacityScratchTextures();
   else if (!mFixedfunction && mOpacityScratchTextures.size() < mOpacitySources.size())
      createOpacityScratchTextures();

   // First, figure out what mip level of the image sources we need to copy
   // data from. Image sources actually work in real mip levels (level 0 is
   // full size, level N is 1px square), so we need to figure the level
   // we want and scale it up as appropriate.

   const F32 levelSize = cse.mScale * mClipMapSize;

   RectF opacityTC;
   calcScaleRegionStuff(mOpacitySources, levelSize, srcRegion,
      mOpacityScratchTextures, opacityTC, mFixedfunction);

   RectF lightmapTC;
   calcScaleRegionStuff(mLightmapSources, levelSize, srcRegion,
      mLightmapScratchTextures, lightmapTC, false);

   RectF masterRect;
   masterRect.point.x = F32(srcRegion.point.x) / levelSize;
   masterRect.point.y = F32(srcRegion.point.y) / levelSize;
   masterRect.extent.x = F32(srcRegion.extent.x) / levelSize;
   masterRect.extent.y = F32(srcRegion.extent.y) / levelSize;

   if (mFixedfunction)
   {
      // Do 5 pass (per opacity) fixedfunction
      // Draw first layer
      GRAPHIC->setStateBlock(mFFBaseLayerSB);
      setupGeometryFF(opacityTC, masterRect, mScales[0], dstRegion);
      GRAPHIC->setTexture(0, mOpacityScratchTextures[0]);
      GRAPHIC->setTexture(1, mSourceImages[0]);
      GRAPHIC->setupGenericShaders();
      GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );

      // Apply additional layers
      GRAPHIC->setStateBlock(mFFAdditionalLayersSB);
      for(S32 i=1; i<mOpacityScratchTextures.size(); i++)
      {
         setupGeometryFF(opacityTC, masterRect, mScales[i], dstRegion);
         GRAPHIC->setTexture(0, mOpacityScratchTextures[i]);
         GRAPHIC->setTexture(1, mSourceImages[i]);
         GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );
      }
      // Multiply by lightmap*2
      if (mLightmapScratchTextures.size() > 0)
      {
         setupGeometryFF(lightmapTC, masterRect, 0, dstRegion);
         GRAPHIC->setStateBlock(mFFLightmapSB);

         for (U32 i = 0; i < mLightmapScratchTextures.size(); i++)
         {
            GRAPHIC->setTexture(0, mLightmapScratchTextures[i]);
            GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );
         }
      }
   }
   else
   {
      // Set up our geometry.
      setupGeometry(
         opacityTC,
         lightmapTC,
         masterRect,
         dstRegion);

      // And draw our blended quad...
      GRAPHIC->setTexture(0, mOpacityScratchTextures[0]); 
      GRAPHIC->setTexture(1, mLightmapScratchTextures[0]); 

      if(GRAPHIC->getPixelShaderVersion() < 2.0f)
      {
         // Do two pass shader w/ additive blending.
         GRAPHIC->setShader(mTwoPass[0]);
         GRAPHIC->setStateBlock(mOnePassSB);
         GRAPHIC->setTexture(2, (0 < mSourceImages.size()) ? mSourceImages[0] : NULL);
         GRAPHIC->setTexture(3, (1 < mSourceImages.size()) ? mSourceImages[1] : NULL);
         GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );

         GRAPHIC->setShader(mTwoPass[1]);
         GRAPHIC->setStateBlock(mTwoPassSB);
         GRAPHIC->setTexture(2, (2 < mSourceImages.size()) ? mSourceImages[2] : NULL);
         GRAPHIC->setTexture(3, (3 < mSourceImages.size()) ? mSourceImages[3] : NULL);
         GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );

         // Additional passes for extra opacity maps
         for (U32 i = 1; i < mOpacityScratchTextures.size(); i++)
         {
            GRAPHIC->setTexture(0, mOpacityScratchTextures[i]);

            mTempScaleFactors.set(mScales[i * 4], mScales[i * 4 + 1], mScales[i * 4 + 2], mScales[i * 4 + 3]);
            mShaderConsts->set(mSourceTexScalesSC, mTempScaleFactors);

            GRAPHIC->setShader(mTwoPass[0]);
            GRAPHIC->setStateBlock(mOnePassSB);
            GRAPHIC->setTexture(2, (i * 4     < mSourceImages.size()) ? mSourceImages[i * 4]     : NULL);
            GRAPHIC->setTexture(3, (i * 4 + 1 < mSourceImages.size()) ? mSourceImages[i * 4 + 1] : NULL);
            GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );

            GRAPHIC->setShader(mTwoPass[1]);
            GRAPHIC->setStateBlock(mTwoPassSB);
            GRAPHIC->setTexture(2, (i * 4 + 2 < mSourceImages.size()) ? mSourceImages[i * 4 + 2] : NULL);
            GRAPHIC->setTexture(3, (i * 4 + 3 < mSourceImages.size()) ? mSourceImages[i * 4 + 3] : NULL);
            GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );
         }
      }
      else
      {
         // Do one pass per opacity.
         GRAPHIC->setShader(mOnePass);
         GRAPHIC->setStateBlock(mOnePassSB);
         
         GRAPHIC->setTexture(2, (0 < mSourceImages.size()) ? mSourceImages[0] : NULL);
         GRAPHIC->setTexture(3, (1 < mSourceImages.size()) ? mSourceImages[1] : NULL);
         GRAPHIC->setTexture(4, (2 < mSourceImages.size()) ? mSourceImages[2] : NULL);
         GRAPHIC->setTexture(5, (3 < mSourceImages.size()) ? mSourceImages[3] : NULL);

         GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );

         GRAPHIC->setStateBlock(mTwoPassSB);

         for (U32 i = 1; i < mOpacityScratchTextures.size(); i++)
         {
            GRAPHIC->setTexture(0, mOpacityScratchTextures[i]);

            mTempScaleFactors.set(mScales[i * 4], mScales[i * 4 + 1], mScales[i * 4 + 2], mScales[i * 4 + 3]);
            mShaderConsts->set(mSourceTexScalesSC, mTempScaleFactors);

            GRAPHIC->setTexture(2, (i * 4     < mSourceImages.size()) ? mSourceImages[i * 4]     : NULL);
            GRAPHIC->setTexture(3, (i * 4 + 1 < mSourceImages.size()) ? mSourceImages[i * 4 + 1] : NULL);
            GRAPHIC->setTexture(4, (i * 4 + 2 < mSourceImages.size()) ? mSourceImages[i * 4 + 2] : NULL);
            GRAPHIC->setTexture(5, (i * 4 + 3 < mSourceImages.size()) ? mSourceImages[i * 4 + 3] : NULL);
            GRAPHIC->drawPrimitive( GTriangleStrip, 0, 2 );
         }
      }
   }

   GRAPHIC->setVertexBuffer( NULL );
}

void ClipMapBlenderCache::finishRectUpdates( ClipMap::ClipStackEntry &cse )
{
   pushDepth--;
   AssertFatal(pushDepth == 0, "BAD2");

   // And restore the render target.
   GRAPHIC->popActiveRenderTarget();

#ifndef TORQUE_OS_XENON
   // Extrude mips...
//   ((GD3D9TextureObject*)cse.mTex.getPointer())->get2DTex()->GenerateMipSubLevels();
#else
#  pragma message("Gotta do a stretchrect here pat, Love Pat." )
#endif

   // Reset texture stages.
   // For sanity, let's just purge all our texture states.
   for(S32 i=0; i<GRAPHIC->getNumSamplers(); i++)
      GRAPHIC->setTexture(i, NULL);

   // And clear render states.
   GRAPHIC->popWorldMatrix();
   GRAPHIC->setProjectionMatrix(proj);
   // Don't reset viewport if saved value is zero size rect or if there is no active render target.
   // it will cause d3d error.  This can happen if clipmap stuff is updated
   // before the canvas has rendered a frame (e.g., in Instant Action website).
   if (GRAPHIC->getActiveRenderTarget() != NULL && viewPort.len_x() > 0 && viewPort.len_y() > 0)
      GRAPHIC->setViewport(viewPort);   

   // Reset the shader constants
   GRAPHIC->setShaderConstBuffer(NULL);   

   if(mustEndScene)
      GRAPHIC->endScene();
}

void ClipMapBlenderCache::setupGeometry(
   const RectF &srcRect0,
   const RectF &srcRect1,
   const RectF &masterCoords,
   const RectI &dstRect)
{
   GVertexBufferHandle<GVertexPTTT> verts(GRAPHIC, 4, GBufferTypeVolatile );

   const F32 masterLeft   = F32(masterCoords.point.y);
   const F32 masterRight  = F32(masterCoords.point.y + masterCoords.extent.y);
   const F32 masterTop    = F32(masterCoords.point.x);
   const F32 masterBottom = F32(masterCoords.point.x + masterCoords.extent.x);

   const F32 tex0Left   = F32(srcRect0.point.y)                     / 1.0f; //F32(mClipMapSize);
   const F32 tex0Right  = F32(srcRect0.point.y + srcRect0.extent.y) / 1.0f; //F32(mClipMapSize);
   const F32 tex0Top    = F32(srcRect0.point.x)                     / 1.0f; //F32(mClipMapSize);
   const F32 tex0Bottom = F32(srcRect0.point.x + srcRect0.extent.x) / 1.0f; //F32(mClipMapSize);

   const F32 tex1Left   = F32(srcRect1.point.y)                     / 1.0f; //F32(mClipMapSize);
   const F32 tex1Right  = F32(srcRect1.point.y + srcRect1.extent.y) / 1.0f; //F32(mClipMapSize);
   const F32 tex1Top    = F32(srcRect1.point.x)                     / 1.0f; //F32(mClipMapSize);
   const F32 tex1Bottom = F32(srcRect1.point.x + srcRect1.extent.x) / 1.0f; //F32(mClipMapSize);

   const F32 screenLeft   = F32(dstRect.point.y);
   const F32 screenRight  = F32(dstRect.point.y + dstRect.extent.y);
   const F32 screenTop    = F32(dstRect.point.x);
   const F32 screenBottom = F32(dstRect.point.x + dstRect.extent.x);

   const F32 fillConv = GRAPHIC->getFillConventionOffset();

   verts.lock();

   verts[0].point.set( screenLeft  - fillConv, screenTop    - fillConv, 0.0f );
   verts[0].texCoord1.set( tex0Left,  tex0Top );
   verts[0].texCoord2.set( tex1Left,  tex1Top );
   verts[0].texCoord3.set( masterLeft,  masterTop );

   verts[1].point.set( screenRight - fillConv, screenTop    - fillConv, 0.0f );
   verts[1].texCoord1.set( tex0Right, tex0Top );
   verts[1].texCoord2.set( tex1Right, tex1Top );
   verts[1].texCoord3.set( masterRight, masterTop );

   verts[2].point.set( screenLeft  - fillConv, screenBottom - fillConv, 0.0f );
   verts[2].texCoord1.set( tex0Left,  tex0Bottom );
   verts[2].texCoord2.set( tex1Left,  tex1Bottom );
   verts[2].texCoord3.set( masterLeft,  masterBottom );

   verts[3].point.set( screenRight - fillConv, screenBottom - fillConv, 0.f );
   verts[3].texCoord1.set( tex0Right, tex0Bottom );
   verts[3].texCoord2.set( tex1Right, tex1Bottom );
   verts[3].texCoord3.set( masterRight, masterBottom );

   verts.unlock();

   GRAPHIC->setVertexBuffer( verts );
}

void ClipMapBlenderCache::setupGeometryFF(
   const RectF &srcRect0,
   const RectF &masterCoords,
   const F32 scaleFactor,
   const RectI &dstRect)
{
   GVertexBufferHandle<GVertexPTT> verts(GRAPHIC, 4, GBufferTypeVolatile );

   const F32 masterLeft   = F32(masterCoords.point.y)*scaleFactor;
   const F32 masterRight  = F32(masterCoords.point.y + masterCoords.extent.y)*scaleFactor;
   const F32 masterTop    = F32(masterCoords.point.x)*scaleFactor;
   const F32 masterBottom = F32(masterCoords.point.x + masterCoords.extent.x)*scaleFactor;

   const F32 tex0Left   = F32(srcRect0.point.y)                     / 1.0f; //F32(mClipMapSize);
   const F32 tex0Right  = F32(srcRect0.point.y + srcRect0.extent.y) / 1.0f; //F32(mClipMapSize);
   const F32 tex0Top    = F32(srcRect0.point.x)                     / 1.0f; //F32(mClipMapSize);
   const F32 tex0Bottom = F32(srcRect0.point.x + srcRect0.extent.x) / 1.0f; //F32(mClipMapSize);

   const F32 screenLeft   = F32(dstRect.point.y);
   const F32 screenRight  = F32(dstRect.point.y + dstRect.extent.y);
   const F32 screenTop    = F32(dstRect.point.x);
   const F32 screenBottom = F32(dstRect.point.x + dstRect.extent.x);

   const F32 fillConv = GRAPHIC->getFillConventionOffset();

   verts.lock();

   verts[0].point.set( screenLeft  - fillConv, screenTop    - fillConv, 0.0f );
   verts[0].texCoord1.set( tex0Left,  tex0Top );
   verts[0].texCoord2.set( masterLeft,  masterTop );

   verts[1].point.set( screenRight - fillConv, screenTop    - fillConv, 0.0f );
   verts[1].texCoord1.set( tex0Right, tex0Top );
   verts[1].texCoord2.set( masterRight, masterTop );

   verts[2].point.set( screenLeft  - fillConv, screenBottom - fillConv, 0.0f );
   verts[2].texCoord1.set( tex0Left,  tex0Bottom );
   verts[2].texCoord2.set( masterLeft,  masterBottom );

   verts[3].point.set( screenRight - fillConv, screenBottom - fillConv, 0.0f );
   verts[3].texCoord1.set( tex0Right, tex0Bottom );
   verts[3].texCoord2.set( masterRight, masterBottom );

   verts.unlock();

   GRAPHIC->setVertexBuffer( verts );
}

void ClipMapBlenderCache::registerSourceImage( const String &filename )
{
   mSourceImages.increment();
   mSourceWidths.push_back(256.0f);

   if(filename.isEmpty())
   {
      // If we are fed blank, we need to have a blank source
      // for the shader.
      GBitmap* bmp = new GBitmap(2,2);
      dMemset(bmp->getWritableBits(0), 0, bmp->getByteSize());
      mSourceImages.last().set(bmp, &GDefaultStaticDiffuseProfile, true, String("ClipmapBlender Source Image"));
      return;
   }    

   Resource<GBitmap> bmp = GBitmap::load(filename);
   if(bmp != NULL)
   {
      // GTextureHandle is going to own this, so let's copy it.
      GBitmap  *newBM = new GBitmap(*bmp);
      newBM->extrudeMipLevels();
      mSourceImages.last().set(newBM, &GDefaultStaticDiffuseProfile, true, String("ClipmapBlender Source File Image"));
   }

   if(mSourceImages.last().isValid())
      mSourceWidths.last() = mSourceImages.last().getWidth();

   // If we can't find it, complain.
   if(!mSourceImages.last() && !filename.isEmpty())
      Con::warnf("ClipMapBlenderCache::registerSourceImage - couldn't locate texture '%s' for blending.", filename.c_str());
}

void ClipMapBlenderCache::clearSourceImages()
{
   mSourceImages.clear();
   mScales.clear();
}

void ClipMapBlenderCache::clearOpacitySources()
{
   mOpacitySources.clear();
}

void ClipMapBlenderCache::registerOpacitySource(IClipMapImageSource *opacitySource)
{
   mOpacitySources.push_back(opacitySource);
}

void ClipMapBlenderCache::setNewLightmapSource(IClipMapImageSource *lightmapSource)
{
   IClipMapImageSource * currLightMapSource = mLightmapSources.last();
   SAFE_DELETE(currLightMapSource);
   mLightmapSources.last() = lightmapSource;
}
