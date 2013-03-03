#ifndef __FAIRYRESOURCES_H__
#define __FAIRYRESOURCES_H__

#include "FairyPrerequisites.h"

#include <OgreResource.h>

namespace Fairy {

extern const Ogre::String GRAMMAR_RESOURCE_GROUP_NAME;
extern const Ogre::String EDITOR_RESOURCE_GROUP_NAME;

extern const Ogre::String SCENE_SCHEMA_NAME;
extern const Ogre::String TERRAIN_SCHEMA_NAME;

extern const Ogre::String DEFAULT_RESOURCE_GROUP_NAME;
extern const Ogre::String BRUSH_RESOURCE_GROUP_NAME;

extern const Ogre::String AUTO_ANIMATION_NAME;

extern const Ogre::String BOOTSTRAP_RESOURCE_GROUP_NAME;

class FakeManualResourceLoader : public Ogre::ManualResourceLoader
{
public:
    void loadResource(Ogre::Resource* resource)
    {
        // do nothing
    }
};

extern FakeManualResourceLoader gsFakeManualResourceLoader;

extern bool
splitResourceName(const Ogre::String& name,Ogre::String& resourceName,Ogre::String& groupName);

extern bool
findCorrelativeResource(String& resourceName,Ogre::String& groupName,
                        const Ogre::String& baseResourceName, const Ogre::String& baseGroupName);

extern Ogre::ResourcePtr
loadCorrelativeResource(const Ogre::String& resourceName, const Ogre::String& groupName,
                        const Ogre::String& baseResourceName, const Ogre::String& baseGroupName,
                        Ogre::ResourceManager& resourceManager);

extern Ogre::MeshPtr
loadMesh(const Ogre::String& meshName, const Ogre::String& groupName,
                    const Ogre::String& baseResourceName, const Ogre::String& baseGroupName);

}

#endif // 
