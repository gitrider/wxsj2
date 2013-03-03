#include "FairyBulletHeightFieldCollisionTester.h"
#include "FairyBulletSystem.h"
#include "Core/Terrain.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletHeightFieldCollisionTester::CmdLessThan			BulletHeightFieldCollisionTester::ms_lessThanCmd;


	BulletHeightFieldCollisionTester::BulletHeightFieldCollisionTester(BulletEventSystem* eventSystem) : BulletTester()
	{
		m_type = "heightfieldcollision";
		m_parent = eventSystem;
		if (createParamDictionary("BulletHeightFieldCollisionTester"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("lessthan", 
				"The lessthan of bullet heightfieldcollision tester.",
				Ogre::PT_REAL),&ms_lessThanCmd);
			dict->addParameter(Ogre::ParameterDef("output", 
				"The output of bullet Tester.",
				Ogre::PT_STRING),&ms_outputCmd);	
		}
	}
	BulletHeightFieldCollisionTester::~BulletHeightFieldCollisionTester()
	{
	}
	void BulletHeightFieldCollisionTester::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletTester::copyParameters(newOperator);
	
		BulletHeightFieldCollisionTester* newHeightFieldTester =  dynamic_cast<BulletHeightFieldCollisionTester*>(&newOperator);
		if(newHeightFieldTester)
		{
			newHeightFieldTester->m_lessThan = m_lessThan;
		}
    }
	void BulletHeightFieldCollisionTester::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it = 
			m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem)
			{
				Real fHeight =pBulletSystem->getHeightToTerrain();
				if(fHeight < m_lessThan)
				{
					m_parent->addTransition(m_outputEventName,pBulletSystem);
				}
			}
			it ++;

		}
	}
	String BulletHeightFieldCollisionTester::CmdLessThan::doGet(const void* source) const
	{
		const BulletHeightFieldCollisionTester* object = static_cast<const BulletHeightFieldCollisionTester*>(source);
		return Ogre::StringConverter::toString(object->getLessThan());
	}
	void BulletHeightFieldCollisionTester::CmdLessThan::doSet(void* target, const String& val)
	{
		BulletHeightFieldCollisionTester* object = static_cast<BulletHeightFieldCollisionTester*>(target);
		object->setLessThan(Ogre::StringConverter::parseReal(val));
	}
	
} 