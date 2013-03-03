//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _ATLASRESOURCETOC_H_
#define _ATLASRESOURCETOC_H_

#include "core/stream/memstream.h"
#include "console/console.h"
#include "atlas/core/atlasBaseTOC.h"
#include "atlas/core/atlasDeferredIO.h"
#include "atlas/core/atlasRequestNote.h"
#include "atlas/core/atlasClassFactory.h"

/// Base class for resource stubs.
///
/// For a resource TOC, the stubs need to know where in the file a given chunk
/// is, as well as other information (like generation of the stub, and how to
/// perform IO).
///
/// @ingroup AtlasCore
template<class ChunkTypeT>
class AtlasResourceStub : public AtlasBaseStub
{
public:
   typedef ChunkTypeT ChunkType;

   AtlasResourceStub()
   {
      mChunk = NULL;
      optimizeOrder=stubOffset=offset=length=generation=0;
      mState = Unloaded;
      mPriority = mHeat = 0.f;
   }

   /// Is a chunk for this stub currently loaded?
   const inline bool hasResource() const
   {
      return mChunk;
   }

   ChunkType *mChunk;

   /// Position relative to file start, in bytes, for this stub's chunk.
   U32 offset;

   /// Length of the chunk.
   U32 length;

   /// fancy pants generation number... let's say this always
   /// gets tracked so we can see how many generations went
   /// into this.
   U32 generation;

   /// Offset of this stub in the file. NOT serialized; used for
   /// updating the stub.
   U32 stubOffset;

   /// When writing optimized, we can provide an optimization
   /// order which allows us to optimize for write order, e.g.
   /// on an optical disk for a console. This order is
   /// applied across all TOCs, in order of the TOCs in the
   /// the file. If the same ID is encountered in multiple
   /// places, ties are broken with that rule.
   ///
   /// Zero is considered a non-order; zeros are
   /// written after every other number.
   U32 optimizeOrder;

   /// Track the current state of this stub's paged data.
   enum {
      Unloaded, ///< No chunk data loaded.
      Pending,  ///< Requested, but not currently queued for load.
      Loading,  ///< Currently queued for load; in-flight.
      Loaded,   ///< Chunk is currently loaded.
   } mState;

   virtual void read(Stream *s, const U32 version)
   {
      s->read(&offset);
      s->read(&length);
      s->read(&generation);
   }

   virtual void write(Stream *s)
   {
      s->write(offset);
      s->write(length);
      s->write(generation);
   }

   /// Forcibly purge our chunk.
   void purge()
   {
      // Blast the chunk.
      if(mChunk)
      {
         delete mChunk;
         mChunk = NULL;

         // Getting hot!
         mHeat += 0.5f;
      }

      mState = Unloaded;
   }

   F32 mPriority;
   F32 mHeat;
   AtlasRequestHeader mRequests;
};

/// Base resource TOC implementation.
///
/// This is where most of the meat vis a vis paged loading of chunks, as well
/// as generation of Atlas files, occurs. This adds several important elements
/// to AtlasBaseTOC, most notably typed implementations of a prioritized
/// background loader for that TOC, and functionality to write updates to
/// the Atlas file.
///
/// @ingroup AtlasCore
template<class StubTypeT> 
class AtlasResourceTOC : public AtlasBaseTOC<StubTypeT>
{
public:
   typedef StubTypeT StubType;
   typedef AtlasBaseTOC<StubTypeT>  Parent;
   typedef typename Parent::ChunkType ChunkType;
      
   using Parent::mStubCount;
   using Parent::mStubs;
   using Parent::mTreeDepth;
   using Parent::mFile;
   using Parent::mIsQuadtree;
   
protected:
   virtual void cbOnChunkWriteComplete(U32 key, AtlasDeferredIO *adio);
   virtual void cbOnStubWriteComplete(U32 key, AtlasDeferredIO *adio);

   virtual void cbOnChunkReadComplete(U32 key, AtlasChunk *adio);
   virtual void cbOnChunkReadStarted(U32 stubIdx);

   virtual void cbPopulateChunkReadNote(AtlasReadNote *arn);
   virtual void queueChunkUpdate(StubType *t);

   virtual void syncCallback(const F32 deltaTime)
   {
      for(S32 i=0; i<mStubCount; i++)
         mStubs[i].mHeat = getMax(getMin(mStubs[i].mHeat, F32(5)) - deltaTime, 0.f);
   }

   /// Version of file format that we read from.
   U32 mVersion;

   /// Outstanding load requests.
   Vector<StubType*> mLoadQueue;

   /// Number of stubs currently in loading state.
   U32 mNumStubsLoading;

   /// Number of stubs that no longer needed their chunk data when it finally arrived.
   U32 mNumChunksDeadOnArrival;

   /// Number of chunk bytes in pending state.
   U32 mNumBytesPending;

   /// Number of chunk bytes in loading state.
   U32 mNumBytesLoading;

public:

   AtlasResourceTOC()
   {
      mVersion = -1;
      mNumStubsLoading = 0;
      mNumChunksDeadOnArrival = 0;
      mNumBytesPending = 0;
      mNumBytesLoading = 0;
   }

   /// Instate a loaded chunk into a stub.
   ///
   /// TOCs may want to perform callbacks or other book-keeing when a chunk
   /// is loaded, and this method provides a hook for that behavior.
   ///
   /// @note chunk is required to be a valid chunk pointer.
   virtual void instateLoadedChunk(StubType *stub, ChunkType *chunk);

   /// Purge a stub's chunk. Forcibly remove a stub's paged data from memory.
   virtual void purgeStub(StubType *stub);

   /// Instate a new chunk into a stub. This queues the provided chunk for
   /// serialization, as well as the stub.
   ///
   /// @note This is a superset of instateLoadedChunk().
   virtual void instateNewChunk(StubType *stub, ChunkType *chunk, bool blockTillSerialized = false);

   /// Internal callback to indicate to the AtlasFile that we have a pending deserialize.
   virtual void cbPostBackToAtlasFile(U32 key, AtlasDeferredIO *adio);

   /// Regenerate all the chunks that overlap with the invalidated region.
   virtual void generate(RectI invalidationRegion);

   /// Forcibly load a file immediately. Will cause a reload if the stub
   /// already has a chunk loaded.
   virtual void immediateLoad(StubType *stub, U32 reason);

   /// Called by an instance TOC when load requests are made.
   virtual void requestLoad(StubType *stub, U32 reason, F32 priority);

   /// Called by an instance TOC when load requests are cancelled.
   virtual void cancelLoadRequest(StubType *stub, U32 reason);

   /// Write the whole TOC to a stream.
   virtual bool write(Stream *s);

   /// Read the whole TOC from a stream.
   virtual bool read(Stream *s);

   /// Reserialize the passed stub to disk.
   void queueStubUpdate(StubType *t);

   /// Sort function for recalculateUpdates().
   static S32 updateStubSortFunc(const void *a, const void *b)
   {
      const StubType *aS = *(const StubType**)a;
      const StubType *bS = *(const StubType**)b;

      if( aS->mPriority == aS->mPriority )
         return 0;
      else if( aS->mPriority > bS->mPriority )
         return 1;
      else
         return -1;
   }

   /// Fill provided vector with up to maxItems load requests for this TOC,
   /// sorted by priority. Used by the AtlasFile to implement our
   /// round-robin loading scheme.
   void recalculateUpdates(Vector<AtlasReadNote*> &list, U32 maxItems);

   virtual void dumpStubLoadStatus(U32 stubIdx);

   /// Construct a new chunk of the apropriate type and link it to this TOC.
   AtlasChunk *constructNewChunk()
   {
      ChunkType * foo = new ChunkType;
      foo->mOwningTOC = this;
      return foo;
   };

   /// Atlas file version. For now we just increment a global counter for
   /// each revision of the file format.
   ///
   /// 101 - Stored bounding box for geom stubs, instead of min/max. Also
   ///       stored bounds in the chunk.
   ///
   /// 120 - Added collision information to AtlasGeomChunk.
   ///
   /// 130 - Reordered vertex data to match order of structure.
   ///
   /// 140 - Added normals to AGC.
   ///
   /// 150 - added multi-layer AtlasTexChunks to support >4 blend layers.
   ///
   /// 160 - removed geomorphing information.
   static const U32 csmVersion = 160;

   /// Return the current file format version; ie, the version we loaded.
   /// @see csmVersion for the most modern supported version.
   U32 getFormatVersion()
   {
      return mVersion;
   }

   /// Copy ourselves from the specified TOC. This only copies
   /// stubs; chunks are handled separately.
   virtual bool copyFromTOC(AtlasResourceTOC<StubType> *toc)
   {
      // Grab generic data.
      mVersion = csmVersion;

      // Copy stubs.
      mTreeDepth = toc->mTreeDepth;
      mStubCount = toc->mStubCount;
      mStubs = new StubType[mStubCount];

      for(U32 i=0; i<mStubCount; i++)
         mStubs[i] = toc->mStubs[i];

      // Subclasses can copy their stuff over on their own.
      return true;
   }

   /// Copy all of our chunks to another TOC.
   ///
   /// @param reformat Optional parameter to request a reformat of the transferred
   ///                 data. This is passed to generateCopy.
   virtual bool copyChunksToTOC(AtlasResourceTOC<StubType> *toc, S32 reformat = -1)
   {
      for(S32 i=0; i<getStubCount(); i++)
      {
         // Get our original stub, and issue the load.
         StubType *origArgs = getStub(i);
         immediateLoad(origArgs, AtlasTOC::EditorLoad);

         // Make a chunk copy and instate it.
         StubType *args = toc->getStub(i);
         AssertFatal(origArgs->hasResource(), "AtlasResourceTOC<T>::copyChunksToTOC - no chunk present in chunk copy.");

         toc->instateNewChunk(args, origArgs->mChunk->generateCopy(reformat), true);

         args->purge();
         origArgs->purge();
      }

      U32 len = mFile->getDeferredFile().lockStream(true)->getStreamSize();
      mFile->getDeferredFile().unlockStream();

      Con::printf("   - Copied %d chunks via copyChunksToTOC, stream len approx. %d bytes", getStubCount(), len);


      return true;
   }

   virtual void getIOStatus( AtlasFile::IOStatus::TOCIOStatus& status )
   {
      status.mLoadQueueLength          = mLoadQueue.size();
      status.mNumStubsLoading          = mNumStubsLoading;
      status.mNumChunksDeadOnArrival   = mNumChunksDeadOnArrival;
      status.mBytesLoading             = mNumBytesLoading;
      status.mBytesPending             = mNumBytesPending;
   }
};

template<class StubType>
inline void AtlasResourceTOC<StubType>::recalculateUpdates(Vector<AtlasReadNote*> &list, U32 maxItems)
{
   // If we've got nothing, skip it.
   if(!mLoadQueue.size())
      return;

   // Generate most recent priorities.
   for(S32 i=0; i<mLoadQueue.size(); i++)
   {
      // Don't requeue things that have chunks loaded or are Pending or higher.
      if(mLoadQueue[i]->mState > StubType::Pending || mLoadQueue[i]->mChunk)
      {
         mLoadQueue.erase_fast(i);

         // Go back and process this index again.
         i--;
         continue;
      }

      mLoadQueue[i]->mPriority = mLoadQueue[i]->mRequests.calculateCumulativePriority();
   }

   // qsort the items on the list, then stuff first maxItems onto the provided list.
   dQsort(mLoadQueue.address(), mLoadQueue.size(), sizeof(StubType*), updateStubSortFunc);

   // Now take first N and blast 'em up.
   U32 itemCount = getMin(mLoadQueue.size(), (S32)maxItems);
   list.reserve(list.size() + itemCount);
   for(S32 i=0; i < itemCount; i++)
   {
      list.increment();
      list.last() = new AtlasReadNote();
      list.last()->toc      = this;
      list.last()->stubIdx  = mLoadQueue[i] - mStubs;
   }
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::requestLoad(StubType *stub, U32 reason, F32 priority)
{
   AssertFatal(reason, "AtlasResourceTOC<StubType>::requestLoad - no reason supplied!");

   if(stub->mRequests.getRefCount() == 1)
   {
      // If it's 1 it was JUST added, so let's queue it up for loading.
      if(stub->mState == StubType::Unloaded)
      {
         mLoadQueue.push_back(stub);
         stub->mState = StubType::Pending;
         stub->mHeat += 0.1f;

         mNumBytesPending += stub->length;
      }
   }
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::cancelLoadRequest(StubType *stub, U32 reason)
{
   AssertFatal(reason, "AtlasResourceTOC<StubType>::cancelLoadRequest - no reason supplied!");

   if(stub->mRequests.getRefCount() == 0)
   {
      // Was just removed, so let's make sure it's not in the list.
      // This is O(n), which may be a bad idea.
      for(typename Vector<StubType *>::iterator i=mLoadQueue.begin(); i!=mLoadQueue.end(); i++)
      {
         if(*i == stub)
         {
            mLoadQueue.erase_fast(i);
            break;
         }
      }

      if( stub->mState == StubType::Pending )
         mNumBytesPending -= stub->length;

      // Also have to purge it. I think.
      stub->purge();
   }
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::cbPostBackToAtlasFile(U32 key, AtlasDeferredIO *adio)
{
   mFile->queuePendingDeserialize((AtlasReadNote*)key);
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::cbPopulateChunkReadNote(AtlasReadNote *arn)
{
   // Construct the ADIO for this chunk's read into this note.

   // Get the stub.
   const StubType &s = mStubs[arn->stubIdx];

   // Prep the read.
   arn->adio = new AtlasDeferredIO(AtlasDeferredIO::DeferredRead);
   arn->adio->setReadBuffer(s.offset, s.length);
   arn->adio->setCallback(this, &AtlasTOC::cbPostBackToAtlasFile, (U32)arn);
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::cbOnChunkReadStarted(U32 stubIdx)
{
   // It's in-flight so let's mark it as such and take it off the queues.
   StubType* stub = &mStubs[ stubIdx ];

   mNumStubsLoading ++;
   if( stub->mState == StubType::Pending )
      mNumBytesPending -= stub->length;
   mNumBytesLoading += stub->length;

   stub->mState = StubType::Loading;
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::cbOnChunkReadComplete(U32 stubIdx, AtlasChunk *chunk)
{
   // If there are no load requests we have to discard.
   StubType *stub = mStubs + stubIdx;

   mNumStubsLoading --;
   mNumBytesLoading -= stub->length;

   // Check to see if we're re-loading something that we already have in memory.
   // This should technically never happen but it's good to check. ;)
   if(stub->mChunk)
   {
      //FIXME: this occurs from time to time.  looks like a race condition in the current pipeline
   #ifdef TORQUE_DEBUG
      Con::warnf("AtlasResourceTOC<StubType>::cbOnChunkReadComplete - "
         "discarding already-loaded chunk (%d @ %d, %d)!",
         stub->level, stub->pos.x, stub->pos.y);
   #endif
      delete chunk;
      return;
   }

   // Discard, if the chunk is no longer requested.
   if(!stub->mRequests.getRefCount())
   {
   #ifdef TORQUE_DEBUG
      Con::warnf("AtlasResourceTOC<StubType>::cbOnChunkReadComplete - "
         "discarding cancelled chunk (%d @ %d, %d)!",
         stub->level, stub->pos.x, stub->pos.y);
   #endif
      mNumChunksDeadOnArrival ++;
      stub->mState = StubType::Unloaded; //FIXME: should not be necessary.  another race condition?
      delete chunk;
      return;
   }

   //Con::printf("AtlasResourceTOC<StubType>::cbOnChunkReadComplete - loaded stub (%d@%d,%d)",stub->level, stub->pos.x, stub->pos.y); 

   // BJGTODO - find way to defer process() call until needed.
   instateLoadedChunk(stub, (ChunkType *)chunk);
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::cbOnChunkWriteComplete(U32 key, AtlasDeferredIO *adio)
{
   // Updated the chunk, so let's queue up a stub update, too.
   AssertFatal(adio->offset, "AtlasResourceTOC<StubType>::cbOnChunkWriteComplete - got a zero offset!");
   AssertISV(key < mStubCount, "AtlasResourceTOC<StubType>::cbOnChunkWriteComplete - bad key; invalid stub!");

   // Key is the stub index. Update the stub.
   mStubs[key].offset = adio->offset;
   mStubs[key].length = adio->length;
   mStubs[key].generation++;

   // This queues up an update.
   queueStubUpdate(mStubs + key);
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::cbOnStubWriteComplete(U32 key, AtlasDeferredIO *adio)
{
   // Do anything?
   // Guess not - BJG
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::queueChunkUpdate(StubType *s)
{
   AssertISV(s->mChunk, "AtlasResourceTOC<StubType>::queueChunkUpdate - no chunk on stub!");
   AssertFatal(mFile, "AtlasResourceTOC<StubType>::queueChunkUpdate - not registered with a file!");

   // Note the previous chunk position, if any.
   s->mChunk->mPreviousChunk = s->offset;

   // Prepare an ADIO.
   AtlasDeferredIO *adio;
   adio = AtlasChunk::prepareDeferredWrite(s->mChunk);

   // Note callback so we update the stub, too.
   U32 key = s - mStubs;
   AssertISV(key < mStubCount, "AtlasResourceTOC<StubType>::queueChunkUpdate - invalid stub!");
   adio->setCallback(this, &AtlasTOC::cbOnChunkWriteComplete, key);

   // And queue.
   mFile->getDeferredFile().queue(adio);
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::immediateLoad(StubType *stub, U32 reason)
{
   AssertISV(stub->offset, "AtlasResourceTOC<T>::immediateLoad - got an empty stub!");

   stub->purge();

   ChunkType* chunk = (ChunkType*)constructNewChunk();

   Stream *s = mFile->getDeferredFile().lockStream();
   s->setPosition(stub->offset);
   AtlasChunk::readFromStream(chunk, s);
   mFile->getDeferredFile().unlockStream();

   instateLoadedChunk(stub, chunk);
}

template<class StubType>
inline bool AtlasResourceTOC<StubType>::write(Stream *s)
{
   s->write(mTreeDepth);
   s->write(mStubCount);
   s->write(csmVersion);

   for(S32 i=0; i<mStubCount; i++)
   {
      mStubs[i].stubOffset = s->getPosition();
      mStubs[i].write(s);
   }

   return true;
}

template<class StubType>
inline bool AtlasResourceTOC<StubType>::read(Stream *s)
{
   s->read(&mTreeDepth);
   AssertISV(mTreeDepth < 16,
      "AtlasResourceTOC<StubType>::read - got a tree depth greater than our base data types can support!");

   s->read(&mStubCount);
   s->read(&mVersion);

   // Initialize the stubs.
   mStubs = new StubType[mStubCount];

   // And read 'em.
   for(S32 i=0; i<mStubCount; i++)
   {
      mStubs[i].stubOffset = s->getPosition();
      mStubs[i].read(s, mVersion);
   }

   // Fill position level info in, or null.
   if(mIsQuadtree)
   {
      for(S32 i=0; i<mTreeDepth; i++)
      {
         for(S32 x=0; x<BIT(i); x++)
         {
            for(S32 y=0; y<BIT(i); y++)
            {
               StubType *s = getStub(i, Point2I(x,y));
               s->level = i;
               s->pos.set(x,y);
            }
         }
      }
   }
   else
   {
      // Fill with some debug data so we can see if we're doing something
      // we ought not with it when we're not a quadtree!
      for(S32 i=0; i<mStubCount; i++)
      {
         mStubs[i].level = -1;
         mStubs[i].pos.set(-1, -1);
      }
   }

   return true;
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::queueStubUpdate(StubType *t)
{
   AssertISV(t->stubOffset, 
      "AtlasResourceTOC<StubType>::updateStubOnDisk - no stub offset stored, but trying to update!");

   // Do this as a deferred operation.

   AtlasDeferredIO *adio = new AtlasDeferredIO(AtlasDeferredIO::DeferredWrite);
   adio->flags.set(AtlasDeferredIO::DeleteDataOnComplete);
   adio->flags.set(AtlasDeferredIO::DeleteStructOnComplete);
   adio->flags.set(AtlasDeferredIO::DataIsArray);

   // BJGTODO - get a real max for stub size on disk.
   U8 *data = new U8[512];
   MemStream ms(512, data);

   t->write(&ms);

   adio->offset = t->stubOffset;
   adio->length = ms.getPosition();
   adio->data = data;

   //Con::printf("queueing stub update for stub# %d", t - mStubs);
   mFile->getDeferredFile().queue(adio);
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::generate(RectI invalidationRegion)
{
   AssertFatal(mIsQuadtree, 
      "AtlasBaseToc<StubType>::generate - treating non-quadtree TOC as a quadtree!" );

   // We basically need to generate new chunks on tile affected by the
   // specified region.  We start at one less than the biggest level (since
   // we assume we have source data everywhere) and work our way up.

   for(S32 shift=mTreeDepth-2; shift>=0; shift--)
   {
      U32 sz = BIT(mTreeDepth - (shift + 1));

      for(S32 i=(S32)mFloor(F32(invalidationRegion.point.x) / F32(sz));
         i<(S32)mCeil(F32(invalidationRegion.extent.x+invalidationRegion.point.x)/F32(sz));
         i++)
      {
         for(S32 j=(S32)mFloor(F32(invalidationRegion.point.y) / F32(sz));
            j<(S32)mCeil(F32(invalidationRegion.extent.y+invalidationRegion.point.y)/F32(sz));
            j++)
         {

            // Ok, grab the children of this square and generate a new chunk.
            ChunkType *agc[4];
            dMemset(agc, 0, sizeof(ChunkType*) * 4);

            const U8 offsets[4][2] =
            {
               {0,0},
               {0,1},
               {1,0},
               {1,1},
            };

            // Print out a debug notification.
            Con::printf(" generating (%d @ %d, %d) from (%d @ %d, %d), (%d @ %d, %d), (%d @ %d, %d), (%d @ %d, %d)",
               shift, i,j,
               shift+1, i*2+offsets[0][0], j*2+offsets[0][1],
               shift+1, i*2+offsets[1][0], j*2+offsets[1][1],
               shift+1, i*2+offsets[2][0], j*2+offsets[2][1],
               shift+1, i*2+offsets[3][0], j*2+offsets[3][1]);

            // Load all the chunk so we can do our processing.
            for(S32 sq=0; sq<4; sq++)
            {
               StubType *args = getStub(shift+1, Point2I(i*2+offsets[sq][0], j*2+offsets[sq][1]));
               immediateLoad(args, AtlasTOC::EditorLoad);
               agc[sq] = args->mChunk;
            }

            // Generate new chunk...
            ChunkType *newagc = (ChunkType *)constructNewChunk();
            newagc->generate((AtlasChunk**)agc);

            // Store it.
            StubType *thisgs = getStub(shift, Point2I(i,j));
            instateNewChunk(thisgs, newagc, true);

            // Wipe to save on memory.
            thisgs->purge();

            // Wipe children, too.
            for(S32 sq=0; sq<4; sq++)
            {
               StubType *args = getStub(shift+1, Point2I(i*2+offsets[sq][0], j*2+offsets[sq][1]));
               args->purge();
            }

         }
      }
   }
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::instateLoadedChunk(StubType *stub, ChunkType *chunk)
{
   AssertFatal(chunk, "AtlasResourceTOC<StubType>::instateLoadedChunk - chunk not present!");
   AssertFatal(stub, "AtlasResourceTOC<StubType>::instateLoadedChunk - stub not present!");

   // Purge old data if any.
   purgeStub(stub);

   // Instate new data.
   chunk->process();
   stub->mState = StubType::Loaded;
   stub->mHeat += 1.0f;
   stub->mChunk = (ChunkType*)chunk;
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::instateNewChunk(StubType *stub, ChunkType *chunk, bool blockTillSerialized)
{
   const U32 gen = stub->generation;

   // Instate new pointer...
   instateLoadedChunk(stub, chunk);

   // And queue the write.
   queueChunkUpdate(stub);

   if(blockTillSerialized)
   {
      // Make sure it's serialized, too. The gen will change, so just
      // spin. (Famous last words.) Note that we cast to add a volatile
      // so that the compiler doesn't do anything dumb - we want to re-read
      // the generation every time through this loop.
      volatile U32 &currentGen = stub->generation;
      while(gen == currentGen)
         Platform::sleep(1);
   }
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::purgeStub(StubType *stub)
{
   stub->purge();
}

template<class StubType>
inline void AtlasResourceTOC<StubType>::dumpStubLoadStatus(U32 stubIdx)
{
   AssertFatal(stubIdx < mStubCount, "AtlasResourceTOC<StubType>::dumpStubLoadStatus - invalid stub index.");
   StubType *stub = mStubs + stubIdx;

   Con::printf("  %s - priority %f, pos = %d@(%d, %d), refCount=%d",
      AtlasClassFactory::getTOCName(this),
      stub->mRequests.calculateCumulativePriority(),
      stub->level, stub->pos.x, stub->pos.y,
      stub->mRequests.getRefCount());
}

#endif