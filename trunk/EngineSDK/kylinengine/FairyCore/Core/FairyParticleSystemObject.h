#ifndef __FAIRYPARTICLESYSTEMOBJECT_H__
#define __FAIRYPARTICLESYSTEMOBJECT_H__

#include "FairyPlacementObject.h"

namespace Fairy {

    class ObjectFactory;

    class ParticleSystemObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        ParticleSystemObject(void);
        virtual ~ParticleSystemObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        void setParticleSystemName( const Ogre::String &name );

        const Ogre::String &getParticleSystemName(void) const
        {
            return mParticleSystemName;
        }

        //////////////////////////////////////////////////////////////////////////
        Ogre::ParticleSystem * getParticleSystem(void)
        {
            return mParticleSystem;
        }

		void setParticleSystem(Ogre::ParticleSystem *system)
		{
			assert (system);
			mParticleSystem = system;
		}

    protected:
       Ogre::String mParticleSystemName;

        Ogre::ParticleSystem *mParticleSystem;

    protected:

        void createParticleSystem(void);
        void destroyParticleSystem(void);
    };
}

#endif // 
