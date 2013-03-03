//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "core/util/tVector.h"
#include "core/frameAllocator.h"
#include "math/mathIO.h"
#include "core/util/safeDelete.h"
#include "util/triBoxCheck.h"
#include "atlas/editor/atlasDiscreteMesh.h"
#include "atlas/resource/atlasObjectChunk.h"
#include "atlas/resource/atlasResourceObjectTOC.h"
#include "atlas/runtime/atlasInstance2.h"


AtlasObjectChunk::TSStaticNode::TSStaticNode()
{
	objectType = 0;
	objectId = -1;
	pos.set(0,0,0);
	rot.set(0,0,0,0);
	scale.set(1,1,1);
	objectType = tsStatic_type;
	shapeLength = 0;
	materialCount = 0;


}
AtlasObjectChunk::TSStaticNode::~TSStaticNode()
{

}
void AtlasObjectChunk::TSStaticNode::read(Stream *s)
{
	s->read(&objectType);
	s->readString(object_Name);
	s->read(&objectId);

	s->read(&pos.x);
	s->read(&pos.y);
	s->read(&pos.z);

	s->read(&rot.x);
	s->read(&rot.y);
	s->read(&rot.z);
	s->read(&rot.w);
	s->readString(shapeFileName);
	s->read(&shapePos);
	s->read(&shapeLength);

}
void AtlasObjectChunk::TSStaticNode::write(Stream *s)
{
	s->write(objectType);
	s->writeString(object_Name);
	s->write(objectId);

	s->write(pos.x);
	s->write(pos.y);
	s->write(pos.z);

	s->write(rot.x);
	s->write(rot.y);
	s->write(rot.z);
	s->write(rot.w);
	s->writeString(shapeFileName);
	s->write(shapePos);
	s->write(shapeLength);
}
AtlasObjectChunk::AtlasObjectChunk()
{

   // Set a default invalid bounding box.
   mBounds.minExtents.set(1,1,1);
   mBounds.maxExtents.set(-1,-1,-1);

   mTCBounds.point.set(0,0);
   mTCBounds.extent.set(0,0);
   mTSObjects = NULL;
   mMaterials = NULL;
   mBitmaps = NULL;
   mSceneObject = NULL;
   bitmapCount = 0;
   tsObjectCount = 0;
   materialCount = 0;
   mObjectTreeDepth = 0;

   mOwningStub = 0;
}

AtlasObjectChunk::~AtlasObjectChunk()
{
	U32 i = 0;
	if(tsObjectCount>0)
	{	
		for(U32 i = 0;i<tsObjectCount;i++)
		{
			SimObject* obj = Sim::findObject(mTSObjects[i].objectId);
			if(obj)
				obj->deleteObject();
		}
		delete[] mTSObjects;
	}
	if(materialCount > 0)
	{
		if(mMaterials)
			  delete[] mMaterials;
	}

	if(bitmapCount > 0)
	{
		if(mBitmaps)
			  delete[] mBitmaps;
	}
}
bool AtlasObjectChunk::addTSStatic(TSStatic* obj)
{
	for(S32 i=0;i<tsObjectCount;i++)
	{
		if(mTSObjects[i].objectId == obj->getId())
			return false;
	}
	TSStaticNode* tsNodes = new TSStaticNode[tsObjectCount+1];
	for(S32 i =0;i<tsObjectCount;i++)
	{
        dMemcpy(&tsNodes[i],&mTSObjects[i],sizeof(mTSObjects[i]));
	}

	dSprintf(tsNodes[tsObjectCount].object_Name,256,"%s", obj->getName());
	tsNodes[tsObjectCount].objectId = obj->getId();
	tsNodes[tsObjectCount].pos = obj->getPosition();
	MatrixF mat = obj->getTransform();
	QuatF rot = QuatF(mat);

	F32 x = rot.x;
	F32 y = rot.y;
	F32 z = rot.z;
	F32 w = rot.w;

	tsNodes[tsObjectCount].rot.x = x;
	tsNodes[tsObjectCount].rot.y = y;
	tsNodes[tsObjectCount].rot.z = z;
	tsNodes[tsObjectCount].rot.w = w;
	tsNodes[tsObjectCount].scale = obj->getScale();
	dSprintf(tsNodes[tsObjectCount].shapeFileName,256,"%s", obj->getShapeName());
	tsNodes[tsObjectCount].materialCount = obj->getShape()->materialList->size();

	if(mTSObjects)
	   delete[] mTSObjects;
	mTSObjects = tsNodes;
	tsObjectCount = tsObjectCount+1;
	obj->mCreateTypes = SceneObject::atlas;
	return true;

}
bool AtlasObjectChunk::removeTSStaic(TSStatic* obj)
{
	if(tsObjectCount<1)
		return false;
	S32  index = -1;
	for(U32 i=0;i<tsObjectCount;i++)
	{
		if(mTSObjects[i].objectId == obj->getId())
		{
			index = i;
			break;
		}
	}
	if(index == -1)
		return false;

	TSStaticNode* tsNodes = new TSStaticNode[tsObjectCount-1];
	S32 copyIndex = 0;
	for(S32 i =0;i<(tsObjectCount-1);i++)
	{
		if(i == index)
			copyIndex ++;
		dMemcpy(&tsNodes[i],&mTSObjects[i],sizeof(mTSObjects[copyIndex]));
		copyIndex ++;
	}

	if(mTSObjects)
		delete[] mTSObjects;
	mTSObjects = tsNodes;
	tsObjectCount = tsObjectCount-1;

    return true;
}
bool AtlasObjectChunk::updateTSStaic(TSStatic* obj)
{
	for(S32 i=0;i<tsObjectCount;i++)
	{
		if(mTSObjects[i].objectId == obj->getId())
		{
			dSprintf(mTSObjects[i].object_Name,256,"%s", obj->getName());
			mTSObjects[i].objectId = obj->getId();
			mTSObjects[i].pos = obj->getPosition();
			MatrixF mat = obj->getTransform();
			QuatF rot = QuatF(mat);

			F32 x = rot.x;
			F32 y = rot.y;
			F32 z = rot.z;
			F32 w = rot.w;

			mTSObjects[i].rot.x = x;
			mTSObjects[i].rot.y = y;
			mTSObjects[i].rot.z = z;
			mTSObjects[i].rot.w = w;
			mTSObjects[i].scale = obj->getScale();
			dSprintf(mTSObjects[i].shapeFileName,256,"%s", obj->getShapeName());
			mTSObjects[i].materialCount = obj->getShape()->materialList->size();
			return true;
		}

	}
	return false;
}
U32 AtlasObjectChunk::getHeadSentinel()
{
   return MakeFourCC('a', 'o', 'c', 'h');
}

U32 AtlasObjectChunk::getTailSentinel()
{
   return MakeFourCC('a', 'o', 'c', 't');
}

void AtlasObjectChunk::generate(AtlasChunk *children[4])
{


   Box3F b ;
   
   // Make sure that our new bounds include all the child bounds. This will
   // help prevent culling/LOD strangeness.
   for(S32 i=0; i<4; i++)
   {
      Box3F childBounds = ((AtlasObjectChunk*)children[i])->getBounds();

      AssertFatal(childBounds.minExtents.x >= -100.f, "AtlasObjectChunk::generate - bad child bounds! (1)");
      AssertFatal(childBounds.minExtents.y >= -100.f, "AtlasObjectChunk::generate - bad child bounds! (2)");
      AssertFatal(childBounds.minExtents.z >= -100.f, "AtlasObjectChunk::generate - bad child bounds! (3)");

      b.minExtents.setMin( childBounds.minExtents );
      b.maxExtents.setMax( childBounds.maxExtents );

      AssertFatal(b.minExtents.x >= -100.f, "AtlasObjectChunk::generate - bad parent bounds! (1)");
      AssertFatal(b.minExtents.y >= -100.f, "AtlasObjectChunk::generate - bad parent bounds! (2)");
      AssertFatal(b.minExtents.z >= -100.f, "AtlasObjectChunk::generate - bad parent bounds! (3)");
   }

   // Update our bounds.
   mBounds = b;

   /*Con::printf("       - FINAL mesh bounds = (%f, %f, %f), (%f, %f, %f)",
      mBounds.minExtents.x, mBounds.minExtents.y, mBounds.minExtents.z,
      mBounds.maxExtents.x, mBounds.maxExtents.y, mBounds.maxExtents.z); */



   // All done!
}
void AtlasObjectChunk::write(Stream *s)
{
   Parent::write(s);

   // Serialize our bounds.
   mathWrite(*s, mBounds);

   U32 idxStartPos = s->getPosition();
   S32 i = 0;
   // Write our indices.
   s->write(tsObjectCount);
   
   for(i=0; i<tsObjectCount; i++)
   {

	  mTSObjects[i].write(s);
   }

 
   U32 vertEndPos = s->getPosition();
}

void AtlasObjectChunk::read(Stream *s)
{
   Parent::read(s);

   // Read our bounds.
   mathRead(*s, &mBounds);
   S32 i = 0;
   // Read our indices.
   s->read(&tsObjectCount);
   if(tsObjectCount>0)
   {
	   if(mTSObjects)
		   delete[] mTSObjects;
	   mTSObjects = new TSStaticNode[tsObjectCount];
	   for( i= 0;i<tsObjectCount;i++)
		   mTSObjects[i].read(s);
   }

   if(tsObjectCount<=0)
	   return;

   for(U32 i =0;i<tsObjectCount;i++)
   {
	   TSStatic *tsObject = new TSStatic();
	   tsObject->mCreateTypes = TSStatic::atlas;
	   tsObject->setPosition(mTSObjects[i].pos);
	   char rotBuff[64];
	   tsObject->setShapeName(mTSObjects[i].shapeFileName);
	   dSprintf(rotBuff,64,"%f %f %f %f",mTSObjects[i].rot.x,mTSObjects[i].rot.y,mTSObjects[i].rot.z,mTSObjects[i].rot.w);
	   tsObject->setField("rotation",rotBuff);
	   tsObject->setScale(mTSObjects[i].scale);
	   //tsObject->mAtlasObjectStub = this;

	   SimObject * obj = NULL;
	   tsObject->mToc = dynamic_cast<AtlasResourceObjectTOC*>(mOwningTOC);
	   tsObject->mStub = mOwningStub;
	   tsObject->registerObject(mTSObjects[i].objectId);

   }
}

AtlasObjectChunk *AtlasObjectChunk::generateCopy(S32 reformat)
{
   AtlasObjectChunk *aoc = new AtlasObjectChunk();
   aoc->mBounds = mBounds;
   aoc->mTCBounds = mTCBounds;
   aoc->tsObjectCount = tsObjectCount;
   U32 i =0;
   if(tsObjectCount>0)
   {
	   aoc->mTSObjects = new TSStaticNode[tsObjectCount];
	   for(i = 0;i<tsObjectCount;i++)
		   dMemcpy(&(aoc->mTSObjects[i]),&mTSObjects[i],sizeof(mTSObjects[i]));
		   
   }

   aoc->bitmapCount = bitmapCount;
   //if(bitmapCount>0)
   //{
	  // aoc->mBitmaps.setSize(bitmapCount);
	  // for(i = 0;i<bitmapCount;i++)
		 //  aoc->mBitmaps[i] = mBitmaps[i];
   //}
   aoc->materialCount = materialCount;
   //if(materialCount>0)
   //{
	  // aoc->mMaterials.setSize(materialCount);
	  // for(i = 0;i<materialCount;i++)
		 //  aoc->mMaterials[i] = mMaterials[i];
   //}

   return aoc;
}

//-----------------------------------------------------------------------------

void AtlasObjectChunk::process()
{

}

void AtlasObjectChunk::render()
{
}

Box3F AtlasObjectChunk::getBounds()
{
   return mBounds;
}

void AtlasObjectChunk::generateCollision()
{

}



void AtlasObjectChunk::calculatePoints(const Point3F &camPos, Point3F &outNearPos, 
                                     Point3F &outFarPos, Point2F &outNearTC, Point2F &outFarTC)
{
   // The camera position is relative to our bounding box, so in filespace.
   const Point3F &min = mBounds.minExtents;
   const Point3F &max = mBounds.maxExtents;
   Point3F center;
   mBounds.getCenter(&center);

   if      (camPos.x <= min.x) { outNearPos.x = min.x; outFarPos.x = max.x; }
   else if (camPos.x >  max.x) { outNearPos.x = max.x; outFarPos.x = min.x; }
   else                        { outNearPos.x = camPos.x; outFarPos.x = (camPos.x > center.x ?  min.x : max.x); }

   if      (camPos.y <= min.y) { outNearPos.y = min.y; outFarPos.y = max.y; }
   else if (camPos.y >  max.y) { outNearPos.y = max.y; outFarPos.y = min.y; }
   else                        { outNearPos.y = camPos.y; outFarPos.y = (camPos.y > center.y ?  min.y : max.y); }

   if      (camPos.z <= min.z) { outNearPos.z = min.z; outFarPos.z = max.z; }
   else if (camPos.z >  max.z) { outNearPos.z = max.z; outFarPos.z = min.z; }
   else                        { outNearPos.z = camPos.z; outFarPos.z = (camPos.z > center.z ?  min.z : max.z); }

   // This is a hack but since we only care about delta at this point might be
   // ok. Fix me later -- BJG
   outNearTC = mTCBounds.point;
   outFarTC = mTCBounds.point + mTCBounds.extent;
}


