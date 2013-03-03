

#include "stdafx.h"
#include "GWTeamInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "Team.h"
#include "WGTeamError.h"
#include "WGTeamResult.h"
#include "WGTeamAskInvite.h"
#include "WGTeamLeaderAskInvite.h"



uint GWTeamInviteHandler::Execute( GWTeamInvite* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID();

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( pPacket->GetDestName() );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...User Dest Name=%s not find!", 
			pPacket->GetDestName() );

		return PACKET_EXE_CONTINUE;
	}

	//判断双方是否同一国家,不同国家不能组队
	INT SourCountry = pSourUser->GetCountry();
	INT DestCountry = pDestUser->GetCountry();

	//国家不同
	if ( SourCountry != DestCountry ) 
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...SourCountry %d != DestCountry %d. ", 
			SourCountry,DestCountry );
		return PACKET_EXE_CONTINUE;	
	}

	GUID_t dGuid = pDestUser->GetGUID();

	// 设置了拒绝被邀请
	if( pDestUser->IsRefuseTeamInvite() )
	{
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetErrorCode(TEAM_ERROR_REFUSEINVITESETTING);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestGUID=%X refuse to be invited. ", 
			dGuid );
		return PACKET_EXE_CONTINUE;
	}

	//如果被邀请的人掉线，则给邀请人返回掉线提示信息
	if ( pDestUser->UserStatus() == US_OFFLINE )
	{
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetErrorCode(TEAM_ERROR_INVITEEOFFLINE);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestOffLine SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
		return PACKET_EXE_CONTINUE;
	}

	//如果被邀请的人死亡，则给邀请人返回死亡提示信息
	//就是在这里获得不到FULLUSERDATA的指针，GWAskUserDataHandler.cpp就被销毁了
	/*FULLUSERDATA* FullUserData = pDestUser->GetFullUserData();
	if (FullUserData != NULL)
	{
		if( FullUserData->m_Human.m_HP<=0 )
		{
			WGTeamError Msg;
			Msg.SetPlayerID( pSourUser->GetPlayerID() );
			Msg.SetErrorCode(TEAM_ERROR_INVITEEISDIE);
			pSourServerPlayer->SendPacket( &Msg );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestIsDie SourGUID=%X DestGUID=%X", 
				sGuid, dGuid );
			return PACKET_EXE_CONTINUE;
		}
	}*/
	

	if( pDestUser->GetTeamID() != INVALID_ID )
	{//被邀请人已经属于某个队伍了
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetErrorCode(TEAM_ERROR_INVITEDESTHASTEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( sGuid==dGuid && pSourUser->GetTeamID()==INVALID_ID )
	{//邀请人和被邀请人属于同一个人, 且不属于某个队伍
		//创建队伍
		TeamID_t tid = g_pTeamList->CreateTeam( );
		Assert( tid!=INVALID_ID );
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

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
		Msg.SetSceneID( pSourUser->GetSceneID() );
		Msg.SetDataID( pSourUser->GetSex() );
		Msg.SetFamily( pSourUser->GetMenpai() );
		Msg.SetIcon( pSourUser->GetPortrait() );
		Msg.SetLevel( pSourUser->GetLevel() );

		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...CreateTeam GUID=%X TeamID=%d", 
			sGuid, tid );
	}
	else
	{
		if ( pSourUser->GetTeamID() != INVALID_ID )
		{
			Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );
			Assert( pTeam );

			GUID_t tlGUID = pTeam->Leader()->m_Member; // 队长GUID

			if ( tlGUID != sGuid )
			{ // 不是队长的情况
				//向被邀请人回复，等待其同意

				/*WGTeamAskInvite Msg;
				Msg.SetPlayerID( pDestUser->GetPlayerID() );
				Msg.SetGUID( sGuid );

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
					{//如果队员离线,则用户数据是空
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

				ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( pDestUser->GetServerID() );
				pDestServerPlayer->SendPacket( &Msg );

				Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...TeamLeaderAskInvite SourGUID=%X DestGUID=%X", 
					sGuid, dGuid );
				return PACKET_EXE_CONTINUE;*/



				// 邀请人有队伍但不是队长
				/*USER* pLeader = g_pOnlineUser->FindUser( tlGUID );

				pLeaderServerPlayer = g_pServerManager->GetServerPlayer( pLeader->GetServerID() );
				if ( pLeaderServerPlayer == NULL )
				{
					Assert( FALSE );
					pLeader = NULL;
				}*/

				//通知队长
				USER* pLeaderUser = g_pOnlineUser->FindUser( tlGUID );
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

		ID_t DestServerID = pDestUser->GetServerID();
		ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
		if( pDestServerPlayer )
		{//向被邀请人发送邀请消息
			WGTeamAskInvite Msg;
			Msg.SetPlayerID( pDestUser->GetPlayerID() );
			Msg.SetGUID( sGuid );

			if ( pSourUser->GetTeamID() != INVALID_ID )
			{
				Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );
				Assert( pTeam );

				//多此一举，AddInviterInfo中会设置MemberSize
				//Msg.SetMemberCount( pTeam->MemberCount() );

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
					{//如果队员离线,则用户数据是空
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

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...AskInvite SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );

	}


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
