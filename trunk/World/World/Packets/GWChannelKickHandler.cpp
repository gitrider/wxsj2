

#include "stdafx.h"
#include "GWChannelKick.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "ChatCenter.h"
#include "WGChannelError.h"
#include "WGChannelResult.h"





uint GWChannelKickHandler::Execute( GWChannelKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t sGuid = pPacket->GetSourGUID() ;
	GUID_t dGuid = pPacket->GetDestGUID() ;


	USER* pSourUser = g_pOnlineUser->FindUser( sGuid ) ;
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelKickHandler...User Sour GUID=%X not find!", 
			sGuid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid ) ;
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelKickHandler...User Dest GUID=%X not find!", 
			dGuid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pSourUser->GetChannelID() == INVALID_ID )
	{//������û�д���Ƶ��
		WGChannelError Msg ;
		Msg.SetPlayerID( pSourUser->GetPlayerID() ) ;
		Msg.SetErrorCode( CHANNEL_ERROR_NOCHANNEL ) ;

		pSourServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWChannelKickHandler...User Sour GUID=%X not channel!", 
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

		if( !pChannel->IsMember( pDestUser->GetGUID() ) )
		{//�����߲��ڴ�Ƶ����
			WGChannelError Msg ;
			Msg.SetPlayerID( pSourUser->GetPlayerID() ) ;
			Msg.SetErrorCode( CHANNEL_ERROR_NOTINCHANNEL ) ;

			pSourServerPlayer->SendPacket( &Msg ) ;

			Log::SaveLog( WORLD_LOGFILE, "GWChannelKickHandler...User Dest GUID=%X not in channel!", 
				dGuid ) ;
		}
		else
		{
			WGChannelResult Msg ;
			Msg.SetReturn( CHANNEL_RESULT_KICKMEMBER ) ;
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

			//ɾ��Ƶ��������
			pChannel->DelMember( pDestUser->GetGUID() ) ;

			Log::SaveLog( WORLD_LOGFILE, "GWChannelKickHandler...User Dest GUID=%X kicked from ChannelID=%d!", 
				dGuid, pSourUser->GetChannelID() ) ;
		}
	}


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
