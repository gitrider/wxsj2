///////////////////////////////////////////////////////////////////////////////
// 文件名：Obj_Pet_Attributes.cpp
// 程序员：liam
// 功能说明：角色层属性
//
// 修改记录：
//	将所有的m_pdbPet转换成Pet的主人Human->pPetDB，Pet本身不拥有任务DB数据
//	武文斌 2005/12/16	
//  修改所有关于m_pdbPet的数据
//  武文斌 2005/12/19
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Type.h"
#include "Scene.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "Obj_Pet.h"
#include "GameTable.h"
#include "Obj_Human.h"
#include "ItemOperator.h"


VOID Obj_Pet::IncrementExp(INT nExp)
{
__ENTER_FUNCTION
	// 如果已经到达等级上限则直接返回
	if (GetLevel() == PET_LEVEL_NUM)
	{
		return ;
	}

	// 判断是否 >= 该级别升级的经验
	INT nCurExp = GetExp();
	nCurExp += nExp;
	INT nNeedExp = g_PetLevelUpTbl.GetExp(GetLevel());

	while (nCurExp > nNeedExp)
	{
		LevelUp();
		nCurExp -= nNeedExp;
		if (GetLevel() == PET_LEVEL_NUM)
		{
			nCurExp = 0;
			break;
		}
	}
	SetExp(nCurExp);
	
__LEAVE_FUNCTION
}
////////////////////////////////////////////////////////////////////////////////
//角色属性部分
//StrikePoint
INT	Obj_Pet::GetMaxStrikePoint(VOID)
{
	return 0;
}
//Rage
INT Obj_Pet::GetMaxRage(VOID)
{
	return 0;
}

INT	Obj_Pet::GetBaseMaxRage(VOID)
{
	return 0;
}

VOID 	Obj_Pet::SetBaseMaxRage(INT const nRage)
{
}

INT	Obj_Pet::GetRageRegenerate(VOID)
{
	return 0;
} 

INT Obj_Pet::GetBaseRageRegenerate(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseRageRegenerate(INT const nValue)
{
}

//Attr1: Str
INT	Obj_Pet::GetStr(VOID)
{
	if(TRUE==GetStrDirtyFlag())
	{
		INT nValue=GetBaseStr()+GetStrRefix();
		nValue = MINLV0(nValue);		
		SetIntAttr(CharIntAttrs_T::ATTR_STR, nValue);
		ClearStrDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_STR);
}

INT	Obj_Pet::GetBaseStr(VOID)
{
	return 0;
}

VOID 	Obj_Pet::SetBaseStr(INT const nValue)
{
}

//Attr1: Con
INT	Obj_Pet::GetCon(VOID)
{
	return 0;
}

INT	Obj_Pet::GetBaseCon(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseCon(INT const nValue)
{

}

//Attr1: Int
INT	Obj_Pet::GetInt(VOID)
{
	return 0;
}

INT	Obj_Pet::GetBaseInt(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseInt(INT const nValue)
{
}

//Attr1: Dex
INT	Obj_Pet::GetDex(VOID)
{
	return 0;
}

INT	Obj_Pet::GetBaseDex(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseDex(INT const nValue)
{
}

//HP
INT	Obj_Pet::GetMaxHP(VOID) 
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_MAXHP );
}
 
INT Obj_Pet::GetBaseMaxHP(VOID)
{
	return 100;
}

VOID 	Obj_Pet::SetBaseMaxHP(INT const nHp)
{
}

INT	Obj_Pet::GetHPRegenerate(VOID) 
{
	return 0;
}
 
INT Obj_Pet::GetBaseHPRegenerate(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_RESTOREHP );
}

VOID 	Obj_Pet::SetBaseHPRegenerate(INT const nValue)
{// forbiden modify this attribute
}

//MP
INT Obj_Pet::GetMaxMP(VOID)
{
	return 0;
}

INT Obj_Pet::GetBaseMaxMP(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_MAXMP) ;	
}

VOID 	Obj_Pet::SetBaseMaxMp(INT const nMp)
{// forbiden modify this attribute
}

INT	Obj_Pet::GetMPRegenerate(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_RESTOREMP) ;	
}
 
INT Obj_Pet::GetBaseMPRegenerate(VOID)
{
	return 0;
}

VOID 	Obj_Pet::SetBaseMPRegenerate(INT const nValue)
{// forbiden modify this attribute
}

//CampID
const _CAMP_DATA *Obj_Pet::GetCampData(VOID)
{
	Obj_Character *pOwner = GetOwner();
	if (!pOwner)	
	{
		return Obj_Character::GetCampData();
	}
	return pOwner->GetCampData();
}

const _CAMP_DATA *Obj_Pet::GetBaseCampData(VOID)const
{
	return NULL;//GetCampData();
}

VOID Obj_Pet::SetBaseCampData(const _CAMP_DATA *pCampData)
{
}

INT Obj_Pet::GetCampID(VOID)
{
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
}

INT Obj_Pet::GetBaseCampID(VOID) const
{
	//if (!GetConstCreator())	
	//{
	//	return m_PetDB.m_CampData.m_nCampID;
	//}

	//ItemContainer* pPetContainer = GetCreator()->GetPetContain();
	//Item* pPetItem = pPetContainer->GetItem(m_nIndex);
	//Assert(pPetItem);

	//return pPetItem->GetCampData()->m_nCampID;
	return NULL;//GetCampData();
}

VOID	Obj_Pet::SetBaseCampID(INT const nID)
{ // forbiden modify this attribute , only allow to modify Campid Refix,
}

//modelID
INT Obj_Pet::GetModelID(VOID) 
{
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
	//INT iDataID = GetDataID();
	//_HORSE_ATTR* pPetAttr = g_HorseAttrTbl.GetAttr(iDataID);
	//if (!pPetAttr)
	//	return INVALID_ID;
	//return g_HorseAttrTbl.m_aHorseTbl[iDataID].m_iBehaveID;
	////return 2006;
}

INT Obj_Pet::GetBaseModelID(VOID) const
{
	return INVALID_ID;
}

VOID	Obj_Pet::SetBaseModelID(INT const nID)
{ // forbiden modify this attribute
}

// Alive flag, wraped in ObjCharacter
// In Combat flag, wraped in ObjCharacter
// Can move flag, wraped in ObjCharacter
// Can Action flag, wraped in ObjCharacter
// Unbreakable flag, wraped in ObjCharacter
// Attr2: Move Speed, wraped in ObjCharacter
FLOAT	Obj_Pet::GetMoveSpeed(VOID)
{
	if(TRUE==GetMoveSpeedDirtyFlag())
	{
		INT nValue=GetBaseMoveSpeed()+GetMoveSpeedRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_MOVE_SPEED, nValue);
		ClearMoveSpeedDirtyFlag();
	}

	FLOAT fMoveSpeed  = GetIntAttr(CharIntAttrs_T::ATTR_MOVE_SPEED) / 1000.f;

	ENUM_MOVE_MODE eMoveMode = GetMoveMode();
	if(eMoveMode == MOVE_MODE_HOBBLE)
	{//变成基础速度的50%
		fMoveSpeed = fMoveSpeed*0.5f;
	}
	else if(eMoveMode == MOVE_MODE_RUN)
	{//变成基础速度的150%
		fMoveSpeed = fMoveSpeed*1.5f;
	}
	else if (eMoveMode == MOVE_MODE_SPRINT)
	{//变成基础速度的500%
		fMoveSpeed = fMoveSpeed*5.0f;
	}

	return fMoveSpeed;

}

INT	Obj_Pet::GetBaseMoveSpeed(VOID)
{
	if( NULL != m_pCreator )
	{
		return (INT)(m_pCreator->GetMoveSpeed() * 1000);
	}

	MONSTER_EXT_ATTR* pPetAttr = g_MonsterAttrExTbl.GetExtAttr( GetDataID() );
	if (!pPetAttr)
		return 0;
	return pPetAttr->m_WalkSpeed;
}

VOID	Obj_Pet::SetBaseMoveSpeed(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack Speed
INT Obj_Pet::GetAttackSpeed(VOID)
{
	//if(TRUE==GetAttackSpeedDirtyFlag())
	//{
	//	INT nValue=GetBaseAttackSpeed()+GetAttackSpeedRefix();
	//	if(400<nValue)
	//	{
	//		nValue = 400;
	//	}
	//	if(25>=nValue)
	//	{
	//		nValue = 25;
	//	}		
	//	SetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SPEED, nValue);
	//	ClearAttackSpeedDirtyFlag();
	//}
	//return GetIntAttr(CharIntAttrs_T::ATTR_ATTACK_SPEED);
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACKSPEED );
}

INT Obj_Pet::GetBaseAttackSpeed(VOID)
{
	return BASE_ATTACK_SPEED;

}

VOID	Obj_Pet::SetBaseAttackSpeed(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Miss
INT Obj_Pet::GetMiss(VOID)
{
	//if(TRUE==GetMissDirtyFlag())
	//{
	//	INT nValue=GetBaseMiss()+GetMissRefix();
	//	SetIntAttr(CharIntAttrs_T::ATTR_MISS, nValue);
	//	ClearMissDirtyFlag();
	//}
	//return GetIntAttr(CharIntAttrs_T::ATTR_MISS);
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_MISS);
}

INT Obj_Pet::GetBaseMiss(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseMiss(INT const nValue)
{// forbiden modify this attribute
}

// Attr2 Hit
INT Obj_Pet::GetHit(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_HIT);
}

INT Obj_Pet::GetBaseHit(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseHit(INT const nValue)
{// forbiden modify this attribute
}

// Attr2 Critical
INT Obj_Pet::GetCritical(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_CRITRATE);
}

INT Obj_Pet::GetBaseCritical(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseCritical(INT const nValue)
{// forbiden modify this attribute
}

//add by liam------------------------------------
INT Obj_Pet::GetDefenceNear(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_NEAR);
}

INT Obj_Pet::GetBaseDefenceNear (VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseDefenceNear (INT const nValue)
{
	// forbiden modify this attribute
}

INT Obj_Pet::GetDefenceFar(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_FAR);
}

INT Obj_Pet::GetBaseDefenceFar (VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseDefenceFar (INT const nValue)
{
	// forbiden modify this attribute
}


INT Obj_Pet::GetAttackNear(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACK_NEAR);
}

INT Obj_Pet::GetBaseAttackNear(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseAttackNear(INT const nValue)
{
	// forbiden modify this attribute
}


INT Obj_Pet::GetAttackFar(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACK_FAR);
}


INT Obj_Pet::GetBaseAttackFar(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseAttackFar(INT const nValue)
{
	// forbiden modify this attribute
}

INT Obj_Pet::GetToughness(VOID)
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

INT Obj_Pet::GetBaseToughness(VOID)
{
	//to do:
	return 0;
}

VOID Obj_Pet::SetBaseToughness(INT const nValue)
{
	// forbiden modify this attribute
}

INT Obj_Pet::GetNearAttReduce(VOID)
{
	//if(TRUE==GetNearAttReduceDirtyFlag())
	//{
	//	INT nValue=GetBaseNearAttReduce()+GetNearAttReduceRefix();
	//	SetIntAttr(CharIntAttrs_T::ATTR_NEARATT_REDUCE, nValue);
	//	ClearNearAttReduceDirtyFlag();
	//}
	//return GetIntAttr(CharIntAttrs_T::ATTR_NEARATT_REDUCE);
	return 0;
}

INT Obj_Pet::GetBaseNearAttReduce(VOID)
{
	//to do:
	return 0;
}

VOID Obj_Pet::SetBaseNearAttReduce(INT const nValue)
{
	// forbiden modify this attribute
}

INT Obj_Pet::GetFarAttReduce(VOID)
{
	//if(TRUE==GetFarAttReduceDirtyFlag())
	//{
	//	INT nValue=GetBaseFarAttReduce()+GetFarAttReduceRefix();
	//	nValue = MINLV0(nValue);
	//	SetIntAttr(CharIntAttrs_T::ATTR_FARATT_REDUCE, nValue);
	//	ClearFarAttReduceDirtyFlag();
	//}
	//return GetIntAttr(CharIntAttrs_T::ATTR_FARATT_REDUCE);
	return 0;
}

INT Obj_Pet::GetBaseFarAttReduce(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseFarAttReduce(INT const nValue)
{

}

INT Obj_Pet::GetMagicNearAttReduce(VOID)
{
	return 0;
}
INT Obj_Pet::GetMagicFarAttReduce(VOID)
{
	return 0;
}

INT Obj_Pet::GetBaseMagicNearAttReduce(VOID)
{
	return 0;
}
INT Obj_Pet::GetBaseMagicFarAttReduce(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseMagicNearAttReduce(INT const nValue)
{

}
VOID Obj_Pet::SetBaseMagicFarAttReduce(INT const nValue)
{

}

INT Obj_Pet::GetDreadAttResist(VOID)
{
	//if(TRUE==GetDreadAttResistDirtyFlag())
	//{
	//	INT nValue=GetBaseDreadAttResist()+GetDreadAttResistRefix();
	//	nValue = MINLV0(nValue);
	//	SetIntAttr(CharIntAttrs_T::ATTR_DREAD_RESIST, nValue);
	//	ClearDreadAttResistDirtyFlag();
	//}
	//return GetIntAttr(CharIntAttrs_T::ATTR_DREAD_RESIST);
	return 0;
}

INT Obj_Pet::GetBaseDreadAttResist(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseDreadAttResist(INT const nValue)
{

}

INT Obj_Pet::GetComaAttResist(VOID)
{
	return 0;
}
INT 				Obj_Pet::GetBaseComaAttResist(VOID)
{
	return 0;
}
VOID Obj_Pet::SetBaseComaAttResist(INT const nValue)
{

}

//沉默抗性
INT Obj_Pet::GetHushAttResist(VOID)
{	
	return 0;	
}

INT Obj_Pet::GetBaseHushAttResist(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseHushAttResist(INT const nValue)
{

}

//缴械抗性
INT Obj_Pet::GetUnArmyAttResist(VOID)
{
	return 0;
}

INT Obj_Pet::GetBaseUnArmyAttResist(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseUnArmyAttResist(INT const nValue)
{

}

//减攻速抗性
INT Obj_Pet::GetAttSpeedAttResist(VOID)
{
	return 0;
}
INT Obj_Pet::GetBaseAttSpeedAttResist(VOID)
{
	return 0;
}
VOID Obj_Pet::SetBaseAttSpeedAttResist(INT const nValue)
{

}

//减攻速抗性
INT Obj_Pet::GetSkillSpeedAttResist(VOID)
{
	return 0;
}

INT Obj_Pet::GetBaseSkillSpeedAttResist(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseSkillSpeedAttResist(INT const nValue)
{

}

//减移速抗性
INT Obj_Pet::GetMoveSpeedAttResist(VOID)
{
	return 0;
}

INT Obj_Pet::GetBaseMoveSpeedAttResist(VOID)
{
	return 0;
}

VOID Obj_Pet::SetBaseMoveSpeedAttResist(INT const nValue)
{
}
//-----------------------------------------------------------

// Attr2: 
INT Obj_Pet::GetAttackMagicNear(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACKRATE_MAGIC_NEAR );
}
INT Obj_Pet::GetAttackMagicFar(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACKRATE_MAGIC_FAR);
}

INT Obj_Pet::GetBaseAttackMagicNear(VOID)
{
	return 0;
}
INT Obj_Pet::GetBaseAttackMagicFar(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseAttackMagicNear(INT const nValue)
{// forbiden modify this attribute
}
VOID	Obj_Pet::SetBaseAttackMagicFar(INT const nValue)
{// forbiden modify this attribute
}


INT Obj_Pet::GetDefenceMagicNear(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_MAGIC_NEAR);
}
INT Obj_Pet::GetDefenceMagicFar(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_MAGIC_FAR);
}

INT Obj_Pet::GetBaseDefenceMagicNear(VOID)
{
	return 0;
}
INT Obj_Pet::GetBaseDefenceMagicFar(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseDefenceMagicNear(INT const nValue)
{// forbiden modify this attribute
}
VOID	Obj_Pet::SetBaseDefenceMagicFar(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of gold
INT Obj_Pet::GetAttackGold(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACK_GOLD);
}

INT Obj_Pet::GetBaseAttackGold(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseAttackGold(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Pet::GetDefenceGold(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_GOLD);
}

INT Obj_Pet::GetBaseDefenceGold(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseDefenceGold(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of Wood
INT Obj_Pet::GetAttackWood(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACK_WOOD );
}

INT Obj_Pet::GetBaseAttackWood(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseAttackWood(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Pet::GetDefenceWood(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_WOOD);
}

INT Obj_Pet::GetBaseDefenceWood(VOID)
{
	return 0;	
}

VOID	Obj_Pet::SetBaseDefenceWood(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of Water
INT Obj_Pet::GetAttackWater(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACK_WATER);
}

INT Obj_Pet::GetBaseAttackWater(VOID)
{
	return 0;	
}

VOID	Obj_Pet::SetBaseAttackWater(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Pet::GetDefenceWater(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_WATER);
}
INT Obj_Pet::GetBaseDefenceWater(VOID)
{
	return 0;	
}

VOID	Obj_Pet::SetBaseDefenceWater(INT const nValue)
{// forbiden modify this attribute
}


// Attr2: Attack and Defence power of Fire
INT Obj_Pet::GetAttackFire(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACK_FIRE);
}

INT Obj_Pet::GetBaseAttackFire(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseAttackFire(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Pet::GetDefenceFire(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_FIRE );
}

INT Obj_Pet::GetBaseDefenceFire(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseDefenceFire(INT const nValue)
{// forbiden modify this attribute
}

// Attr2: Attack and Defence power of Soil
INT Obj_Pet::GetAttackSoil(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_ATTACK_SOIL);
}

INT Obj_Pet::GetBaseAttackSoil(VOID)
{
	return 0;
}

VOID	Obj_Pet::SetBaseAttackSoil(INT const nValue)
{// forbiden modify this attribute
}

INT Obj_Pet::GetDefenceSoil(VOID)
{
	return g_MonsterAttrExTbl.Get( GetDataID(), CATTR_LEVEL2_DEFENCE_SOIL);
}

INT Obj_Pet::GetBaseDefenceSoil(VOID)
{
	return 0;	
}

VOID	Obj_Pet::SetBaseDefenceSoil(INT const nValue)
{// forbiden modify this attribute
}

// Attr2 Reduce Slower Duration
INT Obj_Pet::GetReduceSlowerDuration(VOID)
{
	return GetReduceSlowerDurationRefix();
}

// Attr2 Reduce Weaken Duration
INT Obj_Pet::GetReduceWeakenDuration(VOID)
{
	return GetReduceWeakenDurationRefix();
}

// Attr2 Reduce Faint Duration
INT Obj_Pet::GetReduceFaintDuration(VOID)
{
	return GetReduceFaintDurationRefix();
}

// Attr2 Reduce Poisoned Duration
INT Obj_Pet::GetReducePoisonedDuration(VOID)
{
	return GetReducePoisonedDurationRefix();
}

// Attr2 VisionRange
INT Obj_Pet::GetVisionRange(VOID)
{
	if(TRUE==GetVisionRangeDirtyFlag())
	{
		INT nValue=GetBaseVisionRange()+GetVisionRangeRefix();
		SetIntAttr(CharIntAttrs_T::ATTR_VISION_RANGE, nValue);
		ClearVisionRangeDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_VISION_RANGE);
}

INT Obj_Pet::GetBaseVisionRange(VOID)
{

	return 16;
}

VOID	Obj_Pet::SetBaseVisionRange(INT const nValue)
{// forbiden modify this attribute
}


