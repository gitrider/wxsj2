//-----------------------------------------------------------------------------
// Torque Game Engine Advanced
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ATLAS_RUNTIME_ATLASCLIPMAPBATCHER_H_
#define _ATLAS_RUNTIME_ATLASCLIPMAPBATCHER_H_

#include "clipmap/clipMap.h"
#include "atlas/resource/atlasGeomChunk.h"
#include "core/dataChunker.h"

class AtlasResourceGeomStub;
class AtlasRenderPlugin;
class SceneState;
struct SceneGraphData;
class SceneObject;
class AtlasRenderPlugin;

/// Manage optimal rendering of a set of AtlasGeomChunks with a clipmap
/// applied. Also provides functionality related to detail maps and lighting.
class AtlasClipMapBatcher
{
public:
   struct RenderNote
   {
      U8 levelStart, levelEnd, levelCount, pad;
      F32 nearDist;
      AtlasGeomChunk *chunk;
   };
private:
   /// What clipmap are we going to be rendering from?
   ClipMap *mClipMap;
   SceneState *mState;

   bool mFixedFunction;

   BaseMatInstance* mDetailMat;
   BaseMatInstance* mFogMat;

   MaterialParameterHandle* mBrightnessScaleSC;
   MaterialParameterHandle* mDetDataSC;
   
   FreeListChunker<RenderNote> mRenderNoteAlloc;

   /// List of chunks to render for each shader.
   Vector<RenderNote*> mRenderList[4];

   /// RenderNotes to be rendered with detail maps.
   ///
   /// Offsets to entries in mRenderList
   Vector<RenderNote*> mDetailList;

   /// RenderNotes to be rendered with fog.
   Vector<RenderNote*> mFogList;

   /// RenderNotes to be rendered with lighting.
   Vector<RenderNote*> mLightList;
  
   AtlasInstance *mObject;

   bool mIsDetailMappingEnabled;
   
   static Vector<AtlasRenderPlugin*> mRenderPlugins;

   /// RenderNote comparison function; sorts by distance (closest first).
   static S32 cmpRenderNote(const void *a, const void *b);

   void renderClipMap( SceneGraphData& sgData, BaseMatInstance* overrideMat );
   void renderDetail( SceneGraphData& sgData );
   void renderFog( SceneGraphData& sgData );

public:

   AtlasClipMapBatcher();
   ~AtlasClipMapBatcher();

   void initMaterials();

   void init(ClipMap *acm, SceneState *state, AtlasInstance *renderer);
   void queue(const Point3F &camPos, AtlasResourceGeomStub *args);
   void sort();
   void render( BaseMatInstance* overrideMat );

   bool isDetailMappingEnabled()
   {
	   return mIsDetailMappingEnabled;
   }
   void isDetailMappingEnabled( bool value )
   {
	   mIsDetailMappingEnabled = value;
   }


   F32 mDetailMapFadeEndDistance;
   F32 mDetailMapFadeStartDistance;
   F32 mDetailMapTextureMultiply;

   static void registerRenderPlugin(AtlasRenderPlugin* arp);
   static void unregisterRenderPlugin(AtlasRenderPlugin* arp);
};

// Allow extra passes over atlas geometry
class AtlasRenderPlugin
{
public:
   virtual void init() = 0;   
   virtual void queue(AtlasClipMapBatcher::RenderNote* rn, SceneObject* mObject, AtlasResourceGeomStub *args) = 0;
   virtual void render(SceneState* state, SceneGraphData& sgData, SceneObject* obj) = 0;
};

#endif