//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "atlas/resource/atlasResourceObjectTOC.h"
#include "core/frameAllocator.h"
#include "math/mathIO.h"

void AtlasResourceObjectStub::read(Stream *s, const U32 version)
{
   Parent::read(s, version);

   // Ok, load that box.
   mathRead(*s, &mBounds);

   // Check sentinel.
   U8 sentinel;
   s->read(&sentinel);

   AssertISV(sentinel == 0xd0, "AtlasResourceObjectStub::read - did not encounter valid sentinel.")

}

void AtlasResourceObjectStub::write(Stream *s)
{
   Parent::write(s);
   mathWrite(*s, mBounds);

   // Add a sentinel
   s->write(U8(0xd0));
}


//-------------------------------------------------------------------------

AtlasResourceObjectTOC::AtlasResourceObjectTOC()
{
   mGoalBatchSize = 1500;
   mDistance_LODMax = 50.f;
   mIsQuadtree = true;
}

AtlasResourceObjectTOC::~AtlasResourceObjectTOC()
{
}

bool AtlasResourceObjectTOC::write(Stream *s)
{
   s->write(mGoalBatchSize);

   return Parent::write(s);
}

bool AtlasResourceObjectTOC::read(Stream *s)
{
   s->read(&mGoalBatchSize);

   return Parent::read(s);
}

void AtlasResourceObjectTOC::instateLoadedChunk(StubType *stub, ChunkType *newChunk )
{
   // Got a new chunk, so let's update the stub's bounding box.
   stub->mBounds = newChunk->getBounds();

   // And pass control on up.
   Parent::instateLoadedChunk(stub, newChunk);
}
void AtlasResourceObjectTOC::AddObject(SceneObject* obj,StubType* stub)
{
	immediateLoad(stub,AtlasTOC::EditorLoad);
	
	//getAtlasFile()->startLoaderThreads();
	AtlasObjectChunk* aoc = stub->mChunk;
	AtlasObjectChunk* new_aoc = aoc->generateCopy();
    new_aoc->mOwningTOC = this;
	TSStatic* tsObject = dynamic_cast<TSStatic*>(obj);
	if(tsObject && new_aoc->addTSStatic(tsObject))
	{
		instateNewChunk(stub,new_aoc);
		new_aoc->mOwningStub = stub;
	}
}
void AtlasResourceObjectTOC::updateObject(SceneObject* obj,StubType* stub)
{
	immediateLoad(stub,AtlasTOC::EditorLoad);

	//getAtlasFile()->startLoaderThreads();
	AtlasObjectChunk* aoc = stub->mChunk;
	AtlasObjectChunk* new_aoc = aoc->generateCopy();
	new_aoc->mOwningTOC = this;
	TSStatic* tsObject = dynamic_cast<TSStatic*>(obj);
	if(tsObject && new_aoc->updateTSStaic(tsObject))
	{
		instateNewChunk(stub,new_aoc);
	}

}
void AtlasResourceObjectTOC::removeObject(SceneObject* obj,StubType* stub)
{
	immediateLoad(stub,AtlasTOC::EditorLoad);

	//getAtlasFile()->startLoaderThreads();
	AtlasObjectChunk* aoc = stub->mChunk;
	AtlasObjectChunk* new_aoc = aoc->generateCopy();
	new_aoc->mOwningTOC = this;
	TSStatic* tsObject = dynamic_cast<TSStatic*>(obj);
	if(tsObject && new_aoc->removeTSStaic(tsObject))
	{
		instateNewChunk(stub,new_aoc);
	}

}