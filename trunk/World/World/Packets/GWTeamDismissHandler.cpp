

#include "stdafx.h"
#include "GWTeamDismiss.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "WGTeamResult.h"
#include "ServerManager.h"



uint GWTeamDismissHandler::Execute( GWTeamDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t guid = pPacket->GetGUID() ;//��ɢ�������

	USER* pUser = g_pOnlineUser->FindUser( guid ) ;
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X not find!", 
			guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	TeamID_t tid = pUser->GetTeamID() ;
	Team* pTeam = g_pTeamList->GetTeam( tid ) ;
	if( pTeam==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X TeamID=%d not find team!", 
			guid, tid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pTeam->Leader()->m_Member != guid )
	{//��ɢ������˲��Ƕӳ�
		WGTeamError Msg ;
		Msg.SetPlayerID( pUser->GetPlayerID() ) ;
		Msg.SetErrorCode( TEAM_ERROR_DISMISSNOTLEADER ) ;

		pServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X TeamID=%d not leader!", 
			guid, tid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	WGTeamResult Msg ;
	Msg.SetReturn( TEAM_RESULT_TEAMDISMISS ) ;
	Msg.SetTeamID( tid ) ;
	Msg.SetSceneID( pUser->GetSceneID() );
	Msg.SetGUID( pUser->GetGUID() );
	Msg.SetFamily( pUser->GetMenpai() );
	Msg.SetDataID( pUser->GetSex() );
	Msg.SetIcon( pUser->GetPortrait() );
	Msg.SetLevel( pUser->GetLevel() );

	//Msg.SetPlayerID(  );

	//֪ͨ�������ڵ����
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

		//�����û�������Ϣ
		pUser->SetTeamID( INVALID_ID );
	}
	
	//���������Ϣ
	g_pTeamList->DestoryTeam( tid );

	Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X TeamID=%d Success!", 
		guid, tid ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
