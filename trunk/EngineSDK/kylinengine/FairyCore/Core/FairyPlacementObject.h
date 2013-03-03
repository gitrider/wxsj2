#ifndef __FAIRYPLACEMENTOBJECT_H__
#define __FAIRYPLACEMENTOBJECT_H__

#include "FairyObject.h"
#include "FairyTransform.h"

namespace Fairy
{
    class System;

    class PlacementObject : public Object
    {
    public:
        PlacementObject(void);
        virtual ~PlacementObject();

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        Variant getProperty(const Ogre::String& name) const;
       Ogre::String getPropertyAsString(const Ogre::String& name) const;

        void processMessage(const Ogre::String& msg, const Variant& params);

        //////////////////////////////////////////////////////////////////////////

        Ogre::SceneNode* getSceneNode(void) const
        {
            return mSceneNode;
        }

        Ogre::Real getCreateLevel(void) const
        {
            return mCreateLevel;
        }

        void setCreateLevel(Ogre::Real level);

    protected:
        void addBaseProperty(void);

        void _updateCreateLevel(void);

        Ogre::SceneNode* mSceneNode;

        Ogre::Real mCreateLevel;

        System* mSystem;

    public:     // Intend for direct access by property commands only, DO NOT access outside
        Transform mTransform;
    };

}

#endif 
