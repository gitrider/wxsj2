/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:44
	file base:	FairyBulletAgeTesterFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletAgeTesterFactory_h__
#define FairyBulletAgeTesterFactory_h__

#include "FairyBulletOperatorFactory.h"
#include "FairyBulletAgeTester.h"

namespace Fairy
{
	class BulletAgeTesterFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "age";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletAgeTester* tmpOperator = new BulletAgeTester(eventSystem);
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

#endif // FairyBulletAgeTesterFactory_h__