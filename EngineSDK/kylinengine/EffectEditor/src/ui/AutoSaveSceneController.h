/********************************************************************
filename:   AutoSaveSceneController.h

purpose:    一个每帧运行的Controller，用于在规定时间间隔内自动保存场景
*********************************************************************/

#ifndef _AUTOSAVESCENECONTROLLER_H_
#define _AUTOSAVESCENECONTROLLER_H_

#include "Core/FairyPrerequisites.h"

#include <OgreController.h>

class EffectEditorFrame;

class AutoSaveSceneController
{

public:

    AutoSaveSceneController(EffectEditorFrame* frame, float interval);
    ~AutoSaveSceneController();

    void setIntervalTime(float time);
    float getIntervalTime(void)
    {
        return mIntervalTime;
    }

    void setAutoSaveEnable(bool enable);
    bool getAutoSaveEnable(void)
    {
        return mEnableAutoSave;
    }

    EffectEditorFrame* getFrame(void)
    {
        return mFrame;
    }

    void reset(void);

private:

    void _createController(EffectEditorFrame* frame, float interval);

private:

    Ogre::Controller<float>* mController;

    EffectEditorFrame* mFrame;

    float mIntervalTime;

    bool mEnableAutoSave;
};

#endif