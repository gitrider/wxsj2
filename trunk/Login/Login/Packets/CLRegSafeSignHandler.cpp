#include "stdafx.h" 
#include "CLRegSafeSign.h"
#include "LoginPlayer.h"
#include "LoginPlayerManager.h"
#include "Log.h"
#include "BLRetAuth.h"
#include "PacketFactoryManager.h"


UINT	CLRegSafeSignHandler::Execute(CLRegSafeSign* pPacket, Player* pPlayer )
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
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: CLRegSafeSignHandler Get Guid Errors,acc = %s,Packet acc = %s",
			pLoginPlayer->GetAccount(),pPacket->GetAccount());
		return PACKET_EXE_ERROR;
	}
	
	//����û��Ƿ�ָ��״̬
	//ֻ��׼��ȷ�ϰ�ȫ��ʶ���û��ſ��Է������Ϣ
	if(pLoginPlayer->GetPlayerStatus()!= PS_LOGIN_WAIT_SAFESIGN_CLIENT)
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: CLRegSafeSignHandler Get acc Status Errors,acc = %s,status = %u",
			pLoginPlayer->GetAccount(),pLoginPlayer->GetPlayerStatus());
		return PACKET_EXE_CONTINUE;
	}
	
	pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_AUTH);
	//LoginPlayerManager�̣߳�֪ͨLoginPlayerManager�߳�
	BLRetAuth*	pMsg = (BLRetAuth*)g_pPacketFactoryManager->CreatePacket(PACKET_BL_RETAUTH);
	Assert(pMsg);

	//�������
	pMsg->SetAccount(pPacket->GetAccount());
	//����ֵ����
	switch(pPacket->GetUserAnswer())
	{
	case CLRegSafeSign::RSSR_SUCCESS:	//ȷ�ϰ�ȫ��ʶ
		{
			pMsg->SetResult(LOGINR_SAFESIGN_SUCCESS);
			g_pLoginPlayerManager->SendPacket(pMsg,pLoginPlayer->PlayerID());
			g_pLog->FastSaveLog(LOG_FILE_0,"CLRegSafeSignHandler... Acc = %s NotifySafeSign Success!",pPacket->GetAccount());
		}
		break;
	case CLRegSafeSign::RSSR_FAIL:		//������ȫ��ʶ
	default:
		{
			pMsg->SetResult(LOGINR_NEW_LICENSE_FAIL);
			g_pLoginPlayerManager->SendPacket(pMsg,pLoginPlayer->PlayerID());
			g_pLog->FastSaveLog(LOG_FILE_0,"CLRegSafeSignHandler... Acc = %s NotifySafeSign Fail!",pPacket->GetAccount());
		}
		break;
	}
	
	g_pLog->FastSaveLog(LOG_FILE_0, "CLRegSafeSignHandler::Execute(...) Account:%s IP:%s PlayerID=%d...OK ", 
		pPacket->GetAccount(), pLoginPlayer->GetSocket()->m_Host,pLoginPlayer->PlayerID() ) ;
		
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}