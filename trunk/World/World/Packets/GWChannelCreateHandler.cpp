

#include "stdafx.h"
#include "GWChannelCreate.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "ChatCenter.h"
#include "WGChannelError.h"
#include "WGChannelResult.h"



uint GWChannelCreateHandler::Execute( GWChannelCreate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t Guid = pPacket->GetGUID() ;

	USER* pUser = g_pOnlineUser->FindUser( Guid ) ;
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelCreateHandler...User GUID=%X not find!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pUser->GetChannelID() != INVALID_ID )
	{//���û��Ѿ�������һ������Ƶ��
		WGChannelError Msg ;

		Msg.SetPlayerID( pUser->GetPlayerID() ) ;
		Msg.SetErrorCode( CHANNEL_ERROR_HASCHANNEL ) ;
		
		pServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWChannelCreateHandler...GUID=%X has channel! cannot create!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	ChannelID_t cid = g_pChatCenter->CreateChatChannel() ;
	if( cid==INVALID_ID )
	{
		WGChannelError Msg ;

		Msg.SetPlayerID( pUser->GetPlayerID() ) ;
		Msg.SetErrorCode( CHANNEL_ERROR_CHANNELFULL ) ;
		
		pServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWChannelCreateHandler...GUID=%X create fail !", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	//�����Խ�����Ƶ����Ϣ
	pUser->SetChannelID( cid );

	//���ͽ��
	WGChannelResult Msg ;
	Msg.SetPlayerID( pUser->GetPlayerID() ) ;
	Msg.SetReturn( CHANNEL_RESULT_CREATE ) ;
	Msg.SetChannelID( pUser->GetChannelID() ) ;
	
	pServerPlayer->SendPacket( &Msg ) ;

	Log::SaveLog( WORLD_LOGFILE, "GWChannelCreateHandler...GUID=%X ChannelID=%d!", 
		Guid, cid ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
