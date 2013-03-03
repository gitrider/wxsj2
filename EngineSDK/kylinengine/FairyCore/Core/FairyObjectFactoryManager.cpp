#include "FairyObjectFactoryManager.h"

template<> Fairy::ObjectFactoryManager* Ogre::Singleton<Fairy::ObjectFactoryManager>::ms_Singleton = 0;

namespace Fairy
{
    //-----------------------------------------------------------------------
    ObjectFactoryManager* ObjectFactoryManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    ObjectFactoryManager& ObjectFactoryManager::getSingleton(void)
    {  
        assert(ms_Singleton);  return (*ms_Singleton);
    }
    //-----------------------------------------------------------------------
    ObjectFactoryManager::ObjectFactoryManager()
    {
    }
    //-----------------------------------------------------------------------
    ObjectFactoryManager::~ObjectFactoryManager()
    {
        PropertyInterface::cleanupDictionary();
    }
    //-----------------------------------------------------------------------
    ObjectPtr ObjectFactoryManager::createInstance(const Ogre::String& type) const
    {
        Factory<Object>* factory = findFactory(type);
        Object* object = factory->createInstance();
        assert(object->getType() == type);
        return ObjectPtr(object, Factory<Object>::Deleter(factory));
    }
}
