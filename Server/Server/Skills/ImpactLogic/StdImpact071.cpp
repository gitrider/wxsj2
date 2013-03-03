/********************************************************************
	created:	2008/05/09
	created:	9:5:2008   14:36
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact071.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact071
	file ext:	cpp
	author:		Richard
	
	purpose:	HOT
*********************************************************************/

#include "stdafx.h"
#include "StdImpact071.h"
#include "Obj_Human.h"


using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact071_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}


		BOOL StdImpact071_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			__LEAVE_FUNCTION
			return TRUE;
		}

		VOID StdImpact071_T::InitDamages( OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION
				SetHP
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					ImpactDataDescriptorIndex_T::IDX_REFIX_HP,
					rImp.GetSkillLevel()
					)
				);
			SetRage0
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE0,
					rImp.GetSkillLevel()
					)
				);
			SetRage1
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE1,
				rImp.GetSkillLevel()
				)
				);
			SetRage2
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE2,
				rImp.GetSkillLevel()
				)
				);
			SetRage3
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE3,
				rImp.GetSkillLevel()
				)
				);
			SetRage4
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE4,
				rImp.GetSkillLevel()
				)
				);
			SetRage5
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE5,
				rImp.GetSkillLevel()
				)
				);
			__LEAVE_FUNCTION
		}



		VOID StdImpact071_T::OnIntervalOver( OWN_IMPACT& rImp, Obj_Character& rMe ) const
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

			INT iRefixType = Impact_GetImpactDataDescriptorValueByIndex
							(
								rImp.GetDataIndex(), 
								ImpactDataDescriptorIndex_T::IDX_REFIX_TYPE,
								rImp.GetSkillLevel()
							);


			InitDamages( rImp );
			
			if( rMe.GetObjType() != Obj::OBJ_TYPE_HUMAN )
			{
				return;
			}
			INT iDamage = GetDamageRefix( pImpactSender, rImp );
			Obj_Human& rHuman = static_cast<Obj_Human&>(rMe);
			INT iRefixParaHorse = GetRefixPara( rImp, rMe );

			INT iBase		= rMe.GetRage();
			INT iRefix		= GetRageByZhiYe( rImp, rHuman.GetMenPai() );
			INT iResult		= GetRifixResult( iBase, iRefix, iRefixType );
			if( INVALID_ID != iRefix )
			{
				iResult += iDamage;
				//增加马修正
				iResult += iRefixParaHorse;
				rMe.RageIncrement( iResult, rImp.GetCasterLogicCount() );
			}
			
			iBase	= rMe.GetHP();
			iRefix	= GetHP( rImp );
			iResult	= GetRifixResult( iBase, iRefix, iRefixType );
			iResult += iDamage;
			//增加马修正
			iResult += iRefixParaHorse;
			rMe.HealthIncrement( iResult, rImp.GetCasterLogicCount(), NULL, FALSE, TRUE );
			__LEAVE_FUNCTION
		}

		INT StdImpact071_T::GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const
		{
			__ENTER_FUNCTION

			INT iRefixResult = 0;
			//按点数修正
			if( iRefixType <= 0 )
			{
				iRefixResult = iRefix;
			}
			else
			{//按百分比修正
				iRefixResult = ((iBase*iRefix)/100);
			}
			return iRefixResult;
			__LEAVE_FUNCTION
			return 0;
		}

		INT StdImpact071_T::GetRageByZhiYe( OWN_IMPACT& rImp, INT iZhiYe ) const
		{
			switch(iZhiYe)
			{
			case 0:
				return GetRage0( rImp );
				break;
			case 1:
				return GetRage1( rImp );
				break;
			case 2:
				return GetRage2( rImp );
			    break;
			case 3:
				return GetRage3( rImp );
			    break;
			case 4:
				return GetRage4( rImp );
				break;
			case 5:
				return GetRage5( rImp );
				break;
			default:
			    break;
			}
			return 0;
		}
		INT StdImpact071_T::GetDamageRefix( Obj_Character * pImpactSender, OWN_IMPACT& rImp ) const
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
		INT StdImpact071_T::GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const
		{
			FLOAT fRate = iRate / 1000.0f;
			return (INT)((iPara1*fPara2/1000.0f + iPara3*iPara4/1000.0f) * fRate);
		}

		INT StdImpact071_T::GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe ) const
		{
			INT iPara = GetParaID( rImp );
			INT iRate = GetRefixRateHorse( rImp );
			Obj_Human& rHuman = static_cast<Obj_Human&>(rMe);
			PET_GUID_t guidHorse = rHuman.GetGUIDOfCallUpHorse();

			if( guidHorse.IsNull() )
			{
				return 0;
			}

			Item* pPetItem = rHuman.GetHorseItem(guidHorse);
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

