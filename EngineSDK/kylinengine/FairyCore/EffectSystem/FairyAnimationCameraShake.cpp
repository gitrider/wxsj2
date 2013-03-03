/********************************************************************
filename:   FairyAnimationRibbon.cpp

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#include "FairyAnimationCameraShake.h"

#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include "FairyBulletCallbacks.h"
#include "FairyEffectManager.h"
namespace Fairy	{

	AnimationCameraShake::CmdAttachTime	AnimationCameraShake::msAttachTimeCmd;
	AnimationCameraShake::CmdAmplitudeName AnimationCameraShake::msAmplitudeCmd;
	AnimationCameraShake::CmdDurationName AnimationCameraShake::msDurationCmd;
	AnimationCameraShake::CmdFrequencyName AnimationCameraShake::msFrequencyCmd;
	//////////////////////////////////////////////////////////////////////////
	AnimationCameraShake::AnimationCameraShake(System *system) : 
			  mSystem(system)
		    , mCreaterOperator(NULL)
			, mAttachTime(0.0f)
			, mAmplitude(0.0f)
			, mDuration(0.0f)
			, mFrequency(0.0f)
			, mCreateInstance(false)
			, mAge(0.0f)
			, mCameraPosBackup()
			, mCamera(NULL)
	{
		if (createParamDictionary("AnimationCameraShake"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("AttachTime", 
				"the time that CameraShake.",
				Ogre::PT_REAL),&msAttachTimeCmd);

			dict->addParameter(Ogre::ParameterDef("Amplitud", 
				"the amplitud  of CameraShake.",
				Ogre::PT_REAL),&msAmplitudeCmd);

			dict->addParameter(Ogre::ParameterDef("Duration", 
				"the duration  of CameraShake.",
				Ogre::PT_REAL),&msDurationCmd);

			dict->addParameter(Ogre::ParameterDef("Frequency", 
				"the frequency  of CameraShake.",
				Ogre::PT_REAL),&msFrequencyCmd);
		}

	}
	AnimationCameraShake::~AnimationCameraShake(void)
	{
		destroyInstance();
	}
    //-----------------------------------------------------------------------
    void AnimationCameraShake::copyParameters(AnimationCameraShake& newInfo) const
    {
        newInfo.mAttachTime = mAttachTime;
		newInfo.mAmplitude = mAmplitude;
		newInfo.mDuration = mDuration;
		newInfo.mFrequency = mFrequency;
    }
	//////////////////////////////////////////////////////////////////////////
	String AnimationCameraShake::CmdAttachTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationCameraShake *>(target)->getAttachTime() );
	}
	void AnimationCameraShake::CmdAttachTime::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationCameraShake *>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationCameraShake::CmdAttachTime::doCopy(void* target, const void* source)
    {
        static_cast<AnimationCameraShake *>(target)->setAttachTime(
            static_cast<const AnimationCameraShake *>(source)->getAttachTime() );
    }
#endif

	String AnimationCameraShake::CmdAmplitudeName::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationCameraShake *>(target)->getAmplitude() );
	}
	void AnimationCameraShake::CmdAmplitudeName::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationCameraShake *>(target)->setAmplitude(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationCameraShake::CmdAmplitudeName::doCopy(void* target, const void* source)
    {
        static_cast<AnimationCameraShake *>(target)->setAmplitude(
            static_cast<const AnimationCameraShake *>(source)->getAmplitude() );
    }
#endif

	String AnimationCameraShake::CmdDurationName::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationCameraShake *>(target)->getDuration() );
	}
	void AnimationCameraShake::CmdDurationName::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationCameraShake *>(target)->setDuration(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
	//-----------------------------------------------------------------------
	void AnimationCameraShake::CmdDurationName::doCopy(void* target, const void* source)
	{
		static_cast<AnimationCameraShake *>(target)->setDuration(
			static_cast<const AnimationCameraShake *>(source)->getDuration() );
	}
#endif

	String AnimationCameraShake::CmdFrequencyName::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationCameraShake *>(target)->getFrequency() );
	}
	void AnimationCameraShake::CmdFrequencyName::doSet(void* target, const Ogre::String& val)
	{
		static_cast<AnimationCameraShake *>(target)->setFrequency(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
	//-----------------------------------------------------------------------
	void AnimationCameraShake::CmdFrequencyName::doCopy(void* target, const void* source)
	{
		static_cast<AnimationCameraShake *>(target)->setFrequency(
			static_cast<const AnimationCameraShake *>(source)->getFrequency() );
	}
#endif

	bool AnimationCameraShake::createInstance(void)
	{
		if (mCreateInstance)
		{
			return true;
		}
		else
		{
			if(mDuration < 0.0f)
				return false;

			// 对持续时间进行约数，保证震动保持在周期的整数倍
			//int times = (int)mDuration * mFrequency;
			//mDuration = (times > 0 ? times : 1) / mFrequency;
			//mCamera = mSystem->getCamera();
			//mCameraPosBackup = mCamera->getPosition();
			EffectManager::getSingleton().getEffectCameraShakeCallback()->onShakeCallBack(mAmplitude,mDuration,mFrequency);
			mCreateInstance = true;

			return true;
		}	
	}

	void AnimationCameraShake::destroyInstance(void)
	{
		mCreateInstance = false;
	}

	/** 更新相机震动动画		[5/31/2010 陈军龙]
	@remarks  将时间映射到曲线函数Sin上
	映射公式： (2 * PI) / (1 / Frequency) = x / Age   
	计算偏移量公式： offset = Amplitude * Sin(x) * (1 - Age / Duration)
	*/
	void AnimationCameraShake::updateInstance( float time)
	{
		//mAge += time;
		//
		//if (NULL != mCamera)
		//{
		//	// 若超时则归位
		//	if(mAge > mDuration)
		//	{
		//		mCamera->setPosition(mCameraPosBackup);
		//		return;
		//	}

		//	Real offset = mAmplitude * Ogre::Math::Sin(mAge * Ogre::Math::TWO_PI * mFrequency)* (1 - mAge / mDuration);
		//	Ogre::Vector3 vCameraPos = mCameraPosBackup;
		//	vCameraPos.y += offset;
		//	mCamera->setPosition(vCameraPos);
		//}
	}
}