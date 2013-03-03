///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact012.cpp
// 程序员：高骐
// 功能说明：效果--State: refix all attack, Defence and MaxHP, MaxMP, MaxRage, MaxStrikePoint
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact012.h"

namespace Combat_Module
{

	namespace Impact_Module
	{
		BOOL StdImpact012_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact012_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION

			// Attacks
			if(0!=GetAttackNearRefix(rImp))
			{
				rMe.MarkAttackNearRefixDirtyFlag();
			}
			if(0!=GetAttackFarRefix(rImp))
			{
				rMe.MarkAttackFarRefixDirtyFlag();
			}
			if(0!=GetAttackMagicNearRefix(rImp))
			{
				rMe.MarkAttackMagicNearRefixDirtyFlag();
			}
			if(0!=GetAttackMagicFarRefix(rImp))
			{
				rMe.MarkAttackMagicFarRefixDirtyFlag();
			}
			if(0!=GetAttackGoldRefix(rImp))
			{
				rMe.MarkAttackGoldRefixDirtyFlag();
			}
			if(0!=GetAttackWoodRefix(rImp))
			{
				rMe.MarkAttackWoodRefixDirtyFlag();
			}
			if(0!=GetAttackWaterRefix(rImp))
			{
				rMe.MarkAttackWaterRefixDirtyFlag();
			}
			if(0!=GetAttackFireRefix(rImp))
			{
				rMe.MarkAttackFireRefixDirtyFlag();
			}
			if(0!=GetAttackSoilRefix(rImp))
			{
				rMe.MarkAttackSoilRefixDirtyFlag();
			}
			// defences
			if(0!=GetDefenceNearRefix(rImp))
			{
				rMe.MarkDefenceNearRefixDirtyFlag();
			}
			if(0!=GetDefenceFarRefix(rImp))
			{
				rMe.MarkDefenceFarRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicNearRefix(rImp))
			{
				rMe.MarkDefenceMagicNearRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicFarRefix(rImp))
			{
				rMe.MarkDefenceMagicFarRefixDirtyFlag();
			}
			if(0!=GetResistGoldRefix(rImp))
			{
				rMe.MarkResistGoldRefixDirtyFlag();
			}
			if(0!=GetResistWoodRefix(rImp))
			{
				rMe.MarkResistWoodRefixDirtyFlag();
			}
			if(0!=GetResistWaterRefix(rImp))
			{
				rMe.MarkResistWaterRefixDirtyFlag();
			}
			if(0!=GetResistFireRefix(rImp))
			{
				rMe.MarkResistFireRefixDirtyFlag();
			}
			if(0!=GetResistSoilRefix(rImp))
			{
				rMe.MarkResistSoilRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact012_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			INT nRefixRate = GetRefixRate(rImp);
			nRefixRate += 100;
			INT nValue = 0;
			switch (nIdx)
			{
				//Attacks
				case CharIntAttrRefixs_T::REFIX_ATTACK_NEAR:
					{
						if(0!=GetAttackNearRefix(rImp))
						{
							nValue = Float2Int((GetAttackNearRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackNear()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_FAR:
					{
						if(0!=GetAttackFarRefix(rImp))
						{
							nValue = Float2Int((GetAttackFarRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackFar()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseAttackMagicNear()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR:
					{
						if(0!=GetAttackMagicFarRefix(rImp))
						{
							nValue = Float2Int((GetAttackMagicFarRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackMagicFar()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_GOLD:
					{
						if(0!=GetAttackGoldRefix(rImp))
						{
							nValue = Float2Int((GetAttackGoldRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackGold()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_WOOD:
					{
						if(0!=GetAttackWoodRefix(rImp))
						{
							nValue = Float2Int((GetAttackWoodRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackWood()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				case CharIntAttrRefixs_T::REFIX_ATTACK_WATER:
					{
						if(0!=GetAttackWaterRefix(rImp))
						{
							nValue = Float2Int((GetAttackWaterRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackWater()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;


				case CharIntAttrRefixs_T::REFIX_ATTACK_FIRE:
					{
						if(0!=GetAttackFireRefix(rImp))
						{
							nValue = Float2Int((GetAttackFireRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackFire()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				case CharIntAttrRefixs_T::REFIX_ATTACK_SOIL:
					{
						if(0!=GetAttackSoilRefix(rImp))
						{
							nValue = Float2Int((GetAttackSoilRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackSoil()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseDefenceNear()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_FAR:
					{
						if(0!=GetDefenceFarRefix(rImp))
						{
							nValue = Float2Int((GetDefenceFarRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefenceFar()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseDefenceMagicNear()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR:
					{
						if(0!=GetDefenceMagicFarRefix(rImp))
						{
							nValue = Float2Int((GetDefenceMagicFarRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefenceMagicFar()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_GOLD:
					{
						if(0!=GetResistGoldRefix(rImp))
						{
							nValue = Float2Int((GetResistGoldRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefenceGold()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_WOOD:
					{
						if(0!=GetResistWoodRefix(rImp))
						{
							nValue = Float2Int((GetResistWoodRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefenceWood()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				case CharIntAttrRefixs_T::REFIX_RESIST_WATER:
					{
						if(0!=GetResistWaterRefix(rImp))
						{
							nValue = Float2Int((GetResistWaterRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefenceWater()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				case CharIntAttrRefixs_T::REFIX_RESIST_FIRE:
					{
						if(0!=GetResistFireRefix(rImp))
						{
							nValue = Float2Int((GetResistFireRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefenceFire()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				case CharIntAttrRefixs_T::REFIX_RESIST_SOIL:
					{
						if(0!=GetResistSoilRefix(rImp))
						{
							nValue = Float2Int((GetResistSoilRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefenceSoil()*nValue)/100.0f);
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
		BOOL StdImpact012_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

