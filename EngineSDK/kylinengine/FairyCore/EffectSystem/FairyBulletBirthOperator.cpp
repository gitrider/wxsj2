#include "FairyBulletBirthOperator.h"
#include "FairyBulletSystemManager.h"
#include "FairyBulletEventSystem.h"
#include "FairyBulletFlowSystem.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletBirthOperator::CmdBulletSystemTemplateName	BulletBirthOperator::ms_bulletTemplateCmd;
	BulletBirthOperator::CmdBirthTimes					BulletBirthOperator::ms_birthTimesCmd;


	BulletBirthOperator::BulletBirthOperator(BulletEventSystem* eventSystem) : BulletOperator(),
		m_birthedTimes(0.0f),m_birthTimes(0.0f),m_isOperated(false)
	{
		m_type = "birth";
		m_parent = eventSystem;

		if (createParamDictionary("BulletBirthOperator"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("bulletsystemname", 
				"The bulletsystemname of bullet birth operator.",
				Ogre::PT_STRING),&ms_bulletTemplateCmd);	  

			dict->addParameter(Ogre::ParameterDef("birthtimes", 
				"The times of bullet birth operator.",
				Ogre::PT_REAL),&ms_birthTimesCmd);	

		}
	}
	BulletBirthOperator::~BulletBirthOperator()
	{
	}
	void BulletBirthOperator::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletBirthOperator* newBirthOperator =  dynamic_cast<BulletBirthOperator*>(&newOperator);
		if(newBirthOperator)
		{
			newBirthOperator->m_bulletSystemName = m_bulletSystemName;
			newBirthOperator->m_birthTimes = m_birthTimes;
		}
    }
	String BulletBirthOperator::CmdBulletSystemTemplateName::doGet(const void* source) const
	{
		const BulletBirthOperator* object = static_cast<const BulletBirthOperator*>(source);
		return object->getBulletSystemName();
	}
	void BulletBirthOperator::CmdBulletSystemTemplateName::doSet(void* target, const String& val)
	{
		BulletBirthOperator* object = static_cast<BulletBirthOperator*>(target);
		object->setBulletSystemName(val);
	}
	String BulletBirthOperator::CmdBirthTimes::doGet(const void* source) const
	{
		const BulletBirthOperator* object = static_cast<const BulletBirthOperator*>(source);
		return Ogre::StringConverter::toString(object->getBirthTimes());
	}

	void BulletBirthOperator::CmdBirthTimes::doSet(void* target, const String& val)
	{
		BulletBirthOperator* object = static_cast<BulletBirthOperator*>(target);
		object->setBirthTimes(Ogre::StringConverter::parseReal(val));
	}

	void BulletBirthOperator::operateBulletEventSystem(Real timeElapsed)
	{
		
		if(m_birthedTimes >= m_birthTimes && !m_isOperated)
		{
			BulletSystem* pBulletSystemTemplate = BulletSystemManager::getSingleton().getBulletSystemTemplate(m_bulletSystemName);
			if(pBulletSystemTemplate)
			{
				BulletFlowSystem* pBulletFlow = m_parent->getParent();
				BulletSystem* pNewBulletSystem = pBulletFlow->addBulletSystem(m_bulletSystemName);
				m_parent->addActiveBulletSystem(pNewBulletSystem);
				m_isOperated = true;
			}
		}
		m_birthedTimes += timeElapsed;
	}
}