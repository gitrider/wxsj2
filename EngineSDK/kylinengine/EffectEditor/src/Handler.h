#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "Core/FairyPrerequisites.h"

namespace Fairy {

    class SceneManipulator;
    class OperatorManager;
    class Handler {
    protected:
        SceneManipulator* mSceneManipulator;

    public:
        Handler(SceneManipulator* sceneManipulator) : mSceneManipulator(sceneManipulator) {}
        virtual ~Handler() {}

        // Accessor

        SceneManipulator* getSceneManipulator(void) const   { return mSceneManipulator; }

        OperatorManager* getOperatorManager(void) const;

        Ogre::Root* getRoot(void) const;
        Ogre::RenderWindow* getRenderWindow(void) const;
        Ogre::SceneManager* getSceneManager(void) const;
        Ogre::Camera* getCamera(void) const;
        Ogre::Viewport* getViewport(void) const;

        Ogre::SceneNode* getIndicatorRootSceneNode(void) const;

    public:
        virtual const String& getName(void) const = 0;
    };

}

#endif 
