///////////////////////////////////////////////////////////////////////////////
// �ļ�����TianShanSkill001.cpp
// ����Ա��frankwu
// ����˵����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TianShanSkill001.h"
#include "StdImpact051.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class TianShanSkill001_T : public SkillLogic_T
		//Skill Refix interfaces		
		BOOL TianShanSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			OWN_IMPACT impact;
			
			SOT_ShadowGuard_T logic;
			INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);

			g_ImpactCore.InitImpactFromData(nDataIndex, impact, rMe);
			INT nGuardDataID = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 1);
			//Call ����ӿ�,Ŀǰд������
			Obj_Character* pGuard = rMe.CreateGuard(nGuardDataID);
			if(NULL==pGuard)
			{
				AssertEx(FALSE,"[TianShanSkill001_T::TakeEffectNow]:Can't Create Guard. check now.");
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			logic.SetSummonerID(impact, rMe.GetID());
			logic.SetShadowGuardID(impact, pGuard->GetID());

			//register impact event
			RegisterImpactEvent(rMe, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			RegisterImpactEvent(*pGuard, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
