/********************************************************************
filename:   FairyEffectObject.h

purpose:    the object that wrap the effect,so we can put the effect in the scene
*********************************************************************/

#ifndef __BULLETOBJECT_H__
#define __BULLETOBJECT_H__

#include "FairyPlacementObject.h"

namespace Fairy {

    class ObjectFactory;
	class BulletSystem;

    class BulletObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        BulletObject(void);
        virtual ~BulletObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        void setBulletSystemName( const Ogre::String &name );

        const Ogre::String &getBulletSystemName(void) const
        {
            return mBulletSystemName;
        }

        //////////////////////////////////////////////////////////////////////////
        BulletSystem * getBulletSystem(void) const
        {
            return mBulletSystem;
        }

    protected:

		/// effect template name
       Ogre::String mBulletSystemName;
       BulletSystem *mBulletSystem;

    protected:

        void createBulletSystem(void);
        void destroyBulletSystem(void);
    };
}

#endif // __BULLETOBJECT_H__
