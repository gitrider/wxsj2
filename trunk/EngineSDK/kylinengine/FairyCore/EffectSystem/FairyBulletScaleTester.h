/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:45
	file base:	FairyBulletScaleTester
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletScaleTester_h__
#define FairyBulletScaleTester_h__

#include "FairyBulletTester.h"
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class BulletEventSystem;

	class BulletScaleTester : public BulletTester
	{
	public:
		BulletScaleTester(BulletEventSystem* eventSystem);
		virtual ~BulletScaleTester();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		const Fairy::Real& getMaxScale() const  { return m_maxScale; }
		void setMaxScale(const Fairy::Real& val) { m_maxScale = val; }

		class CmdMaxScale : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		static CmdMaxScale	ms_maxScaleCmd;
	protected:
		Real	m_maxScale;

	private:
	};
}
#endif // FairyBulletScaleTester_h__