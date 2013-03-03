#include "FairyResources.h"

#include <OgreException.h>
#include <OgreResourceGroupManager.h>
#include <OgreResourceManager.h>
#include <OgreMesh.h>
#include <OgreSkeleton.h>
#include <OgreMeshManager.h>
#include <OgreSkeletonManager.h>

namespace Fairy {

extern const Ogre::String GRAMMAR_RESOURCE_GROUP_NAME = "Grammar";
extern const Ogre::String EDITOR_RESOURCE_GROUP_NAME = "Fairy Editor";

extern const Ogre::String SCENE_SCHEMA_NAME = "FairyScene.xsd";
extern const Ogre::String TERRAIN_SCHEMA_NAME = "FairyTerrain.xsd";

extern const Ogre::String DEFAULT_RESOURCE_GROUP_NAME = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
extern const Ogre::String BRUSH_RESOURCE_GROUP_NAME = "General";

extern const Ogre::String AUTO_ANIMATION_NAME = "[auto]";

extern const Ogre::String BOOTSTRAP_RESOURCE_GROUP_NAME = "Bootstrap";

FakeManualResourceLoader gsFakeManualResourceLoader;

bool
splitResourceName(const Ogre::String& name,Ogre::String& resourceName,Ogre::String& groupName)
{
   Ogre::String::size_type pos = name.find_first_of(':');
    if (pos ==Ogre::String::npos)
    {
        if (groupName.empty())
            groupName = DEFAULT_RESOURCE_GROUP_NAME;
        resourceName = name;
        return false;
    }
    else
    {
        groupName = name.substr(0, pos);
        resourceName = name.substr(pos+1,Ogre::String::npos);
        return true;
    }
}

bool
findCorrelativeResource(String& resourceName,Ogre::String& groupName,
                        const Ogre::String& baseResourceName, const Ogre::String& baseGroupName)
{
    Ogre::ResourceGroupManager& mgr = Ogre::ResourceGroupManager::getSingleton();

   Ogre::String name, path;
    Ogre::StringUtil::splitFilename(resourceName, name, path);
    bool existsPath = !path.empty();
	String grp = baseGroupName;

    // First, find in correlatived group and path if resource name doesn't exists path
    if (!existsPath)
    {
        Ogre::StringUtil::splitFilename(baseResourceName, name, path);
        if (!path.empty())
        {
            name = path + resourceName;
            if (mgr.resourceExists(grp, name))
            {
                resourceName = name;
                groupName = baseGroupName;
                return true;
            }
        }
    }

    // Second, find in correlatived group
    if (mgr.resourceExists(grp, resourceName))
    {
        groupName = baseGroupName;
        return true;
    }

    // Three, find in user given group
    if (!groupName.empty())
    {
        if (mgr.resourceExists(groupName, resourceName))
        {
            return true;
        }
    }

    // Four, find in global default group
    if (groupName != DEFAULT_RESOURCE_GROUP_NAME)
    {
		Ogre::String grp = DEFAULT_RESOURCE_GROUP_NAME;
        if (mgr.resourceExists(grp, resourceName))
        {
            groupName = grp;
            return true;
        }
    }

    return false;
}

Ogre::ResourcePtr
tryLoadResource(Ogre::ResourceManager& resourceManager,
                 const Ogre::String& resourceName, const Ogre::String& groupName)
{
    try
    {
        return resourceManager.load(resourceName, groupName,false,NULL,NULL);
    }
    catch (...)
    {
        return Ogre::ResourcePtr();
    }
}

Ogre::ResourcePtr
loadCorrelativeResource(const Ogre::String& resourceName, const Ogre::String& groupName,
                        const Ogre::String& baseResourceName, const Ogre::String& baseGroupName,
                        Ogre::ResourceManager& resourceManager)
{
    Ogre::ResourcePtr res;

   Ogre::String name, path;
    Ogre::StringUtil::splitFilename(resourceName, name, path);
    bool existsPath = !path.empty();

    // First, load in correlatived group and path if resource name doesn't exists path
    if (!existsPath)
    {
        Ogre::StringUtil::splitFilename(baseResourceName, name, path);
        if (!path.empty())
        {
            name = path + resourceName;
            res = tryLoadResource(resourceManager, name, baseGroupName);
            if (!res.isNull())
                return res;
        }
    }

    // Second, load in correlatived group
    res = tryLoadResource(resourceManager, resourceName, baseGroupName);
    if (!res.isNull())
        return res;

    // Three, load in user given group
    if (!groupName.empty())
    {
        res = tryLoadResource(resourceManager, resourceName, groupName);
        if (!res.isNull())
            return res;
    }

    // Four, load in global default group
    if (groupName != DEFAULT_RESOURCE_GROUP_NAME)
    {
        res = tryLoadResource(resourceManager, resourceName, groupName);
        if (!res.isNull())
            return res;
    }

    return res;
}

Ogre::MeshPtr
loadMesh(const Ogre::String& meshName, const Ogre::String& groupName,
         const Ogre::String& baseResourceName, const Ogre::String& baseGroupName)
{
    // Load the mesh
    Ogre::MeshPtr mesh = loadCorrelativeResource(
        meshName, groupName,
        baseResourceName, baseGroupName,
        Ogre::MeshManager::getSingleton());

    if (mesh.isNull())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Unable to load mesh " + meshName,
            "loadMesh");
    }

    // Try to resolve skeleton resource
    if (mesh->hasSkeleton() && mesh->getSkeleton().isNull())
    {
        // resolve correlative with mesh
        Ogre::SkeletonPtr skeleton = loadCorrelativeResource(
            mesh->getSkeletonName(), groupName,
            mesh->getName(), mesh->getGroup(),
            Ogre::SkeletonManager::getSingleton());

        if (skeleton.isNull())
        {
            // resolve correlative with base resource
            skeleton = loadCorrelativeResource(
                mesh->getSkeletonName(), groupName,
                baseResourceName, baseGroupName,
                Ogre::SkeletonManager::getSingleton());
        }

        if (skeleton.isNull())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Unable to load skeleton " + mesh->getSkeletonName() +
                " for mesh " + mesh->getName(),
                "loadMesh");
        }

        // Set to the actual name
        mesh->setSkeletonName(skeleton->getName());
    }

    return mesh;
}

}
