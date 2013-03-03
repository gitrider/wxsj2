/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:44
	file base:	FairyBulletAgeTester
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletAgeTester_h__
#define FairyBulletAgeTester_h__

#include "FairyBulletTester.h"
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class BulletEventSystem;

	class BulletAgeTester : public BulletTester
	{
	public:
		BulletAgeTester(BulletEventSystem* eventSystem);
		virtual ~BulletAgeTester();


		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		const Fairy::Real& getMaxTime() const {return m_maxTime;}
		void setMaxTime(const Fairy::Real& val) {m_maxTime = val;}

		class CmdMaxTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		static CmdMaxTime	ms_maxTimeCmd;
	protected:
		Real	m_maxTime;
	private:
	};
}
#endif // FairyBulletAgeTester_h__