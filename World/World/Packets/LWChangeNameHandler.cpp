#include "stdafx.h"
#include "LWChangeName.h"
//#include "WGChangeName.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
//#include "MergeServerRulerW.h"

UINT LWChangeNameHandler::Execute( LWChangeName* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	Assert( pPacket ) ;

	//SChgNamePacketUnit*	pPacketUnit	= &pPacket->m_sPacketUnit ;

	//// 获得玩家信息
	//USER*			pUser			= g_pOnlineUser->FindUser( pPacketUnit->GetUserGuid() ) ;
	//if( pUser == NULL )
	//{
	//	g_pLog->SaveLog( LOG_FILE_4, "ERROR:LWChangeNameHandler...GUID(%0X) not found User",
	//		pPacketUnit->GetUserGuid() ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

 //   // 获得玩家所在Server
	//ServerPlayer*	pServerPlayer	= g_pServerManager->GetServerPlayer( pUser->GetServerID() ) ;
	//if( pServerPlayer == NULL )
	//{
	//	g_pLog->SaveLog( LOG_FILE_4, "ERROR:LWChangeNameHandler...GUID(%0X),ServerID(%d) not found ServerPlayer",
	//		pPacketUnit->GetUserGuid(),
	//		pUser->GetServerID() ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	//// 若更名成功，刷新帮会列表
	//if( pPacketUnit->GetResultType() == Packets::CHGNAME_RE_OK )
	//{
	//	MergeServerRulerW::DoRefreshAllGuildList( pUser, pPacketUnit->GetUserNewName() );
	//	MergeServerRulerW::DoRefreshMailList( pPacketUnit->GetUserOldName(), pPacketUnit->GetUserNewName() );
	//	MergeServerRulerW::DoRefreshPetProcreateData( pPacketUnit->GetUserOldName(), pPacketUnit->GetUserNewName() );
	//	// 确保最后进行此操作	
	//	MergeServerRulerW::DoRefreshOnlineUserList( pUser, pPacketUnit->GetUserNewName() );
	//}

	//switch( pPacketUnit->GetOperateType() )
	//{
	//case Packets::CHGNAME_OP_USER :
	//	{
	//		WGChangeName		msg ;
	//		msg.m_sPacketUnit	= pPacket->m_sPacketUnit ;
	//		pServerPlayer->SendPacket( &msg ) ;
	//	}
	//	break;
	//case Packets::CHGNAME_OP_GUILD :
	//	break;
	//}
	
	//g_pLog->SaveLog( LOG_FILE_4, "LWChangeNameHandler...OpType(%d),GUID(%0X),OldName(%s),NewName(%s),ReType(%d)",
	//	pPacketUnit->GetOperateType(),
	//	pPacketUnit->GetUserGuid(),
	//	pPacketUnit->GetUserOldName(),
	//	pPacketUnit->GetUserNewName(),
	//	pPacketUnit->GetResultType() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
