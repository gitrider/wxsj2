///////////////////////////////////////////////////////////////////////////////
// �ļ�����GaiBangSkill004.cpp
// ����Ա������
// ����˵����ؤ�＼�ܴ���չ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Type.h"
#include "Obj_Character.h"
#include "GaiBangSkill004.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "DI_Damages.h"
#include "LogicManager.h"
#include "ScanOperator_AESkill.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class GaiBangSkill004_T : public SkillLogic_T
		//Skill Refix interfaces
		VOID GaiBangSkill004_T::RefixAttackPhyByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			SkillInstanceData_T::Descriptor_T& rDescriptor0 = (SkillInstanceData_T::Descriptor_T&)*(rSkill.GetDescriptorByIndex(0));
			rDescriptor0.SetValue(rDescriptor0.GetValue()*(100+nRate)/100);
			SkillInstanceData_T::Descriptor_T& rDescriptor1 = (SkillInstanceData_T::Descriptor_T&)*(rSkill.GetDescriptorByIndex(1));
			rDescriptor1.SetValue(rDescriptor1.GetValue()*(100+nRate)/100);
			SkillInstanceData_T::Descriptor_T& rDescriptor2 = (SkillInstanceData_T::Descriptor_T&)*(rSkill.GetDescriptorByIndex(2));
			rDescriptor2.SetValue(rDescriptor2.GetValue()*(100+nRate)/100);
			__LEAVE_FUNCTION
		}
		BOOL GaiBangSkill004_T::IsScanedTargetValid(Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(FALSE==rTar.IsAliveInDeed())
			{
				return FALSE;
			}
			return TRUE==rMe.IsEnemy(&rTar);
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL GaiBangSkill004_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			INT nDepleteStrikePointSegment = rParams.GetDepletedStrikePoints()/STRIKE_POINT_SEGMENT_SIZE;
			if(1>nDepleteStrikePointSegment)
			{
				AssertEx(FALSE,"[GaiBangSkill004_T::TakeEffectNow]: Depleted strike point segment illegal!");
				nDepleteStrikePointSegment=1;
			}
			else if(MAX_STRIKE_POINT_SEGMENT<nDepleteStrikePointSegment)
			{
				AssertEx(FALSE,"[GaiBangSkill004_T::TakeEffectNow]: Depleted strike point segment illegal!");
				nDepleteStrikePointSegment=MAX_STRIKE_POINT_SEGMENT;
			}
			INT nAdditionalAttackPower = rSkillInfo.GetDescriptorByIndex(nDepleteStrikePointSegment)->GetValue();
			// init impact1
			OWN_IMPACT impact;
			
			CombatCore_T myCombatCore;
			myCombatCore.Reset();			
			myCombatCore.SetAdditionalAttackLight(nAdditionalAttackPower);
			myCombatCore.GetResultImpact(rMe, rTar, impact);

			// register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};





