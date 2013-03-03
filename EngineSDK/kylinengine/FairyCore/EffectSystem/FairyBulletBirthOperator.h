/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:42
	file base:	FairyBulletBirthOperator
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletBirthOperator_h__
#define FairyBulletBirthOperator_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletBirthOperator : public BulletOperator
	{
	public:
		BulletBirthOperator(BulletEventSystem* eventSystem);
		virtual ~BulletBirthOperator();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdBulletSystemTemplateName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);

		};

		class CmdBirthTimes : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		const Fairy::String&	getBulletSystemName() const { return m_bulletSystemName;}
		void					setBulletSystemName(const Fairy::String& val) { m_bulletSystemName = val;}

		float					getBirthTimes() const { return m_birthTimes; }
		void					setBirthTimes(float val) { m_birthTimes = val;}

		static CmdBulletSystemTemplateName	ms_bulletTemplateCmd;
		static CmdBirthTimes				 ms_birthTimesCmd;
	protected:
		String				m_bulletSystemName;
		float				m_birthTimes;
		float				m_birthedTimes;
		bool				m_isOperated;
	private:
	};
}
#endif // FairyBulletBirthOperator_h__