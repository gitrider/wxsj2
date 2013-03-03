///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact077.cpp
// 程序员：
// 功能说明：效果--在一定时间内修正、命中、闪避、生命上限、物理和魔法攻防
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact077.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact077_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact077_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetHitRefix(rImp))
			{
				rMe.MarkHitRefixDirtyFlag();
			}
			if(0!=GetMissRefix(rImp))
			{
				rMe.MarkMissRefixDirtyFlag();
			}
			if(0!=GetCriticalRefix(rImp))
			{
				rMe.MarkCriticalRefixDirtyFlag();
			}
			if(0!=GetMaxHpRefix(rImp))
			{
				rMe.MarkMaxHPRefixDirtyFlag();
			}
			if(0!=GetMaxMpRefix(rImp))
			{
				rMe.MarkMaxMPRefixDirtyFlag();
			}
			if(0!=GetAttackNearRefix(rImp))
			{
				rMe.MarkAttackNearRefixDirtyFlag();
			}
			if(0!=GetDefenceNearRefix(rImp))
			{
				rMe.MarkDefenceNearRefixDirtyFlag();
			}
			if(0!=GetAttackMagicNearRefix(rImp))
			{
				rMe.MarkAttackMagicNearRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicNearRefix(rImp))
			{
				rMe.MarkDefenceMagicNearRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact077_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			INT nRefixRate = GetRefixRate(rImp);
			nRefixRate += 100;
			INT nValue = 0;
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_HIT:
					{
						if(0!=GetHitRefix(rImp))
						{
							nValue = Float2Int((GetHitRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MISS:
					{
						if(0!=GetMissRefix(rImp))
						{
							nValue = Float2Int((GetMissRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_CRITICAL:
					{
						if(0!=GetCriticalRefix(rImp))
						{
							nValue = Float2Int((GetCriticalRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;			
				// Maxes
				case CharIntAttrRefixs_T::REFIX_MAX_HP:
					{
						if(0!=GetMaxHpRefix(rImp))
						{
							nValue = Float2Int((GetMaxHpRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MAX_MP:
					{
						if(0!=GetMaxMpRefix(rImp))
						{
							nValue = Float2Int((GetMaxMpRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
					
				//Attacks
				case CharIntAttrRefixs_T::REFIX_ATTACK_NEAR:
					{
						if(0!=GetAttackNearRefix(rImp))
						{
							nValue = Float2Int((GetAttackNearRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR:
					{
						if(0!=GetAttackMagicNearRefix(rImp))
						{
							nValue = Float2Int((GetAttackMagicNearRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				//Defences
				case CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR:
					{
						if(0!=GetDefenceNearRefix(rImp))
						{
							nValue = Float2Int((GetDefenceNearRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR:
					{
						if(0!=GetDefenceMagicNearRefix(rImp))
						{
							nValue = Float2Int((GetDefenceMagicNearRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact077_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

