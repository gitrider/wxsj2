///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact007.cpp
// ����Ա������
// ����˵����Ч��--���ٷֱȻָ�����������Ϳ���ֵ
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact007.h"
#include "Obj_Pet.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact007_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetHpModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_HP,
				rImp.GetSkillLevel())
			);
			SetHappinessModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_HAPPINESS,
				rImp.GetSkillLevel())
			);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact007_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_PET!=rMe.GetObjType())
			{
				return;
			}
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			Obj_Pet& rPet = (Obj_Pet&)rMe;
			Obj_Character * pImpactSender = (Obj_Character*)Impact_GetCaster(rMe, rImp);
			//process HP
			INT nHP = GetHpModification(rImp);
			nHP += 100;
			nHP = Float2Int((nHP*rPet.GetBaseMaxHP())/100.0f);
			rPet.HealthIncrement(nHP, rImp.GetCasterLogicCount(), pImpactSender);
			//process Happyness
			INT nHappiness = GetHappinessModification(rImp);
			nHappiness += 100;
			nHappiness = Float2Int((nHappiness*PET_BASE_MAX_HAPPINESS)/100.0f);
			rPet.IncrementHappyness(nHappiness);
			__LEAVE_FUNCTION
		}
		BOOL StdImpact007_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += 100;
			SetHpModification(rImp, Float2Int((GetHpModification(rImp)*nRate)/100.0f));
			SetHappinessModification(rImp, Float2Int((GetHappinessModification(rImp)*nRate)/100.0f));
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

