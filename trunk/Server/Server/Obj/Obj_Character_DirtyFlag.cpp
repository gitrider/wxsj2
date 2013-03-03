//***************************************************************************************************
// 文件名称:	Obj_Character_DirtyFlag.cpp
// 系统名称:	
// 模块名称:	角色层属性的脏标记操作
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
#include "Obj_Character.h"
//MaxStrikePoint
BOOL Obj_Character::GetMaxStrikePointDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAX_STRIKE_POINT)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT);
}
VOID Obj_Character::ClearMaxStrikePointDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAX_STRIKE_POINT);
}
VOID Obj_Character::MarkMaxStrikePointDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_STRIKE_POINT);
}
BOOL Obj_Character::GetMaxStrikePointRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT);
}
VOID Obj_Character::ClearMaxStrikePointRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT);
}
VOID Obj_Character::MarkMaxStrikePointRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT);	
}
//Max Rage
BOOL Obj_Character::GetMaxRageDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAX_RAGE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_RAGE);
}
VOID Obj_Character::ClearMaxRageDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAX_RAGE);
}

VOID Obj_Character::MarkMaxRageDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_RAGE);
}

BOOL Obj_Character::GetMaxRageRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_RAGE);
}

VOID Obj_Character::ClearMaxRageRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_RAGE);
}

VOID Obj_Character::MarkMaxRageRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_RAGE);	
}

BOOL Obj_Character::GetRageRegenerateDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_RAGE_REGENERATE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE);
}

VOID Obj_Character::ClearRageRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_RAGE_REGENERATE);
}

VOID Obj_Character::MarkRageRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_RAGE_REGENERATE);
}

BOOL Obj_Character::GetRageRegenerateRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE);
}

VOID Obj_Character::ClearRageRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE);
}

VOID Obj_Character::MarkRageRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_RAGE_REGENERATE);	
}

//Str
BOOL Obj_Character::GetStrDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_STR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_STR);
}
VOID Obj_Character::ClearStrDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_STR);
}
VOID Obj_Character::MarkStrDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_STR);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_NEAR);

}
BOOL Obj_Character::GetStrRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_STR);
}
VOID Obj_Character::ClearStrRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_STR);
}
VOID Obj_Character::MarkStrRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_STR);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_STR);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_NEAR);
}
//Spr
//BOOL Obj_Character::GetSprDirtyFlag(VOID)
//{
//	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_SPR)
//		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_SPR);
//}
//VOID Obj_Character::ClearSprDirtyFlag(VOID)
//{
//	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_SPR);
//}
//VOID Obj_Character::MarkSprDirtyFlag(VOID)
//{
//	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_SPR);
//	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR);
//}
//BOOL Obj_Character::GetSprRefixDirtyFlag(VOID)
//{
//	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_SPR);
//}
//VOID Obj_Character::ClearSprRefixDirtyFlag(VOID)
//{
//	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_SPR);
//}
//VOID Obj_Character::MarkSprRefixDirtyFlag(VOID)
//{
//	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_SPR);	
//	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_SPR);
//	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR);
//}
//Con
BOOL Obj_Character::GetConDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_CON)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_CON);
}
VOID Obj_Character::ClearConDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_CON);
}
VOID Obj_Character::MarkConDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_CON);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_HP);
	//m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_PHY);
}
BOOL Obj_Character::GetConRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_CON);
}
VOID Obj_Character::ClearConRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_CON);
}
VOID Obj_Character::MarkConRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_CON);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_CON);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_HP);
	//m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_PHY);
}
//Int
BOOL Obj_Character::GetIntDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_INT)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_INT);
}
VOID Obj_Character::ClearIntDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_INT);
}
VOID Obj_Character::MarkIntDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_INT);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_MP);
}
BOOL Obj_Character::GetIntRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_INT);
}
VOID Obj_Character::ClearIntRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_INT);
}
VOID Obj_Character::MarkIntRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_INT);	
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_INT);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_MP);
}
//Dex
BOOL Obj_Character::GetDexDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DEX)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEX);
}
VOID Obj_Character::ClearDexDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DEX);
}
VOID Obj_Character::MarkDexDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEX);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_HIT);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MISS);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_CRITICAL);
}
BOOL Obj_Character::GetDexRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEX);
}
VOID Obj_Character::ClearDexRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DEX);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEX);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_HIT);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MISS);
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_CRITICAL);
}
VOID Obj_Character::MarkDexRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DEX);	
}
//MaxHP
BOOL Obj_Character::GetMaxHPDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAX_HP)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_HP);
}
VOID Obj_Character::ClearMaxHPDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAX_HP);
}
VOID Obj_Character::MarkMaxHPDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_HP);
}
BOOL Obj_Character::GetMaxHPRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_HP);
}
VOID Obj_Character::ClearMaxHPRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_HP);
}
VOID Obj_Character::MarkMaxHPRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_HP);	
}
//HP Regenerate
BOOL Obj_Character::GetHPRegenerateDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_HP_REGENERATE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_HP_REGENERATE);
}
VOID Obj_Character::ClearHPRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_HP_REGENERATE);
}
VOID Obj_Character::MarkHPRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_HP_REGENERATE);
}
BOOL Obj_Character::GetHPRegenerateRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_HP_REGENERATE);
}
VOID Obj_Character::ClearHPRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_HP_REGENERATE);
}
VOID Obj_Character::MarkHPRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_HP_REGENERATE);	
}
//MAX MP
BOOL Obj_Character::GetMaxMPDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAX_MP)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_MP);
}
VOID Obj_Character::ClearMaxMPDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAX_MP);
}
VOID Obj_Character::MarkMaxMPDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_MP);
}
BOOL Obj_Character::GetMaxMPRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_MP);
}
VOID Obj_Character::ClearMaxMPRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_MP);
}
VOID Obj_Character::MarkMaxMPRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_MP);	
}
//MP Regenerate
BOOL Obj_Character::GetMPRegenerateDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MP_REGENERATE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MP_REGENERATE);
}
VOID Obj_Character::ClearMPRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MP_REGENERATE);
}
VOID Obj_Character::MarkMPRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MP_REGENERATE);
}
BOOL Obj_Character::GetMPRegenerateRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MP_REGENERATE);
}
VOID Obj_Character::ClearMPRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MP_REGENERATE);
}
VOID Obj_Character::MarkMPRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MP_REGENERATE);	
}

//MAX Pneuma
BOOL Obj_Character::GetMaxPneumaDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAX_PNEUMA)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA);
}
VOID Obj_Character::ClearMaxPneumaDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAX_PNEUMA);
}
VOID Obj_Character::MarkMaxPneumaDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_PNEUMA);
}
BOOL Obj_Character::GetMaxPneumaRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA);
}
VOID Obj_Character::ClearMaxPneumaRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA);
}
VOID Obj_Character::MarkMaxPneumaRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MAX_PNEUMA);	
}
//MP Regenerate
BOOL Obj_Character::GetPneumaRegenerateDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_PNEUMA_REGENERATE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE);
}
VOID Obj_Character::ClearPneumaRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_PNEUMA_REGENERATE);
}
VOID Obj_Character::MarkPneumaRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_PNEUMA_REGENERATE);
}
BOOL Obj_Character::GetPneumaRegenerateRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE);
}
VOID Obj_Character::ClearPneumaRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE);
}
VOID Obj_Character::MarkPneumaRegenerateRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_PNEUMA_REGENERATE);	
}

//CampID
BOOL Obj_Character::GetCampIDDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_CAMP)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_CAMP);
}
VOID Obj_Character::ClearCampIDDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_CAMP);
}
VOID Obj_Character::MarkCampIDDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_CAMP);
}
BOOL Obj_Character::GetCampIDRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_CAMP);
}
VOID Obj_Character::ClearCampIDRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_CAMP);
}
VOID Obj_Character::MarkCampIDRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_CAMP);	
}
//ModelID
BOOL Obj_Character::GetModelIDDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MODEL_ID)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MODEL_ID);
}

VOID Obj_Character::ClearModelIDDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MODEL_ID);
}
VOID Obj_Character::MarkModelIDDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MODEL_ID);
}
BOOL Obj_Character::GetModelIDRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MODEL_ID);
}
VOID Obj_Character::ClearModelIDRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MODEL_ID);
}
VOID Obj_Character::MarkModelIDRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MODEL_ID);	
}
//MountID
BOOL Obj_Character::GetMountIDDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MOUNT_ID)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MOUNT_ID);
}
VOID Obj_Character::ClearMountIDDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MOUNT_ID);
}
VOID Obj_Character::MarkMountIDDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MOUNT_ID);
}
BOOL Obj_Character::GetMountIDRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MOUNT_ID);
}
VOID Obj_Character::ClearMountIDRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MOUNT_ID);
}
VOID Obj_Character::MarkMountIDRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MOUNT_ID);	
}
//EXP Refix
BOOL Obj_Character::GetExpRefixDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_EXP_REFIX)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_EXP_REFIX);
}
VOID Obj_Character::ClearExpRefixDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_EXP_REFIX);
}
VOID Obj_Character::MarkExpRefixDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_EXP_REFIX);
}
BOOL Obj_Character::GetExpRefixRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_EXP_REFIX);
}
VOID Obj_Character::ClearExpRefixRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_EXP_REFIX);
}
VOID Obj_Character::MarkExpRefixRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_EXP_REFIX);	
}
//Stealth Level
BOOL Obj_Character::GetStealthLevelDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_STEALTH_LEVEL)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL);
}
VOID Obj_Character::ClearStealthLevelDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_STEALTH_LEVEL);
}
VOID Obj_Character::MarkStealthLevelDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_STEALTH_LEVEL);
}
BOOL Obj_Character::GetStealthLevelRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL);
}
VOID Obj_Character::ClearStealthLevelRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL);
}
VOID Obj_Character::MarkStealthLevelRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL);	
	OnStealthLevelChanged();
}
//Detect Level
BOOL Obj_Character::GetDetectLevelDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DETECT_LEVEL)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL);
}
VOID Obj_Character::ClearDetectLevelDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DETECT_LEVEL);
}
VOID Obj_Character::MarkDetectLevelDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DETECT_LEVEL);
}
BOOL Obj_Character::GetDetectLevelRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL);
}
VOID Obj_Character::ClearDetectLevelRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL);
}
VOID Obj_Character::MarkDetectLevelRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DETECT_LEVEL);
	OnDetectLevelChanged();
}

//Alive
BOOL Obj_Character::GetAliveDirtyFlag(VOID)
{
	return m_BoolAttrDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_ALIVE);
		//|| m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_ALIVE);
}
VOID Obj_Character::ClearAliveDirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_ALIVE);
}
VOID Obj_Character::MarkAliveDirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_ALIVE);
}
/*
BOOL Obj_Character::GetAliveRefixDirtyFlag(VOID)
{
	return m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_ALIVE);
}
VOID Obj_Character::ClearAliveRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_ALIVE);
}
VOID Obj_Character::MarkAliveRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_ALIVE);	
}
*/
//CanMove
BOOL Obj_Character::GetCanMoveDirtyFlag(VOID)
{
	return m_BoolAttrDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_MOVE);
		//|| m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_MOVE);
}
VOID Obj_Character::ClearCanMoveDirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_CAN_MOVE);
}

VOID Obj_Character::MarkCanMoveDirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_CAN_MOVE);
}
/*
BOOL Obj_Character::GetCanMoveRefixDirtyFlag(VOID)
{
	return m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_MOVE);
}
VOID Obj_Character::ClearCanMoveRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_CAN_MOVE);
}
VOID Obj_Character::MarkCanMoveRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_CAN_MOVE);	
}
*/
//CanAction Flag 1
BOOL Obj_Character::GetCanAction1DirtyFlag(VOID)
{
	return m_BoolAttrDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION1);
		//|| m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION2);
}
VOID Obj_Character::ClearCanAction1DirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION1);
}
VOID Obj_Character::MarkCanAction1DirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION1);
}
//CanAction Flag 2
BOOL Obj_Character::GetCanAction2DirtyFlag(VOID)
{
	return m_BoolAttrDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION2);
		//|| m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION2);
}
VOID Obj_Character::ClearCanAction2DirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION2);
}
VOID Obj_Character::MarkCanAction2DirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION2);
}
/*
BOOL Obj_Character::GetCanActionRefixDirtyFlag(VOID)
{
	return m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION);
}
VOID Obj_Character::ClearCanActionRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION);
}
VOID Obj_Character::MarkCanActionRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_CAN_ACTION);	
}
*/
//Unbreakable
BOOL Obj_Character::GetUnbreakableDirtyFlag(VOID)
{
	return m_BoolAttrDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_UNBREAKABLE);
		//|| m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_UNBREAKABLE);
}
VOID Obj_Character::ClearUnbreakableDirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_UNBREAKABLE);
}
VOID Obj_Character::MarkUnbreakableDirtyFlag(VOID)
{
	m_BoolAttrDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_UNBREAKABLE);
}
/*
BOOL Obj_Character::GetUnbreakableRefixDirtyFlag(VOID)
{
	return m_BoolAttrRefixDirtyFlags.GetFlagByIndex(CharBoolAttrs_T::ATTR_UNBREAKABLE);
}
VOID Obj_Character::ClearUnbreakableRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.ClearFlagByIndex(CharBoolAttrs_T::ATTR_UNBREAKABLE);
}
VOID Obj_Character::MarkUnbreakableRefixDirtyFlag(VOID)
{
	m_BoolAttrRefixDirtyFlags.MarkFlagByIndex(CharBoolAttrs_T::ATTR_UNBREAKABLE);	
}
*/
//Move Speed
BOOL Obj_Character::GetMoveSpeedDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MOVE_SPEED)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVE_SPEED);
}
VOID Obj_Character::ClearMoveSpeedDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MOVE_SPEED);
}	
VOID Obj_Character::MarkMoveSpeedDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MOVE_SPEED);
}
BOOL Obj_Character::GetMoveSpeedRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVE_SPEED);
}
VOID Obj_Character::ClearMoveSpeedRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVE_SPEED);
}
VOID Obj_Character::MarkMoveSpeedRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVE_SPEED);	
}
//Attack Speed
BOOL Obj_Character::GetAttackSpeedDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_SPEED)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED);
}
VOID Obj_Character::ClearAttackSpeedDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_SPEED);
}
VOID Obj_Character::MarkAttackSpeedDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_SPEED);
}
BOOL Obj_Character::GetAttackSpeedRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED);
}
VOID Obj_Character::ClearAttackSpeedRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED);
}
VOID Obj_Character::MarkAttackSpeedRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SPEED);	
}
//Hit
BOOL Obj_Character::GetHitDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_HIT)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_HIT);
}
VOID Obj_Character::ClearHitDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_HIT);
}
VOID Obj_Character::MarkHitDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_HIT);
}
BOOL Obj_Character::GetHitRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_HIT);
}
VOID Obj_Character::ClearHitRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_HIT);
}
VOID Obj_Character::MarkHitRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_HIT);	
}
//Miss
BOOL Obj_Character::GetMissDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MISS)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MISS);
}
VOID Obj_Character::ClearMissDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MISS);
}
VOID Obj_Character::MarkMissDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MISS);
}
BOOL Obj_Character::GetMissRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MISS);
}
VOID Obj_Character::ClearMissRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MISS);
}
VOID Obj_Character::MarkMissRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MISS);	
}
//Critical
BOOL Obj_Character::GetCriticalDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_CRITICAL)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_CRITICAL);
}
VOID Obj_Character::ClearCriticalDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_CRITICAL);
}
VOID Obj_Character::MarkCriticalDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_CRITICAL);
}
BOOL Obj_Character::GetCriticalRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_CRITICAL);
}
VOID Obj_Character::ClearCriticalRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_CRITICAL);
}
VOID Obj_Character::MarkCriticalRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_CRITICAL);	
}
//Attack Physics
//BOOL Obj_Character::GetAttackPhysicsDirtyFlag(VOID)
//{
//	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_PHY)
//		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_PHY);
//}
//VOID Obj_Character::ClearAttackPhysicsDirtyFlag(VOID)
//{
//	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_PHY);
//}
//VOID Obj_Character::MarkAttackPhysicsDirtyFlag(VOID)
//{
//	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_PHY);
//}
//BOOL Obj_Character::GetAttackPhysicsRefixDirtyFlag(VOID)
//{
//	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_PHY);
//}
//VOID Obj_Character::ClearAttackPhysicsRefixDirtyFlag(VOID)
//{
//	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_PHY);
//}
//VOID Obj_Character::MarkAttackPhysicsRefixDirtyFlag(VOID)
//{
//	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_PHY);	
//}
////Defence Physics
//BOOL Obj_Character::GetDefencePhysicsDirtyFlag(VOID)
//{
//	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_PHY)
//		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY);
//}
//VOID Obj_Character::ClearDefencePhysicsDirtyFlag(VOID)
//{
//	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_PHY);
//}
//VOID Obj_Character::MarkDefencePhysicsDirtyFlag(VOID)
//{
//	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_PHY);
//}
//BOOL Obj_Character::GetDefencePhysicsRefixDirtyFlag(VOID)
//{
//	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY);
//}
//VOID Obj_Character::ClearDefencePhysicsRefixDirtyFlag(VOID)
//{
//	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY);
//}
//VOID Obj_Character::MarkDefencePhysicsRefixDirtyFlag(VOID)
//{
//	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_PHY);	
//}

//add by liam----------------------------------------------
//近程防御
BOOL Obj_Character::GetDefenceNearDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_NEAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR);
}

VOID Obj_Character::ClearDefenceNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_NEAR);
}

VOID Obj_Character::MarkDefenceNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_NEAR);
}

BOOL Obj_Character::GetDefenceNearRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR);
}

VOID Obj_Character::ClearDefenceNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR);
}

VOID Obj_Character::MarkDefenceNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR);
}

//远程防御
BOOL Obj_Character::GetDefenceFarDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_FAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR);
}

VOID Obj_Character::ClearDefenceFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_FAR);
}

VOID Obj_Character::MarkDefenceFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_FAR);
}

BOOL Obj_Character::GetDefenceFarRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR);
}

VOID Obj_Character::ClearDefenceFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR);
}

VOID Obj_Character::MarkDefenceFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_FAR);
}

//近程攻击
BOOL Obj_Character::GetAttackNearDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_NEAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR);
}

VOID Obj_Character::ClearAttackNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_NEAR);
}

VOID Obj_Character::MarkAttackNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_NEAR);
}

BOOL Obj_Character::GetAttackNearRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR);
}

VOID Obj_Character::ClearAttackNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR);
}

VOID Obj_Character::MarkAttackNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_NEAR);
}

//远程攻击
BOOL Obj_Character::GetAttackFarDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_FAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FAR);
}

VOID Obj_Character::ClearAttackFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_FAR);
}

VOID Obj_Character::MarkAttackFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_FAR);
}

BOOL Obj_Character::GetAttackFarRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FAR);
}

VOID Obj_Character::ClearAttackFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FAR);
}

VOID Obj_Character::MarkAttackFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FAR);
}

//韧性
BOOL Obj_Character::GetToughnessDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_TOUGHNESS)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_TOUGHNESS);
}

VOID Obj_Character::ClearToughnessDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_TOUGHNESS);
}

VOID Obj_Character::MarkToughnessDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_TOUGHNESS);
}

BOOL Obj_Character::GetToughnessRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_TOUGHNESS);
}

VOID Obj_Character::ClearToughnessRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_TOUGHNESS);
}

VOID Obj_Character::MarkToughnessRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_TOUGHNESS);
}

//近程抗性
BOOL Obj_Character::GetNearAttReduceDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_NEARATT_REDUCE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE);
}

VOID Obj_Character::ClearNearAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_NEARATT_REDUCE);
}

VOID Obj_Character::MarkNearAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_NEARATT_REDUCE);
}

BOOL Obj_Character::GetNearAttReduceRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE);
}

VOID Obj_Character::ClearNearAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE);
}

VOID Obj_Character::MarkNearAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_NEARATT_REDUCE);
}

//远程抗性
BOOL Obj_Character::GetFarAttReduceDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_FARATT_REDUCE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE);
}

VOID Obj_Character::ClearFarAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_FARATT_REDUCE);
}

VOID Obj_Character::MarkFarAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_FARATT_REDUCE);
}

BOOL Obj_Character::GetFarAttReduceRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE);
}

VOID Obj_Character::ClearFarAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE);
}

VOID Obj_Character::MarkFarAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_FARATT_REDUCE);
}

//近程内功抗性
BOOL Obj_Character::GetMagicNearAttReduceDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAGIC_NEAR_ATT_REDUCE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE);
}
BOOL Obj_Character::GetMagicFarAttReduceDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAGIC_FAR_ATT_REDUCE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE);
}

VOID Obj_Character::ClearMagicNearAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAGIC_NEAR_ATT_REDUCE);
}
VOID Obj_Character::ClearMagicFarAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAGIC_FAR_ATT_REDUCE);
}

VOID Obj_Character::MarkMagicNearAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAGIC_NEAR_ATT_REDUCE);
}
VOID Obj_Character::MarkMagicFarAttReduceDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAGIC_FAR_ATT_REDUCE);
}

BOOL Obj_Character::GetMagicNearAttReduceRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE);
}
BOOL Obj_Character::GetMagicFarAttReduceRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE);
}

VOID Obj_Character::ClearMagicNearAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE);
}
VOID Obj_Character::ClearMagicFarAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE);
}

VOID Obj_Character::MarkMagicNearAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_NEAR_ATT_REDUCE);
}
VOID Obj_Character::MarkMagicFarAttReduceRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MAGIC_FAR_ATT_REDUCE);
}

//恐惧抗性
BOOL Obj_Character::GetDreadAttResistDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DREAD_RESIST)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DREAD_RESIST);
}

VOID Obj_Character::ClearDreadAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DREAD_RESIST);
}

VOID Obj_Character::MarkDreadAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DREAD_RESIST);
}

BOOL Obj_Character::GetDreadAttResistRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DREAD_RESIST);
}

VOID Obj_Character::ClearDreadAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DREAD_RESIST);
}

VOID Obj_Character::MarkDreadAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DREAD_RESIST);
}

//昏迷抗性
BOOL Obj_Character::GetComaAttResistDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_COMA_RESIST)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_COMA_RESIST);
}

VOID Obj_Character::ClearComaAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_COMA_RESIST);
}

VOID Obj_Character::MarkComaAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_COMA_RESIST);
}

BOOL Obj_Character::GetComaAttResistRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_COMA_RESIST);
}

VOID Obj_Character::ClearComaAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_COMA_RESIST);
}

VOID Obj_Character::MarkComaAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_COMA_RESIST);
}

//沉默抗性
BOOL Obj_Character::GetHushAttResistDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_HUSH_RESIST)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_HUSH_RESIST);
}

VOID Obj_Character::ClearHushAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_HUSH_RESIST);
}

VOID Obj_Character::MarkHushAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_HUSH_RESIST);
}

BOOL Obj_Character::GetHushAttResistRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_HUSH_RESIST);
}

VOID Obj_Character::ClearHushAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_HUSH_RESIST);
}

VOID Obj_Character::MarkHushAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_HUSH_RESIST);
}

//缴械抗性
BOOL Obj_Character::GetUnArmyAttResistDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_UNARMY_RESIST)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST);
}

VOID Obj_Character::ClearUnArmyAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_UNARMY_RESIST);
}

VOID Obj_Character::MarkUnArmyAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_UNARMY_RESIST);
}

BOOL Obj_Character::GetUnArmyAttResistRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST);
}

VOID Obj_Character::ClearUnArmyAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST);
}

VOID Obj_Character::MarkUnArmyAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_UNARMY_RESIST);
}

//减攻速抗性
BOOL Obj_Character::GetAttSpeedAttResistDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACKSPEED_RESIST)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST);
}

VOID Obj_Character::ClearAttSpeedAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACKSPEED_RESIST);
}

VOID Obj_Character::MarkAttSpeedAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACKSPEED_RESIST);
}

BOOL Obj_Character::GetAttSpeedAttResistRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST);
}

VOID Obj_Character::ClearAttSpeedAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST);
}

VOID Obj_Character::MarkAttSpeedAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACKSPEED_RESIST);
}

//减施速抗性
BOOL Obj_Character::GetSkillSpeedAttResistDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_SKILLSPEED_RESIST)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST);
}

VOID Obj_Character::ClearSkillSpeedAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_SKILLSPEED_RESIST);
}

VOID Obj_Character::MarkSkillSpeedAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_SKILLSPEED_RESIST);
}

BOOL Obj_Character::GetSkillSpeedAttResistRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST);
}

VOID Obj_Character::ClearSkillSpeedAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST);
}

VOID Obj_Character::MarkSkillSpeedAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_SKILLSPEED_RESIST);
}

//减移速抗性
BOOL Obj_Character::GetMoveSpeedAttResistDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MOVEREDUCE_RESIST)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST);
}

VOID Obj_Character::ClearMoveSpeedAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MOVEREDUCE_RESIST);
}

VOID Obj_Character::MarkMoveSpeedAttResistDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MOVEREDUCE_RESIST);
}

BOOL Obj_Character::GetMoveSpeedAttResistRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST);
}

VOID Obj_Character::ClearMoveSpeedAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST);
}

VOID Obj_Character::MarkMoveSpeedAttResistRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_MOVEREDUCE_RESIST);
}
//---------------------------------------------------------

//Attack 
BOOL Obj_Character::GetAttackMagicNearDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR);
}
BOOL Obj_Character::GetAttackMagicFarDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR);
}

VOID Obj_Character::ClearAttackMagicNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR);
}
VOID Obj_Character::ClearAttackMagicFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR);
}

VOID Obj_Character::MarkAttackMagicNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR);
}
VOID Obj_Character::MarkAttackMagicFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR);
}

BOOL Obj_Character::GetAttackMagicNearRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR);
}
BOOL Obj_Character::GetAttackMagicFarRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR);
}

VOID Obj_Character::ClearAttackMagicNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR);
}
VOID Obj_Character::ClearAttackMagicFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR);
}

VOID Obj_Character::MarkAttackMagicNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR);	
}
VOID Obj_Character::MarkAttackMagicFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR);	
}

//Defence 
BOOL Obj_Character::GetDefenceMagicNearDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_NEAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR);
}

BOOL Obj_Character::GetDefenceMagicFarDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_FAR)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR);
}

VOID Obj_Character::ClearDefenceMagicNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_NEAR);
}
VOID Obj_Character::ClearDefenceMagicFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_FAR);
}

VOID Obj_Character::MarkDefenceMagicNearDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_NEAR);
}
VOID Obj_Character::MarkDefenceMagicFarDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_FAR);
}

BOOL Obj_Character::GetDefenceMagicNearRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR);
}
BOOL Obj_Character::GetDefenceMagicFarRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR);
}

VOID Obj_Character::ClearDefenceMagicNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR);
}
VOID Obj_Character::ClearDefenceMagicFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR);
}

VOID Obj_Character::MarkDefenceMagicNearRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR);	
}
VOID Obj_Character::MarkDefenceMagicFarRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR);	
}
//Attack gold
BOOL Obj_Character::GetAttackGoldDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_GOLD)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD);
}
VOID Obj_Character::ClearAttackGoldDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_GOLD);
}
VOID Obj_Character::MarkAttackGoldDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_GOLD);
}
BOOL Obj_Character::GetAttackGoldRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD);
}
VOID Obj_Character::ClearAttackGoldRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD);
}
VOID Obj_Character::MarkAttackGoldRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_GOLD);	
}
//Resist gold
BOOL Obj_Character::GetResistGoldDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_RESIST_GOLD)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_GOLD);
}
VOID Obj_Character::ClearResistGoldDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_RESIST_GOLD);
}
VOID Obj_Character::MarkResistGoldDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_RESIST_GOLD);
}
BOOL Obj_Character::GetResistGoldRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_GOLD);
}
VOID Obj_Character::ClearResistGoldRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_GOLD);
}
VOID Obj_Character::MarkResistGoldRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_GOLD);	
}

//Attack Wood
BOOL Obj_Character::GetAttackWoodDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_WOOD)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD);
}
VOID Obj_Character::ClearAttackWoodDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_WOOD);
}
VOID Obj_Character::MarkAttackWoodDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_WOOD);
}
BOOL Obj_Character::GetAttackWoodRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD);
}
VOID Obj_Character::ClearAttackWoodRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD);
}
VOID Obj_Character::MarkAttackWoodRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WOOD);	
}
//Resist Wood
BOOL Obj_Character::GetResistWoodDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_RESIST_WOOD)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WOOD);
}
VOID Obj_Character::ClearResistWoodDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_RESIST_WOOD);
}
VOID Obj_Character::MarkResistWoodDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_RESIST_WOOD);
}
BOOL Obj_Character::GetResistWoodRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WOOD);
}
VOID Obj_Character::ClearResistWoodRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WOOD);
}
VOID Obj_Character::MarkResistWoodRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WOOD);	
}

//Attack Water
BOOL Obj_Character::GetAttackWaterDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_WATER)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WATER);
}
VOID Obj_Character::ClearAttackWaterDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_WATER);
}
VOID Obj_Character::MarkAttackWaterDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_WATER);
}
BOOL Obj_Character::GetAttackWaterRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WATER);
}
VOID Obj_Character::ClearAttackWaterRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WATER);
}
VOID Obj_Character::MarkAttackWaterRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_WATER);	
}
//Resist Water
BOOL Obj_Character::GetResistWaterDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_RESIST_WATER)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WATER);
}
VOID Obj_Character::ClearResistWaterDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_RESIST_WATER);
}
VOID Obj_Character::MarkResistWaterDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_RESIST_WATER);
}
BOOL Obj_Character::GetResistWaterRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WATER);
}
VOID Obj_Character::ClearResistWaterRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WATER);
}
VOID Obj_Character::MarkResistWaterRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_WATER);	
}

//Attack Fire
BOOL Obj_Character::GetAttackFireDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_FIRE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE);
}
VOID Obj_Character::ClearAttackFireDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_FIRE);
}
VOID Obj_Character::MarkAttackFireDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_FIRE);
}
BOOL Obj_Character::GetAttackFireRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE);
}
VOID Obj_Character::ClearAttackFireRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE);
}
VOID Obj_Character::MarkAttackFireRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_FIRE);	
}
//Resist Fire
BOOL Obj_Character::GetResistFireDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_RESIST_FIRE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_FIRE);
}
VOID Obj_Character::ClearResistFireDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_RESIST_FIRE);
}
VOID Obj_Character::MarkResistFireDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_RESIST_FIRE);
}
BOOL Obj_Character::GetResistFireRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_FIRE);
}
VOID Obj_Character::ClearResistFireRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_FIRE);
}
VOID Obj_Character::MarkResistFireRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_FIRE);	
}

//Attack Soil
BOOL Obj_Character::GetAttackSoilDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_SOIL)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL);
}
VOID Obj_Character::ClearAttackSoilDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_SOIL);
}
VOID Obj_Character::MarkAttackSoilDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ATTACK_SOIL);
}
BOOL Obj_Character::GetAttackSoilRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL);
}
VOID Obj_Character::ClearAttackSoilRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL);
}
VOID Obj_Character::MarkAttackSoilRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_ATTACK_SOIL);	
}
//Resist Soil
BOOL Obj_Character::GetResistSoilDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_RESIST_SOIL)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_SOIL);
}
VOID Obj_Character::ClearResistSoilDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_RESIST_SOIL);
}
VOID Obj_Character::MarkResistSoilDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_RESIST_SOIL);
}
BOOL Obj_Character::GetResistSoilRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_SOIL);
}
VOID Obj_Character::ClearResistSoilRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_SOIL);
}
VOID Obj_Character::MarkResistSoilRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_RESIST_SOIL);	
}

//Reduce Slower Duration
BOOL Obj_Character::GetReduceSlowerDurationDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_SLOWER_DURATION)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION);
}
VOID Obj_Character::ClearReduceSlowerDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_SLOWER_DURATION);
}
VOID Obj_Character::MarkReduceSlowerDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_SLOWER_DURATION);
}
BOOL Obj_Character::GetReduceSlowerDurationRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION);
}
VOID Obj_Character::ClearReduceSlowerDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION);
}
VOID Obj_Character::MarkReduceSlowerDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_SLOWER_DURATION);	
}
//Reduce Weaken Duration
BOOL Obj_Character::GetReduceWeakenDurationDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_WEAKEN_DURATION)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION);
}
VOID Obj_Character::ClearReduceWeakenDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_WEAKEN_DURATION);
}
VOID Obj_Character::MarkReduceWeakenDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_WEAKEN_DURATION);
}
BOOL Obj_Character::GetReduceWeakenDurationRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION);
}
VOID Obj_Character::ClearReduceWeakenDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION);
}
VOID Obj_Character::MarkReduceWeakenDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_WEAKEN_DURATION);	
}
//Reduce Faint Duration
BOOL Obj_Character::GetReduceFaintDurationDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_FAINT_DURATION)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION);
}
VOID Obj_Character::ClearReduceFaintDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_FAINT_DURATION);
}
VOID Obj_Character::MarkReduceFaintDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_FAINT_DURATION);
}
BOOL Obj_Character::GetReduceFaintDurationRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION);
}
VOID Obj_Character::ClearReduceFaintDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION);
}
VOID Obj_Character::MarkReduceFaintDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_FAINT_DURATION);	
}
//Reduce Poison Duration
BOOL Obj_Character::GetReducePoisonedDurationDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_POISONED_DURATION)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION);
}
VOID Obj_Character::ClearReducePoisonedDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_POISONED_DURATION);
}
VOID Obj_Character::MarkReducePoisonedDurationDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_REDUCE_POISONED_DURATION);
}
BOOL Obj_Character::GetReducePoisonedDurationRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION);
}
VOID Obj_Character::ClearReducePoisonedDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION);
}
VOID Obj_Character::MarkReducePoisonedDurationRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_REDUCE_POISONED_DURATION);	
}
//Vision Range
BOOL Obj_Character::GetVisionRangeDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_VISION_RANGE)
		|| m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_VISION_RANGE);
}
VOID Obj_Character::ClearVisionRangeDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_VISION_RANGE);
}
VOID Obj_Character::MarkVisionRangeDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_VISION_RANGE);
}
BOOL Obj_Character::GetVisionRangeRefixDirtyFlag(VOID)
{
	return m_IntAttrRefixDirtyFlags.GetFlagByIndex(CharIntAttrRefixs_T::REFIX_VISION_RANGE);
}
VOID Obj_Character::ClearVisionRangeRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.ClearFlagByIndex(CharIntAttrRefixs_T::REFIX_VISION_RANGE);
}
VOID Obj_Character::MarkVisionRangeRefixDirtyFlag(VOID)
{
	m_IntAttrRefixDirtyFlags.MarkFlagByIndex(CharIntAttrRefixs_T::REFIX_VISION_RANGE);	
}

//Energy
BOOL Obj_Character::GetMaxEnergyDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAX_ENERGY);
}
VOID Obj_Character::ClearMaxEnergyDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAX_ENERGY);
}
VOID Obj_Character::MarkMaxEnergyDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_ENERGY);
}
//Vigor
BOOL Obj_Character::GetMaxVigorDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_MAX_VIGOR);
}
VOID Obj_Character::ClearMaxVigorDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_MAX_VIGOR);
}
VOID Obj_Character::MarkMaxVigorDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_MAX_VIGOR);
}
//Energy Regenerate
BOOL Obj_Character::GetEnergyRegenerateDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_ENERGY_REGENERATE);
}
VOID Obj_Character::ClearEnergyRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_ENERGY_REGENERATE);
}
VOID Obj_Character::MarkEnergyRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_ENERGY_REGENERATE);
}
//Vigor Regenerate
BOOL Obj_Character::GetVigorRegenerateDirtyFlag(VOID)
{
	return m_IntAttrDirtyFlags.GetFlagByIndex(CharIntAttrs_T::ATTR_VIGOR_REGENERATE);
}
VOID Obj_Character::ClearVigorRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.ClearFlagByIndex(CharIntAttrs_T::ATTR_VIGOR_REGENERATE);
}
VOID Obj_Character::MarkVigorRegenerateDirtyFlag(VOID)
{
	m_IntAttrDirtyFlags.MarkFlagByIndex(CharIntAttrs_T::ATTR_VIGOR_REGENERATE);
}