/********************************************************************
created:	2010/06/02
created:	6:2:2010   13:20
file base:	FairyBulletSwayOperatorFactory
file ext:	h
author:		³Â¾üÁú
email:		pizi0475@163.com
purpose:	
*********************************************************************/
#ifndef FairyBulletSwayOperatorFactory_h__
#define FairyBulletSwayOperatorFactory_h__

#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyBulletSwayOperator.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"

namespace Fairy
{
	class BulletSwayOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "sway";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletSwayOperator* tmpOperator = new BulletSwayOperator(eventSystem);
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
#endif // FairyBulletSwayOperatorFactory_h__