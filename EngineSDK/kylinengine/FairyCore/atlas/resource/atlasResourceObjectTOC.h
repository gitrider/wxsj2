//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _ATLASRESOURCEOBJECTTOC_H_
#define _ATLASRESOURCEOBJECTTOC_H_

#include "atlas/core/atlasResourceTOC.h"
#include "atlas/resource/atlasObjectChunk.h"
#include "Core/FairyPrerequisites.h"
struct RayInfo;
class Convex;
class AbstractPolyList;

/// @defgroup AtlasResource Atlas Resource System
///
/// Although the core background loading system is implemented in AtlasCore, 
/// the specific data types that are loaded, the rules for regenerating them,
/// and so forth all live in this group.
///
/// @ingroup Atlas

/// Atlas resource stub subclass for geometry.
///
/// @ingroup AtlasResource
class AtlasResourceObjectStub : public AtlasResourceStub<AtlasObjectChunk>
{
public:
   AtlasResourceObjectStub()
   {
      mBounds.minExtents.set(-100, -100, -100);
      mBounds.maxExtents.set( 100,  100,  100);
   }

   /// Geometry bounds.
   Box3F mBounds;
  
   typedef AtlasResourceStub<AtlasObjectChunk> Parent;
   virtual void read(Stream *s, const U32 version);
   virtual void write(Stream *s);
};

/// Atlas resource TOC subclass for geometry.
///
/// @ingroup AtlasResource
class AtlasResourceObjectTOC : public AtlasResourceTOC<AtlasResourceObjectStub>
{
public:

   typedef AtlasResourceTOC<AtlasResourceObjectStub> Parent;

   U32 mGoalBatchSize;
   F32 mDistance_LODMax;

   void initializeTOC(U32 depth)
   {
      helpInitializeTOC(depth);
   }

   AtlasResourceObjectTOC();
   ~AtlasResourceObjectTOC();

   virtual bool write(Stream *s);
   virtual bool read(Stream *s);

   /// Override so we can update stub bounds when a new chunk is stored.
   virtual void instateLoadedChunk(StubType *stub, ChunkType *newChunk);
   void AddObject(SceneObject* obj,StubType* stub);
   void removeObject(SceneObject* obj,StubType* stub);
   void updateObject(SceneObject* obj,StubType* stub);
}; 


#endif
