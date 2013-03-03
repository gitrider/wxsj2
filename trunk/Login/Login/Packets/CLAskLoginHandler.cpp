#include "stdafx.h" 
#include "CLAskLogin.h"
#include "LoginPlayer.h"
#include "LoginPlayerManager.h"
#include "ReLogin.h"
#include "Log.h"
#include "LCRetLogin.h"
#include "BLRetAuth.h"
#include "LBAskAuth.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"


UINT	CLAskLoginHandler::Execute(CLAskLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()== g_pLoginPlayerManager->m_ThreadID) ;
	
	LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

	Assert(pLoginPlayer);
	
	//����û��Ƿ�ָ��״̬
	//ֻ�и����ӽ�����û��ſ��Է������Ϣ
	if(pLoginPlayer->GetPlayerStatus()!= PS_LOGIN_CONNECT)
	{
		return PACKET_EXE_ERROR;
	}
	
	Assert(pPacket);

	CHAR	Account[MAX_ACCOUNT+1] = {0};
	strncpy(Account,pPacket->GetAccount(),MAX_ACCOUNT);
	Account[MAX_ACCOUNT] = '\0';

	if(pPacket->GetVersion()!= CURRENT_LOGIN_VERSION)
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_VERSION_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskLoginHandler::Execute()....Version is't compatible,CV= %d,TV=%d,Account = %s ",
		pPacket->GetVersion(),CURRENT_LOGIN_VERSION,Account) ;
		return PACKET_EXE_CONTINUE;
	}
	

	//INT Md5PwLength = pPacket->GetPassWordLength();	// no need[11/24/2006]

	if(strlen(Account) == 0||!AccNameCheck(Account,MAX_ACCOUNT+1,1))//g_Config.m_ConfigInfo.m_nLanguage))
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_AUTH_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskLoginHandler::Execute()....Account is empty or invalid ,ver=%d,acc=%s",
			CURRENT_LOGIN_VERSION,pLoginPlayer->GetAccount() ) ;
		return PACKET_EXE_CONTINUE;
		
	}
	
	//���û��ͻ��˰汾��¼ס
	pLoginPlayer->SetVersion(pPacket->GetVersion());
	pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_AUTH);
	pLoginPlayer->SetAccount(pPacket->GetAccount());
	//�������������ܱ�ֵ
	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		MiBaoUint& mu = pLoginPlayer->m_MiBaoGroup.unit[i];
		strncpy(mu.value,pPacket->GetMiBaoValue(i),MIBAOUNIT_VALUE_LENGTH);

		g_pLog->FastSaveLog(LOG_FILE_0, "MIBAO:CLAskLoginHandler Receive Account=%s PlayerID=%d...idx=%d,[%s]=%s", 
			pPacket->GetAccount(), pLoginPlayer->PlayerID(),i+1,mu.key,mu.value);
	}
	
	//20���¼��֤����[2007/12/12 YangJun]
	if(FALSE == g_pReLoginManager->IsAccCanReLogin(Account))
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_MUST_WAIT);
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLAskLoginHandler::Execute()....Account IsAccCanReLogin Fail, acc=%s",
			pLoginPlayer->GetAccount() ) ;
		return PACKET_EXE_CONTINUE;
	}

	//����Ϣ���͵�BillingSystem,������״̬ΪPS_LOGIN_WAIT_AUTH
	LBAskAuth* pMsg = (LBAskAuth*)g_pPacketFactoryManager->CreatePacket(PACKET_LB_ASKAUTH);
	if(!pMsg)
	{
		AssertEx(FALSE,"���� LBAskAuth ��Ϣʧ��");
	}

	pMsg->SetAccount(pPacket->GetAccount());
	pMsg->SetPassWord(pPacket->GetPassWord());
	pMsg->SetPlayerID(pLoginPlayer->PlayerID());
	pMsg->SetIPAddr(pLoginPlayer->GetSocket()->m_Host);
	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		MiBaoUint& mu = pLoginPlayer->m_MiBaoGroup.unit[i];
		(TRUE == pLoginPlayer->m_bEnableMiBao)?pMsg->SetMiBaoKey(i,mu.row,mu.column):pMsg->ClearMiBaoKey(i);
		pMsg->SetMiBaoValue(i,mu.value);
	}
	pMsg->SetMacAddr(pPacket->GetMacAddr());
	g_pServerManager->SendPacket(pMsg,BILLING_PLAYER_ID);
	

	g_pLog->FastSaveLog(LOG_FILE_0, "CLAskLoginHandler::Execute(...) Account:%s PlayerID=%d MAC=%s...OK ", 
		pPacket->GetAccount(), pLoginPlayer->PlayerID(),pPacket->GetMacAddr());
		
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}