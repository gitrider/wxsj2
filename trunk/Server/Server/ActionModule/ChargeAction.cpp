///////////////////////////////////////////////////////////////////////////////
// 文件名：ChargeAction.cpp
// 程序员：高骐
// 功能说明：聚气动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Type.h"
#include "ChargeAction.h"
#include "ActionCallBack.h"
#include "ActionDelegator.h"
#include "Obj_Character.h"
namespace Action_Module
{
	using namespace Action_Module;
	//class ChargeActionLogic_T : ActionLogic_T
	BOOL ChargeActionLogic_T::HeartBeat(ActionParams_T& rParams, Time_t nDeltaTime) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL==pCallBack)
		{
			return FALSE;
		}
		if(FALSE==pCallBack->CanDoThisActionInThisStatus(rParams))
		{
			return FALSE;
		}
		Time_t nContinuance = rParams.GetContinuance() - nDeltaTime;
		BOOL bRet = TRUE;
		if(0>=nContinuance)
		{
			nContinuance = 0;
		}
		rParams.SetContinuance(nContinuance);
		if(0==nContinuance)
		{
			rParams.GetActor()->StopCharacterLogic();
			pCallBack->OnActivateOnce(rParams);
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ChargeActionLogic_T::OnDisturb(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL!=pCallBack)
		{
			if(TRUE == pCallBack->InterruptWhenDisturbed(rParams))
			{
				return FALSE;
			}
			return pCallBack->OnDisturbForCharging(rParams);
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ChargeActionLogic_T::OnInterrupt(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL!=pCallBack)
		{
			return pCallBack->OnInterrupt(rParams);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
}

