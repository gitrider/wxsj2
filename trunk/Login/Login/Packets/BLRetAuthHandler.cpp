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
		{   //ServerManager ��ִ��
			g_pLoginPlayerManager->SendPacket(pPacket,PlayerID);
			return PACKET_EXE_NOTREMOVE; 
		}
		return PACKET_EXE_CONTINUE;
	}
	else if ( CurrentThreadID == g_pLoginPlayerManager->m_ThreadID)
	{
		//���GUID �Ƿ���ȷ
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//Ӧ����һ�δ������
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler::Execute Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		if (pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_AUTH)
		{
			if( pPacket->GetResult() == LOGINR_SUCCESS ||
				pPacket->GetResult() == LOGINR_REG_SUCCESS)
			{
				//�����ʺŵķ����Ա�ʶ[7/10/2007]
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
					//�ѿͻ��˴ӵ�ǰLoginPlayerManager ���ͷ�
					g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
					//������ProcessManager ������Ϣ
					g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
					//�޸Ŀͻ���״̬Ϊ PS_LOGIN_WAIT_PROCESS_TURN
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

					//�޸Ŀͻ���״̬Ϊ�ȴ�ȷ�ϰ�ȫ��ʶ
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
					//�ѿͻ��˴ӵ�ǰLoginPlayerManager ���ͷ�
					g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
					//������ProcessManager ������Ϣ
					g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
					//�޸Ŀͻ���״̬Ϊ PS_LOGIN_WAIT_PROCESS_TURN
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

					//�޸Ŀͻ���״̬Ϊ�ȴ�ȷ�ϰ�ȫ��ʶ
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
				//�ѿͻ��˴ӵ�ǰLoginPlayerManager ���ͷ�
				g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
				//������ProcessManager ������Ϣ
				g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
				//�޸Ŀͻ���״̬Ϊ PS_LOGIN_WAIT_PROCESS_TURN
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_PROCESS_TURN);

				return PACKET_EXE_NOTREMOVE;
			}
			else if(pPacket->GetResult() == LOGINR_NEED_REG)
			{
				LCRetLogin Msg;
				Msg.SetAccount(pLoginPlayer->GetAccount());
				Msg.SetResult(LOGINR_NEED_REG);
				pLoginPlayer->SendPacket(&Msg);

				//�޸Ŀͻ���״̬Ϊ�ȴ�ע��PassPort
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

				//�޸Ŀͻ���״̬Ϊ�ȴ�ȷ����ע��Э��
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_LICENSE_CLIENT);
				//�����ʺŵķ����Ա�ʶ[7/10/2007]
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
		//���GUID �Ƿ���ȷ
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//Ӧ����һ�δ������
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler::Execute Process Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		//���뵽g_pProcessPlayerManager �У��ܹ������Ӧ��HeartBeat
		if(FALSE == g_pProcessPlayerManager->AddPlayer(pLoginPlayer))
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler::Execute Process Errors,acc = %s,Packet acc = %s maybe online",
				pLoginPlayer->GetAccount(),pPacket->GetAccount());

			//���е������ҵ�״̬�óɵȴ����ߵ�״̬
			LoginPlayer* pAlreadyInPlayer = (LoginPlayer*)(g_pProcessPlayerManager->FindAccName(pLoginPlayer->GetAccount()));
			if(pAlreadyInPlayer)
			{
				if(pAlreadyInPlayer->GetPlayerStatus() != PS_LOGIN_WAIT_WORLD_KICK)
				{
					//����World�ߵ����������
					LWNotifyUser* ptMsg = (LWNotifyUser*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_NOTIFYUSER);
					ptMsg->SetAccount(pAlreadyInPlayer->GetAccount());
					ptMsg->SetNotifyStatus(LNOTIFY_KICK_REQUEST);
					ptMsg->SetUserKey(pAlreadyInPlayer->GetUserKey());

					g_pServerManager->SendPacket(ptMsg, WORLD_PLAYER_ID);
				}
				
				//��Ϊ�ȴ�����״̬
				pAlreadyInPlayer->SetPlayerStatus(PS_LOGIN_WAIT_WORLD_KICK);
			}

			//�����Ͽ���ǰ�����
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

		//�ȴ���login ���
		LCRetLogin Msg;
		Msg.SetAccount(pLoginPlayer->GetAccount());
		Msg.SetResult(pPacket->GetResult());
		pLoginPlayer->SendPacket(&Msg);

		UINT QueuePos;
		//���뵽g_pProcessPlayerQueue �У��ܹ��Կͻ����Ŷ�
		if(g_pProcessPlayerQueue->AddInPlayer(pLoginPlayer->PlayerID(),
			pLoginPlayer->GetAccount(),QueuePos))
		{
			//pLoginPlayer->SetQueuePos(0);
			//���õ�ǰ���״̬
			pLoginPlayer->SetPlayerStatus(PS_LOGIN_PROCESS_TURN);
			pLoginPlayer->SetLastSendTurnTime(g_pTimeManager->CurrentTime());	
			//���Ϳ�ʼ�Ŷ���Ϣ(HeartBeat ��ִ�У�
		}
		else
		{	//�Ŷ���Ҷ�����MAX_TURN_PLAYER��
			//����ֻ�ܶϿ�����ҵ���������
			
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: BLRetAuthHandler Reach MAX_TURN_PLAYER Acc = %s",
				pLoginPlayer->GetAccount()) ;

			//�ɹ���������������
			//return PACKET_EXE_ERROR ;

			/**	doing 29929��2007-12-25��alan
			*	�ﵽ�Ŷ��������ޣ�֪ͨ�ͻ��ˣ����ø��û������˳�����ʱ��
			*/
			pLoginPlayer->SetPlayerStatus(PS_LOGIN_PROCESS_OVER_TURN);

			LCStatus errMsg;
			errMsg.SetClientStatus(CTS_OVER);
			pLoginPlayer->SendPacket(&errMsg);
			pLoginPlayer->TrigerDisconnectCountDown(3000);//3���Ͽ�����
			return PACKET_EXE_CONTINUE;
		}
	}
	
	g_pLog->FastSaveLog(LOG_FILE_0, "BLRetAuthHandler::Execute()....OK! acc = %s ",pLoginPlayer->GetAccount()) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}