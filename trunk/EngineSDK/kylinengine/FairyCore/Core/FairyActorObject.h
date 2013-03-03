#ifndef __FAIRYACTOROBJECT_H__
#define __FAIRYACTOROBJECT_H__

#include "FairyPlacementObject.h"

namespace Fairy
{
    class Actor;
    class ObjectFactory;
    class ObjectProxy;
	class System;

    class ActorObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        ActorObject(void);
        virtual ~ActorObject();

        const Ogre::String& getType(void) const;
        const Ogre::String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        Variant getProperty(const Ogre::String& name) const;
       Ogre::String getPropertyAsString(const Ogre::String& name) const;

        //////////////////////////////////////////////////////////////////////////

        static const Ogre::String msType;
        static const Ogre::String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Actor* getActor(void) const
        {
            return mActor;
        }

        const Ogre::String& getActorName(void) const
        {
            return mActorName;
        }

        void setActorName(const Ogre::String& actorName);

    protected:
        ObjectProxy* mProxy;
        Actor* mActor;
        Ogre::String mActorName;
		System *mSystem;

        void createActor(void);
        void destroyActor(void);
    };

}

#endif 
