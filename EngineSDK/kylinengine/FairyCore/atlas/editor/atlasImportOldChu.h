//-----------------------------------------------------------------------------
// Torque Game Engine Advanced Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ATLAS_EDITOR_ATLASIMPORTOLDCHU_H_
#define _ATLAS_EDITOR_ATLASIMPORTOLDCHU_H_

/// Helper class to assist in importing the old .chu format into an Atlas
/// file.
class AtlasImportOldCHUHelper
{
private:
   struct OldCHUHeader
   {
      U32 fourCC;
      S16 version;
      S16 treeDepth;
      F32 baseMaxError, verticalScale, leafDimensions;
      U32 chunkCount, colTreeDepth;
   };

   struct OldCHUStub
   {
      U32 sentinel; // == 0xDEADBEEFa
      S32 label;
      S32 neighbors[4];
      U8  level;
      S16 x,y;
      S16 min, max;
      U32 chunkOffset;
   };

   struct OldCHUChunk
   {
      U32 sentinel; // == 0xbeef1234

      U16 vertCount;
      struct Vert
      {
         S16 x,y,z,morph;
      } *verts;

      S32 indexCount;
      U16 *index;
      U32 triCount;
      U8 colDataFlag; // 0 or 1

      // if(colDataFlag)
      // {

      //for each coltree node (depth in tree)
      struct ColTreeNode
      {
         S16 min, max;
      };
      ColTreeNode *colTreeNodes;

      U32 sent2;                 //== 0xb33fd34d
      U16 *colIndicesOffsets;    // [colGridCount]
      U32 bufferSize; 
      U16 *colIndexBuffer;       //[bufferSize]

      // }

      U32 post; // == 0xb1e2e3f4

      OldCHUChunk();

      ~OldCHUChunk();

      bool read(Stream &chuFs, U32 colTreeCount, U32 colTreeGridCount);

      Point3F *pos;
      Point3F *morph;
      Point2F *texCoord;

      /// Given loaded data, go through and generate vertex arrays suitable
      /// for use in the new Atlas infrastructure.
      void generateVertexArrays(F32 verticalScale, F32 chunkSize, Point2F chunkOffset, Point2F terrainSize);
   };

   const char   *mChuFileName;
   FileStream   *mChuFile;
   AtlasFile    *mAtlasFile;
   AtlasResourceGeomTOC *mTOC;
   OldCHUHeader mHeader;

public:
   AtlasImportOldCHUHelper(const char *chuFile, AtlasFile *outFile);
   ~AtlasImportOldCHUHelper();

   bool registerTOC();
   bool populateChunks();

   AtlasTOC *getTOC()
   {
      return mTOC;
   }
};

#endif
