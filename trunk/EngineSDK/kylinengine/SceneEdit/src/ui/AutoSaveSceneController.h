/********************************************************************
filename:   AutoSaveSceneController.h

purpose:    һ��ÿ֡���е�Controller�������ڹ涨ʱ�������Զ����泡��
*********************************************************************/

#ifndef _AUTOSAVESCENECONTROLLER_H_
#define _AUTOSAVESCENECONTROLLER_H_

#include "Core/FairyPrerequisites.h"

#include <OgreController.h>

class FairyEditorFrame;

class AutoSaveSceneController
{

public:

    AutoSaveSceneController(FairyEditorFrame* frame, float interval);
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

    FairyEditorFrame* getFrame(void)
    {
        return mFrame;
    }

    void reset(void);

private:

    void _createController(FairyEditorFrame* frame, float interval);

private:

    Ogre::Controller<float>* mController;

    FairyEditorFrame* mFrame;

    float mIntervalTime;

    bool mEnableAutoSave;
};

#endif