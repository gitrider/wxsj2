/********************************************************************
	created:	2008/05/04
	created:	4:5:2008   19:22
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact062.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact062
	file ext:	cpp
	author:		Richard
	
	purpose:	骑乘专用一次性百分比伤害
*********************************************************************/

#include "stdafx.h"
#include "StdImpact069.h"
#include "Obj_Human.h"


using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact069_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION

				return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}
		VOID StdImpact069_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
				if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
				{
					return;
				}

				InitDamages( rImp );

				INT iResult = GetRefixPara( rImp, rMe );

				//按伤害类型初始化vDamage
				INT vDamage[DAMAGE_TYPE_NUMBER];
				//vDamage[DAMAGE_TYPE_NEAR]	= (GetDamageNear(rImp)	+100) / 100;
				//vDamage[DAMAGE_TYPE_FAR]	= (GetDamageFar(rImp)	+100) / 100;
				//vDamage[DAMAGE_TYPE_MAGIC_NEAR]		= (GetDamageMagicNear(rImp)	+100) / 100;
				//vDamage[DAMAGE_TYPE_MAGIC_FAR]		= (GetDamageMagicNear(rImp)	+100) / 100;
				//vDamage[DAMAGE_TYPE_GOLD]	= (GetDamageGold(rImp)	+100) / 100;
				//vDamage[DAMAGE_TYPE_WOOD]	= (GetDamageWood(rImp) +100) / 100;
				//vDamage[DAMAGE_TYPE_WATER]	= (GetDamageWater(rImp)+100) / 100;
				//vDamage[DAMAGE_TYPE_FIRE]	= (GetDamageFire(rImp)	+100) / 100;
				//vDamage[DAMAGE_TYPE_SOIL]	= (GetDamageSoil(rImp) +100) / 100;
				
				vDamage[DAMAGE_TYPE_NEAR]	= GetDamageNear(rImp);
				vDamage[DAMAGE_TYPE_FAR]	= GetDamageFar(rImp);
				vDamage[DAMAGE_TYPE_MAGIC_NEAR]	= GetDamageMagicNear(rImp);
				vDamage[DAMAGE_TYPE_MAGIC_FAR]	= GetDamageMagicFar(rImp);

				vDamage[DAMAGE_TYPE_GOLD]	= GetDamageGold(rImp);
				vDamage[DAMAGE_TYPE_WOOD]	= GetDamageWood(rImp);
				vDamage[DAMAGE_TYPE_WATER]	= GetDamageWater(rImp);
				vDamage[DAMAGE_TYPE_FIRE]	= GetDamageFire(rImp);
				vDamage[DAMAGE_TYPE_SOIL]	= GetDamageSoil(rImp);

				for( INT i=0; i<DAMAGE_TYPE_NUMBER; ++i )
				{
					if( 0 == vDamage[i] )
					{
						continue;
					}
					vDamage[i] = ((vDamage[i] + 100) * iResult) / 100;
				}
				FinalDamage(vDamage, rMe);


				//产生伤害
				rMe.OnDamages(vDamage, rImp.GetCasterObjID(), rImp.IsCriticalHit(), rImp.GetSkillID(), rImp.GetCasterLogicCount());
				__LEAVE_FUNCTION
		}
		VOID StdImpact069_T::CriticalRefix(OWN_IMPACT& rImp) const
		{
			__ENTER_FUNCTION
				SetDamageNear(rImp, GetDamageNear(rImp)*2);
			SetDamageFar(rImp, GetDamageFar(rImp)*2);
			SetDamageMagicNear(rImp, GetDamageMagicNear(rImp)*2);
			SetDamageMagicFar(rImp, GetDamageMagicFar(rImp)*2);
			SetDamageGold(rImp, GetDamageGold(rImp)*2);
			SetDamageWood(rImp, GetDamageWood(rImp)*2);
			SetDamageWater(rImp, GetDamageWater(rImp)*2);
			SetDamageFire(rImp, GetDamageFire(rImp)*2);
			SetDamageSoil(rImp, GetDamageSoil(rImp)*2);
			rImp.MarkCriticalFlag();
			__LEAVE_FUNCTION
		}
		BOOL StdImpact069_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
				nRate += 100;
			INT iTemp = GetDamageNear(rImp);
			SetDamageNear(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageFar(rImp);
			SetDamageFar(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageMagicNear(rImp);
			SetDamageMagicNear(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageMagicFar(rImp);
			SetDamageMagicFar(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageGold(rImp);
			SetDamageGold(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageWood(rImp);
			SetDamageWood(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageWater(rImp);
			SetDamageWater(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageFire(rImp);
			SetDamageFire(rImp, Float2Int((iTemp*nRate)/100.0f));

			iTemp = GetDamageSoil(rImp);
			SetDamageSoil(rImp, Float2Int((iTemp*nRate)/100.0f));

			__LEAVE_FUNCTION
				return TRUE;
		}

		VOID StdImpact069_T::InitDamages( OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION
			SetParaID
			(
				rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_PARA_ID,
				rImp.GetSkillLevel()
				)
			);
			SetRefixRate
			(
				rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_RATE,
				rImp.GetSkillLevel()
				)
			);

			SetDamageNear
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_NEAR,
				rImp.GetSkillLevel()
				)
				);
			SetDamageFar
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_FAR,
				rImp.GetSkillLevel()
				)
				);
			SetDamageMagicNear
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_MAGIC_NEAR,
				rImp.GetSkillLevel()
				)
				);
			SetDamageMagicFar
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_MAGIC_FAR,
				rImp.GetSkillLevel()
				)
				);
			SetDamageGold
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_GOLD,
				rImp.GetSkillLevel()
				)
				);

			SetDamageWood
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_WOOD,
				rImp.GetSkillLevel()
				)
				);

			SetDamageWater
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_WATER,
				rImp.GetSkillLevel()
				)
				);

			SetDamageFire
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_FIRE,
				rImp.GetSkillLevel()
				)
				);

			SetDamageSoil
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_DAMAGE_SOIL,
				rImp.GetSkillLevel()
				)
				);

			__LEAVE_FUNCTION
		}
		INT StdImpact069_T::GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const
		{
			FLOAT fRate = iRate / 1000.0f;
			return (INT)((iPara1*fPara2/1000.0f + iPara3*iPara4/1000.0f) * fRate);
		}

		INT StdImpact069_T::GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe ) const
		{
			INT iPara = GetParaID( rImp );
			INT iRate = GetRefixRate( rImp );
			Obj_Character* pChar = (Obj_Character*)Impact_GetCaster( rMe, rImp );
			if( NULL == pChar)
			{
				return 0;
			}
			Obj_Human* rHuman = static_cast<Obj_Human*>(pChar);
			PET_GUID_t guidHorse = rHuman->GetGUIDOfCallUpHorse();

			if( guidHorse.IsNull() )
			{
				return 0;
			}

			Item* pPetItem = rHuman->GetHorseItem(guidHorse);
			if(NULL == pPetItem) 
			{
				return 0;
			}

			INT iResult = 0;
			switch( iPara )
			{
			case 0://力量
				iResult = GetResult( pPetItem->GetLevel(), pPetItem->GetGrowRate(), pPetItem->GetStrengthPer(), pPetItem->GetLvl1Attr(CATTR_LEVEL1_STR), iRate );
				break;																			  
			case 1://敏捷	
				iResult = GetResult( pPetItem->GetLevel(), pPetItem->GetGrowRate(), pPetItem->GetSmartnessPer(), pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX), iRate );
				break;
			case 2://智利
				iResult = GetResult( pPetItem->GetLevel(), pPetItem->GetGrowRate(), pPetItem->GetMindPer(), pPetItem->GetLvl1Attr(CATTR_LEVEL1_INT), iRate );
				break;
			case 3://体质
				iResult = GetResult( pPetItem->GetLevel(), pPetItem->GetGrowRate(), pPetItem->GetConstitutionPer(), pPetItem->GetLvl1Attr(CATTR_LEVEL1_CON), iRate );
				break;
			default:
				break;
			}
			return iResult;
		}
		VOID StdImpact069_T::FinalDamage( INT * vDamage, Obj_Character &rMe ) const
		{
			vDamage[DAMAGE_TYPE_NEAR]	-= rMe.GetDefenceNear();
			if( rMe.GetNearAttReduce() != 0 )
				vDamage[DAMAGE_TYPE_NEAR]	/= rMe.GetNearAttReduce();

			vDamage[DAMAGE_TYPE_FAR]	-= rMe.GetDefenceFar();
			if( rMe.GetNearAttReduce() != 0 )
				vDamage[DAMAGE_TYPE_FAR]	/= rMe.GetFarAttReduce();

			vDamage[DAMAGE_TYPE_MAGIC_NEAR]		-= rMe.GetDefenceMagicNear();
			if( rMe.GetNearAttReduce() != 0 )
				vDamage[DAMAGE_TYPE_MAGIC_NEAR]		/= rMe.GetMagicNearAttReduce();

			vDamage[DAMAGE_TYPE_MAGIC_FAR]		-= rMe.GetDefenceMagicFar();
			if( rMe.GetFarAttReduce() != 0 )
				vDamage[DAMAGE_TYPE_MAGIC_FAR]		/= rMe.GetMagicFarAttReduce();

			vDamage[DAMAGE_TYPE_GOLD]	-= rMe.GetDefenceGold();
			vDamage[DAMAGE_TYPE_WOOD]	-= rMe.GetDefenceWood();	
			vDamage[DAMAGE_TYPE_WATER]	-= rMe.GetDefenceWater();
			vDamage[DAMAGE_TYPE_FIRE]	-= rMe.GetDefenceFire();	
			vDamage[DAMAGE_TYPE_SOIL]	-= rMe.GetDefenceSoil();
		}

	};
};

