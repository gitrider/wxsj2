//***************************************************************************************************
// 文件名称:	MenPai.cpp
// 系统名称:	
// 模块名称:	MenPai
// 功能说明:	所有门派的实现
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
//				修改时间:2007/10/30
//				修改内容:属性第一部分重构(具体参见文档)
//          2.
//				修 改 人:liam
//				修改时间:2007/11/02
//				修改内容:属性第二部分重构(具体参见文档)
//***************************************************************************************************
#include "stdafx.h"
#include "MenPai.h"
#include "GameTable.h"
namespace MenPai_Module
{
	SHORT MenPai_T::GetInitMaxHP(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_HP, GetID());
	}

	SHORT MenPai_T::GetMaxHPConRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_CON_HP, GetID());
	}

	SHORT MenPai_T::GetMaxHPLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_HP, GetID());		
	}

	//HP Regenerate
	SHORT MenPai_T::GetInitHPRegenerate(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_HPRESTORE, GetID());
	}

	SHORT MenPai_T::GetHPRegenerateLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_HPRESTORE, GetID());
	}

	//MaxMP
	SHORT MenPai_T::GetInitMaxMP(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_MP, GetID());
	}

	SHORT MenPai_T::GetMaxMPIntRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_INT_MP, GetID());
	}

	SHORT MenPai_T::GetMaxMPLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_MP, GetID());
	}

	//MP Regenerate
	SHORT MenPai_T::GetInitMPRegenerate(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_MPRESTORE, GetID());
	}

	SHORT MenPai_T::GetMPRegenerateLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_MPRESTORE, GetID() ) ;
	}

	//MaxPneuma
	SHORT MenPai_T::GetInitMaxPneuma(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_MAX_PNEUMA, GetID());
	}

	SHORT MenPai_T::GetMaxPneumaLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_MAX_PNEUMA, GetID());
	}

	//Pneuma Regenerate
	SHORT MenPai_T::GetInitPneumaRegenerate(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_RESTORE_PNEUMA, GetID());
	}

	SHORT MenPai_T::GetPneumaRegenerateLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_RESTORE_PNEUMA, GetID());
	}

	//MaxRAGE
	SHORT MenPai_T::GetInitMaxRage(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_RAGE, GetID());
	}

	SHORT MenPai_T::GetMaxRageIntRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_INT_RAGE, GetID());
	}

	SHORT MenPai_T::GetMaxRageLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_RAGE, GetID());
	}

	//RAGE Regenerate
	SHORT MenPai_T::GetInitRageRegenerate(VOID) const
	{
		return 0 /*g_BaseValueTbl.Get(AINFOTYPE_BASE_RAGERESTORE, GetID())*/;
	}

	SHORT MenPai_T::GetRageRegenerateLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_RAGERESTORE, GetID() ) ;
	}
	
	//Attr Level 1 Sstr

	//等级对力道影响(从BaseValueTable.ini读取。一个门派所有级别全局控制)
	SHORT MenPai_T::GetBaseStrLevelupRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_STRENGTH, GetID() ) ;
	}

	//等级对力道影响(从attr_lvup.tab读取。一个门派每一个等级的分点控制)
	SHORT MenPai_T::GetStrLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL_STR,GetID(),nLevel);
	}


	//Attr Level 1 Spr
	/*SHORT	MenPai_T::GetInitSpr(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_SPR,GetID(),0);
	}

	SHORT MenPai_T::GetSprLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_SPR,GetID(),nLevel);
	}*/

	//Attr Level 1 Con
	SHORT MenPai_T::GetInitCon(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL_CON,GetID(),0);
	}

	SHORT MenPai_T::GetBaseConLevelupRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_CON, GetID()) ;
	}

	SHORT MenPai_T::GetConLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL_CON,GetID(),nLevel);
	}

	//Attr Level 1 Int
	SHORT MenPai_T::GetInitInt(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL_INT,GetID(),0);
	}

	//等级对内力影响(从BaseValueTable.ini读取。一个门派所有级别全局控制)
	SHORT MenPai_T::GetBaseIntLevelupRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_INT, GetID()) ;
	}

	SHORT MenPai_T::GetIntToughness() const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_INT_TOUGHNESS, GetID()) ;
	}

	SHORT MenPai_T::GetIntLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL_INT,GetID(),nLevel);
	}

	//Attr Level 1 Dex
	SHORT MenPai_T::GetInitDex(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL_DEX,GetID(),0);
	}

	//等级对身法影响(从BaseValueTable.ini读取。一个门派所有级别全局控制)
	SHORT MenPai_T::GetBaseDexLevelupRefix() const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_DEX, GetID()) ;
	}

	SHORT MenPai_T::GetDexLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL_DEX,GetID(),nLevel);
	}
	//////Attr Level 2 Attack Physics
	//SHORT MenPai_T::GetInitAttackPhysics(VOID) const
	//{
	//	return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_P, GetID());
	//}

	//SHORT MenPai_T::GetAttackPhysicsStrRefix(VOID) const
	//{
	//	return g_BaseValueTbl.Get(AINFOTYPE_STR_ATTACK_P, GetID());
	//}

	//SHORT MenPai_T::GetAttackPhysicsLevelRefix(VOID) const
	//{
	//	return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_P, GetID());
	//}

	//////Attr Level 2 Defence Physics
	//SHORT MenPai_T::GetInitDefencePhysics(VOID) const
	//{
	//	return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_P, GetID());
	//}

	//SHORT MenPai_T::GetDefencePhysicsLevelRefix(VOID) const
	//{
	//	return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_DEFENCE_P, GetID());
	//}
	
	//add by liam-------------------------------------------------------------
	//近程防御
	SHORT MenPai_T::GetInitNearDefencePhysics(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_NEARDEFENCE_P, GetID());
	}
	//等级对近程防御的影响系数
	SHORT MenPai_T::GetNearDefencePhysicsLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_NEARDEFENCE_P, GetID());
	}
	//远程防御
	SHORT MenPai_T::GetInitFarDefencePhysics(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_FARDEFENCE_P, GetID());
	}
	//等级对远程防御影响
	SHORT MenPai_T::GetFarDefencePhysicsLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_FARDEFENCE_P, GetID());
	}

	////初始近程攻击
	SHORT MenPai_T::GetInitNearAttackPhysics(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_NEARATTACK, GetID());
	}

	//内力对远程内功攻击的影响系数
	SHORT MenPai_T::GetNearAttackPhysicsIntRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_INT_NEARATTACK, GetID());
	}

	//等级对近程攻击的影响系数
	SHORT MenPai_T::GetNearAttackPhysicsLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_NEARATTACK, GetID());
	}
	
	////初始远程攻击
	SHORT MenPai_T::GetInitFarAttackPhysics(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_FARATTACK, GetID());
	}

	//力道对远程外攻攻击的影响系数
	SHORT MenPai_T::GetFarAttackPhysicsStrRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_STR_FARATTACK, GetID());
	}

	//等级对远程攻击的影响系数
	SHORT MenPai_T::GetFarAttackPhysicsLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_FARATTACK, GetID());
	}

	//初始韧性   
	SHORT MenPai_T::GetInitToughness(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_TOUGHNESS, GetID());
	}

	//等级对韧性的影响系数
	SHORT MenPai_T::GetToughnessLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_TOUGHNESS, GetID());
	}

	//初始金系攻击力
	SHORT MenPai_T::GetInitAttackGold(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_GOLD, GetID());
	}

	//初始木系攻击力
	SHORT MenPai_T::GetInitAttackWood(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_WOOD, GetID());
	}

	//初始水系攻击力
	SHORT MenPai_T::GetInitAttackWater(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_WATER, GetID());
	}

	//初始火系攻击力
	SHORT MenPai_T::GetInitAttackFire(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_FIRE, GetID());
	}
	
	//初始土系攻击力
	SHORT MenPai_T::GetInitAttackSoil(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_SOIL, GetID());
	}

	//初始金系防御力
	SHORT MenPai_T::GetInitDefenceGold(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_GOLD, GetID());
	}

	//初始木系防御力
	SHORT MenPai_T::GetInitDefenceWood(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_WOOD, GetID());
	}

	//初始水系防御力
	SHORT MenPai_T::GetInitDefenceWater(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_WATER, GetID());
	}

	//初始火系防御力
	SHORT MenPai_T::GetInitDefenceFire(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_FIRE, GetID());
	}

	//初始土系防御力
	SHORT MenPai_T::GetInitDefenceSoil(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_SOIL, GetID());
	}

	//等级对活力上限值的影响
	SHORT MenPai_T::GetMaxVigorLevelupPoint(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_MAX_VIGOR, GetID());
	}

	//等级对活力回复的影响系数
	SHORT MenPai_T::GetLevelRestoreVigor(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_RESTORE_VIGOR, GetID());
	}

	SHORT MenPai_T::GetLevelAttackGold(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_GOLD, GetID());
	}

	SHORT MenPai_T::GetLevelAttackWood(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_WOOD, GetID());
	}

	SHORT MenPai_T::GetLevelAttackWater(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_WATER, GetID());
	}

	SHORT MenPai_T::GetLevelAttackFire(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_FIRE, GetID());
	}

	SHORT MenPai_T::GetLevelAttackSoil(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_SOIL, GetID());
	}

	SHORT MenPai_T::GetLevelDefenceGold(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_DEFENCE_GOLD, GetID());
	}

	SHORT MenPai_T::GetLevelDefenceWood(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_DEFENCE_WOOD, GetID());
	}

	SHORT MenPai_T::GetLevelDefenceWater(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_DEFENCE_WATER, GetID());
	}

	SHORT MenPai_T::GetLevelDefenceFire(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_DEFENCE_FIRE, GetID());
	}

	SHORT MenPai_T::GetLevelDefenceSoil(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_DEFENCE_SOIL, GetID());
	}

	SHORT MenPai_T::GetRestoreRageFromEXP(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_RESTORE_RAGE_FROM_EXP, GetID());
	}

	SHORT MenPai_T::GetBaseVigorRestore(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_VIGOR_RESTORE, GetID());
	}
	
	//------------------------------------------------------------------------
	//Attr Level 2 
	SHORT MenPai_T::GetInitAttackMagicNear(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_MAGIC_NEAR, GetID());
	}
	SHORT MenPai_T::GetInitAttackMagicFar(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_MAGIC_FAR, GetID());
	}

	SHORT MenPai_T::GetAttackMagicNearIniRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_DEX_ATTACK_MAGIC_NEAR, GetID());
	}
	SHORT MenPai_T::GetAttackMagicFarIniRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_STR_ATTACK_MAGIC_FAR, GetID());
	}

	SHORT MenPai_T::GetAttackMagicNearLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_MAGIC_NEAR, GetID());
	}
	SHORT MenPai_T::GetAttackMagicFarLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_MAGIC_FAR, GetID());
	}

	//Attr Level 2 
	SHORT MenPai_T::GetInitDefenceMagicNear(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_MAGIC_NEAR, GetID());
	}
	SHORT MenPai_T::GetInitDefenceMagicFar(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_MAGIC_FAR, GetID());
	}

	SHORT MenPai_T::GetDefenceMagicNearLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_DEFENCE_MAGIC_NEAR,GetID());
	}
	SHORT MenPai_T::GetDefenceMagicFarLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_DEFENCE_MAGIC_FAR,GetID());
	}

	//Attr Level 2 Hit
	SHORT MenPai_T::GetInitHit(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_HIT, GetID());
	}

	SHORT MenPai_T::GetHitStrRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_STR_HIT, GetID());
	}

	SHORT MenPai_T::GetHitLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_HIT, GetID());
	}

	//Attr Level 2 Critical
	SHORT MenPai_T::GetInitCritical(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_CRITRATE, GetID());
	}

	SHORT MenPai_T::GetCriticalIntRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_INT_CRITRATE, GetID());
	}

	//敏捷对暴击的影响
	SHORT MenPai_T::GetCriticalDexRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_DEX_CRITRATE, GetID());
	}

	SHORT MenPai_T::GetCriticalLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_CRITRATE, GetID());
	}

	//Attr Level 2 Miss
	SHORT MenPai_T::GetInitMiss(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_MISS, GetID());
	}

	SHORT MenPai_T::GetMissDexRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_DEX_MISS, GetID());
	}

	SHORT MenPai_T::GetMissLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_MISS, GetID());
	}
	//Attr Attack Fluctuation
	SHORT MenPai_T::GetAttackFluctuation(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_ATTACK_FLUCTUATION, GetID());
	}
	VOID MenPai_T::OnDamage(Obj_Human& rMe, INT nDamage) const
	{
		//if(0>=nDamage)
		//{
		//	return;
		//}

		//INT nRage = nDamage/40;
		//rMe.RefixRageRegeneration(nRage);
		//rMe.RageIncrement(nRage,rMe.GetLogicCount(),NULL);
	}
	VOID MenPai_T::OnDamageTarget(Obj_Human& rMe, Obj_Character& rTar, INT& nDamage) const
	{
		__ENTER_FUNCTION

		if(0 >= nDamage)
		{
			return;
		}

		//如果攻击方与被攻击方都是玩家，则判断被攻击方是否有“门派减少伤害”的属性
		if (Obj::OBJ_TYPE_HUMAN == rTar.GetObjType())
		{
			Obj_Human& rHumanTar = (Obj_Human&)(*(&rTar));

			MENPAI_ATTRIBUTE meMenPaiID = rMe.GetMenPai();
			MENPAI_ATTRIBUTE tarMenPaiID = rHumanTar.GetMenPai();

			Assert(meMenPaiID >= MATTRIBUTE_SHAOLIN_GWS && meMenPaiID <= MATTRIBUTE_WUMENPAI);
			if (meMenPaiID < MATTRIBUTE_SHAOLIN_GWS || meMenPaiID > MATTRIBUTE_WUMENPAI)
			{
				return;
			}

			Assert(tarMenPaiID >= MATTRIBUTE_SHAOLIN_GWS && tarMenPaiID <= MATTRIBUTE_WUMENPAI);
			if (tarMenPaiID < MATTRIBUTE_SHAOLIN_GWS || tarMenPaiID > MATTRIBUTE_WUMENPAI)
			{
				return;
			}

			ITEM_ATTRIBUTE nReduceDamageID = (ITEM_ATTRIBUTE)(meMenPaiID - MATTRIBUTE_SHAOLIN_GWS + IATTRIBUTE_REDUCE_DAMAGE_SHAOLIN_GWS);

			_ITEM_EFFECT* pIE = rHumanTar.ItemEffect(nReduceDamageID);
			Assert(pIE);
			if(!pIE->IsActive())
			{
				return;
			}

			nDamage = Float2Int((100 - pIE->m_Attr.m_Value) * nDamage/100.0f);			
		}

		__LEAVE_FUNCTION

		return;
	}
}

