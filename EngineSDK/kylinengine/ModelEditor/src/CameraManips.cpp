#include "CameraManips.h"
#include "SceneManipulator.h"
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>

namespace Fairy {

//////////////////////////////////////////////////////////////////////////
    
CameraPan::CameraPan(SceneManipulator* sceneManipulator)
    : Manip(sceneManipulator)
{
}

const String& CameraPan::getName(void) const
{
    static const String name = "CameraPan";
    return name;
}

void CameraPan::_onDrag(const Point& pt)
{
    getCamera()->moveRelative(
        Ogre::Vector3(+mDragDelta.x * getSceneManipulator()->_getMoveFactor() / getViewport()->getActualWidth(),
                      -mDragDelta.y * getSceneManipulator()->_getMoveFactor() / getViewport()->getActualHeight(),
                       0));
    getSceneManipulator()->_fireCameraPosChanged();
}

//////////////////////////////////////////////////////////////////////////
    
CameraRotate::CameraRotate(SceneManipulator* sceneManipulator)
    : Manip(sceneManipulator)
{
}

const String& CameraRotate::getName(void) const
{
    static const String name = "CameraRotate";
    return name;
}

void CameraRotate::_onDrag(const Point& pt)
{
    getCamera()->yaw(Ogre::Degree(-mDragDelta.x * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth()));

    // 只有在不使用真实摄像机的情况下才允许上下旋转摄像机
    if ( !getSceneManipulator()->getUseRealCameraAngle() )
        getCamera()->pitch(Ogre::Degree(-mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualHeight()));

    getSceneManipulator()->_fireCameraPosChanged();
}

//////////////////////////////////////////////////////////////////////////

void HintBasedCameraManip::_onBegin(const Point& pt)
{
}

//////////////////////////////////////////////////////////////////////////

CameraDragMove::CameraDragMove(SceneManipulator* sceneManipulator)
    : HintBasedCameraManip(sceneManipulator)
{
}

const String& CameraDragMove::getName(void) const
{
    static const String name = "CameraDragMove";
    return name;
}

void CameraDragMove::_onDrag(const Point& pt)
{
	float fDistance = getCamera()->getPosition().length ();
	getCamera()->yaw(Ogre::Degree(-mDragDelta.x * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth()));
	getCamera()->pitch(Ogre::Degree(-mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualHeight()));
	Ogre::Vector3 dir = -(getCamera()->getDirection());
	dir.normalise();
	Ogre::Vector3 position = dir * fDistance;
	getCamera()->setPosition(position);
	getSceneManipulator()->_fireCameraPosChanged();
 
}

//////////////////////////////////////////////////////////////////////////

CameraDragRotate::CameraDragRotate(SceneManipulator* sceneManipulator)
    : HintBasedCameraManip(sceneManipulator)
{
}

const String& CameraDragRotate::getName(void) const
{
    static const String name = "CameraDragRotate";
    return name;
}

void CameraDragRotate::_onDrag(const Point& pt)
{

}

}
