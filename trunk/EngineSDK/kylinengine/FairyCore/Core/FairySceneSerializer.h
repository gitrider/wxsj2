#ifndef __FAIRYSCENESERIALIZER_H__
#define __FAIRYSCENESERIALIZER_H__

#include "FairyPrerequisites.h"

namespace Fairy
{
    class Scene;
    class XMLParser;
    class XMLWriter;

    class SceneSerializer
    {
    public:
        SceneSerializer(void);
        virtual ~SceneSerializer();

        virtual void load(Scene* scene, XMLParser* parser, const Ogre::String& filename, const Ogre::String& groupName);
        virtual void save(Scene* scene, XMLWriter* writer, const Ogre::String& filename);
    };
}

#endif // 
