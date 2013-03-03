

#include "stdafx.h"
#include "GWChannelDismiss.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "ChatCenter.h"
#include "WGChannelError.h"
#include "WGChannelResult.h"




uint GWChannelDismissHandler::Execute( GWChannelDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t Guid = pPacket->GetGUID() ;

	USER* pUser = g_pOnlineUser->FindUser( Guid ) ;
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X not find!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pUser->GetChannelID() == INVALID_ID )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X not channel!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	ChatChannel* pChannel = g_pChatCenter->GetChatChannel( pUser->GetChannelID() ) ;
	if( pChannel==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X cannot find channel!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	WGChannelResult Msg ;
	Msg.SetReturn( CHANNEL_RESULT_DISMISS ) ;
	Msg.SetChannelID( pUser->GetChannelID() ) ;

    //֪ͨ����Ƶ���ڵ����
	for( int i=0; i<pChannel->MemberCount(); i++ )
	{
		GUID_t guid = pChannel->Member( i ) ;
		if( guid==INVALID_ID )
		{
			Assert(FALSE) ;
			continue ;
		}

		USER* pUser = g_pOnlineUser->FindUser( guid ) ;
		if( pUser==NULL )
		{//�����Ա����,���û������ǿ�
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

	//����Խ�����Ƶ����Ϣ
	g_pChatCenter->DestoryChatChannel( pUser->GetChannelID() ) ;

	//����û�����
	pUser->SetChannelID( INVALID_ID );

	Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X ChatChannel Dismiss!", 
		Guid ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
