#include "FairyBulletScaleTester.h"

#include <OgreStringConverter.h>
namespace Fairy
{
	BulletScaleTester::CmdMaxScale			BulletScaleTester::ms_maxScaleCmd;


	BulletScaleTester::BulletScaleTester(BulletEventSystem* eventSystem) : BulletTester()
	{
		m_type = "scale";
		m_parent = eventSystem;

		m_maxScale = 0.0;

		if (createParamDictionary("BulletScaleTester"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("maxScale", 
				"The maxScale of bullet scale tester.",
				Ogre::PT_REAL),&ms_maxScaleCmd);
			dict->addParameter(Ogre::ParameterDef("output", 
				"The output of bullet Tester.",
				Ogre::PT_STRING),&ms_outputCmd);	
		}
	}
	BulletScaleTester::~BulletScaleTester()
	{
	}
	void BulletScaleTester::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletTester::copyParameters(newOperator);
	
		BulletScaleTester* newScaleTester =  dynamic_cast<BulletScaleTester*>(&newOperator);
		if(newScaleTester)
		{
			newScaleTester->m_maxScale = m_maxScale;
		}
    }
	void BulletScaleTester::operateBulletEventSystem(Real timeElapsed)
	{
	}
	String BulletScaleTester::CmdMaxScale::doGet(const void* source) const
	{
		const BulletScaleTester* object = static_cast<const BulletScaleTester*>(source);
		return Ogre::StringConverter::toString(object->getMaxScale());
	}
	void BulletScaleTester::CmdMaxScale::doSet(void* target, const String& val)
	{
		BulletScaleTester* object = static_cast<BulletScaleTester*>(target);
		object->setMaxScale(Ogre::StringConverter::parseReal(val));
	}
	
} 