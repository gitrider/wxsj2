/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:42
	file base:	FairyBulletBirthOperatorFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletBirthOperatorFactory_h__
#define FairyBulletBirthOperatorFactory_h__

#include "Core/FairyPrerequisites.h"
#include "EffectSystem/FairyBulletBirthOperator.h"
#include "EffectSystem/FairyBulletOperatorFactory.h"

namespace Fairy
{
	class BulletBirthOperatorFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "birth";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletBirthOperator* tmpOperator = new BulletBirthOperator(eventSystem);
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
#endif // FairyBulletBirthOperatorFactory_h__