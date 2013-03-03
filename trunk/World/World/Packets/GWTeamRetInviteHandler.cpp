

#include "stdafx.h"
#include "GWTeamRetInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"
#include "WGTeamLeaderAskInvite.h"




uint GWTeamRetInviteHandler::Execute( GWTeamRetInvite* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetSourGUID();//�����˵�GUID
	GUID_t dGuid = pPacket->GetDestGUID();//�������˵�GUID

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...User Dest GUID=%X not find!", 
			dGuid );

		return PACKET_EXE_CONTINUE;
	}

	ServerPlayer* pDestServerPlayer = (ServerPlayer*)pPlayer;

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		if( pPacket->GetReturn()==TRUE )
		{
			WGTeamError dMsg;
			dMsg.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg.SetErrorCode( TEAM_ERROR_TARGETNOTONLINE );
			pDestServerPlayer->SendPacket( &dMsg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...User Sour GUID=%X not find!", 
			sGuid );
		return PACKET_EXE_CONTINUE;
	}

	ID_t SourServerID = pSourUser->GetServerID();
	ServerPlayer* pSourServerPlayer = g_pServerManager->GetServerPlayer( SourServerID );
	if( pSourServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	USER* pLeader = NULL;
	ServerPlayer* pLeaderServerPlayer = NULL;

	if( pPacket->GetReturn()==FALSE )
	{//�������˲�ͬ��������
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetErrorCode( TEAM_ERROR_INVITEREFUSE );

		pSourServerPlayer->SendPacket( &Msg );

		if ( pLeader != NULL )
		{
			Msg.SetPlayerID( pLeader->GetPlayerID() );
			pLeaderServerPlayer->SendPacket( &Msg );
		}
		
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Invite Refuse SourGUID=%X DestGUID=%X", 
		sGuid, dGuid );
		return PACKET_EXE_CONTINUE;
	}

	if ( pSourUser->GetTeamID() != INVALID_ID )
	{
		Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );

		if ( pTeam == NULL )
		{
			Assert( pTeam );
			pSourUser->SetTeamID( INVALID_ID );
		}
		else if ( pTeam->Leader()->m_Member != sGuid )
		{ // �����˲��Ƕӳ�
			pLeader = g_pOnlineUser->FindUser( pTeam->Leader()->m_Member );

			pLeaderServerPlayer = g_pServerManager->GetServerPlayer( pLeader->GetServerID() );
			if ( pLeaderServerPlayer == NULL )
			{
				Assert( FALSE );
				pLeader = NULL;
			}

			//֪ͨ�ӳ�
			USER* pLeaderUser = g_pOnlineUser->FindUser( pTeam->Leader()->m_Member );
			if( pLeaderUser == NULL )
			{
				Assert(FALSE);
			}

			ID_t LeaderServerID = pLeaderUser->GetServerID();
			ServerPlayer* pLeaderServerPlayer = g_pServerManager->GetServerPlayer( LeaderServerID );
			if( pLeaderServerPlayer==NULL )
			{
				Assert(FALSE);
			}

			WGTeamLeaderAskInvite Msg;
			Msg.SetPlayerID( pLeaderUser->GetPlayerID() );//�ӳ���Player_t
			Msg.SetSourGUID( sGuid );		//������
			Msg.SetDestGUID( dGuid );		//��Ҫ����
			Msg.SetSourName( pSourUser->GetName() );
			Msg.SetDestName( pDestUser->GetName() );

			pLeaderServerPlayer->SendPacket( &Msg );

			return PACKET_EXE_CONTINUE;
		}
	}

	if( pDestUser->GetTeamID() != INVALID_ID )
	{//���������Ѿ��ж���
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetErrorCode(TEAM_ERROR_INVITEDESTHASTEAM);

		pSourServerPlayer->SendPacket( &Msg );

		if ( pLeader != NULL )
		{
			Msg.SetPlayerID( pLeader->GetPlayerID() );
			pLeaderServerPlayer->SendPacket( &Msg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( pSourUser->GetTeamID()==INVALID_ID )
	{//�����˺ͱ������˶��޶���

		if ( pSourUser->GetGUID() != pDestUser->GetGUID() )
		{// �����ѱ���ɢ
			WGTeamError sMsg;
			sMsg.SetPlayerID( pDestUser->GetPlayerID() );
			sMsg.SetErrorCode( TEAM_ERROR_LEAVETEAM );
			pSourServerPlayer->SendPacket( &sMsg );

			return PACKET_EXE_CONTINUE;
		}

		TeamID_t tid = g_pTeamList->CreateTeam( );
		Assert( tid!=INVALID_ID );
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

		if ( pSourUser->GetGUID() == pDestUser->GetGUID() )
		{
			//����Ҽ��������
			TEAMMEMBER Member;
			Member.m_Member = sGuid;
			Member.m_GUIDEx = pSourUser->GetPlayerID();
			Member.m_SceneID = pSourUser->GetSceneID();
			strncpy((char*)Member.m_Name,(char*)pSourUser->GetName(),MAX_CHARACTER_NAME-1);
			Member.m_nPortrait = pSourUser->GetPortrait();
			Member.m_uDataID = pSourUser->GetSex();
			Member.m_uFamily = pSourUser->GetMenpai();
			Member.m_Level   = pSourUser->GetLevel();

			pTeam->AddMember( &Member );

			//������Ҷ�������
			pSourUser->SetTeamID( tid );

			//���ؽ��
			WGTeamResult Msg;
			Msg.SetPlayerID( pSourUser->GetPlayerID() );
			Msg.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg.SetGUID( sGuid );
			Msg.SetTeamID( tid );
			Msg.SetGUIDEx( pSourUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			Msg.SetIcon( pSourUser->GetPortrait() );
			Msg.SetSceneID( pSourUser->GetSceneID() );
			Msg.SetDataID( pSourUser->GetSex() );
			Msg.SetFamily( pSourUser->GetMenpai() );
			Msg.SetLevel( pSourUser->GetLevel() );

			pSourServerPlayer->SendPacket( &Msg );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...CreateTeam GUID=%X TeamID=%d", 
				sGuid, tid );
		}
		else
		{
			//����Ҽ��������
			TEAMMEMBER sMember;
			sMember.m_Member = sGuid;
			sMember.m_GUIDEx = pSourUser->GetPlayerID();
			sMember.m_SceneID = pSourUser->GetSceneID();
			strncpy((char*)sMember.m_Name,(char*)pSourUser->GetName(),MAX_CHARACTER_NAME-1);
			sMember.m_nPortrait = pSourUser->GetPortrait();
			sMember.m_uDataID = pSourUser->GetSex();
			sMember.m_uFamily = pSourUser->GetMenpai();
			sMember.m_Level   = pSourUser->GetLevel();

			pTeam->AddMember( &sMember );
			TEAMMEMBER dMember;
			dMember.m_Member = dGuid;
			dMember.m_GUIDEx = pDestUser->GetPlayerID();
			dMember.m_SceneID = pDestUser->GetSceneID();
			strncpy((char*)dMember.m_Name,(char*)pDestUser->GetName(),MAX_CHARACTER_NAME-1);
			dMember.m_nPortrait = pDestUser->GetPortrait();
			dMember.m_uDataID = pDestUser->GetSex();
			dMember.m_uFamily = pDestUser->GetMenpai();
			dMember.m_Level   = pDestUser->GetLevel();

			pTeam->AddMember( &dMember );

			//������Ҷ�������
			pSourUser->SetTeamID( tid );
			pDestUser->SetTeamID( tid );

			//���ؽ��
			WGTeamResult sMsg1;
			sMsg1.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
			sMsg1.SetGUID( sGuid );
			sMsg1.SetTeamID( tid );
			sMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			sMsg1.SetSceneID( pSourUser->GetSceneID() );
			sMsg1.SetFamily( pSourUser->GetMenpai() );
			sMsg1.SetDataID( pSourUser->GetSex() );
			sMsg1.SetIcon( pSourUser->GetPortrait() );
			sMsg1.SetLevel( pSourUser->GetLevel() );

			pSourServerPlayer->SendPacket( &sMsg1 );

			WGTeamResult sMsg2;
			sMsg2.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			sMsg2.SetGUID( dGuid );
			sMsg2.SetTeamID( tid );
			sMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			sMsg2.SetSceneID( pDestUser->GetSceneID() );
			sMsg2.SetName( pDestUser->GetName() );
			sMsg2.SetIcon( pDestUser->GetPortrait() );
			sMsg2.SetDataID( pDestUser->GetSex() );
			sMsg2.SetFamily( pDestUser->GetMenpai() );
			sMsg2.SetLevel( pDestUser->GetLevel() );

			pSourServerPlayer->SendPacket( &sMsg2 );

			WGTeamResult dMsg1;
			dMsg1.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
			dMsg1.SetGUID( sGuid );
			dMsg1.SetTeamID( tid );
			dMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			dMsg1.SetSceneID( pSourUser->GetSceneID() );
			dMsg1.SetName( pSourUser->GetName() );
			dMsg1.SetIcon( pSourUser->GetPortrait() );
			dMsg1.SetDataID( pSourUser->GetSex() );
			dMsg1.SetFamily( pSourUser->GetMenpai() );
			dMsg1.SetLevel( pSourUser->GetLevel() );
			dMsg1.SetLevel( pSourUser->GetLevel() );

			pDestServerPlayer->SendPacket( &dMsg1 );

			WGTeamResult dMsg2;
			dMsg2.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			dMsg2.SetGUID( dGuid );
			dMsg2.SetTeamID( tid );
			dMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			dMsg2.SetSceneID( pDestUser->GetSceneID() );
			dMsg2.SetFamily( pDestUser->GetMenpai() );
			dMsg2.SetDataID( pDestUser->GetSex() );
			dMsg2.SetIcon( pDestUser->GetPortrait() );
			pDestServerPlayer->SendPacket( &dMsg2 );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...CreateTeam sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}

	}
	else
	{//�������ж���, ���������޶���
		TeamID_t tid = pSourUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );
		if( pTeam->IsFull() )
		{//���������Ѿ�����
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError dMsg;
			dMsg.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg.SetErrorCode( TEAM_ERROR_APPLYTEAMFULL );
			pDestServerPlayer->SendPacket( &dMsg );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Team Full sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}
		else
		{ //����Ҽ������
			TEAMMEMBER Member;
			Member.m_Member = dGuid;
			Member.m_GUIDEx = pDestUser->GetPlayerID();
			Member.m_SceneID = pDestUser->GetSceneID();
			strncpy((char*)Member.m_Name,(char*)pDestUser->GetName(),MAX_CHARACTER_NAME-1);
			Member.m_nPortrait = pDestUser->GetPortrait();
			Member.m_uDataID = pDestUser->GetSex();
			Member.m_uFamily = pDestUser->GetMenpai();
			Member.m_Level   = pDestUser->GetLevel();
			pTeam->AddMember( &Member );

			//������Ҷ�����Ϣ
			pDestUser->SetTeamID( pTeam->GetTeamID() );
			WGTeamResult Msg1; // �����¶�Ա��
			Msg1.SetPlayerID( pDestUser->GetPlayerID() );
			Msg1.SetTeamID( tid );
			WGTeamResult Msg2; // ����ÿ����Ա��
			Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg2.SetTeamID( tid );
			Msg2.SetGUID( dGuid );
			Msg2.SetGUIDEx( pDestUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			Msg2.SetSceneID( pDestUser->GetSceneID() );
			Msg2.SetName( pDestUser->GetName() );
			Msg2.SetIcon( pDestUser->GetPortrait() );
			Msg2.SetDataID( pDestUser->GetSex() );
			Msg2.SetFamily( pDestUser->GetMenpai() );
			Msg2.SetLevel( pDestUser->GetLevel() );

			//֪ͨ�������ڵ����
			for( int i=0; i<pTeam->MemberCount(); i++ )
			{
				TEAMMEMBER* pMember = pTeam->Member( i );
				if( pMember==NULL )
				{
					Assert(FALSE);
					continue;
				}

				if( pMember->m_Member != dGuid )
				{ // ���Լ��������Ҵ��������
					Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
					Msg1.SetName( pMember->m_Name );
					Msg1.SetIcon( pMember->m_nPortrait );
					Msg1.SetDataID( pMember->m_uDataID );
					Msg1.SetFamily( pMember->m_uFamily );

					Msg1.SetGUID( pMember->m_Member );
					Msg1.SetGUIDEx( pMember->m_GUIDEx ); // ����ҵ� PlayerID ����
					Msg1.SetSceneID( pMember->m_SceneID );
					Msg1.SetLevel( pMember->m_Level );

					pDestServerPlayer->SendPacket( &Msg1 );
				}

				USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
				if( pUser==NULL )
				{//�����Ա����,���û������ǿ�
					continue;
				}

				ID_t ServerID = pUser->GetServerID();
				ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
				if( pServerPlayer==NULL )
				{
					Assert(FALSE);
					continue;
				}

				Msg2.SetPlayerID( pUser->GetPlayerID() );
				pServerPlayer->SendPacket( &Msg2 );

			}

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Success! sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
