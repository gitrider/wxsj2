/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:45
	file base:	FairyBulletScaleTesterFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletScaleTesterFactory_h__
#define FairyBulletScaleTesterFactory_h__

#include "FairyBulletOperatorFactory.h"
#include "FairyBulletScaleTester.h"

namespace Fairy
{
	class BulletScaleTesterFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "scale";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletScaleTester* tmpOperator = new BulletScaleTester(eventSystem);
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
#endif // FairyBulletScaleTesterFactory_h__