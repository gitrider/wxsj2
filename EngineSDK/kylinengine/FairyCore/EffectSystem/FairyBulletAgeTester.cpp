#include "FairyBulletAgeTester.h"
#include "FairyBulletEventSystem.h"
#include "FairyBulletSystem.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletAgeTester::CmdMaxTime			BulletAgeTester::ms_maxTimeCmd;


	BulletAgeTester::BulletAgeTester(BulletEventSystem* eventSystem) : BulletTester()
	{
		m_type = "age";
		m_parent = eventSystem;

		m_maxTime = 0.0;

		if (createParamDictionary("BulletAgeTester"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("maxTime", 
				"The maxTime of bullet age tester.",
				Ogre::PT_REAL),&ms_maxTimeCmd);

			dict->addParameter(Ogre::ParameterDef("output", 
				"The output of bullet Tester.",
				Ogre::PT_STRING),&ms_outputCmd);	
		}
	}
	BulletAgeTester::~BulletAgeTester()
	{
	}
	void BulletAgeTester::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletTester::copyParameters(newOperator);
	
		BulletAgeTester* newAgeTester =  dynamic_cast<BulletAgeTester*>(&newOperator);
		if(newAgeTester)
		{
			newAgeTester->m_maxTime = m_maxTime;
		}
    }
	void BulletAgeTester::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it = 
			m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			Real fAge = pBulletSystem->getAge();
			if(fAge > m_maxTime)
			{
				m_parent->addTransition(m_outputEventName,pBulletSystem);
			}
			it ++;

		}
	}
	String BulletAgeTester::CmdMaxTime::doGet(const void* source) const
	{
		const BulletAgeTester* object = static_cast<const BulletAgeTester*>(source);
		return Ogre::StringConverter::toString(object->getMaxTime());
	}
	void BulletAgeTester::CmdMaxTime::doSet(void* target, const String& val)
	{
		BulletAgeTester* object = static_cast<BulletAgeTester*>(target);
		object->setMaxTime(Ogre::StringConverter::parseReal(val));
	}
	
} 