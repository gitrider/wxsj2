//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _ATLASOBJECTCHUNK_H_
#define _ATLASOBJECTCHUNK_H_

#include "atlas/core/atlasChunk.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxStructs.h"
#include "collision/convex.h"
#include "math/mPoint3.h"
#include "math/mQuat.h"
#include "core/util/tVector.h"
#include "materials/materialDefinition.h"
#include "ts/tsShape.h"
#include "client/tsStatic.h"
#include "core/util/byteBuffer.h"
#include "Core/FairyPrerequisites.h"
class AtlasInstance;

/// Atlas chunk subclass for geometry.
///
/// @ingroup AtlasResource
class AtlasObjectChunk : public AtlasChunk
{
public:
   AtlasObjectChunk();
   virtual ~AtlasObjectChunk();

   typedef AtlasChunk Parent;

   struct TSStaticNode
   {
	   TSStaticNode();
	   ~TSStaticNode();
	   char object_Name[256];
	   S32 objectType;
	   S32 objectId;
	   Point3F pos;
	   QuatF rot;
	   VectorF scale;
	   char shapeFileName[256];
	   U32 shapePos;
	   U32 shapeLength;
	   U32 materialCount;
       
	   void read(Stream *s);
	   void write(Stream *s);
   };

   TSStaticNode* mTSObjects;
   Material* mMaterials;
   GBitmap* mBitmaps;
   SceneObject* mSceneObject;

   U32 tsObjectCount;
   U32 materialCount;
   U32 bitmapCount;
   /// Bounds of this chunk; duplicated in stub so we can get to it w/o
   /// paging.
   Box3F mBounds;

   /// Bounding box for this chunk's TC's.
   RectF mTCBounds;

   enum EObjectType
   {
	   null_typ = 0,
	   interior_type ,
	   tsStatic_type,
	   effect_type,
	   staticShape_type,
	   sfxEmitter_type,
	   trigger_type,
   };



   U32 mObjectTreeDepth;
 
                        ///  NULL, there isn't any collision info.

   /// Helper function to update our collision structures for this chunk.
   void generateCollision();

   /// @}

   virtual void write(Stream *s);
   virtual void read(Stream *s);

   void generate(AtlasChunk *children[4]);

   virtual U32 getHeadSentinel();
   virtual U32 getTailSentinel();

   virtual void process();

   void render();

   /// Calculate some useful information about our near/far points, used primarily
   /// for clipmaps.
   void calculatePoints(const Point3F &camPos, Point3F &outNearPos, Point3F &outFarPos, Point2F &outNearTC, Point2F &outFarTC);

   AtlasObjectChunk *generateCopy(S32 reformat = -1);

   Box3F getBounds();
   
   void registerConvex(const U16 offset, Convex *convex, AtlasInstance *object, const Box3F &queryBox);
   Convex mConvexList;

   bool addTSStatic(TSStatic* obj);
   bool removeTSStaic(TSStatic* obj);
   bool updateTSStaic(TSStatic* obj);

   AtlasResourceObjectStub* mOwningStub;


   /// @}
};


#endif
