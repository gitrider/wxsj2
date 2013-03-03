#include "FairyBulletScriptTester.h"

#include <OgreStringConverter.h>
namespace Fairy
{

	BulletScriptTester::BulletScriptTester(BulletEventSystem* eventSystem) : BulletTester()
	{
		m_type = "script";
		m_parent = eventSystem;
		if (createParamDictionary("BulletScriptTester"))
        { 
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("output", 
				"The output of bullet Tester.",
				Ogre::PT_STRING),&ms_outputCmd);	
		}
	}
	BulletScriptTester::~BulletScriptTester()
	{
	}
	void BulletScriptTester::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletTester::copyParameters(newOperator);
	
    }
	void BulletScriptTester::operateBulletEventSystem(Real timeElapsed)
	{
	}
}