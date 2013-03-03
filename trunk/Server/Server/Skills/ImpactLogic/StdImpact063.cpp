/********************************************************************
	created:	2008/05/05
	created:	5:5:2008   14:50
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact063.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact063
	file ext:	cpp
	author:		Richard
	
	purpose:	DOT
*********************************************************************/


#include "stdafx.h"
#include "StdImpact063.h"
#include "Obj_Human.h"
#include "Scene.h"


using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact063_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION

			InitDamages( rImp );

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		VOID StdImpact063_T::InitDamages( OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION
				SetDamageType
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_DAMAGE_TYPE,
				rImp.GetSkillLevel()
				)
				);
			SetDamagePara
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_PARA,
				rImp.GetSkillLevel()
				)
				);
			SetAttackWeight
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_ATTACK_WEIGHT,
				rImp.GetSkillLevel()
				)
				);
			SetDefenceWeight
				(rImp, 
				Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_DEFENCE_WEIGHT,
				rImp.GetSkillLevel()
				)
				);
			__LEAVE_FUNCTION
		}



		VOID StdImpact063_T::OnIntervalOver( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}

			INT iRefixType = Impact_GetImpactDataDescriptorValueByIndex
				(
				rImp.GetDataIndex(), 
				ImpactDataDescriptorIndex_T::IDX_REFIX_TYPE,
				rImp.GetSkillLevel()
				);


			INT iAttackerID = rImp.GetCasterObjID();
			Scene* pScene = rMe.getScene();
			if( pScene == NULL )
			{
				return;
			}
			Obj_Character* pAttacker = (Obj_Character*)(pScene->GetObjManager()->GetObj( iAttackerID ));
			if( pAttacker == NULL )
			{
				return;
			}

			if(FALSE==pAttacker->IsAlive() || FALSE==pAttacker->IsActiveObj())
			{
				return;
			}


			INT iDamageType		= GetDamageType( rImp );
			INT iDamagePara		= GetDamagePara( rImp );
			INT iAttackWeight	= GetAttackWeight( rImp );
			INT iDefenceWeight	= GetDefenceWeight( rImp );

			//按伤害类型初始化vDamage
			INT vDamage[DAMAGE_TYPE_NUMBER];

			vDamage[DAMAGE_TYPE_NEAR]	= pAttacker->GetAttackNear();
			vDamage[DAMAGE_TYPE_FAR]	= pAttacker->GetAttackFar();
			vDamage[DAMAGE_TYPE_MAGIC_NEAR]	= pAttacker->GetAttackMagicNear();
			vDamage[DAMAGE_TYPE_MAGIC_FAR]	= pAttacker->GetAttackMagicFar();
			vDamage[DAMAGE_TYPE_GOLD]	= pAttacker->GetAttackGold();
			vDamage[DAMAGE_TYPE_WOOD]	= pAttacker->GetAttackWood();
			vDamage[DAMAGE_TYPE_WATER]	= pAttacker->GetAttackWater();
			vDamage[DAMAGE_TYPE_FIRE]	= pAttacker->GetAttackFire();
			vDamage[DAMAGE_TYPE_SOIL]	= pAttacker->GetAttackSoil();
						
			INT vDefence[DAMAGE_TYPE_NUMBER];
			vDefence[DAMAGE_TYPE_NEAR]		= rMe.GetDefenceNear();
			vDefence[DAMAGE_TYPE_FAR]		= rMe.GetDefenceFar();
			vDefence[DAMAGE_TYPE_MAGIC_NEAR]= rMe.GetDefenceMagicNear();
			vDefence[DAMAGE_TYPE_MAGIC_FAR]	= rMe.GetDefenceMagicFar();
			vDefence[DAMAGE_TYPE_GOLD]		= rMe.GetDefenceGold();
			vDefence[DAMAGE_TYPE_WOOD]		= rMe.GetDefenceWood();
			vDefence[DAMAGE_TYPE_WATER]		= rMe.GetDefenceWater();
			vDefence[DAMAGE_TYPE_FIRE]		= rMe.GetDefenceFire();
			vDefence[DAMAGE_TYPE_SOIL]		= rMe.GetDefenceSoil();
			
			INT vReduce[DAMAGE_TYPE_NUMBER];
			memset( vReduce, 0, sizeof(vReduce) );
			vReduce[DAMAGE_TYPE_NEAR]		= rMe.GetNearAttReduce();
			vReduce[DAMAGE_TYPE_FAR]		= rMe.GetFarAttReduce();
			vReduce[DAMAGE_TYPE_MAGIC_NEAR]		= rMe.GetMagicNearAttReduce();
			vReduce[DAMAGE_TYPE_MAGIC_FAR]		= rMe.GetMagicFarAttReduce();
			
			INT iBase  = rMe.GetHP();
			INT iRefix = (iDamagePara + (vDamage[iDamageType]*iAttackWeight/100 ) - vDefence[iDamageType]*iDefenceWeight/100);
			iRefix = (iRefix * ( 100 - vReduce[iDamageType] )) / 100;
			INT iResult = GetRifixResult( iBase, iRefix, iRefixType );
			iResult = iResult<0 ? 0:iResult;
			rMe.HealthIncrement( -iResult, rImp.GetCasterLogicCount(), pAttacker, FALSE, TRUE );

			__LEAVE_FUNCTION
		}

		INT StdImpact063_T::GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const
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

	};
};

