/********************************************************************
	created:	2008/05/04
	created:	4:5:2008   19:21
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact061.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact061
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "StdImpact061.h"

using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact061_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact061_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			INT nDeltaHP = rMe.GetHP();
			//Calculate Total damage for char
			INT nDamageNear		=	GetDamageNear(rImp);
			INT nDamageFar		=	GetDamageFar(rImp);
			INT nDamageMagicNear=	GetDamageMagicNear(rImp);
			INT nDamageMagicFar	=	GetDamageMagicFar(rImp);
			INT nDamageGold		=	GetDamageGold(rImp);
			INT nDamageWood		=	GetDamageWood(rImp);
			INT nDamageWater	=	GetDamageWater(rImp);
			INT nDamageFire		=	GetDamageFire(rImp);
			INT nDamageSoil		=	GetDamageSoil(rImp);
			InitDamages( rImp );
			//按伤害类型初始化vDamage
			INT vDamage[DAMAGE_TYPE_NUMBER];
			vDamage[DAMAGE_TYPE_NEAR]		=nDamageNear		*	(GetDamageNear(rImp)		+100) / 100;
			vDamage[DAMAGE_TYPE_FAR]		=nDamageFar			*	(GetDamageFar(rImp)			+100) / 100;
			vDamage[DAMAGE_TYPE_MAGIC_NEAR] =nDamageMagicNear	*	(GetDamageMagicNear(rImp)	+100) / 100;
			vDamage[DAMAGE_TYPE_MAGIC_FAR]	=nDamageMagicFar	*	(GetDamageMagicFar(rImp)	+100) / 100;

			vDamage[DAMAGE_TYPE_GOLD]		=nDamageGold		*	(GetDamageGold(rImp)		+100) / 100;
			vDamage[DAMAGE_TYPE_WOOD]		=nDamageWood		*	(GetDamageWood(rImp)		+100) / 100;

			vDamage[DAMAGE_TYPE_WATER]		=nDamageWater		*	(GetDamageWater(rImp)		+100) / 100;
			vDamage[DAMAGE_TYPE_FIRE]		=nDamageFire		*	(GetDamageFire(rImp)		+100) / 100;
			vDamage[DAMAGE_TYPE_SOIL]		=nDamageSoil		*	(GetDamageSoil(rImp)		+100) / 100;

			//产生伤害
			rMe.OnDamages(vDamage, rImp.GetCasterObjID(), rImp.IsCriticalHit(), rImp.GetSkillID(), rImp.GetCasterLogicCount());
			__LEAVE_FUNCTION
		}
		VOID StdImpact061_T::CriticalRefix(OWN_IMPACT& rImp) const
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
		BOOL StdImpact061_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
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

		VOID StdImpact061_T::InitDamages( OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION

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
	};
};
