///////////////////////////////////////////////////////////////////////////////
// 文件名：EventCore.cpp
// 程序员：高骐
// 功能说明：事件缓存逻辑内核
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Type.h"
#include "EventCore.h"
#include "Scene.h"
#include "LogicManager.h"
#include "GameStruct_Skill.h"
#include "GCCharSkill_Missed.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;
using namespace Packets;

namespace Event_Module
{
	//class EventCore_T
	BOOL EventCore_T::Init(Scene* pScene)
	{
		__ENTER_FUNCTION
		CleanUp();
		if(NULL!=pScene)
		{
			m_pScene = pScene;
			m_EventQueue.Init(512);
			m_ImpactQueue.Init(256);
			return TRUE;
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	
	VOID EventCore_T::CleanUp(VOID)
	{
		__ENTER_FUNCTION
		//Don't delete the m_pScene pointer
		m_pScene = NULL;
		m_nCountOfEvents = 0;
		m_nCountOfImpacts = 0;
		m_nMaxCountOfEvent = 0;
		m_nMaxCountOfImpacts = 0;
		__LEAVE_FUNCTION
	}
	VOID EventCore_T::HeartBeat(Time_t nDeltaTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return;
		}
		//....
		__LEAVE_FUNCTION
	}
	INT EventCore_T::GetIndexOfFirstAvailableEventSlot(VOID)
	{
		__ENTER_FUNCTION
		return -1;
		//....
		__LEAVE_FUNCTION
		return -1;
	}
	INT EventCore_T::GetIndexOfFirstAvailableImpactSlot(VOID)
	{
		__ENTER_FUNCTION
		return -1;
		//....
		__LEAVE_FUNCTION
		return -1;
	}
	BOOL ResizeAllVector(VOID)
	{
		__ENTER_FUNCTION
		return TRUE;
		//....
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL EventCore_T::RegisterImpactEvent(ObjID_t nReceiver, ObjID_t nSender, OWN_IMPACT& rImp, Time_t nDelayTime, SkillID_t nSkillID)
	{
		__ENTER_FUNCTION
		Scene& rScene = *m_pScene;
		// Routine process
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		if(NULL!=pObj)
		{	
			//如果特殊物体是角色并且处于活动状态，则使其成为接收者
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		//获得发送者对象
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			//如果是角色并且处于活动状态则使其成为发送者
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		//获得效果逻辑
		ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			return FALSE;
		}
		////如果是会心攻击则按会心修正rImp中的效果参数
		//if(rImp.IsCriticalHit())
		//{
		//	pLogic->CriticalRefix(rImp);
		//}
		//如果发送对象合法
		if(NULL!=pSender)
		{
			rImp.SetCasterObjID(pSender->GetID());
			rImp.SetCasterUniqueID(pSender->GetUniqueID());
			//如果发送者是人类
			if(Obj::OBJ_TYPE_HUMAN==pSender->GetObjType())
			{
				//做标记
				rImp.MarkCreateByPlayerFlag();
			}
			rImp.SetCasterLogicCount(pSender->GetLogicCount());
			if(INVALID_ID != nSkillID)
			{
				//用效果逻辑修正角色身上的效果对象属性
				pSender->RefixImpact(rImp);
			}
		}
		//如果接收者合法
		if(NULL!=pReceiver && NULL!=pSender)
		{
			//如果接收者处于无敌状态，并且为敌对行为向client发送未击中消息，原因为免疫
			if(TRUE==pReceiver->IsUnbreakable() && BEHAVIOR_TYPE_HOSTILITY==Impact_GetStandFlag(rImp))
			{
				GCCharSkill_Missed Msg2All;
				Msg2All.SetReceiverID(pReceiver->GetID());
				Msg2All.SetSenderID(pSender->GetID());
				Msg2All.SetFlag(MissFlag_T::FLAG_IMMU);
				Msg2All.SetSenderLogicCount(pSender->GetLogicCount());
				rScene.BroadCast(&Msg2All, pReceiver, TRUE);
			}
			else
			{
				//未击中原因如果不是MissFlag_T::FLAG_NORMAL，向client发送未击中消息
				INT nRet = pReceiver->OnFiltrateImpact(rImp);
				if(MissFlag_T::FLAG_NORMAL!=nRet)
				{
					GCCharSkill_Missed Msg2All;
					Msg2All.SetReceiverID(pReceiver->GetID());
					Msg2All.SetSenderID(pSender->GetID());
					Msg2All.SetFlag(nRet);
					Msg2All.SetSenderLogicCount(pSender->GetLogicCount());
					rScene.BroadCast(&Msg2All, pReceiver, TRUE);
				}
				//否则注册效果，使效果生效
				else
				{
					pReceiver->Impact_RegisterImpact(rImp);
				}
			}
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL EventCore_T::RegisterBeSkillEvent(ObjID_t nReceiver, ObjID_t nSender, INT nBehaviorType, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		//获得接收者
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		//获得发送者
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}

		//test code
		//与宠物和助手有关
		if(NULL!=pReceiver && NULL!=pSender)
		{
			pReceiver->OnBeSkill(*pSender, nBehaviorType);
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL EventCore_T::RegisterSkillMissEvent(ObjID_t nReceiver, ObjID_t nSender, SkillID_t nSkill, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}
		if( NULL==pSender || NULL==pReceiver )
		{
			return FALSE;
		}

		/////////////////////////////////////
		if(TRUE==pObj->IsActiveObj())
		{
			if(TRUE==pReceiver->IsActiveObj() && TRUE==pSender->IsActiveObj())
			{
				pReceiver->OnBeMiss(*pSender);
				pSender->OnMissTarget(*pReceiver);
			}
		}
		////////////////////////////////////
		
		//安全值的保护
		INT iHurtDelayTime = nDelayTime;
		if( nDelayTime < 0 )
		{
			iHurtDelayTime = 0;
		}
		else if( nDelayTime>3000 )
		{
			iHurtDelayTime = 3000;
		}

		if( iHurtDelayTime <= 50 )
		{
			SendDelayTimeMissMSG( nReceiver, nSender, nSkill, pSender->GetLogicCount() );
			return TRUE;
		}
		//向受击玩家缓存延时消息所需信息
		_SkillMissMSGDelayTime oMissDelaytime;
		oMissDelaytime.m_nDelayTime		= iHurtDelayTime;
		oMissDelaytime.m_nLogicCount	= pSender->GetLogicCount();
		oMissDelaytime.m_nSender		= nSender;
		oMissDelaytime.m_nSkill			= nSkill;
		pReceiver->AddDelayTimeSkillMissList( oMissDelaytime );
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	VOID EventCore_T::SendDelayTimeMissMSG( ObjID_t nReceiver, ObjID_t nSender, SkillID_t nSkill, INT iLogicCount )
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		Obj_Character* pReceiver = NULL;

		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		//通知client没有命中
		Packets::GCCharSkill_Missed Msg2All;
		Msg2All.SetReceiverID(nReceiver);
		Msg2All.SetSenderID(nSender);
		Msg2All.SetFlag(MissFlag_T::FLAG_MISS);
		Msg2All.SetSenderLogicCount(iLogicCount);
		if(NULL!=pReceiver)
		{
			if(TRUE==pReceiver->IsActiveObj())
			{
				rScene.BroadCast(&Msg2All, pReceiver, TRUE);
			}
		}
		return;
		__LEAVE_FUNCTION
		return;
	}

	//技能命中目标的事件
	BOOL EventCore_T::RegisterSkillHitEvent(ObjID_t nReceiver, ObjID_t nSender, SkillID_t nSkill, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		//获得接收者
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		//获得发送者
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()))
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		if(NULL!=pReceiver && NULL!=pSender && TRUE==pObj->IsActiveObj())
		{
			if(TRUE==pReceiver->IsActiveObj() && TRUE==pSender->IsActiveObj())
			{
				//调用接收者的Impact逻辑对象的OnBeHit消息函数，并计算盔甲磨损
				pReceiver->OnBeHit(*pSender);
				//调用接收者的Impact逻辑对象的OnBeHit消息函数，并计算武器磨损
				pSender->OnHitTarget(*pReceiver);
			}
		}
		
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL EventCore_T::RegisterActiveSpecialObjEvent(ObjID_t nSpecialObj, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nSpecialObj);
		Obj_Character* pSpecialobj = NULL;
		Obj_Character* pSender = NULL;
		if(NULL!=pObj)
		{
			if(IsSpecialObj(pObj->GetObjType()) && FALSE==pObj->IsActiveObj())
			{
				pSpecialobj = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		if(NULL!=pSpecialobj)
		{
			pSpecialobj->SetActiveFlag(TRUE);
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
};

