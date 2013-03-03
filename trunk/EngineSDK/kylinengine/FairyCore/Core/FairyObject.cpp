#include "FairyObject.h"

namespace Fairy
{
    Object::Object(void)
        : mName()
		, mData(0)
    {
    }
    //-----------------------------------------------------------------------
    Object::~Object()
    {
    }
    //-----------------------------------------------------------------------
    void Object::setName(const Ogre::String& name)
    {
        mName = name;
    }
    //-----------------------------------------------------------------------
    void Object::setData(Ogre::ulong data)
    {
        mData = data;
    }
    //-----------------------------------------------------------------------
    void Object::queryReferenceResources(ResourceCollection* resourceCollection)
    {
    }
    //-----------------------------------------------------------------------
    void Object::queryBakableEntities(EntityList& entities)
    {
    }
    //-----------------------------------------------------------------------
    void Object::bakeStaticGeometry(Ogre::StaticGeometry* staticGeometry, const EntityList& entity)
    {
    }
    //-----------------------------------------------------------------------
    void Object::processMessage(const Ogre::String& msg, const Variant& params)
    {
    }
}
