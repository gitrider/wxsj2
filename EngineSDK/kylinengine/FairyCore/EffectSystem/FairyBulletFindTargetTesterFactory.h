#ifndef FairyBulletFindTargetTesterFactory_h__
#define FairyBulletFindTargetTesterFactory_h__

#include "FairyBulletOperatorFactory.h"
#include "FairyBulletFindTargetTester.h"

namespace Fairy
{
	class BulletFindTargetTesterFactory : public BulletOperatorFactory
	{
	public:
		virtual String getName() const
		{
			return "findtarget";
		}

		virtual BulletOperator*	createOperator(BulletEventSystem* eventSystem)
		{
			BulletFindTargetTester* tmpOperator = new BulletFindTargetTester(eventSystem);
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
#endif // FairyBulletFindTargetTesterFactory_h__