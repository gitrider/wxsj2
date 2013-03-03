

#include "stdafx.h"
#include "GWChannelInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "ChatCenter.h"
#include "WGChannelError.h"
#include "WGChannelResult.h"



uint GWChannelInviteHandler::Execute( GWChannelInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t sGuid = pPacket->GetSourGUID() ;
	GUID_t dGuid = pPacket->GetDestGUID() ;


	USER* pSourUser = g_pOnlineUser->FindUser( sGuid ) ;
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelInviteHandler...User Sour GUID=%X not find!", 
			sGuid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid ) ;
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelInviteHandler...User Dest GUID=%X not find!", 
			dGuid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pSourUser->GetChannelID() == INVALID_ID )
	{//������û�д���Ƶ��
		WGChannelError Msg ;
		Msg.SetPlayerID( pSourUser->GetPlayerID() ) ;
		Msg.SetErrorCode( CHANNEL_ERROR_NOCHANNEL ) ;

		pSourServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWChannelInviteHandler...User Sour GUID=%X not Create Channel!", 
			sGuid ) ;
	}
	else
	{
		ChatChannel* pChannel = g_pChatCenter->GetChatChannel( pSourUser->GetChannelID() ) ;
		if( pChannel==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}

		if( pChannel->IsFull() )
		{//����Ƶ������������
			WGChannelError Msg ;
			Msg.SetPlayerID( pSourUser->GetPlayerID() ) ;
			Msg.SetErrorCode( CHANNEL_ERROR_CHANNELMEMBERFULL ) ;

			pSourServerPlayer->SendPacket( &Msg ) ;

			Log::SaveLog( WORLD_LOGFILE, "GWChannelInviteHandler...User Sour GUID=%X Channel Full!", 
				sGuid ) ;
		}
		else if( pChannel->IsMember( pDestUser->GetGUID() ) )
		{
			WGChannelError Msg ;
			Msg.SetPlayerID( pSourUser->GetPlayerID() ) ;
			Msg.SetErrorCode( CHANNEL_ERROR_MEMBEREXIST ) ;

			pSourServerPlayer->SendPacket( &Msg ) ;

			Log::SaveLog( WORLD_LOGFILE, "GWChannelInviteHandler...User Dest GUID=%X not in ChannelID=%d!", 
				dGuid, pSourUser->GetChannelID() ) ;
		}
		else
		{
			//��ӳ�Ա
			pChannel->AddMember( pDestUser->GetGUID() ) ;

			WGChannelResult Msg ;
			Msg.SetReturn( CHANNEL_RESULT_ADDMEMBER ) ;
			Msg.SetChannelID( pSourUser->GetChannelID() ) ;
			Msg.SetGUID( pDestUser->GetGUID() ) ;

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

			Log::SaveLog( WORLD_LOGFILE, "GWChannelInviteHandler...User Dest GUID=%X join ChannelID=%d!", 
				dGuid, pSourUser->GetChannelID() ) ;
		}
	}


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
