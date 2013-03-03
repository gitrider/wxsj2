

#include "stdafx.h"
#include "GWChat.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "WGChat.h"
#include "Team.h"

#include "ChatCenter.h"
#include "PacketFactoryManager.h"


uint GWChatHandler::Execute( GWChat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t SourGUID = pPacket->GetSourGUID() ;

	USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChatHandler...User Sour GUID=%X not find!", 
			SourGUID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	WGChat Msg ;
	Msg.SetChatShowPos(pPacket->GetChatShowPos());
	Msg.SetChatType( pPacket->GetChatType() ) ;
	Msg.SetContexSize( pPacket->GetContexSize() ) ;
	Msg.SetContex( pPacket->GetContex() ) ;
	Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
	Msg.SetSourName( pSourUser->GetName() ) ;
	Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

	switch( pPacket->GetChatType() )
	{
	case CHAT_TYPE_NEAR:
		{
			Assert(FALSE) ;
		}
		break ;
	case CHAT_TYPE_GUILD:
		{
			// add by gh 为了验证有无帮会--有必要--这样会提高效率和安全性 2010/01/29
			if (0 <= pPacket->GetGuildID())
			{
				Msg.SetGuildID( pPacket->GetGuildID() ) ;
				g_pServerManager->BroadCastServer( &Msg ) ;
			} 
		}
		break ;
	case CHAT_TYPE_JIAZU:
		{
			if (0 <= pPacket->GetGuildID() && 0 <= pPacket->GetJiaZuID() )
			{
				Msg.SetGuildID( pPacket->GetGuildID() );
				Msg.SetJiaZuID( pPacket->GetJiaZuID() );
				g_pServerManager->BroadCastServer( &Msg ) ;
			}
		}
		break ;
	case CHAT_TYPE_COUNTRY:
		{
			if (0 <= pPacket->GetCountryID())
			{
				Msg.SetCountryID( pPacket->GetCountryID() );
				g_pServerManager->BroadCastServer( &Msg ) ;
			}
		}
		break ;
	case CHAT_TYPE_SYSTEM:
	case CHAT_TYPE_WORLD:
		{
			g_pServerManager->BroadCastServer( &Msg ) ;
		}
		break ;
	case CHAT_TYPE_TEAM:
		{
			Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() ) ;
			if( pTeam==NULL )
				break ;

			for( int i=0; i<pTeam->MemberCount(); i++ )
			{
				TEAMMEMBER* pMember = pTeam->Member( i ) ;
				if( pMember==NULL )
				{
					Assert(FALSE) ;
					continue ;
				}

				USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member ) ;
				if( pUser==NULL )
				{//如果队员离线,则用户数据是空
					continue ;
				}
				
				ID_t ServerID = pUser->GetServerID() ;
				ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
				if( pServerPlayer==NULL )
				{
					Assert(FALSE) ;
					continue ;
				}

				Msg.SetPlayerID( pUser->GetPlayerID() ) ;

				pServerPlayer->SendPacket( &Msg ) ;
			}
		}
		break ;
	case CHAT_TYPE_USER:
		{
			USER* pUser = g_pOnlineUser->FindUser( pPacket->GetTargetName() ) ;
			if( pUser==NULL || pUser->UserStatus() == US_OFFLINE )//如果私聊对象离线
			{
				CHAR* szName="system" ;
				CHAR szContex[32] ;
				sprintf( szContex, "@@%s", pPacket->GetTargetName() ) ;

				WGChat MsgR ;
				MsgR.SetPlayerID( pSourUser->GetPlayerID() ) ;
				MsgR.SetChatType( CHAT_TYPE_SELF ) ;
				MsgR.SetContexSize( sizeof(szContex) ) ;
				MsgR.SetContex( szContex ) ;
				MsgR.SetSourNameSize( (BYTE)(strlen(szName)) ) ;
				MsgR.SetSourName( szName ) ;
				MsgR.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;
				
				pSourServerPlayer->SendPacket( &MsgR ) ;

				Log::SaveLog( WORLD_LOGFILE, "GWChatHandler...User Name=%s not find!",
					pPacket->GetTargetName() );	

				break ;
			}
				
			ID_t ServerID = pUser->GetServerID() ;
			ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
			if( pServerPlayer==NULL )
			{
				Assert(FALSE) ;
				break ;
			}

			Msg.SetPlayerID( pUser->GetPlayerID() ) ;

			pServerPlayer->SendPacket( &Msg ) ;

			// add by gh for chat_type_self
			WGChat MsgSource ;
			MsgSource.SetPlayerID( pSourUser->GetPlayerID() ) ;
			MsgSource.SetChatType( CHAT_TYPE_USER ) ;
			MsgSource.SetContexSize( pPacket->GetContexSize() ) ;
			MsgSource.SetContex( pPacket->GetContex() ) ;
			MsgSource.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
			MsgSource.SetSourName( pSourUser->GetName() ) ;
			MsgSource.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;
			pSourServerPlayer->SendPacket( &MsgSource ) ;
			// end of add
		}
		break ;
	default :
		{
			Assert(FALSE) ;
		}
		break ;
	};

	g_pChatCenter->PushChatPacket( pPacket ) ;

	Log::SaveLog( WORLD_LOGFILE, "GWChatHandler...GUID=%X ChatType=%d Contex=%s", 
		SourGUID, pPacket->GetChatType(), pPacket->GetContex() ) ;

	return PACKET_EXE_NOTREMOVE ;//不能删除pPacket, 已经放入预存管道

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
