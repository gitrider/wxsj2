//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _TERRBATCH_H_
#define _TERRBATCH_H_


namespace TerrBatch
{

   // We buffer everything here then send it off to the GPU
   extern GAtlasVert2*       mVertexStore;
   extern GVertexPCNT*       mVertexStorePCNT;
   extern GVertexPCNTT*      mVertexStorePCNTT;
   extern U16                  mCurVertex;
   extern U16*                 mIndexStore;
   extern U16                  mCurIndex;
   extern GPrimitive*        mPrimitiveStore;
   extern U16                  mCurPrim;
   extern U16                  mCurTexture;
   extern U32                  mCurXF;

   // Store previous counts for batching
   extern U16                  mLastIndex;
   extern U16                  mLastVertex;

   // Statistics
   extern S32                  mBiggestVertexBatch;
   extern S32                  mBiggestIndexBatch;

   enum vertexType
   {
      vertexTypeGeneric,
      vertexTypeFullClipMapping,
      vertexTypeSingleTextureClipMapping,
      vertexTypeDetail,
      vertexTypeDLight,
      vertexTypeFog,
   };

   void init();
   void destroy();
   void begin();

   // Stuff a point into the store and return its index...
   inline U16 emit(const EdgePoint &ep)
   {
      mVertexStore[mCurVertex].point  = ep;

      // Get the normal...
      static Point2F p2;
      p2.x = ep.x;
      p2.y = ep.y;
      if (!TerrainRender::mCurrentBlock->getNormal(p2, &(mVertexStore[mCurVertex].normal)))
         mVertexStore[mCurVertex].normal.set(0,0,1);

      return mCurVertex++;
   }

   inline U16 emit(const Point3F &p)
   {
      mVertexStore[mCurVertex].point  = p;

      // Get the normal...
      static Point2F p2;
      p2.x = p.x;
      p2.y = p.y;

      if (!TerrainRender::mCurrentBlock->getNormal(p2, &(mVertexStore[mCurVertex].normal)))
         mVertexStore[mCurVertex].normal.set(0,0,1);

      return mCurVertex++;
   }

   // Implicitly get z from height at this position...
   inline U16 emit(S32 x, S32 y)
   {
      GAtlasVert2 &pt = mVertexStore[mCurVertex];

      // Set things up via magic...
      pt.point.x = F32(x * TerrainRender::mSquareSize);
      pt.point.y = F32(y * TerrainRender::mSquareSize);
      pt.point.z = fixedToFloat(TerrainRender::mCurrentBlock->getHeight(x, y));

      // Get the normal...
      static Point2F p2;
      p2.x = pt.point.x;
      p2.y = pt.point.y;

      if (!TerrainRender::mCurrentBlock->getNormal(p2, &(pt.normal)))
         pt.normal.set(0,0,1);

      return mCurVertex++;
   }

   inline U16 emitInterp(U16 p1, U16 p2, S32 x, S32 y, F32 growFactor)
   {
      GAtlasVert2 &pt = mVertexStore[mCurVertex];

      pt.point.x  = F32(x * TerrainRender::mSquareSize);
      pt.point.y  = F32(y * TerrainRender::mSquareSize);
      pt.point.z  = fixedToFloat(TerrainRender::mCurrentBlock->getHeight(x, y));
      pt.point.z += growFactor * (((mVertexStore[p1].point.z + mVertexStore[p2].point.z) * 0.5f) - pt.point.z);

      // Get the normal...
      static Point2F np;
      np.x = pt.point.x;
      np.y = pt.point.y;

      if (!TerrainRender::mCurrentBlock->getNormal(np, &(pt.normal)))
         pt.normal.set(0,0,1);

      return mCurVertex++;
   }

   inline void emit(ChunkEdge &edge)
   {
      // Deal with caching...
      if(edge.xfIndex == mCurXF)
         return;

      edge.xfIndex = mCurXF;

      // Insert vertices...
      switch(edge.pointCount)
      {
      case 1:
         edge.pointIndex = emit(* ((EdgePoint *) &edge.pt[1]));
         break;

      case 3:
         edge.pointIndex = emit(* ((EdgePoint *) &edge.pt[0]));
                           emit(* ((EdgePoint *) &edge.pt[1]));
                           emit(* ((EdgePoint *) &edge.pt[2]));
         break;
      default:
         // Do nothing...
         break;
      }
   }

   inline U16 emit(ChunkCornerPoint *p)
   {
      // Add the point if we haven't already...
      if(p->xfIndex != mCurXF)
      {
         p->pointIndex = emit(*((EdgePoint *) p));
         p->xfIndex = mCurXF;
      }

      return p->pointIndex;
   }

   // Emit a triangle made of points at indices a, b, c
   inline void emitTriangle(U16 a, U16 b, U16 c)
   {
      mIndexStore[mCurIndex++] = a;
      mIndexStore[mCurIndex++] = b;
      mIndexStore[mCurIndex++] = c;
   }

   // Suck the last two vertices in and make a new triangle...
   inline void emitFanStep(U16 a)
   {
      U16 root = mCurIndex;
      mIndexStore[mCurIndex++] = mIndexStore[root-3];
      mIndexStore[mCurIndex++] = mIndexStore[root-1];
      mIndexStore[mCurIndex++] = a;
   }

   void end(bool final, vertexType vtype, Point4F *clipmapMapping, const MatrixF &blockTransform, const Point3F &cameraPosition, LightInfo *light, SceneState *);
};



#endif
