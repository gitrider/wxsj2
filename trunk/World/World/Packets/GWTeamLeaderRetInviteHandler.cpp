

#include "stdafx.h"
#include "GWTeamLeaderRetInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"
#include "WGTeamLeaderAskInvite.h"
#include "WGTeamAskInvite.h"


uint GWTeamLeaderRetInviteHandler::Execute( GWTeamLeaderRetInvite* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetSourGUID();//�����˵�GUID
	GUID_t dGuid = pPacket->GetDestGUID();//�������˵�GUID
	GUID_t lGuid = pPacket->GetLeaderGUID();//�ӳ���GUID


	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...User Dest GUID=%X not find!", 
			dGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pLeaderUser = g_pOnlineUser->FindUser( lGuid );
	if( pLeaderUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...User Leader GUID=%X not find!", 
			lGuid );

		return PACKET_EXE_CONTINUE;
	}

	ID_t SourServerID = pSourUser->GetServerID();
	ServerPlayer* pSourServerPlayer = g_pServerManager->GetServerPlayer( SourServerID );
	if( pSourServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	ID_t DestServerID = pDestUser->GetServerID();
	ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
	if( pDestServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	ServerPlayer* pLeaderServerPlayer = (ServerPlayer*)pPlayer;

	if( pSourUser->GetTeamID() != pLeaderUser->GetTeamID() )
	{ // �����˺Ͷӳ��Ѿ�����һ��������
		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetErrorCode( TEAM_ERROR_INVITERNOTINTEAM );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Inviter(sGUID=%X) invited dGUID=%X not in Leader's(lGUID=%X) team.", 
			sGuid, dGuid, lGuid );
	}

	//�ж�˫���Ƿ�ͬһ����,��ͬ���Ҳ������
	INT SourCountry = pSourUser->GetCountry();
	INT DestCountry = pDestUser->GetCountry();

	//���Ҳ�ͬ
	if ( SourCountry != DestCountry ) 
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...SourCountry %d != DestCountry %d. ", 
			SourCountry,DestCountry );
		return PACKET_EXE_CONTINUE;	
	}


	else if( pPacket->GetReturn()==FALSE )
	{//�ӳ���ͬ��
		WGTeamError sMsg;
		sMsg.SetPlayerID( pSourUser->GetPlayerID() );
		sMsg.SetErrorCode( TEAM_ERROR_INVITELEADERREFUSE );
		pSourServerPlayer->SendPacket( &sMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Invite Leader Refuse sGUID=%X dGUID=%X lGUID=%X", 
			sGuid, dGuid, lGuid );
	}
	else if( pDestUser->GetTeamID() != INVALID_ID )
	{//�����������ж�����
		WGTeamError sMsg;
		sMsg.SetPlayerID( pSourUser->GetPlayerID() );
		sMsg.SetErrorCode( TEAM_ERROR_INVITEDESTHASTEAM );
		pSourServerPlayer->SendPacket( &sMsg );

		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetErrorCode( TEAM_ERROR_INVITEDESTHASTEAM );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...DestInTeam  sGUID=%X dGUID=%X lGUID=%X", 
			sGuid, dGuid, lGuid );
	}
	else
	{//�������
		TeamID_t tid = pLeaderUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );
		if( pTeam->IsFull() )
		{//���������Ѿ�����
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError lMsg;
			lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			lMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pLeaderServerPlayer->SendPacket( &lMsg );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Team Full  sGUID=%X dGUID=%X lGUID=%X", 
				sGuid, dGuid, lGuid );
		}
		else
		//{//ͬ�ⱻ�����˼������
		//	//����Ҽ������
		//	TEAMMEMBER Member;
		//	Member.m_Member = dGuid;
		//	Member.m_GUIDEx = pDestUser->GetPlayerID();
		//	Member.m_SceneID = pDestUser->GetSceneID();
		//	strncpy((char*)Member.m_Name,(char*)pDestUser->GetName(),MAX_CHARACTER_NAME-1);
		//	Member.m_nPortrait = pDestUser->GetPortrait();
		//	Member.m_uDataID = pDestUser->GetSex();
		//	Member.m_uFamily = pDestUser->GetMenpai();
		//	Member.m_Level   = pDestUser->GetLevel();
		//	pTeam->AddMember( &Member );

		//	//������Ҷ�����Ϣ
		//	pDestUser->SetTeamID( pTeam->GetTeamID() );
		//	WGTeamResult Msg1; // �����¶�Ա��
		//	Msg1.SetPlayerID( pDestUser->GetPlayerID() );
		//	Msg1.SetTeamID( tid );
		//	WGTeamResult Msg2; // ����ÿ����Ա��
		//	Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
		//	Msg2.SetTeamID( tid );
		//	Msg2.SetGUID( dGuid );
		//	Msg2.SetGUIDEx( pDestUser->GetPlayerID() ); // ����ҵ� PlayerID ����
		//	Msg2.SetSceneID( pDestUser->GetSceneID() );
		//	Msg2.SetName( pDestUser->GetName() );
		//	Msg2.SetIcon( pDestUser->GetPortrait() );
		//	Msg2.SetDataID( pDestUser->GetSex() );
		//	Msg2.SetFamily( pDestUser->GetMenpai() );
		//	Msg2.SetLevel( pDestUser->GetLevel() );

		//	//֪ͨ�������ڵ����
		//	for( int i=0; i<pTeam->MemberCount(); i++ )
		//	{
		//		TEAMMEMBER* pMember = pTeam->Member( i );
		//		if( pMember==NULL )
		//		{
		//			Assert(FALSE);
		//			continue;
		//		}

		//		if( pMember->m_Member != dGuid )
		//		{ // ���Լ��������Ҵ��������
		//			Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
		//			Msg1.SetName( pMember->m_Name );
		//			Msg1.SetIcon( pMember->m_nPortrait );
		//			Msg1.SetDataID( pMember->m_uDataID );
		//			Msg1.SetFamily( pMember->m_uFamily );

		//			Msg1.SetGUID( pMember->m_Member );
		//			Msg1.SetGUIDEx( pMember->m_GUIDEx ); // ����ҵ� PlayerID ����
		//			Msg1.SetSceneID( pMember->m_SceneID );
		//			Msg1.SetLevel( pMember->m_Level );

		//			pDestServerPlayer->SendPacket( &Msg1 );
		//		}

		//		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
		//		if( pUser==NULL )
		//		{//�����Ա����,���û������ǿ�
		//			continue;
		//		}

		//		ID_t ServerID = pUser->GetServerID();
		//		ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
		//		if( pServerPlayer==NULL )
		//		{
		//			Assert(FALSE);
		//			continue;
		//		}

		//		Msg2.SetPlayerID( pUser->GetPlayerID() );
		//		pServerPlayer->SendPacket( &Msg2 );
		//	}


		//	Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...AgreeAndInvite!  sGUID=%X dGUID=%X lGUID=%X", 
		//		sGuid, dGuid, lGuid );
		//}
		{
			//ԭ���ǲ��ñ�������ͬ�⣬ֱ����ӵ������У����ڵñ�������ͬ�������ӵ�������
			ID_t DestServerID = pDestUser->GetServerID();
			ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
			if( pDestServerPlayer )
			{
				//�������˷���������Ϣ
				WGTeamAskInvite Msg;
				Msg.SetPlayerID( pDestUser->GetPlayerID() );
				Msg.SetGUID( lGuid );//����øĳɶӳ���ID�������Ƕ�Ա���룬��������ѭ��

				if ( pSourUser->GetTeamID() != INVALID_ID )
				{
					Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );
					Assert( pTeam );

					for( INT i=0; i<pTeam->MemberCount(); ++i )
					{
						WGTeamAskInvite::InviterInfo info;
						TEAMMEMBER* pMember = pTeam->Member( i );
						if( pMember == NULL )
						{
							Assert(FALSE);
							continue ;
						}

						USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
						if( pUser == NULL )
						{//�����Ա����,���û������ǿ�
							info.m_PlayerID = INVALID_ID;
							//continue ;
						}
						else
							info.m_PlayerID = pUser->GetPlayerID();

						info.m_NickSize = (UCHAR)strlen(pMember->m_Name);
						strncpy( (CHAR*)info.m_szNick, pMember->m_Name, info.m_NickSize );
						info.m_uFamily = pMember->m_uFamily;
						info.m_Scene = pMember->m_SceneID;
						info.m_Level = pMember->m_Level;
						info.m_uDataID = pMember->m_uDataID;

						Msg.AddInviterInfo( info );
					}
				}
				else
				{
					//	Msg.SetMemberCount( 1 );
					WGTeamAskInvite::InviterInfo info;
					info.m_PlayerID = pSourUser->GetPlayerID();
					info.m_NickSize = (UCHAR)strlen(pSourUser->GetName());
					strncpy( (CHAR*)info.m_szNick, pSourUser->GetName(), info.m_NickSize );
					info.m_uFamily = pSourUser->GetMenpai();
					info.m_Scene = pSourUser->GetSceneID();
					info.m_Level = pSourUser->GetLevel();
					info.m_uDataID = pSourUser->GetSex();

					Msg.AddInviterInfo( info );
				}

				pDestServerPlayer->SendPacket( &Msg );
			}
			else
			{
				Assert(FALSE);
			}

			Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...AskInvite SourGUID=%X DestGUID=%X", 
				sGuid, dGuid );

		}
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
