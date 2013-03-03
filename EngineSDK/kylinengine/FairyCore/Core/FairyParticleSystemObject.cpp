#include "FairyParticleSystemObject.h"
#include "FairyObjectFactory.h"

#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace Fairy {

    const Ogre::String ParticleSystemObject::msType = "ParticleSystem";
    const Ogre::String ParticleSystemObject::msCategory = "StaticObject";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class ParticleSystemObject::Factory : public ObjectFactory
    {
    public:
        const Ogre::String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new ParticleSystemObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct ParticleSysNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const ParticleSystemObject* object = static_cast<const ParticleSystemObject*>(source);
                return object->getParticleSystemName();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const ParticleSystemObject* object = static_cast<const ParticleSystemObject*>(source);
                return object->getParticleSystemName();
            }

            void doSet(void* target, const Variant& value)
            {
                ParticleSystemObject* object = static_cast<ParticleSystemObject*>(target);
               Ogre::String particleSysName = VariantCast<String>(value);
                object->setParticleSystemName(particleSysName);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                ParticleSystemObject* object = static_cast<ParticleSystemObject*>(target);
                object->setParticleSystemName(value);
            }
        };

    }


    //////////////////////////////////////////////////////////////////////////
    // ParticleSystemObject class
    //////////////////////////////////////////////////////////////////////////
    ParticleSystemObject::ParticleSystemObject(void)
        : PlacementObject(),
        mParticleSystem(NULL)
    {
        if (createPropertyDictionary("ParticleSystem"))
        {
            static ParticleSysNameCmd particleSysNameCmd;
            defineProperty(
                "particle system name",
                "The particle system template name use to create this particle system.",
                "ParticleSystemName",
                &particleSysNameCmd,
                PF_PRIMARY);

            addBaseProperty();
        }
    }
    //-----------------------------------------------------------------------
    ParticleSystemObject::~ParticleSystemObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const Ogre::String& ParticleSystemObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const Ogre::String& ParticleSystemObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void ParticleSystemObject::createRenderInstance(System* system)
    {
        assert(system);

        PlacementObject::createRenderInstance(system);

        if (!mParticleSystemName.empty())
        {
            createParticleSystem();
        }
    }
    //-----------------------------------------------------------------------
    void ParticleSystemObject::destroyRenderInstance(void)
    {
        if (mParticleSystem)
        {
            destroyParticleSystem();
        }
       
        PlacementObject::destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void ParticleSystemObject::createParticleSystem(void)
    {
        assert( mParticleSystem == NULL );
        assert(!mParticleSystemName.empty());

        Ogre::SceneNode* parent = getSceneNode();

        if (parent)
        {
            mParticleSystem = parent->getCreator()->createParticleSystem( parent->getName(), mParticleSystemName );
            assert(mParticleSystem);

            // Set render queue for particle system, the particle system should be render after world geometry 2
            mParticleSystem->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);

            parent->attachObject(mParticleSystem);
        }
    }
    //-----------------------------------------------------------------------
    void ParticleSystemObject::destroyParticleSystem(void)
    {
        assert(mParticleSystem);
		mSceneNode->getCreator()->destroyParticleSystem(mParticleSystem);
        mParticleSystem = NULL;
    }
    //-----------------------------------------------------------------------
    void ParticleSystemObject::setParticleSystemName( const Ogre::String &name )
    {
        if (mParticleSystemName != name)
        {
            if (mParticleSystem)
            {
                destroyParticleSystem();
            }

            mParticleSystemName = name;

            if (!mParticleSystemName.empty())
            {
                createParticleSystem();
            }
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* ParticleSystemObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }
}
