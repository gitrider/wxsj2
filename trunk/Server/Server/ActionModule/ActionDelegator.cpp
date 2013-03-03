///////////////////////////////////////////////////////////////////////////////
// �ļ�����ActionDelegator_T
// ����Ա������
// ����˵���������Ĵ����� , ��ɫ�����Ĺ����ӿںͳ��ù���
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Type.h"
#include "ActionLogic.h"
#include "ChargeAction.h"
#include "ChannelAction.h"
#include "InstantAction.h"
#include "GCCharSkill_Gather.h"
#include "GCCharSkill_Lead.h"
#include "GCCharSkill_Send.h"
#include "GCCharSkill_Gather_Modify.h"
//#include "GCCharSkill_Lead_Modify.h"
#include "GCCharModifyAction.h"
#include "SkillInfo.h"
#include "ActionDelegator.h"
#include "Obj_Character.h"
#include "Scene.h"
#include "DataRecords.h"
#include "GameTable.h"

using namespace Packets;
using namespace Combat_Module;
using namespace Action_Module;
using namespace Combat_Module::Skill_Module;

namespace Action_Module
{
	enum
	{
		MIN_ACTION_TIME = 500,
	};
	//ע:����������action�����Ľӿڣ��������ý�ɫ�߼�״̬״̬�Ľӿ�
	//������治�������÷�������ɫ���߼�״̬
	BOOL ActionDelegator_T::IsPerformingTheSpecificSkill(Obj_Character& rActor, SkillID_t nSkillID) const
	{
		__ENTER_FUNCTION
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(NULL==pActionLogic)
		{
			return FALSE;
		}
		return pActionLogic->IsPerformingSpecificSkill(rActionParams, nSkillID);
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::IsPerformingTheSpecificScript(Obj_Character& rActor, ScriptID_t nScriptID) const
	{
		__ENTER_FUNCTION
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(NULL==pActionLogic)
		{
			return FALSE;
		}
		return pActionLogic->IsPerformingSpecificScript(rActionParams, nScriptID);
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::IsChanneling(Obj_Character& rActor) const
	{
		__ENTER_FUNCTION
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(NULL==pActionLogic)
		{
			return FALSE;
		}
		if(ACTION_CHANNEL!=pActionLogic->GetLogicID())
		{
			return FALSE;
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::RegisterChargeActionForSkill(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const
	{
		__ENTER_FUNCTION
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(FALSE==CanDoNextAction(rActor))
		{
			return FALSE;
		}
		rActionParams.Reset();
		rActionParams.SetActor(&rActor);
		rActionParams.SetContinuance(nContinuance);
		rActionParams.SetCallBackFunctor(GetGlobalSkillCallBackFunctor());
		rActor.SetActionLogic(&(GetGlobalChargeActionLogic()));
		//info client Charge action start
		rActor.AddLogicCount();
		SkillInfo_T& oSkillInf = rActor.GetSkillInfo();
		oSkillInf.SetSkillLogicCount( rActor.GetLogicCount() );
		BroadcastUnitStartChargeAction(rActor,nAction, nContinuance);
		if(0==rActor.GetActionTime())
		{
			rActor.SetActionTime(MIN_ACTION_TIME);
		}
		rActor.OnActionStarted();
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::RegisterChargeActionForScript(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const
	{
		__ENTER_FUNCTION
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(FALSE==CanDoNextAction(rActor))
		{
			return FALSE;
		}
		rActionParams.Reset();
		rActionParams.SetActor(&rActor);
		rActionParams.SetContinuance(nContinuance);
		rActionParams.SetCallBackFunctor(GetGlobalScriptCallBackFunctor());
		rActor.SetActionLogic(&(GetGlobalChargeActionLogic()));
		//info client Charge action start
		rActor.AddLogicCount();
		BroadcastUnitStartChargeAction(rActor,nAction, nContinuance);
		if(0==rActor.GetActionTime())
		{
			rActor.SetActionTime(MIN_ACTION_TIME);
		}
		rActor.OnActionStarted();
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::RegisterChannelActionForSkill(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance, Time_t nInterval) const
	{
		__ENTER_FUNCTION
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(FALSE==CanDoNextAction(rActor))
		{
			return FALSE;
		}
		rActionParams.Reset();
		rActionParams.SetActor(&rActor);
		if(500>nContinuance)
		{
			AssertEx(FALSE,"[ActionDelegator_T::RegisterChannelActionForSkill] illegal nCountinuance found!!");
			nContinuance = 2000;
		}
		rActionParams.SetContinuance(nContinuance);
		if(500>nInterval)
		{
			AssertEx(FALSE,"[ActionDelegator_T::RegisterChannelActionForSkill] illegal interval found!!");
			nInterval = 2000;
		}
		rActionParams.SetInterval(nInterval);
		rActionParams.SetCallBackFunctor(GetGlobalSkillCallBackFunctor());
		rActor.SetActionLogic(&(GetGlobalChannelActionLogic()));
		//info client Channel action start
		rActor.AddLogicCount();
		SkillInfo_T& oSkillInf = rActor.GetSkillInfo();
		oSkillInf.SetSkillLogicCount( rActor.GetLogicCount() );
		BroadcastUnitStartChannelAction(rActor,nAction, nContinuance);
		if(0==rActor.GetActionTime())
		{
			rActor.SetActionTime(MIN_ACTION_TIME);
		}
		rActor.OnActionStarted();
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::RegisterChannelActionForScript(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance, Time_t nInterval) const
	{
		__ENTER_FUNCTION
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(FALSE==CanDoNextAction(rActor))
		{
			return FALSE;
		}
		rActionParams.Reset();
		rActionParams.SetActor(&rActor);
		if(500>nContinuance)
		{
			AssertEx(FALSE,"[ActionDelegator_T::RegisterChannelActionForScript] illegal nCountinuance found!!");
			nContinuance = 2000;
		}
		rActionParams.SetContinuance(nContinuance);
		if(500>nInterval)
		{
			AssertEx(FALSE,"[ActionDelegator_T::RegisterChannelActionForScript] illegal interval found!!");
			nInterval = 2000;
		}
		rActionParams.SetInterval(nInterval);
		rActionParams.SetCallBackFunctor(GetGlobalScriptCallBackFunctor());
		rActor.SetActionLogic(&(GetGlobalChannelActionLogic()));
		//info client Channel action start
		rActor.AddLogicCount();
		BroadcastUnitStartChannelAction(rActor,nAction, nContinuance);
		if(0==rActor.GetActionTime())
		{
			rActor.SetActionTime(MIN_ACTION_TIME);
		}
		rActor.OnActionStarted();
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::RegisterInstantActionForSkill(Obj_Character& rActor, ActionID_t nAction, Time_t nActionTime) const
	{
		__ENTER_FUNCTION
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(FALSE==CanDoNextAction(rActor))
		{
			return FALSE;
		}
		rActionParams.Reset();
		rActionParams.SetActor(&rActor);
		rActor.SetActionLogic(NULL);
		//info client Instant action start
		rActor.AddLogicCount();
		BroadcastUnitStartInstantAction(rActor,nAction, nActionTime);
		if(0==rActor.GetActionTime())
		{
			rActor.SetActionTime(MIN_ACTION_TIME);
		}
		rActor.OnActionStarted();
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::RegisterInstantActionForScript(Obj_Character& rActor, ActionID_t nAction, Time_t nActionTime) const
	{
		__ENTER_FUNCTION
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(FALSE==CanDoNextAction(rActor))
		{
			return FALSE;
		}
		rActionParams.Reset();
		rActionParams.SetActor(&rActor);
		rActor.SetActionLogic(NULL);
		//info client Charge action start
		rActor.AddLogicCount();
		BroadcastUnitStartInstantAction(rActor,nAction, nActionTime);
		if(0==rActor.GetActionTime())
		{
			rActor.SetActionTime(MIN_ACTION_TIME);
		}
		rActor.OnActionStarted();
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::RegisterEmoteAction(Obj_Character& rActor, ActionID_t nAction) const
	{
		__ENTER_FUNCTION
		//����ӿ�ר������һЩ���鶯�����Ժ��ٿ����Ƿ�����
		if(FALSE==CanDoNextAction(rActor))
		{
			return TRUE; //����ֵ��Զ�ɹ�,����ʹ�ö�����ʱ��,��������Ϣ��Ȼ������.
		}		
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		rActionParams.Reset();
		rActionParams.SetActor(&rActor);
		rActor.SetActionLogic(NULL);
		SkillTemplateData_T const* pSkill = g_SkillTemplateDataMgr.GetInstanceByID(nAction);
		if(NULL==pSkill)
		{
			AssertEx(FALSE, "[ActionDelegator_T::RegisterEmoteAction]: unknow Action found!!");
			return FALSE;
		}
		//info client Charge action start
		rActor.AddLogicCount();
		BroadcastUnitStartInstantAction(rActor,nAction, pSkill->GetPlayActionTime());
		//���ﲻ���ö���ʱ��,����ζ������������Ա���ʱ���.
		rActor.OnActionStarted();
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::DisturbCurrentAction(Obj_Character& rActor) const
	{
		__ENTER_FUNCTION
		BOOL bRet = FALSE;
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(NULL==pActionLogic)
		{
			return FALSE;
		}
		bRet = pActionLogic->OnDisturb(rActionParams);
		if(FALSE==bRet)
		{
			InterruptCurrentAction(rActor);
		}
		return bRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::OnHeartBeat(Obj_Character& rActor, Time_t nDeltaTime) const
	{
		__ENTER_FUNCTION
		BOOL bRet = FALSE;
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(NULL==pActionLogic)
		{
			return FALSE;
		}
		if(FALSE==rActor.IsAlive()||FALSE==rActor.IsActiveObj())
		{
			return FALSE;
		}
		bRet = pActionLogic->HeartBeat(rActionParams, nDeltaTime);
		if(FALSE == bRet)
		{
			InterruptCurrentAction(rActor);
		}
		else
		{
			if(0>=rActionParams.GetContinuance())
			{
				CurrentActionFinished(rActor);
			}
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionDelegator_T::CanDoNextAction(Obj_Character& rActor) const
	{
		__ENTER_FUNCTION
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(NULL==pActionLogic)
		{
			return TRUE;
		}
		if(0>=rActionParams.GetContinuance())
		{
			return TRUE;
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	VOID ActionDelegator_T::CurrentActionFinished(Obj_Character& rActor) const
	{
		__ENTER_FUNCTION
		BOOL bRet = FALSE;
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(rActor.GetCharacterLogic()==CHARACTER_LOGIC_USE_SKILL)
		{
			rActor.StopCharacterLogic(FALSE);
		}
		ResetForNextAction(rActor);
		__LEAVE_FUNCTION
	}
	VOID ActionDelegator_T::InterruptCurrentAction(Obj_Character& rActor) const
	{
		__ENTER_FUNCTION
		BOOL bRet = FALSE;
		ActionLogic_T const * pActionLogic = rActor.GetActionLogic();
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		if(NULL!=pActionLogic)
		{
			pActionLogic->OnInterrupt(rActionParams);
		}
		if(rActor.GetCharacterLogic()==CHARACTER_LOGIC_USE_SKILL)
		{
			rActor.StopCharacterLogic(TRUE);
		}
		ResetForNextAction(rActor);
		__LEAVE_FUNCTION
	}
	VOID ActionDelegator_T::BroadcastUnitStartChargeAction(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const
	{
		__ENTER_FUNCTION
		//���ü��ܵ���Ϣ����һ�£��������ɶ�������Ϣ
		// send msg to client...
		SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rActor.GetTargetingAndDepletingParams();
		
		GCCharSkill_Gather Msg ;
		Msg.setObjID( rActor.GetID() );
		Msg.setLogicCount( rActor.GetLogicCount() );
		Msg.setSkillDataID(nAction);
		Msg.setUserPos(*(rActor.getWorldPos()));
		Msg.setTargetID(rParams.GetTargetObj());
		Msg.setTargetPos((WORLD_POS&)rParams.GetTargetPosition());
		Msg.setDir(rParams.GetTargetDirection());
		Msg.setTotalTime(nContinuance);

		Scene* pScene= rActor.getScene();
		if(NULL!=pScene)
		{				
			pScene->BroadCast(&Msg, &rActor, TRUE);
		}
		__LEAVE_FUNCTION
	}
	VOID ActionDelegator_T::BroadcastUnitStartChannelAction(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const
	{
		//���ü��ܵ���Ϣ����һ�£��������ɶ�������Ϣ
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rActor.GetTargetingAndDepletingParams();
		
		// send msg to client...
		GCCharSkill_Lead Msg ;
		Msg.setObjID(rActor.GetID() );
		Msg.setLogicCount( rActor.GetLogicCount() );
		Msg.setSkillDataID(rParams.GetActivatedSkill());
		Msg.setUserPos(*(rActor.getWorldPos()));
		Msg.setTargetID(rParams.GetTargetObj());
		Msg.setTargetPos((WORLD_POS&)rParams.GetTargetPosition());
		Msg.setDir(rParams.GetTargetDirection());
		Msg.setTotalTime(nContinuance);

		Scene* pScene= rActor.getScene();
		if(NULL!=pScene)
		{				
			pScene->BroadCast(&Msg, &rActor, TRUE);
		}
		__LEAVE_FUNCTION
	}
	VOID ActionDelegator_T::BroadcastUnitStartInstantAction(Obj_Character& rActor, ActionID_t nAction, Time_t nActionTime) const
	{
		//���ü��ܵ���Ϣ����һ�£��������ɶ�������Ϣ
		__ENTER_FUNCTION
		//SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
		//TargetingAndDepletingParams_T& rParams = rActor.GetTargetingAndDepletingParams();
		//
		//// send msg to client...
		//GCCharSkill_Send Msg ;
		//Msg.setObjID(rActor.GetID());
		//Msg.setLogicCount(rActor.GetLogicCount());
		//Msg.setSkillDataID(rParams.GetActivatedSkill());
		//Msg.setUserPos(*(rActor.getWorldPos()));
		//Msg.setTargetID(rParams.GetTargetObj());
		//Msg.setTargetPos((WORLD_POS&)rParams.GetTargetPosition());
		//Msg.setDir(rParams.GetTargetDirection());
		//Msg.setTotalTime(nActionTime);
		//Scene* pScene= rActor.getScene();
		//if(NULL!=pScene)
		//{
		//	pScene->BroadCast(&Msg, &rActor, TRUE);
		//}
		__LEAVE_FUNCTION
	}
	VOID ActionDelegator_T::BroadcastUnitChargeTimeChanged(Obj_Character& rActor, Time_t nDeltaTime) const
	{
		//���ü��ܵ���Ϣ����һ�£��������ɶ�������Ϣ
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rActor.GetTargetingAndDepletingParams();
		
		Scene* pScene = rActor.getScene();
		if(NULL!=pScene)
		{
			// send msg to client...
			//GCCharSkill_Gather_Modify Msg ;
			//Msg.setObjID(rActor.GetID());
			//Msg.setSubTime(nDeltaTime);
			//pScene->BroadCast(&Msg, &rActor, TRUE);

			GCCharModifyAction msgModifyAction;
			msgModifyAction.setObjID(rActor.GetID());
			msgModifyAction.setLogicCount(rActor.GetLogicCount());
			msgModifyAction.setModifyTime(nDeltaTime);
			pScene->BroadCast(&msgModifyAction, &rActor, TRUE);
		}
		__LEAVE_FUNCTION
	}
	VOID ActionDelegator_T::BroadcastUnitChannelTimeChanged(Obj_Character& rActor, Time_t nDeltaTime) const
	{
		//���ü��ܵ���Ϣ����һ�£��������ɶ�������Ϣ
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rActor.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rActor.GetTargetingAndDepletingParams();
		
		Scene* pScene = rActor.getScene();
		if(NULL!=pScene)
		{
			// send msg to client...
			//GCCharSkill_Lead_Modify Msg ;
			//Msg.setObjID(rActor.GetID());
			//Msg.setSubTime(nDeltaTime);
			//pScene->BroadCast(&Msg, &rActor, TRUE);

			GCCharModifyAction msgModifyAction;
			msgModifyAction.setObjID(rActor.GetID());
			msgModifyAction.setLogicCount(rActor.GetLogicCount());
			msgModifyAction.setModifyTime(nDeltaTime);
			pScene->BroadCast(&msgModifyAction, &rActor, TRUE);
		}
		__LEAVE_FUNCTION
	}
	VOID ActionDelegator_T::ResetForNextAction(Obj_Character& rActor) const
	{
		__ENTER_FUNCTION
		ActionParams_T&	rActionParams = rActor.GetActionParams();
		ActionCallBackFunctor_T const* pCallBack = rActionParams.GetCallBackFunctor();
		if(NULL!=pCallBack)
		{
			pCallBack->ResetForNextAction(rActionParams);
		}
		rActionParams.Reset();
		rActor.SetActionLogic(NULL);
		__LEAVE_FUNCTION
	}
	
	ActionDelegator_T const g_ActionDelegator;
	ActionDelegator_T const& GetGlobalActionDelegator(VOID)
	{
		return g_ActionDelegator;
	}
}

