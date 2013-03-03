/********************************************************************
filename:   FairyAnimationSound.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#ifndef _AnimationCameraShake_H_
#define _AnimationCameraShake_H_

#include "Core/FairyPrerequisites.h"

#include <OgreStringInterface.h>
#include "Core/FairySystem.h"
#include "FairyBulletOperator.h"
namespace Fairy	{

	class AnimationCameraShake : public Ogre::StringInterface
	{
	public:
		class CmdAttachTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdAmplitudeName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdDurationName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif	
		};

		class CmdFrequencyName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const Ogre::String& val);
#if OGRE_STRING_INTERFACE_COPY
			virtual void doCopy(void* target, const void* source);
#endif	
		};

		AnimationCameraShake(System *system);
		~AnimationCameraShake(void);

		bool createInstance(void);
		void destroyInstance(void);

		/** 更新相机震动动画		[5/31/2010 陈军龙]
		@remarks  将时间映射到曲线函数Sin上
		映射公式： (2 * PI) / (1 / Frequency) = x / Age   
		计算偏移量公式： offset = Amplitude * Sin(x) * (1 - Age / Duration)
		*/
		void updateInstance( float time);

		void setAttachTime(float time)
		{
			mAttachTime = time;
		}
		float getAttachTime(void) const
		{
			return mAttachTime;
		}

		void setAmplitude( const Real &val )
		{
			mAmplitude = val;
		}
		const Real& getAmplitude(void) const
		{
			return mAmplitude;
		}

		void setDuration( const Real &val )
		{
			mDuration = val;
		}
		const Real& getDuration(void) const
		{
			return mDuration;
		}

		void setFrequency( const Real &val )
		{
			mFrequency = val;
		}
		const Real& getFrequency(void) const
		{
			return mFrequency;
		}

        /** 拷贝所有特效参数变量
        @remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
        来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
        用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
        中添加相对项
        */
        void copyParameters(AnimationCameraShake& newInfo) const;

		//创建该类型实例的Bullet操作器
		BulletOperator* mCreaterOperator;

	public:


	protected:

		bool	mCreateInstance;
		float   mAttachTime;

		System *mSystem;


		//  [5/31/2010 陈军龙]

		/// 震动前相机位置
        Ogre::Vector3 mCameraPosBackup;
        Ogre::Camera* mCamera;
		/// 振幅
		Real	mAmplitude;
		/// 持续时间
		Real	mDuration;
		/// 频率
		Real	mFrequency;
		/// 到目前持续的时间
		Real	mAge;


	//////////////////////////////////////////////////////////////////////////
		static CmdAttachTime msAttachTimeCmd;
		static CmdAmplitudeName msAmplitudeCmd;
		static CmdDurationName msDurationCmd;
		static CmdFrequencyName msFrequencyCmd;
	};
}

#endif