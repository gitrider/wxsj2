///////////////////////////////////////////////////////////////////////////////
// �ļ�����MingJiaoSkill002.cpp
// ����Ա������
// ����˵�������̼��ܺ������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MingJiaoSkill002.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class MingJiaoSkill002_T : public SkillLogic_T
		BOOL MingJiaoSkill002_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nRate = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);
			OWN_IMPACT impact;

			CombatCore_T myCombatCore;
			myCombatCore.Reset();			
			INT nAdditionalAttack = Float2Int((rParams.GetDepletedRage()* nRate)/100.0f);
			INT nRefixRate = rSkillInfo.GetPowerRefixByRate();
			nRefixRate += 100;
			nAdditionalAttack = Float2Int(nAdditionalAttack*nRefixRate/100.0f);
			myCombatCore.SetAdditionalAttackNear(nAdditionalAttack);
			myCombatCore.GetResultImpact(rMe, rTar, impact);

			// Register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

