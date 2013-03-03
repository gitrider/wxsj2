/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:45
	file base:	FairyBulletScriptTester
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletScriptTester_h__
#define FairyBulletScriptTester_h__

#include "FairyBulletTester.h"
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class BulletEventSystem;

	class BulletScriptTester : public BulletTester
	{
	public:
		BulletScriptTester(BulletEventSystem* eventSystem);
		virtual ~BulletScriptTester();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

	protected:

	private:
	};
}
#endif // FairyBulletScriptTester_h__