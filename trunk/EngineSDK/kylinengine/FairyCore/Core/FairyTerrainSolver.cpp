#include "FairyTerrainSolver.h"
#include "FairyResourceSolverHelper.h"
#include "FairyResources.h"
#include "Terrain.h"

#include <OgreResourceGroupManager.h>

namespace Fairy {

    //////////////////////////////////////////////////////////////////////////

    class TerrainSolver : public ResourceSolverBase
    {
    public:
        bool modified;

        TerrainSolver(void)
            : ResourceSolverBase()
            , modified()
        {
        }

        const Ogre::String& getName(void) const
        {
            static const Ogre::String name = "Terrain";
            return name;
        }

        bool commitPermanent(void)
        {
            bool ret = modified;
            modified = false;
            return ret;
        }
    };

    //////////////////////////////////////////////////////////////////////////

    class UnsolvedTerrainBrush : public UnsolvedResourceSimple
    {
    public:
        TerrainSolver* solver;

        UnsolvedTerrainBrush(TerrainSolver* solver)
            : UnsolvedResourceSimple()
            , solver(solver)
        {
            assert(solver);
        }

        bool setResolvedValue(const Ogre::String& value)
        {
            if (!UnsolvedResourceSimple::setResolvedValue(value))
                return false;

            solver->modified = true;
            return true;
        }
    };

    //////////////////////////////////////////////////////////////////////////

    ResourceSolverPtr findUnsolvedTerrainBrushes(Terrain* terrain)
    {
       Ogre::String groupName = BRUSH_RESOURCE_GROUP_NAME;
        Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();

        TerrainSolver* solver = new TerrainSolver;
        ResourceSolverPtr holder(solver);

       Ogre::StringVectorPtr possibles;

        TerrainData* data = terrain->getData();
        for (TerrainData::TextureArray::const_iterator it = data->mTextures.begin(); it != data->mTextures.end(); ++it)
        {
            const Ogre::String& textureName = *it;
            if (!rgm.resourceExists(groupName, textureName))
            {
                // Create new unsolved resource
                UnsolvedTerrainBrush* item = new UnsolvedTerrainBrush(solver);
                solver->unsolvedResources.push_back(UnsolvedResourcePtr(item));

                // Setup origin name
                item->origin = textureName;

                // Setup description
                Ogre::StringUtil::StrStreamType str;
                str << "Reference by terrain '" << data->mName << "'";
                item->description = str.str();

                // Setup recommend values
               Ogre::String textureBasename, texturePath;
                Ogre::StringUtil::splitFilename(textureName, textureBasename, texturePath);
                item->recommends = rgm.findResourceNames(groupName, textureBasename);

                // Setup possible values
                if (possibles.isNull())
                {
                    possibles = findImageResources(groupName);
                }
                item->possibles = possibles;

                // HACK
                item->references.push_back(const_cast<String*>(&textureName));
            }
        }

        return holder;
    }

}
