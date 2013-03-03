#include "FairyBulletTester.h"

#include <OgreStringConverter.h>
namespace Fairy
{
	BulletTester::CmdOutput	BulletTester::ms_outputCmd;



	BulletTester::BulletTester() : BulletOperator(),m_outputEventName("")
	{
	}
	BulletTester::~BulletTester()
	{
	}
	void BulletTester::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletTester* newTester =  dynamic_cast<BulletTester*>(&newOperator);
		if(newTester)
		{
			newTester->m_outputEventName = m_outputEventName;
		}
    }
	String BulletTester::CmdOutput::doGet(const void* source) const
	{
		const BulletTester* object = static_cast<const BulletTester*>(source);
		return object->getOutputEventName();
	}
	void BulletTester::CmdOutput::doSet(void* target, const String& val)
	{
		BulletTester* object = static_cast<BulletTester*>(target);
		object->setOutputEventName(val);
	}

}