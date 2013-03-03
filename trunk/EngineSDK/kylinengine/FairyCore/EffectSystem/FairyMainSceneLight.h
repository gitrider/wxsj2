/********************************************************************
filename:   FairyMainSceneLight.h
created:    2006/05/17
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    keep the info of main scene light of scene.
*********************************************************************/

#ifndef _FairyMainSceneLight_H_
#define _FairyMainSceneLight_H_

#include <OgreColourValue.h>

namespace Ogre
{
    class Light;
}

namespace Fairy
{
    class System;

    /// �����ƹ�仯�ļ����׶�
    enum LightModifyPhase
    {
        LMP_IDLE,       /// ���У���ʱ�����ʹ����Щ�ܿ��Ƴ����ƹ����Ч
        LMP_UPDATE,     /// ����״̬��ֻ�������״̬�£��ƹ���Ϣ�Ż���иı�
        LMP_FADEIN,     /// �����ƹ�������fade in״̬
        LMP_LAST,       /// �����ƹ��������ȶ�״̬
        LMP_FADEOUT,    /// �����ƹ�������fade out״̬
    };

    class MainSceneLight
    {
    public:
        MainSceneLight(System* system);

        /// ���³����ƹ���Ϣ���ڴ����³�������ã��Եõ���ǰ�����Ĺ�Դ��Ϣ
        void updateLightInfo(void);

        /// ���µƹ�仯
        void updateLight(float deltaTime);

        /// ���ӵƹ����ʱ��
        void addLastTime(float time)
        {
            mLastTime += time;
        }

        /// ���õƹ�ĳ���ʱ��
        void setLastTime(float time)
        {
            mLastTime = time;
        }

        /// ���õƹ�仯��Ŀ����ɫֵ
        void setColourValue(const Ogre::ColourValue& colour)
        {
            mDestColour = colour;
        }

        /// ����fade in��fade outʱ��
        void setFadeTime(float fadeInTime, float fadeOutTime)
        {
            mFadeInTime = fadeInTime;
            mFadeOutTime = fadeOutTime;
            mCurrentFadeInTime = 0.0f;
            mCurrentFadeOutTime = 0.0f;
            mLightModifyPhase = LMP_UPDATE;
        }

        /// ��ȡ��ǰ�ƹ�仯��״̬
        LightModifyPhase getLightModifyPhase(void)
        {
            return mLightModifyPhase;
        }

    protected:

        /// ���泡������������Դ��ָ��
        Ogre::Light* mMainSceneLightOne;
        Ogre::Light* mMainSceneLightTwo;

        float mLastTime;
        float mFadeInTime;
        float mCurrentFadeInTime;
        float mFadeOutTime;
        float mCurrentFadeOutTime;

        /// ��ǰ�ƹ���µ���ʱ��
        float mCurrentTime;

        Ogre::ColourValue mOriginOneLightColour;
        Ogre::ColourValue mOriginTwoLightColour;
        Ogre::ColourValue mOriginAmbientColour;
        Ogre::ColourValue mDestColour;

        LightModifyPhase mLightModifyPhase;

        System* mSystem;
    };
}

#endif