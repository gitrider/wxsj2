///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact011.cpp
// 程序员：高骐
// 功能说明：效果--State: refix all attack, Defence and MaxHP, MaxMP, MaxRage, MaxStrikePoint
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact067.h"
#include "GameDefine2.h"
#include "Obj_Human.h"

namespace Combat_Module
{

	namespace Impact_Module
	{
		BOOL StdImpact067_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
		
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact067_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			for( INT i=0; i<MAX_REFIX; ++i )
			{
				INT iType = GetRefixType( rImp, i );
				if( INVALID_ID == iType )
				{
					break;
				}
				MarkDirtyFlag(rImp, rMe, iType);
			}
			__LEAVE_FUNCTION
		}
		
		INT StdImpact067_T::FindIndex( OWN_IMPACT & rImp, Obj_Character& rMe, INT iType ) const
		{
			__ENTER_FUNCTION
			INT iResult = INVALID_ID;
			for( INT i=0; i<MAX_REFIX; ++i )
			{
				if( INVALID_ID == iType )
				{
					break;
				}
				if( iType == GetRefixType( rImp, i ) )
				{
					iResult = i;
					break;
				}
			}
			return iResult;
			__LEAVE_FUNCTION
			return INVALID_ID;
		}

		BOOL StdImpact067_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION

			switch (nIdx)
			{
				//Attacks
			case CharIntAttrRefixs_T::REFIX_STR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_STR);

					rIntAttrRefix += iResult;
					return TRUE;
				}

				break;
			case CharIntAttrRefixs_T::REFIX_CON:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_CON);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_INT:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_INT);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_DEX:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEX);

					rIntAttrRefix += iResult;
					return TRUE;
				}

				break;
			case CharIntAttrRefixs_T::REFIX_HP_REGENERATE:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_RESTORE_HP);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_MAX_HP:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_MAX_HP);

					rIntAttrRefix += iResult;
					return TRUE;
				}
			break;
			case CharIntAttrRefixs_T::REFIX_HIT:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_HIT);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_MISS:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_MISS);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_CRITICAL:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_CRIT_RATE);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_MOVE_SPEED:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_SPEED);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;		
			case CharIntAttrRefixs_T::REFIX_ATTACK_SPEED:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_SPEED);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;			
			case CharIntAttrRefixs_T::REFIX_MAX_RAGE:
				{					
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ANGER);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;	
			case CharIntAttrRefixs_T::REFIX_ATTACK_NEAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_NEAR);
					
					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_ATTACK_FAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_FAR);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_MAGIC_NEAR);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_FAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_MAGIC_FAR);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_ATTACK_GOLD:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_GOLD);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_ATTACK_WOOD:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_WOOD);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			case CharIntAttrRefixs_T::REFIX_ATTACK_WATER:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_WATER);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			case CharIntAttrRefixs_T::REFIX_ATTACK_FIRE:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_FIRE);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			case CharIntAttrRefixs_T::REFIX_ATTACK_SOIL:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_ATTACK_SOIL);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			//Defences
			case CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_NEAR);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_DEFENCE_FAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_FAR);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_MAGIC_NEAR);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_FAR:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_MAGIC_FAR);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_RESIST_GOLD:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_GOLD);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;
			case CharIntAttrRefixs_T::REFIX_RESIST_WOOD:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_WOOD);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			case CharIntAttrRefixs_T::REFIX_RESIST_WATER:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_WATER);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			case CharIntAttrRefixs_T::REFIX_RESIST_FIRE:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_FIRE);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			case CharIntAttrRefixs_T::REFIX_RESIST_SOIL:
				{
					INT iResult = GetRefixResult(rImp, rMe, CHAR_ATT_DEFENCE_SOIL);

					rIntAttrRefix += iResult;
					return TRUE;
				}
				break;

			default:
				break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact067_T::MarkDirtyFlag( OWN_IMPACT & rImp, Obj_Character &rMe, INT iType ) const
		{
			__ENTER_FUNCTION

			switch( iType )
			{
			case CHAR_ATT_INVALID:
				break;
			case CHAR_ATT_STR:
				rMe.MarkStrRefixDirtyFlag();
				break;
			case CHAR_ATT_CON:
				rMe.MarkConRefixDirtyFlag();
				break;
			case CHAR_ATT_INT:
				rMe.MarkIntRefixDirtyFlag();
				break;
			case CHAR_ATT_DEX:
				rMe.MarkDexRefixDirtyFlag();
				break;
			case CHAR_ATT_HP:
				rMe.MarkMaxHPRefixDirtyFlag();
				break;
			case CHAR_ATT_MAX_HP:
				rMe.MarkMaxHPRefixDirtyFlag();
				break;
			case CHAR_ATT_RESTORE_HP:
				rMe.MarkHPRegenerateRefixDirtyFlag();
				break;
			case CHAR_ATT_HIT:
				rMe.MarkHitRefixDirtyFlag();
				break;
			case CHAR_ATT_MISS:
				rMe.MarkMissRefixDirtyFlag();
				break;
			case CHAR_ATT_CRIT_RATE:
				rMe.MarkCriticalRefixDirtyFlag();
				break;
			case CHAR_ATT_SPEED:
				rMe.MarkMoveSpeedRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_SPEED:
				rMe.MarkAttackSpeedRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_NEAR:
				rMe.MarkAttackNearRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_NEAR:
				rMe.MarkDefenceNearRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_FAR:
				rMe.MarkAttackFarRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_FAR:
				rMe.MarkDefenceFarRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_MAGIC_NEAR:
				rMe.MarkAttackMagicNearRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_MAGIC_FAR:
				rMe.MarkAttackMagicFarRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_MAGIC_NEAR:
				rMe.MarkDefenceMagicNearRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_MAGIC_FAR:
				rMe.MarkDefenceMagicFarRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_GOLD:
				rMe.MarkAttackGoldRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_GOLD:
				rMe.MarkResistGoldRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_WOOD:
				rMe.MarkAttackWoodRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_WOOD:
				rMe.MarkResistWoodRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_WATER:
				rMe.MarkAttackWaterRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_WATER:
				rMe.MarkResistWaterRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_FIRE:
				rMe.MarkAttackFireRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_FIRE:
				rMe.MarkResistFireRefixDirtyFlag();
				break;
			case CHAR_ATT_ATTACK_SOIL:
				rMe.MarkAttackSoilRefixDirtyFlag();
				break;
			case CHAR_ATT_DEFENCE_SOIL:
				rMe.MarkResistSoilRefixDirtyFlag();
				break;
			case CHAR_ATT_ANGER:
				rMe.MarkMaxRageRefixDirtyFlag();
				break;
			default:
				break;
			}
			__LEAVE_FUNCTION
		}
		INT StdImpact067_T::GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const
		{
			FLOAT fRate = iRate / 1000.0f;
			return (INT)((iPara1*fPara2/1000.0f + iPara3*iPara4/1000.0f) * fRate);
		}

		INT StdImpact067_T::GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe, INT iIndex ) const
		{
			INT iPara = GetRefixParaID( rImp, iIndex );
			INT iRate = GetRefixRate( rImp, iIndex );
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

		INT StdImpact067_T::GetRefixResult( OWN_IMPACT & rImp, Obj_Character& rMe, INT iType ) const
		{
			INT iIndex = FindIndex( rImp, rMe, iType );
			if( INVALID_ID == iIndex )
			{
				return 0;
			}
			return GetRefixPara(rImp, rMe, iIndex );
		}
	};
};



