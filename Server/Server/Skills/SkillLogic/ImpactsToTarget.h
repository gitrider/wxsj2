///////////////////////////////////////////////////////////////////////////////
// 文件名：ImpactsToTarget.h
// 程序员：高骐
// 功能说明：基础技能-只产生(特定效果)， 不分敌我，AE时对任何人都有效
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef IMPACTTOTARGET_H
#define IMPACTTOTARGET_H

#include "Type.h"
#include "SkillLogic.h"
#include "StdImpact003.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class ImpactsToTarget_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = IMPACTS_TO_TARGET,
				};
				enum
				{
					IMPACT_NUMBER = 6,
				};
				class SkillInfoDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_ONCE_IMPACT =0,
							IDX_ACTIVATE_EACH_TICK_IMPACT,
						};
				};
				// public interface
				ImpactsToTarget_T(){};
				~ImpactsToTarget_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
				// Accessor
				INT GetActivateOnceImpactByIndex(SkillInfo_T const& rSkill, INT nIndex) const;
				INT GetActivateEachTickImpactByIndex(SkillInfo_T const& rSkill, INT nIndex) const;
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;

				VOID ComputeCriticalDamages( OWN_IMPACT &impact, DI_DamagesByValue_T &logic, Obj_Character& rMe, Obj_Character& rTar ) const;
				INT GetCriticalDamageRefix( Obj_Character &rMe, Obj_Character &rTar, INT iTmp ) const;
				BOOL EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //IMPACTTOTARGET_H
