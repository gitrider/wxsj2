#include "stdafx.h" 
#include "BLRetAuth.h"
#include "LoginPlayer.h"
#include "ServerPlayer.h"
#include "PlayerPool.h"
#include "LoginPlayerManager.h"
#include "LCRetLogin.h"
#include "LCStatus.h"
#include "LWNotifyUser.h"
#include "ServerManager.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "PacketFactoryManager.h"
#include "TurnPlayerQueue.h"
#include "Log.h"
#include "TimeManager.h"


#define MAX_WRONGPW_TIMES 3

UINT BLRetAuthHandler::Execute(BLRetAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	PlayerID_t PlayerID ;
	LoginPlayer* pLoginPlayer = NULL ;
	Assert( pPlayer ) ;
	if( pPlayer->IsServerPlayer() )
	{
		PlayerID = pPacket->GetPlayerID();

		pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else if( pPlayer->IsLoginPlayer() )
	{
		PlayerID = pPlayer->PlayerID() ;
		pLoginPlayer = (LoginPlayer*)pPlayer ;
		
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	TID CurrentThreadID = MyGetCurrentThreadID();
	
	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		if(pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_AUTH)
		{   //ServerManager 中执行
			g_pLoginPlayerManager->SendPacket(pPacket,PlayerID);
			return PACKET_EXE_NOTREMOVE; 
		}
		return PACKET_EXE_CONTINUE;
	}
	else if ( CurrentThreadID == g_pLoginPlayerManager->m_ThreadID)
	{
		//检查GUID 是否正确
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//应该是一次错误操作
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler::Execute Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		if (pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_AUTH)
		{
			if( pPacket->GetResult() == LOGINR_SUCCESS ||
				pPacket->GetResult() == LOGINR_REG_SUCCESS)
			{
				//设置帐号的防沉迷标识[7/10/2007]
				pLoginPlayer->SetAccFatigueSign(pPacket->GetFatigueSign());
				pLoginPlayer->SetAccOnlineTime(pPacket->GetTotalOnlineTime());
				pLoginPlayer->SetPhoneBind(pPacket->GetPhoneBind());
				pLoginPlayer->SetIPBind(pPacket->GetIPBind());
				pLoginPlayer->SetMiBaoBind(pPacket->GetMiBaoBind());
				pLoginPlayer->SetMacBind(pPacket->GetMacBind());
				pLoginPlayer->SetRealNameBind(pPacket->GetRealNameBind());
				pLoginPlayer->SetInputNameBind(pPacket->GetInputNameBind());

				INT nSafeSign = g_Config.m_LoginInfo.NotifySafeSign;
				if(nSafeSign<=0)
				{
					//pLoginPlayer->SetAccount(pPacket->GetAccount());	// no need to set[11/24/2006]
					pLoginPlayer->SetPlayerStatus(PS_LOGIN_AUTHED);
					//把客户端从当前LoginPlayerManager 中释放
					g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
					//并且向ProcessManager 发送消息
					g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
					//修改客户端状态为 PS_LOGIN_WAIT_PROCESS_TURN
					pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_PROCESS_TURN);

					return PACKET_EXE_NOTREMOVE;
				}
				else
				{
					LCRetLogin Msg;
					Msg.SetAccount(pLoginPlayer->GetAccount());
					Msg.SetResult(LOGINR_NOTIFY_SAFESIGN);
					//Msg.SetSafeSign(pLoginPlayer->m_AccountSafeSign);

					pLoginPlayer->SendPacket(&Msg);

					//修改客户端状态为等待确认安全标识
					pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_SAFESIGN_CLIENT);

					g_pLog->FastSaveLog(LOG_FILE_0, "BLRetAuthHandler Wait Client NotifySafeSign acc=%s",
						pLoginPlayer->GetAccount());

					return PACKET_EXE_CONTINUE;
				}

				return PACKET_EXE_CONTINUE;
			}
			else if(pPacket->GetResult() == LOGINR_ONLY_SUCCESS)
			{
				INT nSafeSign = g_Config.m_LoginInfo.NotifySafeSign;
				if(nSafeSign<=0)
				{
					pPacket->SetResult(LOGINR_SUCCESS);
					//pLoginPlayer->SetAccount(pPacket->GetAccount());	// no need to set[11/24/2006]
					pLoginPlayer->SetPlayerStatus(PS_LOGIN_AUTHED);
					//把客户端从当前LoginPlayerManager 中释放
					g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
					//并且向ProcessManager 发送消息
					g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
					//修改客户端状态为 PS_LOGIN_WAIT_PROCESS_TURN
					pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_PROCESS_TURN);

					return PACKET_EXE_NOTREMOVE;
				}
				else
				{
					LCRetLogin Msg;
					Msg.SetAccount(pLoginPlayer->GetAccount());
					Msg.SetResult(LOGINR_NOTIFY_SAFESIGN);
					Msg.SetSafeSign(pLoginPlayer->m_AccountSafeSign);

					pLoginPlayer->SendPacket(&Msg);

					//修改客户端状态为等待确认安全标识
					pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_SAFESIGN_CLIENT);

					g_pLog->FastSaveLog(LOG_FILE_0, "BLRetAuthHandler Wait Client NotifySafeSign acc=%s",
						pLoginPlayer->GetAccount());

					return PACKET_EXE_CONTINUE;
				}

				return PACKET_EXE_CONTINUE;
			}
			else if(pPacket->GetResult() == LOGINR_SAFESIGN_SUCCESS)
			{
				pPacket->SetResult(LOGINR_SUCCESS);
				//pLoginPlayer->SetAccount(pPacket->GetAccount());	// no need to set[11/24/2006]
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_AUTHED);
				//把客户端从当前LoginPlayerManager 中释放
				g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
				//并且向ProcessManager 发送消息
				g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
				//修改客户端状态为 PS_LOGIN_WAIT_PROCESS_TURN
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_PROCESS_TURN);

				return PACKET_EXE_NOTREMOVE;
			}
			else if(pPacket->GetResult() == LOGINR_NEED_REG)
			{
				LCRetLogin Msg;
				Msg.SetAccount(pLoginPlayer->GetAccount());
				Msg.SetResult(LOGINR_NEED_REG);
				pLoginPlayer->SendPacket(&Msg);

				//修改客户端状态为等待注册PassPort
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_REG_PASSPORT);

				g_pLog->FastSaveLog(LOG_FILE_0, "REG: BLRetAuthHandler Wait Client Send PassPort Reg Info acc=%s",
					pLoginPlayer->GetAccount()) ;

				return PACKET_EXE_CONTINUE;
			}
			else if(pPacket->GetResult() == LOGINR_NO_NEW_LICENSE)
			{
				LCRetLogin Msg;
				Msg.SetAccount(pLoginPlayer->GetAccount());
				Msg.SetResult(LOGINR_NO_NEW_LICENSE);
				pLoginPlayer->SendPacket(&Msg);

				//修改客户端状态为等待确认新注册协议
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_LICENSE_CLIENT);
				//设置帐号的防沉迷标识[7/10/2007]
				pLoginPlayer->SetAccFatigueSign(pPacket->GetFatigueSign());
				pLoginPlayer->SetAccOnlineTime(pPacket->GetTotalOnlineTime());
				pLoginPlayer->SetPhoneBind(pPacket->GetPhoneBind());
				pLoginPlayer->SetIPBind(pPacket->GetIPBind());
				pLoginPlayer->SetMiBaoBind(pPacket->GetMiBaoBind());
				pLoginPlayer->SetMacBind(pPacket->GetMacBind());
				pLoginPlayer->SetRealNameBind(pPacket->GetRealNameBind());
				pLoginPlayer->SetInputNameBind(pPacket->GetInputNameBind());

				g_pLog->FastSaveLog(LOG_FILE_0, "BLRetAuthHandler Wait Client Send New License Info acc=%s",
					pLoginPlayer->GetAccount()) ;

				return PACKET_EXE_CONTINUE;
			}
			else if(pPacket->GetResult() == LOGINR_AUTH_FAIL || 
				    pPacket->GetResult() == LOGINR_MIBAO_ERROR ||
					pPacket->GetResult() == LOGINR_MAC_ERROR)
			{
				//pLoginPlayer->SetAccount(pPacket->GetAccount());	// no need to set[11/24/2006]
				pLoginPlayer->SetWrongPWCount(pLoginPlayer->GetWrongPWCount()+1);
				LCRetLogin Msg;
				Msg.SetAccount(pLoginPlayer->GetAccount());
				Msg.SetResult(pPacket->GetResult());
				pLoginPlayer->SendPacket(&Msg);

				if(pLoginPlayer->GetWrongPWCount()>=MAX_WRONGPW_TIMES)
				{
					g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler Reach Max Password Wrong Times acc=%s",
						pLoginPlayer->GetAccount()) ;
					return PACKET_EXE_ERROR;
				}
				else
					return PACKET_EXE_CONTINUE;
			}
			else
			{
				//pLoginPlayer->SetAccount(pPacket->GetAccount());	// no need to set[11/24/2006]
				LCRetLogin Msg;
				Msg.SetAccount(pLoginPlayer->GetAccount());
				Msg.SetResult(pPacket->GetResult());
				Msg.SetServerIP(pPacket->GetServerIP());
				pLoginPlayer->SendPacket(&Msg);
				return PACKET_EXE_CONTINUE;
			}
			
		}
	}
	else if (CurrentThreadID == g_pProcessPlayerManager->m_ThreadID )
	{
		//检查GUID 是否正确
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//应该是一次错误操作
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler::Execute Process Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		//加入到g_pProcessPlayerManager 中，能够处理对应的HeartBeat
		if(FALSE == g_pProcessPlayerManager->AddPlayer(pLoginPlayer))
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler::Execute Process Errors,acc = %s,Packet acc = %s maybe online",
				pLoginPlayer->GetAccount(),pPacket->GetAccount());

			//已有的这个玩家的状态置成等待被踢的状态
			LoginPlayer* pAlreadyInPlayer = (LoginPlayer*)(g_pProcessPlayerManager->FindAccName(pLoginPlayer->GetAccount()));
			if(pAlreadyInPlayer)
			{
				if(pAlreadyInPlayer->GetPlayerStatus() != PS_LOGIN_WAIT_WORLD_KICK)
				{
					//请求World踢掉此已有玩家
					LWNotifyUser* ptMsg = (LWNotifyUser*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_NOTIFYUSER);
					ptMsg->SetAccount(pAlreadyInPlayer->GetAccount());
					ptMsg->SetNotifyStatus(LNOTIFY_KICK_REQUEST);
					ptMsg->SetUserKey(pAlreadyInPlayer->GetUserKey());

					g_pServerManager->SendPacket(ptMsg, WORLD_PLAYER_ID);
				}
				
				//设为等待被踢状态
				pAlreadyInPlayer->SetPlayerStatus(PS_LOGIN_WAIT_WORLD_KICK);
			}

			//主动断开当前的玩家
			return PACKET_EXE_ERROR;
		}

		pLoginPlayer->SetUserKey(rand());

		g_pLog->FastSaveLog(LOG_FILE_0, "BLRetAuthHandler Acc=%s, SetAccFatigueSign=%s, SetAccOnlineTime=%us,PhoneBind=%s,IPBind=%s,MiBaoBind=%s,MacBind=%s,ReadNameBind=%s,InputNameBind=%s",
			pLoginPlayer->GetAccount(),
			pLoginPlayer->GetAccFatigueSign()?"Y":"N",
			pLoginPlayer->GetAccOnlineTime(),
			pLoginPlayer->GetPhoneBind()?"Y":"N",
			pLoginPlayer->GetIPBind()?"Y":"N",
			pLoginPlayer->GetMiBaoBind()?"Y":"N",
			pLoginPlayer->GetMacBind()?"Y":"N",
			pLoginPlayer->GetRealNameBind()?"Y":"N",
			pLoginPlayer->GetInputNameBind()?"Y":"N");

		//先处理login 结果
		LCRetLogin Msg;
		Msg.SetAccount(pLoginPlayer->GetAccount());
		Msg.SetResult(pPacket->GetResult());
		pLoginPlayer->SendPacket(&Msg);

		UINT QueuePos;
		//加入到g_pProcessPlayerQueue 中，能够对客户端排队
		if(g_pProcessPlayerQueue->AddInPlayer(pLoginPlayer->PlayerID(),
			pLoginPlayer->GetAccount(),QueuePos))
		{
			//pLoginPlayer->SetQueuePos(0);
			//设置当前玩家状态
			pLoginPlayer->SetPlayerStatus(PS_LOGIN_PROCESS_TURN);
			pLoginPlayer->SetLastSendTurnTime(g_pTimeManager->CurrentTime());	
			//发送开始排队消息(HeartBeat 中执行）
		}
		else
		{	//排队玩家都超过MAX_TURN_PLAYER个
			//所以只能断开此玩家的网络连接
			
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler Reach MAX_TURN_PLAYER Acc = %s",
				pLoginPlayer->GetAccount()) ;

			//由管理器来清除此玩家
			//return PACKET_EXE_ERROR ;

			/**	doing 29929，2007-12-25，alan
			*	达到排队人数上限，通知客户端，并让该用户进入退出倒计时。
			*/
			pLoginPlayer->SetPlayerStatus(PS_LOGIN_PROCESS_OVER_TURN);

			LCStatus errMsg;
			errMsg.SetClientStatus(CTS_OVER);
			pLoginPlayer->SendPacket(&errMsg);
			pLoginPlayer->TrigerDisconnectCountDown(3000);//3秒后断开连接
			return PACKET_EXE_CONTINUE;
		}
	}
	
	g_pLog->FastSaveLog(LOG_FILE_0, "BLRetAuthHandler::Execute()....OK! acc = %s ",pLoginPlayer->GetAccount()) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}