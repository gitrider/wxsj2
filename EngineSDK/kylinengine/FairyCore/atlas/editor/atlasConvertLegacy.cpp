//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "atlas/core/atlasFile.h"
#include "atlas/resource/atlasResourceConfigTOC.h"
#include "atlas/resource/atlasResourceGeomTOC.h"
#include "atlas/resource/atlasResourceTexTOC.h"
#include "atlas/editor/atlasExportInterfaces.h"
#include "atlas/editor/atlasImageImport.h"
#include "terrain/terrData.h"

class U8BufferImageSource : public IAtlasImageImportSource
{
public:
   U8 *mSourceBuffer;
   U32 mSourceSize;
   U32 mNumberOfChannels;
   U32 mScale;
   bool mPreserve;

   U8BufferImageSource()
   {
      mScale = 1;
      mSourceBuffer = NULL;
      mSourceSize = 0;
      mNumberOfChannels = -1;
      mPreserve = false;
   }

   virtual void getExtents(int &width, int &height)
   {
      height = width = mSourceSize;
   }

   virtual void getColor(int x, int y, unsigned char &r, unsigned char &b, unsigned char &g, unsigned char &a)
   {
      const U32 off = (y * mSourceSize + x) * mNumberOfChannels;

      r = mClamp(U32(mSourceBuffer[off + 0])  *mScale, 0, 0xFF);

      if(mNumberOfChannels >= 2)
         b = mClamp(U32(mSourceBuffer[off + 1]) * mScale, 0, 0xFF);
      else
         b = 0;

      if(mNumberOfChannels >= 3)
         g = mClamp(U32(mSourceBuffer[off + 2]) * mScale, 0, 0xFF);
      else
         g = 0;

      if(mNumberOfChannels >= 4)
         a = mClamp(U32(mSourceBuffer[off + 3]) * mScale, 0, 0xFF);
      else
         a = 0;
   }

   virtual int getNumberChannels()
   {
      if(mPreserve)
         return mNumberOfChannels;
      else
         // Indicate we're not a multiplane image - just one layer.
         return 0;
   }

   virtual void sampleChannels(int x, int y, unsigned char *out)
   {
      const U32 off = (x * mSourceSize + y) * mNumberOfChannels;

      for(S32 i=0; i<getNumberChannels(); i++)
         out[i] = mClamp(mSourceBuffer[off + i] * mScale, 0x0, 0xFF);
   }

};

class LegacyTerrainInfo : public ITerrainInfo
{
public:

   TerrainBlock *mBlock;

   virtual float getMinHeight()
   {
      return fixedToFloat(mBlock->findSquare(TerrainBlock::BlockShift, Point2I(0,0))->minHeight);
   }

   virtual float getMaxHeight()
   {
      return fixedToFloat(mBlock->findSquare(TerrainBlock::BlockShift, Point2I(0,0))->maxHeight);
   }

   virtual int getWidth()
   {
      return TerrainBlock::BlockSize;
   }

   virtual int getHeight()
   {
      return TerrainBlock::BlockSize;
   }

   virtual float getSampleSpacing()
   {
      return mBlock->getSquareSize();
   }

   virtual float getAltitude(int x, int y)
   {
      return fixedToFloat(mBlock->getHeight(x,y));
   }

   virtual void handleLog(const char *str)
   {
      Con::printf("%s", str);
   }

   virtual void startProgress(const char *title)
   {
   }

   virtual void updateProgress(int curAmt, int totalAmt, const char *newTitle)
   {
   }

   virtual void endProgress()
   {
   }

   virtual bool hasMaterialFilenames()
   {
      return true;
   }

   virtual const char *getMaterialName(const int i)
   {
      U32 matCount = 0;
      for(S32 j=0; j<TerrainBlock::MaterialGroups; j++)
      {
         if(mBlock->mMaterialFileName[j].isEmpty())
            continue;

         if(matCount == i)
            return mBlock->mMaterialFileName[j];

         matCount++;
      }

      return NULL;
   }
};

ConsoleFunction(atlasExportLegacy, void, 3, 3, "(TerrainBlock terrainObj, filename outAtlasFile)"
                "Import a legacy terrain file (.ter) and store the resulting atlas file in the "
                "output directory.")
{
   // Grab the active terrain object.
   TerrainBlock *terrObj = NULL;

   if(!Sim::findObject(argv[1], terrObj))
   {
      Con::errorf("atlasExportLegacy - Failed to find object '%s'", argv[1]);
      return;
   }

   // Set up export sources from the object.
   LegacyTerrainInfo legacyInfo;
   legacyInfo.mBlock = terrObj;

   // Lightmap
   U8BufferImageSource lightSource;

   GBitmap *lightBmp = terrObj->lightMap;
   AssertFatal(lightBmp->getWidth() == lightBmp->getHeight(), "atlasExportLegacy - non-square lightmap.");

   lightSource.mSourceBuffer     = lightBmp->getAddress(0,0,0);
   lightSource.mSourceSize       = lightBmp->getWidth();
   lightSource.mNumberOfChannels = 3;
   lightSource.mScale            = 2;
   lightSource.mPreserve         = false;

   // Grab the first four materials and their opacity maps.
   Vector<const char *> matFile;
   U8BufferImageSource opacitySource;
   opacitySource.mNumberOfChannels = 4;
   opacitySource.mSourceBuffer     = new U8[TerrainBlock::BlockSize * TerrainBlock::BlockSize * 4];
   opacitySource.mSourceSize       = TerrainBlock::BlockSize;
   opacitySource.mPreserve         = true;

   StringTableEntry terrFilePath, terrFileName;
   gResourceManager->getPaths(terrObj->getFile().getPath().getFullPath(), terrFilePath, terrFileName);

   for(S32 i=0; i<TerrainBlock::MaterialGroups; i++)
   {
      // Only count alpha maps with specified textures.
      if(terrObj->mMaterialFileName[i].isEmpty())
         continue;

      // Store out this material filename.
      const char *matName = terrObj->mMaterialFileName[i];
      matFile.push_back(Platform::makeRelativePathName(matName, terrFilePath));

      // Now copy data into the image source.
      for(S32 x=0; x<TerrainBlock::BlockSize; x++)
      {
         for(S32 y=0; y<TerrainBlock::BlockSize; y++)
         {
            opacitySource.mSourceBuffer[(x*TerrainBlock::BlockSize + y) * 4 + (matFile.size()-1)]
               = terrObj->getMaterialAlphaMap(matFile.size()-1)[(x*TerrainBlock::BlockSize + y)];
         }
      }
   }
   
   // Call the export code.
   if(!atlasGenerateFromAtlasInterface(argv[2], &legacyInfo, &lightSource, &opacitySource ))
   {
      // Clean up the source buffer.
      delete [] opacitySource.mSourceBuffer;

      Con::errorf("atlasExportLegacy - export to '%s' failed!", argv[2]);
      return;
   }

   // Clean up the source buffer.
   delete [] opacitySource.mSourceBuffer;

   Con::printf("atlasExportLegacy - export to '%s' successful.", argv[2]);
}

//-----------------------------------------------------------------------------

/// Simple struct to hold interesting parts of .ter file and encapsulate
/// IO.
struct TerrainFileData
{
   struct Layer
   {
      U8 *mOpacity;
      StringTableEntry mTexture;

      Layer()
      {
         mOpacity = NULL;
         mTexture = NULL;
      }

      ~Layer()
      {
         SAFE_DELETE_ARRAY(mOpacity);
      }
   };

   Vector<Layer> mOpacityMap;
   U16 *mHeightField;

   // These mirror the values in TerrainBlock - we dupe them to avoid dependencies.
   const static U32 csmExpectedFileVersion = 4; ///< Same as TerrainFile::FILE_VERSION
   const static U32 csmBlockSize = 256;         ///< Same as TerrainBlock::BlockSize
   const static U32 csmBlockMask = 0xFF;        ///< Same as TerrainBlock::BlockMask
   const static U32 csmMaxLayers = 8;           ///< Same as TerrainBlock::MaterialGroups

   bool readFromStream(Stream &stream)
   {
      U8 version;
      stream.read(&version);
      if (version != csmExpectedFileVersion)
      {
         Con::errorf("TerrainFileData::readFromStream - unexpected version %d, can only read version %d.",
            version, csmExpectedFileVersion);
         return false;
      }

      // Read the heightfield
      bool ok = true;
      mHeightField = new U16[csmBlockSize*csmBlockSize];
      for(S32 i=0; i<csmBlockSize*csmBlockSize; i++)
         ok &= stream.read(mHeightField + i);

      if(!ok)
      {
         Con::errorf("TerrainFileData::readFromStream - failed to read heightfield data.");
         return false;
      }

      // Skip the material group map and flags.
      for (S32 j=0; j < (csmBlockSize * csmBlockSize); j++)
      {
         U8 garbage;
         ok &= stream.read(&garbage);
      }

      if(!ok)
      {
         Con::errorf("TerrainFileData::readFromStream - failed to read material group map data.");
         return false;
      }

      // Process the material list.
      for(S32 k=0; k<csmMaxLayers; k++)
      {
         StringTableEntry matFile = stream.readSTString(true);

         if(matFile && matFile[0])
         {
            mOpacityMap.increment();
            mOpacityMap.last().mTexture = matFile;
         }
      }

      for(S32 i=0; i<mOpacityMap.size(); i++)
      {
         mOpacityMap[i].mOpacity = new U8[csmBlockSize*csmBlockSize];
         ok = stream.read(csmBlockSize*csmBlockSize, mOpacityMap[i].mOpacity);

         if(!ok)
         {
            Con::errorf("TerrainFileData::readFromStream - failed to read opacity map #%d", i);
            return false;
         }
      }

      // All done - we skip the heightfield/texture scripts.
      return true;
   }

   F32 getHeight(S32 x, S32 y)
   {
      x &= csmBlockMask;
      y &= csmBlockMask;

      return fixedToFloat(mHeightField[x*csmBlockSize+y]);
   }

   /// Generate the normal for a given location on the heightfield.
   Point3F calculateNormal(S32 x, S32 y, const F32 squareSize)
   {
      Point3F normal;
      x &= csmBlockMask;
      y &= csmBlockMask;

      F32 zBottomLeft  = fixedToFloat(getHeight(x, y));
      F32 zTopLeft     = fixedToFloat(getHeight(x, y + 1));
      F32 zTopRight    = fixedToFloat(getHeight(x + 1, y + 1));

      normal.set(zTopLeft-zTopRight, zBottomLeft-zTopLeft, (F32)squareSize);
      normal.normalize();
      return normal;
   }

   GBitmap *generateLightmap(Point3F lightVec, ColorF lightDiffuse, ColorF lightAmbient, F32 squareSize)
   {
      // Do some simple dot-normal light calculations.
      GBitmap *lm = new GBitmap(csmBlockSize, csmBlockSize);
      lightVec.normalize();

      for(S32 i=0; i<csmBlockSize; i++)
      {
         for(S32 j=0; j<csmBlockSize; j++)
         {
            // Get the normal.
            Point3F n = calculateNormal(i,j, 1.f);

            // Determine lighting factor
            F32 lightFac = mClampF(mDot(lightVec, n), 0.f, 1.0f);

            ColorF lightHere = (lightDiffuse * lightFac) + lightAmbient;

            // This block will dump a normal map instead of a lightmap -
            // good for debugging!
            //lightHere.red   = n.x * 0.5 + 0.5f;
            //lightHere.green = n.y * 0.5 + 0.5f;
            //lightHere.blue  = n.z * 0.5 + 0.5f;

            // Optionally downscale by half.
            //lightHere *= 0.5f;
            lightHere.clamp();

            ColorI bmpColor = lightHere;

            lm->setColor(i,j,bmpColor);
         }
      }

      return lm;
   }
};

/// Interface to support height queries & other fun stuff against a TER file
class TerrainFileDataInfo : public ITerrainInfo
{
public:

   TerrainFileData *mFile;

   static const U32 csmBlockSize = 256;

   virtual float getMinHeight()
   {
      // Walk all the heights and find the lowest one.
      F32 minHeight = F32_MAX;
      for(S32 i=0; i<csmBlockSize; i++)
      {
         for(S32 j=0; j<csmBlockSize; j++)
         {
            F32 thisHeight = fixedToFloat(mFile->mHeightField[i*csmBlockSize+j]);
            if(thisHeight < minHeight)
               minHeight = thisHeight;
         }
      }

      return minHeight;
   }

   virtual float getMaxHeight()
   {
      // Walk all the heights and find the highest one.
      F32 maxHeight = -F32_MAX;
      for(S32 i=0; i<csmBlockSize; i++)
      {
         for(S32 j=0; j<csmBlockSize; j++)
         {
            F32 thisHeight = fixedToFloat(mFile->mHeightField[i*csmBlockSize+j]);
            if(thisHeight > maxHeight)
               maxHeight = thisHeight;
         }
      }

      return maxHeight;
   }

   virtual int getWidth()
   {
      return csmBlockSize;
   }

   virtual int getHeight()
   {
      return csmBlockSize;
   }

   virtual float getSampleSpacing()
   {
      // Normally on the live TerrainBlock, so we just fake it and choose 8.f
      // which is the default.
      return 8.f;
   }

   virtual float getAltitude(int x, int y)
   {
      return fixedToFloat(mFile->mHeightField[x*csmBlockSize+y]);
   }

   virtual void handleLog(const char *str)
   {
      Con::printf("%s", str);
   }

   virtual void startProgress(const char *title)
   {
   }

   virtual void updateProgress(int curAmt, int totalAmt, const char *newTitle)
   {
   }

   virtual void endProgress()
   {
   }

   virtual bool hasMaterialFilenames()
   {
      return true;
   }

   virtual const char *getMaterialName(const int i)
   {
      if(i >= mFile->mOpacityMap.size())
         return NULL;

      return mFile->mOpacityMap[i].mTexture;
   }
};

ConsoleFunction( exportAtlasFromLegacyTER, void, 3, 7, "(filename terFile, filename atlasFile, [Point3F sunDir, [ColorF sunColor, [ColorF sunAmbient, [F32 squareSize]]]])")
{
   // Parse our various arguments.
   Point3F sunDir(0.57f, -0.57f, 0.57f);
   ColorF sunColor(1, 1, 1), sunAmbient(1,1,1);
   F32 squareSize = 8.0f;

   if(argc >= 4)
      dSscanf(argv[3], "%g %g %g", &sunDir.x, &sunDir.y, &sunDir.z);

   if(argc >= 5)
      dSscanf(argv[4], "%g %g %g", &sunColor.red, &sunColor.green, &sunColor.blue);

   if(argc >= 6)
      dSscanf(argv[5], "%g %g %g", &sunAmbient.red, &sunAmbient.green, &sunAmbient.blue);

   if(argc >= 7)
      squareSize = dAtof(argv[6]);

   // Open the .ter file and parse it.
   FileStream fs;
   if(!fs.open(argv[1], Torque::FS::File::Read))
   {
      Con::errorf("exportAtlasFromLegacyTER - failed to open '%s'", argv[1]);
      return;
   }

   TerrainFileData tfd;
   if(!tfd.readFromStream(fs))
   {
      Con::errorf("exportAtlasFromLegacyTER - failed to process '%s'", argv[1]);
      return;
   }

   // Set up export sources from the object.
   TerrainFileDataInfo terInfo;
   terInfo.mFile = &tfd;

   // Lightmap
   U8BufferImageSource lightSource;

   sunDir.normalize();

   GBitmap *lightBmp = tfd.generateLightmap(sunDir, sunColor, sunAmbient, squareSize);
   AssertFatal(lightBmp->getWidth() == lightBmp->getHeight(), "atlasExportLegacy - non-square lightmap.");

   lightSource.mSourceBuffer     = lightBmp->getAddress(0,0,0);
   lightSource.mSourceSize       = lightBmp->getWidth();
   lightSource.mNumberOfChannels = 3;
   lightSource.mScale            = 2;
   lightSource.mPreserve         = false;

   // Grab the first four materials and their opacity maps.
   Vector<const char *> matFile;
   U8BufferImageSource opacitySource;
   opacitySource.mNumberOfChannels = 4;
   opacitySource.mSourceBuffer     = new U8[TerrainFileData::csmBlockSize * TerrainFileData::csmBlockSize * 4];
   opacitySource.mSourceSize       = TerrainFileData::csmBlockSize;
   opacitySource.mPreserve         = true;

   for(S32 i=0; i<tfd.mOpacityMap.size(); i++)
   {
      // Only count alpha maps with specified textures.
      if(!tfd.mOpacityMap[i].mTexture || !tfd.mOpacityMap[i].mTexture[0])
         continue;

      // Store out this material filename.
      matFile.push_back(tfd.mOpacityMap[i].mTexture);

      // Now copy data into the image source.
      for(S32 x=0; x<TerrainFileData::csmBlockSize; x++)
      {
         for(S32 y=0; y<TerrainFileData::csmBlockSize; y++)
         {
            opacitySource.mSourceBuffer[(x*TerrainFileData::csmBlockSize + y) * 4 + (matFile.size()-1)]
            = tfd.mOpacityMap[i].mOpacity[(x*TerrainFileData::csmBlockSize + y)];
         }
      }
   }

   // Call the export code.
   if(!atlasGenerateFromAtlasInterface(argv[2], &terInfo, &lightSource, &opacitySource ))
   {
      // Clean up the sources.
      delete [] opacitySource.mSourceBuffer;
      delete lightBmp;

      Con::errorf("atlasExportLegacy - export to '%s' failed!", argv[2]);
      return;
   }

   // Clean up the sources.
   delete [] opacitySource.mSourceBuffer;
   delete lightBmp;

   Con::printf("atlasExportLegacy - export to '%s' successful.", argv[2]);
}