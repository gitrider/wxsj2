
#include "stdafx.h"
#include "Player.h"
#include "GameTable.h"
#include "ItemTable.h"
#include "TimeManager.h"
#include "GCDetailSkillList.h"
#include "GCOperateResult.h"
#include "Obj_Human.h"
#include "Config.h"
#include "LogicManager.h"
#include "HumanItemLogic.h"
#include "Scene.h"


using namespace Combat_Module::Skill_Module;
using namespace MenPai_Module;
using namespace Packets;

VOID Obj_Human::Levelup(VOID)
{
	__ENTER_FUNCTION
	SHORT nMenPai = GetMenPai();
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPai);
	if(NULL==pMenPaiLogic)
	{
		Assert("[Obj_Human::AttrLevel1Levelup]:Can't find MenPai Logic!");
		return;
	}
	if(100<=GetLevel())
	{
		return;
	}
	SetLevel(GetLevel()+1);
	getScene()->OnSceneHumanLevelUp( this ) ;
	
	Set_RemainPoints(Get_RemainPoints()+g_Config.m_ConfigInfo.m_LevelupAllocPoints);
	Set_RemainSkillPoints(Get_RemainSkillPoints()+g_Config.m_ConfigInfo.m_LevelupAllocPoints);

	MarkAllIntAttrDirtyFlags();

	INT nLevelPoint = Float2Int(pMenPaiLogic->GetMaxVigorLevelupPoint() / 100.0);

	SetMaxVigor(GetMaxVigor() + nLevelPoint);
	
	SetVigorRegeneRate(GetLevel() * (Float2Int((pMenPaiLogic->GetLevelRestoreVigor())/100.0)));

	SetMaxEnergy(GetMaxEnergy() + 5);

	SetHP(GetMaxHP());
	SetMP(GetMaxMP());
	SetPneuma(GetMaxPneuma());

	__LEAVE_FUNCTION;
}

//转换门派的时候需要进行的点数从新计算
VOID Obj_Human::ChangeMenpaiPoints(VOID)
{
	if(GetLevel() >= DEFAULT_WASHPOINT_LEVEL)
	{
		WashPoints();
		SetMP(GetMaxMP());
		SetPneuma(GetMaxPneuma());
		SetHP(GetMaxHP());
		return;
	}

	INT nMenPai		=  GetMenPai();

	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPai);
	if(NULL==pMenPaiLogic)
	{
		Assert("[Obj_Human::ChangeMenpaiPoints]:Can't find MenPai Logic!");
		return;
	}
	SHORT nValue = 0;
	SHORT nStr = 0;
	SHORT nCon = 0;
	SHORT nInt = 0;
	SHORT nDex = 0;

	for(INT nLevel =0;nLevel <= GetLevel();nLevel++)
	{
		//Str
		nStr += pMenPaiLogic->GetStrLevelupRefix(nLevel);
		//Con
		nCon += pMenPaiLogic->GetConLevelupRefix(nLevel);
		//Int
		nInt += pMenPaiLogic->GetIntLevelupRefix(nLevel);
		//Dex
		nDex += pMenPaiLogic->GetDexLevelupRefix(nLevel);
	}
	//set attr1
	SetBaseStr(nStr);
	SetBaseCon(nCon);
	SetBaseInt(nInt);
	SetBaseDex(nDex);

	// 给玩家加满HP和MP、元气
	SetMP(GetMaxMP());
	SetPneuma(GetMaxPneuma());
	SetHP(GetMaxHP());
}

VOID Obj_Human::WashPoints(VOID)
{
	INT nWashLevels = DEFAULT_WASHPOINT_LEVEL;
	INT nMenPai		=  GetMenPai();
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPai);
	if(NULL==pMenPaiLogic)
	{
		Assert("[Obj_Human::WashPoints]:Can't find MenPai Logic!");
		return;
	}
	SHORT nValue = 0;
	SHORT nStr = 0;
	//SHORT nSpr = 0;
	SHORT nCon = 0;
	SHORT nInt = 0;
	SHORT nDex = 0;
	SHORT nRemainPoint = 0;
	SHORT nTotalPoint = 0;
	/// \block calculate total points
	//@{
	for(INT nLevel =0;nLevel<=GetLevel();nLevel++)
	{
		//Str
		nTotalPoint += pMenPaiLogic->GetStrLevelupRefix(nLevel);
		//Con
		nTotalPoint += pMenPaiLogic->GetConLevelupRefix(nLevel);
		//Int
		nTotalPoint += pMenPaiLogic->GetIntLevelupRefix(nLevel);
		//Dex
		nTotalPoint += pMenPaiLogic->GetDexLevelupRefix(nLevel);
	}
	//@}
	
	for(INT nLevel =0;nLevel<DEFAULT_WASHPOINT_LEVEL;nLevel++)
	{
		//Str
		nStr += pMenPaiLogic->GetStrLevelupRefix(nLevel);
		//Con
		nCon += pMenPaiLogic->GetConLevelupRefix(nLevel);
		//Int
		nInt += pMenPaiLogic->GetIntLevelupRefix(nLevel);
		//Dex
		nDex += pMenPaiLogic->GetDexLevelupRefix(nLevel);
	}
	//set attr1
	SetBaseStr(nStr);
	//SetBaseSpr(nSpr);
	SetBaseCon(nCon);
	SetBaseInt(nInt);
	SetBaseDex(nDex);

	//Remain Points
	nRemainPoint = nTotalPoint-nStr-nCon-nInt-nDex;
	Set_RemainPoints(nRemainPoint);	
}

BOOL Obj_Human::ManualAttrPoint(CHAR_ATTR_LEVEL1 attrType)
{
	if(0>Get_RemainPoints())
	{
		AssertEx(FALSE,"[Obj_Human::ManualAttrPoint]: Assert at not enough remain point!");
		return FALSE;
	}
	switch(attrType)
	{
		case CATTR_LEVEL1_STR:
			SetBaseStr(GetBaseStr()+1);
			break;
		case CATTR_LEVEL1_CON:
			SetBaseCon(GetBaseCon()+1);
			break;
		case CATTR_LEVEL1_INT:
			SetBaseInt(GetBaseInt()+1);
			break;
		case CATTR_LEVEL1_DEX:
			SetBaseDex(GetBaseDex()+1);
			break;
		default:
			return FALSE;
			break;
	}
	Set_RemainPoints(Get_RemainPoints()-1);
	return TRUE;
}

BOOL Obj_Human::ManualAttrPoint(INT nStr, INT nCon, INT nInt, INT nDex)
{
	if(0>nStr||0>nCon||0>nInt||0>nDex)
	{
		return FALSE;
	}
	if((nStr+nCon+nInt+nDex)>Get_RemainPoints())
	{
		AssertEx(FALSE,"[Obj_Human::ManualAttrPoint]: Assert at not enough remain point!");
		return FALSE;
	}
	//set attrs
	SetBaseStr(GetBaseStr()+nStr);
	SetBaseCon(GetBaseCon()+nCon);
	SetBaseInt(GetBaseInt()+nInt);
	SetBaseDex(GetBaseDex()+nDex);
	//update remain point
	Set_RemainPoints(Get_RemainPoints()-(nStr+nCon+nInt+nDex));
	SendMsg_RefeshAttrib();
	return TRUE;
}

INT Obj_Human::Get_RemainPoints()
{
	return m_DB.GetRemainPoints();
}

//-------------------------------------------------------------------------------------------

INT	Obj_Human::Get_RemainSkillPoints()
{
	return m_DB.GetRemainSkillPoints();
}

INT	Obj_Human::Get_S_SkillPoints(INT nS)
{
	Assert((COMMON_S)nS<COMMON_S_NUMBER);
	return m_DB.Get_S_SkillPoints(nS);
}

_ITEM_EFFECT_SKILL*  Obj_Human::GetEquipEffectSkill(INT nSkillID)
{
	if (nSkillID == INVALID_INDEX)
	{
		return NULL;
	}
	for (INT i=0; i<m_EffectSkillNum; ++i)
	{
		if (m_EquipEffectSkill[i].m_nSkillID == nSkillID)
		{
			return &m_EquipEffectSkill[i];
		}
	}
	return NULL;
}

_ITEM_IMPACT*	Obj_Human::GetEquipImpact(INT nImpactID)
{
	if (nImpactID == INVALID_INDEX)
	{
		return NULL;
	}
	for (INT i=0; i<m_EquipImpactNum; ++i)
	{
		if (m_EquipImpactEffect[i].m_ImpactID == nImpactID)
		{
			return &m_EquipImpactEffect[i];
		}
	}
	return NULL;
}

VOID	Obj_Human::CleanEquipEffectSkill()
{
	m_EffectSkillNum = 0;
	memset(m_EquipEffectSkill, 0, sizeof(_ITEM_EFFECT_SKILL)*(HEQUIP_NUMBER*MAX_EQUIP_EFFECT_SKILL));
}

VOID	Obj_Human::CleanEquipImpact()
{
	m_EquipImpactNum = 0;
	memset(m_EquipImpactEffect, 0, sizeof(_ITEM_IMPACT)*(HEQUIP_NUMBER+MAX_ITEM_SET_ATTR));
}

VOID   Obj_Human::UnRegEquipImpact()
{
	for (INT i=0; i<m_EquipImpactNum; ++i)
	{
		Impact_DispelSpecificImpactByDataIndex(m_EquipImpactEffect[i].m_ImpactID, m_EquipImpactEffect[i].m_ImpactSum);
	}
}

VOID   Obj_Human::RegEquipImpact()
{
	for (INT i=0; i<m_EquipImpactNum; ++i)
	{
		for (INT j=0; j<m_EquipImpactEffect[i].m_ImpactSum; ++j)
		{
			g_ImpactCore.SendImpactToUnit(*this, (ID_t)m_EquipImpactEffect[i].m_ImpactID, GetID());
		}
		
	}
}
//-----------------------------------------------------------------------------------


VOID Obj_Human::ItemEffectFlush( )
{
__ENTER_FUNCTION
	UnRegEquipImpact();
	//将物品的影响因素值清零；
	memset( m_pItemEffect, 0, sizeof(_ITEM_EFFECT)*IATTRIBUTE_NUMBER );
	CleanEquipEffectSkill();
	CleanEquipImpact();

	for( INT i=0; i<HEQUIP_NUMBER; i++ )
	{	
		/*
		if( !m_DB.GetEquipDB()->IsSet( (HUMAN_EQUIP)i ) )//没有装备此物品
			continue ;
		*/
		ItemContainer* pEquipContainer = GetEquipContain();
		if(NULL==pEquipContainer)
		{
			AssertEx(FALSE,"[Obj_Human::ItemEffectFlush]: NULL EquipContainer pointer found!");
			break;
		}
		Item*	pEquipItem = pEquipContainer->GetItem((UINT)i);
		if(NULL==pEquipItem)
		{
			AssertEx(FALSE,"[Obj_Human::ItemEffectFlush]: NULL EquipItem pointer found!");
			continue;
		}
		if(TRUE==pEquipItem->IsEmpty())
		{
			continue;//这个位置是空的
		}
		
		if(0==pEquipItem->GetDurPoints())
		{
			continue;
		}
		//这个位置装备有物品
		const	_ITEM* pCurItem = m_DB.GetEquipItem((HUMAN_EQUIP)i);
	
		INT nItemClass = pCurItem->ItemClass();
	
		INT nItemType = pCurItem->ItemType();
		Assert(ICLASS_EQUIP==nItemClass) ; //只能为装备，不能为别的物品

		EQUIP_BIND_TB* pBindInfoTb = NULL;
		if(pCurItem->GetEquipData()->m_Bind != 0)
		{
			pBindInfoTb = g_ItemTable.GetEquipBindTB(pCurItem->GetEquipData()->m_Bind);
			Assert(pBindInfoTb);
			if (!pBindInfoTb)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::ItemEffectFlush] GetEquipBindTB index is error [%d]",pCurItem->GetEquipData()->m_Bind);
				return;
		}
		}
		//取得每个物品的各个属性
		for( INT j=0; j<pCurItem->GetEquipData()->m_AttrCount; j++ )
		{
			switch(pCurItem->GetEquipData()->m_pAttr[j].m_AttrType) 
			{
			case IATTRIBUTE_BASE_ATTACK_NEAR:			//基础近程攻击
			case IATTRIBUTE_BASE_ATTACK_FAR:			//基础远程攻击
			case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:		//基础近程内功攻击
			case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:		//基础近程外功攻击
			case IATTRIBUTE_BASE_DEFENCE_NEAR:			//基础近程防御
			case IATTRIBUTE_BASE_DEFENCE_FAR:			//基础远程防御
			case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:	//基础近程内功防御
			case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR:		//基础近程外功防御
				{
					_ITEM_ATTR itemAtt = pCurItem->GetEquipData()->m_pAttr[j];

					//增加绑定对属性的影响
					if(pCurItem->GetEquipData()->m_Bind != 0)
					{
						float value = (float)pBindInfoTb->m_EffectRate*
							(float)pCurItem->GetEquipData()->m_pAttr[j].m_Value.m_Value/100 + (float)0.5;
						itemAtt.m_Value.m_Value += (SHORT)value;
					}
					CaculateEffect(itemAtt, (HUMAN_EQUIP)i, nItemType);
				}
				break;
			default:
				{
					CaculateEffect(pCurItem->GetEquipData()->m_pAttr[j],(HUMAN_EQUIP)i, nItemType);
				}
				break;
			}
		}

		INT EffectSkillCount = pEquipItem->GetEffectSkillCount();
		for (INT m=0; m<EffectSkillCount; ++m)
		{
			_ITEM_EFFECT_SKILL* pEffectSkill = pEquipItem->GetEffectSkill(m);
			BOOL bAlive = FALSE;
			for (INT i=0; i<m_EffectSkillNum; ++i)
			{
				if (m_EquipEffectSkill[i].m_nSkillID == pEffectSkill->m_nSkillID)
				{
					m_EquipEffectSkill[i].m_nLevel += pEffectSkill->m_nLevel;
					bAlive = TRUE;
				}
			}
			if (!bAlive)
			{
				m_EquipEffectSkill[m_EffectSkillNum].m_nSkillID = pEffectSkill->m_nSkillID;
				m_EquipEffectSkill[m_EffectSkillNum].m_nLevel   = pEffectSkill->m_nLevel;
			}
		}
		
		//附加装备每个宝石属性
		if(pCurItem->GetEquipData()->m_StoneCount != 0)
		{
			//刷新装备宝石属性
			pEquipItem->UpDataEquipGem();

			for( INT nGemHole=0; nGemHole<pCurItem->GetEquipData()->m_CurGemHole; ++nGemHole)
			{
				if((pCurItem->GetEquipData()->m_pGemInfo[nGemHole].m_GemIndex != INVALID_ITEM_ID)
					&&(pCurItem->GetEquipData()->m_pGemInfo[nGemHole].m_bEnable))
				{
					GEMINFO_TB* pGET = g_ItemTable.GetGemInfoTB(pCurItem->GetEquipData()->m_pGemInfo[nGemHole].m_GemIndex);
					if(pGET)
					{
						for(UINT m = 0;m<pGET->m_AttCount; ++m)
						{
							CaculateEffect(pGET->m_GenAttr[m],(HUMAN_EQUIP)i, pGET->m_ItemType.m_Type);
						}
					}
				}
			}
		}
	}
	
	CaculateEquipSetEffect();
	CaculatePaiEffect();
	CaculateEquipSoulAndChainEffect();
	RegEquipImpact();
	MarkAllIntAttrDirtyFlags();//mark the int attributes dirty
__LEAVE_FUNCTION
}

VOID		Obj_Human::CaculateEffect(_ITEM_ATTR& Attr,INT EquipOffset, INT nItemType)
{
	__ENTER_FUNCTION

	SHORT				value;
	ITEM_ATTRIBUTE		type;

	value	=	Attr.m_Value.m_Value;
	type	=	(ITEM_ATTRIBUTE)Attr.m_AttrType;
	// Passive Skill refix
	if(INVALID_ID != nItemType)
	{
		switch(type) 
		{
		case IATTRIBUTE_BASE_ATTACK_NEAR:			//基础近程攻击
		case IATTRIBUTE_BASE_ATTACK_FAR:			//基础远程攻击
		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:		//基础近程内功攻击
		case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:		//基础近程外功攻击
		case IATTRIBUTE_BASE_DEFENCE_NEAR:			//基础近程防御
		case IATTRIBUTE_BASE_DEFENCE_FAR:			//基础远程防御
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:	//基础近程内功防御
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR:		//基础近程外功防御
			{
				//passive skill refixs value here
				Skill_RefixItemAttr(EquipOffset, nItemType, type, value);
			}
			break;
		default:
			break;
		}
	}
	switch(type) 
	{
		case IATTRIBUTE_POINT_STR:			//力量点数
		case IATTRIBUTE_RATE_STR:			//力量百分比
		case IATTRIBUTE_POINT_DEX:			//敏捷点数
		case IATTRIBUTE_RATE_DEX:			//敏捷百分比
		case IATTRIBUTE_POINT_INT:			//智力点数
		case IATTRIBUTE_RATE_INT:			//智力百分比
		case IATTRIBUTE_POINT_CON:			//体质点数
		case IATTRIBUTE_RATE_CON:			//体质百分比
		case IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR://近程内功点数
		case IATTRIBUTE_POINT_ATTACK_MAGIC_FAR://近程外功点数
		case IATTRIBUTE_RATE_ATTACK_MAGIC_NEAR:	//近程内功百分比
		case IATTRIBUTE_RATE_ATTACK_MAGIC_FAR:	//近程外功百分比
		case IATTRIBUTE_POINT_ATTACK_NEAR:	//近攻点数
		case IATTRIBUTE_RATE_ATTACK_NEAR:	//近攻百分比
		case IATTRIBUTE_POINT_ATTACK_FAR:	//远攻点数
		case IATTRIBUTE_RATE_ATTACK_FAR:	//远攻百分比
		case IATTRIBUTE_POINT_DEFENCE_MAGIC_NEAR://近程内功点数
		case IATTRIBUTE_POINT_DEFENCE_MAGIC_FAR://近程外功点数
		case IATTRIBUTE_RATE_DEFENCE_MAGIC_NEAR://近程内功防御百分比
		case IATTRIBUTE_RATE_DEFENCE_MAGIC_FAR://近程外功防御百分比
		case IATTRIBUTE_POINT_DEFENCE_NEAR:	//近防点数
		case IATTRIBUTE_RATE_DEFENCE_NEAR:	//近防百分比
		case IATTRIBUTE_POINT_DEFENCE_FAR:	//远防点数
		case IATTRIBUTE_RATE_DEFENCE_FAR:	//远防百分比
		case IATTRIBUTE_POINT_MAXHP:		//生命值上限点数
		case IATTRIBUTE_RATE_MAXHP:			//生命值上限百分比
		case IATTRIBUTE_POINT_MAXMP:		//魔法值上限点数
		case IATTRIBUTE_RATE_MAXMP:			//魔法值上限百分比
		case IATTRIBUTE_POINT_MAX_PNEUMA:	//元气值上限点数
		case IATTRIBUTE_RATE_MAX_PNEUMA:	//元气值上限百分比
		case IATTRIBUTE_MAXRAGE:			//怒气上限
		case IATTRIBUTE_RATE_MAXRAGE:		//怒气上限百分比
		case IATTRIBUTE_MAX_VIGOR:			//活力上限点数
		case IATTRIBUTE_RATE_MAX_VIGOR:		//活力上限百分比
		case IATTRIBUTE_VIGOR_REGENERATE:	//活力回复
		case IATTRIBUTE_HIT:				//命中
		case IATTRIBUTE_MISS:				//闪避
		case IATTRIBUTE_RATE_HIT:			//命中率
		case IATTRIBUTE_CRIT:				//暴击
		case IATTRIBUTE_TOUGHNESS:			//韧性
		case IATTRIBUTE_RATE_CRIT:			//爆击率

		case IATTRIBUTE_GOLD_ATTACK:		//金攻击点数
		case IATTRIBUTE_RATE_GOLD_ATTACK:	//金攻击百分比

		case IATTRIBUTE_WOOD_ATTACK:		//木攻击点数
		case IATTRIBUTE_RATE_WOOD_ATTACK:	//木攻击百分比

		case IATTRIBUTE_WATER_ATTACK:		//水攻击点数
		case IATTRIBUTE_RATE_WATER_ATTACK:	//水攻击百分比

		case IATTRIBUTE_FIRE_ATTACK:		//火攻击点数
		case IATTRIBUTE_RATE_FIRE_ATTACK:	//火攻击百分比

		case IATTRIBUTE_SOIL_ATTACK:		//土攻击点数
		case IATTRIBUTE_RATE_SOIL_ATTACK:	//土攻击百分比

		case IATTRIBUTE_GOLD_RESIST:		//金防御点数
		case IATTRIBUTE_RATE_GOLD_RESIST:	//金防御百分比
		case IATTRIBUTE_WOOD_RESIST:		//木防御点数
		case IATTRIBUTE_RATE_WOOD_RESIST:	//木防御百分比
		case IATTRIBUTE_WATER_RESIST:		//水防御点数
		case IATTRIBUTE_RATE_WATER_RESIST:	//水防御百分比
		case IATTRIBUTE_FIRE_RESIST:		//火防御点数
		case IATTRIBUTE_RATE_FIRE_RESIST:	//火防御百分比
		case IATTRIBUTE_SOIL_RESIST:		//土防御点数
		case IATTRIBUTE_RATE_SOIL_RESIST:	//土防御百分比
		case IATTRIBUTE_SPEED_MOVE:			//移动速度
		case IATTRIBUTE_ATTACK_SPEED:		//攻击速度
		case IATTRIBUTE_COLOR_HAIR:			//发色
		case IATTRIBUTE_MODEL_HAIR:			//发型
		case IATTRIBUTE_HP_RESTORE:			//生命回复
		case IATTRIBUTE_MP_RESTORE:			//魔法回复
		case IATTRIBUTE_PNEUMA_RESTORE:		//元气回复
		case IATTRIBUTE_CRIT_HURT:			//增加爆击伤害

		case IATTRIBUTE_SHIELD_NEAR:		//近程盾
		case IATTRIBUTE_SHIELD_MAGIC_NEAR:	//近程内功盾
		case IATTRIBUTE_SHIELD_MAGIC_FAR:	//近程外功盾
		case IATTRIBUTE_SHIELD_FAR:			//远程盾

		case IATTRIBUTE_SHIELD_GOLD:		//金盾
		case IATTRIBUTE_SHIELD_WOOD:		//木盾
		case IATTRIBUTE_SHIELD_WATER:		//水盾
		case IATTRIBUTE_SHIELD_FIRE:		//火盾
		case IATTRIBUTE_SHIELD_SOIL:		//土盾
		case IATTRIBUTE_EXPRENCE_GET:		//增加经验获得
		case IATTRIBUTE_MONEY_GET:			//增加金钱获得
		case IATTRIBUTE_BASE_ATTACK_NEAR:	//基础近程攻击
		case IATTRIBUTE_BASE_ATTACK_FAR:	//基础远程攻击
		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR://基础近程内功攻击
		case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR://基础近程外功攻击
		case IATTRIBUTE_BASE_DEFENCE_NEAR:	//基础近程防御
		case IATTRIBUTE_BASE_DEFENCE_FAR:	//基础远程防御
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR://基础近程内功防御
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR://基础近程外功防御
		case IATTRIBUTE_RATE_NEARATT_REDUCE:		//按百分比抵消近程攻击
		case IATTRIBUTE_RATE_FARATT_REDUCE:		//按百分比抵消远程攻击
		case IATTRIBUTE_RATE_MAGIC_NEAR_ATT_REDUCE:		//按百分比抵消近程内功攻击
		case IATTRIBUTE_RATE_MAGIC_FAR_ATT_REDUCE:		//按百分比抵消近程外功攻击
		case IATTRIBUTE_RATE_GOLD_ATT_REDUCE:		//按百分比抵消金攻击
		case IATTRIBUTE_RATE_WOOD_ATT_REDUCE:		//按百分比抵消木攻击
		case IATTRIBUTE_RATE_WATER_REDUCE:		//按百分比抵消水攻击
		case IATTRIBUTE_RATE_FIREATT_REDUCE:		//按百分比抵消火攻击
		case IATTRIBUTE_RATE_SOIL_ATT_REDUCE:		//按百分比抵消土攻击
		case IATTRIBUTE_REDUCE_DAMAGE_SHAOLIN_GWS:		//减少伤害--少林派[棍武僧]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_SHAOLIN_TLH:	//减少伤害--少林派[铁罗汉]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_GGB:	//减少伤害--丐帮[棍丐帮]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_ZGB:	//减少伤害--丐帮[掌丐帮]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_JWD:	//减少伤害--武当派[剑武当]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_QWD:	//减少伤害--武当派[气武当]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_DT:	//减少伤害--唐门[刀唐]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_XJT:	//减少伤害--唐门[陷阱唐]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_XXY:	//减少伤害--逍遥派[扇逍遥]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_ZXY:	//减少伤害--逍遥派[指逍遥]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_KYW:	//减少伤害--药王谷[狂药王]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_YYW:	//减少伤害--药王谷[医药王]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_DTH:	//减少伤害--桃花岛[笛桃花]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_YTH:	//减少伤害--桃花岛[音桃花]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_CEM:		//减少伤害--峨嵋派[刺峨眉]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_FEM:		//减少伤害--峨嵋派[辅峨眉]
		case IATTRIBUTE_REDUCE_DAMAGE_ID_WUMENPAI:		//减少伤害--无门派，如新手。。等
			{
				_ITEM_EFFECT* pEffect = &(m_pItemEffect[type]);

				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}
				break;
			}
		case IATTRIBUTE_POINT_ALL_BASEATT:					//增加所有的人物一级属性
			{
				_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_STR]);

				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}
				pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_CON]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_INT]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_DEX]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}				
				break;
			}
		case IATTRIBUTE_RATE_ALL_BASEATT:					//增加所有的人物一级属性
			{
				_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_STR]);

				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}
				pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_CON]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_INT]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_DEX]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}				
				break;
			}
		case IATTRIBUTE_RESIST_ALL:			//按点数抵消所有属性攻击
			{
				_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_GOLD_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_WOOD_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_WATER_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			

				pEffect = &(m_pItemEffect[IATTRIBUTE_FIRE_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_SOIL_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				break;
			}
		case IATTRIBUTE_RATE_RESIST_ALL:	//按百分比抵消所有属性攻击
			{
				_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_GOLD_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_WOOD_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_WATER_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			

				pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_FIRE_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			

				pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_SOIL_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				break;
			}
		case IATTRIBUTE_SHIELD_ALL:
			{
				_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_NEAR]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}
				
				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_MAGIC_NEAR]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_MAGIC_FAR]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			

				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_FAR]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_GOLD]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			

				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_WOOD]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_WATER]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_FIRE]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			

				pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_SOIL]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				break;
			}
		case IATTRIBUTE_ID_IMPACT:
			{
				BOOL bAlive = FALSE;
				for (INT i=0; i<m_EquipImpactNum; ++i)
				{
					if (m_EquipImpactEffect[i].m_ImpactID == value)
					{
						++m_EquipImpactEffect[i].m_ImpactSum;
						bAlive = TRUE;
						break;
					}
				}
				if (!bAlive)
				{
					m_EquipImpactEffect[m_EquipImpactNum].m_ImpactID  = value;
					++m_EquipImpactEffect[m_EquipImpactNum].m_ImpactSum;
					++m_EquipImpactNum;
				}
				//g_ImpactCore.SendImpactToUnit( *this, (ID_t)value, GetID() );
			}
			break;
		default:
			{
				Assert(FALSE);
				break;
			}
	//--------------------------------------------------------------------------------------*/
	}

	__LEAVE_FUNCTION
}

VOID	Obj_Human::CaculateEquipLevelEffect(UINT nRate)
{
	__ENTER_FUNCTION

	_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_ATTACK_NEAR]);

	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;

	pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_ATTACK_FAR]);
	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;


	pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR]);
	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;
	pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_ATTACK_MAGIC_FAR]);
	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;

	pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_DEFENCE_NEAR]);
	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;

	pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_DEFENCE_FAR]);
	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;

	pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR]);
	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;

	pEffect = &(m_pItemEffect[IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR]);
	if(pEffect->IsActive())
		pEffect->m_Attr.m_Value += nRate*pEffect->m_Attr.m_Value;

	__LEAVE_FUNCTION
}

VOID	Obj_Human::CaculateEquipSetEffect()
{
	__ENTER_FUNCTION
		
		UINT	setInfo[1000];
		UINT	validSet[HEQUIP_NUMBER];
		UINT    validSetCount	= 0;
		
		memset(setInfo, 0, sizeof(UINT)*1000);
		memset(validSet, 0, sizeof(UINT)*HEQUIP_NUMBER);
		Item* pEquip = HumanItemLogic::GetEquip(this,HEQUIP_MAINHAND);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iMainHandSetNum	 = pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iMainHandSetNum != -1)&&(iMainHandSetNum != 0))
			{
				++setInfo[iMainHandSetNum];
				validSet[validSetCount] = iMainHandSetNum;
				++validSetCount;
			}
		}
		
		pEquip = HumanItemLogic::GetEquip(this,HEQUIP_ASSIHAND);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iAssiHandSetNum	 = pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iAssiHandSetNum != -1)&&(iAssiHandSetNum != 0))
			{
				if (setInfo[iAssiHandSetNum] == 0)
				{
					validSet[validSetCount] = iAssiHandSetNum;
					++validSetCount;
				}
				++setInfo[iAssiHandSetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_HEAD);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iHeadSetNum		= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iHeadSetNum != -1)&&(iHeadSetNum != 0))
			{
				if (setInfo[iHeadSetNum] == 0)
				{
					validSet[validSetCount] = iHeadSetNum;
					++validSetCount;
				}
				++setInfo[iHeadSetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_NECK);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iNeckSetNum		= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iNeckSetNum != -1)&&(iNeckSetNum != 0))
			{
				if (setInfo[iNeckSetNum] == 0)
				{
					validSet[validSetCount] = iNeckSetNum;
					++validSetCount;
				}
				++setInfo[iNeckSetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_WRIST);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iWristSetNum	= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iWristSetNum != -1)&&(iWristSetNum != 0))
			{
				if (setInfo[iWristSetNum] == 0)
				{
					validSet[validSetCount] = iWristSetNum;
					++validSetCount;
				}
				++setInfo[iWristSetNum];
			}
		}
		
		pEquip  = HumanItemLogic::GetEquip(this,HEQUIP_SHOULDER);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iShoulderSetNum	= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iShoulderSetNum != -1)&&(iShoulderSetNum != 0))
			{
				if (setInfo[iShoulderSetNum] == 0)
				{
					validSet[validSetCount] = iShoulderSetNum;
					++validSetCount;
				}
				++setInfo[iShoulderSetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_BACK);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iBackSetNum		= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iBackSetNum != -1)&&(iBackSetNum != 0))
			{
				if (setInfo[iBackSetNum] == 0)
				{
					validSet[validSetCount] = iBackSetNum;
					++validSetCount;
				}
				++setInfo[iBackSetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_HAND);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iHandSetNum		= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iHandSetNum != -1)&&(iHandSetNum != 0))
			{
				if (setInfo[iHandSetNum] == 0)
				{
					validSet[validSetCount] = iHandSetNum;
					++validSetCount;
				}
				++setInfo[iHandSetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_RING1);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iRing1SetNum	= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iRing1SetNum != -1)&&(iRing1SetNum != 0))
			{
				if (setInfo[iRing1SetNum] == 0)
				{
					validSet[validSetCount] = iRing1SetNum;
					++validSetCount;
				}
				++setInfo[iRing1SetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_RING2);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iRing2SetNum	= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iRing2SetNum != -1)&&(iRing2SetNum != 0))
			{
				if (setInfo[iRing2SetNum] == 0)
				{
					validSet[validSetCount] = iRing2SetNum;
					++validSetCount;
				}
				++setInfo[iRing2SetNum];
			}
		}
		
		pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_FEET);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iFeetSetNum		= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iFeetSetNum != -1)&&(iFeetSetNum != 0))
			{
				if (setInfo[iFeetSetNum] == 0)
				{
					validSet[validSetCount] = iFeetSetNum;
					++validSetCount;
				}
				++setInfo[iFeetSetNum];
			}
		}
		
		pEquip  = HumanItemLogic::GetEquip(this,HEQUIP_ADORN1);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iAdorn1SetNum	= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iAdorn1SetNum != -1)&&(iAdorn1SetNum != 0))
			{
				if (setInfo[iAdorn1SetNum] == 0)
				{
					validSet[validSetCount] = iAdorn1SetNum;
					++validSetCount;
				}
				++setInfo[iAdorn1SetNum];
			}
		}
		
		pEquip  = HumanItemLogic::GetEquip(this,HEQUIP_ADORN2);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iAdorn2SetNum	= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iAdorn2SetNum != -1)&&(iAdorn2SetNum != 0))
			{
				if (setInfo[iAdorn2SetNum] == 0)
				{
					validSet[validSetCount] = iAdorn2SetNum;
					++validSetCount;
				}
				++setInfo[iAdorn2SetNum];
			}
		}
		
		pEquip   = HumanItemLogic::GetEquip(this,HEQUIP_WAIST);
		if (pEquip->GetDurPoints() != 0)
		{
			INT	iWaistSetNum	= pEquip->IsEmpty()?0:pEquip->GetSetNum();
			if ((iWaistSetNum != -1)&&(iWaistSetNum != 0))
			{
				if (setInfo[iWaistSetNum] == 0)
				{
					validSet[validSetCount] = iWaistSetNum;
					++validSetCount;
				}
				++setInfo[iWaistSetNum];
			}
		}
		
		EQUIP_SET_TB* pGET = NULL;
		for (UINT j=0; j<validSetCount; ++j)
		{
			if (validSet[j]>0)
			{
				pGET = g_ItemTable.GetEquipSetTB(validSet[j]);
				Assert(pGET);
				if (!pGET)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSetEffect] GetEquipSetTB index is error [%d]",validSet[j]);
					return;
				}
				//by方远,修改成,大于指定的装备数量后就生效对应的属性
				//if ((UINT)pGET->m_nEquipSetCount <= setInfo[validSet[j]])
				{
					for(INT i = 0;i<pGET->m_nAttrCount;i++)
					{
						if (pGET->m_ActiveAttrNeedNum[i] <= setInfo[validSet[j]])
						{
							CaculateEffect(pGET->m_ItemAttr[i], HEQUIP_NUMBER+i);
						}
					}
				}
			}
		}

	__LEAVE_FUNCTION
}

void	Obj_Human::AddEffect(_ITEM_ATTR& Attr)
{
	__ENTER_FUNCTION

	SHORT				value;
	ITEM_ATTRIBUTE		type;

	value	=	Attr.m_Value.m_Value;
	type	=	(ITEM_ATTRIBUTE)Attr.m_AttrType;
	switch(type) 
	{
	case IATTRIBUTE_POINT_STR:			//力量点数
	case IATTRIBUTE_RATE_STR:			//力量百分比
	case IATTRIBUTE_POINT_DEX:			//敏捷点数
	case IATTRIBUTE_RATE_DEX:			//敏捷百分比
	case IATTRIBUTE_POINT_INT:			//智力点数
	case IATTRIBUTE_RATE_INT:			//智力百分比
	case IATTRIBUTE_POINT_CON:			//体质点数
	case IATTRIBUTE_RATE_CON:			//体质百分比
	case IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR://近程内功点数
	case IATTRIBUTE_POINT_ATTACK_MAGIC_FAR://近程外功点数
	case IATTRIBUTE_RATE_ATTACK_MAGIC_NEAR:	//近程内功百分比
	case IATTRIBUTE_RATE_ATTACK_MAGIC_FAR:	//近程外功百分比
	case IATTRIBUTE_POINT_ATTACK_NEAR:	//近攻点数
	case IATTRIBUTE_RATE_ATTACK_NEAR:	//近攻百分比
	case IATTRIBUTE_POINT_ATTACK_FAR:	//远攻点数
	case IATTRIBUTE_RATE_ATTACK_FAR:	//远攻百分比
	case IATTRIBUTE_POINT_DEFENCE_MAGIC_NEAR://近程内功点数
	case IATTRIBUTE_POINT_DEFENCE_MAGIC_FAR://近程外功点数
	case IATTRIBUTE_RATE_DEFENCE_MAGIC_NEAR://近程内功防御百分比
	case IATTRIBUTE_RATE_DEFENCE_MAGIC_FAR://近程外功防御百分比
	case IATTRIBUTE_POINT_DEFENCE_NEAR:	//近防点数
	case IATTRIBUTE_RATE_DEFENCE_NEAR:	//近防百分比
	case IATTRIBUTE_POINT_DEFENCE_FAR:	//远防点数
	case IATTRIBUTE_RATE_DEFENCE_FAR:	//远防百分比
	case IATTRIBUTE_POINT_MAXHP:		//生命值上限点数
	case IATTRIBUTE_RATE_MAXHP:			//生命值上限百分比
	case IATTRIBUTE_POINT_MAXMP:		//魔法值上限点数
	case IATTRIBUTE_RATE_MAXMP:			//魔法值上限百分比
	case IATTRIBUTE_POINT_MAX_PNEUMA:	//元气值上限点数
	case IATTRIBUTE_RATE_MAX_PNEUMA:	//元气值上限百分比
	case IATTRIBUTE_MAXRAGE:			//怒气上限
	case IATTRIBUTE_RATE_MAXRAGE:		//怒气上限百分比
	case IATTRIBUTE_MAX_VIGOR:			//活力上限点数
	case IATTRIBUTE_RATE_MAX_VIGOR:		//活力上限百分比
	case IATTRIBUTE_VIGOR_REGENERATE:	//活力回复

	case IATTRIBUTE_HIT:				//命中
	case IATTRIBUTE_MISS:				//闪避
	case IATTRIBUTE_RATE_HIT:			//命中率
	case IATTRIBUTE_CRIT:				//暴击
	case IATTRIBUTE_TOUGHNESS:			//韧性
	case IATTRIBUTE_RATE_CRIT:			//爆击率

	case IATTRIBUTE_GOLD_ATTACK:		//金攻击点数
	case IATTRIBUTE_RATE_GOLD_ATTACK:	//金攻击百分比

	case IATTRIBUTE_WOOD_ATTACK:		//木攻击点数
	case IATTRIBUTE_RATE_WOOD_ATTACK:	//木攻击百分比

	case IATTRIBUTE_WATER_ATTACK:		//水攻击点数
	case IATTRIBUTE_RATE_WATER_ATTACK:	//水攻击百分比

	case IATTRIBUTE_FIRE_ATTACK:		//火攻击点数
	case IATTRIBUTE_RATE_FIRE_ATTACK:	//火攻击百分比

	case IATTRIBUTE_SOIL_ATTACK:		//土攻击点数
	case IATTRIBUTE_RATE_SOIL_ATTACK:	//土攻击百分比

	case IATTRIBUTE_GOLD_RESIST:		//金防御点数
	case IATTRIBUTE_RATE_GOLD_RESIST:	//金防御百分比
	case IATTRIBUTE_WOOD_RESIST:		//木防御点数
	case IATTRIBUTE_RATE_WOOD_RESIST:	//木防御百分比
	case IATTRIBUTE_WATER_RESIST:		//水防御点数
	case IATTRIBUTE_RATE_WATER_RESIST:	//水防御百分比
	case IATTRIBUTE_FIRE_RESIST:		//火防御点数
	case IATTRIBUTE_RATE_FIRE_RESIST:	//火防御百分比
	case IATTRIBUTE_SOIL_RESIST:		//土防御点数
	case IATTRIBUTE_RATE_SOIL_RESIST:	//土防御百分比
	case IATTRIBUTE_SPEED_MOVE:			//移动速度
	case IATTRIBUTE_ATTACK_SPEED:		//攻击速度
	case IATTRIBUTE_COLOR_HAIR:			//发色
	case IATTRIBUTE_MODEL_HAIR:			//发型
	case IATTRIBUTE_HP_RESTORE:			//生命回复
	case IATTRIBUTE_MP_RESTORE:			//魔法回复
	case IATTRIBUTE_PNEUMA_RESTORE:		//元气回复
	case IATTRIBUTE_CRIT_HURT:			//增加爆击伤害

	case IATTRIBUTE_SHIELD_NEAR:		//近程盾
	case IATTRIBUTE_SHIELD_MAGIC_NEAR:	//近程内功盾
	case IATTRIBUTE_SHIELD_MAGIC_FAR:	//近程外功盾
	case IATTRIBUTE_SHIELD_FAR:			//远程盾
	case IATTRIBUTE_SHIELD_GOLD:		//金盾
	case IATTRIBUTE_SHIELD_WOOD:		//木盾
	case IATTRIBUTE_SHIELD_WATER:		//水盾
	case IATTRIBUTE_SHIELD_FIRE:		//火盾
	case IATTRIBUTE_SHIELD_SOIL:		//土盾
	case IATTRIBUTE_EXPRENCE_GET:		//增加经验获得
	case IATTRIBUTE_MONEY_GET:			//增加金钱获得
	case IATTRIBUTE_RATE_NEARATT_REDUCE:		//按百分比抵消近程攻击
	case IATTRIBUTE_RATE_FARATT_REDUCE:		//按百分比抵消远程攻击
	case IATTRIBUTE_RATE_MAGIC_NEAR_ATT_REDUCE:		//按百分比抵消近程内功攻击
	case IATTRIBUTE_RATE_MAGIC_FAR_ATT_REDUCE:		//按百分比抵消近程外功攻击
	case IATTRIBUTE_RATE_GOLD_ATT_REDUCE:		//按百分比抵消金攻击
	case IATTRIBUTE_RATE_WOOD_ATT_REDUCE:		//按百分比抵消木攻击
	case IATTRIBUTE_RATE_WATER_REDUCE:		//按百分比抵消水攻击
	case IATTRIBUTE_RATE_FIREATT_REDUCE:		//按百分比抵消火攻击
	case IATTRIBUTE_RATE_SOIL_ATT_REDUCE:		//按百分比抵消土攻击
	case IATTRIBUTE_RATE_DREAD_RESIST:		//恐惧抗性
	case IATTRIBUTE_RATE_COMA_RESIST:		//昏迷抗性
	case IATTRIBUTE_RATE_HUSH_RESIST:		//沉默抗性
	case IATTRIBUTE_RATE_UNARM_RESIST:		//缴械抗性
	case IATTRIBUTE_RATE_ATTACKSPEED_RESIST:		//减攻速抗性
	case IATTRIBUTE_RATE_SKILLSPEED_RESIST:		//减施速抗性
	case IATTRIBUTE_RATE_MOVEREDUCE_RESIST:		//减移速抗性
	case IATTRIBUTE_REDUCE_DAMAGE_SHAOLIN_GWS:		//减少伤害--少林派[棍武僧]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_SHAOLIN_TLH:	//减少伤害--少林派[铁罗汉]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_GGB:	//减少伤害--丐帮[棍丐帮]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_GAIBANG_ZGB:	//减少伤害--丐帮[掌丐帮]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_JWD:	//减少伤害--武当派[剑武当]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_WUDANG_QWD:	//减少伤害--武当派[气武当]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_DT:	//减少伤害--唐门[刀唐]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_TANGMEN_XJT:	//减少伤害--唐门[陷阱唐]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_XXY:	//减少伤害--逍遥派[扇逍遥]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_XIAOYAO_ZXY:	//减少伤害--逍遥派[指逍遥]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_KYW:	//减少伤害--药王谷[狂药王]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_YAOWANGGU_YYW:	//减少伤害--药王谷[医药王]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_DTH:	//减少伤害--桃花岛[笛桃花]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_TAOHUAGU_YTH:	//减少伤害--桃花岛[音桃花]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_CEM:		//减少伤害--峨嵋派[刺峨眉]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_EMEI_FEM:		//减少伤害--峨嵋派[辅峨眉]
	case IATTRIBUTE_REDUCE_DAMAGE_ID_WUMENPAI:		//减少伤害--无门派，如新手。。等
		{
			_ITEM_EFFECT* pEffect = &(m_pItemEffect[type]);

			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
			break;
		}
	case IATTRIBUTE_POINT_ALL_BASEATT:					//增加所有的人物一级属性
		{
			_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_STR]);

			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
			
			pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_CON]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
					
			pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_INT]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
						
			pEffect = &(m_pItemEffect[IATTRIBUTE_POINT_DEX]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;			
			break;
		}
	case IATTRIBUTE_RATE_ALL_BASEATT:					//增加所有的人物一级属性
		{
			_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_STR]);

			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
			
			pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_CON]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
						
			pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_INT]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
						
			pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_DEX]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
				
			break;
		}
	case IATTRIBUTE_RESIST_ALL:			//按点数抵消所有属性攻击
		{
			_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_GOLD_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_WOOD_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_WATER_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;			
			
			pEffect = &(m_pItemEffect[IATTRIBUTE_FIRE_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_SOIL_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
						
			break;
		}
	case IATTRIBUTE_RATE_RESIST_ALL:	//按百分比抵消所有属性攻击
		{
			_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_GOLD_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_WOOD_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_WATER_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;			
			
			pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_FIRE_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_RATE_SOIL_RESIST]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
						
			break;
		}
	case IATTRIBUTE_SHIELD_ALL:
		{
			_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_NEAR]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
			
			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_MAGIC_NEAR]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_MAGIC_FAR]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
						
			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_FAR]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
			
			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_GOLD]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_WOOD]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_WATER]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;
					
			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_FIRE]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			pEffect = &(m_pItemEffect[IATTRIBUTE_SHIELD_SOIL]);
			if(pEffect->IsActive())
				pEffect->m_Attr.m_Value += value;

			break;
		}
	case IATTRIBUTE_ID_IMPACT:
		{
			g_ImpactCore.SendImpactToUnit( *this, (ID_t)value, GetID() );
		}
		break;
	default:
		{
			Assert(FALSE);
			break;
		}
		//--------------------------------------------------------------------------------------*/
	}

	__LEAVE_FUNCTION
}
void	Obj_Human::CaculatePaiEffect()
{
	PAI_INFO* pPaiInfo = NULL;
	Item* pEquip	   = NULL;

	pEquip = HumanItemLogic::GetEquip(this,HEQUIP_MAINHAND);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip = HumanItemLogic::GetEquip(this,HEQUIP_ASSIHAND);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());			
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_HEAD);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());			
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_NECK);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_WRIST);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip  = HumanItemLogic::GetEquip(this,HEQUIP_SHOULDER);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_BACK);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_HAND);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_RING1);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_RING2);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip	= HumanItemLogic::GetEquip(this,HEQUIP_FEET);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip  = HumanItemLogic::GetEquip(this,HEQUIP_ADORN1);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip  = HumanItemLogic::GetEquip(this,HEQUIP_ADORN2);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
	pEquip   = HumanItemLogic::GetEquip(this,HEQUIP_WAIST);
	if(!pEquip->IsEmpty())
	{
		if ((pEquip->GetEquipPaiType() != INVALID_PAI_TYPE)&&(pEquip->GetDurPoints() != 0))
		{
			pPaiInfo = g_ItemTable.GetPaiInfo(pEquip->GetEquipPaiType());
			Assert(pPaiInfo);
			if (!pPaiInfo)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculatePaiEffect] GetPaiInfo index is error [%d]",pEquip->GetEquipPaiType());
				return;
			}
			CaculateEffect(pPaiInfo->m_Att, -1);
		}
	}
}

void	Obj_Human::CaculateEquipSoulAndChainEffect()
{
	__ENTER_FUNCTION

	//局部激活标志（保存每组偏移）
	Flag128 EquipFlag0;
	Flag128 EquipFlag1;
	Flag128 EquipFlag2;
	Flag128 EquipFlag3;
	Flag128 EquipFlag4;

	BOOL  bTrue = TRUE;
	//三个部位牌型
	BYTE nPaiType1	= 0;
	BYTE nPaiType2	= 0;
	BYTE nPaiType3	= 0;
	
	//局部激活在激活表中的位置
	UINT	PosFlat0 = 0;
	UINT	PosFlat1 = 0;
	UINT	PosFlat2 = 0;
	UINT	PosFlat3 = 0;
	UINT	PosFlat4 = 0;
	
	//保存每个局部激活是否是顺或者连
	UINT PaiFlat[4];
	memset(PaiFlat, 0, sizeof(UINT)*4);

	UINT	AnyKeNum	= 0;
	UINT	AnyShunNum	= 0;
	PAI_INFO* pPaiInfo = NULL;
	//计算武器激活
	Item* pMainHandEquip = HumanItemLogic::GetEquip(this,HEQUIP_MAINHAND);
	Item* pAssiHandEquip = HumanItemLogic::GetEquip(this,HEQUIP_ASSIHAND);
	if (pMainHandEquip->IsEmpty()&&pAssiHandEquip->IsEmpty())
	{
		EquipFlag0.CleanUp();
	}
	else
	{
		if ((pMainHandEquip->GetDurPoints() == 0)||(pAssiHandEquip->GetDurPoints() == 0))
		{
			EquipFlag0.CleanUp();
		}
		else
		{
			BYTE nMainHandType = pMainHandEquip->GetEquipPaiType();
			BYTE nAssiHandType = pAssiHandEquip->GetEquipPaiType();

			if ((nMainHandType == INVALID_PAI_TYPE)||(nAssiHandType == INVALID_PAI_TYPE))
			{
				EquipFlag0.CleanUp();
			}

			else if(nMainHandType != nAssiHandType)
			{
				EquipFlag0.CleanUp();
			}

			else
			{
				EquipFlag0.UpdateBits(nMainHandType,bTrue);
				EquipFlag0.UpdateBits(nAssiHandType,bTrue);

				PosFlat0 = INDEX_ANYJIANG;
			}
		}
	}
	
	//计算激活
	Item* pHeadEquip	= HumanItemLogic::GetEquip(this,HEQUIP_HEAD);
	Item* pNeckEquip	= HumanItemLogic::GetEquip(this,HEQUIP_NECK);
	Item* pWristEquip	= HumanItemLogic::GetEquip(this,HEQUIP_WRIST);
	if ((pHeadEquip->IsEmpty())||(pNeckEquip->IsEmpty())||(pWristEquip->IsEmpty()))
	{
		EquipFlag1.CleanUp();
	}
	else
	{
		if ((pHeadEquip->GetDurPoints() == 0)||(pNeckEquip->GetDurPoints() == 0)||(pWristEquip->GetDurPoints() == 0))
		{
			EquipFlag1.CleanUp();
		}
		else
		{
			nPaiType1 = pHeadEquip->GetEquipPaiType();
			nPaiType2 = pNeckEquip->GetEquipPaiType();
			nPaiType3 = pWristEquip->GetEquipPaiType();
			if ((nPaiType1 == INVALID_PAI_TYPE)||(nPaiType2 == INVALID_PAI_TYPE)||(nPaiType3 == INVALID_PAI_TYPE))
			{
				EquipFlag1.CleanUp();
			}

			else
			{
				EquipFlag1.UpdateBits(nPaiType1,bTrue);
				EquipFlag1.UpdateBits(nPaiType2,bTrue);
				EquipFlag1.UpdateBits(nPaiType3,bTrue);

				UINT nMinTypeValue = nPaiType1;
				UINT nMaxTypeValue = nPaiType1;
				if (nMinTypeValue>nPaiType2)
				{
					nMinTypeValue = nPaiType2;
				}
				else
				{
					nMaxTypeValue = nPaiType2;
				}
				if (nMinTypeValue>nPaiType3)
				{
					nMinTypeValue = nPaiType3;
				}
				else
				{
					nMaxTypeValue = nPaiType3;
				}

				
				//任意顺
				if ((nMaxTypeValue - nMinTypeValue)==2)
				{
					PosFlat1 = INDEX_ANYSHUN;
					++AnyShunNum;
					PaiFlat[0] = EQUIPPAI_ANYSHUN;
				}

				//任意刻
				if (nMaxTypeValue == nMinTypeValue)
				{
					PosFlat1 = INDEX_ANYKE;
					++AnyKeNum;

					PaiFlat[0] = EQUIPPAI_ANYKE;
				}
			}
		}
	}
	

	//计算激活
	Item* pShouldEquip  = HumanItemLogic::GetEquip(this,HEQUIP_SHOULDER);
	Item* pBackEquip	= HumanItemLogic::GetEquip(this,HEQUIP_BACK);
	Item* pHandEquip	= HumanItemLogic::GetEquip(this,HEQUIP_HAND);
	if ((pShouldEquip->IsEmpty())||(pBackEquip->IsEmpty())||(pHandEquip->IsEmpty()))
	{
		EquipFlag2.CleanUp();
	}
	else
	{
		if ((pShouldEquip->GetDurPoints() == 0)||(pBackEquip->GetDurPoints() == 0)||(pHandEquip->GetDurPoints() == 0))
		{
			EquipFlag2.CleanUp();
		}
		else
		{
			nPaiType1 = pShouldEquip->GetEquipPaiType();
			nPaiType2 = pBackEquip->GetEquipPaiType();
			nPaiType3 = pHandEquip->GetEquipPaiType();
			if((nPaiType1 == INVALID_PAI_TYPE)||(nPaiType2 == INVALID_PAI_TYPE)||(nPaiType3 == INVALID_PAI_TYPE))
			{
				EquipFlag2.CleanUp();
			}
			else
			{
				EquipFlag2.UpdateBits(nPaiType1,bTrue);
				EquipFlag2.UpdateBits(nPaiType2,bTrue);
				EquipFlag2.UpdateBits(nPaiType3,bTrue);

				UINT nMinTypeValue = nPaiType1;
				UINT nMaxTypeValue = nPaiType1;
				if (nMinTypeValue>nPaiType2)
				{
					nMinTypeValue = nPaiType2;
				}
				else
				{
					nMaxTypeValue = nPaiType2;
				}
				if (nMinTypeValue>nPaiType3)
				{
					nMinTypeValue = nPaiType3;
				}
				else
				{
					nMaxTypeValue = nPaiType3;
				}


				//任意顺
				if ((nMaxTypeValue - nMinTypeValue)==2)
				{
					PosFlat2 = INDEX_ANYSHUN;
					++AnyShunNum;
					PaiFlat[1] = EQUIPPAI_ANYSHUN;
				}

				//任意刻
				if (nMaxTypeValue == nMinTypeValue)
				{
					PosFlat2 = INDEX_ANYKE;
					++AnyKeNum;

					PaiFlat[1] = EQUIPPAI_ANYKE;
				}
			}
		}
	}

	//计算激活
	Item* pRing1Equip	= HumanItemLogic::GetEquip(this,HEQUIP_RING1);
	Item* pRing2Equip	= HumanItemLogic::GetEquip(this,HEQUIP_RING2);
	Item* pFeetEquip	= HumanItemLogic::GetEquip(this,HEQUIP_FEET);
	if ((pRing1Equip->IsEmpty())||(pRing2Equip->IsEmpty())||(pFeetEquip->IsEmpty()))
	{
		EquipFlag3.CleanUp();
	}
	else
	{
		if ((pRing1Equip->GetDurPoints() == 0)||(pRing2Equip->GetDurPoints() == 0)||(pFeetEquip->GetDurPoints() == 0))
		{
			EquipFlag3.CleanUp();
		}
		else
		{
			nPaiType1 = pRing1Equip->GetEquipPaiType();
			nPaiType2 = pRing2Equip->GetEquipPaiType();
			nPaiType3 = pFeetEquip->GetEquipPaiType();
			if((nPaiType1 == INVALID_PAI_TYPE)||(nPaiType2 == INVALID_PAI_TYPE)||(nPaiType3 == INVALID_PAI_TYPE))
			{
				EquipFlag3.CleanUp();
			}

			else
			{
				EquipFlag3.UpdateBits(nPaiType1,bTrue);
				EquipFlag3.UpdateBits(nPaiType2,bTrue);
				EquipFlag3.UpdateBits(nPaiType3,bTrue);

				UINT nMinTypeValue = nPaiType1;
				UINT nMaxTypeValue = nPaiType1;
				if (nMinTypeValue>nPaiType2)
				{
					nMinTypeValue = nPaiType2;
				}
				else
				{
					nMaxTypeValue = nPaiType2;
				}
				if (nMinTypeValue>nPaiType3)
				{
					nMinTypeValue = nPaiType3;
				}
				else
				{
					nMaxTypeValue = nPaiType3;
				}


				//任意顺
				if ((nMaxTypeValue - nMinTypeValue)==2)
				{
					PosFlat3 = INDEX_ANYSHUN;
					++AnyShunNum;
					PaiFlat[2] = EQUIPPAI_ANYSHUN;
				}

				//任意刻
				if (nMaxTypeValue == nMinTypeValue)
				{
					PosFlat3 = INDEX_ANYKE;
					++AnyKeNum;

					PaiFlat[2] = EQUIPPAI_ANYKE;
				}
			}
		}
	}	

	//计算激活
	Item* pAdorn1Equip  = HumanItemLogic::GetEquip(this,HEQUIP_ADORN1);
	Item* pAdorn2Equip  = HumanItemLogic::GetEquip(this,HEQUIP_ADORN2);
	Item* pWaistEquip   = HumanItemLogic::GetEquip(this,HEQUIP_WAIST);
	if ((pAdorn1Equip->IsEmpty())||(pAdorn2Equip->IsEmpty())||(pWaistEquip->IsEmpty()))
	{
		EquipFlag4.CleanUp();
	}
	else
	{
		if ((pAdorn1Equip->GetDurPoints() == 0)||(pAdorn2Equip->GetDurPoints() == 0)||(pWaistEquip->GetDurPoints() == 0))
		{
			EquipFlag4.CleanUp();
		}
		else
		{
			nPaiType1 = pAdorn1Equip->GetEquipPaiType();
			nPaiType2 = pAdorn2Equip->GetEquipPaiType();
			nPaiType3 = pWaistEquip->GetEquipPaiType();
			if((nPaiType1 == INVALID_PAI_TYPE)||(nPaiType2 == INVALID_PAI_TYPE)||(nPaiType3 == INVALID_PAI_TYPE))
			{
				EquipFlag4.CleanUp();
			}

			else
			{
				EquipFlag4.UpdateBits(nPaiType1,bTrue);
				EquipFlag4.UpdateBits(nPaiType2,bTrue);
				EquipFlag4.UpdateBits(nPaiType3,bTrue);

				UINT nMinTypeValue = nPaiType1;
				UINT nMaxTypeValue = nPaiType1;
				if (nMinTypeValue>nPaiType2)
				{
					nMinTypeValue = nPaiType2;
				}
				else
				{
					nMaxTypeValue = nPaiType2;
				}
				if (nMinTypeValue>nPaiType3)
				{
					nMinTypeValue = nPaiType3;
				}
				else
				{
					nMaxTypeValue = nPaiType3;
				}


				//任意顺
				if ((nMaxTypeValue - nMinTypeValue)==2)
				{
					PosFlat4 = INDEX_ANYSHUN;
					++AnyShunNum;
					PaiFlat[3] = EQUIPPAI_ANYSHUN;
				}

				//任意刻
				if (nMaxTypeValue == nMinTypeValue)
				{
					PosFlat4 = INDEX_ANYKE;
					++AnyKeNum;

					PaiFlat[3] = EQUIPPAI_ANYKE;
				}
			}
		}
	}
	
	UINT nChainInfoCount = g_ItemTable.GetEquipChainInfoCount();
	EQUIPCHAIN_INFO_TB* pTB =  NULL;

	for (UINT i=INDEX_NOANYPAINUM; i<nChainInfoCount; ++i)
	{
		pTB = g_ItemTable.GetEquipChainInfoTB(i);
		Assert(pTB);
		if (!pTB)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipChainInfoTB index is error [%d]",i);
			return;
		}
		if ((pTB->m_Flat.m_uLLowFlags == 0)&&(pTB->m_Flat.m_uLHighFlags == 0)
			&&(pTB->m_Flat.m_uHLowFlags == 0)&&(pTB->m_Flat.m_uHHighFlags == 0))
		{
			continue;
		}
		if (pTB->m_ReqPai3 == INVALID_PAI_TYPE)
		{
			if ((pTB->m_Flat.m_uLLowFlags == EquipFlag0.m_uLLowFlags)&&(pTB->m_Flat.m_uLHighFlags == EquipFlag0.m_uLHighFlags)
				&&(pTB->m_Flat.m_uHLowFlags == EquipFlag0.m_uHLowFlags)&&(pTB->m_Flat.m_uHHighFlags == EquipFlag0.m_uHHighFlags))
			{	
				PosFlat0	= i;
			}
		}
		if ((pTB->m_Flat.m_uLLowFlags == EquipFlag1.m_uLLowFlags)&&(pTB->m_Flat.m_uLHighFlags == EquipFlag1.m_uLHighFlags)
			&&(pTB->m_Flat.m_uHLowFlags == EquipFlag1.m_uHLowFlags)&&(pTB->m_Flat.m_uHHighFlags == EquipFlag1.m_uHHighFlags))
		{	
			PosFlat1	= i;
		}
		if ((pTB->m_Flat.m_uLLowFlags == EquipFlag2.m_uLLowFlags)&&(pTB->m_Flat.m_uLHighFlags == EquipFlag2.m_uLHighFlags)
			&&(pTB->m_Flat.m_uHLowFlags == EquipFlag2.m_uHLowFlags)&&(pTB->m_Flat.m_uHHighFlags == EquipFlag2.m_uHHighFlags))
		{
			PosFlat2	= i;
		}
		if ((pTB->m_Flat.m_uLLowFlags == EquipFlag3.m_uLLowFlags)&&(pTB->m_Flat.m_uLHighFlags == EquipFlag3.m_uLHighFlags)
			&&(pTB->m_Flat.m_uHLowFlags == EquipFlag3.m_uHLowFlags)&&(pTB->m_Flat.m_uHHighFlags == EquipFlag3.m_uHHighFlags))
		{
			PosFlat3	= i;
		}
		if ((pTB->m_Flat.m_uLLowFlags == EquipFlag4.m_uLLowFlags)&&(pTB->m_Flat.m_uLHighFlags == EquipFlag4.m_uLHighFlags)
			&&(pTB->m_Flat.m_uHLowFlags == EquipFlag4.m_uHLowFlags)&&(pTB->m_Flat.m_uHHighFlags == EquipFlag4.m_uHHighFlags))
		{
			PosFlat4	= i;
		}
	}

	//胡牌
	UINT SoulValue0 = 0;   //将
	//保存特殊局部激活
	UINT SoulType[1000];

	UINT ValidSoul[4];			//有效不重复的特殊局部激活索引号
	UINT SoulFlat[4];			//有效激活局部激活代表的顺或者连的标记
	UINT ValidSoulNum = 0;		//有效不重复的局部激活数
	memset(SoulType, 0, sizeof(UINT)*200);
	memset(ValidSoul, 0, sizeof(UINT)*4);
	memset(SoulFlat, 0, sizeof(UINT)*4);
	if (PosFlat0)
	{
		pTB = g_ItemTable.GetEquipChainInfoTB(PosFlat0);
		Assert(pTB);
		if (!pTB)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipChainInfoTB index is error [%d]",PosFlat0);
			return;
		}
		CaculateEffect(pTB->m_Att, -1);
		SoulValue0 = pTB->m_ChainID;
	}

	if (PosFlat1)
	{
		pTB = g_ItemTable.GetEquipChainInfoTB(PosFlat1);
		Assert(pTB);
		if (!pTB)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipChainInfoTB index is error [%d]",PosFlat1);
			return;
		}
		CaculateEffect(pTB->m_Att, -1);
		
		SoulFlat[ValidSoulNum] = PaiFlat[0];
		if (SoulType[pTB->m_ChainID] == 0)
		{
			ValidSoul[ValidSoulNum] = pTB->m_ChainID;
			++ValidSoulNum;
		}
		++SoulType[pTB->m_ChainID];
	}

	if (PosFlat2)
	{
		pTB = g_ItemTable.GetEquipChainInfoTB(PosFlat2);
		Assert(pTB);
		if(!pTB)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipChainInfoTB index is error [%d]",PosFlat2);
			return;
		}
		CaculateEffect(pTB->m_Att, -1);
		
		SoulFlat[ValidSoulNum] = PaiFlat[1];
		if (SoulType[pTB->m_ChainID] == 0)
		{
			ValidSoul[ValidSoulNum] = pTB->m_ChainID;
			++ValidSoulNum;
		}
		++SoulType[pTB->m_ChainID];
	}

	if (PosFlat3)
	{
		pTB = g_ItemTable.GetEquipChainInfoTB(PosFlat3);
		Assert(pTB);
		if (!pTB)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipChainInfoTB index is error [%d]",PosFlat3);
			return;
		}
		CaculateEffect(pTB->m_Att, -1);
		
		SoulFlat[ValidSoulNum] = PaiFlat[2];
		if (SoulType[pTB->m_ChainID] == 0)
		{
			ValidSoul[ValidSoulNum] = pTB->m_ChainID;
			++ValidSoulNum;
		}
		++SoulType[pTB->m_ChainID];
	}

	if (PosFlat4)
	{
		pTB = g_ItemTable.GetEquipChainInfoTB(PosFlat4);
		Assert(pTB);
		if (!pTB)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipChainInfoTB index is error [%d]",PosFlat4);
			return;
		}
		CaculateEffect(pTB->m_Att, -1);
		
		SoulFlat[ValidSoulNum] = PaiFlat[3];
		if (SoulType[pTB->m_ChainID] == 0)
		{
			ValidSoul[ValidSoulNum] = pTB->m_ChainID;
			++ValidSoulNum;
		}
		++SoulType[pTB->m_ChainID];
	}
	
	//不存在胡牌
	if ((EquipFlag0.m_uLLowFlags==0)&&(EquipFlag0.m_uLHighFlags==0)
		&&(EquipFlag0.m_uHLowFlags==0)&&(EquipFlag0.m_uHHighFlags==0))
	{
		return;
	}
	if ((EquipFlag1.m_uLLowFlags==0)&&(EquipFlag1.m_uLHighFlags==0)
		&&(EquipFlag1.m_uHLowFlags==0)&&(EquipFlag1.m_uHHighFlags==0))
	{
		return;
	}
	if ((EquipFlag2.m_uLLowFlags==0)&&(EquipFlag2.m_uLHighFlags==0)
		&&(EquipFlag2.m_uHLowFlags==0)&&(EquipFlag2.m_uHHighFlags==0))
	{
		return;
	}
	if ((EquipFlag3.m_uLLowFlags==0)&&(EquipFlag3.m_uLHighFlags==0)
		&&(EquipFlag3.m_uHLowFlags==0)&&(EquipFlag3.m_uHHighFlags==0))
	{
		return;
	}
	if ((EquipFlag4.m_uLLowFlags==0)&&(EquipFlag4.m_uLHighFlags==0)
		&&(EquipFlag4.m_uHLowFlags==0)&&(EquipFlag4.m_uHHighFlags==0))
	{
		return;
	}
	if (!(PosFlat0&&PosFlat1&&PosFlat2&&PosFlat3&&PosFlat4))
	{
		return;
	}

	//存在胡牌
	else
	{
		PosFlat0;
		UINT nSoulInfoCount = g_ItemTable.GetEquipSoulInfoCount();
		EQUIPSOUL_INFO_TB* pSoulTB = NULL;

		//最小屁胡
		UINT	nSoulPos = 1;	

		//查找胡牌
		bool bAlive = TRUE;
		for (UINT i=1; i<nSoulInfoCount; ++i)
		{
			bAlive = TRUE;
			pSoulTB = g_ItemTable.GetEquipSoulInfoTB(i);
			Assert(pSoulTB);
			if (!pSoulTB)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipSoulInfoTB index is error [%d]",i);
				return;
			}
			if (SoulValue0!=pSoulTB->m_ChainID5)
			{
				if (pSoulTB->m_ChainID5 != EQUIPPAI_ANYJIANG)
				{
					bAlive = FALSE;
					continue;
				}
			}
			if ((pSoulTB->m_AnyKeNum ==0)&&(pSoulTB->m_AnyShunNum ==0))
			{
				if (pSoulTB->ValidSoulNum != ValidSoulNum)
				{
					bAlive = FALSE;
					continue;
				}
				else
				{
					for (UINT j=0; j<pSoulTB->ValidSoulNum; ++j)
					{
						if(pSoulTB->SoulType[pSoulTB->ValidSoul[j]] != SoulType[pSoulTB->ValidSoul[j]])
							bAlive = FALSE;
					}
				}
			}
			else
			{
				if (pSoulTB->ValidSoulNum > ValidSoulNum)
				{
					bAlive = FALSE;
					continue;
				}
				else
				{
					for (UINT j=0; j<pSoulTB->ValidSoulNum; ++j)
					{
						if(pSoulTB->SoulType[pSoulTB->ValidSoul[j]] < SoulType[pSoulTB->ValidSoul[j]])
						{
							bAlive = FALSE;
							break;
						}
						else 
						{
							if (SoulFlat[j] == EQUIPPAI_ANYKE)
							{
								--AnyKeNum;
							}
							if (SoulFlat[j] == EQUIPPAI_ANYKE)
							{
								--AnyShunNum;
							}
						}
					}
				}
				if ((pSoulTB->m_AnyShunNum !=AnyShunNum)||(pSoulTB->m_AnyKeNum != AnyKeNum))
				{
					bAlive = FALSE;
				}
			}
			if (bAlive)
			{
				nSoulPos = i;
				break;
			}
		}

		//胡牌
		if ((nSoulPos != INVALID_INDEX)&&(nSoulPos != INVALID_SOUL_TYPE))
		{
			pSoulTB = g_ItemTable.GetEquipSoulInfoTB(nSoulPos);
			Assert(pSoulTB);
			if (!pSoulTB)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::CaculateEquipSoulAndChainEffect] GetEquipSoulInfoTB index is error [%d]",nSoulPos);
				return;
			}
			for (UINT i=0; i<pSoulTB->m_EffectAttCount; ++i)
			{
				CaculateEffect(pSoulTB->m_Att[i], -1);
			}
		}
	}
	
	__LEAVE_FUNCTION
}
