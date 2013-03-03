#include "FairyBulletCameraShakeOperator.h"
#include "FairyBulletSystem.h"
#include "FairyAnimationCameraShake.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletCameraShakeOperator::CmdAmplitude					BulletCameraShakeOperator::ms_amplitudeCmd;
	BulletCameraShakeOperator::CmdDuration					BulletCameraShakeOperator::ms_durationCmd;
	BulletCameraShakeOperator::CmdFrequencyName				BulletCameraShakeOperator::msFrequencyCmd;


	BulletCameraShakeOperator::BulletCameraShakeOperator(BulletEventSystem* eventSystem) : BulletOperator()
		,m_amplitude(0.0f),m_duration(0.0f),m_frequency(0.0f),m_isStart(false),m_age(0.0f)
	{
		m_type = "camerashake";
		m_parent = eventSystem;

		if (createParamDictionary("BulletCameraShakeOperator"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("amplitude", 
				"The amplitude of bullet CameraShake operator.",
				Ogre::PT_REAL),&ms_amplitudeCmd);	  

			dict->addParameter(Ogre::ParameterDef("duration", 
				"The duration of bullet CameraShake operator.",
				Ogre::PT_REAL),&ms_durationCmd);

			dict->addParameter(Ogre::ParameterDef("frequency", 
				"the frequency  of bullet CameraShake operator.",
				Ogre::PT_REAL),&msFrequencyCmd);

		}
	}
	BulletCameraShakeOperator::~BulletCameraShakeOperator()
	{
	}
	void BulletCameraShakeOperator::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletCameraShakeOperator* newCameraShakeOperator =  dynamic_cast<BulletCameraShakeOperator*>(&newOperator);
		if(newCameraShakeOperator)
		{
			newCameraShakeOperator->m_amplitude = m_amplitude;
			newCameraShakeOperator->m_duration = m_duration;
			newCameraShakeOperator->m_frequency = m_frequency;
		}
    }
	String BulletCameraShakeOperator::CmdAmplitude::doGet(const void* source) const
	{
		const BulletCameraShakeOperator* object = static_cast<const BulletCameraShakeOperator*>(source);
		return Ogre::StringConverter::toString(object->getAmplitude());
	}
	void BulletCameraShakeOperator::CmdAmplitude::doSet(void* target, const String& val)
	{
		BulletCameraShakeOperator* object = static_cast<BulletCameraShakeOperator*>(target);
		object->setAmplitude(Ogre::StringConverter::parseReal(val));
	}
	String BulletCameraShakeOperator::CmdDuration::doGet(const void* source) const
	{
		const BulletCameraShakeOperator* object = static_cast<const BulletCameraShakeOperator*>(source);
		return Ogre::StringConverter::toString(object->getDuration());
	}

	void BulletCameraShakeOperator::CmdDuration::doSet(void* target, const String& val)
	{
		BulletCameraShakeOperator* object = static_cast<BulletCameraShakeOperator*>(target);
		object->setDuration(Ogre::StringConverter::parseReal(val));
	}

	String BulletCameraShakeOperator::CmdFrequencyName::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BulletCameraShakeOperator *>(target)->getFrequency() );
	}
	void BulletCameraShakeOperator::CmdFrequencyName::doSet(void* target, const Ogre::String& val)
	{
		static_cast<BulletCameraShakeOperator *>(target)->setFrequency(Ogre::StringConverter::parseReal(val));
	}

	void BulletCameraShakeOperator::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it = 
			m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem)
			{
				bool hasCreater = false;
				for(int i =0; i< pBulletSystem->getNumAnimationCameraShake();i++)
				{
					AnimationCameraShake* pAnimationCameraShake = pBulletSystem->getAnimationCameraShake(i);
					if(pAnimationCameraShake->mCreaterOperator && pAnimationCameraShake->mCreaterOperator == this)
						hasCreater = true;
				}
				if(!hasCreater)
				{
					AnimationCameraShake* pAnimationCameraShake = pBulletSystem->addAnimationCameraShake();
					pAnimationCameraShake->setAttachTime(0.0f);
					pAnimationCameraShake->setAmplitude(m_amplitude);
					pAnimationCameraShake->setDuration(m_duration);
					pAnimationCameraShake->setFrequency(m_frequency);
					pAnimationCameraShake->mCreaterOperator = this;
				}
				
			}
			it ++;

		}
	}

}