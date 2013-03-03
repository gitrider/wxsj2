///////////////////////////////////////////////////////////////////////////////
// 文件名：Combat_Core.cpp
// 程序员：高骐
// 功能说明：战斗逻辑内核
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/**
*
*/
#include "stdafx.h"
#include "Type.h"
#include "Obj_Character.h"
#include "Combat_Core.h"
#include "SkillInfo.h"
#include "SkillLogics.h"
#include "Skill_Core.h"
#include "LogicManager.h"
#include "Scene.h"
#include "StdImpact002.h"
#include "Impact_Core.h"
#include "StdImpact003.h"
#include "StdImpact061.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;
using namespace MenPai_Module;

namespace Combat_Module
{

	INT GetFinalDamages( INT iDamage, INT iResist )
	{
		iResist = Attr_VerifyResist(iResist);
		return iDamage * ( 100 - iResist ) / 100;
	}

	static DI_DamagesByValue_T	logicDamages;
	enum
	{
		IMPACT_DATA_INDEX_OF_DEFAULT_DAMAGES = 0,
	};
	//class CombatCore_T
	VOID CombatCore_T::Reset()
	{
		m_nAdditionalAttackNear		= 0;
		m_nAdditionalAttackFar		= 0;
		m_nAdditionalAttackMagicNear= 0;
		m_nAdditionalAttackMagicFar = 0;

		m_nAdditionalAttackGold		= 0;
		m_nAdditionalAttackWood		= 0;

		m_nAdditionalAttackWater	= 0;
		m_nAdditionalAttackFire		= 0;
		m_nAdditionalAttackSoil		= 0;
		
		m_nAdditionalDefenceNear	= 0;
		m_nAdditionalDefenceFar		= 0;
		m_nAdditionalDefenceMagicNear= 0;
		m_nAdditionalDefenceMagicFar= 0;

		m_nAdditionalDefenceGold	= 0;
		m_nAdditionalDefenceWood	= 0;

		m_nAdditionalDefenceWater	= 0;
		m_nAdditionalDefenceFire	= 0;
		m_nAdditionalDefenceSoil	= 0;
		
	}
	BOOL CombatCore_T::GetResultImpact(Obj_Character& rAttacker, Obj_Character& rDefencer, OWN_IMPACT& rImp)
	{
		__ENTER_FUNCTION
		INT nDamage = 0;
		INT nMaxDamage = 0;
		INT nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_NEAR;
		//Init
		DI_DamagesByValue_T logic;
		if(INVALID_ID==rImp.GetDataIndex())
		{
			g_ImpactCore.InitImpactFromData(IMP_DAMAGES_OF_ATTACKS, rImp, rAttacker);
		}
		else if(logic.ID!=Impact_GetLogicID(rImp) && DI_DamagesByRate_T::ID!=Impact_GetLogicID(rImp) )
		{
			g_ImpactCore.InitImpactFromData(IMP_DAMAGES_OF_ATTACKS, rImp, rAttacker);
		}
		m_nAdditionalAttackNear		+= logic.GetDamageNear(rImp);
		m_nAdditionalAttackFar		+= logic.GetDamageFar(rImp);
		m_nAdditionalAttackMagicNear+= logic.GetDamageMagicNear(rImp);
		m_nAdditionalAttackMagicFar += logic.GetDamageMagicFar(rImp);

		m_nAdditionalAttackGold		+= logic.GetDamageGold(rImp);
		m_nAdditionalAttackWood		+= logic.GetDamageWood(rImp);

		m_nAdditionalAttackWater	+= logic.GetDamageWater(rImp);
		m_nAdditionalAttackFire		+= logic.GetDamageFire(rImp);
		m_nAdditionalAttackSoil		+= logic.GetDamageSoil(rImp);
		

		//Damage fluctuation
		INT nFluctuation = g_Config.m_ConfigInfo.m_nDefaultDamageFluctuation;
		
		MenPaiID_t nMenPai = 9;
		MenPai_Module::MenPai_T const* pMenPai = NULL;
		if(Obj::OBJ_TYPE_HUMAN==rAttacker.GetObjType())
		{
			Obj_Human& rHuman = (Obj_Human&)rAttacker;
			nMenPai = rHuman.GetMenPai();
			pMenPai = g_MenPaiLogicList.GetLogicById(nMenPai);
			if(NULL!=pMenPai)
			{
				nFluctuation=pMenPai->GetAttackFluctuation();
			}
		}
		Scene* pScene = rAttacker.getScene();
		// calculate the fluctuation
		INT nRand=50;
		if(NULL!=pScene)
		{
			nRand -= pScene->GetRand100();
		}
		nFluctuation = Float2Int((2*nFluctuation*nRand)/100.0f);
		
		//Calculate Damages
		//Near
		nDamage = NearDamage(rAttacker, rDefencer, m_nAdditionalAttackNear, m_nAdditionalDefenceNear);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageNear(rImp, nDamage);
		nMaxDamage = nDamage;
		nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_NEAR;

		//Far
		nDamage = FarDamage(rAttacker, rDefencer, m_nAdditionalAttackFar, m_nAdditionalDefenceFar);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageFar(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_FAR;
		}
		//近程内功
		nDamage = MagicalNearDamage(rAttacker, rDefencer, m_nAdditionalAttackMagicNear, m_nAdditionalDefenceMagicNear);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageMagicNear(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_MAGIC_NEAR;
		}
		//近程外功
		nDamage = MagicalFarDamage(rAttacker, rDefencer, m_nAdditionalAttackMagicFar, m_nAdditionalDefenceMagicFar);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageMagicFar(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_MAGIC_FAR;
		}
		//Gold
		nDamage = GoldDamage(rAttacker, rDefencer, m_nAdditionalAttackGold, m_nAdditionalDefenceGold);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageGold(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_GOLD;
		}
		
		//Wood
		nDamage = WoodDamage(rAttacker, rDefencer, m_nAdditionalAttackWood, m_nAdditionalDefenceWood);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageWood(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_WOOD;
		}

		//Water
		nDamage = WaterDamage(rAttacker, rDefencer, m_nAdditionalAttackWater, m_nAdditionalDefenceWater);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageWater(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_WATER;
		}

		//Fire
		nDamage = FireDamage(rAttacker, rDefencer, m_nAdditionalAttackFire, m_nAdditionalDefenceFire);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageFire(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_FIRE;
		}

		//Soil
		nDamage = SoilDamage(rAttacker, rDefencer, m_nAdditionalAttackSoil, m_nAdditionalDefenceSoil);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageSoil(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_SOIL;
		}

		rImp.SetImpactID(nSpecialEffectID);

		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	

	INT CombatCore_T::CalculateHitRate(Obj_Character& rAttacker, Obj_Character& rTag)
	{
		__ENTER_FUNCTION
		INT nHit  = rAttacker.GetHit();
		INT nMiss = rTag.GetMiss();
		nHit = Attr_VerifyHitMiss(nHit);
		nMiss = Attr_VerifyHitMiss(nMiss);

		//INT iItemPointRefix  = 0;
		//INT iSkillPointRefix = 0;
		//if( rAttacker.GetObjType() == Obj::OBJ_TYPE_HUMAN )
		//{
		//	//装备对属性的点数影响
		//	iItemPointRefix  = static_cast<Obj_Human&>(rAttacker).GetIattributeHit();
		//}
		//iSkillPointRefix = rAttacker.GetHitRefix();

		//INT iItemPointRefixMiss  = 0;
		//INT iSkillPointRefixMiss = 0;
		//if( rTag.GetObjType() == Obj::OBJ_TYPE_HUMAN )
		//{
		//	//装备对属性的点数影响
		//	iItemPointRefixMiss  = static_cast<Obj_Human&>(rTag).GetIattributeMiss();
		//}
		//iSkillPointRefixMiss = rTag.GetMissRefix();

		INT iRateHit = 0;
		if (Obj::OBJ_TYPE_HUMAN==rAttacker.GetObjType())
		{
			Obj_Human& rHuman = (Obj_Human&)rAttacker;
			_ITEM_EFFECT* pIE=NULL;
			pIE = rHuman.ItemEffect(IATTRIBUTE_RATE_HIT);
			Assert( pIE );
			if( pIE->IsActive() )
			{
				iRateHit = pIE->m_Attr.m_Value;
			}
		}

		//总命中率=（75+攻击方总命中-受击方总闪避）/100+攻击方装备对命中率的影响+攻击方技能对命中率的影响-受击方装备对闪避率的影响-受击方技能对闪避率的影响
		SkillInfo_T& oSkillInf = rAttacker.GetSkillInfo();
		INT iRefix = oSkillInf.GetAccuracy();
		if( iRefix < 0 )
		{
			iRefix = 0;
		}
		INT iFainlHitRate = ((75 + nHit - nMiss)/100.0 + iRateHit/100.0 + iRefix/100.0) * 100.0;

		return iFainlHitRate;
		__LEAVE_FUNCTION
		return 0;
	}

	BOOL CombatCore_T::IsHit(INT nHitRate, INT nRand)
	{
		__ENTER_FUNCTION

		0>nHitRate?nHitRate=0:NULL;
		100<nHitRate?nHitRate=100:NULL;
		if(nRand<nHitRate)
		{
			return TRUE;
		}
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CombatCore_T::IsCriticalHit(Obj_Character& rAttacker, INT nCriticalRate, INT nRand, INT iToughness)
	{
		__ENTER_FUNCTION

		INT iSkillPointRefix = 0;

		SkillInfo_T& rSkillInf = rAttacker.GetSkillInfo();
		iSkillPointRefix = rSkillInf.GetCriticalRate();
		if( 0 > iSkillPointRefix )
		{
			iSkillPointRefix = 0;
		}

		INT iRateCrit = 0;
		if (Obj::OBJ_TYPE_HUMAN==rAttacker.GetObjType())
		{
			Obj_Human& rHuman = (Obj_Human&)rAttacker;
			_ITEM_EFFECT* pIE=NULL;
			pIE = rHuman.ItemEffect(IATTRIBUTE_RATE_CRIT);
			Assert( pIE );
			if( pIE->IsActive() )
			{
				iRateCrit = pIE->m_Attr.m_Value;
			}
		}

		//暴击率=（5+攻击方总暴击-受击方总韧性）/100+攻击方装备对暴击率的影响+攻击方技能对暴击率的影响
		INT iFanlCriticalRate = Float2Int((( 5 + nCriticalRate - iToughness )/100.0 + iRateCrit/100.0 +iSkillPointRefix/100.f) * 100.0);
		iFanlCriticalRate = Attr_VerifyCritical(iFanlCriticalRate);

		if( nRand < iFanlCriticalRate )
			return TRUE;//会心一击
		return FALSE;//普通一击
		__LEAVE_FUNCTION
		return FALSE;
	}

	INT CombatCore_T::GoldDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT	nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackGold()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceGold()+nAdditionalResist);
		nDamage = AttrDamage(rMe, nResist, nAttack, IATTRIBUTE_SHIELD_GOLD);

		INT nIgnoreRate = 0;
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate= (INT)(rTar.ItemValue(IATTRIBUTE_RATE_GOLD_ATT_REDUCE).m_Value);
		}
		nDamage = nDamage*(100.0-((double)nIgnoreRate))/100.0f;

		return nDamage;
	}

	INT CombatCore_T::WoodDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackWood()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceWood()+nAdditionalResist);
		nDamage = AttrDamage(rMe, nResist, nAttack, IATTRIBUTE_SHIELD_WOOD);

		INT nIgnoreRate = 0;
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate= (INT)(rTar.ItemValue(IATTRIBUTE_RATE_WOOD_ATT_REDUCE).m_Value);
		}
		nDamage = nDamage*(100.0-((double)nIgnoreRate))/100.0f;

		return nDamage;
	}

	INT CombatCore_T::WaterDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackWater()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceWater()+nAdditionalResist);
		nDamage = AttrDamage(rMe, nResist, nAttack, IATTRIBUTE_SHIELD_WATER);

		INT nIgnoreRate = 0;
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate= (INT)(rTar.ItemValue(IATTRIBUTE_RATE_WATER_REDUCE).m_Value);
		}
		nDamage = nDamage*(100.0-((double)nIgnoreRate))/100.0f;

		return nDamage;
	}

	INT CombatCore_T::FireDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackFire()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceFire()+nAdditionalResist);
		nDamage = AttrDamage(rMe, nResist, nAttack, IATTRIBUTE_SHIELD_FIRE);

		INT nIgnoreRate = 0;
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate= (INT)(rTar.ItemValue(IATTRIBUTE_RATE_FIREATT_REDUCE).m_Value);
		}
		nDamage = nDamage*(100.0-((double)nIgnoreRate))/100.0f;

		return nDamage;
	}

	INT CombatCore_T::SoilDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackSoil()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceSoil()+nAdditionalResist);
		nDamage = AttrDamage(rMe, nResist, nAttack, IATTRIBUTE_SHIELD_SOIL);

		INT nIgnoreRate = 0;
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate= (INT)(rTar.ItemValue(IATTRIBUTE_RATE_SOIL_ATT_REDUCE).m_Value);
		}
		nDamage = nDamage*(100.0-((double)nIgnoreRate))/100.0f;

		return nDamage;
	}

	INT CombatCore_T::NearDamage( Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack/*=0*/, INT nAdditionDefence/*=0*/ )
	{
		__ENTER_FUNCTION

		INT nAttack = Attr_VerifyGeneralAttack(rMe.GetAttackNear()+nAdditionAttack);
		INT nDefence = Attr_VerifyDefence(rTar.GetDefenceNear()+nAdditionDefence);
		INT	nDamage;
		nDamage = NormalDamage(rTar, nAttack, nDefence, IATTRIBUTE_SHIELD_NEAR );

		INT iResist =  rTar.GetNearAttReduce();
		nDamage = GetFinalDamages(nDamage, iResist);

		return nDamage;
		__LEAVE_FUNCTION
		return 0;
	}

	INT CombatCore_T::FarDamage( Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack/*=0*/, INT nAdditionDefence/*=0*/ )
	{
		__ENTER_FUNCTION

		INT	nDamage;
		INT nAttack = Attr_VerifyGeneralAttack(rMe.GetAttackFar()+nAdditionAttack);
		INT nDefence = Attr_VerifyDefence(rTar.GetDefenceFar()+nAdditionDefence);
		nDamage = NormalDamage(rTar, nAttack, nDefence, IATTRIBUTE_SHIELD_FAR );

		INT iResist =  rTar.GetFarAttReduce();
		nDamage = GetFinalDamages(nDamage, iResist);
		return nDamage;
		__LEAVE_FUNCTION
			return 0;
	}

	INT CombatCore_T::MagicalNearDamage(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack, INT nAdditionDefence)
	{
		__ENTER_FUNCTION
		INT	nDamage;
		INT nAttack = Attr_VerifyGeneralAttack(rMe.GetAttackMagicNear()+nAdditionAttack);
		INT nDefence = Attr_VerifyDefence(rTar.GetDefenceMagicNear()+nAdditionDefence);
		nDamage = NormalDamage(rTar, nAttack, nDefence, IATTRIBUTE_SHIELD_MAGIC_NEAR);

		INT iResist =  rTar.GetMagicNearAttReduce();
		nDamage = GetFinalDamages(nDamage, iResist);
		return nDamage;
		__LEAVE_FUNCTION
			return 0;
	}
	INT CombatCore_T::MagicalFarDamage(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack, INT nAdditionDefence)
	{
		__ENTER_FUNCTION
		INT	nDamage;
		INT nAttack = Attr_VerifyGeneralAttack(rMe.GetAttackMagicFar()+nAdditionAttack);
		INT nDefence = Attr_VerifyDefence(rTar.GetDefenceMagicFar()+nAdditionDefence);
		nDamage = NormalDamage(rTar, nAttack, nDefence, IATTRIBUTE_SHIELD_MAGIC_FAR);

		INT iResist =  rTar.GetMagicFarAttReduce();
		nDamage = GetFinalDamages(nDamage, iResist);
		return nDamage;
		__LEAVE_FUNCTION
		return 0;
	}

	INT CombatCore_T::NormalDamage( Obj_Character &rTar, INT nAttack, INT nDefence, INT iType )
	{
		INT nDamage;
		INT nIgnoreRate = 0;
		// calculate damage
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate = rTar.ItemValue(iType).m_Value;
		}
		if(0==nAttack+nDefence)
		{
			//never enter here
			nDamage = 0;
		}
		else
		{
			nDamage = nAttack - nDefence;
		}
		nDamage -= nIgnoreRate;
		0>nDamage?nDamage=0:NULL;
		return nDamage;
	}

	INT CombatCore_T::AttrDamage( Obj_Character &rTar, INT &nResist, INT nAttack, INT iType )
	{
		INT nDamage;
		INT nIgnoreRate = 0;
		if(MAX_EFFECTIVE_RESIST<nResist)
		{
			nResist = MAX_EFFECTIVE_RESIST;
		}
		if(-MAX_EFFECTIVE_RESIST>nResist)
		{
			nResist = -MAX_EFFECTIVE_RESIST;
		}
		nDamage = Float2Int(nAttack*(100-nResist)/100);
		
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate = rTar.ItemValue(iType).m_Value;
		}
		
		nDamage -= nIgnoreRate;
		0>nDamage?nDamage=0:NULL;
		return nDamage;
	}
}

