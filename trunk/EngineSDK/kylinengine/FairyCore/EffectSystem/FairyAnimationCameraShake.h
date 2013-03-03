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

		/** ��������𶯶���		[5/31/2010 �¾���]
		@remarks  ��ʱ��ӳ�䵽���ߺ���Sin��
		ӳ�乫ʽ�� (2 * PI) / (1 / Frequency) = x / Age   
		����ƫ������ʽ�� offset = Amplitude * Sin(x) * (1 - Age / Duration)
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

        /** ����������Ч��������
        @remarks ����ԭ����Ogre::StringInterface::copyParametersTo�������
        �����в����Ŀ����ٶ�̫������Ϊ������һЩ�ַ����Ĳ�����������
        ��һ���������������еĲ���������ÿ��һ����������Ҫ���������
        ����������
        */
        void copyParameters(AnimationCameraShake& newInfo) const;

		//����������ʵ����Bullet������
		BulletOperator* mCreaterOperator;

	public:


	protected:

		bool	mCreateInstance;
		float   mAttachTime;

		System *mSystem;


		//  [5/31/2010 �¾���]

		/// ��ǰ���λ��
        Ogre::Vector3 mCameraPosBackup;
        Ogre::Camera* mCamera;
		/// ���
		Real	mAmplitude;
		/// ����ʱ��
		Real	mDuration;
		/// Ƶ��
		Real	mFrequency;
		/// ��Ŀǰ������ʱ��
		Real	mAge;


	//////////////////////////////////////////////////////////////////////////
		static CmdAttachTime msAttachTimeCmd;
		static CmdAmplitudeName msAmplitudeCmd;
		static CmdDurationName msDurationCmd;
		static CmdFrequencyName msFrequencyCmd;
	};
}

#endif