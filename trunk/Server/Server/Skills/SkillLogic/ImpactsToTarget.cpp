///////////////////////////////////////////////////////////////////////////////
// 文件名：ImpactsToTarget.cpp
// 程序员：高骐
// 功能说明：基础技能-只产生(特定效果)， 不分敌我，AE时对任何人都有效
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImpactsToTarget.h"
#include "Combat_Core.h"
#include "Impact_Core.h"
#include "StdImpact003.h"
#include "StdImpact061.h"
#include "Config.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		//class ImpactsToTarget_T : public SkillLogic_T
		BOOL ImpactsToTarget_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			//获得角色身上的技能实例
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			//获得角色身上的目标及伤害参数对象
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			//Send Activate once impacts
			for(INT nIdx=0; IMPACT_NUMBER>nIdx; ++nIdx)
			{
				//根据impact索引，找到Descriptor_T对象，从Descriptor_T中获取impact对象索引值
				nValue = GetActivateOnceImpactByIndex(rSkillInfo, nIdx);
				//如果索引值合法
				if(INVALID_ID != nValue)
				{
					//根据技能索引值nValue找到impact对象实例，用此实例初始化impact
					//获取ImpactLogic_T实例执行ImpactLogic_T::InitFromData
					//每个ImpactLogic_T实例会有不同的初始化
					g_ImpactCore.InitImpactFromData(nValue, impact, rMe);
					//如果ImpactID为STD_IMPACT_003
					if(DI_DamagesByValue_T::ID == Impact_GetLogicID(impact) ||
						DI_DamagesByRate_T::ID == Impact_GetLogicID(impact))
					{
						DI_DamagesByValue_T logic;
						if(TRUE==bCriticalFlag)
						{
							impact.MarkCriticalFlag();
						}
						//设置能量修正
						logic.RefixPowerByRate(impact, rSkillInfo.GetPowerRefixByRate());
						//如果是会心攻击则按会心修正rImp中的效果参数
						ComputeCriticalDamages(impact, logic, rMe, rTar);

						CombatCore_T myCombatCore;
						myCombatCore.Reset();
						//进一步初始化impact
						myCombatCore.GetResultImpact(rMe, rTar, impact);
					}
					//使效果生效
					RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL ImpactsToTarget_T::EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			//Send Activate once impacts
			for(INT nIdx=0; IMPACT_NUMBER>nIdx; ++nIdx)
			{
				nValue = GetActivateEachTickImpactByIndex(rSkillInfo, nIdx);
				if(INVALID_ID != nValue)
				{
					g_ImpactCore.InitImpactFromData(nValue, impact, rMe);
					if(DI_DamagesByValue_T::ID == Impact_GetLogicID(impact) ||
						DI_DamagesByRate_T::ID == Impact_GetLogicID(impact))
					{
						DI_DamagesByValue_T logic;
						if(TRUE==bCriticalFlag)
						{
							impact.MarkCriticalFlag();
						}
						logic.RefixPowerByRate(impact, rSkillInfo.GetPowerRefixByRate());
						ComputeCriticalDamages(impact, logic, rMe, rTar);
						CombatCore_T myCombatCore;
						myCombatCore.Reset();
						myCombatCore.GetResultImpact(rMe, rTar, impact);
					}
					RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		// Activate once impacts
		INT ImpactsToTarget_T::GetActivateOnceImpactByIndex(SkillInfo_T const& rSkill, INT nIndex) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_ONCE_IMPACT+2*nIndex);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate each tick impacts
		INT ImpactsToTarget_T::GetActivateEachTickImpactByIndex(SkillInfo_T const& rSkill, INT nIndex) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_EACH_TICK_IMPACT+2*nIndex);
			__LEAVE_FUNCTION
			return 0;
		}

		INT ImpactsToTarget_T::GetCriticalDamageRefix( Obj_Character &rMe, Obj_Character &rTar, INT iTmp ) const
		{
			__ENTER_FUNCTION

			INT iCriticalDamageRate = 0;
			if( rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN )
			{
				iCriticalDamageRate = ((Obj_Human*)&rMe)->GetEquipCriticalHurt();
			}

			iCriticalDamageRate = (g_Config.m_ConfigInfo.m_nCriticalDamageRate + iCriticalDamageRate);
			INT iTargetC		= g_Config.m_ConfigInfo.m_nToughEffCriticalDamage;
			INT iTargetCritical = rTar.GetToughness();
			if( iTargetC < 1 )
			{
				iTargetC = 1;
			}
			iTargetCritical = iTargetCritical * 100 / iTargetC;
			
			INT iFinal = (iCriticalDamageRate - iTargetCritical);
			if( iFinal < 1 )
			{
				iFinal = 1;
			}
			iFinal = iTmp * iFinal / 100;
			return iFinal;
			__LEAVE_FUNCTION
			return 0;
		}

		VOID ImpactsToTarget_T::ComputeCriticalDamages( OWN_IMPACT &impact, DI_DamagesByValue_T &logic, Obj_Character& rMe, Obj_Character& rTar ) const
		{
			if(impact.IsCriticalHit())
			{
				logic.CriticalRefix(impact);
				INT iTmp = rMe.GetAttackNear();// logic.GetDamageNear( impact );
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageNear( impact, iTmp );

				iTmp = rMe.GetAttackFar();//logic.GetDamageFar( impact );
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageFar( impact, iTmp );

				iTmp = rMe.GetAttackMagicNear();
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageMagicNear( impact, iTmp );

				iTmp = rMe.GetAttackMagicFar();
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageMagicFar( impact, iTmp );

				iTmp = rMe.GetAttackGold();//rMe.GetDamageGold( impact );
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageGold( impact, iTmp );

				iTmp = rMe.GetAttackWood();//logic.GetDamageWood( impact );
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageWood( impact, iTmp );

				iTmp = rMe.GetAttackWater();//logic.GetDamageWater( impact );
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageWater( impact, iTmp );

				iTmp = rMe.GetAttackFire();//rMe.GetDamageFire( impact );
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageFire( impact, iTmp );

				iTmp = rMe.GetAttackSoil();
				iTmp = GetCriticalDamageRefix(rMe, rTar, iTmp);
				logic.SetDamageSoil( impact, iTmp );
			}
		}
	};
};

