/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:42
	file base:	FairyBulletDeleterOperator
	file ext:	h
	author:		姜威
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletDeleterOperator_h__
#define FairyBulletDeleterOperator_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"
#include "OgreVector3.h"

namespace Fairy
{
	class BulletEventSystem;
	class BulletDeleterOperator : public BulletOperator
	{
	public:
		BulletDeleterOperator(BulletEventSystem* eventSystem);
		virtual ~BulletDeleterOperator();

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdMaturity : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* source) const;
			void doSet(void* target, const String& val);
		};

		const Fairy::Real&			getMaturity() const { return m_maturity; }
		void						setMaturity(const Fairy::Real& val) {m_maturity = val;}

		static CmdMaturity		ms_maturityCmd;

	protected:
		bool				m_isOperated;
		///  持续指定时间后删除
		Real	            m_maturity;

	private:
	};
}
#endif // FairyBulletDeleterOperator_h__