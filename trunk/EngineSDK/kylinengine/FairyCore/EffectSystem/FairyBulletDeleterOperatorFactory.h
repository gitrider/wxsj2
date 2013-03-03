/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:43
	file base:	FairyBulletDeleterOperatorFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletDeleterOperatorFactory_h__
#define FairyBulletDeleterOperatorFactory_h__

#include "FairyBulletOperatorFactory.h"
#include "FairyBulletDeleterOperator.h"

namespace Fairy
{
	class BulletDeleterOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "deleter";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletDeleterOperator* tmpOperator = new BulletDeleterOperator(eventSystem);
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
#endif // FairyBulletDeleterOperatorFactory_h__