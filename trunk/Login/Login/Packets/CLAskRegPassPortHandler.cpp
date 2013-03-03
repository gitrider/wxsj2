#include "stdafx.h" 
#include "CLAskRegPassPort.h"
#include "LoginPlayer.h"
#include "LoginPlayerManager.h"
#include "Log.h"
#include "LCRetLogin.h"
#include "LBAskRegAuth.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"


UINT	CLAskRegPassPortHandler::Execute(CLAskRegPassPort* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//����߳�ִ����Դ�Ƿ���ȷ
	if(MyGetCurrentThreadID()!= g_pLoginPlayerManager->m_ThreadID) return PACKET_EXE_CONTINUE;
	
	LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

	Assert(pLoginPlayer);

	//����û������Ƿ�Ϊͬһ���
	if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: CLAskRegPassPortHandler Get Guid Errors,acc = %s,Packet acc = %s",
			pLoginPlayer->GetAccount(),pPacket->GetAccount());
		return PACKET_EXE_ERROR;
	}
	
	//����û��Ƿ�ָ��״̬
	//ֻ��׼��������֤��Ϣ���û��ſ��Է������Ϣ
	if(pLoginPlayer->GetPlayerStatus()!= PS_LOGIN_WAIT_REG_PASSPORT)
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: CLAskRegPassPortHandler Get acc Status Errors,acc = %s,status = %u",
			pLoginPlayer->GetAccount(),pLoginPlayer->GetPlayerStatus());
		return PACKET_EXE_CONTINUE;
	}
	
	Assert(pPacket);

	//�û������
	CHAR	Account[MAX_ACCOUNT+1] = {0};
	strncpy(Account,pPacket->GetAccount(),MAX_ACCOUNT);
	Account[MAX_ACCOUNT] = '\0';

	if(strlen(Account) == 0||!AccNameCheck(Account,MAX_ACCOUNT+1, 1/*g_Config.m_ConfigInfo.m_nLanguage*/))
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_REG_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskRegPassPortHandler::Execute()....Account is empty or acc Invalid,acc=%s",
			pLoginPlayer->GetAccount());
		return PACKET_EXE_CONTINUE;
	}
	
	//����������
	if( strlen(pPacket->GetSuperPw()) == 0 || strlen(pPacket->GetConfirmSuperPw()) == 0 ||
		(strcmp(pPacket->GetSuperPw(),pPacket->GetConfirmSuperPw()) != 0))
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_REG_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskRegPassPortHandler::Execute()....SuperPwd and ConfirmSuperPwd empty or not same,acc=%s",
			pLoginPlayer->GetAccount());
		return PACKET_EXE_CONTINUE;
	}

	//��Ϸ������
	if( strlen(pPacket->GetPwMd5()) == 0 || strlen(pPacket->GetConfirmPwMd5()) == 0 ||
		(strcmp(pPacket->GetPwMd5(),pPacket->GetConfirmPwMd5()) != 0))
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_REG_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskRegPassPortHandler::Execute()....Pwd and ConfirmPwd empty or not same,acc=%s",
			pLoginPlayer->GetAccount());
		return PACKET_EXE_CONTINUE;
	}

	//��Ϸ����ͳ���������벻ͬ����ζ��md5һ����ͬ
	if(strcmp(pPacket->GetPwMd5(),pPacket->GetSuperPw()) == 0)
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_REG_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskRegPassPortHandler::Execute()....Pwd and SuperPwd is same,acc=%s",
			pLoginPlayer->GetAccount());
		return PACKET_EXE_CONTINUE;
	}

	//�ʼ����ȼ��
	if(strlen(pPacket->GetEmail()) < strlen("1@1.c"))
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_REG_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskRegPassPortHandler::Execute()....Email empty or too short,acc=%s",
			pLoginPlayer->GetAccount());
		return PACKET_EXE_CONTINUE;
	}

	LBAskRegAuth* pMsg = (LBAskRegAuth*)g_pPacketFactoryManager->CreatePacket(PACKET_LB_ASKREGAUTH);
	if(!pMsg)
	{
		AssertEx(FALSE,"���� LBAskRegAuth ��Ϣʧ��");
		return PACKET_EXE_ERROR;
	}

	//���Ŀͻ���״̬ΪPS_LOGIN_WAIT_REG_RESULT
	pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_REG_RESULT);

	//��PassPortע����Ϣ���͵�BillingSystem
	pMsg->SetPlayerID(pLoginPlayer->PlayerID());
	pMsg->SetAccount(Account);
	pMsg->SetSuperPassWord(pPacket->GetSuperPw());
	pMsg->SetPassWord(pPacket->GetPwMd5());
	pMsg->SetIP(pLoginPlayer->GetSocket()->m_Host);
	pMsg->SetEmail(pPacket->GetEmail());
	g_pServerManager->SendPacket(pMsg,BILLING_PLAYER_ID);
	
	g_pLog->FastSaveLog(LOG_FILE_0, "CLAskRegPassPortHandler::Execute(...) Account:%s IP:%s PlayerID=%d...OK ", 
		pPacket->GetAccount(), pLoginPlayer->GetSocket()->m_Host,pLoginPlayer->PlayerID() ) ;
		
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}