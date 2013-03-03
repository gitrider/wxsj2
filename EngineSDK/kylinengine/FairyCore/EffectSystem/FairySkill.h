/********************************************************************
filename:   FairySkill.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    a skill contains effects, sounds ,ribbons or any other things, skill is defined in 
			.skill files.
*********************************************************************/

#ifndef __FairySkill_H__
#define __FairySkill_H__

#include <OgreStringVector.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreStringInterface.h>

#include "Core/FairyPrerequisites.h"
#include "FairyEffectManager.h"
#include "FairyAnimationRibbon.h"
#include "FairyAnimationSound.h"
#include "FairyAnimationCameraShake.h"

namespace Fairy	{
	
	class AnimationEffectInfo;
	class AnimationRibbon;
	class AnimationSound;
    class AnimationSceneLightInfo;
	class AnimationBulletFlow;
	class AnimationCameraShake;

	class Skill : public Ogre::StringInterface
	{
		typedef std::vector<Ogre::Real> TimeArray;

	public:

		class CmdAnimationName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdHitTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdBreakTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdShakeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

        class CmdRepeatEffect : public Ogre::ParamCommand
        {
        public:
           Ogre::String doGet(const void* target) const;
            void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		//////////////////////////////////////////////////////////////////////////
		Skill(const Ogre::String &skillName, System *system);

		~Skill();

		const Ogre::String& getAnimationName(void) const
		{
			return mAnimName;
		}

		void setAnimationName(const Ogre::String &name)
		{
			mAnimName = name;
		}

		const Ogre::String& getSkillName(void) const
		{
			return mSkillName;
		}

		void setSkillName(const  Ogre::String& skill)
		{
			mSkillName = skill;
		}

		System * getSystem(void)
		{
			return mSystem;
		}

		void setBreakTime(Ogre::Real time, unsigned short index)
		{
			assert (index < mBreakTimeArray.size());
			mBreakTimeArray[index] = time;
		}

		Ogre::Real getBreakTime(unsigned short index) const
		{
			assert (index < mBreakTimeArray.size());
			return mBreakTimeArray[index];
		}

		unsigned short getNumBreakTimes(void);

		void setBreakTimeFromString(const Ogre::String &breakTimeStr);

		String getBreakTimeString(void) const;
		
		void setHitTime(Ogre::Real time, unsigned short index)
		{
			assert (index < mHitTimeArray.size());
			mHitTimeArray[index] = time;
		}

		Ogre::Real getHitTime(unsigned short index) const
		{
			assert (index < mHitTimeArray.size());
			return mHitTimeArray[index];
		}

		unsigned short getNumHitTimes(void);

		void setHitTimeFromString(const Ogre::String &hitTimeStr);

		String getHitTimeString(void) const;

		void setShakeTime(Ogre::Real time, unsigned short index)
		{
			assert (index < mShakeTimeArray.size());
			mShakeTimeArray[index] = time;
		}

		Ogre::Real getShakeTime(unsigned short index) const
		{
			assert (index < mShakeTimeArray.size());
			return mShakeTimeArray[index];
		}

		unsigned short getNumShakeTimes(void);

		void setShakeTimeFromString(const Ogre::String &shakeTimeStr);

		String getShakeTimeString(void) const;

		Skill & operator = (const Skill &rhs);

		// 进行一些skill中各种元素的清除操作
		void shutdown(void);

        void setRepeatEffect(bool repeat)
        {
            mRepeatEffect = repeat;
        }
        bool getRepeatEffect(void) const
        {
            return mRepeatEffect;
        }

        /** 拷贝所有特效参数变量
        @remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
        来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
        用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
        中添加相对项
        */
        void copyParameters(Skill* newInfo) const;

		//////////////////////////////////////////////////////////////////////////		
		AnimationEffectInfo* addAnimationEffectInfo(void);
		
		AnimationEffectInfo* getAnimationEffectInfo(unsigned short index) const;

		unsigned short getNumAnimationEffectInfos(void) const;

		void removeAnimationEffectInfo(unsigned short index);

		void removeAllAnimationEffectInfos(void);

		//////////////////////////////////////////////////////////////////////////
		AnimationRibbon* addAnimationRibbon(void);

		AnimationRibbon* getAnimationRibbon(unsigned short index) const;

		unsigned short getNumAnimationRibbons(void) const;

		void removeAnimationRibbon(unsigned short index);

		void removeAllAnimationRibbons(void);

		//////////////////////////////////////////////////////////////////////////
		AnimationSound* addAnimationSound(void);

		AnimationSound* getAnimationSound(unsigned short index) const;

		unsigned short getNumAnimationSounds(void) const;

		void removeAnimationSound(unsigned short index);

		void removeAllAnimationSounds(void);

        //////////////////////////////////////////////////////////////////////////		
        AnimationSceneLightInfo* addAnimationSceneLightInfo(void);

        AnimationSceneLightInfo* getAnimationSceneLightInfo(unsigned short index) const;

        unsigned short getNumAnimationSceneLightInfos(void) const;

        void removeAnimationSceneLightInfo(unsigned short index);

        void removeAllAnimationSceneLightInfos(void);

		//////////////////////////////////////////////////////////////////////////		
		AnimationBulletFlow* addAnimationBulletFlow(void);

		AnimationBulletFlow* getAnimationBulletFlow(unsigned short index) const;

		unsigned short getNumAnimationBulletFlows(void) const;

		void removeAnimationBulletFlow(unsigned short index);

		void removeAllAnimationBulletFlows(void);

		//////////////////////////////////////////////////////////////////////////		
		AnimationCameraShake* addAnimationCameraShake(void);

		AnimationCameraShake* getAnimationCameraShake(unsigned short index) const;

		unsigned short getNumAnimationCameraShake(void) const;

		void removeAnimationCameraShake(unsigned short index);

		void removeAllAnimationCameraShakes(void);


	public:

		typedef std::vector<AnimationEffectInfo *> AnimationEffectInfos;
		typedef std::vector<AnimationRibbon *> AnimationRibbons;
		typedef std::vector<AnimationSound *> AnimationSounds;
        typedef std::vector<AnimationSceneLightInfo *> AnimationSceneLightInfos;
		typedef std::vector<AnimationBulletFlow*> AnimationBulletFlows;
		typedef std::vector<AnimationCameraShake*> AnimationCameraShakes;
	protected:

		bool initParamDictionary(void);

    protected:

		String mAnimName;
		String mSkillName;

		System *mSystem;

		AnimationEffectInfos mAnimationEffectInfos;
		AnimationRibbons mAnimationRibbons;
		AnimationSounds mAnimationSounds;
        AnimationSceneLightInfos mAnimationSceneLightInfos;
		AnimationBulletFlows	mAnimationBulletFlows;
		AnimationCameraShakes mAnimationCameraShakes;
		//Real mHitTime;
		TimeArray mHitTimeArray;

		//Ogre::Real mBreakTime;
		TimeArray mBreakTimeArray;
	
		TimeArray mShakeTimeArray;

		bool mRepeatEffect;
		//////////////////////////////////////////////////////////////////////////
		static CmdAnimationName msAnimationNameCmd;
		static CmdBreakTime msBreakTimeCmd;
		static CmdHitTime msHitTimeCmd;
		static CmdShakeTime msShakeTimeCmd;
        static CmdRepeatEffect msRepeatEffectCmd;

	public:
		void*				m_userData;
	};
}

#endif