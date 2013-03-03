
#include "stdafx.h"
#include "GCBBSMessages.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"


uint GCBBSMessagesHandler::Execute( GCBBSMessages* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE	MessageNum	=	pPacket->GetMessageNum();
			UINT	Serial		=	pPacket->GetSerial();
			CHAR*	pszTitle	=	pPacket->GetTitle();
			if(CObjectManager::GetMe()->GetMySelf()->GetServerID() == pPacket->GetObjID())
			{//�Լ������԰�
				if(Serial != CDataPool::GetMe()->MyStallMsg_GetSerial())
				{//��������
                    CDataPool::GetMe()->MyStallMsg_ClearUp();
					//ÿ����Ʒ��ӵ����ݳ���
					for(INT i=0; i<MessageNum; i++)
					{
						GCBBSMessages::_MESSAGE_T* pMessageData = pPacket->GetMessageData();
						CDataPool::GetMe()->MyStallMsg_SetIDByIndex(i, pMessageData[i].nID);
						CDataPool::GetMe()->MyStallMsg_SetTimeByIndex(i, pMessageData[i].nHour, pMessageData[i].nMin);
						CDataPool::GetMe()->MyStallMsg_SetHasReplyByIndex(i, pMessageData[i].bHasReply);
						CDataPool::GetMe()->MyStallMsg_SetAuthorNameByIndex(i, pMessageData[i].AuthorName);
						CDataPool::GetMe()->MyStallMsg_SetProposedMessageByIndex(i, pMessageData[i].szMessage);
						if(pMessageData[i].bHasReply)
						{
							CDataPool::GetMe()->MyStallMsg_SetReTimeByIndex(i, pMessageData[i].nReHour, pMessageData[i].nReMin);
							CDataPool::GetMe()->MyStallMsg_SetReplyMessageByIndex(i, pMessageData[i].szReplyMessage);
						}
					}
					CDataPool::GetMe()->MyStallMsg_SetMessageNum(MessageNum);
					CDataPool::GetMe()->MyStallMsg_SetSerial(Serial);
					CDataPool::GetMe()->MyStallMsg_SetTitle(pszTitle);

					//����UI
					CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_MESSAGE,"sale");
				}
				else
				{//�汾����ͬȡ���صľ�����
					//����UI
					CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_MESSAGE,"sale");
				}
			}
			else
			{//���˵����԰�
				CDataPool::GetMe()->OtStallMsg_ClearUp();
				//ÿ����Ʒ��ӵ����ݳ���
				for(INT i=0; i<MessageNum; i++)
				{
					GCBBSMessages::_MESSAGE_T* pMessageData = pPacket->GetMessageData();
					CDataPool::GetMe()->OtStallMsg_SetIDByIndex(i, pMessageData[i].nID);
					CDataPool::GetMe()->OtStallMsg_SetTimeByIndex(i, pMessageData[i].nHour, pMessageData[i].nMin);
					CDataPool::GetMe()->OtStallMsg_SetHasReplyByIndex(i, pMessageData[i].bHasReply);
					CDataPool::GetMe()->OtStallMsg_SetAuthorNameByIndex(i, pMessageData[i].AuthorName);
					CDataPool::GetMe()->OtStallMsg_SetProposedMessageByIndex(i, pMessageData[i].szMessage);
					if(pMessageData[i].bHasReply)
					{
						CDataPool::GetMe()->OtStallMsg_SetReTimeByIndex(i, pMessageData[i].nReHour, pMessageData[i].nReMin);
						CDataPool::GetMe()->OtStallMsg_SetReplyMessageByIndex(i, pMessageData[i].szReplyMessage);
					}
				}

				CDataPool::GetMe()->OtStallMsg_SetSerial(Serial);
				CDataPool::GetMe()->OtStallMsg_SetTitle(pszTitle);
				CDataPool::GetMe()->OtStallMsg_SetMessageNum(MessageNum);
				//����UI
				CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_MESSAGE,"buy");
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
