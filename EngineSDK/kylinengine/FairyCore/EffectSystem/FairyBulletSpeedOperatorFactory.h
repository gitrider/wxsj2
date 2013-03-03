/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:44
	file base:	FairyBulletSpeedOperatorFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletSpeedOperatorFactory_h__
#define FairyBulletSpeedOperatorFactory_h__


#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyBulletSpeedOperator.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"

namespace Fairy
{
	class BulletSpeedOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "speed";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletSpeedOperator* tmpOperator = new BulletSpeedOperator(eventSystem);
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
#endif // FairyBulletSpeedOperatorFactory_h__