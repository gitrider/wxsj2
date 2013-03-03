//***************************************************************************************************
// 文件名称:	Obj_Character_Attributes.cpp
// 系统名称:	
// 模块名称:	角色层属性
// 功能说明:	
//
// 相关文档:	
// 作    者:	liam
// 建立日期:	
// 审    核:	
// 软件版权:	
//
//修改记录:
//          1.
//				修 改 人:liam
//				修改时间:2007/11/02
//				修改内容:属性第二部分重构(具体参见文档)
//***************************************************************************************************
#include "stdafx.h"
#include "Type.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "Obj_Character.h"

//StrikePoint
INT	Obj_Character::GetMaxStrikePointRefix(VOID)
{
	if(TRUE==GetMaxStrikePointRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT, nValue);
		ClearMaxStrikePointRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT);
}
VOID Obj_Character::SetMaxStrikePointRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT, nValue);
	MarkMaxStrikePointRefixDirtyFlag();
}
//Rage
INT	Obj_Character::GetMaxRageRefix(VOID)
{
	if(TRUE==GetMaxRageRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_RAGE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_RAGE, nValue);
		ClearMaxRageRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_RAGE);
}
VOID Obj_Character::SetMaxRageRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_RAGE, nValue);
	MarkMaxRageRefixDirtyFlag();
}

INT Obj_Character::GetRageRegenerateRefix(VOID)
{
	if(TRUE==GetRageRegenerateRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE, nValue);
		ClearRageRegenerateRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE);
}
VOID Obj_Character::SetRageRegenerateRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE, nValue);
	MarkRageRegenerateRefixDirtyFlag();
}
//Vigor
//LEVEL
//Attr1: Str
INT	Obj_Character::GetStrRefix(VOID)
{
	if(TRUE==GetStrRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STR, nValue);
		ClearStrRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STR);
}
VOID Obj_Character::SetStrRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STR, nValue);
	MarkStrRefixDirtyFlag();
}
////Attr1: Spr
//INT Obj_Character::GetSprRefix(VOID)
//{
//	if(TRUE==GetSprRefixDirtyFlag())
//	{
//		INT nValue = 0;
//		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SPR, nValue);
//		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SPR, nValue);
//		ClearSprRefixDirtyFlag();
//	}
//	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SPR);
//}
//VOID Obj_Character::SetSprRefix(INT const nValue)
//{
//	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SPR, nValue);
//	MarkSprRefixDirtyFlag();
//}
//Attr1: Con
INT Obj_Character::GetConRefix(VOID)
{
	if(TRUE==GetConRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CON, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CON, nValue);
		ClearConRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CON);
}
VOID Obj_Character::SetConRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CON, nValue);
	MarkConRefixDirtyFlag();
}
//Attr1: Int
INT Obj_Character::GetIntRefix(VOID)
{
	if(TRUE==GetIntRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_INT, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_INT, nValue);
		ClearIntRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_INT);
}
VOID Obj_Character::SetIntRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_INT, nValue);
	MarkIntRefixDirtyFlag();;
}
//Attr1: Dex
INT Obj_Character::GetDexRefix(VOID)
{
	if(TRUE==GetDexRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEX, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEX, nValue);
		ClearDexRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEX);
}
VOID Obj_Character::SetDexRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEX, nValue);
	MarkDexRefixDirtyFlag();
}
//HP
INT Obj_Character::GetMaxHPRefix(VOID)
{
	if(TRUE==GetMaxHPRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_HP, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_HP, nValue);
		ClearMaxHPRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_HP);
}
VOID Obj_Character::SetMaxHPRefix(INT const nHp)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_HP, nHp);
	MarkMaxHPRefixDirtyFlag();
}
INT Obj_Character::GetHPRegenerateRefix(VOID)
{
	if(TRUE==GetHPRegenerateRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HP_REGENERATE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HP_REGENERATE, nValue);
		ClearHPRegenerateRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HP_REGENERATE);
}
VOID Obj_Character::SetHPRegenerateRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HP_REGENERATE, nValue);
	MarkHPRegenerateRefixDirtyFlag();
}

//MP
INT Obj_Character::GetMaxMPRefix(VOID)
{
	if(TRUE==GetMaxMPRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_MP, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_MP, nValue);
		ClearMaxMPRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_MP);
}
VOID Obj_Character::SetMaxMPRefix(INT const nMp)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_MP, nMp);
	MarkMaxMPRefixDirtyFlag();
}
INT Obj_Character::GetMPRegenerateRefix(VOID)
{
	if(TRUE==GetMPRegenerateRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MP_REGENERATE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MP_REGENERATE, nValue);
		ClearMPRegenerateRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MP_REGENERATE);
}
VOID Obj_Character::SetMPRegenerateRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MP_REGENERATE, nValue);
	MarkMPRegenerateRefixDirtyFlag();
}

//Pneuma
INT Obj_Character::GetMaxPneumaRefix(VOID)
{
	if(TRUE==GetMaxPneumaRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA, nValue);
		ClearMaxPneumaRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA);
}
VOID Obj_Character::SetMaxPneumaRefix(INT const nPneuma)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA, nPneuma);
	MarkMaxPneumaRefixDirtyFlag();
}
INT Obj_Character::GetPneumaRegenerateRefix(VOID)
{
	if(TRUE==GetPneumaRegenerateRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE, nValue);
		ClearPneumaRegenerateRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE);
}
VOID Obj_Character::SetPneumaRegenerateRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE, nValue);
	MarkPneumaRegenerateRefixDirtyFlag();
}
//CampID
const _CAMP_DATA *Obj_Character::GetCampData(VOID)
{
	const _CAMP_DATA *pCampData = GetBaseCampData();
	if ( pCampData != NULL )
	{
		m_CampDataRefix = *pCampData;
		m_CampDataRefix.m_nCampID = GetCampID();
		return &m_CampDataRefix;
	}
	else
	{
		return NULL;
	}
}

INT	Obj_Character::GetCampIDRefix(VOID)
{
	if(TRUE==GetCampIDRefixDirtyFlag())
	{
		INT nValue = INVALID_ID;
		if(TRUE==Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CAMP, nValue))
		{
			SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CAMP, nValue);
		}
		ClearCampIDRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CAMP);
}
VOID Obj_Character::SetCampIDRefix(CampID_t const nID)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CAMP, nID);
	MarkCampIDRefixDirtyFlag();
}
//modelID

INT	Obj_Character::GetModelIDRefix(VOID)
{
	if(TRUE==GetModelIDRefixDirtyFlag())
	{
		INT nValue = INVALID_ID;
		if(TRUE==Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MODEL_ID, nValue))
		{
			SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MODEL_ID, nValue);
		}
		ClearModelIDRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MODEL_ID);
}
VOID Obj_Character::SetModelIDRefix(ID_t const nID)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MODEL_ID, nID);
	MarkModelIDRefixDirtyFlag();
}
//mountID
INT Obj_Character::GetMountID(VOID)
{
	//if(TRUE==GetMountIDDirtyFlag())
	//{
	//	INT nValue=GetMountIDRefix();
	//	if(INVALID_ID==nValue)
	//	{
	//		nValue=GetBaseMountID();
	//	}
	//	SetIntAttr(CharIntAttrs_T::ATTR_MOUNT_ID, nValue);
	//	ClearMountIDDirtyFlag();
	//}
	return GetIntAttr(CharIntAttrs_T::ATTR_MOUNT_ID);
}

VOID Obj_Character::SetMountID( INT iMountID )
{
	SetIntAttr(CharIntAttrs_T::ATTR_MOUNT_ID, iMountID );
}

INT	Obj_Character::GetMountIDRefix(VOID)
{
	if(TRUE==GetMountIDRefixDirtyFlag())
	{
		INT nValue = INVALID_ID;
		if(TRUE==Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOUNT_ID, nValue))
		{
			SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOUNT_ID, nValue);
		}
		ClearMountIDRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOUNT_ID);
}
VOID Obj_Character::SetMountIDRefix(ID_t const nID)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOUNT_ID, nID);
	MarkMountIDRefixDirtyFlag();
}
//Exp Refix
INT Obj_Character::GetExpRefix(VOID)
{
	if(TRUE==GetExpRefixDirtyFlag())
	{
		INT nValue = GetBaseExpRefix();
		nValue += GetExpRefixRefix();
		0>nValue?nValue=0:NULL;
		MAX_EXP_REFIX_IN_CHAR<nValue?nValue=MAX_EXP_REFIX_IN_CHAR:NULL;
 		SetIntAttr(CharIntAttrs_T::ATTR_EXP_REFIX, nValue);
		ClearExpRefixDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_EXP_REFIX);
}
INT	Obj_Character::GetExpRefixRefix(VOID)
{
	if(TRUE==GetExpRefixRefixDirtyFlag())
	{
		INT nValue = INVALID_ID;
		if(TRUE==Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_EXP_REFIX, nValue))
		{
			SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_EXP_REFIX, nValue);
		}
		ClearExpRefixRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_EXP_REFIX);
}
VOID Obj_Character::SetExpRefixRefix(ID_t const nID)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_EXP_REFIX, nID);
	MarkExpRefixRefixDirtyFlag();
}
//StealthLevel
INT Obj_Character::GetStealthLevel(VOID)
{
	if(TRUE==GetStealthLevelDirtyFlag())
	{
		INT nValue=GetStealthLevelRefix() + 0;//0 is base stealth level for every character
		SetIntAttr(CharIntAttrs_T::ATTR_STEALTH_LEVEL, nValue);
		ClearStealthLevelDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_STEALTH_LEVEL);
}
INT	Obj_Character::GetStealthLevelRefix(VOID)
{
	if(TRUE==GetStealthLevelRefixDirtyFlag())
	{
		INT nValue = 0;//0 is default stealth level for every character
		if(TRUE==Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL, nValue))
		{
			SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL, nValue);
		}
		ClearStealthLevelRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL);
}
VOID Obj_Character::SetStealthLevelRefix(ID_t const nID)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL, nID);
	MarkStealthLevelRefixDirtyFlag();
}
//DetectLevel
INT Obj_Character::GetDetectLevel(VOID)
{
	if(TRUE==GetDetectLevelDirtyFlag())
	{
		INT nValue=GetDetectLevelRefix() + 0;//0 is base stealth level for every character
		SetIntAttr(CharIntAttrs_T::ATTR_DETECT_LEVEL, nValue);
		ClearDetectLevelDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DETECT_LEVEL);
}
INT	Obj_Character::GetDetectLevelRefix(VOID)
{
	if(TRUE==GetDetectLevelRefixDirtyFlag())
	{
		INT nValue = 0;//0 is default stealth level for every character
		if(TRUE==Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL, nValue))
		{
			SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL, nValue);
		}
		ClearDetectLevelRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL);
}
VOID Obj_Character::SetDetectLevelRefix(ID_t const nID)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL, nID);
	MarkDetectLevelRefixDirtyFlag();
}

//Alive flag
BOOL Obj_Character::IsAlive(VOID)
{
	__ENTER_FUNCTION
	if(0>=GetHP())
	{
		return FALSE;
	}
	if(TRUE==GetAliveDirtyFlag())
	{
		BOOL bValue = TRUE;
		if(TRUE==Impact_GetBoolAttrRefix(CharBoolAttrs_T::ATTR_ALIVE, bValue))
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_ALIVE, bValue);
		}
		else
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_ALIVE, TRUE);
		}
		ClearAliveDirtyFlag();
	}
	return GetBoolAttr(CharBoolAttrs_T::ATTR_ALIVE);
	
	__LEAVE_FUNCTION

	return FALSE;
}
BOOL Obj_Character::IsAliveInDeed(VOID)
{
	return 0<GetHP();
}
BOOL Obj_Character::IsDie(VOID)
{
	return (!IsAliveInDeed());
}
VOID Obj_Character::ClearAliveFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_ALIVE, FALSE);
}
VOID Obj_Character::MarkAliveFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_ALIVE, TRUE);
}
//Obj_Character::In Combat flag
BOOL Obj_Character::InCombat(VOID) const
{
	return GetBoolAttr(CharBoolAttrs_T::ATTR_IN_COMBAT);
}
VOID Obj_Character::ClearCombatFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_IN_COMBAT, FALSE);
}
VOID Obj_Character::MarkCombatFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_IN_COMBAT, TRUE);
}
// Can move flag
BOOL Obj_Character::CanMove(VOID)
{
	if(TRUE==GetCanMoveDirtyFlag())
	{
		BOOL bValue = TRUE;
		if(TRUE==Impact_GetBoolAttrRefix(CharBoolAttrs_T::ATTR_CAN_MOVE, bValue))
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_MOVE, bValue);
		}
		else
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_MOVE, TRUE);
		}
		ClearCanMoveDirtyFlag();
	}
	return GetBoolAttr(CharBoolAttrs_T::ATTR_CAN_MOVE);
}
VOID Obj_Character::ClearCanMoveFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_MOVE, FALSE);
}
VOID Obj_Character::MarkCanMoveFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_MOVE, TRUE);
}
// Can Action flag 1
BOOL Obj_Character::CanActionFlag1(VOID)
{
	if(TRUE==GetCanAction1DirtyFlag())
	{
		BOOL bValue = TRUE;
		if(TRUE==Impact_GetBoolAttrRefix(CharBoolAttrs_T::ATTR_CAN_ACTION1, bValue))
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION1, bValue);
		}
		else
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION1, TRUE);
		}
		ClearCanAction1DirtyFlag();
	}
	return GetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION1);
}
VOID Obj_Character::ClearCanActionFlag1(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION1, FALSE);
}
VOID Obj_Character::MarkCanActionFlag1(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION1, TRUE);
}
// Can Action flag 2
BOOL Obj_Character::CanActionFlag2(VOID)
{
	if(TRUE==GetCanAction2DirtyFlag())
	{
		BOOL bValue = TRUE;
		if(TRUE==Impact_GetBoolAttrRefix(CharBoolAttrs_T::ATTR_CAN_ACTION2, bValue))
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION2, bValue);
		}
		else
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION2, TRUE);
		}
		ClearCanAction2DirtyFlag();
	}
	return GetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION2);
}
VOID Obj_Character::ClearCanActionFlag2(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION2, FALSE);
}
VOID Obj_Character::MarkCanActionFlag2(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_ACTION2, TRUE);
}
// Unbreakable flag
BOOL Obj_Character::IsUnbreakable(VOID)
{
	if(TRUE==GetUnbreakableDirtyFlag())
	{
		BOOL bValue = FALSE;//default value should be FALSE
		if(TRUE==Impact_GetBoolAttrRefix(CharBoolAttrs_T::ATTR_UNBREAKABLE, bValue))
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_UNBREAKABLE, bValue);
		}
		else
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_UNBREAKABLE, FALSE);//default value should be FALSE
		}
		ClearUnbreakableDirtyFlag();
	}
	return GetBoolAttr(CharBoolAttrs_T::ATTR_UNBREAKABLE);
}
VOID Obj_Character::ClearUnbreakableFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_UNBREAKABLE, FALSE);
}
VOID Obj_Character::MarkUnbreakableFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_UNBREAKABLE, TRUE);
}
/*
// InStealthMode flag
BOOL Obj_Character::InStealthMode(VOID)
{
	if(TRUE==GetInStealthModeDirtyFlag())
	{
		BOOL bValue = FALSE;//default value should be FALSE
		if(TRUE==Impact_GetBoolAttrRefix(CharBoolAttrs_T::ATTR_IN_STEALTH_MODE, bValue))
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_IN_STEALTH_MODE, bValue);
		}
		else
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_IN_STEALTH_MODE, FALSE);//default value should be FALSE
		}
		ClearInStealthModeDirtyFlag();
	}
	return GetBoolAttr(CharBoolAttrs_T::ATTR_IN_STEALTH_MODE);
}
VOID Obj_Character::ClearInStealthModeFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_IN_STEALTH_MODE, FALSE);
}
VOID Obj_Character::MarkInStealthModeFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_IN_STEALTH_MODE, TRUE);
}
// CanDetectStealth flag
BOOL Obj_Character::CanDetectStealth(VOID)
{
	if(TRUE==GetCanDetectStealthDirtyFlag())
	{
		BOOL bValue = FALSE;//default value should be FALSE
		if(TRUE==Impact_GetBoolAttrRefix(CharBoolAttrs_T::ATTR_CAN_DETECT_STEALTH, bValue))
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_DETECT_STEALTH, bValue);
		}
		else
		{
			SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_DETECT_STEALTH, FALSE);//default value should be FALSE
		}
		ClearCanDetectStealthDirtyFlag();
	}
	return GetBoolAttr(CharBoolAttrs_T::ATTR_CAN_DETECT_STEALTH);
}
VOID Obj_Character::ClearCanDetectStealthFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_DETECT_STEALTH, FALSE);
}
VOID Obj_Character::MarkCanDetectStealthFlag(VOID)
{
	SetBoolAttr(CharBoolAttrs_T::ATTR_CAN_DETECT_STEALTH, TRUE);
}
*/
// Attr2: Move Speed
INT Obj_Character::GetMoveSpeedRefix(VOID)
{
	if(TRUE==GetMoveSpeedRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVE_SPEED, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVE_SPEED, nValue);
		ClearMoveSpeedRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVE_SPEED);
}
VOID Obj_Character::SetMoveSpeedRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVE_SPEED, nValue);
	MarkMoveSpeedRefixDirtyFlag();
}
// Attr2: Attack Speed
INT	Obj_Character::GetAttackSpeedRefix(VOID)
{
	if(TRUE==GetAttackSpeedRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED, nValue);
		ClearAttackSpeedRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED);
}
VOID Obj_Character::SetAttackSpeedRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED, nValue);
	MarkAttackSpeedRefixDirtyFlag();
}
// Attr2: Miss
INT	Obj_Character::GetMissRefix(VOID)
{
	if(TRUE==GetMissRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MISS, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MISS, nValue);
		ClearMissRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MISS);
}
VOID Obj_Character::SetMissRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MISS, nValue);
	MarkMissRefixDirtyFlag();
}
// Attr2 Hit
INT	Obj_Character::GetHitRefix(VOID)
{
	if(TRUE==GetHitRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HIT, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HIT, nValue);
		ClearHitRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HIT);
}
VOID Obj_Character::SetHitRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HIT, nValue);
	MarkHitRefixDirtyFlag();
}
// Attr2 Critical
INT	Obj_Character::GetCriticalRefix(VOID)
{
	if(TRUE==GetCriticalRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CRITICAL, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CRITICAL, nValue);
		ClearCriticalRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CRITICAL);
}
VOID Obj_Character::SetCriticalRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_CRITICAL, nValue);
	MarkCriticalRefixDirtyFlag();
}
// Attr2: Attack and Defence power physics
//INT	Obj_Character::GetAttackPhysicsRefix(VOID)
//{
//	if(TRUE==GetAttackPhysicsRefixDirtyFlag())
//	{
//		INT nValue = 0;
//		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_PHY, nValue);
//		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_PHY, nValue);
//		ClearAttackPhysicsRefixDirtyFlag();
//	}
//	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_PHY);
//}
//VOID Obj_Character::SetAttackPhysicsRefix(INT const nValue)
//{
//	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_PHY, nValue);
//	MarkAttackPhysicsRefixDirtyFlag();
//}
//INT	Obj_Character::GetDefencePhysicsRefix(VOID)
//{
//	if(TRUE==GetDefencePhysicsRefixDirtyFlag())
//	{
//		INT nValue = 0;
//		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY, nValue);
//		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY, nValue);
//		ClearDefencePhysicsRefixDirtyFlag();		
//	}
//	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY);
//}
//VOID Obj_Character::SetDefencePhysicsRefix(INT const nValue)
//{
//	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY, nValue);
//	MarkDefencePhysicsRefixDirtyFlag();
//}

//add by liam----------------------------------------------
INT	Obj_Character::GetDefenceNearRefix(VOID)
{
	if(TRUE==GetDefenceNearRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR, nValue);
		ClearDefenceNearRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR);
}

VOID Obj_Character::SetDefenceNearRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR, nValue);
	MarkDefenceNearRefixDirtyFlag();
}


INT Obj_Character::GetDefenceFarRefix(VOID)
{
	if(TRUE==GetDefenceFarRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR, nValue);
		ClearDefenceFarRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR);
}

VOID Obj_Character::SetDefenceFarRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR, nValue);
	MarkDefenceFarRefixDirtyFlag();
}

INT Obj_Character::GetAttackNearRefix(VOID)
{
	if(TRUE==GetAttackNearRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR, nValue);
		ClearAttackNearRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR);
}

VOID Obj_Character::SetAttackNearRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR, nValue);
	MarkAttackNearRefixDirtyFlag();
}

INT Obj_Character::GetAttackFarRefix(VOID)
{
	if(TRUE==GetAttackFarRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FAR, nValue);
		ClearAttackFarRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FAR);
}

VOID Obj_Character::SetAttackFarRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FAR, nValue);
	MarkAttackFarRefixDirtyFlag();
}

INT	Obj_Character::GetToughnessRefix(VOID)
{
	if(TRUE==GetToughnessRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_TOUGHNESS, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_TOUGHNESS, nValue);
		ClearToughnessRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_TOUGHNESS);
}

VOID Obj_Character::SetToughnessRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_TOUGHNESS, nValue);
	MarkToughnessRefixDirtyFlag();
}

INT	Obj_Character::GetNearAttReduceRefix(VOID)
{
	if(TRUE==GetNearAttReduceRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE, nValue);
		ClearNearAttReduceRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE);
}

VOID Obj_Character::SetNearAttReduceRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE, nValue);
	MarkNearAttReduceRefixDirtyFlag();
}

INT	Obj_Character::GetFarAttReduceRefix(VOID)
{
	if(TRUE==GetFarAttReduceRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE, nValue);
		ClearFarAttReduceRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE);
}

VOID Obj_Character::SetFarAttReduceRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE, nValue);
	MarkFarAttReduceRefixDirtyFlag();
}

INT	Obj_Character::GetMagicNearAttReduceRefix(VOID)
{
	if(TRUE==GetMagicNearAttReduceRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE, nValue);
		ClearMagicNearAttReduceRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE);
}
INT	Obj_Character::GetMagicFarAttReduceRefix(VOID)
{
	if(TRUE==GetMagicFarAttReduceRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE, nValue);
		ClearMagicFarAttReduceRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE);
}

VOID Obj_Character::SetMagicNearAttReduceRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE, nValue);
	MarkMagicNearAttReduceRefixDirtyFlag();
}
VOID Obj_Character::SetMagicFarAttReduceRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE, nValue);
	MarkMagicFarAttReduceRefixDirtyFlag();
}

INT	Obj_Character::GetDreadAttResistRefix(VOID)
{
	if(TRUE==GetDreadAttResistRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DREAD_RESIST, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DREAD_RESIST, nValue);
		ClearDreadAttResistRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DREAD_RESIST);
}

VOID Obj_Character::SetDreadAttResistRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DREAD_RESIST, nValue);
	MarkDreadAttResistRefixDirtyFlag();
}

INT	Obj_Character::GetComaAttResistRefix(VOID)
{
	if(TRUE==GetComaAttResistRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_COMA_RESIST, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_COMA_RESIST, nValue);
		ClearComaAttResistRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_COMA_RESIST);
}

VOID Obj_Character::SetComaAttResistRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_COMA_RESIST, nValue);
	MarkComaAttResistRefixDirtyFlag();
}

INT	Obj_Character::GetHushAttResistRefix(VOID)
{
	if(TRUE==GetHushAttResistRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HUSH_RESIST, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HUSH_RESIST, nValue);
		ClearHushAttResistRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HUSH_RESIST);
}

VOID Obj_Character::SetHushAttResistRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_HUSH_RESIST, nValue);
	MarkHushAttResistRefixDirtyFlag();
}

INT	Obj_Character::GetUnArmyAttResistRefix(VOID)
{
	if(TRUE==GetUnArmyAttResistRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST, nValue);
		ClearUnArmyAttResistRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST);
}

VOID Obj_Character::SetUnArmyAttResistRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST, nValue);
	MarkUnArmyAttResistRefixDirtyFlag();
}

INT	Obj_Character::GetAttSpeedAttResistRefix(VOID)
{
	if(TRUE==GetAttSpeedAttResistRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST, nValue);
		ClearAttSpeedAttResistRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST);
}

VOID Obj_Character::SetAttSpeedAttResistRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST, nValue);
	MarkAttSpeedAttResistRefixDirtyFlag();
}

INT	Obj_Character::GetSkillSpeedAttResistRefix(VOID)
{
	if(TRUE==GetSkillSpeedAttResistRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST, nValue);
		ClearSkillSpeedAttResistRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST);
}

VOID Obj_Character::SetSkillSpeedAttResistRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST, nValue);
	MarkSkillSpeedAttResistRefixDirtyFlag();
}

INT	Obj_Character::GetMoveSpeedAttResistRefix(VOID)
{
	if(TRUE==GetMoveSpeedAttResistRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST, nValue);
		ClearMoveSpeedAttResistRefixDirtyFlag();		
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST);
}

VOID Obj_Character::SetMoveSpeedAttResistRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST, nValue);
	MarkMoveSpeedAttResistRefixDirtyFlag();
}
//---------------------------------------------------------

// Attr2: 
INT	Obj_Character::GetAttackMagicNearRefix(VOID)
{
	if(TRUE==GetAttackMagicNearRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR, nValue);
		ClearAttackMagicNearRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR);
}
INT	Obj_Character::GetAttackMagicFarRefix(VOID)
{
	if(TRUE==GetAttackMagicFarRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR, nValue);
		ClearAttackMagicFarRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR);
}

VOID Obj_Character::SetAttackMagicNearRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR, nValue);
	MarkAttackMagicNearRefixDirtyFlag();
}
VOID Obj_Character::SetAttackMagicFarRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR, nValue);
	MarkAttackMagicFarRefixDirtyFlag();
}

INT	Obj_Character::GetDefenceMagicNearRefix(VOID)
{
	if(TRUE==GetDefenceMagicNearRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR, nValue);
		ClearDefenceMagicNearRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR);
}
INT	Obj_Character::GetDefenceMagicFarRefix(VOID)
{
	if(TRUE==GetDefenceMagicFarRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR, nValue);
		ClearDefenceMagicFarRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR);
}

VOID Obj_Character::SetDefenceMagicNearRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR, nValue);
	MarkDefenceMagicNearRefixDirtyFlag();
}
VOID Obj_Character::SetDefenceMagicFarRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR, nValue);
	MarkDefenceMagicFarRefixDirtyFlag();
}

// Attr2: Attack and Defence power of gold
INT	Obj_Character::GetAttackGoldRefix(VOID)
{
	if(TRUE==GetAttackGoldRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD, nValue);
		ClearAttackGoldRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD);
}
VOID Obj_Character::SetAttackGoldRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD, nValue);
	MarkAttackGoldRefixDirtyFlag();
}
INT	Obj_Character::GetDefenceGoldRefix(VOID)
{
	if(TRUE==GetResistGoldRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_GOLD, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_GOLD, nValue);
		ClearResistGoldRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_GOLD);
}
VOID Obj_Character::SetDefenceGoldRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_GOLD, nValue);
	MarkResistGoldRefixDirtyFlag();
}

// Attr2: Attack and Defence power of Wood
INT	Obj_Character::GetAttackWoodRefix(VOID)
{
	if(TRUE==GetAttackWoodRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD, nValue);
		ClearAttackWoodRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD);
}
VOID Obj_Character::SetAttackWoodRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD, nValue);
	MarkAttackWoodRefixDirtyFlag();
}
INT	Obj_Character::GetDefenceWoodRefix(VOID)
{
	if(TRUE==GetResistWoodRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WOOD, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WOOD, nValue);
		ClearResistWoodRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WOOD);
}
VOID Obj_Character::SetDefenceWoodRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WOOD, nValue);
	MarkResistWoodRefixDirtyFlag();
}

// Attr2: Attack and Defence power of Water
INT	Obj_Character::GetAttackWaterRefix(VOID)
{
	if(TRUE==GetAttackWaterRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WATER, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WATER, nValue);
		ClearAttackWaterRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WATER);
}
VOID Obj_Character::SetAttackWaterRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_WATER, nValue);
	MarkAttackWaterRefixDirtyFlag();
}
INT	Obj_Character::GetDefenceWaterRefix(VOID)
{
	if(TRUE==GetResistWaterRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WATER, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WATER, nValue);
		ClearResistWaterRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WATER);
}
VOID Obj_Character::SetDefenceWaterRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_WATER, nValue);
	MarkResistWaterRefixDirtyFlag();
}

// Attr2: Attack and Defence power of Fire
INT	Obj_Character::GetAttackFireRefix(VOID)
{
	if(TRUE==GetAttackFireRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE, nValue);
		ClearAttackFireRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE);
}
VOID Obj_Character::SetAttackFireRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE, nValue);
	MarkAttackFireRefixDirtyFlag();
}
INT	Obj_Character::GetDefenceFireRefix(VOID)
{
	if(TRUE==GetResistFireRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_FIRE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_FIRE, nValue);
		ClearResistFireRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_FIRE);
}
VOID Obj_Character::SetDefenceFireRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_FIRE, nValue);
	MarkResistFireRefixDirtyFlag();
}

// Attr2: Attack and Defence power of Soil
INT	Obj_Character::GetAttackSoilRefix(VOID)
{
	if(TRUE==GetAttackSoilRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL, nValue);
		ClearAttackSoilRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL);
}
VOID Obj_Character::SetAttackSoilRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL, nValue);
	MarkAttackSoilRefixDirtyFlag();
}
INT	Obj_Character::GetDefenceSoilRefix(VOID)
{
	if(TRUE==GetResistSoilRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_SOIL, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_SOIL, nValue);
		ClearResistSoilRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_SOIL);
}
VOID Obj_Character::SetDefenceSoilRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_RESIST_SOIL, nValue);
	MarkResistSoilRefixDirtyFlag();
}

// Attr2 Reduce Slower Duration
INT	Obj_Character::GetReduceSlowerDurationRefix(VOID)
{
	if(TRUE==GetReduceSlowerDurationRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION, nValue);
		ClearReduceSlowerDurationRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION);
}
VOID Obj_Character::SetReduceSlowerDurationRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION, nValue);
	MarkReduceSlowerDurationRefixDirtyFlag();
}
// Attr2 Reduce Weaken Duration
INT	Obj_Character::GetReduceWeakenDurationRefix(VOID)
{
	if(TRUE==GetReduceWeakenDurationRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION, nValue);
		ClearReduceWeakenDurationRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION);
}
VOID Obj_Character::SetReduceWeakenDurationRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION, nValue);
	MarkReduceWeakenDurationRefixDirtyFlag();
}
// Attr2 Reduce Faint Duration
INT	Obj_Character::GetReduceFaintDurationRefix(VOID)
{
	if(TRUE==GetReduceFaintDurationRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION, nValue);
		ClearReduceFaintDurationRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION);
}
VOID Obj_Character::SetReduceFaintDurationRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION, nValue);
	MarkReduceFaintDurationRefixDirtyFlag();
}
// Attr2 Reduce Poisoned Duration
INT	Obj_Character::GetReducePoisonedDurationRefix(VOID)
{
	if(TRUE==GetReducePoisonedDurationRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION, nValue);
		ClearReducePoisonedDurationRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION);
}
VOID Obj_Character::SetReducePoisonedDurationRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION, nValue);
	MarkReducePoisonedDurationRefixDirtyFlag();
}
// Attr2 VisionRange
INT	Obj_Character::GetVisionRangeRefix(VOID)
{
	if(TRUE==GetVisionRangeRefixDirtyFlag())
	{
		INT nValue = 0;
		Impact_GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_VISION_RANGE, nValue);
		SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_VISION_RANGE, nValue);
		ClearVisionRangeRefixDirtyFlag();
	}
	return GetIntAttrRefix(CharIntAttrRefixs_T::REFIX_VISION_RANGE);
}
VOID Obj_Character::SetVisionRangeRefix(INT const nValue)
{
	SetIntAttrRefix(CharIntAttrRefixs_T::REFIX_VISION_RANGE, nValue);
	MarkVisionRangeRefixDirtyFlag();
}

//reset data
//整数属性
VOID Obj_Character::ResetIntAttrs(VOID)
{
	m_IntAttrs.ResetAttrs();
	m_IntAttrs.SetAttr(CharIntAttrs_T::ATTR_CAMP, INVALID_ID);
	m_IntAttrs.SetAttr(CharIntAttrs_T::ATTR_MODEL_ID, INVALID_ID);
	MarkAllIntAttrDirtyFlags();
}

//整数属性修正
VOID Obj_Character::ResetIntAttrRefixs(VOID)
{
	m_IntAttrRefixs.ResetAttrs();
	m_IntAttrRefixs.SetAttr(CharIntAttrRefixs_T::REFIX_CAMP, INVALID_ID);
	m_IntAttrRefixs.SetAttr(CharIntAttrRefixs_T::REFIX_MODEL_ID, INVALID_ID);
	MarkAllIntAttrRefixDirtyFlags();
}

//BOOL属性
VOID Obj_Character::ResetBoolAttr(VOID)
{
	m_BoolAttrs.ResetAttrs();
	MarkAllBoolAttrDirtyFlags();
}

