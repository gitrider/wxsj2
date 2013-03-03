///////////////////////////////////////////////////////////////////////////////
// 文件名：Obj_Monster_Attributes.cpp
// 程序员：liam
// 功能说明：角色层属性
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Type.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "Obj_Monster.h"
#include "GameTable.h"
#include "AI_Monster.h"

////////////////////////////////////////////////////////////////////////////////
//角色属性部分
//StrikePoint
INT	Obj_Monster::GetMaxStrikePoint(VOID)
{
	return 0;
}
INT Obj_Monster::GetMaxRage(VOID)
{
	return 0;
}

INT	Obj_Monster::GetBaseMaxRage(VOID)
{
	return 0;
}

VOID 	Obj_Monster::SetBaseMaxRage(INT const nRage)
{
}


INT	Obj_Monster::GetRageRegenerate(VOID)
{
	return 0;
} 

INT Obj_Monster::GetBaseRageRegenerate(VOID)
{
	return 0;
}

VOID Obj_Monster::SetBaseRageRegenerate(INT const nValue)
{
}

//Attr1: Str
INT	Obj_Monster::GetStr(VOID)
{
	return 0;
}
INT	Obj_Monster::GetBaseStr(VOID)
{
	return 0;
}
VOID Obj_Monster::SetBaseStr(INT const nValue)
{
	//don't modify this attribute
}
//Attr1: Spr
//INT	Obj_Monster::GetSpr(VOID)
//{
//	return 0;
//}
//INT	Obj_Monster::GetBaseSpr(VOID)
//{
//	return 0;
//}
//VOID Obj_Monster::SetBaseSpr(INT const nValue)
//{
//	//don't modify this attribute
//}
//Attr1: Con
INT	Obj_Monster::GetCon(VOID)
{
	return 0;
}
INT	Obj_Monster::GetBaseCon(VOID)
{
	return 0;
}
VOID Obj_Monster::SetBaseCon(INT const nValue)
{
	//don't modify this attribute
}
//Attr1: Int
INT	Obj_Monster::GetInt(VOID)
{
	return 0;
}
INT	Obj_Monster::GetBaseInt(VOID)
{
	return 0;
}
VOID Obj_Monster::SetBaseInt(INT const nValue)
{
	//don't modify this attribute
}
//Attr1: Dex
INT	Obj_Monster::GetDex(VOID)
{
	return 0;
}
INT	Obj_Monster::GetBaseDex(VOID)
{
	return 0;
}
VOID Obj_Monster::SetBaseDex(INT const nValue)
{
	//don't modify this attribute
}

INT Obj_Monster::GetMaxHP(VOID) 
{
__ENTER_FUNCTION

	if(TRUE==GetMaxHPDirtyFlag())
	{
		INT nValue=GetBaseMaxHP()+GetMaxHPRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_MAX_HP, nValue);
		ClearMaxHPDirtyFlag();
		if(GetHP()>nValue) SetHP(nValue);
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_HP);

__LEAVE_FUNCTION

	return 1 ;
}

INT Obj_Monster::GetBaseMaxHP(VOID)
{
__ENTER_FUNCTION

	return m_iMaxHP;

__LEAVE_FUNCTION

	return 1 ;
}

VOID Obj_Monster::SetBaseMaxHP(INT const nHp)
{ 
	m_iMaxHP = nHp;
}

INT	Obj_Monster::GetHPRegenerate(VOID) 
{
__ENTER_FUNCTION

	if(TRUE==GetHPRegenerateDirtyFlag())
	{
		INT nValue=GetBaseHPRegenerate()+GetHPRegenerateRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_HP_REGENERATE, nValue);
		ClearHPRegenerateDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_HP_REGENERATE);

__LEAVE_FUNCTION

	return 1 ;
}
 
INT Obj_Monster::GetBaseHPRegenerate(VOID)
{
__ENTER_FUNCTION

	return m_iHPRestore;	

__LEAVE_FUNCTION

	return 1 ;
}

VOID 	Obj_Monster::SetBaseHPRegenerate(INT const nValue)
{
	m_iHPRestore = nValue;
}

//MP
INT Obj_Monster::GetMaxMP(VOID)
{
__ENTER_FUNCTION

	if(TRUE==GetMaxMPDirtyFlag())
	{
		INT nValue=GetBaseMaxMP()+GetMaxMPRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_MAX_MP, nValue);
		ClearMaxMPDirtyFlag();
		if(GetMP()>nValue) SetMP(nValue);
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_MP);

__LEAVE_FUNCTION

	return 1;
}

INT Obj_Monster::GetBaseMaxMP(VOID)
{
	return m_iMaxMP;
}

VOID Obj_Monster::SetBaseMaxMp(INT const nMp)
{
	m_iMaxMP = nMp;
}

INT Obj_Monster::GetMPRegenerate(VOID)
{
__ENTER_FUNCTION
	if(TRUE==GetMPRegenerateDirtyFlag())
	{
		INT nValue=GetBaseMPRegenerate()+GetMPRegenerateRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_MP_REGENERATE, nValue);
		ClearMPRegenerateDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MP_REGENERATE);
__LEAVE_FUNCTION
	return 1 ;
}
 
INT Obj_Monster::GetBaseMPRegenerate(VOID)
{
	return m_iMPRestore;
}

VOID 	Obj_Monster::SetBaseMPRegenerate(INT const nValue)
{
	m_iMPRestore = nValue;
}

const _CAMP_DATA *Obj_Monster::GetBaseCampData(VOID)const
{
	return &m_CampData;
}

VOID Obj_Monster::SetBaseCampData(const _CAMP_DATA *pCampData)
{
	m_CampData = *pCampData;
}

//CampID
INT Obj_Monster::GetCampID(VOID)
{
__ENTER_FUNCTION

	if(TRUE==GetCampIDDirtyFlag())
	{
		INT nValue=GetCampIDRefix();
		if(INVALID_ID==nValue)
		{
			nValue=GetBaseCampID();
		}
		SetIntAttr(CharIntAttrs_T::ATTR_CAMP, nValue);
		ClearCampIDDirtyFlag();			
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_CAMP);

__LEAVE_FUNCTION

	return 0 ;
}

INT Obj_Monster::GetBaseCampID(VOID) const
{
	//return 1;//临时测试
	return m_CampData.m_nCampID;
}

VOID	Obj_Monster::SetBaseCampID(INT const nID)
{
__ENTER_FUNCTION
	m_CampData.m_nCampID = nID;
	MarkCampIDDirtyFlag();
__LEAVE_FUNCTION
}

//modelID
INT Obj_Monster::GetModelID(VOID) 
{
__ENTER_FUNCTION

	if(TRUE==GetModelIDDirtyFlag())
	{
		INT nValue=GetModelIDRefix();
		if(INVALID_ID==nValue)
		{
			nValue=GetBaseModelID();
		}
		SetIntAttr(CharIntAttrs_T::ATTR_MODEL_ID, nValue);
		ClearModelIDDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MODEL_ID);
__LEAVE_FUNCTION
	return -1 ;
}

INT Obj_Monster::GetBaseModelID(VOID) const
{
	return -1; // need modify	
}

VOID	Obj_Monster::SetBaseModelID(INT const nID)
{ // forbiden modify this attribute
}
// Alive flag, wraped in ObjCharacter
// In Combat flag, wraped in ObjCharacter
// Can move flag, wraped in ObjCharacter
// Can Action flag, wraped in ObjCharacter
// Unbreakable flag, wraped in ObjCharacter
BOOL	Obj_Monster::IsUnbreakable(VOID)
{
__ENTER_FUNCTION

  //  if (GetMonsterAI()->AIParam(AIPARAM_CANNOTATTACK) > 0)
		//return TRUE;

	return Obj_Character::IsUnbreakable();
__LEAVE_FUNCTION
	return FALSE ;
}

// Attr2: Move Speed, wraped in ObjCharacter
FLOAT	Obj_Monster::GetMoveSpeed(VOID)
{
__ENTER_FUNCTION

	if(TRUE==GetMoveSpeedDirtyFlag())
	{
		INT nValue=GetBaseMoveSpeed()+GetMoveSpeedRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_MOVE_SPEED, nValue);
		ClearMoveSpeedDirtyFlag();
	}
	FLOAT fMoveSpeed = GetIntAttr(CharIntAttrs_T::ATTR_MOVE_SPEED)/1000.0f;
	ENUM_MOVE_MODE eMoveMode = GetMoveMode();
	if(eMoveMode == MOVE_MODE_HOBBLE)
	{//变成基础速度的50%
		fMoveSpeed = fMoveSpeed*0.5f;
	}
	else if(eMoveMode == MOVE_MODE_RUN)
	{//变成基础速度的150%
		fMoveSpeed = fMoveSpeed*1.5f;
	}
	return fMoveSpeed;
__LEAVE_FUNCTION
	return 1.0f ;
}

INT	Obj_Monster::GetBaseMoveSpeed(VOID)
{
__ENTER_FUNCTION

	return g_MonsterAttrExTbl.Get( m_uDataID, CATTR_LEVEL2_SPEED) ;	
__LEAVE_FUNCTION
	return 1 ;
}

VOID	Obj_Monster::SetBaseMoveSpeed(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack Speed
INT Obj_Monster::GetAttackSpeed(VOID)
{
__ENTER_FUNCTION

	if(TRUE==GetAttackSpeedDirtyFlag())
	{
		INT nValue=GetBaseAttackSpeed()+GetAttackSpeedRefix();
		if(400<nValue)
		{
			nValue = 400;
		}
		if(25>=nValue)
		{
			nValue = 25;
		}		
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SPEED, nValue);
		ClearAttackSpeedDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SPEED);
__LEAVE_FUNCTION
	return 1 ;
}

INT Obj_Monster::GetBaseAttackSpeed(VOID)
{
	INT nAttackSpeed = g_MonsterAttrExTbl.Get( m_uDataID, CATTR_LEVEL2_ATTACKSPEED);
	if(75<nAttackSpeed)
	{
		nAttackSpeed = 75;
	}
	if(-300>nAttackSpeed)
	{
		nAttackSpeed = -300;
	}
	nAttackSpeed = BASE_ATTACK_SPEED - nAttackSpeed;
	return BASE_ATTACK_SPEED;
}

VOID	Obj_Monster::SetBaseAttackSpeed(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Miss
INT Obj_Monster::GetMiss(VOID)
{
__ENTER_FUNCTION
	if(TRUE==GetMissDirtyFlag())
	{
		INT nValue=GetBaseMiss()+GetMissRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_MISS, nValue);
		ClearMissDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MISS);
__LEAVE_FUNCTION
	return 1 ;
}

INT Obj_Monster::GetBaseMiss(VOID)
{
	return m_iMissRate;	
}

VOID	Obj_Monster::SetBaseMiss(INT const nValue)
{
	m_iMissRate = nValue;
}

// Attr2 Hit
INT Obj_Monster::GetHit(VOID)
{
__ENTER_FUNCTION
	if(TRUE==GetHitDirtyFlag())
	{
		INT nValue=GetBaseHit()+GetHitRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_HIT, nValue);
		ClearHitDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_HIT);
__LEAVE_FUNCTION
	return 1 ;
}

INT Obj_Monster::GetBaseHit(VOID)
{
	return m_iHitRate;
}

VOID	Obj_Monster::SetBaseHit(INT const nValue)
{
	m_iHitRate = nValue;
}

// Attr2 Critical
INT Obj_Monster::GetCritical(VOID)
{
	if(TRUE==GetCriticalDirtyFlag())
	{
		INT nValue=GetBaseCritical()+GetCriticalRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_CRITICAL, nValue);
		ClearCriticalDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_CRITICAL);
}

INT Obj_Monster::GetBaseCritical(VOID)
{
	return m_iCriticalRate;
}

VOID	Obj_Monster::SetBaseCritical(INT const nValue)
{
	m_iCriticalRate = nValue;
}

//add by liam----------------------------------------------------------
INT Obj_Monster::GetDefenceNear(VOID)
{
	if(TRUE==GetDefenceNearDirtyFlag())
	{
		INT nValue=GetBaseDefenceNear()+GetDefenceNearRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_NEAR, nValue);
		ClearDefenceNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_NEAR);
}

INT Obj_Monster::GetBaseDefenceNear (VOID)
{
	return m_iDefenceNear;
}

VOID Obj_Monster::SetBaseDefenceNear (INT const nValue)
{
	m_iDefenceNear = nValue;
}

INT Obj_Monster::GetDefenceFar(VOID)
{
	if(TRUE==GetDefenceFarDirtyFlag())
	{
		INT nValue=GetBaseDefenceFar()+GetDefenceFarRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_FAR, nValue);
		ClearDefenceFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_FAR);
}

INT Obj_Monster::GetBaseDefenceFar (VOID)
{
	return m_iDefenceFar;
}

VOID Obj_Monster::SetBaseDefenceFar (INT const nValue)
{
	m_iDefenceFar = nValue;
}


INT Obj_Monster::GetAttackNear(VOID)
{
	if(TRUE==GetAttackNearDirtyFlag())
	{
		INT nValue=GetBaseAttackNear()+GetAttackNearRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_NEAR, nValue);
		ClearAttackNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_NEAR);
}

INT Obj_Monster::GetBaseAttackNear(VOID)
{
	return m_iAttackNear;	
}

VOID Obj_Monster::SetBaseAttackNear(INT const nValue)
{
	m_iAttackNear = nValue;
}


INT Obj_Monster::GetAttackFar(VOID)
{
	if(TRUE==GetAttackFarDirtyFlag())
	{
		INT nValue=GetBaseAttackFar()+GetAttackFarRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FAR, nValue);
		ClearAttackFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FAR);
}

INT Obj_Monster::GetBaseAttackFar(VOID)
{
	return m_iAttackFar;
}
VOID Obj_Monster::SetBaseAttackFar(INT const nValue)
{
	m_iAttackFar = nValue;
}

INT Obj_Monster::GetToughness(VOID)
{
	if(TRUE==GetToughnessDirtyFlag())
	{
		INT nValue=GetBaseToughness()+GetToughnessRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_TOUGHNESS, nValue);
		ClearToughnessDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_TOUGHNESS);
}

INT Obj_Monster::GetBaseToughness(VOID)
{
	//to do:
	return g_MonsterAttrExTbl.Get( m_uDataID, CATTR_LEVEL2_TOUGHNESS );
}

VOID Obj_Monster::SetBaseToughness(INT const nValue)
{
	// forbiden modify this attribute
}

INT Obj_Monster::GetNearAttReduce(VOID)
{
	if(TRUE==GetNearAttReduceDirtyFlag())
	{
		INT nValue=GetBaseNearAttReduce()+GetNearAttReduceRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_NEARATT_REDUCE, nValue);
		ClearNearAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_NEARATT_REDUCE);
}

INT Obj_Monster::GetBaseNearAttReduce(VOID)
{
	//to do:
	return 0;
}

VOID Obj_Monster::SetBaseNearAttReduce(INT const nValue)
{
	// forbiden modify this attribute
}

INT 				Obj_Monster::GetFarAttReduce(VOID)
{
	if(TRUE==GetFarAttReduceDirtyFlag())
	{
		INT nValue=GetBaseFarAttReduce()+GetFarAttReduceRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_FARATT_REDUCE, nValue);
		ClearFarAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_FARATT_REDUCE);
}
INT 				Obj_Monster::GetBaseFarAttReduce(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseFarAttReduce(INT const nValue)
{

}

INT 				Obj_Monster::GetMagicNearAttReduce(VOID)
{
	if(TRUE==GetMagicNearAttReduceDirtyFlag())
	{
		INT nValue=GetBaseMagicNearAttReduce()+GetMagicNearAttReduceRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_MAGIC_NEAR_ATT_REDUCE, nValue);
		ClearMagicNearAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAGIC_NEAR_ATT_REDUCE);
}
INT 				Obj_Monster::GetMagicFarAttReduce(VOID)
{
	if(TRUE==GetMagicFarAttReduceDirtyFlag())
	{
		INT nValue=GetBaseMagicFarAttReduce()+GetMagicFarAttReduceRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_MAGIC_FAR_ATT_REDUCE, nValue);
		ClearMagicFarAttReduceDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAGIC_FAR_ATT_REDUCE);
}

INT 				Obj_Monster::GetBaseMagicNearAttReduce(VOID)
{
	return 0;
}
INT 				Obj_Monster::GetBaseMagicFarAttReduce(VOID)
{
	return 0;
}

VOID				Obj_Monster::SetBaseMagicNearAttReduce(INT const nValue)
{

}
VOID				Obj_Monster::SetBaseMagicFarAttReduce(INT const nValue)
{

}

INT 				Obj_Monster::GetDreadAttResist(VOID)
{
	if(TRUE==GetDreadAttResistDirtyFlag())
	{
		INT nValue=GetBaseDreadAttResist()+GetDreadAttResistRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_DREAD_RESIST, nValue);
		ClearDreadAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DREAD_RESIST);
}
INT 				Obj_Monster::GetBaseDreadAttResist(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseDreadAttResist(INT const nValue)
{

}

INT 				Obj_Monster::GetComaAttResist(VOID)
{
	if(TRUE==GetComaAttResistDirtyFlag())
	{
		INT nValue=GetBaseComaAttResist()+GetComaAttResistRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_COMA_RESIST, nValue);
		ClearComaAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_COMA_RESIST);
}
INT 				Obj_Monster::GetBaseComaAttResist(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseComaAttResist(INT const nValue)
{

}

//沉默抗性
INT 				Obj_Monster::GetHushAttResist(VOID)
{
	if(TRUE==GetHushAttResistDirtyFlag())
	{
		INT nValue=GetBaseHushAttResist()+GetHushAttResistRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_HUSH_RESIST, nValue);
		ClearHushAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_HUSH_RESIST);
}
INT 				Obj_Monster::GetBaseHushAttResist(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseHushAttResist(INT const nValue)
{

}

//缴械抗性
INT 				Obj_Monster::GetUnArmyAttResist(VOID)
{
	if(TRUE==GetUnArmyAttResistDirtyFlag())
	{
		INT nValue=GetBaseUnArmyAttResist()+GetUnArmyAttResistRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_UNARMY_RESIST, nValue);
		ClearUnArmyAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_UNARMY_RESIST);
}
INT 				Obj_Monster::GetBaseUnArmyAttResist(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseUnArmyAttResist(INT const nValue)
{

}

//减攻速抗性
INT 				Obj_Monster::GetAttSpeedAttResist(VOID)
{
	if(TRUE==GetAttSpeedAttResistDirtyFlag())
	{
		INT nValue=GetBaseAttSpeedAttResist()+GetAttSpeedAttResistRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACKSPEED_RESIST, nValue);
		ClearAttSpeedAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACKSPEED_RESIST);
}
INT 				Obj_Monster::GetBaseAttSpeedAttResist(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseAttSpeedAttResist(INT const nValue)
{

}

//减攻速抗性
INT 				Obj_Monster::GetSkillSpeedAttResist(VOID)
{
	if(TRUE==GetSkillSpeedAttResistDirtyFlag())
	{
		INT nValue=GetBaseSkillSpeedAttResist()+GetSkillSpeedAttResistRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_SKILLSPEED_RESIST, nValue);
		ClearSkillSpeedAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_SKILLSPEED_RESIST);
}
INT 				Obj_Monster::GetBaseSkillSpeedAttResist(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseSkillSpeedAttResist(INT const nValue)
{

}

//减移速抗性
INT 				Obj_Monster::GetMoveSpeedAttResist(VOID)
{
	if(TRUE==GetMoveSpeedAttResistDirtyFlag())
	{
		INT nValue=GetBaseMoveSpeedAttResist()+GetMoveSpeedAttResistRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_MOVEREDUCE_RESIST, nValue);
		ClearMoveSpeedAttResistDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MOVEREDUCE_RESIST);
}
INT 				Obj_Monster::GetBaseMoveSpeedAttResist(VOID)
{
	return 0;
}
VOID				Obj_Monster::SetBaseMoveSpeedAttResist(INT const nValue)
{

}
//-----------------------------------------------------------
// Attr2: 
INT Obj_Monster::GetAttackMagicNear(VOID)
{
	if(TRUE==GetAttackMagicNearDirtyFlag())
	{
		INT nValue=GetBaseAttackMagicNear()+GetAttackMagicNearRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR, nValue);
		ClearAttackMagicNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_NEAR);
}
INT Obj_Monster::GetAttackMagicFar(VOID)
{
	if(TRUE==GetAttackMagicFarDirtyFlag())
	{
		INT nValue=GetBaseAttackMagicFar()+GetAttackMagicFarRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR, nValue);
		ClearAttackMagicFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_MAGIC_FAR);
}

INT Obj_Monster::GetBaseAttackMagicNear(VOID)
{
	return m_iAttackMagicNear;
}
INT Obj_Monster::GetBaseAttackMagicFar(VOID)
{
	return m_iAttackMagicFar;
}

VOID	Obj_Monster::SetBaseAttackMagicNear(INT const nValue)
{
	m_iAttackMagicNear = nValue;
}

VOID	Obj_Monster::SetBaseAttackMagicFar(INT const nValue)
{
	m_iAttackMagicFar = nValue;
}

INT Obj_Monster::GetDefenceMagicNear(VOID)
{
	if(TRUE==GetDefenceMagicNearDirtyFlag())
	{
		INT nValue=GetBaseDefenceMagicNear()+GetDefenceMagicNearRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_NEAR, nValue);
		ClearDefenceMagicNearDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_NEAR);
}
INT Obj_Monster::GetDefenceMagicFar(VOID)
{
	if(TRUE==GetDefenceMagicFarDirtyFlag())
	{
		INT nValue=GetBaseDefenceMagicFar()+GetDefenceMagicFarRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_FAR, nValue);
		ClearDefenceMagicFarDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_DEFENCE_MAGIC_FAR);
}

INT Obj_Monster::GetBaseDefenceMagicNear(VOID)
{
	return m_iDefenceMagicNear;
}
INT Obj_Monster::GetBaseDefenceMagicFar(VOID)
{
	return m_iDefenceMagicFar;
}

VOID	Obj_Monster::SetBaseDefenceMagicNear(INT const nValue)
{
	m_iDefenceMagicNear = nValue;
}
VOID	Obj_Monster::SetBaseDefenceMagicFar(INT const nValue)
{
	m_iDefenceMagicFar = nValue;
}

// Attr2: Attack and Defence power of gold
INT Obj_Monster::GetAttackGold(VOID)
{
	if(TRUE==GetAttackGoldDirtyFlag())
	{
		INT nValue=GetBaseAttackGold()+GetAttackGoldRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_GOLD, nValue);
		ClearAttackGoldDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_GOLD);
}

INT Obj_Monster::GetBaseAttackGold(VOID)
{
	return m_iAttackGold;
}

VOID	Obj_Monster::SetBaseAttackGold(INT const nValue)
{
	m_iAttackGold = nValue;
}

INT Obj_Monster::GetDefenceGold(VOID)
{
	if(TRUE==GetResistGoldDirtyFlag())
	{
		INT nValue=GetBaseDefenceGold()+GetDefenceGoldRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_GOLD, nValue);
		ClearResistGoldDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_GOLD);
}

INT Obj_Monster::GetBaseDefenceGold(VOID)
{
	return m_iDefenceGold;
}

VOID	Obj_Monster::SetBaseDefenceGold(INT const nValue)
{
	m_iDefenceGold = nValue;
}

// Attr2: Attack and Defence power of Wood
INT Obj_Monster::GetAttackWood(VOID)
{
	if(TRUE==GetAttackWoodDirtyFlag())
	{
		INT nValue=GetBaseAttackWood()+GetAttackWoodRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WOOD, nValue);
		ClearAttackWoodDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WOOD);
}

INT Obj_Monster::GetBaseAttackWood(VOID)
{
	return m_iAttackWood;
}

VOID	Obj_Monster::SetBaseAttackWood(INT const nValue)
{
	m_iAttackWood = nValue;
}

INT Obj_Monster::GetDefenceWood(VOID)
{
	if(TRUE==GetResistWoodDirtyFlag())
	{
		INT nValue=GetBaseDefenceWood()+GetDefenceWoodRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_WOOD, nValue);
		ClearResistWoodDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_WOOD);
}

INT Obj_Monster::GetBaseDefenceWood(VOID)
{
	return m_iDefenceWood;
}

VOID	Obj_Monster::SetBaseDefenceWood(INT const nValue)
{
	m_iDefenceWood = nValue;
}

// Attr2: Attack and Defence power of Water
INT Obj_Monster::GetAttackWater(VOID)
{
	if(TRUE==GetAttackWaterDirtyFlag())
	{
		INT nValue=GetBaseAttackWater()+GetAttackWaterRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WATER, nValue);
		ClearAttackWaterDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_WATER);
}

INT Obj_Monster::GetBaseAttackWater(VOID)
{
	return m_iAttackWater;
}

VOID	Obj_Monster::SetBaseAttackWater(INT const nValue)
{
	m_iAttackWater = nValue;
}

INT Obj_Monster::GetDefenceWater(VOID)
{
	if(TRUE==GetResistWaterDirtyFlag())
	{
		INT nValue=GetBaseDefenceWater()+GetDefenceWaterRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_WATER, nValue);
		ClearResistWaterDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_WATER);
}
INT Obj_Monster::GetBaseDefenceWater(VOID)
{
	return m_iDefenceWater;
}

VOID	Obj_Monster::SetBaseDefenceWater(INT const nValue)
{
	m_iDefenceWater = nValue;
}

// Attr2: Attack and Defence power of Fire
INT Obj_Monster::GetAttackFire(VOID)
{
	if(TRUE==GetAttackFireDirtyFlag())
	{
		INT nValue=GetBaseAttackFire()+GetAttackFireRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FIRE, nValue);
		ClearAttackFireDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_FIRE);
}

INT Obj_Monster::GetBaseAttackFire(VOID)
{
	return m_iAttackFire;
}

VOID	Obj_Monster::SetBaseAttackFire(INT const nValue)
{
	m_iAttackFire = nValue;
}

INT Obj_Monster::GetDefenceFire(VOID)
{
	if(TRUE==GetResistFireDirtyFlag())
	{
		INT nValue=GetBaseDefenceFire()+GetDefenceFireRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_FIRE, nValue);
		ClearResistFireDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_FIRE);
}

INT Obj_Monster::GetBaseDefenceFire(VOID)
{
	return m_iDefenceFire;
}

VOID	Obj_Monster::SetBaseDefenceFire(INT const nValue)
{
	m_iDefenceFire = nValue;
}

// Attr2: Attack and Defence power of Soil
INT Obj_Monster::GetAttackSoil(VOID)
{
	if(TRUE==GetAttackSoilDirtyFlag())
	{
		INT nValue=GetBaseAttackSoil()+GetAttackSoilRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SOIL, nValue);
		ClearAttackSoilDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SOIL);
}

INT Obj_Monster::GetBaseAttackSoil(VOID)
{
	return m_iAttackSoil;
}

VOID	Obj_Monster::SetBaseAttackSoil(INT const nValue)
{
	m_iAttackSoil = nValue;
}

INT Obj_Monster::GetDefenceSoil(VOID)
{
	if(TRUE==GetResistSoilDirtyFlag())
	{
		INT nValue=GetBaseDefenceSoil()+GetDefenceSoilRefix();
		nValue = MINLV0(nValue);
		SetIntAttr(CharIntAttrs_T::ATTR_RESIST_SOIL, nValue);
		ClearResistSoilDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_RESIST_SOIL);
}

INT Obj_Monster::GetBaseDefenceSoil(VOID)
{
	return m_iDefenceSoil;
}

VOID	Obj_Monster::SetBaseDefenceSoil(INT const nValue)
{
	m_iDefenceSoil = nValue;
}

// Attr2 Reduce Slower Duration
INT Obj_Monster::GetReduceSlowerDuration(VOID)
{
	return GetReduceSlowerDurationRefix();
}

// Attr2 Reduce Weaken Duration
INT Obj_Monster::GetReduceWeakenDuration(VOID)
{
	return GetReduceWeakenDurationRefix();
}

// Attr2 Reduce Faint Duration
INT Obj_Monster::GetReduceFaintDuration(VOID)
{
	return GetReduceFaintDurationRefix();
}

// Attr2 Reduce Poisoned Duration
INT Obj_Monster::GetReducePoisonedDuration(VOID)
{
	return GetReducePoisonedDurationRefix();
}

// Attr2 VisionRange
INT Obj_Monster::GetVisionRange(VOID)
{
	if(TRUE==GetVisionRangeDirtyFlag())
	{
		INT nValue=GetBaseVisionRange()+GetVisionRangeRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_VISION_RANGE, nValue);
		ClearVisionRangeDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_VISION_RANGE);
}

INT Obj_Monster::GetBaseVisionRange(VOID)
{

	return BASE_VISION_RANGE;
}

VOID	Obj_Monster::SetBaseVisionRange(INT const nValue)
{// forbiden modify this attribute
}


