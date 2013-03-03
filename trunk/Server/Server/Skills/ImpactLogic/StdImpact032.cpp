///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact032.cpp
// 程序员：高骐
// 功能说明：效果--武当技能相濡以沫的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameTable.h"
#include "StdImpact032.h"
#include "Scene.h"


using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact032_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetSnareActivateCooldownElapsed(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		VOID StdImpact032_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			SkillTemplateData_T const* pData = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
			if(NULL!=pData)
			{
				if(0==pData->GetRangedSkillFlag())
				{
					if(GetSnareActivateCooldown(rImp)<=GetSnareActivateCooldownElapsed(rImp))
					{
						Scene * pScene = rMe.getScene();
						if(NULL!=pScene)
						{
							INT nRand = pScene->GetRand100();
							if(nRand < GetSnareActivateOdds(rImp))
							{
								if(NULL!=pAttacker)
								{
									SetSnareActivateCooldownElapsed(rImp, 0);
									g_ImpactCore.SendImpactToUnit(*pAttacker, GetSnareImpactDataIndex(rImp), rMe.GetID(), 500);
								}
							}
						}
					}
					else
					{
						Time_t nCooldownElapsed = GetSnareActivateCooldownElapsed(rImp);
						Time_t nDeltaTime = rMe.GetLogicTime();
						nCooldownElapsed += nDeltaTime;
						SetSnareActivateCooldownElapsed(rImp, nCooldownElapsed);
					}
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact032_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			//Defences
			INT nRefixRate = 0;
			nRefixRate += 100;
			INT nValue = 0;
			switch(nIdx)
			{
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
				case CharIntAttrRefixs_T::REFIX_RESIST_GOLD:
					{
						if(0!=GetResistGoldRefix(rImp))
						{
							nValue = Float2Int((GetResistGoldRefix(rImp)*nRefixRate)/100.0f);
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
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				default:
					break;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact032_T::MarkModifiedAttrDirty(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			// defences
			if(0!=GetDefenceNearRefix(rImp))
			{
				rMe.MarkDefenceNearRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicNearRefix(rImp))
			{
				rMe.MarkDefenceMagicNearRefixDirtyFlag();
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
		VOID StdImpact032_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj_Character* pTar=NULL;
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				AssertEx(FALSE, "[StdImpact032_T::OnFadeOut]: Empty scene pointer found!!");
				return;
			}
			OBJLIST Targets;
			if(FALSE==ScanUnitForTarget(rImp, rMe, *(rMe.getWorldPos()), (FLOAT)GetScanRadius(rImp), GetEffectedObjCount(rImp), Targets))
			{
				return;
			}
			INT nImpact = GetFreezeImpactDataIndex(rImp);
			if(INVALID_ID==nImpact)
			{
				return;
			}
			// impacts init finished
			for(INT nIdx=0; Targets.m_Count>nIdx;++nIdx)
			{
				pTar = (Obj_Character*)Targets.m_aObj[nIdx];
				if(NULL!=pTar)
				{
					pScene->GetEventCore().RegisterBeSkillEvent(pTar->GetID(), rMe.GetID(), BEHAVIOR_TYPE_HOSTILITY, 500);
					g_ImpactCore.SendImpactToUnit(*pTar, nImpact, rMe.GetID(), 500);
				}
			}
			return;
			__LEAVE_FUNCTION
		}
		BOOL StdImpact032_T::IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(FALSE==rTar.IsAliveInDeed() || FALSE==rTar.IsActiveObj())
			{
				return FALSE;
			}
			if(rMe.IsEnemy(&rTar) || rTar.IsEnemy(&rMe))
			{
				return TRUE;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

