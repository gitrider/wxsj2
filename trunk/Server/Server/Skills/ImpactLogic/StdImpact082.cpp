#include "stdafx.h"
#include "StdImpact082.h"
#include "Obj_Human.h"


using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact082_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			InitDamages(rImp);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}


		BOOL StdImpact082_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			__LEAVE_FUNCTION
			return TRUE;
		}

		VOID StdImpact082_T::InitDamages( OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION
			INT iRefixType = Impact_GetImpactDataDescriptorValueByIndex
							(
								rImp.GetDataIndex(), 
								ImpactDataDescriptorIndex_T::IDX_REFIX_TYPE,
								rImp.GetSkillLevel()
							);
				
			SetHP(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					ImpactDataDescriptorIndex_T::IDX_REFIX_HP,
					rImp.GetSkillLevel()
					)
				);
			SetMP(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					ImpactDataDescriptorIndex_T::IDX_REFIX_MP,
					rImp.GetSkillLevel()
					)
				);
			SetPneuma(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					ImpactDataDescriptorIndex_T::IDX_REFIX_PNEUMA,
					rImp.GetSkillLevel()
					)
				);
			SetRage(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					ImpactDataDescriptorIndex_T::IDX_REFIX_RAGE,
					rImp.GetSkillLevel()
					)
				);
			__LEAVE_FUNCTION
		}

		VOID StdImpact082_T::OnActive( OWN_IMPACT& rImp, Obj_Character& rMe ) const
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

			if(FALSE==pImpactSender->IsAlive() || FALSE==pImpactSender->IsActiveObj())
			{
				return;
			}
			
			INT iRefixType = Impact_GetImpactDataDescriptorValueByIndex
							(
								rImp.GetDataIndex(), 
								ImpactDataDescriptorIndex_T::IDX_REFIX_TYPE,
								rImp.GetSkillLevel()
							);

			INT iResult = 0;
			{
				INT iBase	= rMe.GetHP();
				INT iRefix	= GetHP( rImp );
				if( INVALID_PARAM_ID != iRefix )
				{			
					iResult	= GetRifixResult( rImp, rMe, *pImpactSender, iBase, iRefix, iRefixType);
					rMe.HealthIncrement( iResult, rImp.GetCasterLogicCount(), pImpactSender, FALSE, TRUE );
				}
			}

			{
				INT iBase	= rMe.GetMP();
				INT iRefix	= GetMP( rImp );
				if( INVALID_PARAM_ID != iRefix )
				{			
					iResult	= GetRifixResult( rImp, rMe, *pImpactSender, iBase, iRefix, iRefixType);
					rMe.ManaIncrement( iResult, rImp.GetCasterLogicCount(), pImpactSender);
				}
			}

			{
				INT iBase	= rMe.GetPneuma();
				INT iRefix	= GetPneuma( rImp );
				if( INVALID_PARAM_ID != iRefix )
				{			
					iResult	= GetRifixResult( rImp, rMe, *pImpactSender, iBase, iRefix, iRefixType);
					rMe.PneumaIncrement( iResult, rImp.GetCasterLogicCount(), pImpactSender);
				}
			}

			{
				INT iBase	= rMe.GetRage();
				INT iRefix	= GetRage( rImp );
				if( INVALID_PARAM_ID != iRefix )
				{			
					iResult	= GetRifixResult( rImp, rMe, *pImpactSender, iBase, iRefix, iRefixType);
					rMe.RageIncrement( iResult, rImp.GetCasterLogicCount(), pImpactSender);
				}
			}
			__LEAVE_FUNCTION
		}

		INT StdImpact082_T::GetRifixResult(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rSender, INT iBase, INT iRefix, INT iRefixType ) const
		{
			__ENTER_FUNCTION

			INT iRefixResult = 0;
			//按点数修正
			if(REFIX_TYPE_POINT == iRefixType)
			{
				iRefixResult = iRefix;
			}
			else if(REFIX_TYPE_RATE == iRefixType)
			{//按百分比修正
				iRefixResult = ((iBase*iRefix)/100);
			}
			else 
			{
				//其它的按点数修正
				iRefixResult = GetRefixParamValue(rImp, rMe, rSender, iRefixType, iRefix);
			}
			return iRefixResult;
			__LEAVE_FUNCTION
			return 0;
		}

		INT	StdImpact082_T::GetRefixParamValue(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rSender, INT iRefixType, INT nParamType) const
		{
			__ENTER_FUNCTION
				
			INT nParamValue = 0;

			Obj_Character * pChar = NULL;

			if(REFIX_TYPE_ME == iRefixType)
			{
				pChar = &rMe;
			}
			else if(REFIX_TYPE_SENDER == iRefixType)
			{
				pChar = &rSender;
			}

			if(NULL == pChar)
			{
				return 0;
			}

			int nSign = 1;
			
			if(nParamType < 0)
			{
				//处理减血逻辑
				nParamType = nParamType * (-1);
				nSign = -1;
			}

			switch(nParamType)
			{
				case REFIX_PARAM_TYPE_0:
					nParamValue = pChar->GetAttackGold();
					break;
				case REFIX_PARAM_TYPE_1:
					nParamValue = pChar->GetAttackWood();
					break;
				case REFIX_PARAM_TYPE_2:
					nParamValue = pChar->GetAttackWater();
					break;
				case REFIX_PARAM_TYPE_3:
					nParamValue = pChar->GetAttackFire();
					break;
				case REFIX_PARAM_TYPE_4:
					nParamValue = pChar->GetAttackSoil();
					break;

				case REFIX_PARAM_TYPE_5:
					nParamValue = pChar->GetAttackNear();
					break;
				case REFIX_PARAM_TYPE_6:
					nParamValue = pChar->GetAttackFar();
					break;
				case REFIX_PARAM_TYPE_7:
					nParamValue = pChar->GetAttackMagicNear();
					break;
				case REFIX_PARAM_TYPE_8:
					nParamValue = pChar->GetAttackMagicFar();
					break;

				default:
					return 0;
					break;
			}

			nParamValue = nParamValue * nSign;

			return nParamValue;
			
			__LEAVE_FUNCTION
			return 0;
		}
	};
};

