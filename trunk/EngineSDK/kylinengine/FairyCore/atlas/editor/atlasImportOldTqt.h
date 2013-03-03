//-----------------------------------------------------------------------------
// Torque Game Engine Advanced Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ATLAS_EDITOR_ATLASIMPORTOLDTQT_H_
#define _ATLAS_EDITOR_ATLASIMPORTOLDTQT_H_

/// Helper class to assist in importing an old-style .tqt file into a TOC
/// in an AtlasFile.
class AtlasImportOldTQTHelper
{
private:
   struct OldTQTHeader
   {
      U32 fourCC;
      U32 version;
      U32 treeDepth;
      U32 tileSize;
   };

   AtlasFile *mAtlasFile;

   const char *mTQTFileName;
   FileStream *mTQTFile;

   AtlasResourceTexTOC *mTOC;
   OldTQTHeader mHeader;

public:
   AtlasImportOldTQTHelper(const char *chuFile, AtlasFile *outFile);
   ~AtlasImportOldTQTHelper();

   void registerTOC();
   void populateChunks();

   AtlasTOC *getTOC()
   {
      return mTOC;
   }
};

#endif
