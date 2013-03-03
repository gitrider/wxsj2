#include "TerrainTileRenderable.h"
#include "TerrainTile.h"

namespace Fairy {

TerrainTileRenderable::TerrainTileRenderable(TerrainTile *parent)
    : BasicRenderable(parent)
{
}
//-----------------------------------------------------------------------
TerrainTileRenderable::~TerrainTileRenderable()
{
}
//-----------------------------------------------------------------------
const Ogre::LightList&
TerrainTileRenderable::getLights(void) const
{
    return static_cast<TerrainTile*>(mParent)->getLights();
}

}
