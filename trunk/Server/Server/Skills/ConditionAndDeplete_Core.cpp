///////////////////////////////////////////////////////////////////////////////
// 文件名：ConditionAndDeplete_Core.cpp
// 程序员：高骐
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConditionAndDeplete_Core.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"
#include "LogDefine.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	ConditionAndDepleteCore_T g_ConditionAndDepleteCore;

	//class CDCore_T
	BOOL CDCore_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		INT nType = rData.GetType();
		switch(nType)
		{
			case C_UnitHaveImpact_T::ID :
				{
					return m_C_UnitHaveImpact.ConditionCheck(rMe, rData);
				}
				break;
			case C_UnitHpLessThanByRate_T::ID :
				{
					return m_C_UnitHpLessThanByRate.ConditionCheck(rMe, rData);
				}
				break;	
			case C_UnitHpMoreThanByRate_T::ID :
				{
					return m_C_UnitHpMoreThanByRate.ConditionCheck( rMe, rData );
				}
			case C_TargetMustHaveImpact_T::ID :
				{
					return m_C_TargetMustHaveImpact.ConditionCheck(rMe, rData);
				}
				break;
			case C_TargetHpLessThanByRate_T::ID :
				{
					return m_C_TargetHpLessThanByRate.ConditionCheck( rMe, rData );
				}
			case C_TargetHpMoreThanByRate_T::ID :
				{
					return m_C_TargetHpMoreThanByRate.ConditionCheck( rMe, rData );
				}
			case CD_HpByValue_T::ID :
				{
					return m_CD_HpByValue.ConditionCheck( rMe, rData );
				}
			case CD_HpByRate_T::ID :
				{
					return m_CD_HpByRate.ConditionCheck(rMe, rData);
				}
			case CD_ManaByValue_T::ID:
				{
					return m_CD_ManaByValue.ConditionCheck(rMe, rData);
				}
				break;
			case CD_ManaByRate_T::ID :
				{
					return m_CD_ManaByRate.ConditionCheck(rMe, rData);
				}
				break;
			case CD_PneumaByValue_T::ID:
				{
					return m_CD_PneumaByValue.ConditionCheck(rMe, rData);
				}
				break;
			case CD_PneumaByRate_T::ID :
				{
					return m_CD_PneumaByRate.ConditionCheck(rMe, rData);
				}
				break;
			case CD_RageByValue_T::ID :
				{
					return m_CD_RageByValue.ConditionCheck(rMe, rData);
				}
			case CD_RageByRate_T::ID :
				{
					return m_CD_RageByRate.ConditionCheck(rMe, rData);
				}
			case D_CancelSpecialImpact_T::ID :
				{
					return m_D_CancelSpecialImpact.ConditionCheck(rMe, rData);
				}
			case CD_CancelSpecialImpact_T::ID :
				{
					return m_CD_CancelSpecialImpact.ConditionCheck(rMe, rData);
				}
			case C_TargetMustBeMySpouse_T::ID :
				{
					return m_C_TargetMustBeMySpouse.ConditionCheck(rMe, rData);
				}
			case CD_HavePetZhaoHuanTime_T::ID ://add by gh 2010/05/20
				{
					return m_CD_HavePetZhaoHuanTime.ConditionCheck(rMe, rData);
				}
			case CD_HaveZuoJiZhaoHuanTime_T::ID ://add by gh 2010/05/20
				{
					return m_CD_HaveZuoJiZhaoHuanTime.ConditionCheck(rMe, rData);
				}
			default :
				return TRUE;
				break;
		};
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	BOOL CDCore_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		switch(rData.GetType())
		{

		case C_UnitHaveImpact_T::ID :
			{
				return m_C_UnitHaveImpact.Deplete(rMe, rData);
			}
			break;
		case C_UnitHpLessThanByRate_T::ID :
			{
				return m_C_UnitHpLessThanByRate.Deplete(rMe, rData);
			}
			break;	
		case C_UnitHpMoreThanByRate_T::ID :
			{
				return m_C_UnitHpMoreThanByRate.Deplete( rMe, rData );
			}
		case C_TargetMustHaveImpact_T::ID :
			{
				return m_C_TargetMustHaveImpact.Deplete(rMe, rData);
			}
			break;
		case C_TargetHpLessThanByRate_T::ID :
			{
				return m_C_TargetHpLessThanByRate.Deplete( rMe, rData );
			}
		case C_TargetHpMoreThanByRate_T::ID :
			{
				return m_C_TargetHpMoreThanByRate.Deplete( rMe, rData );
			}
		case CD_HpByValue_T::ID :
			{
				return m_CD_HpByValue.Deplete( rMe, rData );
			}
		case CD_HpByRate_T::ID :
			{
				return m_CD_HpByRate.Deplete(rMe, rData);
			}
		case CD_ManaByValue_T::ID :
			{
				return m_CD_ManaByValue.Deplete(rMe, rData);
			}
			break;
		case CD_ManaByRate_T::ID :
			{
				return m_CD_ManaByRate.Deplete(rMe, rData);
			}
			break;
		case CD_PneumaByValue_T::ID :
			{
				return m_CD_PneumaByValue.Deplete(rMe, rData);
			}
			break;
		case CD_PneumaByRate_T::ID :
			{
				return m_CD_PneumaByRate.Deplete(rMe, rData);
			}
			break;
		case CD_RageByValue_T::ID :
			{
				return m_CD_RageByValue.Deplete(rMe, rData);
			}
		case CD_RageByRate_T::ID :
			{
				return m_CD_RageByRate.Deplete(rMe, rData);
			}
		case D_CancelSpecialImpact_T::ID :
			{
				return m_D_CancelSpecialImpact.Deplete(rMe, rData);
			}
		case CD_CancelSpecialImpact_T::ID :
			{
				return m_CD_CancelSpecialImpact.Deplete(rMe, rData);
			}
		case C_TargetMustBeMySpouse_T::ID :
			{
				return m_C_TargetMustBeMySpouse.Deplete(rMe, rData);
			}
		case CD_HavePetZhaoHuanTime_T::ID ://add by gh 2010/05/20
			{
				return m_CD_HavePetZhaoHuanTime.Deplete(rMe, rData);
			}
		case CD_HaveZuoJiZhaoHuanTime_T::ID ://add by gh 2010/05/20
			{
				return m_CD_HaveZuoJiZhaoHuanTime.Deplete(rMe, rData);
			}
			break;
			default :
			break;
		};
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL CDCore_T::ConditionCheckByItem( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION

		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

		if(Obj::OBJ_TYPE_HUMAN != rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& oHuman = static_cast<Obj_Human&>(rMe);

		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		INT iItemType  = rData.GetParam0();
		INT iItemCount = rData.GetParam1();
		INT iTmp;
		
		if( iItemCount>0 && iItemType!=INVALID_ID )
		{
			if( !HumanItemLogic::CalcBagItemSpace( &oHuman, iItemType, iItemCount, iTmp ) )
			{
				rParams.SetErrCode(OR_NOT_ENOUGH_ITEM);
				return FALSE;
			}
		}

		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL CDCore_T::DepleteByItem( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION

		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

		if(Obj::OBJ_TYPE_HUMAN != rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& oHuman = static_cast<Obj_Human&>(rMe);

		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		INT iItemType  = rData.GetParam0();
		INT iItemCount = rData.GetParam1();
		INT iTmp;

		if( iItemCount>0 && iItemType!=INVALID_ID )
		{
			if( !HumanItemLogic::CalcBagItemSpace( &oHuman, iItemType, iItemCount, iTmp ) )
			{
				rParams.SetErrCode(OR_NOT_ENOUGH_ITEM);
				return FALSE;
			}
			
			rParams.SetErrCode(OR_OK);
			ITEM_LOG_PARAM oLogParam;
			oLogParam.OpType = ITEM_SKILL_USE;
			return HumanItemLogic::EraseBagItem( &oLogParam, &oHuman, iItemType, iItemCount );
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL CDCore_T::C_UnitHaveImpact_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		if(FALSE==rMe.Impact_HaveImpactInSpecificCollection(nCollection))
		{
			rParams.SetErrCode(OR_U_CANNT_DO_THIS_RIGHT_NOW);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_UnitHaveImpact_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::C_UnitHpLessThanByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}

		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nHP = rMe.GetMaxHP();
		nHP = Float2Int((nHP*iDeplete)/100.0f);

		nHP = RefixValue( nHP, rMe, rData );
		if(rMe.GetHP()>nHP)
		{
			rParams.SetErrCode(OR_TOO_MUCH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_UnitHpLessThanByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::C_UnitHpMoreThanByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nHP = rMe.GetMaxHP();
		nHP = Float2Int((nHP*iDeplete)/100.0f);
		iDeplete = RefixValue( nHP, rMe, rData );
		if(rMe.GetHP()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
	}
	BOOL CDCore_T::C_UnitHpMoreThanByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}


	BOOL CDCore_T::CD_HpByValue_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		iDeplete = RefixValue( iDeplete, rMe, rData );
		if(rMe.GetHP()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
	}
	BOOL CDCore_T::CD_HpByValue_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		iDeplete = RefixValue( iDeplete, rMe, rData );

		rMe.HealthIncrement( -iDeplete, INVALID_ID, &rMe );
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}


	BOOL CDCore_T::C_TargetHpLessThanByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
		if(NULL==pTar)
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}

		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nHP = pTar->GetMaxHP();
		nHP = Float2Int((nHP*iDeplete)/100.0f);
		iDeplete = RefixValue( nHP, rMe, rData );

		if(pTar->GetHP() > iDeplete)
		{
			rParams.SetErrCode(OR_TOO_MUCH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
	}
	BOOL CDCore_T::C_TargetHpLessThanByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::C_TargetHpMoreThanByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
		if(NULL==pTar)
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}

		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nHP = pTar->GetMaxHP();
		nHP = Float2Int((nHP*iDeplete)/100.0f);
		iDeplete = RefixValue( nHP, rMe, rData );
		if(pTar->GetHP()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
	}
	BOOL CDCore_T::C_TargetHpMoreThanByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	
	BOOL CDCore_T::C_TargetMustHaveImpact_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
		if(NULL==pTar)
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		if(FALSE==pTar->Impact_HaveImpactInSpecificCollection(nCollection))
		{
			rParams.SetErrCode(OR_INVALID_TARGET);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_TargetMustHaveImpact_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::D_CancelSpecialImpact_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::D_CancelSpecialImpact_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		rMe.Impact_CancelImpactInSpecificCollection(nCollection);
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_TargetMustBeMySpouse_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;			
		}
		INT nCollection = rData.GetParam0();
		Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
		if(NULL==pTar)
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		if(Obj::OBJ_TYPE_HUMAN!=pTar->GetObjType())
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		Obj_Human& rHuman = static_cast<Obj_Human&>(*pTar);

		if(Obj::OBJ_TYPE_HUMAN!=pTar->GetObjType())
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		Obj_Human& rTar = static_cast<Obj_Human&>(*pTar);
		if(FALSE==rHuman.IsMySpouse(rTar) || FALSE==rTar.IsMySpouse(rHuman))
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_TargetMustBeMySpouse_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE; //不需要消耗
	}
	BOOL CDCore_T::CD_HpByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nHp = rHuman.GetMaxHP();
		nHp = Float2Int((iDeplete*nHp)/100.0f);
		iDeplete = RefixValue( nHp, rMe, rData );

		if(rHuman.GetHP()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_HpByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nHP = rHuman.GetMaxHP();
		nHP = Float2Int((nHP*iDeplete)/100.0f);
		iDeplete = RefixValue( nHP, rMe, rData );

		rHuman.HealthIncrement(-iDeplete, INVALID_ID, &rHuman);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	INT CDCore_T::ConDepLogic_T::RefixValue( INT iValue, Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION

		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		INT iRefixValue = rSkillInfo.GetDepleteRefixByValue();
		INT iRefixRate	= rSkillInfo.GetDepleteRefixByRate();
		if( iRefixRate == 0 )
		{
			iValue += iRefixValue;
		}
		else
		{
			iValue = Float2Int(iValue * iRefixRate / 100.0f);
		}
		return iValue;
		__LEAVE_FUNCTION
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//Mana1
	BOOL CDCore_T::CD_ManaByValue_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		iDeplete = RefixValue( iDeplete, rMe, rData );
		if(rMe.GetMP()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_MP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_ManaByValue_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		iDeplete = RefixValue( iDeplete, rMe, rData );

		rMe.ManaIncrement( -iDeplete, INVALID_ID, &rMe );
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_ManaByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nMp = rHuman.GetMaxMP();
		nMp = Float2Int((iDeplete*nMp)/100.0f);
		iDeplete = RefixValue( nMp, rMe, rData );

		if(rHuman.GetMP()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_MP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_ManaByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nMP = rHuman.GetMaxMP();
		nMP = Float2Int((nMP*iDeplete)/100.0f);
		iDeplete = RefixValue( nMP, rMe, rData );

		rHuman.ManaIncrement(-iDeplete, INVALID_ID, &rHuman);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//Pneuma
	BOOL CDCore_T::CD_PneumaByValue_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		iDeplete = RefixValue( iDeplete, rMe, rData );
		if(rMe.GetPneuma()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_PNEUMA);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_PneumaByValue_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		iDeplete = RefixValue( iDeplete, rMe, rData );

		rMe.PneumaIncrement( -iDeplete, INVALID_ID, &rMe );
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_PneumaByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nPneuma = rHuman.GetMaxPneuma();
		nPneuma = Float2Int((iDeplete*nPneuma)/100.0f);
		iDeplete = RefixValue( nPneuma, rMe, rData );

		if(rHuman.GetPneuma()<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_PNEUMA);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_PneumaByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
		INT nPneuma = rHuman.GetMaxPneuma();
		nPneuma = Float2Int((nPneuma*iDeplete)/100.0f);
		iDeplete = RefixValue( nPneuma, rMe, rData );

		rHuman.PneumaIncrement(-iDeplete, INVALID_ID, &rHuman);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////

	BOOL CDCore_T::CD_RageByValue_T::ConditionCheck( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		return RageConditionCheckByValue( rMe, rData );
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL CDCore_T::CD_RageByValue_T::Deplete( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		return RageDepleteByValue( rMe, rData );
		__LEAVE_FUNCTION
		return FALSE;
	}

	INT CDCore_T::ConDepLogic_T::GetCommonDeplete( ConDepTerm_T const &rData, SkillInfo_T &rSkillInfo ) const 
	{
		INT iPara1		= rData.GetParam0();
		INT iPara2		= rData.GetParam1();
		INT iSkillLevel	= rSkillInfo.GetCharacterSkillLevel();
		return iPara1 * iSkillLevel + iPara2;
	}

	BOOL CDCore_T::ConDepLogic_T::RageConditionCheckByValue( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete(rData, rSkillInfo);
		iDeplete = RefixValue( iDeplete, rMe, rData );

		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iRage = rHuman.GetRage();

		if(iRage<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_RAGE);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL CDCore_T::ConDepLogic_T::RageDepleteByValue( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete(rData, rSkillInfo);
		iDeplete = RefixValue( iDeplete, rMe, rData );

		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iRage = rHuman.GetRage();

		if(iRage<iDeplete)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_RAGE);
			return FALSE;
		}
		//rMe.RageIncrement( -iDeplete, INVALID_ID, &rMe );
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL CDCore_T::ConDepLogic_T::RageConditionCheckByRate( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete(rData, rSkillInfo);

		INT iRageNeed = rMe.GetMaxRage();
		iRageNeed = Float2Int(iRageNeed * iDeplete / 100.0f);
		
		iDeplete = RefixValue( iRageNeed, rMe, rData );

		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iRage = rHuman.GetRage();

		if(iRage<iRageNeed)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_RAGE);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL Combat_Module::CDCore_T::ConDepLogic_T::RageDepleteByRate( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT iDeplete = GetCommonDeplete(rData, rSkillInfo);

		INT iRageNeed = rMe.GetMaxRage();
		iRageNeed = Float2Int(iRageNeed * iDeplete / 100.0f);

		iDeplete = RefixValue( iRageNeed, rMe, rData );
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT iRage = rHuman.GetRage();

		if(iRage<iRageNeed)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_RAGE);
			return FALSE;
		}
		//rMe.RageIncrement( -iDeplete, INVALID_ID, &rMe );
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL CDCore_T::CD_CancelSpecialImpact_T::ConditionCheck( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		if(FALSE==rMe.Impact_HaveImpactInSpecificCollection(nCollection))
		{
			rParams.SetErrCode(OR_U_CANNT_DO_THIS_RIGHT_NOW);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL Combat_Module::CDCore_T::CD_CancelSpecialImpact_T::Deplete( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		rMe.Impact_CancelImpactInSpecificCollection(nCollection);
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL Combat_Module::CDCore_T::CD_RageByRate_T::ConditionCheck( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		return RageConditionCheckByRate( rMe, rData );
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL Combat_Module::CDCore_T::CD_RageByRate_T::Deplete( Obj_Character& rMe, ConDepTerm_T const& rData ) const
	{
		__ENTER_FUNCTION
		return RageDepleteByRate( rMe, rData );
		__LEAVE_FUNCTION
		return FALSE;
	}
}
// add by gh for souxia 2010/05/20
BOOL CDCore_T::CD_HavePetZhaoHuanTime_T::ConditionCheck( Obj_Character& rMe, ConDepTerm_T const& rData ) const
{
	__ENTER_FUNCTION
	SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
	if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
	{
		rParams.SetErrCode(OR_OK);
		return TRUE;
	}
	INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
	iDeplete = RefixValue( iDeplete, rMe, rData );
	if(rMe.GetPetZhaoHuanSkillTimes(rSkillInfo.GetSkillID())<iDeplete)
	{
		rParams.SetErrCode(OR_PETZHAOHUAN_NO_TIMES);
		return FALSE;
	}
	rParams.SetErrCode(OR_OK);
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}
BOOL Combat_Module::CDCore_T::CD_HavePetZhaoHuanTime_T::Deplete( Obj_Character& rMe, ConDepTerm_T const& rData ) const
{
	__ENTER_FUNCTION
	SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
	if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
	{
		rParams.SetErrCode(OR_OK);
		return TRUE;
	}
	INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
	iDeplete = RefixValue( iDeplete, rMe, rData );
	//直接减少,不用	//rMe.PetZhaoHuanTimesIncrement( -iDeplete, INVALID_ID, &rMe, rSkillInfo.GetSkillID());
	if(TRUE==rMe.IsActiveObj())
	{
		rMe.SetPetZhaoHuanSkillTimes(rSkillInfo.GetSkillID(), -iDeplete);
	}
	rParams.SetErrCode(OR_OK);
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL CDCore_T::CD_HaveZuoJiZhaoHuanTime_T::ConditionCheck( Obj_Character& rMe, ConDepTerm_T const& rData ) const
{
	__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
	if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
	{
		rParams.SetErrCode(OR_OK);
		return TRUE;
	}
	INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
	iDeplete = RefixValue( iDeplete, rMe, rData );
	if(rMe.GetZuoJiZhaoHuanSkillTimes(rSkillInfo.GetSkillID()) <iDeplete)
	{
		rParams.SetErrCode(OR_ZUOJIZHAOHUAN_NO_TIMES);
		return FALSE;
	}
	rParams.SetErrCode(OR_OK);
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}
BOOL Combat_Module::CDCore_T::CD_HaveZuoJiZhaoHuanTime_T::Deplete( Obj_Character& rMe, ConDepTerm_T const& rData ) const
{
	__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
	if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
	{
		rParams.SetErrCode(OR_OK);
		return TRUE;
	}
	INT iDeplete = GetCommonDeplete( rData, rSkillInfo );
	iDeplete = RefixValue( iDeplete, rMe, rData );
	//直接减少,不用	//rMe.ZuoJiZhaoHuanTimesIncrement( -iDeplete, INVALID_ID, &rMe, rSkillInfo.GetSkillID());
	if(TRUE==rMe.IsActiveObj())
	{
		rMe.SetZuoJiZhaoHuanSkillTimes(rSkillInfo.GetSkillID(), -iDeplete);
	}
	rParams.SetErrCode(OR_OK);
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}


