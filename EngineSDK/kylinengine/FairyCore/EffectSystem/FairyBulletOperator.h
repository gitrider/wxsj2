/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:43
	file base:	FairyBulletOperator
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletOperator_h__
#define FairyBulletOperator_h__

#include "OgreStringInterface.h"
#include "Core/FairyPrerequisites.h"
#include "FairyBulletEventSystem.h"
namespace Fairy
{
	class BulletEventSystem;

	class BulletOperator : public Ogre::StringInterface
	{
	public:
		virtual void operateBulletEventSystem(Real timeElapsed) = 0;

		const Fairy::String& getType() const { return m_type; }

		bool				isLogicOperator() const { return m_isLogicOperator;}
		bool				isActive()const { return m_active;}
		float				getStartTime()const {return m_startTime;}
		float				getAge()const {return m_age;}

		BulletEventSystem*	getParent() {return m_parent;}

		virtual void copyParameters(BulletOperator& newOperator) const = 0;

		virtual ~BulletOperator() {}
	protected:
		BulletOperator() : m_type(""),m_parent(NULL),m_isLogicOperator(false),m_startTime(0.0f)
		,m_age(0.0f),m_active(false){}

	protected:
		String					m_type;
		BulletEventSystem*		m_parent;
		bool					m_isLogicOperator;
		float					m_startTime;
		float					m_age;
		bool					m_active;
	private:
	};
}
#endif // FairyBulletOperator_h__