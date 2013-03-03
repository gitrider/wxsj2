//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef  _ATLAS_EDITOR_ATLASIMAGEIMPORT_H_
#define  _ATLAS_EDITOR_ATLASIMAGEIMPORT_H_

#ifndef _ATLASEXPORTINTERFACES_H_
class IAtlasImageImportSource
{
public:
   virtual void getExtents(int &width, int &height)=0;
   virtual void getColor(int x, int y, unsigned char &r, unsigned char &b, unsigned char &g, unsigned char &a)=0;
   virtual int getNumberChannels()=0;
   virtual void sampleChannels(int x, int y, unsigned char *out);
};
#endif

class AtlasImageImporter
{
   AtlasFile               *mAtlasFile;
   AtlasResourceTexTOC     *mTOC;
   U32                     mTreeDepth;
   IAtlasImageImportSource *mSource;

public:
   AtlasImageImporter(AtlasFile *atlasFile, IAtlasImageImportSource *source, U32 treeDepth);

   void registerTOC();
   void populateChunks();

   AtlasTOC *getTOC()
   {
      return mTOC;
   }
};

#endif
