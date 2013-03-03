/********************************************************************
filename:   AutoSaveSceneController.h

purpose:    һ��ÿ֡���е�Controller�������ڹ涨ʱ�������Զ����泡��
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