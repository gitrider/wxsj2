/********************************************************************
created:	2010/06/02
created:	6:2:2010   13:20
file base:	FairyBulletHelixOperatorFactory
file ext:	h
author:		³Â¾üÁú
email:		pizi0475@163.com
purpose:	
*********************************************************************/
#ifndef FairyBulletHelixOperatorFactory_h__
#define FairyBulletHelixOperatorFactory_h__

#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyBulletHelixOperator.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"

namespace Fairy
{
	class BulletHelixOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "helix";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletHelixOperator* tmpOperator = new BulletHelixOperator(eventSystem);
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
#endif // FairyBulletHelixOperatorFactory_h__