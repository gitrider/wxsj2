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

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()== g_pLoginPlayerManager->m_ThreadID) ;
	
	LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

	Assert(pLoginPlayer);
	
	//检查用户是否指定状态
	//只有刚连接进入的用户才可以发这个消息
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
	
	//将用户客户端版本纪录住
	pLoginPlayer->SetVersion(pPacket->GetVersion());
	pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_AUTH);
	pLoginPlayer->SetAccount(pPacket->GetAccount());
	//保存玩家输入的密保值
	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		MiBaoUint& mu = pLoginPlayer->m_MiBaoGroup.unit[i];
		strncpy(mu.value,pPacket->GetMiBaoValue(i),MIBAOUNIT_VALUE_LENGTH);

		g_pLog->FastSaveLog(LOG_FILE_0, "MIBAO:CLAskLoginHandler Receive Account=%s PlayerID=%d...idx=%d,[%s]=%s", 
			pPacket->GetAccount(), pLoginPlayer->PlayerID(),i+1,mu.key,mu.value);
	}
	
	//20秒登录验证限制[2007/12/12 YangJun]
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

	//将消息发送到BillingSystem,并设置状态为PS_LOGIN_WAIT_AUTH
	LBAskAuth* pMsg = (LBAskAuth*)g_pPacketFactoryManager->CreatePacket(PACKET_LB_ASKAUTH);
	if(!pMsg)
	{
		AssertEx(FALSE,"创建 LBAskAuth 消息失败");
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