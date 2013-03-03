/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:45
	file base:	FairyBulletScriptTesterFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletScriptTesterFactory_h__
#define FairyBulletScriptTesterFactory_h__

#include "FairyBulletOperatorFactory.h"
#include "FairyBulletScriptTester.h"

namespace Fairy
{
	class BulletScriptTesterFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "script";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletScriptTester* tmpOperator = new BulletScriptTester(eventSystem);
			m_operators.push_back(tmpOperator);
			return tmpOperator;
		}
		virtual void destroyOperator(BulletOperator* bulletOperator)
		{
		}
	protected:
	private:
	};
}
#endif // FairyBulletScriptTesterFactory_h__