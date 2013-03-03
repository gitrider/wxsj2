///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact079.h
// 程序员：陈军
// 功能说明：效果--驱除相同影响性质的效果
// 修改记录：
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameTable.h"
#include "StdImpact079.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact079_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact079_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}

		VOID StdImpact079_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
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


			INT nRand = pScene->GetRand100();
			INT nRate = GetRefixRate(rImp);
			INT nDispeled = 0;
			nRate += 100;
			if(rMe.IsEnemy(pTar))
			{
				INT iEnemyRate = GetEnemyRate( rImp );
				if( nRand > iEnemyRate )
				{
					return;
				}
				INT nDispelCount = GetEnemyDispelCount(rImp);
				nDispelCount = Float2Int((nDispelCount*nRate)/100.0f); //强化只影响驱散数目
				if(nDispelCount > 0)
				{
					rMe.Impact_DispelImpactOfSpecificStand(GetEnemyStandFlag(rImp), nDispelCount);
				}
			}
			else if(rMe.IsFriend(pTar))
			{
				INT iFriendRate = GetFriendRate( rImp );
				if( nRand > iFriendRate )
				{
					return;
				}

				INT nDispelCount = GetFriendDispelCount(rImp);
				nDispelCount = Float2Int((nDispelCount*nRate)/100.0f); //强化只影响驱散数目
				if(nDispelCount > 0)
				{
					rMe.Impact_DispelImpactOfSpecificStand(GetFriendStandFlag(rImp), nDispelCount);
				}
			}
			__LEAVE_FUNCTION
		}

		Obj_Character* StdImpact079_T::GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const
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

