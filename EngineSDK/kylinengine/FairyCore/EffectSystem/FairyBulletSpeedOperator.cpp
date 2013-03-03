#include "FairyBulletSpeedOperator.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "FairyBulletSystem.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletSpeedOperator::CmdMode			BulletSpeedOperator::ms_modeCmd;
	BulletSpeedOperator::CmdDirection		BulletSpeedOperator::ms_directionCmd;
	BulletSpeedOperator::CmdSpeed			BulletSpeedOperator::ms_speedCmd;

	BulletSpeedOperator::BulletSpeedOperator(BulletEventSystem* eventSystem) : BulletOperator(),
		m_isOperated(false)
	{
		m_type = "speed";
		m_parent = eventSystem;
		
		m_direction = Ogre::Vector3::ZERO;
		m_speed = 0.0;

		if (createParamDictionary("BulletSpeedOperator"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("mode", 
				"The mode of bullet speed operator.",
				Ogre::PT_STRING),&ms_modeCmd);	  

			dict->addParameter(Ogre::ParameterDef("direction", 
				"The direction of bullet speed operator.",
				Ogre::PT_VECTOR3),&ms_directionCmd);	

			dict->addParameter(Ogre::ParameterDef("speed", 
				"The speed of bullet speed operator.",
				Ogre::PT_REAL),&ms_speedCmd);	

		}
	}
	BulletSpeedOperator::~BulletSpeedOperator()
	{
	}
	void BulletSpeedOperator::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletSpeedOperator* newSpeedOperator =  dynamic_cast<BulletSpeedOperator*>(&newOperator);
		if(newSpeedOperator)
		{
			newSpeedOperator->m_mode = m_mode;
			newSpeedOperator->m_direction = m_direction;
			newSpeedOperator->m_speed = m_speed;
		}
    }
	void BulletSpeedOperator::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it =  m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem && pBulletSystem->getIsCreated())
			{

				Real fCumulateTime = pBulletSystem->getAge();

				TransformInfo info;
				info = pBulletSystem->getTransformInfo();
				Ogre::Vector3 vDirection = m_direction.normalisedCopy();
				Ogre::Vector3 vBulletPosition = info.mPosition;
				vBulletPosition += vDirection*m_speed*pBulletSystem->getDelta();

				info.mPosition = vBulletPosition;
				pBulletSystem->setTransformInfo(info);
				pBulletSystem->setPosition(info.mPosition);
			}
			it ++;
		}
	}
	String BulletSpeedOperator::CmdMode::doGet(const void* source) const
	{
		const BulletSpeedOperator* object = static_cast<const BulletSpeedOperator*>(source);
		return object->getMode();
	}
	void BulletSpeedOperator::CmdMode::doSet(void* target, const String& val)
	{
		BulletSpeedOperator* object = static_cast<BulletSpeedOperator*>(target);
		object->setMode(val);
	}
	String BulletSpeedOperator::CmdDirection::doGet(const void* source) const
	{
		const BulletSpeedOperator* object = static_cast<const BulletSpeedOperator*>(source);
		return Ogre::StringConverter::toString(object->getDirection());
	}

	void BulletSpeedOperator::CmdDirection::doSet(void* target, const String& val)
	{
		BulletSpeedOperator* object = static_cast<BulletSpeedOperator*>(target);
		object->setDirection(Ogre::StringConverter::parseVector3(val));
	}
	String BulletSpeedOperator::CmdSpeed::doGet(const void* source) const
	{
		const BulletSpeedOperator* object = static_cast<const BulletSpeedOperator*>(source);
		return Ogre::StringConverter::toString(object->getSpeed());
	}
	void BulletSpeedOperator::CmdSpeed::doSet(void* target, const String& val)
	{
		BulletSpeedOperator* object = static_cast<BulletSpeedOperator*>(target);
		object->setSpeed(Ogre::StringConverter::parseReal(val));
	}
}