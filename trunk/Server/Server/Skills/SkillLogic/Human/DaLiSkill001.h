///////////////////////////////////////////////////////////////////////////////
// �ļ�����DaLiSkill001.h
// ����Ա��frankwu
// ����˵����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DaLiSKILL001_H
#define DaLiSKILL001_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class DaLiSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = DA_LI_SKILL_001,
				};
				// public interface
				DaLiSkill001_T(){};
				~DaLiSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //DaLiSKILL001_H

