/********************************************************************
	created:	2008/05/10
	created:	10:5:2008   18:49
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact074.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact074
	file ext:	cpp
	author:		Richard
	
	purpose:	马专用按点数或百分比修正HP和怒气
*********************************************************************/

#include "stdafx.h"
#include "StdImpact074.h"
#include "Obj_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact074_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetHpModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_HP,
				rImp.GetSkillLevel()
				)
			);
			SetMpModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_MP,
				rImp.GetSkillLevel()
				)
			);
			SetRageModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_RAGE,
				rImp.GetSkillLevel()
				)
			);
			SetRage1Modification
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_RAGE1,
				rImp.GetSkillLevel()
				)
				);
			SetRage2Modification
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_RAGE2,
				rImp.GetSkillLevel()
				)
				);
			SetRage3Modification
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_RAGE3,
				rImp.GetSkillLevel()
				)
				);
			SetRage4Modification
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_RAGE4,
				rImp.GetSkillLevel()
				)
				);
			SetRage5Modification
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_RAGE5,
				rImp.GetSkillLevel()
				)
				);
			SetRefixRate
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_RATE,
				rImp.GetSkillLevel()
				)
			);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact074_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			Obj_Character * pImpactSender = (Obj_Character*)Impact_GetCaster(rMe, rImp);
			if( pImpactSender == NULL )
			{
				return;
			}
			//马对人的影响数值
			INT iRefixPara = GetRefixPara( rImp, rMe );
			INT iRefixType = GetRefixType( rImp );

			INT iDamage = GetDamageRefix( pImpactSender, rImp );

			//process HP
			INT nHP = GetHpModification(rImp);
			if( iRefixType == 1 )
			{
				nHP += 100;
				nHP = Float2Int((nHP*rMe.GetBaseMaxHP())/100.0f);
			}

			nHP += iDamage + iRefixPara;

			rMe.HealthIncrement(nHP, rImp.GetCasterLogicCount(), pImpactSender);
			if(NULL==pImpactSender)
			{
				pImpactSender->OnHealTarget(rImp.GetSkillID(), rMe, nHP);
			}

			if (rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN)
			{
				//process MP
				INT nMP = GetMpModification(rImp);
				if( iRefixType == 1 )
				{
					nHP += 100;
					nMP = Float2Int((nMP*rMe.GetBaseMaxMP())/100.0f);
				}
				nMP += iDamage + iRefixPara;
				rMe.ManaIncrement(nMP, rImp.GetCasterLogicCount(), pImpactSender);

				//Process Rage
				INT iRageModify = static_cast<Obj_Human&>(rMe).GetMenPai();
				INT nRage = 0;
				switch(iRageModify)
				{
				case 0:
					nRage = GetRageModification(rImp);
					break;
				case 1:
					nRage = GetRage1Modification(rImp);
					break;
				case 2:
					nRage = GetRage2Modification(rImp);
					break;
				case 3:
					nRage = GetRage3Modification(rImp);
					break;
				case 4:
					nRage = GetRage4Modification(rImp);
					break;
				case 5:
					nRage = GetRage5Modification(rImp);
					break;
				default:
					break;
				}
				if( INVALID_ID != nRage )
				{
					if( iRefixType == 1 )
					{
						nRage += 100;
						nRage = Float2Int((nRage*rMe.GetBaseMaxRage())/100.0f);
					}
					nRage += iDamage + iRefixPara;
					rMe.RageIncrement(nRage, rImp.GetCasterLogicCount(), pImpactSender);
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact074_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += 100;
			SetHpModification(rImp, Float2Int((GetHpModification(rImp)*nRate)/100.0f));
			SetMpModification(rImp, Float2Int((GetMpModification(rImp)*nRate)/100.0f));
			SetRageModification(rImp, Float2Int((GetRageModification(rImp)*nRate)/100.0f));


			SetRage1Modification(rImp, Float2Int((GetRage1Modification(rImp)*nRate)/100.0f));
			SetRage2Modification(rImp, Float2Int((GetRage2Modification(rImp)*nRate)/100.0f));
			SetRage3Modification(rImp, Float2Int((GetRage3Modification(rImp)*nRate)/100.0f));
			SetRage4Modification(rImp, Float2Int((GetRage4Modification(rImp)*nRate)/100.0f));
			SetRage5Modification(rImp, Float2Int((GetRage5Modification(rImp)*nRate)/100.0f));

			__LEAVE_FUNCTION
			return TRUE;
		}
		INT StdImpact074_T::GetDamageRefix( Obj_Character* pImpactSender, OWN_IMPACT& rImp ) const
		{
			if( pImpactSender->GetObjType() != Obj::OBJ_TYPE_HUMAN )
			{
				return 0;
			}
			Obj_Human* pHuman = static_cast<Obj_Human*>(pImpactSender);
			INT iDamage = 0;
			if( pHuman->GetMenPai() < 2 )
			{
				iDamage = pHuman->GetAttackNear();
			}
			else if(  pHuman->GetMenPai() < 4 )
			{
				iDamage = pHuman->GetAttackFar();
			}
			else 
			{
				iDamage = pHuman->GetAttackMagicNear();
			}

			INT iRefixRate = GetRefixRate( rImp );
			iDamage = (INT)(iDamage * iRefixRate / 100.0f);

			return iDamage;
		}
		INT StdImpact074_T::GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const
		{
			FLOAT fRate = iRate / 1000.0f;
			return (INT)((iPara1*fPara2/1000.0f + iPara3*iPara4/1000.0f) * fRate);
		}

		INT StdImpact074_T::GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe ) const
		{
			INT iPara = GetParaID( rImp );
			INT iRate = GetRefixRateHorse( rImp );

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
	};
};

