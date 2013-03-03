#ifndef __FAIRYACTOR_H__
#define __FAIRYACTOR_H__

#include "FairyPrerequisites.h"
#include "OgreExt/OgreUserDefinedObject.h"
// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

namespace Fairy {

	class System;
	class LogicModel;

    class Actor
    {
    protected:
        typedef LogicModel Impl;

        Impl* mImpl;

    public:
    //    Actor(Ogre::SceneManager* creator, const Ogre::String& name, bool editable);
		Actor( System *system, const Ogre::String& name, const Ogre::String& modelName, Ogre::SceneNode* node );
        virtual ~Actor();

        Impl* getImpl(void) const
        {
            return mImpl;
        }

        Ogre::SceneNode* getSceneNode(void) const;

        Ogre::AxisAlignedBox getBoundingBox(void) const;

        void setUserObject(Ogre::UserDefinedObject* userObject);
        Ogre::UserDefinedObject* getUserObject(void) const;
    };

}

#endif 
