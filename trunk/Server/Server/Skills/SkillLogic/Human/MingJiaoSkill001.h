///////////////////////////////////////////////////////////////////////////////
// �ļ�����MingJiaoSkill001.h
// ����Ա������
// ����˵�������̼���Ǩŭ����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MINGJIAOSKILL001_H
#define MINGJIAOSKILL001_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class MingJiaoSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = MING_JIAO_SKILL_001,
				};
				// public interface
				MingJiaoSkill001_T(){};
				~MingJiaoSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
				BOOL IsSpecificTargetWanted(Obj_Character& rMe, Obj_Character& rTar, SkillInfo_T const& rSkillInfo) const;
			protected: //interface for interval using
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //MINGJIAOSKILL001_H

