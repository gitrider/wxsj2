///////////////////////////////////////////////////////////////////////////////
// 文件名：SkillInfo.cpp
// 程序员：高骐
// 功能说明：角色身上技能实例
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SkillInfo.h"
#include "GameTable.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;
	
	Skill_Module::SkillTemplateData_T const* GetSkillTemplateByID(INT nSkill)
	{
		__ENTER_FUNCTION
		return g_SkillTemplateDataMgr.GetInstanceByID(nSkill);
		__LEAVE_FUNCTION
		return NULL;
	}
	Skill_Module::SkillInstanceData_T const* GetSkillInstanceByID(INT nInstance)
	{
		__ENTER_FUNCTION
		return g_SkillDataMgr.GetInstanceByID(nInstance);
		__LEAVE_FUNCTION
		return NULL;
	}
	//class SkillInfo_T
	SkillInfo_T::SkillInfo_T():
		m_nSkillID(INVALID_ID),
		m_nInstanceID(INVALID_ID),
		m_nAccuracy(INVALID_ID),
		m_nCriticalRate(INVALID_ID),
		m_nDepleteRefixByValue(0),
		m_nDepleteRefixByRate(0),
		m_nPowerRefixByValue(0),
		m_nPowerRefixByRate(0),
		m_nTimeRefixByValue(0),
		m_nTimeRefixByRate(0),
		m_nCooldownTime(0),
		m_nChargeTime(0),
		m_nChannelTime(0),
		m_iSkillLevel(0),
		m_fMinSkillDistance(0),
		m_fMaxSkillDistance(0),
		m_ChargesOrInterval(1),
		m_fRadius(0.0),
		m_iMaxTargetNumber(1),
		m_iSkillLogicCount(INVALID_ID),
		m_iSendSkillImpactID(INVALID_ID)
	{
	};
	SkillInfo_T::SkillInfo_T(SkillInfo_T const& rhs) :
		m_nSkillID(rhs.m_nSkillID),
		m_nInstanceID(rhs.m_nInstanceID),
		m_nAccuracy(rhs.m_nAccuracy),
		m_nCriticalRate(rhs.m_nCriticalRate),
		m_nDepleteRefixByValue(rhs.m_nDepleteRefixByValue),
		m_nDepleteRefixByRate(rhs.m_nDepleteRefixByRate),
		m_nPowerRefixByValue(rhs.m_nPowerRefixByValue),
		m_nPowerRefixByRate(rhs.m_nPowerRefixByRate),
		m_nTimeRefixByValue(rhs.m_nTimeRefixByValue),
		m_nTimeRefixByRate(rhs.m_nTimeRefixByRate),
		m_nCooldownTime(rhs.m_nCooldownTime),
		m_nChargeTime(rhs.m_nChargeTime),
		m_nChannelTime(rhs.m_nChannelTime),
		m_iSkillLevel(rhs.m_nChannelTime),
		m_fMinSkillDistance(rhs.m_fMinSkillDistance),
		m_fMaxSkillDistance(rhs.m_fMaxSkillDistance),
		m_ChargesOrInterval(rhs.m_ChargesOrInterval),
		m_fRadius(rhs.m_fRadius),
		m_iMaxTargetNumber(rhs.m_iMaxTargetNumber),
		m_iSkillLogicCount(rhs.m_iSkillLogicCount),
		m_iSendSkillImpactID(rhs.m_iSendSkillImpactID)
	{
	};
	VOID SkillInfo_T::Init(VOID)
	{
		m_nSkillID					= INVALID_ID;
		m_nInstanceID				= INVALID_ID;
		m_nAccuracy					= INVALID_ID;
		m_nCriticalRate				= INVALID_ID;
		m_nDepleteRefixByValue		= 0;
		m_nDepleteRefixByRate		= 0;
		m_nPowerRefixByValue		= 0;
		m_nPowerRefixByRate			= 0;
		m_nTimeRefixByValue			= 0;
		m_nTimeRefixByRate			= 0;
		m_nCooldownTime				= 0;
		m_nChargeTime				= 0;
		m_nChannelTime				= 0;
		m_iSkillLevel				= 0;
		m_fMinSkillDistance			= 0;
		m_fMaxSkillDistance			= 0;
		m_ChargesOrInterval			= 1;
		m_fRadius					= 0.0;
		m_iMaxTargetNumber			= 1;
		m_iSkillLogicCount			= INVALID_ID;
		m_iSendSkillImpactID		= INVALID_ID;
	};
	SkillInfo_T& SkillInfo_T::operator=(SkillTemplateData_T const& rhs)
	{
		SetSkillID(rhs.GetSkillID());
		return *this;
	}
	SkillInfo_T& SkillInfo_T::operator=(SkillInstanceData_T const& rhs)
	{
		SetInstanceID(rhs.GetInstanceID());
		SetCooldownTime(rhs.GetCooldownTimeBySkillLevel(m_iSkillLevel));
		SetChargeTime(rhs.GetChargeTimeBySkillLevel(m_iSkillLevel));
		SetChannelTime(rhs.GetChannelTimeBySkillLevel(m_iSkillLevel));
		SetAccuracy(rhs.GetAccuracy(m_iSkillLevel));
		SetCriticalRate(rhs.GetCriticalRate(m_iSkillLevel));
		SetRadius(rhs.GetRadius(m_iSkillLevel));
		SetMaxTargetNumber(rhs.GetMaxTargetNumber(m_iSkillLevel));		
		return *this;
	};
	SkillInfo_T& SkillInfo_T::operator=(SkillInfo_T const& rhs)
	{
		m_nSkillID					= rhs.m_nSkillID;
		m_nInstanceID				= rhs.m_nInstanceID;
		m_nAccuracy					= rhs.m_nAccuracy;
		m_nCriticalRate				= rhs.m_nCriticalRate;
		m_nDepleteRefixByValue		= rhs.m_nDepleteRefixByValue;
		m_nDepleteRefixByRate		= rhs.m_nDepleteRefixByRate;
		m_nPowerRefixByValue		= rhs.m_nPowerRefixByValue;
		m_nPowerRefixByRate			= rhs.m_nPowerRefixByRate;
		m_nTimeRefixByValue			= rhs.m_nTimeRefixByValue;
		m_nTimeRefixByRate			= rhs.m_nTimeRefixByRate;
		m_nCooldownTime				= rhs.m_nCooldownTime;
		m_nChargeTime				= rhs.m_nChargeTime;
		m_nChannelTime				= rhs.m_nChannelTime;
		m_iSkillLevel				= rhs.m_iSkillLevel;
		m_fMinSkillDistance			= rhs.m_fMinSkillDistance;
		m_fMaxSkillDistance			= rhs.m_fMaxSkillDistance;
		m_ChargesOrInterval			= rhs.m_ChargesOrInterval;
		m_fRadius					= rhs.m_fRadius;
		m_iMaxTargetNumber			= rhs.m_iMaxTargetNumber;
		m_iSkillLogicCount			= rhs.m_iSkillLogicCount;
		m_iSendSkillImpactID		= rhs.m_iSendSkillImpactID;
		return *this;
	};
	MenPaiID_t SkillInfo_T::GetMenPai(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetMenPai();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};

	CHAR const* SkillInfo_T::GetName(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetName();
		}
		return NULL;
		__LEAVE_FUNCTION
		return NULL;
	};
	INT  SkillInfo_T::GetXinFaLevelRequirement(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetXinFaLevelRequirement();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	BOOL SkillInfo_T::GetMustUseWeaponFlag(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetMustUseWeaponFlag();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	BOOL SkillInfo_T::GetDisableByFlag1(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetDisableByFlag1();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	BOOL SkillInfo_T::GetDisableByFlag2(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetDisableByFlag2();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	BOOL SkillInfo_T::GetDisableByFlag3(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetDisableByFlag3();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;

	};
	XinFaID_t SkillInfo_T::GetXinFa(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetXinFa();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	INT  SkillInfo_T::GetXinFaParam(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetXinFaParam();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	INT  SkillInfo_T::GetRangedSkillFlag(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetRangedSkillFlag();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	BOOL SkillInfo_T::IsForceBreakPreSkill(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->ForceBreakPreSkill();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	INT  SkillInfo_T::GetTargetMustInSpecialState(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetTargetMustInSpecialState();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	ID_t SkillInfo_T::GetSkillType(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetSkillType();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	CooldownID_t SkillInfo_T::GetCooldownID(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetCooldownID();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	ID_t SkillInfo_T::GetSelectType(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetSelectType();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	ID_t SkillInfo_T::GetOperateModeForPetSkill(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetOperateModeForPetSkill();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	ID_t SkillInfo_T::GetPetRateOfSkill(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetPetRateOfSkill();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	ID_t SkillInfo_T::GetTypeOfPetSkill(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetTypeOfPetSkill();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	ID_t SkillInfo_T::GetImpactID(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetImpactIDOfSkill();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	ID_t SkillInfo_T::GetTargetingLogic(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetTargetingLogic();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	Time_t SkillInfo_T::GetPlayActionTime(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetPlayActionTime();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	FLOAT SkillInfo_T::GetOptimalRangeMin(VOID) const
	{
		__ENTER_FUNCTION
		//SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		//if(NULL!=pTemplate)
		//{
		//	return pTemplate->GetOptimalRangeMin() + GetMinSkillDistanceRefix();
		//}
		//return 0;
		return m_fMinSkillDistance;
		__LEAVE_FUNCTION
		return 0;
	};
	FLOAT SkillInfo_T::GetOptimalRangeMax(VOID) const
	{
		__ENTER_FUNCTION
		//SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		//if(NULL!=pTemplate)
		//{
		//	return pTemplate->GetOptimalRangeMax() + GetMaxSkillDistanceRefix();
		//}
		//return 0;
		return m_fMaxSkillDistance;
		__LEAVE_FUNCTION
		return 0;
	};
	ID_t SkillInfo_T::GetStandFlag(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetStandFlag();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	ID_t SkillInfo_T::GetTargetLogicByStand(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetTargetLogicByStand();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	INT SkillInfo_T::GetSpeedOfBullet( VOID ) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetSpeedOfBullet();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	}
	INT SkillInfo_T::GetHurtDelayTime( VOID ) const
	{
		__ENTER_FUNCTION
			SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetHurtDelayTime();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	}
	ID_t SkillInfo_T::GetTargetCheckByObjType(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetTargetCheckByObjType();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};

	BOOL SkillInfo_T::IsPartyOnly(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->IsPartyOnly();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	INT SkillInfo_T::GetChargesOrInterval(VOID) const
	{
		__ENTER_FUNCTION
		//SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		//if(NULL!=pTemplate)
		//{
		//	return pTemplate->GetChargesOrInterval();
		//}
		//return 1;
		return m_ChargesOrInterval;
		__LEAVE_FUNCTION
		return 1;
	};
	BOOL SkillInfo_T::IsAutoShotSkill(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->IsAutoShotSkill();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	BOOL SkillInfo_T::UseNormalAttackRate(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->UseNormalAttackRate();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	FLOAT SkillInfo_T::GetAngle(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetAngle();
		}
		return 0.0f;
		__LEAVE_FUNCTION
		return 0.0f;
	};
	INT SkillInfo_T::GetMaxTargetNumber(VOID) const
	{
		__ENTER_FUNCTION
		return m_iMaxTargetNumber;
		__LEAVE_FUNCTION
		return 0;
	};
	BOOL SkillInfo_T::CanInterruptAutoShot(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->CanInterruptAutoShot();
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	Time_t SkillInfo_T::GetDelayTime(VOID) const
	{
		__ENTER_FUNCTION
		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			return pTemplate->GetDelayTime();
		}
		return 0;
		__LEAVE_FUNCTION
		return 0;
	};
	ID_t SkillInfo_T::GetLogicID(VOID) const
	{
		__ENTER_FUNCTION
		SkillInstanceData_T const* pInstance = GetSkillInstanceByID(m_nInstanceID);
		if(NULL!=pInstance)
		{
			return pInstance->GetLogicID();
		}
		return INVALID_ID;
		__LEAVE_FUNCTION
		return INVALID_ID;
	};
	ConDepTerm_T const* SkillInfo_T::GetConDepTermByIndex(INT const nIdx) const
	{
		__ENTER_FUNCTION
		SkillInstanceData_T const* pInstance = GetSkillInstanceByID(m_nInstanceID);
		if(NULL!=pInstance)
		{
			return pInstance->GetConDepTermByIndex(nIdx);
		}
		return NULL;
		__LEAVE_FUNCTION
		return NULL;
	};
	INT SkillInfo_T::GetSkillLevel(VOID) const
	{
		__ENTER_FUNCTION
		SkillInstanceData_T const* pInstance = GetSkillInstanceByID(m_nInstanceID);
		if(NULL!=pInstance)
		{
			return pInstance->GetSkillLevel();
		}
		return NULL;
		__LEAVE_FUNCTION
		return NULL;
	};
	Descriptor_T const* SkillInfo_T::GetDescriptorByIndex(INT const nIdx) const
	{
		__ENTER_FUNCTION
		SkillInstanceData_T const* pInstance = GetSkillInstanceByID(m_nInstanceID);
		if(NULL!=pInstance)
		{
			return pInstance->GetDescriptorByIndex(nIdx);
		}
		return NULL;
		__LEAVE_FUNCTION
		return NULL;
	};

	VOID SkillInfo_T::SetSkillID( SkillID_t const nID )
	{
		m_nSkillID = nID;
		InitOptimalRangeMin();
		InitOptimalRangeMax();
		InitChargesOrInterval();
	}

	VOID SkillInfo_T::InitOptimalRangeMin()
	{
		__ENTER_FUNCTION
			SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			m_fMinSkillDistance = pTemplate->GetOptimalRangeMin();
		}
		__LEAVE_FUNCTION
	}

	VOID SkillInfo_T::InitOptimalRangeMax()
	{
		__ENTER_FUNCTION
			SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			m_fMaxSkillDistance = pTemplate->GetOptimalRangeMax();
		}
		__LEAVE_FUNCTION
	}

	VOID SkillInfo_T::InitChargesOrInterval()
	{
		__ENTER_FUNCTION

		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
		if(NULL!=pTemplate)
		{
			m_ChargesOrInterval = pTemplate->GetChargesOrInterval();
		}
		m_ChargesOrInterval = m_ChargesOrInterval>0 ? m_ChargesOrInterval : 1;
		__LEAVE_FUNCTION
	}

	//VOID SkillInfo_T::InitMaxTargetNumber()
	//{
	//	__ENTER_FUNCTION
	//		SkillTemplateData_T const* pTemplate = GetSkillTemplateByID(m_nSkillID);
	//	if(NULL!=pTemplate)
	//	{
	//		m_iMaxTargetNumber = pTemplate->GetMaxTargetNumber();
	//	}
	//	__LEAVE_FUNCTION
	//}

	VOID SkillInfo_T::SetMaxTargetNumber( INT val )
	{
		m_iMaxTargetNumber = val;
	}

};



