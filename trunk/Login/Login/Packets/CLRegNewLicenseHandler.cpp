#include "stdafx.h" 
#include "CLRegNewLicense.h"
#include "LoginPlayer.h"
#include "LoginPlayerManager.h"
#include "Log.h"
#include "LCRetLogin.h"
#include "LBAskRegLicense.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"


UINT	CLRegNewLicenseHandler::Execute(CLRegNewLicense* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//����߳�ִ����Դ�Ƿ���ȷ
	if(MyGetCurrentThreadID()!= g_pLoginPlayerManager->m_ThreadID) return PACKET_EXE_CONTINUE;
	
	LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

	Assert(pLoginPlayer);
	Assert(pPacket);

	//����û������Ƿ�Ϊͬһ���
	if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: CLRegNewLicenseHandler Get Guid Errors,acc = %s,Packet acc = %s",
			pLoginPlayer->GetAccount(),pPacket->GetAccount());
		return PACKET_EXE_ERROR;
	}
	
	//����û��Ƿ�ָ��״̬
	//ֻ��׼��ȷ�����Э����û��ſ��Է������Ϣ
	if(pLoginPlayer->GetPlayerStatus()!= PS_LOGIN_WAIT_LICENSE_CLIENT)
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: CLRegNewLicenseHandler Get acc Status Errors,acc = %s,status = %u",
			pLoginPlayer->GetAccount(),pLoginPlayer->GetPlayerStatus());
		return PACKET_EXE_CONTINUE;
	}
	
	//�û������
	CHAR	Account[MAX_ACCOUNT+1] = {0};
	strncpy(Account,pPacket->GetAccount(),MAX_ACCOUNT);
	Account[MAX_ACCOUNT] = '\0';

	LBAskRegLicense* pMsg = (LBAskRegLicense*)g_pPacketFactoryManager->CreatePacket(PACKET_LB_ASKREGLICENSE);
	if(!pMsg)
	{
		AssertEx(FALSE,"���� LBAskRegLicense ��Ϣʧ��");
		return PACKET_EXE_ERROR;
	}

	//���Ŀͻ���״̬ΪPS_LOGIN_WAIT_LICENSE_RESULT
	pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_LICENSE_RESULT);

	//���û�ȷ�����Э���״̬���͵�BillingSystem
	pMsg->SetPlayerID(pLoginPlayer->PlayerID());
	pMsg->SetAccount(Account);
	pMsg->SetUserAnswer(pPacket->GetUserAnswer());
	g_pServerManager->SendPacket(pMsg,BILLING_PLAYER_ID);
	
	g_pLog->FastSaveLog(LOG_FILE_0, "CLRegNewLicenseHandler::Execute(...) Account:%s IP:%s PlayerID=%d...OK ", 
		pPacket->GetAccount(), pLoginPlayer->GetSocket()->m_Host,pLoginPlayer->PlayerID() ) ;
		
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}