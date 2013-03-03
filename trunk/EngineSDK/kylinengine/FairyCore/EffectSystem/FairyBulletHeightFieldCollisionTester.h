/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:45
	file base:	FairyBulletHeightFieldCollisionTester
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletHeightFieldCollisionTester_h__
#define FairyBulletHeightFieldCollisionTester_h__

#include "FairyBulletTester.h"
#include "Core/FairyPrerequisites.h"
#include "OgreSceneQuery.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletHeightFieldCollisionTester : public BulletTester
	{
	public:
		BulletHeightFieldCollisionTester(BulletEventSystem* eventSystem);
		virtual ~BulletHeightFieldCollisionTester();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdLessThan : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		const Fairy::Real& getLessThan() const {return m_lessThan;}
		void setLessThan(const Fairy::Real& val) {m_lessThan = val;}

		static CmdLessThan ms_lessThanCmd;

	protected:
		Real					m_lessThan;
		Ogre::RaySceneQuery*	m_rayQuery;
	private:
	};
}
#endif // FairyBulletHeightFieldCollisionTester_h__