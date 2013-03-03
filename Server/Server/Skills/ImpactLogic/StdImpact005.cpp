///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact005.cpp
// 程序员：高骐
// 功能说明：效果--生命、魔法、怒气、连击点的百分率直接修改
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact005.h"
#include "Obj_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact005_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
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
		VOID StdImpact005_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
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
			INT iDamage = GetDamageRefix( pImpactSender, rImp );
			//process HP
			INT nHP = GetHpModification(rImp);
			if( INVALID_ID != nHP )
			{
				nHP = Float2Int((nHP*rMe.GetBaseMaxHP())/100.0f);
				nHP += iDamage;
				rMe.HealthIncrement(nHP, rImp.GetCasterLogicCount(), pImpactSender);
			}

			if(NULL==pImpactSender)
			{
				pImpactSender->OnHealTarget(rImp.GetSkillID(), rMe, nHP);
			}
			// 只有Human才会用到这段代码 --->FrankWu
			if (rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN)
			{
				//process MP
				//INT nMP = GetMpModification(rImp);
				//nMP = Float2Int((nMP*rMe.GetBaseMaxMP())/100.0f);
				//rMe.ManaIncrement(nMP, pImpactSender);

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
					nRage = Float2Int((nRage*rMe.GetBaseMaxRage())/100.0f);
					nRage += iDamage;
					rMe.RageIncrement(nRage, rImp.GetCasterLogicCount(), pImpactSender);
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact005_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
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
		INT StdImpact005_T::GetDamageRefix( Obj_Character * pImpactSender, OWN_IMPACT& rImp ) const
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
	};
};

