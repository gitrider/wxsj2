

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

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==g_pIncomingPlayerManager->m_ThreadID ) ;

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

	//检查IP地址是否合法
	if( !IsActiveIP(pGamePlayer->GetSocket()->m_Host ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Not active ip address! " ) ;
		return PACKET_EXE_ERROR ;
	}

	//验证用户接入的合法性（可以即时完成）
	if( !pGamePlayer->CheckKey(pPacket->GetKey()) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::GamePlayer->CheckKey " ) ;
		return PACKET_EXE_ERROR ;
	}
	if(	pPacket->GetCheckVer()!=CURRENT_VERSION )
	{//检查不合格
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Versions FALSE " ) ;
		return PACKET_EXE_ERROR ;
	}

	if( pPacket->GetCountry() >= MAX_COUNTRY_NUM)
	{
		//检查不合格
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Country FALSE " ) ;
		return PACKET_EXE_ERROR ;
	}

	if( pPacket->GetProfession() > MAX_MENPAI_NUM)
	{
		//检查不合格
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::profession FALSE " ) ;
		return PACKET_EXE_ERROR ;
	}
	//取得用户角色数据，并初始化用户角色信息
	//  如果数据在别的服务器上（或者在数据库上），
	//  则需要等待别的服务器端将数据发送过来

	ID_t ServerID = pPacket->GetServerID() ;

	
	GWAskUserData* AskMsg = (GWAskUserData*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKUSERDATA)) ;
	AskMsg->SetPlayerID( pGamePlayer->PlayerID() ) ;
	AskMsg->SetGUID( pPacket->GetGUID() ) ;
	AskMsg->SetKey( pPacket->GetKey() ) ;
	AskMsg->SetAccount( pPacket->GetAccount() ) ;
	AskMsg->SetGender( pPacket->GetGender() ) ;//性别
	AskMsg->SetCountry( pPacket->GetCountry() );
	AskMsg->SetProfession( pPacket->GetProfession() );
	AskMsg->SetFaceModel( pPacket->GetFaceModel() );
	g_pServerManager->SendPacket( AskMsg, ServerID ) ;

	pGamePlayer->m_HumanGUID = pPacket->GetGUID() ;

	//将Player状态设置为“等待服务器取得资料”
	pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_SERVER_DATA ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGConnect::Execute(GUID=%X SOCKET=%d IP=%s) ...OK ", 
		pPacket->GetGUID(), pGamePlayer->GetSocket()->getSOCKET(), pGamePlayer->GetSocket()->m_Host ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}




