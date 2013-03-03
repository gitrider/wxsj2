#ifndef __FAIRYACTOR_H__
#define __FAIRYACTOR_H__

#include "FairyPrerequisites.h"
#include <OgreAxisAlignedBox.h>
#include <OgreSharedPtr.h>
#include "materialeffectbase.h"
#include "OgreExt/OgreUserDefinedObject.h"

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

namespace Fairy {

    class ModelSerializer;

    class Model
    {
        friend ModelSerializer;
    protected:
        typedef std::list<Ogre::SceneNode*> SceneNodeList;
        typedef std::list<Ogre::MovableObject*> ObjectList;
        typedef std::list<Ogre::AnimationState*> AnimationStateList;

        Ogre::SceneManager* mCreator;
        Ogre::SceneNode* mParent;
        Ogre::UserDefinedObject* mUserObject;
        SceneNodeList mSceneNodes;
        ObjectList mMasterObjects;
        ObjectList mMovables;
        AnimationStateList mAnimationStates;
        Ogre::AxisAlignedBox mBoundingBox;
        bool mCastShadows;
        AnimationType mAnimationType;
        Ogre::Controller<Ogre::Real>* mController;
		Ogre::Controller<Ogre::Real>* mTransController;
		typedef Ogre::SharedPtr< MTransEffect >	__MDumpPtr;
		std::list< __MDumpPtr >			mMaterialCopyArray;

    public:
        Model(Ogre::SceneNode* parent, const Ogre::String& resourceName, const Ogre::String& resourceGroup);
        virtual ~Model();

        void setUserObject(Ogre::UserDefinedObject* userObject);
        Ogre::UserDefinedObject* getUserObject(void) const;

        /// Gets the bounding box
        const Ogre::AxisAlignedBox& getBoundingBox(void) const;

        void setCastShadows(bool castShadows);

        bool getCastShadows(void) const
        {
            return mCastShadows;
        }

		void updateTransparent(float nNewAlpha ){ return;};
		void setTransparent( float nAlphaBegin, float nAlphaEnd, float nTimeLast ){return;};

        void setAnimationType(AnimationType animationType);

        AnimationType getAnimationType(void) const
        {
            return mAnimationType;
        }

        /// Modifies the time position
        void addTime(Ogre::Real offset);

        bool existsObject(Ogre::MovableObject* object) const;

	const std::list< Ogre::MovableObject* >	&GetObjList()	const {return mMovables;}

    protected:
        void create(const Ogre::String& resourceName, const Ogre::String& resourceGroup);
        void cleanup();

        void updateCastShadows(void);
        void updateAnimationType(void);
        void enableAnimationStates(void);
        void disableAnimationStates(void);
        void createController(void);
        void destroyController(void);

        static void addTime(AnimationStateList& animationStates, Ogre::Real offset);
    };

}

#endif // 
