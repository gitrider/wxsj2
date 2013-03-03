///////////////////////////////////////////////////////////////////////////////
// 文件名：ChannelAction.cpp
// 程序员：高骐
// 功能说明：引导动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Type.h"
#include "ChannelAction.h"
#include "Obj_Character.h"
#include "SkillLogic.h"
#include "SkillInfo.h"

#include "ActionDelegator.h"
#include "ActionCallBackForSkill.h"
#include "Scene.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;

namespace Action_Module
{	
	//class ActionCallBackFunctorForSkill_T : public ActionCallBackFunctor_T
	BOOL ActionCallBackFunctorForSkill_T::IsPerformingSpecificSkill(ActionParams_T& rParams, SkillID_t nID) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
			if(nID == rSkillInfo.GetSkillID())
			{
				return TRUE;
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForSkill_T::CanDoThisActionInThisStatus(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			TargetingAndDepletingParams_T& rTargetingAndDepletingParams = rActor.GetTargetingAndDepletingParams();
			if(TRUE==rActor.Skill_CanUseThisSkillInThisStatus(rTargetingAndDepletingParams.GetActivatedSkill()))
			{
				return TRUE;
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL ActionCallBackFunctorForSkill_T::OnInterrupt(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
			SkillLogic_T const * pLogic = Skill_GetLogic(rSkillInfo);
			if(NULL!=pLogic)
			{
				pLogic->OnInterrupt(rActor);
				return TRUE;
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForSkill_T::OnActivateOnce(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
			SkillLogic_T const * pLogic = Skill_GetLogic(rSkillInfo);
			if(NULL!=pLogic)
			{
				return pLogic->Action_ActivateOnceHandler(rActor);
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForSkill_T::OnActivateEachTick(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
			SkillLogic_T const * pLogic = Skill_GetLogic(rSkillInfo);
			if(NULL!=pLogic)
			{
				return pLogic->Action_ActivateEachTickHandler(rActor);
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForSkill_T::OnDisturbForCharging(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();

			if( !IsCanInteruptChargOrChannel(rParams) )
			{
				return TRUE;
			}

			Time_t nContinuance = rParams.GetContinuance();
			Time_t const nChargeTime = rSkillInfo.GetChargeTime();
			//Time_t nDeltaTime = nChargeTime-nContinuance;
			//if(500<nDeltaTime)
			//{
			//	nDeltaTime = 500;
			//}
			Time_t nLimitTime = nChargeTime-nContinuance;
			SkillID_t iSkillID = rSkillInfo.GetSkillID();
			Skill_Module::SkillTemplateData_T * pSkillTemplate = (Skill_Module::SkillTemplateData_T*)g_SkillTemplateDataMgr.GetInstanceByID(iSkillID);
			FLOAT fBackRate = (FLOAT)pSkillTemplate->GetChannelBackRate();
			Time_t nDeltaTime = (INT)(fBackRate * nChargeTime / 100.0f);
			if( nDeltaTime > nLimitTime )
			{
				nDeltaTime = nLimitTime;
			}
			nContinuance += nDeltaTime; // 干扰聚气时间
			rParams.SetContinuance(nContinuance);
			Action_Module::ActionDelegator_T ActionOptor;
			ActionOptor.BroadcastUnitChargeTimeChanged(rActor, nDeltaTime);
			return TRUE;
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForSkill_T::OnDisturbForChanneling(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();

			if( !IsCanInteruptChargOrChannel(rParams) )
			{
				return TRUE;
			}

			Time_t nContinuance = rParams.GetContinuance();
			//Time_t nDeltaTime = rParams.GetInterval();

			Time_t nChannelTime = rSkillInfo.GetChannelTime();

			SkillID_t iSkillID = rSkillInfo.GetSkillID();
			Skill_Module::SkillTemplateData_T * pSkillTemplate = (Skill_Module::SkillTemplateData_T*)g_SkillTemplateDataMgr.GetInstanceByID(iSkillID);
			FLOAT fBackRate = (FLOAT)pSkillTemplate->GetChannelBackRate();
			Time_t nDeltaTime = (INT)(fBackRate * nChannelTime / 100.0f);

			if(nDeltaTime>nContinuance)
			{
				nDeltaTime = nContinuance;
			}
			nContinuance -= nDeltaTime; // 干扰引导时间
			rParams.SetContinuance(nContinuance);
			Action_Module::ActionDelegator_T ActionOptor;
			ActionOptor.BroadcastUnitChannelTimeChanged(rActor, nDeltaTime);
			if(0>=nContinuance)
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	VOID ActionCallBackFunctorForSkill_T::ResetForNextAction(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rActor.GetTargetingAndDepletingParams();
			rParams.Reset();
		}
		__LEAVE_FUNCTION
	}
	BOOL ActionCallBackFunctorForSkill_T::IsCanInteruptChargOrChannel( ActionParams_T& rParams ) const
	{
		__ENTER_FUNCTION

			Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
			SkillID_t iSkillID = rSkillInfo.GetSkillID();
			Skill_Module::SkillTemplateData_T* pSkillTemplate = (Skill_Module::SkillTemplateData_T*)g_SkillTemplateDataMgr.GetInstanceByID(iSkillID);
			//如果此技能不能使时间回退则不做处理
			if( !pSkillTemplate->IsCanInteruptChannel() )
			{
				return FALSE;
			}
			//如果打断概率不符合不进行回退处理
			INT iRate = pSkillTemplate->GetChannelBackHappanRate();
			if( rActor.getScene()->GetRand100() > iRate )
			{
				return FALSE;
			}
			return TRUE;
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
}


