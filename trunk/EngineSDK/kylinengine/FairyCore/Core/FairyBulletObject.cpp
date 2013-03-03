/********************************************************************
filename:   FairyEffectObject.cpp

purpose:    the object that wrap the effect,so we can put the effect in the scene
*********************************************************************/

#include "FairyBulletObject.h"
#include "FairyObjectFactory.h"
#include "EffectSystem/FairyBulletSystemManager.h"
#include "EffectSystem/FairyBulletSystem.h"
#include "EffectSystem/FairyEffectBasic.h"
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreSceneNode.h>

namespace Fairy {

    const Ogre::String BulletObject::msType = "Bullet";
    const Ogre::String BulletObject::msCategory = "StaticObject";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class BulletObject::Factory : public ObjectFactory
    {
    public:
        const Ogre::String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new BulletObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct BulletSystemNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const BulletObject* object = static_cast<const BulletObject*>(source);
                return object->getBulletSystemName();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const BulletObject* object = static_cast<const BulletObject*>(source);
                return object->getBulletSystemName();
            }

            void doSet(void* target, const Variant& value)
            {
                BulletObject* object = static_cast<BulletObject*>(target);
               Ogre::String BulletSystemName = VariantCast<String>(value);
                object->setBulletSystemName(BulletSystemName);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                BulletObject* object = static_cast<BulletObject*>(target);
                object->setBulletSystemName(value);
            }
        };

    }


    //////////////////////////////////////////////////////////////////////////
    // BulletObject class
    //////////////////////////////////////////////////////////////////////////
    BulletObject::BulletObject(void)
        : PlacementObject(),
        mBulletSystem(NULL)
    {
        if (createPropertyDictionary("BulletObject"))
        {
            static BulletSystemNameCmd bulletSystemNameCmd;
            defineProperty(
                "BulletSystemName",
                "The bullet system template name use to create this bullet object.",
                "bullet template name",
                &bulletSystemNameCmd,
                PF_PRIMARY);

            addBaseProperty();
        }
    }
    //-----------------------------------------------------------------------
    BulletObject::~BulletObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const Ogre::String& BulletObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const Ogre::String& BulletObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void BulletObject::createRenderInstance(System* system)
    {
        assert(system);

        PlacementObject::createRenderInstance(system);

        if (!mBulletSystemName.empty())
        {
            createBulletSystem();
        }
    }
    //-----------------------------------------------------------------------
    void BulletObject::destroyRenderInstance(void)
    {
        if (mBulletSystem)
        {
            destroyBulletSystem();
        }
       
        PlacementObject::destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void BulletObject::createBulletSystem(void)
    {
        assert( mBulletSystem == NULL );
        assert(!mBulletSystemName.empty());

        Ogre::SceneNode* parent = getSceneNode();

		if (parent)
		{
			mBulletSystem = BulletSystemManager::getSingleton().createBulletSystem( mBulletSystemName );
			assert(mBulletSystem);

			mBulletSystem->setPosition(Ogre::Vector3::ZERO);

			// the scene node of placement object will be the parent scene node of
			// effect object, so the position of the scene node will be the position of
			// the effect.
			mBulletSystem->createSceneNode(parent);	
		}
    }
    //-----------------------------------------------------------------------
    void BulletObject::destroyBulletSystem(void)
    {
        assert(mBulletSystem);

        BulletSystemManager::getSingleton().destroyBulletSystem(mBulletSystem);

        mBulletSystem = NULL;
    }
    //-----------------------------------------------------------------------
    void BulletObject::setBulletSystemName( const Ogre::String &name )
    {
        if (mBulletSystemName != name)
        {
            if (mBulletSystem)
            {
                destroyBulletSystem();
            }

            mBulletSystemName = name;

            if (!mBulletSystemName.empty())
            {
                createBulletSystem();
            }
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* BulletObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }
}
