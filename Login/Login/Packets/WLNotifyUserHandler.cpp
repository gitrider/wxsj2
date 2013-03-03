#include "stdafx.h" 
#include "WLNotifyUser.h"
#include "LWNotifyUser.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "PacketFactoryManager.h"
#include "LoginPlayer.h"
#include "LCStatus.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT WLNotifyUserHandler::Execute(WLNotifyUser* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		g_pProcessManager->SendPacket(pPacket,INVALID_ID);

		return PACKET_EXE_NOTREMOVE;
	}
	else if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID) 
	{
		Assert(pPacket);
		Assert(NULL == pPlayer);

		LoginPlayer* pLoginPlayer = (LoginPlayer*)(g_pProcessPlayerManager->FindAccName(pPacket->GetAccName()));
		if(NULL == pLoginPlayer)
		{
			LWNotifyUser* ptMsg = (LWNotifyUser*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_NOTIFYUSER);
			ptMsg->SetAccount(pPacket->GetAccName());
			ptMsg->SetNotifyStatus(LNOTIFY_KICK_NOUSR);

			g_pServerManager->SendPacket(ptMsg, WORLD_PLAYER_ID);

			g_pLog->FastSaveLog(LOG_FILE_0,"WLNotifyUserHandler::Execute()....acc=%s status=%d Can't find User",
				pPacket->GetAccName(), pPacket->GetNotifyStatus());

			return PACKET_EXE_CONTINUE;
		}
		
		Assert(pPacket->GetNotifyStatus() != WNOTIFY_NONE);
		//询问成功的玩家可以开始获得人物列表了
		if( pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_WORLD_ASK && 
		    pPacket->GetNotifyStatus() == WNOTIFY_ASK_OK)
		{
			//有可能玩家是通过断线上来的，需要检查UKEY
			if(pLoginPlayer->GetUserKey() == pPacket->GetUserKey())
			{
				//通知客户端可以开始下一步流程
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_NORMAL);
				pLoginPlayer->ResetKick();

				LCStatus Msg;
				Msg.SetClientStatus(CTS_NORMAL);
				pLoginPlayer->SendPacket(&Msg);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLNotifyUserHandler::Execute()....acc=%s status=%d ask status Success",
					pPacket->GetAccName(), pPacket->GetNotifyStatus());
			}
			else
			{
				//UKEY不相同的WNOTIFY_ASK_OK消息包，忽略
				g_pLog->FastSaveLog(LOG_FILE_0,"WLNotifyUserHandler::Execute()....acc=%s status=%d ukey diff ignore ask result",
					pPacket->GetAccName(), pPacket->GetNotifyStatus());
			}

			return PACKET_EXE_CONTINUE;
		}

		//往下的操作，不管UKEY是否正确，都要断开现在的玩家
		if(pPacket->GetNotifyStatus() == WNOTIFY_KICK_REQUEST)
		{
			LWNotifyUser* ptMsg = (LWNotifyUser*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_NOTIFYUSER);
			ptMsg->SetAccount(pLoginPlayer->GetAccount());
			ptMsg->SetNotifyStatus(LNOTIFY_KICK_OK);
			ptMsg->SetUserKey(pLoginPlayer->GetUserKey());

			g_pServerManager->SendPacket(ptMsg, WORLD_PLAYER_ID);

			//注意，此函数的参数Player* pPlayer是空的，所以不能通过返回PACKET_EXE_ERROR断开连接
			//这样的设计，是考率到踢人包的逻辑不和具体某一PlayerID相关
			g_pProcessPlayerManager->RemovePlayer(pLoginPlayer);

			g_pLog->FastSaveLog(LOG_FILE_0,"WLNotifyUserHandler::Execute()....acc=%s status=%d kick order receive from world",
				pPacket->GetAccName(), pPacket->GetNotifyStatus());
		}
		
		if(pPacket->GetNotifyStatus() == WNOTIFY_KICK_NOUSR)
		{
			//暂不考率PS_LOGIN_WAIT_WORLD_KICK
			//if(pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_WORLD_KICK)
			//{
				//注意，此函数的参数Player* pPlayer是空的，所以不能通过返回PACKET_EXE_ERROR断开连接
				//这样的设计，是考率到踢人包的逻辑不和具体某一PlayerID相关
				g_pProcessPlayerManager->RemovePlayer(pLoginPlayer);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLNotifyUserHandler::Execute()....acc=%s status=%d kick not found in world",
					pPacket->GetAccName(), pPacket->GetNotifyStatus());
			//}
		}

		return PACKET_EXE_CONTINUE;
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}