///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact003.cpp
// 程序员：高骐
// 功能说明：效果--分类型的一次性伤害
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact003.h"

using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact003_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
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

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact003_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			INT nDeltaHP = rMe.GetHP();
#ifdef _DEBUG
			//Calculate Total damage for display
			INT nDamage	=	GetDamageNear(rImp);
				nDamage	+=	GetDamageFar(rImp);
				nDamage	+=	GetDamageMagicNear(rImp);
				nDamage	+=	GetDamageMagicFar(rImp);
				nDamage	+=	GetDamageGold(rImp);
				nDamage	+=	GetDamageWood(rImp);
				nDamage	+=	GetDamageWater(rImp);
				nDamage	+=	GetDamageFire(rImp);
				nDamage	+=	GetDamageSoil(rImp);
#endif
			//按伤害类型初始化vDamage
			INT vDamage[DAMAGE_TYPE_NUMBER];
			vDamage[DAMAGE_TYPE_NEAR]=GetDamageNear(rImp);
			vDamage[DAMAGE_TYPE_FAR]=GetDamageFar(rImp);
			vDamage[DAMAGE_TYPE_MAGIC_NEAR]=GetDamageMagicNear(rImp);
			vDamage[DAMAGE_TYPE_MAGIC_FAR]=GetDamageMagicFar(rImp);
			vDamage[DAMAGE_TYPE_GOLD]=GetDamageGold(rImp);
			vDamage[DAMAGE_TYPE_WOOD]=GetDamageWood(rImp);
			vDamage[DAMAGE_TYPE_WATER]=GetDamageWater(rImp);
			vDamage[DAMAGE_TYPE_FIRE]=GetDamageFire(rImp);
			vDamage[DAMAGE_TYPE_SOIL]=GetDamageSoil(rImp);
			//产生伤害
			rMe.OnDamages(vDamage, rImp.GetCasterObjID(), rImp.IsCriticalHit(), rImp.GetSkillID(), rImp.GetCasterLogicCount());
			__LEAVE_FUNCTION
		}
		VOID StdImpact003_T::CriticalRefix(OWN_IMPACT& rImp) const
		{
			__ENTER_FUNCTION
			SetDamageNear(rImp, GetDamageNear(rImp));
			SetDamageFar(rImp, GetDamageFar(rImp));
			SetDamageMagicNear(rImp, GetDamageMagicNear(rImp));
			SetDamageMagicFar(rImp, GetDamageMagicFar(rImp));
			SetDamageGold(rImp, GetDamageGold(rImp));
			SetDamageWood(rImp, GetDamageWood(rImp));
			SetDamageWater(rImp, GetDamageWater(rImp));
			SetDamageFire(rImp, GetDamageFire(rImp));
			SetDamageSoil(rImp, GetDamageSoil(rImp));
			rImp.MarkCriticalFlag();
			__LEAVE_FUNCTION
		}
		BOOL StdImpact003_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
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
	};
};
