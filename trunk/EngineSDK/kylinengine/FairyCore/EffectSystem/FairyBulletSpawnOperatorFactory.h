/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:43
	file base:	FairyBulletSpawnOperatorFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletSpawnOperatorFactory_h__
#define FairyBulletSpawnOperatorFactory_h__

#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyBulletSpawnOperator.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"

namespace Fairy
{
	class BulletSpawnOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "spawn";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletSpawnOperator* tmpOperator = new BulletSpawnOperator(eventSystem);
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
#endif // FairyBulletSpawnOperatorFactory_h__