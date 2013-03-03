#ifndef __TerrainTileOptimized_H__
#define __TerrainTileOptimized_H__

#include "TerrainTile.h"

namespace Fairy {

class TerrainData;

//////////////////////////////////////////////////////////////////////////

class TerrainTileOptimized : public TerrainTile
{
protected:
    /// renderable list
    RenderableList mRenderables;

    bool mGeometryOutOfDate;

	void createGeometry(TerrainData* data, int xbase, int zbase, int xsize, int zsize);
	void destoryGeometry(void);

public:
    TerrainTileOptimized(Ogre::SceneNode* parent, Terrain *creator,
					  int xbase, int zbase, int xsize, int zsize);
    ~TerrainTileOptimized();

public:
    /** Returns the type of the movable. */
    const Ogre::String& getMovableType(void) const;

    /** Internal method add Renderable subclass instances to the rendering queue. */
    void _updateRenderQueue(Ogre::RenderQueue* queue);

	virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, 
		bool debugRenderables = false);
};

}

#endif // 
