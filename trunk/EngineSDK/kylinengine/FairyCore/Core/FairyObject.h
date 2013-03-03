#ifndef __FAIRYOBJECT_H__
#define __FAIRYOBJECT_H__

#include "FairyPropertyInterface.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace Fairy
{
    class System;
    class ResourceCollection;
    struct ObjectSetIndices;

    typedef std::list<Ogre::Entity*> EntityList;

    class Object : public PropertyInterface, public boost::enable_shared_from_this<Object>
    {
        // Allow ObjectSetIndices access mName to simply multi-index container adaptor
        friend struct ObjectSetIndices;

    protected:
       Ogre::String mName;
		Ogre::ulong  mData;

    public:
        Object(void);
        virtual ~Object();

        const Ogre::String& getName(void) const
        {
            return mName;
        }

        void setName(const Ogre::String& name);

		Ogre::ulong getData(void) const
        {
            return mData;
        }

        void setData(Ogre::ulong data);

        virtual const Ogre::String& getType(void) const = 0;
        virtual const Ogre::String& getCategory(void) const = 0;

        virtual void queryReferenceResources(ResourceCollection* resourceCollection);

        virtual void queryBakableEntities(EntityList& entities);
        virtual void bakeStaticGeometry(Ogre::StaticGeometry* staticGeometry, const EntityList& entities);

        virtual void createRenderInstance(System* system) = 0;
        virtual void destroyRenderInstance(void) = 0;

        virtual void processMessage(const Ogre::String& msg, const Variant& params);
    };

    typedef boost::shared_ptr<Object> ObjectPtr;
    typedef boost::weak_ptr<Object> ObjectWeakPtr;

}

#endif 
