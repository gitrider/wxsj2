

#include "stdafx.h"
#include "CGConnect.h"
#include "GamePlayer.h"
#include "Log.h"
#include "GWAskUserData.h"
#include "IncomingPlayerManager.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GameTable.h"
#include "SocketAPI.h"
#ifdef __LINUX__
#include <arpa/inet.h>			// for inet_xxx()
#endif

BOOL IsActiveIP( CHAR* szHostIP )
{
	UINT curip = inet_addr( szHostIP ) ;
	INT i ;
	for( i=0; i<g_IPRange.m_DisableCount; i++ )
	{
		UINT dip = inet_addr(g_IPRange.m_aaIPDisable[i]) ;
		UINT dmask = inet_addr(g_IPRange.m_aaMaskDisable[i]) ;
		if( (dip&dmask) == (curip&dmask) )
			return FALSE ;
	}
	for( i=0; i<g_IPRange.m_EnableCount; i++ )
	{
		UINT eip = inet_addr(g_IPRange.m_aaIPEnable[i]) ;
		UINT emask = inet_addr(g_IPRange.m_aaMaskEnable[i]) ;
		if( (eip&emask) == (curip&emask) )
			return TRUE ;
	}
	return FALSE ;
}

uint CGConnectHandler::Execute( CGConnect* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==g_pIncomingPlayerManager->m_ThreadID ) ;

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

	//���IP��ַ�Ƿ�Ϸ�
	if( !IsActiveIP(pGamePlayer->GetSocket()->m_Host ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Not active ip address! " ) ;
		return PACKET_EXE_ERROR ;
	}

	//��֤�û�����ĺϷ��ԣ����Լ�ʱ��ɣ�
	if( !pGamePlayer->CheckKey(pPacket->GetKey()) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::GamePlayer->CheckKey " ) ;
		return PACKET_EXE_ERROR ;
	}
	if(	pPacket->GetCheckVer()!=CURRENT_VERSION )
	{//��鲻�ϸ�
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Versions FALSE " ) ;
		return PACKET_EXE_ERROR ;
	}

	if( pPacket->GetCountry() >= MAX_COUNTRY_NUM)
	{
		//��鲻�ϸ�
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Country FALSE " ) ;
		return PACKET_EXE_ERROR ;
	}

	if( pPacket->GetProfession() > MAX_MENPAI_NUM)
	{
		//��鲻�ϸ�
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::profession FALSE " ) ;
		return PACKET_EXE_ERROR ;
	}
	//ȡ���û���ɫ���ݣ�����ʼ���û���ɫ��Ϣ
	//  ��������ڱ�ķ������ϣ����������ݿ��ϣ���
	//  ����Ҫ�ȴ���ķ������˽����ݷ��͹���

	ID_t ServerID = pPacket->GetServerID() ;

	
	GWAskUserData* AskMsg = (GWAskUserData*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKUSERDATA)) ;
	AskMsg->SetPlayerID( pGamePlayer->PlayerID() ) ;
	AskMsg->SetGUID( pPacket->GetGUID() ) ;
	AskMsg->SetKey( pPacket->GetKey() ) ;
	AskMsg->SetAccount( pPacket->GetAccount() ) ;
	AskMsg->SetGender( pPacket->GetGender() ) ;//�Ա�
	AskMsg->SetCountry( pPacket->GetCountry() );
	AskMsg->SetProfession( pPacket->GetProfession() );
	AskMsg->SetFaceModel( pPacket->GetFaceModel() );
	g_pServerManager->SendPacket( AskMsg, ServerID ) ;

	pGamePlayer->m_HumanGUID = pPacket->GetGUID() ;

	//��Player״̬����Ϊ���ȴ�������ȡ�����ϡ�
	pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_SERVER_DATA ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGConnect::Execute(GUID=%X SOCKET=%d IP=%s) ...OK ", 
		pPacket->GetGUID(), pGamePlayer->GetSocket()->getSOCKET(), pGamePlayer->GetSocket()->m_Host ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}




