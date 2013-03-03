/********************************************************************
	created:	2008/05/30
	created:	30:5:2008   10:44
	filename: 	d:\prj\src\MapServer\Server\Other\DoubleMoodAction.cpp
	file path:	d:\prj\src\MapServer\Server\Other
	file base:	DoubleMoodAction
	file ext:	cpp
	author:		wuwenbin
	
	purpose:	双人情感动作
*********************************************************************/

#include "stdafx.h"
#include "DoubleMoodAction.h"
#include "Obj_Human.h"
#include "GCChat.h"
#include "PacketFactoryManager.h"
#include "ChatPipe.h"
#include "Scene.h"
#include "GCOperateResult.h"
#include "GameDefine2.h"

//BOOL DoubleMoodAction::IsDoubleMoodAction(const CHAR* szMoodAct) const
//{
//__ENTER_FUNCTION
//	Assert(szMoodAct);
//
//	if(strstr(szMoodAct, "*D") != NULL){
//		return TRUE;
//	}
//	return FALSE;
//
//__LEAVE_FUNCTION
//	return FALSE;
//}

VOID DoubleMoodAction::ProcessDoubleMoodAction(INT iChatType, const CHAR* szMoodAct, 
											   Obj_Human* pSource, Obj_Human* pDest)
{
__ENTER_FUNCTION
	Assert(szMoodAct);
	Assert(pSource);
	Assert(pDest);		

	switch (iChatType)
	{
	case CHAT_TYPE_DMA_FORCE:
		DoDoubleMoodAction(szMoodAct, pSource, pDest);
		break;
	case CHAT_TYPE_DMA_AGREE:
		DoDoubleMoodAction(szMoodAct, pDest, pSource);
		break;
	case CHAT_TYPE_DMA_REQUEST:
		RequestDoubleMoodAction(szMoodAct, pSource, pDest);
		break;
	case CHAT_TYPE_DMA_REFUSE:
		RefuseDoubleMoodAction(szMoodAct, pSource, pDest);
		break;
	case CHAT_TYPE_DMA_CANCEL:
		CancelDoubleMoodAction(szMoodAct, pSource, pDest);
		break;
	default:
		Assert(FALSE);
		break;
	}
__LEAVE_FUNCTION
}

//INT DoubleMoodAction::GetDoubleMoodActionOptType(const CHAR* szMoodAct) const
//{
//__ENTER_FUNCTION
//	Assert(szMoodAct);
//
//	INT iOptType = MA_OPTT_FORCE;
//	if(strstr(szMoodAct, "*DForce") != NULL){
//		iOptType = MA_OPTT_FORCE;
//	}
//	else if(strstr(szMoodAct, "*DRequest") != NULL){
//		iOptType = MA_OPTT_REFUSE;
//	}
//	else if(strstr(szMoodAct, "*DAgree") != NULL){
//		iOptType = MA_OPTT_AGREE;
//	}
//	else if(strstr(szMoodAct, "*DRefuse") != NULL){
//		iOptType = MA_OPTT_REFUSE;
//	}
//	else if(strstr(szMoodAct, "*DCancel") != NULL){
//		iOptType = MA_OPTT_CANCEL;
//	}
//	else{
//		AssertEx(FALSE, "DoubleMoodAction::GetDoubleMoodActionOptType...szMoodAct's OptType is invalid!");
//	}
//	return iOptType;
//__LEAVE_FUNCTION
//	return MA_OPTT_FORCE;
//}

VOID DoubleMoodAction::DoDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest)
{
__ENTER_FUNCTION
	Assert(szMoodAct);
	Assert(pSource);
	Assert(pDest);		

	/// 给玩家加上相关的Buff

	/// 通知玩家做相应的情感动作
	Scene* pScene = pSource->getScene();
	Assert(pScene);

	GCChat* pPacket = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT));
	Assert(pPacket);

	pPacket->SetChatType(CHAT_TYPE_DMA_FORCE);
	pPacket->SetContexSize((BYTE)strlen(szMoodAct));
	pPacket->SetContex(const_cast<CHAR*>(szMoodAct));
	pPacket->SetSourID(pSource->GetID());
	pPacket->SetDestID(pDest->GetID());

	pScene->BroadCast_Chat(pPacket, pSource->getZoneID());
	//pScene->GetChatPipe()->SendPacket( pPacket, pSource->GetID(), pDest->GetID());

__LEAVE_FUNCTION
}

VOID DoubleMoodAction::RequestDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest)
{
__ENTER_FUNCTION
	Assert(szMoodAct);
	Assert(pSource);
	Assert(pDest);		

	/// 向pDest玩家发送邀请
	GCChat Msg;
	Msg.SetChatType(CHAT_TYPE_DMA_REQUEST);
	Msg.SetContexSize((BYTE)strlen(szMoodAct));
	Msg.SetContex(const_cast<CHAR*>(szMoodAct));
	Msg.SetSourID(pSource->GetID());
	Msg.SetDestID(pDest->GetID());
	/// ...

	pDest->GetPlayer()->SendPacket(&Msg);

__LEAVE_FUNCTION
}

VOID DoubleMoodAction::CancelDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest)
{
__ENTER_FUNCTION
	Assert(szMoodAct);
	Assert(pSource);
	Assert(pDest);		

	/// 去除玩家的相关Buff

	/// 通知玩家停止情感动作
	//////////////////////////////////////////////////////////////////////////


__LEAVE_FUNCTION
}

VOID DoubleMoodAction::RefuseDoubleMoodAction(const CHAR* szMoodAct, Obj_Human* pSource, Obj_Human* pDest)
{
__ENTER_FUNCTION
	Assert(szMoodAct);
	Assert(pSource);
	Assert(pDest);		

	/// 通知pSource邀请被拒绝
	GCOperateResult Msg;
	Msg.setResult(OR_DOUBLEMOODACT_REFUSED);

	pDest->GetPlayer()->SendPacket(&Msg);

__LEAVE_FUNCTION
}

