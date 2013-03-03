/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:46
	file base:	FairyBulletTester
	file ext:	h
	author:		½ªÍþ
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletTester_h__
#define FairyBulletTester_h__

#include "FairyBulletOperator.h"
#include "Core/FairyPrerequisites.h"

namespace Fairy
{
	class BulletEventSystem;

	class BulletTester : public BulletOperator
	{
	public:
		virtual ~BulletTester();

		virtual void copyParameters(BulletOperator& newOperator) const;

		class CmdOutput : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		const Fairy::String& getOutputEventName() const{return m_outputEventName;}
		void setOutputEventName(const Fairy::String& val){m_outputEventName = val;}

		bool				initBaseParamters(const String& type);

		static CmdOutput	ms_outputCmd;

	protected:
		BulletTester();
	protected:
		String				m_outputEventName;

	};
}
#endif // FairyBulletTester_h__