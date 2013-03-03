#include <OgreStringConverter.h>
#include "FairyBulletSystem.h"
#include "FairyBulletDeleterOperator.h"
#include "FairyBulletFlowSystem.h"

namespace Fairy
{
	BulletDeleterOperator::CmdMaturity		BulletDeleterOperator::ms_maturityCmd;

	BulletDeleterOperator::BulletDeleterOperator(BulletEventSystem* eventSystem) : BulletOperator(),
		m_isOperated(false), m_maturity(6)
	{
		m_type = "deleter";
		m_parent = eventSystem;
		if (createParamDictionary("BulletDeleterOperator"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("maturity", 
				"The maturity of bullet deleter operator.",
				Ogre::PT_REAL),&ms_maturityCmd);	
		}
	}
	BulletDeleterOperator::~BulletDeleterOperator()
	{
	}

	void BulletDeleterOperator::operateBulletEventSystem(Real timeElapsed)
	{
		if(m_parent->getActiveBulletSystem().empty())
			return;
        
		bool isMaturity = false;
		std::vector<BulletSystem*>::iterator it = m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem)
			{
				Real fCumulateTime = pBulletSystem->getAge();
				if (m_maturity < fCumulateTime)
				{
					m_parent->getParent()->removeBulletSystem(pBulletSystem);
					isMaturity = true;
				}
			}
			it ++;
		}

		if(isMaturity)
		{
			m_parent->removeAllActiveBulletSystem();
		}

	}
	void BulletDeleterOperator::copyParameters(BulletOperator& newOperator) const
	{
		assert(newOperator);
		BulletDeleterOperator* newDeleterOperator =  dynamic_cast<BulletDeleterOperator*>(&newOperator);
		if(newDeleterOperator)
		{
			newDeleterOperator->m_maturity = m_maturity;
		}
	}

	String BulletDeleterOperator::CmdMaturity::doGet(const void* source) const
	{
		const BulletDeleterOperator* object = static_cast<const BulletDeleterOperator*>(source);
		return Ogre::StringConverter::toString(object->getMaturity());
	}
	void BulletDeleterOperator::CmdMaturity::doSet(void* target, const String& val)
	{
		BulletDeleterOperator* object = static_cast<BulletDeleterOperator*>(target);
		object->setMaturity(Ogre::StringConverter::parseReal(val));
	}
}
