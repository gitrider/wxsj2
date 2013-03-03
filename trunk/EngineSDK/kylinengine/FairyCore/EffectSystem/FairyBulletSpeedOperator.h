/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:44
	file base:	FairyBulletSpeedOperator
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletSpeedOperator_h__
#define FairyBulletSpeedOperator_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"
#include "OgreVector3.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletSpeedOperator : public BulletOperator
	{
	public:
		BulletSpeedOperator(BulletEventSystem* eventSystem);
		virtual ~BulletSpeedOperator();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdMode : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* source) const;
			void doSet(void* target, const String& val);
		};

		class CmdDirection : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* source) const;
			void doSet(void* target, const String& val);
		};

		class CmdSpeed : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* source) const;
			void doSet(void* target, const String& val);
		};

		const Fairy::String&		getMode() const {return m_mode;}
		void						setMode(const Fairy::String& val) {m_mode = val;}

		const Ogre::Vector3&		getDirection() const {return m_direction;}
		void						setDirection(const Ogre::Vector3& val){m_direction = val;}

		const Fairy::Real&			getSpeed() const { return m_speed; }
		void						setSpeed(const Fairy::Real& val) {m_speed = val;}

		static CmdMode			ms_modeCmd;
		static CmdDirection		ms_directionCmd;
		static CmdSpeed			ms_speedCmd;
	protected:
		String				m_mode;
		Ogre::Vector3		m_direction;
		Real				m_speed;
		bool				m_isOperated;
	private:
	};
}
#endif // FairyBulletSpeedOperator_h__