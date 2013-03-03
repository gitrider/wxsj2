/********************************************************************
	created:	2008/03/17
	created:	17:3:2008   16:54
	file base:	FairyBulletCameraShakeOperatorFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletCameraShakeOperatorFactory_h__
#define FairyBulletCameraShakeOperatorFactory_h__

#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyBulletCameraShakeOperator.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"

namespace Fairy
{
	class BulletCameraShakeOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "camerashake";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletCameraShakeOperator* tmpOperator = new BulletCameraShakeOperator(eventSystem);
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
#endif // FairyBulletCameraShakeOperatorFactory_h__