/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:43
	file base:	FairyBulletForceOperator
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletForceOperator_h__
#define FairyBulletForceOperator_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"
#include "OgreVector3.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletForceOperator : public BulletOperator
	{
	public:
		BulletForceOperator(BulletEventSystem* eventSystem);
		virtual ~BulletForceOperator();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdForceVector : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		const Ogre::Vector3& getForceVector() const { return m_forceVector; }
		void setForceVector(const Ogre::Vector3& val) { m_forceVector = val; }

		static CmdForceVector ms_forceVectorCmd;
	protected:
		bool				m_isOperated;
		Ogre::Vector3		m_forceVector;

	private:
	};
}
#endif // FairyBulletForceOperator_h__