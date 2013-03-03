/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:43
	file base:	FairyBulletSpawnOperator
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletSpawnOperator_h__
#define FairyBulletSpawnOperator_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletSpawnOperator : public BulletOperator
	{
	public:
		BulletSpawnOperator(BulletEventSystem* eventSystem);
		virtual ~BulletSpawnOperator();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdBulletSystemTemplateName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);

		};

		class CmdIsDeleteSource : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		const Fairy::String&	getBulletSystemName() const { return m_bulletSystemName;}
		void					setBulletSystemName(const Fairy::String& val){ m_bulletSystemName = val;}

		bool					getIsDeleteSource() const {return m_isDeleteSource;}
		void					setIsDeleteSource(bool val) {m_isDeleteSource = val;}

		static CmdBulletSystemTemplateName	ms_bulletTemplateCmd;
		static CmdIsDeleteSource			ms_isDeleteSourceCmd;
	protected:
		String				m_bulletSystemName;
		bool				m_isDeleteSource;
		int					m_spawnedTimes;
		bool				m_isOperated;
	private:
	};
}
#endif // FairyBulletSpawnOperator_h__