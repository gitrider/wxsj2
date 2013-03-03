// 文件名：StdImpact078.h
// 程序员：陈军
// 功能说明：效果--在一定时间间隔中，扫描玩家周围的obj，并给每个玩家触发impact
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact078.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact078_T::OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION

			if(FALSE==rMe.IsAliveInDeed() || FALSE==rMe.IsActiveObj())
			{
				return;
			}

			Obj_Character* pTar=NULL;
			OBJLIST Targets;
			if(FALSE==ScanUnitForTarget(rImp, rMe, *(rMe.getWorldPos()), (FLOAT)GetScanRadius(rImp), GetEffectedObjCount(rImp), Targets))
			{
				return;
			}
			for(INT nIdx=0; Targets.m_Count>nIdx;++nIdx)
			{
				pTar = (Obj_Character*)Targets.m_aObj[nIdx];
				if(NULL!=pTar)
				{
					if(FALSE==pTar->IsAliveInDeed() || FALSE==pTar->IsActiveObj())
					{
						continue;
					}
					g_ImpactCore.SendImpactToUnit(*pTar, GetSubImpactDataIndex(rImp), rMe.GetID(), 0);
				}
			}
			return;
			__LEAVE_FUNCTION
		}
		BOOL StdImpact078_T::IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(FALSE==rTar.IsAliveInDeed() || FALSE==rTar.IsActiveObj())
			{
				return FALSE;
			}

			if(FALSE==rTar.IsAliveInDeed() || FALSE==rTar.IsActiveObj())
			{
				return FALSE;
			}

			if(ENUM_SCAN_OBJ_TYPE_ENEMY == GetScanObjType(rImp))
			{
				if(rMe.IsEnemy(&rTar) || rTar.IsEnemy(&rMe))
				{
					return TRUE;
				}
			}
			else if(ENUM_SCAN_OBJ_TYPE_TEAMMATE == GetScanObjType(rImp))
			{
				//如果是朋友关系,就不能是怪物或NPC
				if(rMe.IsFriend(&rTar))
				{
					if(Obj::OBJ_TYPE_MONSTER == rTar.GetObjType())
					{
						Obj_Monster & rMonster = (Obj_Monster &)rTar;

						//不能战斗,认为是NPC
						if(1 == rMonster.CannotFight())
						{
							return FALSE;
						}
						else
						{
							return TRUE;
						}
					}
					else
					{
						return TRUE;
					}
				}
			}
			else if(ENUM_SCAN_OBJ_TYPE_TEAMMATE_OR_ENEMY == GetScanObjType(rImp))
			{
				//如果是敌人 + 朋友的模式
				if(((rMe.IsEnemy(&rTar) || rTar.IsEnemy(&rMe))) || rMe.IsFriend(&rTar))
				{
					if(Obj::OBJ_TYPE_MONSTER == rTar.GetObjType())
					{
						Obj_Monster & rMonster = (Obj_Monster &)rTar;

						//不能战斗,认为是NPC
						if(1 == rMonster.CannotFight())
						{
							return FALSE;
						}
						else
						{
							return TRUE;
						}
					}
					else
					{
						return TRUE;
					}
				}
			}
			
			return FALSE;

			__LEAVE_FUNCTION
			return FALSE;
		}

	};
};

