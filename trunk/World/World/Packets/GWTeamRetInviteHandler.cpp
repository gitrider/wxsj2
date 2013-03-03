

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

	GUID_t sGuid = pPacket->GetSourGUID();//邀请人的GUID
	GUID_t dGuid = pPacket->GetDestGUID();//被邀请人的GUID

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
	{//被邀请人不同意加入队伍
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
		{ // 邀请人不是队长
			pLeader = g_pOnlineUser->FindUser( pTeam->Leader()->m_Member );

			pLeaderServerPlayer = g_pServerManager->GetServerPlayer( pLeader->GetServerID() );
			if ( pLeaderServerPlayer == NULL )
			{
				Assert( FALSE );
				pLeader = NULL;
			}

			//通知队长
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
			Msg.SetPlayerID( pLeaderUser->GetPlayerID() );//队长的Player_t
			Msg.SetSourGUID( sGuid );		//邀请人
			Msg.SetDestGUID( dGuid );		//被要请人
			Msg.SetSourName( pSourUser->GetName() );
			Msg.SetDestName( pDestUser->GetName() );

			pLeaderServerPlayer->SendPacket( &Msg );

			return PACKET_EXE_CONTINUE;
		}
	}

	if( pDestUser->GetTeamID() != INVALID_ID )
	{//被邀请人已经有队伍
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
	{//邀请人和被邀请人都无队伍

		if ( pSourUser->GetGUID() != pDestUser->GetGUID() )
		{// 队伍已被解散
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
			//将玩家加入队伍中
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

			//设置玩家队伍数据
			pSourUser->SetTeamID( tid );

			//返回结果
			WGTeamResult Msg;
			Msg.SetPlayerID( pSourUser->GetPlayerID() );
			Msg.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg.SetGUID( sGuid );
			Msg.SetTeamID( tid );
			Msg.SetGUIDEx( pSourUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
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
			//将玩家加入队伍中
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

			//设置玩家队伍数据
			pSourUser->SetTeamID( tid );
			pDestUser->SetTeamID( tid );

			//返回结果
			WGTeamResult sMsg1;
			sMsg1.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
			sMsg1.SetGUID( sGuid );
			sMsg1.SetTeamID( tid );
			sMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
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
			sMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
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
			dMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
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
			dMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
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
	{//邀请人有队伍, 被邀请人无队伍
		TeamID_t tid = pSourUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );
		if( pTeam->IsFull() )
		{//队伍人数已经满了
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
		{ //将玩家加入队伍
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

			//设置玩家队伍信息
			pDestUser->SetTeamID( pTeam->GetTeamID() );
			WGTeamResult Msg1; // 发给新队员的
			Msg1.SetPlayerID( pDestUser->GetPlayerID() );
			Msg1.SetTeamID( tid );
			WGTeamResult Msg2; // 发给每个队员的
			Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg2.SetTeamID( tid );
			Msg2.SetGUID( dGuid );
			Msg2.SetGUIDEx( pDestUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			Msg2.SetSceneID( pDestUser->GetSceneID() );
			Msg2.SetName( pDestUser->GetName() );
			Msg2.SetIcon( pDestUser->GetPortrait() );
			Msg2.SetDataID( pDestUser->GetSex() );
			Msg2.SetFamily( pDestUser->GetMenpai() );
			Msg2.SetLevel( pDestUser->GetLevel() );

			//通知所有组内的玩家
			for( int i=0; i<pTeam->MemberCount(); i++ )
			{
				TEAMMEMBER* pMember = pTeam->Member( i );
				if( pMember==NULL )
				{
					Assert(FALSE);
					continue;
				}

				if( pMember->m_Member != dGuid )
				{ // 将自己以外的玩家传给新玩家
					Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
					Msg1.SetName( pMember->m_Name );
					Msg1.SetIcon( pMember->m_nPortrait );
					Msg1.SetDataID( pMember->m_uDataID );
					Msg1.SetFamily( pMember->m_uFamily );

					Msg1.SetGUID( pMember->m_Member );
					Msg1.SetGUIDEx( pMember->m_GUIDEx ); // 将玩家的 PlayerID 传回
					Msg1.SetSceneID( pMember->m_SceneID );
					Msg1.SetLevel( pMember->m_Level );

					pDestServerPlayer->SendPacket( &Msg1 );
				}

				USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
				if( pUser==NULL )
				{//如果队员离线,则用户数据是空
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
