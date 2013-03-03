///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact080.h
// 程序员：陈军
// 功能说明：效果--偷impact给自己
// 修改记录：
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameTable.h"
#include "StdImpact080.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact080_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact080_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}

		VOID StdImpact080_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			
			Obj_Character* pTar = GetBeHitTarget(rMe, rImp);
			if(!pTar || FALSE==pTar->IsAlive() || FALSE==pTar->IsActiveObj())
			{
				return;
			}

			//别自己偷自己
			if(rMe.GetID() == pTar->GetID())
			{
				return;
			}

			INT nRand = pScene->GetRand100();
			INT nDispeled = 0;
			INT iStealRate = GetRate( rImp );
			if( nRand > iStealRate )
			{
				return;
			}

			INT nStealFromType = GetStealFromType(rImp);

			if(IDX_STEAL_FROM_ENEMY == nStealFromType)
			{
				if(!rMe.IsEnemy(pTar))
				{
					return;
				}
			}
			else if(IDX_STEAL_FROM_FRIEND == nStealFromType)
			{
				if(!rMe.IsFriend(pTar))
				{
					return;
				}
			}
			else
			{
				Assert(FALSE);
				return;
			}
			INT nRate = GetRefixRate(rImp);
			nRate += 100;
			INT nStealCount = GetCount(rImp);
			nStealCount = Float2Int((nStealCount*nRate)/100.0f); //强化只影响驱散数目
			pTar->Impact_StealImpact(rMe, GetCollection(rImp), GetStandFlag(rImp), nStealCount, GetRefixRate(rImp));
			__LEAVE_FUNCTION
		}

		Obj_Character* StdImpact080_T::GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION

			Scene* pScene = rMe.getScene();
			if( pScene == NULL )
			{
				return NULL;
			}
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Obj_Character* pChar = NULL;
			pChar = (Obj_Character*)pScene->GetObjManager()->GetObj( rParams.GetTargetObj() );
			if(NULL == pChar)
			{
				pChar = (Obj_Character*)pScene->GetObjManager()->GetObj( rImp.GetCasterObjID());
			}
			return pChar;
			__LEAVE_FUNCTION
			return NULL;
		}	

	};
};

