/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:45
	file base:	FairyBulletHeightFieldCollisionTesterFactory
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletHeightFieldCollisionTesterFactory_h__
#define FairyBulletHeightFieldCollisionTesterFactory_h__

#include "FairyBulletOperatorFactory.h"
#include "FairyBulletHeightFieldCollisionTester.h"

namespace Fairy
{
	class BulletHeightFieldTesterFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "heightfieldcollision";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletHeightFieldCollisionTester* tmpOperator = new BulletHeightFieldCollisionTester(eventSystem);
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
#endif // FairyBulletHeightFieldCollisionTesterFactory_h__