/********************************************************************
	created:	2008/05/21
	created:	21:5:2008   17:31
	filename: 	MapServer\Server\Skills\SkillLogic\ImpactsToTargetAndSelfForRun.cpp
	file path:	MapServer\Server\Skills\SkillLogic
	file base:	ImpactsToTargetAndSelfForRun
	file ext:	cpp
	author:		Richard
	
	purpose:	为冲锋技能专用，同时给自己和目标各加一个impact
*********************************************************************/

#include "stdafx.h"
#include "ImpactsToTargetAndSelfForRun.h"
#include "Impact_Core.h"
#include "Combat_Core.h"
#include "StdImpact003.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class ImpactsToTargetAndSelf_T : public SkillLogic_T
		BOOL ImpactsToTargetAndSelfForRun_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			//rMe.setWorldPos( rTar.getWorldPos() );
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			//向自己注册效果
			nValue = GetActivateOnceImpactForSelf(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact, rMe);
				RegisterImpactEvent(rMe, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			//向目标注册效果
			nValue = GetActivateOnceImpactForTarget(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact, rMe);
				if(DI_DamagesByValue_T::ID == Impact_GetLogicID(impact))
				{
					DI_DamagesByValue_T logic;
					logic.RefixPowerByRate(impact, rSkillInfo.GetPowerRefixByRate());
					CombatCore_T myCombatCore;
					myCombatCore.Reset();
					myCombatCore.GetResultImpact(rMe, rTar, impact);
				}
				RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL ImpactsToTargetAndSelfForRun_T::EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			nValue = GetActivateEachTickImpactForSelf(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact, rMe);
				RegisterImpactEvent(rMe, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			nValue = GetActivateEachTickImpactForTarget(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact, rMe);
				if(DI_DamagesByValue_T::ID == Impact_GetLogicID(impact))
				{
					DI_DamagesByValue_T logic;
					logic.RefixPowerByRate(impact, rSkillInfo.GetPowerRefixByRate());
					CombatCore_T myCombatCore;
					myCombatCore.Reset();
					myCombatCore.GetResultImpact(rMe, rTar, impact);
				}
				RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		// Activate once impacts
		INT ImpactsToTargetAndSelfForRun_T::GetActivateOnceImpactForSelf(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_ONCE_IMPACT_FOR_SELF);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate each tick impacts
		INT ImpactsToTargetAndSelfForRun_T::GetActivateEachTickImpactForSelf(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_SELF);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate once impacts
		INT ImpactsToTargetAndSelfForRun_T::GetActivateOnceImpactForTarget(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_ONCE_IMPACT_FOR_TARGET);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate each tick impacts
		INT ImpactsToTargetAndSelfForRun_T::GetActivateEachTickImpactForTarget(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_TARGET);
			__LEAVE_FUNCTION
			return 0;
		}
	};
};
