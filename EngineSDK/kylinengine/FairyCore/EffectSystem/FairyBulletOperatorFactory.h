/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:43
	file base:	FairyBulletOperatorFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletOperatorFactory_h__
#define FairyBulletOperatorFactory_h__

#include "Core/FairyPrerequisites.h"
#include <vector>

namespace Fairy
{
	class BulletOperator;
	class BulletEventSystem;
	class BulletOperatorFactory
	{
	public:
		virtual String getName() const = 0;

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem) = 0;

		virtual void destroyOperator(BulletOperator* bulletOperator) = 0;

		virtual ~BulletOperatorFactory(){}

		BulletOperatorFactory(){}
	protected:
		std::vector<BulletOperator*> m_operators;
	};
}
#endif // FairyBulletOperatorFactory_h__