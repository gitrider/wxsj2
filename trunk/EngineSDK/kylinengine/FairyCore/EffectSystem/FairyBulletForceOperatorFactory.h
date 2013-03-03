/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:43
	file base:	FairyBulletForceOperatorFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletForceOperatorFactory_h__
#define FairyBulletForceOperatorFactory_h__

#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyBulletForceOperator.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"

namespace Fairy
{
	class BulletForceOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "force";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletForceOperator* tmpOperator = new BulletForceOperator(eventSystem);
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
#endif // FairyBulletForceOperatorFactory_h__