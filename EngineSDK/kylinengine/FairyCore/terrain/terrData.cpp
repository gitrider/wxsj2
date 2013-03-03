//-----------------------------------------------------------------------------

// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "core/resourceManager.h"
#include "core/stream/fileStream.h"
#include "core/stream/bitStream.h"
#include "core/util/fourcc.h"
#include "core/util/path.h"
#include "math/mathIO.h"

#include "terrain/terrRender.h"
#include "console/consoleTypes.h"
#include "core/util/safeDelete.h"
#include "client/objectTypes.h"
#include "renderManager/renderPassManager.h"
#include "sceneManager/sceneState.h"
#include "missionEditor/terrainEditor.h"
#include "missionEditor/terraformer.h"
#include "materials/materialManager.h"
#include "graphic/gTextureManager.h"


IMPLEMENT_CO_NETOBJECT_V1(TerrainBlock);


Signal<void(U32,TerrainBlock*,const Point2I& ,const Point2I&)> TerrainBlock::smUpdateSignal;

//RBP - Global function declared in Terrdata.h
TerrainBlock* getTerrainUnderWorldPoint(const Point3F & wPos)
{
	// Cast a ray straight down from the world position and see which
	// Terrain is the closest to our starting point
	Point3F startPnt = wPos;
	Point3F endPnt = wPos + Point3F(0.0f, 0.0f, -10000.0f);

	S32 blockIndex = -1;
	F32 nearT = 1.0f;

	SimpleQueryList queryList;
	gClientContainer.findObjects( TerrainObjectType, SimpleQueryList::insertionCallback, &queryList);

	for (U32 i = 0; i < queryList.mList.size(); i++)
	{
		Point3F tStartPnt, tEndPnt;
		TerrainBlock* terrBlock = dynamic_cast<TerrainBlock*>(queryList.mList[i]);
		terrBlock->getWorldTransform().mulP(startPnt, &tStartPnt);
		terrBlock->getWorldTransform().mulP(endPnt, &tEndPnt);

		RayInfo ri;
		if (terrBlock->castRayI(tStartPnt, tEndPnt, &ri, true))
		{
			if (ri.t < nearT)
			{
				blockIndex = i;
				nearT = ri.t;
			}
		}
	}

	if (blockIndex > -1)
		return (TerrainBlock*)(queryList.mList[blockIndex]);

	return NULL;
}

ConsoleFunction(getTerrainUnderWorldPoint, S32, 2, 4, "(Point3F x/y/z) Gets the terrain block that is located under the given world point.\n"
                                                      "@param x/y/z The world coordinates (floating point values) you wish to query at. " 
                                                      "These can be formatted as either a string (\"x y z\") or separately as (x, y, z)\n"
                                                      "@return Returns the ID of the requested terrain block (0 if not found).\n\n")
{
   Point3F pos;
   if(argc == 2)
      dSscanf(argv[1], "%f %f %f", &pos.x, &pos.y, &pos.z);
   else if(argc == 4)
   {
      pos.x = dAtof(argv[1]);
      pos.y = dAtof(argv[2]);
      pos.z = dAtof(argv[3]);
   }

   else
   {
      Con::errorf("getTerrainUnderWorldPoint(Point3F): Invalid argument count! Valid arguments are either \"x y z\" or x,y,z\n");
      return 0;
   }

   TerrainBlock* terrain = getTerrainUnderWorldPoint(pos);
   if(terrain != NULL)
   {
      return terrain->getId();
   }

   return 0;

}

//--------------------------------------
TerrainBlock::TerrainBlock()
{
   mSquareSize = 8;
   mDetailDistance = 25.0f;
   mDetailBrightness = 1.0f;
   mDetailScale = 256.0f;

   lightMap = NULL;
   mSwizzledLightMap = NULL;

   for(S32 i = BlockShift; i >= 0; i--)
      gridMap[i] = NULL;

   heightMap = NULL;
   materialMap = NULL;
   mBaseMaterialMap = NULL;
   mMaterialFileName = NULL;
   mTypeMask = TerrainObjectType | StaticObjectType | StaticRenderedObjectType | ShadowCasterObjectType;
   mCollideEmpty = false;
   mTile = true;

   flagMap = NULL;

   mTexelsPerMeter = 8;
   mClipMapSizeLog2 = getBinLog2(1024);

   mLightmapSource = NULL;
   mClipBlender = NULL;
   mClipMap = NULL;

   mTerrainMaterial = NULL;

   mRenderDelegate.bind(this, &TerrainBlock::renderObject);
}

//--------------------------------------
extern Convex sTerrainConvexList;

TerrainBlock::~TerrainBlock()
{
   // Kill collision
   sTerrainConvexList.nukeList();

   if( GDevice::devicePresent() )
   {
      GRAPHIC->getTextureManager()->unregisterTexCallback(mTexCallback);
   }
   mDetailTextureHandle = NULL;

   for (U32 i = 0; i < mOpacityMaps.size(); i++)
      SAFE_DELETE(mOpacityMaps[i]);
   mOpacityMaps.clear();

   mOpacitySources.clear();

   if (mSwizzledLightMap != NULL)
      SAFE_DELETE(lightMap);

   SAFE_DELETE(mClipMap);

   TerrainEditor* editor = dynamic_cast<TerrainEditor*>(Sim::findObject("ETerrainEditor"));
   if (editor)
      editor->detachTerrain(this);

   mSwizzledLightMap = NULL;
}

//--------------------------------------
void TerrainBlock::texManagerCallback(GTexCallbackCode code, void *userData)
{
   //TerrainBlock *localBlock = (TerrainBlock*)userData;

   if(code == GZombify)
   {
      // Clear the texture cache.
      //AllocatedTexture::flushCache();
      //localBlock->mBlender->zombify();
   }
   else if(code == GResurrect)
   {
      //localBlock->mBlender->resurrect();
   }
}

//--------------------------------------
void TerrainBlock::setFile(Resource<TerrainFile> terr)
{
   mFile = terr;
   for(U32 i = 0; i <= BlockShift; i++)
      gridMap[i] = mFile->mGridMap[i];

   mBaseMaterialMap = mFile->mBaseMaterialMap;
   mMaterialFileName= mFile->mMaterialFileName;
   mChunkMap = mFile->mChunkMap;
   materialMap = mFile->mMaterialMap;
   heightMap = mFile->mHeightMap;
   flagMap = mFile->mFlagMap;
}


//--------------------------------------------------------------------------
bool TerrainBlock::save(const char *filename)
{
   return mFile->save(filename);
}


//--------------------------------------
static U16 calcDev(const PlaneF &pl, const Point3F &pt)
{
   F32 z = (pl.d + pl.x * pt.x + pl.y * pt.y) / -pl.z;
   F32 diff = z - pt.z;
   if(diff < 0.0f)
      diff = -diff;

   if(diff > 0xFFFF)
      return 0xFFFF;
   else
      return U16(diff);
}

static U16 Umax(U16 u1, U16 u2)
{
   return u1 > u2 ? u1 : u2;
}

//------------------------------------------------------------------------------

bool TerrainBlock::unpackEmptySquares()
{
   U32 size = BlockSquareWidth * BlockSquareWidth;

   U32 i;
   for(i = 0; i < size; i++)
      materialMap[i].flags &= ~Material::Empty;

   // walk the pairs
   for(i = 0; i < mEmptySquareRuns.size(); i++)
   {
      U32 offset = mEmptySquareRuns[i] & 0xffff;
      U32 run = U32(mEmptySquareRuns[i]) >> 16;

      //
      for(U32 j = 0; j < run; j++)
      {
         if((offset+j) >= size)
         {
            Con::errorf(ConsoleLogEntry::General, "TerrainBlock::unpackEmpties: invalid entry.");
            return(false);
         }
         materialMap[offset+j].flags |= Material::Empty;
      }
   }

   rebuildEmptyFlags();
   return(true);
}

void TerrainBlock::packEmptySquares()
{
   mEmptySquareRuns.clear();

   // walk the map
   U32 run = 0;
   U32 offset = 0xFFFFFFFF;

   U32 size = BlockSquareWidth * BlockSquareWidth;
   for(U32 i = 0; i < size; i++)
   {
      if(materialMap[i].flags & Material::Empty)
      {
         if(!run)
            offset = i;
         run++;
      }
      else if(run)
      {
         AssertFatal(offset != 0xFFFFFFFF, "TerrainBlock::packEmptySquares");
         mEmptySquareRuns.push_back((run << 16) | offset);
         run = 0;

         // cap it
         if(mEmptySquareRuns.size() == MaxEmptyRunPairs)
            break;
      }
   }

   //
   if(run && mEmptySquareRuns.size() != MaxEmptyRunPairs)
      mEmptySquareRuns.push_back((run << 16) | offset);

   if(mEmptySquareRuns.size() == MaxEmptyRunPairs)
      Con::warnf(ConsoleLogEntry::General, "TerrainBlock::packEmptySquares: More than %d run pairs encountered.  Extras will not persist.", MaxEmptyRunPairs);

   //
}

//------------------------------------------------------------------------------

void TerrainBlock::rebuildEmptyFlags()
{
   // rebuild entire maps empty flags!
   for(U32 y = 0; y < TerrainBlock::ChunkSquareWidth; y++)
   {
      for(U32 x = 0; x < TerrainBlock::ChunkSquareWidth; x++)
      {
         GridChunk &gc = *(mChunkMap + x + (y << TerrainBlock::ChunkShift));
         gc.emptyFlags = 0;
         U32 sx = x << TerrainBlock::ChunkDownShift;
         U32 sy = y << TerrainBlock::ChunkDownShift;
         for(U32 j = 0; j < 4; j++)
         {
            for(U32 i = 0; i < 4; i++)
            {
               TerrainBlock::Material *mat = getMaterial(sx + i, sy + j);
               if(mat->flags & TerrainBlock::Material::Empty)
                  gc.emptyFlags |= (1 << (j * 4 + i));
            }
         }
      }
   }

   for(S32 i = BlockShift; i >= 0; i--)
   {
      S32 squareCount = 1 << (BlockShift - i);
      S32 squareSize = (TerrainBlock::BlockSize) / squareCount;

      for(S32 squareX = 0; squareX < squareCount; squareX++)
      {
         for(S32 squareY = 0; squareY < squareCount; squareY++)
         {
            GridSquare *parent = NULL;
            if(i < BlockShift)
               parent = findSquare(i+1, Point2I(squareX * squareSize, squareY * squareSize));
            bool empty = true;

            for(S32 sizeX = 0; empty && sizeX <= squareSize; sizeX++)
            {
               for(S32 sizeY = 0; empty && sizeY <= squareSize; sizeY++)
               {
                  S32 x = squareX * squareSize + sizeX;
                  S32 y = squareY * squareSize + sizeY;

                  if(sizeX != squareSize && sizeY != squareSize)
                  {
                     TerrainBlock::Material *mat = getMaterial(x, y);
                     if(!(mat->flags & TerrainBlock::Material::Empty))
                        empty = false;
                  }
               }
            }
            GridSquare *sq = findSquare(i, Point2I(squareX * squareSize, squareY * squareSize));
            if(empty)
               sq->flags |= GridSquare::Empty;
            else
               sq->flags &= ~GridSquare::Empty;
         }
      }
   }
}

//------------------------------------------------------------------------------

void TerrainBlock::calcBounds()
{
   // We know our bounds x, y are going to go from 0, 0 to BlockSize * SquareSize
   // but our height will be variable according to the heightfield
   mBounds.minExtents.set(0.0f, 0.0f, -1e10f);
   mBounds.maxExtents.set(BlockSize * (float)mSquareSize, BlockSize * (float)mSquareSize, 1e10f);

   // Little trick I picked up from the EditTSCtrl::renderMissionArea()
   // I *believe* that this works because the lowest level of detail has
   // the absolute max and min heights
   GridSquare * gs = findSquare(TerrainBlock::BlockShift, Point2I(0,0));
   mBounds.maxExtents.z = fixedToFloat(gs->maxHeight);
   mBounds.minExtents.z = fixedToFloat(gs->minHeight);

   // If we aren't tiling go ahead and set our mObjBox to be equal to mBounds
   // This will get overridden with global bounds if tiling is on but it is useful to store
   if (!mTile)
   {
      if (mObjBox.maxExtents != mBounds.maxExtents || mObjBox.minExtents != mBounds.minExtents)
      {
         mObjBox = mBounds;
         resetWorldBox();

      }
   }
}

void TerrainBlock::setHeight(const Point2I & pos, F32 height)
{
   // set the height
   U16 ht = floatToFixed(height);
   *((U16*)getHeightAddress(pos.x, pos.y)) = ht;
}


inline void getMinMax(U16 &inMin, U16 &inMax, U16 height)
{
   if(height < inMin)
      inMin = height;
   if(height > inMax)
      inMax = height;
}

inline void checkSquareMinMax(GridSquare *parent, const GridSquare *child)
{
   if(parent->minHeight > child->minHeight)
      parent->minHeight = child->minHeight;
   if(parent->maxHeight < child->maxHeight)
      parent->maxHeight = child->maxHeight;
}

void TerrainBlock::updateGridMaterials(const Point2I &minPt, const Point2I &maxPt)
{
   // ok:
   // build the chunk materials flags for all the chunks in the bounding rect
   // ((min - 1) >> ChunkDownShift) up to ((max + ChunkWidth) >> ChunkDownShift)

   // we have to make sure to cover boundary conditions as as stated above
   // since, for example, x = 0 affects 2 chunks

   for (S32 y = minPt.y - 1; y < maxPt.y + 1; y++)
   {
      for (S32 x=minPt.x - 1; x < maxPt.x + 1; x++)
      {
         GridSquare *sq = findSquare(0, Point2I(x, y));
         sq->flags &= (GridSquare::MaterialStart -1);
         
         S32 xpl = (x + 1) & TerrainBlock::BlockMask;
         S32 ypl = (y + 1) & TerrainBlock::BlockMask;

         for(U32 i = 0; i < TerrainBlock::MaterialGroups; i++)
         {
            if (mFile->mMaterialAlphaMap[i] != NULL) 
            {
               U32 mapVal = (mFile->mMaterialAlphaMap[i][(y   << TerrainBlock::BlockShift) + x]     |
                             mFile->mMaterialAlphaMap[i][(ypl << TerrainBlock::BlockShift) + x]   |
                             mFile->mMaterialAlphaMap[i][(ypl << TerrainBlock::BlockShift) + xpl] |
                             mFile->mMaterialAlphaMap[i][(y   << TerrainBlock::BlockShift) + xpl]);
               if(mapVal)
                  sq->flags |= (GridSquare::MaterialStart << i);
            }
         }
      }
   }

   for (S32 y = minPt.y - 2; y < maxPt.y + 2; y += 2)
   {
      for (S32 x= minPt.x - 2; x < maxPt.x + 2; x += 2)
      {
         GridSquare *sq = findSquare(1, Point2I(x, y));
         GridSquare *s1 = findSquare(0, Point2I(x, y));
         GridSquare *s2 = findSquare(0, Point2I(x+1, y));
         GridSquare *s3 = findSquare(0, Point2I(x, y+1));
         GridSquare *s4 = findSquare(0, Point2I(x+1, y+1));
         sq->flags |= (s1->flags | s2->flags | s3->flags | s4->flags) & ~(GridSquare::MaterialStart -1);
      }
   }

   //AllocatedTexture::flushCacheRect(RectI(min, max - min));
   updateOpacity();

   // Signal anyone that cares that the opacity was changed.
   smUpdateSignal.trigger( OpacityUpdate, this, minPt, maxPt );

}

U16 TerrainBlock::getHeight(U32 x, U32 y) const
{
   return mFile->getHeight(x,y); 
}

Point2I TerrainBlock::getGridPos( const Point3F& worldPos )
{
   Point3F terrainPos = worldPos;
   getWorldTransform().mulP( terrainPos );

   F32 squareSize = ( F32 ) getSquareSize();
   F32 halfSquareSize = squareSize / 2.0;

   F32 x = ( terrainPos.x + halfSquareSize ) / squareSize;
   F32 y = ( terrainPos.y + halfSquareSize ) / squareSize;

   Point2I gridPos( ( S32 ) mFloor( x ), ( S32 ) mFloor( y ) );
   return gridPos;
}

void TerrainBlock::updateGrid(const Point2I &minPt, const Point2I &maxPt)
{
   // On the client we just signal everyone that the height
   // map has changed... the server does the actual changes.
      smUpdateSignal.trigger( HeightmapUpdate, this, minPt, maxPt );
      return;

   // ok:
   // build the chunk deviance for all the chunks in the bounding rect,
   // ((min - 1) >> ChunkDownShift) up to ((max + ChunkWidth) >> ChunkDownShift)

   // update the chunk deviances for the affected chunks
   // we have to make sure to cover boundary conditions as as stated above
   // since, for example, x = 0 affects 2 chunks

   for(S32 x = (minPt.x - 1) >> ChunkDownShift;x < (maxPt.x + ChunkSize) >> ChunkDownShift; x++)
   {
      for(S32 y = (minPt.y - 1) >> ChunkDownShift;y < (maxPt.y + ChunkSize) >> ChunkDownShift; y++)
      {
         S32 px = x;
         S32 py = y;
         if(px < 0)
            px += BlockSize >> ChunkDownShift;
         if(py < 0)
            py += BlockSize >> ChunkDownShift;

         buildChunkDeviance(px, py);
      }
   }

   // ok the chunk deviances are rebuilt... now rebuild the affected area
   // of the grid map:

   // here's how it works:
   // for the current terrain renderer we only care about
   // the minHeight and maxHeight on the GridSquare
   // so we do one pass through, updating minHeight and maxHeight
   // on the level 0 squares, then we loop up the grid map from 1 to
   // the top, expanding the bounding boxes as necessary.
   // this should end up being way, way, way, way faster for the terrain
   // editor

   for(S32 y = minPt.y - 1; y < maxPt.y + 1; y++)
   {
      for(S32 x = minPt.x - 1; x < maxPt.x + 1; x++)
      {
         S32 px = x;
         S32 py = y;
         if(px < 0)
            px += BlockSize;
         if(py < 0)
            py += BlockSize;

         GridSquare *sq = findSquare(0, px, py);

         sq->minHeight = 0xFFFF;
         sq->maxHeight = 0;

         getMinMax(sq->minHeight, sq->maxHeight, getHeight(x, y));
         getMinMax(sq->minHeight, sq->maxHeight, getHeight(x+1, y));
         getMinMax(sq->minHeight, sq->maxHeight, getHeight(x, y+1));
         getMinMax(sq->minHeight, sq->maxHeight, getHeight(x+1, y+1));
      }
   }

   // ok, all the level 0 grid squares are updated:
   // now update all the parent grid squares that need to be updated:

   for(S32 level = 1; level <= TerrainBlock::BlockShift; level++)
   {
      S32 size = 1 << level;
      S32 halfSize = size >> 1;
      for(S32 y = (minPt.y - 1) >> level; y < (maxPt.y + size) >> level; y++)
      {
         for(S32 x = (minPt.x - 1) >> level; x < (maxPt.x + size) >> level; x++)
         {
            S32 px = x << level;
            S32 py = y << level;

            GridSquare *square = findSquare(level, px, py);
            square->minHeight = 0xFFFF;
            square->maxHeight = 0;

            checkSquareMinMax(square, findSquare(level - 1, px, py));
            checkSquareMinMax(square, findSquare(level - 1, px + halfSize, py));
            checkSquareMinMax(square, findSquare(level - 1, px, py + halfSize));
            checkSquareMinMax(square, findSquare(level - 1, px + halfSize, py + halfSize));
         }
      }
   }

   calcBounds();

   // Signal again here for any server side observers.
   smUpdateSignal.trigger( HeightmapUpdate, this, minPt, maxPt );

}


//--------------------------------------
bool TerrainBlock::getHeight(const Point2F &pos, F32 *height)
{
   F32 invSquareSize = 1.0f / (F32)mSquareSize;
   F32 xp = pos.x * invSquareSize;
   F32 yp = pos.y * invSquareSize;
   int x = (S32)mFloor(xp);
   int y = (S32)mFloor(yp);
   xp -= (F32)x;
   yp -= (F32)y;

   // If we disable repeat, then skip non-primary block
   if(!mTile && (x&~BlockMask || y&~BlockMask))
      return false;

   x &= BlockMask;
   y &= BlockMask;
   
   GridSquare * gs = findSquare(0, Point2I(x,y));

   if (gs->flags & GridSquare::Empty)
      return false;

   F32 zBottomLeft = fixedToFloat(getHeight(x, y));
   F32 zBottomRight = fixedToFloat(getHeight(x + 1, y));
   F32 zTopLeft = fixedToFloat(getHeight(x, y + 1));
   F32 zTopRight = fixedToFloat(getHeight(x + 1, y + 1));

   if(gs->flags & GridSquare::Split45)
   {
      if (xp>yp)
         // bottom half
         *height = zBottomLeft + xp * (zBottomRight-zBottomLeft) + yp * (zTopRight-zBottomRight);
      else
         // top half
         *height = zBottomLeft + xp * (zTopRight-zTopLeft) + yp * (zTopLeft-zBottomLeft);
   }
   else
   {
      if (1.0f-xp>yp)
         // bottom half
         *height = zBottomRight + (1.0f-xp) * (zBottomLeft-zBottomRight) + yp * (zTopLeft-zBottomLeft);
      else
         // top half
         *height = zBottomRight + (1.0f-xp) * (zTopLeft-zTopRight) + yp * (zTopRight-zBottomRight);
   }
   return true;
}

bool TerrainBlock::getNormal(const Point2F & pos, Point3F * normal, bool normalize)
{
   F32 invSquareSize = 1.0f / (F32)mSquareSize;
   F32 xp = pos.x * invSquareSize;
   F32 yp = pos.y * invSquareSize;
   int x = (S32)mFloor(xp);
   int y = (S32)mFloor(yp);
   xp -= (F32)x;
   yp -= (F32)y;

   // If we disable repeat, then skip non-primary block
   if(!mTile && (x&~BlockMask || y&~BlockMask))
      return false;

   x &= BlockMask;
   y &= BlockMask;
   
   GridSquare * gs = findSquare(0, Point2I(x,y));

   if (gs->flags & GridSquare::Empty)
      return false;

   F32 zBottomLeft = fixedToFloat(getHeight(x, y));
   F32 zBottomRight = fixedToFloat(getHeight(x + 1, y));
   F32 zTopLeft = fixedToFloat(getHeight(x, y + 1));
   F32 zTopRight = fixedToFloat(getHeight(x + 1, y + 1));

   if(gs->flags & GridSquare::Split45)
   {
      if (xp>yp)
         // bottom half
         normal->set(zBottomLeft-zBottomRight, zBottomRight-zTopRight, (F32)mSquareSize);
      else
         // top half
         normal->set(zTopLeft-zTopRight, zBottomLeft-zTopLeft, (F32)mSquareSize);
   }
   else
   {
      if (1.0f-xp>yp)
         // bottom half
         normal->set(zBottomLeft-zBottomRight, zBottomLeft-zTopLeft, (F32)mSquareSize);
      else
         // top half
         normal->set(zTopLeft-zTopRight, zBottomRight-zTopRight, (F32)mSquareSize);
   }
   if (normalize)
      normal->normalize();
   return true;
}

bool TerrainBlock::getNormalAndHeight(const Point2F & pos, Point3F * normal, F32 * height, bool normalize)
{
   F32 invSquareSize = 1.0f / (F32)mSquareSize;
   F32 xp = pos.x * invSquareSize;
   F32 yp = pos.y * invSquareSize;
   int x = (S32)mFloor(xp);
   int y = (S32)mFloor(yp);
   xp -= (F32)x;
   yp -= (F32)y;

   // If we disable repeat, then skip non-primary block
   if(!mTile && (x&~BlockMask || y&~BlockMask))
      return false;

   x &= BlockMask;
   y &= BlockMask;

   GridSquare * gs = findSquare(0, Point2I(x,y));

   if (gs->flags & GridSquare::Empty)
      return false;

   F32 zBottomLeft  = fixedToFloat(getHeight(x, y));
   F32 zBottomRight = fixedToFloat(getHeight(x + 1, y));
   F32 zTopLeft     = fixedToFloat(getHeight(x, y + 1));
   F32 zTopRight    = fixedToFloat(getHeight(x + 1, y + 1));

   if(gs->flags & GridSquare::Split45)
   {
      if (xp>yp)
      {
         // bottom half
         normal->set(zBottomLeft-zBottomRight, zBottomRight-zTopRight, (F32)mSquareSize);
         *height = zBottomLeft + xp * (zBottomRight-zBottomLeft) + yp * (zTopRight-zBottomRight);
      }
      else
      {
         // top half
         normal->set(zTopLeft-zTopRight, zBottomLeft-zTopLeft, (F32)mSquareSize);
         *height = zBottomLeft + xp * (zTopRight-zTopLeft) + yp * (zTopLeft-zBottomLeft);
      }
   }
   else
   {
      if (1.0f-xp>yp)
      {
         // bottom half
         normal->set(zBottomLeft-zBottomRight, zBottomLeft-zTopLeft, (F32)mSquareSize);
         *height = zBottomRight + (1.0f-xp) * (zBottomLeft-zBottomRight) + yp * (zTopLeft-zBottomLeft);
      }
      else
      {
         // top half
         normal->set(zTopLeft-zTopRight, zBottomRight-zTopRight, (F32)mSquareSize);
         *height = zBottomRight + (1.0f-xp) * (zTopLeft-zTopRight) + yp * (zTopRight-zBottomRight);
      }
   }
   if (normalize)
      normal->normalize();
   return true;
}

//--------------------------------------


//--------------------------------------
void TerrainBlock::setBaseMaterials(S32 argc, const char *argv[])
{
   for (S32 i = 0; i < argc; i++)
      mMaterialFileName[i] = argv[i];
   for (S32 j = argc; j < MaterialGroups; j++)
      mMaterialFileName[j] = String();
}

//--------------------------------------

//--------------------------------------

bool TerrainBlock::buildMaterialMap()
{
   PROFILE_SCOPE(TerrainBlock_BuildMaterialMap);
   // We have to make sure we have the right number of alpha maps - should
   // have one for every specified texture. So add or delete to conform.
   for(S32 i=0; i<TerrainBlock::MaterialGroups; i++)
   {
      if(!mFile->mMaterialFileName[i].isEmpty())
      {
         if(!mFile->mMaterialAlphaMap[i])
         {
            mFile->mMaterialAlphaMap[i] = new U8[TerrainBlock::BlockSize*TerrainBlock::BlockSize];
            dMemset(mFile->mMaterialAlphaMap[i], 0, sizeof(U8)*TerrainBlock::BlockSize*TerrainBlock::BlockSize);

            addOpacitySources();

            if (mClipBlender)
               mClipBlender->createOpacityScratchTextures();
         }
      }
      else
      {
         if(mFile->mMaterialAlphaMap[i])
         {
            delete [] mFile->mMaterialAlphaMap[i];
            mFile->mMaterialAlphaMap[i] = NULL;
         }
      }
   }

   if(mClipMap && lightMap)
   {
      // Update the lightmap source.  We have to keep two copies of the lightmap around for now. ;/
      // One is the RGB version which is used by getAmbientLighting and is loaded/store in the ml file.
      // One is the swizzled BGR version which the clip mapper wants.
      SAFE_DELETE(mSwizzledLightMap);
      mSwizzledLightMap = new GBitmap(*lightMap);
      mSwizzledLightMap->swizzle(GRAPHIC->getDeviceSwizzle24());
      mLightmapSource->mBitmap = mSwizzledLightMap;

      // Refill clipmap.
      mClipMap->fillWithTextureData();
   }

   // Reload the detail texture
   if( !mDetailTextureName.isEmpty() )
      mDetailTextureHandle.set(mDetailTextureName, &GDefaultStaticDiffuseProfile, avar("%s() - tex (line %d)", __FUNCTION__, __LINE__));

   // Signal anyone that cares on the lightmap update.
   smUpdateSignal.trigger( LightmapUpdate, this, Point2I(), Point2I() );

   return true;
}

bool TerrainBlock::initMMXBlender()
{
   U32 validMaterials = 0;
   S32 i;
   for (i = 0; i < MaterialGroups; i++) 
   {
      if (!mMaterialFileName[i].isEmpty())
         validMaterials++;
      else
         break;
   }
   AssertFatal(validMaterials != 0, "Error, must have SOME materials here!");

   updateOpacity();

   // ok, if the material list load failed...
   // if this is a local connection, we'll assume that's ok
   // and just have white textures...
   // otherwise we want to return false.
   return true;
}

void TerrainBlock::getMaterialAlpha(const Point2I &pos, U8 alphas[MaterialGroups]) const
{
   Point2I  thePos( pos );
   thePos.x &= BlockMask;
   thePos.y &= BlockMask;

   U32 offset = thePos.x + thePos.y * BlockSize;
   for(S32 i = 0; i < MaterialGroups; i++)
   {
      U8 *map = mFile->mMaterialAlphaMap[i];
      if(map)
         alphas[i] = map[offset];
      else
         alphas[i] = 0;
   }
}

void TerrainBlock::setMaterialAlpha(const Point2I &pos, const U8 alphas[MaterialGroups])
{
   Point2I  thePos( pos );
   thePos.x &= BlockMask;
   thePos.y &= BlockMask;

   U32 offset = thePos.x + thePos.y * BlockSize;
   for(S32 i = 0; i < MaterialGroups; i++)
   {
      U8 *map = mFile->mMaterialAlphaMap[i];
      if(map)
         map[offset] = alphas[i];
   }
}

S32 TerrainBlock::getMaterialAlphaIndex(const String &materialName)
{
   for (S32 i = 0; i < MaterialGroups; i++)
   {
      if (!mMaterialFileName[i].isEmpty() && materialName.equal( mMaterialFileName[i], String::NoCase ))
         return i;
   }

   // ok, it wasn't in the list
   // see if we can add it:
   for (S32 i = 0; i < MaterialGroups; i++)
   {
      if (mMaterialFileName[i].isEmpty())
      {
         mMaterialFileName[i] = materialName;
         mFile->mMaterialAlphaMap[i] = new U8[TerrainBlock::BlockSize * TerrainBlock::BlockSize];
         dMemset(mFile->mMaterialAlphaMap[i], 0, TerrainBlock::BlockSize * TerrainBlock::BlockSize);
         buildMaterialMap();
         return i;
      }
   }
   return -1;
}

BaseMatInstance* TerrainBlock::getMaterialInst( U32 x, U32 y )
{
   // Get all material alphas.

   U8 alphas[ MaterialGroups ];
   getMaterialAlpha( Point2I( x, y ), alphas );

   // Find the material with the max alpha at the given
   // coordinates.

   U32 matIndex = 0;
   S32 maxAlpha = -1;
   for( U32 i = 0; i < MaterialGroups; ++ i )
      if( alphas[ i ] > maxAlpha )
      {
         maxAlpha = alphas[ i ];
         matIndex = i;
      }

   // Return the corresponding material instance.

   return mFile->mMaterialList.getMaterialInst( matIndex );
}

//------------------------------------------------------------------------------

void TerrainBlock::refreshMaterialLists()
{
}

//------------------------------------------------------------------------------

void TerrainBlock::onEditorEnable()
{
   // need to load in the material base material lists
      refreshMaterialLists();
}

void TerrainBlock::onEditorDisable()
{

}

//------------------------------------------------------------------------------

bool TerrainBlock::onAdd()
{
   if(!Parent::onAdd())
      return false;

   if (mTile)
      setPosition(Point3F((F32)(-mSquareSize * (BlockSize >> 1)), (F32)(-mSquareSize * (BlockSize >> 1)), 0.0f));

   if (mTerrFileName.isEmpty())
   {
       createNewTerrainFile(Con::getVariable("$Client::MissionFile"));
   }

   Resource<TerrainFile> terr = ResourceManager::get().load(mTerrFileName);
   if(terr == NULL)
   {
      return false;
   }

   setFile(terr);

   if (terr->mNeedsResaving)
   {
      if (Platform::messageBox("Update Terrain File", "You appear to have a Terrain file in an older format. Do you want Torque to update it?", MBOkCancel, MIQuestion) == MROk)
      {
         terr->save(terr->mFilePath.getFullPath());
         terr->mNeedsResaving = false;
      }
   }

   if (terr->mFileVersion != TerrainFile::FILE_VERSION || terr->mNeedsResaving)
   {
      Con::errorf(" *********************************************************");
      Con::errorf(" *********************************************************");
      Con::errorf(" *********************************************************");
      Con::errorf(" PLEASE RESAVE THE TERRAIN FILE FOR THIS MISSION!  THANKS!");
      Con::errorf(" *********************************************************");
      Con::errorf(" *********************************************************");
      Con::errorf(" *********************************************************");
   }

   calcBounds();
   
   if (mTile)
      setGlobalBounds();

   resetWorldBox();
   setRenderTransform(mObjToWorld);

	  mTerrainMaterial = MaterialManager::get()->createMatInstance("TerrainMaterial");
	  if (!mTerrainMaterial)
	  {
		 Con::errorf("TerrainMaterial not defined, legacy terrain will not render correctly!");
	  }
	  lightMap = new GBitmap(LightmapSize, LightmapSize, false, GFormatR8G8B8);

	  if (!buildMaterialMap())
		 return false;

	  // Set up the blended clipmap.
	  mClipMap = new ClipMap();
	  mClipMap->mTextureSize = TerrainBlock::BlockSize * mSquareSize * mTexelsPerMeter;
	  mClipMap->mClipMapSize = BIT(mClipMapSizeLog2);
	  mClipMap->mTile = mTile;

	  // Lightmap is quad based, so no offset needed. 
	  mLightmapSource = new TerrClipMapImageSource(lightMap, 0.0f);

	  mClipBlender = new ClipMapBlenderCache(mLightmapSource);

	  updateOpacity();

	  mClipMap->setCache(mClipBlender);
	  mClipMap->initClipStack();
	  
	  updateOpacity();

	  // We don't page data in legacy, so this shouldn't need to loop.
	  mClipMap->fillWithTextureData();

   addToScene();

   if(!unpackEmptySquares())
      return(false);

   return true;
}

//--------------------------------------
void TerrainBlock::createNewTerrainFile(StringTableEntry terrainFileName, U16* hmap, Vector<StringTableEntry> *materials)
{
   TerrainFile *file = new TerrainFile;

   if (!materials)
   {
      file->mMaterialFileName[0] = String("Default");
      file->mMaterialAlphaMap[0] = new U8[TerrainBlock::BlockSize*TerrainBlock::BlockSize];
      dMemset(file->mMaterialAlphaMap[0], 0, TerrainBlock::BlockSize*TerrainBlock::BlockSize*sizeof(U8));

   }
   else
   {
      for (S32 i = 0; i < materials->size(); i++)
      {
         file->mMaterialFileName[i] = String((*materials)[i]);
         file->mMaterialAlphaMap[i] = new U8[TerrainBlock::BlockSize*TerrainBlock::BlockSize];

         if (!i)
         {
            dMemset(file->mMaterialAlphaMap[i], 255, TerrainBlock::BlockSize*TerrainBlock::BlockSize*sizeof(U8));
         }
         else
         {
            dMemset(file->mMaterialAlphaMap[i], 0, TerrainBlock::BlockSize*TerrainBlock::BlockSize*sizeof(U8));

            // Need at last one valid alpha value otherwise serializer strips the material off!
            file->mMaterialAlphaMap[i][0] = U8(1);
         }         
      }
   }

   file->mTextureScript = dStrdup("");
   file->mHeightfieldScript = dStrdup("");

   // create circular cone in the middle of the map:
   for(U32 i = 0; i < TerrainBlock::BlockSize; i++)
   {
      for(U32 j = 0; j < TerrainBlock::BlockSize; j++)
      {
         U32 offset = i + (j << TerrainBlock::BlockShift);

         if (hmap)
            file->mHeightMap[offset] = hmap[offset];
         else
            file->mHeightMap[offset] = (U16)(200);

         

         file->mBaseMaterialMap[offset] = 0;
      }
   }

   // We need to construct a default file name
   char fileName[1024];
   fileName[0] = 0;

   // See if we know our current mission name
   char missionName[1024];
   dSprintf(missionName, sizeof(missionName), "%s\0", terrainFileName);
   char * dot = dStrstr((const char*)missionName, ".mis");
   if(dot)
	   *dot = '\0';

   // Find the first file that doesn't exist
   for (U32 i = 0; i < 32; i++)
   {
      char testName[1024];

      if (dStrlen(missionName) == 0 || dStricmp(missionName, "0") == 0)
         dSprintf(testName, sizeof(testName), "%s/data/missions/terrain%d.ter\0", Con::getVariable( "$defaultGame" ), i);
      else
         dSprintf(testName, sizeof(testName), "%s%d.ter\0", missionName, i);

      char fullName[1024];
      Platform::makeFullPathName(testName, fullName, sizeof(fullName));
      
      if (!Platform::isFile(fullName))
      {
         dSprintf(fileName, sizeof(fileName), "%s", fullName);
         break;
      }
   }

   // If we found a valid file name then save it out
   if (dStrlen(fileName) > 0)
   {
      file->save(fileName);

      mTerrFileName = String(fileName);
   }
   
   delete file;

}

//--------------------------------------
void TerrainBlock::onRemove()
{
   removeFromScene();

   SAFE_DELETE(mClipMap);
   SAFE_DELETE(mTerrainMaterial);

   mSwizzledLightMap = NULL;

   Parent::onRemove();
}

//--------------------------------------------------------------------------
bool TerrainBlock::prepRenderImage(SceneState* state, const U32 stateKey,
                                   const U32 /*startZone*/, const bool /*modifyBaseState*/)
{
   PROFILE_START(TerrainBlock_prepRenderImage);

   if (isLastState(state, stateKey))
   {
      PROFILE_END();
      return false;
   }
   setLastState(state, stateKey);

   // This should be sufficient for most objects that don't manage zones, and
   //  don't need to return a specialized RenderImage...
   bool render = false;
   if (state->isTerrainOverridden() == false)
      render = state->isObjectRendered(this);
   else
      render = true;

   // small hack to reduce "stutter" in framerate if terrain is suddenly seen (ie. from an interior)
   if( !mHasRendered )
   {
      mHasRendered = true;
      render = true;
      state->enableTerrainOverride();
   }


   if (render == true)
   {
      ObjectRenderInst *ri = gRenderInstManager->allocInst<ObjectRenderInst>();
      ri->mRenderDelegate = mRenderDelegate;
      ri->state = state;
      ri->type = RenderPassManager::RIT_Object;
      gRenderInstManager->addInst( ri );
   }

   PROFILE_END();
   return false;
}

void TerrainBlock::buildChunkDeviance(S32 x, S32 y)
{
   mFile->buildChunkDeviance(x, y);
}

void TerrainBlock::buildGridMap()
{
   mFile->buildGridMap();
}

//------------------------------------------------------------------------------
void TerrainBlock::setTransform(const MatrixF & mat)
{
   Parent::setTransform(mat);

   // Since the terrain is a static object, it's render transform changes 1 to 1
   //  with it's collision transform
   setRenderTransform(mat);
}

void TerrainBlock::renderObject(ObjectRenderInst *ri, BaseMatInstance* overrideMat)
{
   TerrainRender::mOverrideMat = overrideMat;

   MatrixF proj   = GRAPHIC->getProjectionMatrix();
   MatrixF cleanProj = proj;
   RectI viewport = GRAPHIC->getViewport();

   SceneState* state = ri->state;

   GRAPHIC->pushWorldMatrix();
   GRAPHIC->multWorld( getRenderTransform() );
   
   // Set up scenegraph data
   SceneGraphData sgData;
   sgData.setFogParams(state->getSceneManager());
   sgData.reflectPass = state->mFlipCull;

   // Store object and camera transform data
   sgData.objTrans = getRenderTransform();
   sgData.camPos   = state->getCameraPosition();

   LightManager* lm = state->getLightManager();
   if (lm)
      lm->setupLights(this, state->getCameraPosition(), Point3F::Zero, state->getVisibleDistance(), MaxVisibleLights);   

   // Actually render the thing.
   static bool bMatInited = false;   
   if (!bMatInited)
   {
      // Make sure our material is initialized so that we get lighting info.
      GMaterialFeatureData::FeatureListHandle features = MaterialManager::get()->getDefaultFeatureData().codify();
      mTerrainMaterial->init(features, features, (GVertexFlags)getGVertFlags((GVertexPNT*)NULL) );
      bMatInited = true;
   }

   TerrainRender::renderBlock(this, state, mTerrainMaterial, sgData);

   if (lm)
      lm->resetLights();

   GRAPHIC->popWorldMatrix();

   GRAPHIC->setViewport( viewport );
   GRAPHIC->setProjectionMatrix( cleanProj );
}

//--------------------------------------
void TerrainBlock::initPersistFields()
{
   Parent::initPersistFields();

   addGroup("Media");
   addField("detailTexture",     TypeStringFilename,  Offset(mDetailTextureName, TerrainBlock));
   addField("terrainFile",       TypeStringFilename,  Offset(mTerrFileName,      TerrainBlock));
   endGroup("Media");

   addGroup("Misc");
   addField("squareSize",        TypeS32,             Offset(mSquareSize,       TerrainBlock));
   addField("emptySquares",      TypeS32Vector,       Offset(mEmptySquareRuns,  TerrainBlock));
   addField("tile",              TypeBool,            Offset(mTile,             TerrainBlock));
   addField("detailDistance",    TypeF32,             Offset(mDetailDistance,   TerrainBlock));
   addField("detailScale",       TypeF32,             Offset(mDetailScale,   TerrainBlock));
   addField("detailBrightness",  TypeF32,             Offset(mDetailBrightness, TerrainBlock));
   addField("texelsPerMeter",    TypeS32,             Offset(mTexelsPerMeter,   TerrainBlock));
   addField("clipMapSizeLog2",   TypeS32,             Offset(mClipMapSizeLog2,  TerrainBlock));
   endGroup("Misc");
}


//--------------------------------------
ConsoleFunction(makeTestTerrain, void, 2, 10, "(string fileName, ...) - makes a test terrain file - arguments after the fileName are the names of the initial terrain materials.")
{
   TerrainFile *file = new TerrainFile;
   S32 nMaterialTypes;
   argc -= 2;

   // Load materials
   if (argc > 0)
   {
      nMaterialTypes = argc;
      for (S32 i=0; i<TerrainBlock::MaterialGroups && i < argc; i++)
      {
         char material[256];
         char *ext;
         dStrcpy(material, argv[i+2]);
         ext = dStrrchr(material, '.');
         if (ext)
            *ext = 0;
         file->mMaterialFileName[i] = material;
         file->mMaterialAlphaMap[i] = new U8[TerrainBlock::BlockSize*TerrainBlock::BlockSize];
         dMemset(file->mMaterialAlphaMap[i], 0, TerrainBlock::BlockSize*TerrainBlock::BlockSize*sizeof(U8));
         
         // Make sure at least one square uses the material so it isn't dropped.
         file->mMaterialAlphaMap[i][i] = 1;
      }
      for(S32 i=argc; i<TerrainBlock::MaterialGroups; i++)
         file->mMaterialFileName[i] = String();

   }
   else
   {
      nMaterialTypes = 1;
      file->mMaterialFileName[0] = "Default";
      file->mMaterialAlphaMap[0] = new U8[TerrainBlock::BlockSize*TerrainBlock::BlockSize];
      dMemset(file->mMaterialAlphaMap[0], 0, TerrainBlock::BlockSize*TerrainBlock::BlockSize*sizeof(U8));
   }

   file->mTextureScript = dStrdup("");
   file->mHeightfieldScript = dStrdup("");

   // create circular cone in the middle of the map:
   S32 i, j;
   for(i = 0; i < TerrainBlock::BlockSize; i++)
   {
      for(j = 0; j < TerrainBlock::BlockSize; j++)
      {
         S32 x = i & 0x7f;
         S32 y = j & 0x7f;

         F32 dist = mSqrt(F32((64 - x) * (64 - x) + (64 - y) * (64 - y)));
         dist /= 64.0f;

         if(dist > 1)
            dist = 1;

         U32 offset = i + (j << TerrainBlock::BlockShift);
         file->mHeightMap[offset] = (U16)((1 - dist) * (1 - dist) * 20000);
         file->mBaseMaterialMap[offset] = 0;
      }
   }

   //
   char filename[256];
   dStrcpy(filename,argv[1]);
   char* ext = dStrrchr(filename, '.');
   if (!ext || dStricmp(ext, ".ter") != 0)
      dStrcat(filename, ".ter");
   file->save(filename);
   delete file;
}


//--------------------------------------

void TerrainBlock::flushCache()
{
   //AllocatedTexture::flushCache();
}

//--------------------------------------
TerrainFile::TerrainFile()
:  mTextureScript( NULL ),
   mHeightfieldScript( NULL ),
   mNeedsResaving( false ),
   mFileVersion( FILE_VERSION )
{
   dMemset( mHeightMap, 0, TerrainBlock::BlockSize * TerrainBlock::BlockSize * sizeof(U16) );
   dMemset( mBaseMaterialMap, 0, TerrainBlock::BlockSize * TerrainBlock::BlockSize * sizeof(U8) );
   dMemset( mGridMapBase, 0, TerrainBlock::GridMapSize * sizeof(GridSquare) );
   dMemset( mGridMap, 0, TerrainBlock::BlockShift+1 * sizeof(GridSquare *) );
   dMemset( mChunkMap, 0, TerrainBlock::ChunkSquareWidth * TerrainBlock::ChunkSquareWidth * sizeof(GridChunk) );
   dMemset( mFlagMap, 0, TerrainBlock::FlagMapWidth * TerrainBlock::FlagMapWidth );
   dMemset( mMaterialMap, 0, TerrainBlock::BlockSquareWidth * TerrainBlock::BlockSquareWidth * sizeof(TerrainBlock::Material) );
   dMemset( mMaterialAlphaMap, 0, TerrainBlock::MaterialGroups * sizeof(U8*) );

   mEdgeTerrainFiles[0] = NULL;
   mEdgeTerrainFiles[1] = NULL;
}

TerrainFile::~TerrainFile()
{
   for(S32 i=0; i < TerrainBlock::MaterialGroups; i++)
   {
      delete[] mMaterialAlphaMap[i];
      mMaterialAlphaMap[i] = NULL;
   }
   dFree(mTextureScript);
   dFree(mHeightfieldScript);
}

void TerrainFile::setTextureScript(const char *script)
{
   dFree(mTextureScript);
   mTextureScript = dStrdup(script);
}

void TerrainFile::setHeightfieldScript(const char *script)
{
   dFree(mHeightfieldScript);
   mHeightfieldScript = dStrdup(script);
}

void TerrainBlock::getMinMaxHeight( F32* minHeight, F32* maxHeight ) const 
{
   *minHeight =  fixedToFloat( gridMap[BlockShift]->minHeight );
   *maxHeight =  fixedToFloat( gridMap[BlockShift]->maxHeight );
}


const char *TerrainFile::getTextureScript()
{
   return mTextureScript ? mTextureScript : "";
}

const char *TerrainFile::getHeightfieldScript()
{
   return mHeightfieldScript ? mHeightfieldScript : "";
}

//--------------------------------------
bool TerrainFile::save(const char *filename)
{
   Path terFilePath = Platform::makeRelativePathName(filename, Platform::getMainDotCsDir());
   FileStream writeFile;

   writeFile.open( filename, File::Write );

   if(writeFile.getStatus() != Stream::Ok)
      return false;

   // write the VERSION and HeightField
   writeFile.write((U8)FILE_VERSION);
   for (S32 i=0; i < (TerrainBlock::BlockSize * TerrainBlock::BlockSize); i++)
      writeFile.write(mHeightMap[i]);

   // write the material group map, after merging the flags...
   TerrainBlock::Material * materialMap = (TerrainBlock::Material*)mMaterialMap;
   for (S32 j=0; j < (TerrainBlock::BlockSize * TerrainBlock::BlockSize); j++)
   {
      U8 val = mBaseMaterialMap[j];
      val |= materialMap[j].flags & TerrainBlock::Material::PersistMask;
      writeFile.write(val);
   }

   // write the MaterialList Info
   S32 k;
   for(k=0; k < TerrainBlock::MaterialGroups; k++)
   {
      String matRelPath;
      if(!mMaterialFileName[k].isEmpty())
      {
         matRelPath = Path::MakeRelativePath(mMaterialFileName[k], terFilePath.getPath());
      }
      writeFile.write(matRelPath);
   }
   for(k=0; k < TerrainBlock::MaterialGroups; k++)
   {
      if(!mMaterialFileName[k].isEmpty())
      {
         AssertFatal(mMaterialAlphaMap[k] != NULL, "Error, must have a material map here!");
         writeFile.write(TerrainBlock::BlockSize * TerrainBlock::BlockSize, mMaterialAlphaMap[k]);
      }
   }
   if(mTextureScript)
   {
      U32 len = dStrlen(mTextureScript);
      writeFile.write(len);
      writeFile.write(len, mTextureScript);
   }
   else
      writeFile.write(U32(0));

   if(mHeightfieldScript)
   {
      U32 len = dStrlen(mHeightfieldScript);
      writeFile.write(len);
      writeFile.write(len, mHeightfieldScript);
   }
   else
      writeFile.write(U32(0));

   if (mEdgeTerrainFiles[0])
      writeFile.writeString(Platform::makeRelativePathName(mEdgeTerrainFiles[0]->mFilePath.getFullPath(),Platform::getMainDotCsDir()));
   else
      writeFile.writeString("");

   if (mEdgeTerrainFiles[1])
      writeFile.writeString(Platform::makeRelativePathName(mEdgeTerrainFiles[1]->mFilePath.getFullPath(),Platform::getMainDotCsDir()));
   else
      writeFile.writeString("");

   return (writeFile.getStatus() == FileStream::Ok);
}

//--------------------------------------

void TerrainFile::heightDevLine(U32 p1x, U32 p1y, U32 p2x, U32 p2y, U32 pmx, U32 pmy, U16 *devPtr)
{
   S32 h1 = getHeight(p1x, p1y);
   S32 h2 = getHeight(p2x, p2y);
   S32 hm = getHeight(pmx, pmy);
   S32 dev = ((h2 + h1) >> 1) - hm;
   if(dev < 0)
      dev = -dev;
   if(dev > *devPtr)
      *devPtr = dev;
}

void TerrainFile::buildChunkDeviance(S32 x, S32 y)
{
   GridChunk &gc = *(mChunkMap + x + (y << TerrainBlock::ChunkShift));
   gc.emptyFlags = 0;
   U32 sx = x << TerrainBlock::ChunkDownShift;
   U32 sy = y << TerrainBlock::ChunkDownShift;

   gc.heightDeviance[0] = 0;

   heightDevLine(sx, sy, sx + 2, sy, sx + 1, sy, &gc.heightDeviance[0]);
   heightDevLine(sx + 2, sy, sx + 4, sy, sx + 3, sy, &gc.heightDeviance[0]);

   heightDevLine(sx, sy + 2, sx + 2, sy + 2, sx + 1, sy + 2, &gc.heightDeviance[0]);
   heightDevLine(sx + 2, sy + 2, sx + 4, sy + 2, sx + 3, sy + 2, &gc.heightDeviance[0]);

   heightDevLine(sx, sy + 4, sx + 2, sy + 4, sx + 1, sy + 4, &gc.heightDeviance[0]);
   heightDevLine(sx + 2, sy + 4, sx + 4, sy + 4, sx + 3, sy + 4, &gc.heightDeviance[0]);

   heightDevLine(sx, sy, sx, sy + 2, sx, sy + 1, &gc.heightDeviance[0]);
   heightDevLine(sx + 2, sy, sx + 2, sy + 2, sx + 2, sy + 1, &gc.heightDeviance[0]);
   heightDevLine(sx + 4, sy, sx + 4, sy + 2, sx + 4, sy + 1, &gc.heightDeviance[0]);

   heightDevLine(sx, sy + 2, sx, sy + 4, sx, sy + 3, &gc.heightDeviance[0]);
   heightDevLine(sx + 2, sy + 2, sx + 2, sy + 4, sx + 2, sy + 3, &gc.heightDeviance[0]);
   heightDevLine(sx + 4, sy + 2, sx + 4, sy + 4, sx + 4, sy + 3, &gc.heightDeviance[0]);

   gc.heightDeviance[1] = gc.heightDeviance[0];

   heightDevLine(sx, sy, sx + 2, sy + 2, sx + 1, sy + 1, &gc.heightDeviance[1]);
   heightDevLine(sx + 2, sy, sx, sy + 2, sx + 1, sy + 1, &gc.heightDeviance[1]);

   heightDevLine(sx + 2, sy, sx + 4, sy + 2, sx + 3, sy + 1, &gc.heightDeviance[1]);
   heightDevLine(sx + 2, sy + 2, sx + 4, sy, sx + 3, sy + 1, &gc.heightDeviance[1]);

   heightDevLine(sx, sy + 2, sx + 2, sy + 4, sx + 1, sy + 3, &gc.heightDeviance[1]);
   heightDevLine(sx + 2, sy + 4, sx, sy + 2, sx + 1, sy + 3, &gc.heightDeviance[1]);

   heightDevLine(sx + 2, sy + 2, sx + 4, sy + 4, sx + 3, sy + 3, &gc.heightDeviance[1]);
   heightDevLine(sx + 2, sy + 4, sx + 4, sy + 2, sx + 3, sy + 3, &gc.heightDeviance[1]);

   gc.heightDeviance[2] = gc.heightDeviance[1];

   heightDevLine(sx, sy, sx + 4, sy, sx + 2, sy, &gc.heightDeviance[2]);
   heightDevLine(sx, sy + 4, sx + 4, sy + 4, sx + 2, sy + 4, &gc.heightDeviance[2]);
   heightDevLine(sx, sy, sx, sy + 4, sx, sy + 2, &gc.heightDeviance[2]);
   heightDevLine(sx + 4, sy, sx + 4, sy + 4, sx + 4, sy + 2, &gc.heightDeviance[2]);

   for(U32 j = 0; j < 4; j++)
   {
      for(U32 i = 0; i < 4; i++)
      {
         TerrainBlock::Material *mat = getMaterial(sx + i, sy + j);
         if(mat->flags & TerrainBlock::Material::Empty)
            gc.emptyFlags |= (1 << (j * 4 + i));
      }
   }
}

void TerrainFile::buildGridMap()
{
   S32 y;
   for(y = 0; y < TerrainBlock::ChunkSquareWidth; y++)
      for(U32 x = 0; x < TerrainBlock::ChunkSquareWidth; x++)
         buildChunkDeviance(x, y);

   GridSquare * gs = mGridMapBase;
   S32 i;
   for(i = TerrainBlock::BlockShift; i >= 0; i--)
   {
      mGridMap[i] = gs;
      gs += 1 << (2 * (TerrainBlock::BlockShift - i));
   }
   for(i = TerrainBlock::BlockShift; i >= 0; i--)
   {
      S32 squareCount = 1 << (TerrainBlock::BlockShift - i);
      S32 squareSize = (TerrainBlock::BlockSize) / squareCount;

      for(S32 squareX = 0; squareX < squareCount; squareX++)
      {
         for(S32 squareY = 0; squareY < squareCount; squareY++)
         {
            U16 min = 0xFFFF;
            U16 max = 0;
            U16 mindev45 = 0;
            U16 mindev135 = 0;

            // determine max error for both possible splits.

            const Point3F p1(0, 0, getHeight(squareX * squareSize, squareY * squareSize));
            const Point3F p2(0, (F32)squareSize, getHeight(squareX * squareSize, squareY * squareSize + squareSize));
            const Point3F p3((F32)squareSize, (F32)squareSize, getHeight(squareX * squareSize + squareSize, squareY * squareSize + squareSize));
            const Point3F p4((F32)squareSize, 0, getHeight(squareX * squareSize + squareSize, squareY * squareSize));

            // pl1, pl2 = split45, pl3, pl4 = split135
            const PlaneF pl1(p1, p2, p3);
            const PlaneF pl2(p1, p3, p4);
            const PlaneF pl3(p1, p2, p4);
            const PlaneF pl4(p2, p3, p4);

            bool parentSplit45 = false;
            GridSquare *parent = NULL;
            if(i < TerrainBlock::BlockShift)
            {
               parent = findSquare(i+1, Point2I(squareX * squareSize, squareY * squareSize));
               parentSplit45 = parent->flags & GridSquare::Split45;
            }
            bool empty = true;
            bool hasEmpty = false;

            for(S32 sizeX = 0; sizeX <= squareSize; sizeX++)
            {
               for(S32 sizeY = 0; sizeY <= squareSize; sizeY++)
               {
                  S32 x = squareX * squareSize + sizeX;
                  S32 y = squareY * squareSize + sizeY;

                  if(sizeX != squareSize && sizeY != squareSize)
                  {
                     TerrainBlock::Material *mat = getMaterial(x, y);
                     if(!(mat->flags & TerrainBlock::Material::Empty))
                        empty = false;
                     else
                        hasEmpty = true;
                  }
                  U16 ht = getHeight(x, y);

                  if(ht < min)
                     min = ht;
                  if(ht > max)
                     max = ht;
                  Point3F pt((F32)sizeX, (F32)sizeY, (F32)ht);
                  U16 dev;

                  if(sizeX < sizeY)
                     dev = calcDev(pl1, pt);
                  else if(sizeX > sizeY)
                     dev = calcDev(pl2, pt);
                  else
                     dev = Umax(calcDev(pl1, pt), calcDev(pl2, pt));

                  if(dev > mindev45)
                     mindev45 = dev;

                  if(sizeX + sizeY < squareSize)
                     dev = calcDev(pl3, pt);
                  else if(sizeX + sizeY > squareSize)
                     dev = calcDev(pl4, pt);
                  else
                     dev = Umax(calcDev(pl3, pt), calcDev(pl4, pt));

                  if(dev > mindev135)
                     mindev135 = dev;
               }
            }
            GridSquare *sq = findSquare(i, Point2I(squareX * squareSize, squareY * squareSize));
            sq->minHeight = min;
            sq->maxHeight = max;

            sq->flags = empty ? GridSquare::Empty : 0;
            if(hasEmpty)
               sq->flags |= GridSquare::HasEmpty;

            bool shouldSplit45 = ((squareX ^ squareY) & 1) == 0;
            bool split45;

            //split45 = shouldSplit45;
            if(i == 0)
               split45 = shouldSplit45;
            else if(i < 4 && shouldSplit45 == parentSplit45)
               split45 = shouldSplit45;
            else
               split45 = mindev45 < mindev135;

            //split45 = shouldSplit45;
            if(split45)
            {
               sq->flags |= GridSquare::Split45;
               sq->heightDeviance = mindev45;
            }
            else
               sq->heightDeviance = mindev135;
            if(parent)
               if(parent->heightDeviance < sq->heightDeviance)
                  parent->heightDeviance = sq->heightDeviance;
         }
      }
   }
   for (y = 0; y < TerrainBlock::BlockSize; y++)
   {
      for (S32 x=0; x < TerrainBlock::BlockSize; x++)
      {
         GridSquare *sq = findSquare(0, Point2I(x, y));
         S32 xpl = (x + 1) & TerrainBlock::BlockMask;
         S32 ypl = (y + 1) & TerrainBlock::BlockMask;
         for(U32 i = 0; i < TerrainBlock::MaterialGroups; i++)
         {
            if (mMaterialAlphaMap[i] != NULL) {
               U32 mapVal = (mMaterialAlphaMap[i][(y << TerrainBlock::BlockShift) + x]     +
                             mMaterialAlphaMap[i][(ypl << TerrainBlock::BlockShift) + x]   +
                             mMaterialAlphaMap[i][(ypl << TerrainBlock::BlockShift) + xpl] +
                             mMaterialAlphaMap[i][(y << TerrainBlock::BlockShift) + xpl]);
               if(mapVal)
                  sq->flags |= (GridSquare::MaterialStart << i);
            }
         }
      }
   }
   for (y = 0; y < TerrainBlock::BlockSize; y += 2)
   {
      for (S32 x=0; x < TerrainBlock::BlockSize; x += 2)
      {
         GridSquare *sq = findSquare(1, Point2I(x, y));
         GridSquare *s1 = findSquare(0, Point2I(x, y));
         GridSquare *s2 = findSquare(0, Point2I(x+1, y));
         GridSquare *s3 = findSquare(0, Point2I(x, y+1));
         GridSquare *s4 = findSquare(0, Point2I(x+1, y+1));
         sq->flags |= (s1->flags | s2->flags | s3->flags | s4->flags) & ~(GridSquare::MaterialStart -1);
      }
   }
   GridSquare *s = findSquare(1, Point2I(0, 0));
   U16 *dflags = mFlagMap;
   U16 *eflags = mFlagMap + TerrainBlock::FlagMapWidth * TerrainBlock::FlagMapWidth;

   for(;dflags != eflags;s++,dflags++)
      *dflags = s->flags;
}

void TerrainFile::initMaterialList()
{
   for( U32 i = 0; i < TerrainBlock::MaterialGroups; ++ i )
      if( mMaterialFileName[ i ].isNotEmpty() )
      {
         // Cut off the path and extension for the sake of
         // ugly global texture name mapping.

         Path path( mMaterialFileName[ i ] );
         mMaterialList.push_back( path.getFileName() );
      }

   mMaterialList.mapMaterials();
}

//--------------------------------------

template<> void *Resource<TerrainFile>::create(const Path &path)
{
   Path terRelPath = Platform::makeRelativePathName(path.getFullPath(), Platform::getMainDotCsDir());

   FileStream  stream;

   stream.open( path.getFullPath(), File::Read );
   if ( stream.getStatus() != Stream::Ok )
   {
      Con::errorf( "Resource<TerrainFile>::create - could not open '%s'", path.getFullPath().c_str() );
      return NULL;
   }

   U8 version;
   stream.read(&version);
   if (version > TerrainFile::FILE_VERSION)
   {
      Con::errorf( "Resource<TerrainFile>::create - file version '%i' is newer than engine version '%i'", version, TerrainFile::FILE_VERSION );
      return NULL;
   }

   TerrainFile* ret = new TerrainFile;

   ret->mFileVersion = version;
   ret->mFilePath = path;

   // read the HeightField
   for (S32 i=0; i < (TerrainBlock::BlockSize * TerrainBlock::BlockSize); i++)
      stream.read(&ret->mHeightMap[i]);

   // read the material group map and flags...
   dMemset(ret->mMaterialMap, 0, sizeof(ret->mMaterialMap));
   TerrainBlock::Material * materialMap = (TerrainBlock::Material*)ret->mMaterialMap;

   AssertFatal(!(TerrainBlock::Material::PersistMask & TerrainFile::MATERIAL_GROUP_MASK),
      "Doh! We have flag clobberage...");

   for (S32 j=0; j < (TerrainBlock::BlockSize * TerrainBlock::BlockSize); j++)
   {
      U8 val;
      stream.read(&val);

      //
      ret->mBaseMaterialMap[j] = val & TerrainFile::MATERIAL_GROUP_MASK;
      materialMap[j].flags = val & TerrainBlock::Material::PersistMask;
   }

   // read the MaterialList Info
   S32 k, maxMaterials = TerrainBlock::MaterialGroups;
   for(k=0; k < maxMaterials;)
   {
      String matRelFile;
      stream.read(&matRelFile);

      if(matRelFile.isNotEmpty() && version > 3)
      {
         // From version 4, paths are stored relative to the terrain file
         String   matFileStr = Path::CompressPath( terRelPath.getPath() + '/' + matRelFile );

         ret->mMaterialFileName[k] = matFileStr;
      }
      else
      {
         ret->mMaterialFileName[k] = matRelFile;
      }

      if(!ret->mMaterialFileName[k].isEmpty())
         k++;
      else
         maxMaterials--;
   }
   for(;k < TerrainBlock::MaterialGroups; k++)
      ret->mMaterialFileName[k] = String();

   if (version <= 3)
   {
      GTexHandle terrainMat = NULL;

      Path matRelPath;

      // Try to automatically fix up our material file names
      for (U32 i = 0; i < TerrainBlock::MaterialGroups; i++)
      {
         if (ret->mMaterialFileName[i].isNotEmpty())
         {
            terrainMat.set(ret->mMaterialFileName[i], &GDefaultPersistentProfile, avar("%s() - terrainMat (line %d)", __FUNCTION__, __LINE__));
            if (terrainMat)
               continue;

            matRelPath = ret->mMaterialFileName[i];

            String tempPath = matRelPath.getPath();

            String::SizeType n = tempPath.find( '/', 0, String::NoCase );
            if( n != String::NPos )
            {
               matRelPath.setPath( String(Con::getVariable( "$defaultGame" )) + tempPath.substr( n, tempPath.length() - n ) );

               terrainMat.set(matRelPath, &GDefaultPersistentProfile, avar("%s() - terrainMat (line %d)", __FUNCTION__, __LINE__));
               if (terrainMat)
               {
                  ret->mMaterialFileName[i] = matRelPath.getFullPath();
                  ret->mNeedsResaving = true;
               }
            }
         }
      }
   }

   if(version == 1)
   {
      for(S32 j = 0; j < (TerrainBlock::BlockSize * TerrainBlock::BlockSize); j++)
      {
         if (ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]] == NULL) {
            ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]] = new U8[TerrainBlock::BlockSize * TerrainBlock::BlockSize];
            dMemset(ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]], 0, TerrainBlock::BlockSize * TerrainBlock::BlockSize);
         }

         ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]][j] = 255;
      }
   }
   else
   {
      for(S32 k=0; k < TerrainBlock::MaterialGroups; k++)
      {
         if(ret->mMaterialFileName[k].isNotEmpty())
         {
            AssertFatal(ret->mMaterialAlphaMap[k] == NULL, "Bad assumption.  There should be no alpha map at this point...");
            ret->mMaterialAlphaMap[k] = new U8[TerrainBlock::BlockSize * TerrainBlock::BlockSize];
            stream.read(TerrainBlock::BlockSize * TerrainBlock::BlockSize, ret->mMaterialAlphaMap[k]);
         }
      }
   }
   if(version >= 3)
   {
      U32 len;
      stream.read(&len);
      ret->mTextureScript = (char *) dMalloc(len + 1);
      stream.read(len, ret->mTextureScript);
      ret->mTextureScript[len] = 0;

      stream.read(&len);
      ret->mHeightfieldScript = (char *) dMalloc(len + 1);
      stream.read(len, ret->mHeightfieldScript);
      ret->mHeightfieldScript[len] = 0;
   }
   else
   {
      ret->mTextureScript = 0;
      ret->mHeightfieldScript = 0;
   }

   if (version >= 5)
   {
       for (S32 i=0; i<2 ;i++)
       {
          String tfilename = stream.readSTString(true);
          if (tfilename.isNotEmpty())
          {
             Resource<TerrainFile> terr = ResourceManager::get().load(tfilename);
             AssertISV(bool(terr),tfilename); //TODO: Better error message
             ret->mEdgeTerrainFiles[i] = terr;
          }
       }
   }

   ret->initMaterialList();
   ret->buildGridMap();
   return ret;
}

template<> ResourceBase::Signature  Resource<TerrainFile>::signature()
{
   return MakeFourCC('t','e','r','d');
}

void TerrainBlock::setBaseMaterial(U32 /*x*/, U32 /*y*/, U8 /*matGroup*/)
{
}

void TerrainBlock::addOpacitySources()
{
   U32 numOpacityLayers = 0;

   for (U32 i = 0; i < TerrainBlock::MaterialGroups; i++)
   {
      if(mFile->mMaterialAlphaMap[i])
         numOpacityLayers++;
   }

   if (numOpacityLayers == 0)
      numOpacityLayers++;

   U32 numOpacity = (numOpacityLayers - 1) / 4 + 1;

   bool needRegister = false;

   // Allocate the opacity map if it isn't present.
   for (U32 i = mOpacityMaps.size(); i < numOpacity; i++)
   {
      needRegister = true;

      mOpacityMaps.push_back(new GBitmap(TerrainBlock::BlockSize, TerrainBlock::BlockSize, 0, GFormatR8G8B8A8));

      // Opacity map is vertex based, so pass an offset in
      mOpacitySources.push_back(new TerrClipMapImageSource(mOpacityMaps[i], 0.5f));
   }

   if (needRegister)
   {
      if (mClipBlender)
      {
         mClipBlender->clearOpacitySources();

         for (U32 i = 0; i < mOpacitySources.size(); i++)
            mClipBlender->registerOpacitySource(mOpacitySources[i]);
      }
   }
}

void TerrainBlock::updateOpacity()
{
   PROFILE_SCOPE(TerrainBlock_updateOpacity);

   // Make sure we have enough mOpacityMaps
   addOpacitySources();

   // Copy our various opacity maps in.
   for(S32 opacityLyr = 0; opacityLyr < TerrainBlock::MaterialGroups; opacityLyr++)
   {
      // Skip empty layers.
      if(!mFile->mMaterialAlphaMap[opacityLyr])
         continue;

      GBitmap* opacityMap = mOpacityMaps[U32(opacityLyr) / 4];

      // Not empty - so copy it in.
      U8 *tmpBits = (U8*)opacityMap->getWritableBits() + opacityLyr % 4;
      U8 *opacityBits = mFile->mMaterialAlphaMap[opacityLyr];
      const U8 *opacityEnd = opacityBits + (TerrainBlock::BlockSize*TerrainBlock::BlockSize);

      while(opacityBits != opacityEnd)
      {
         *tmpBits = *opacityBits;
         tmpBits += 4; opacityBits++;
      }
   }

   // Swizzle opacity map into device format.
   for (U32 i = 0; i < mOpacityMaps.size(); i++)
      GRAPHIC->getDeviceSwizzle32()->InPlace((U8*)mOpacityMaps[i]->getWritableBits(), 
         4 * TerrainBlock::BlockSize*TerrainBlock::BlockSize);

   // Update the blender image cache with our current source materials.
   if(mClipBlender)
   {
      mClipBlender->clearSourceImages();
      for(S32 i=0; i<TerrainBlock::MaterialGroups; i++)
         mClipBlender->registerSourceImage(mFile->mMaterialFileName[i]);
   }

   // Flush the clipmap (if it's active and has levels)
   if(mClipMap->mLevels.size())
      mClipMap->fillWithTextureData();
}

bool TerrainBlock::isTiling() const
{
   return mTile;
}

void TerrainBlock::refillClipmap()
{
   mClipMap->fillWithTextureData();
}

void TerrainBlock::setMaterial(S32 offset, const String &newValue)
{
   if(offset > -1 && offset < MaterialGroups)
      mMaterialFileName[offset] = newValue;
}


//-----------------------------------------------------------------------------
// Console Methods
//-----------------------------------------------------------------------------

ConsoleMethod(TerrainBlock, save, bool, 3, 3, "(string fileName) - saves the terrain block's terrain file to the specified file name.")
{
   char filename[256];
   dStrcpy(filename,argv[2]);
   char *ext = dStrrchr(filename, '.');
   if (!ext || dStricmp(ext, ".ter") != 0)
      dStrcat(filename, ".ter");
   return object->save(filename);
}

ConsoleFunction(getTerrainHeight, F32, 2, 3, "(Point2 pos) - gets the terrain height at the specified position."
				"@param pos The world space point, minus the z (height) value\n Can be formatted as either (\"x y\") or (x,y)\n"
				"@return Returns the terrain height at the given point as an F32 value.\n")
{
	Point2F pos;
	F32 height = 0.0f;

	if(argc == 2)
		dSscanf(argv[1],"%f %f",&pos.x,&pos.y);
	else if(argc == 3)
	{
		pos.x = dAtof(argv[1]);
		pos.y = dAtof(argv[2]);
	}

	TerrainBlock * terrain = getTerrainUnderWorldPoint(Point3F(pos.x, pos.y, 5000.0f));
	if(terrain)
	{

			Point3F offset;
			terrain->getTransform().getColumn(3, &offset);
			pos -= Point2F(offset.x, offset.y);
			terrain->getHeight(pos, &height);
	}
	return height;
}

ConsoleFunction(getTerrainHeightBelowPosition, F32, 2, 4, "(Point3F pos) - gets the terrain height at the specified position."
				"@param pos The world space point. Can be formatted as either (\"x y z\") or (x,y,z)\n"
				"@note This function is useful if you simply want to grab the terrain height underneath an object.\n"
				"@return Returns the terrain height at the given point as an F32 value.\n")
{
	Point3F pos;
	F32 height = 0.0f;

   if(argc == 2)
      dSscanf(argv[1], "%f %f %f", &pos.x, &pos.y, &pos.z);
   else if(argc == 4)
   {
      pos.x = dAtof(argv[1]);
      pos.y = dAtof(argv[2]);
      pos.z = dAtof(argv[3]);
   }

   else
   {
      Con::errorf("getTerrainHeightBelowPosition(Point3F): Invalid argument count! Valid arguments are either \"x y z\" or x,y,z\n");
      return 0;
   }

	TerrainBlock * terrain = getTerrainUnderWorldPoint(pos);
	
	Point2F nohghtPos(pos.x, pos.y);

	if(terrain)
	{
		Point3F offset;
		terrain->getTransform().getColumn(3, &offset);
		nohghtPos -= Point2F(offset.x, offset.y);
		terrain->getHeight(nohghtPos, &height);
	}
	
	return height;
}

ConsoleMethod(TerrainBlock, setTextureScript, void, 3, 3, "(string script) - sets the texture script associated with this terrain file.")
{
   object->getFile()->setTextureScript(argv[2]);
}

ConsoleMethod(TerrainBlock, setHeightfieldScript, void, 3, 3, "(string script) - sets the heightfield script associated with this terrain file.")
{
   object->getFile()->setHeightfieldScript(argv[2]);
}

ConsoleMethod(TerrainBlock, getTextureScript, const char *, 2, 2, "() - gets the texture script associated with the terrain file.")
{
   return object->getFile()->getTextureScript();
}

ConsoleMethod(TerrainBlock, getHeightfieldScript, const char *, 2, 2, "() - gets the heightfield script associated with the terrain file.")
{
   return object->getFile()->getHeightfieldScript();
}

ConsoleMethod(TerrainBlock, purgeClipmap, void, 2, 2, "Rebuilds the clipmap textures")
{
   return object->refillClipmap();
}

//World Map Helper

ConsoleFunction(buildMegaTerrain, void, 14, 14, "buildMegaTerrain helper function")
{
   SimObject *guiObject;
   SimGroup *missionGroup;

   TerrainEditor* editor = dynamic_cast<TerrainEditor*>(Sim::findObject("ETerrainEditor"));

   if( !editor )
   {
      Con::errorf("Missing ETerrainEditor!\n");
      return;
   }

   if( !Sim::findObject( "MissionGroup", missionGroup ) )
   {
      Con::errorf("Missing MissionGroup!\n");
      return;
   }

   if( !Sim::findObject( argv[1], guiObject ) )
   {
      Con::errorf("Missing GuiObjectBuilder object!\n");
      return;
   }

   S32 squareSize = dAtoi(argv[2]);
   const char* hmap = argv[3];
   F32 heightScalar = dAtof(argv[4]);

   if (squareSize != 4 && squareSize != 8 && squareSize != 16 && squareSize != 32) 
   {
      Con::errorf("Square Size - must be 4, 8, 16, or 32");
      return;
   }

   S32 width = 2;
   S32 height = 2;

   if (heightScalar < 32) heightScalar = 32;
   if (heightScalar > 255) heightScalar = 255;

   const char* detail = StringTable->insert(argv[5]);

   Vector<StringTableEntry> materials;

   for (S32 i = 6; i < 14; i++)
   {
      const char* texture = argv[i];
      if( texture && texture[ 0 ] )
         materials.push_back(StringTable->insert(texture));
   }

   Resource< GBitmap > heightmap;
   if( hmap && hmap[ 0 ] )
   {
      heightmap = GBitmap::load(hmap);
      if( heightmap != NULL )
      {
         U32 hwidth = heightmap->getWidth();
         U32 hheight = heightmap->getHeight();

         if (hwidth != 512 || hheight != 512)
         {
            Con::errorf("Heightmap - height and width must be 512");
            return;
         }

         // Use heightmap width and height
         width = hwidth/256;
         height = hheight/256;

      }
   }

   SimGroup* mapGroup = new SimGroup();
   mapGroup->assignName("MegaTerrain");
   mapGroup->registerObject();

   char missionName[1024], terrainName[1024];
   dSprintf(missionName,sizeof(missionName),"%s",Con::getVariable("$Client::MissionFile"));
   char * dot = dStrstr((const char*)missionName, ".mis");
   if(dot)
      *dot = '\0';

   S32 terrainSize = squareSize * TerrainBlock::BlockSize;
   S32 worldY = (-terrainSize*height)/2;
   U16 hinfo[TerrainBlock::BlockSize * TerrainBlock::BlockSize];

   Terraformer* tformer = new Terraformer();
   tformer->setTerrainInfo(512, squareSize, 0.0f, heightScalar*16.0f, 0.0f);

   if (heightmap)
   {
      tformer->loadGreyscale(0, heightmap);

      // Smoothing could be exposed to object builder
      tformer->smooth(0, 0, 1.0f, 4);
   }

   TerrainBlock* neighbors[4][4];

   for (S32 y=0; y<height; y++, worldY+=terrainSize)
   {
      S32 worldX = (-terrainSize*width)/2;

      for (S32 x=0; x<width; x++, worldX+=terrainSize)
      {
         dSprintf(terrainName,sizeof(terrainName),"%s_megaterrain_%i_%i",missionName,x,y);

         TerrainBlock *tblock = new TerrainBlock();

         tformer->setHeightmap(0, hinfo, heightScalar*16.f, x*TerrainBlock::BlockSize, y*TerrainBlock::BlockSize);
         
         tblock->mTerrFileName = StringTable->insert(terrainName);
         tblock->createNewTerrainFile(terrainName,hinfo,&materials);
         
         tblock->setTiling(false);
         tblock->setSquareSize(squareSize);
         tblock->setPosition(Point3F(worldX,worldY,0));

         tblock->mDetailTextureName = detail;

         tblock->registerObject();
         mapGroup->addObject(tblock);

         editor->attachTerrain(tblock);

         neighbors[x][y]=tblock;

      }
   }

   delete tformer;

   missionGroup->addObject(mapGroup);

   // Setup neighbors
   for (S32 y=0; y<height; y++)
      for (S32 x=0; x<width; x++)
      {
         if (x != width-1)
            neighbors[x][y]->getFile()->mEdgeTerrainFiles[0]=neighbors[x+1][y]->getFile();
         if (y!=height-1)
            neighbors[x][y]->getFile()->mEdgeTerrainFiles[1]=neighbors[x][y+1]->getFile();
      }

   editor->setMissionDirty();
   editor->setDirty();

}
