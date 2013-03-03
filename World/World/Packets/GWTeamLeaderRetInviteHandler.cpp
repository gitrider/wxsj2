

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

	GUID_t sGuid = pPacket->GetSourGUID();//邀请人的GUID
	GUID_t dGuid = pPacket->GetDestGUID();//被邀请人的GUID
	GUID_t lGuid = pPacket->GetLeaderGUID();//队长的GUID


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
	{ // 邀请人和队长已经不在一个队伍了
		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetErrorCode( TEAM_ERROR_INVITERNOTINTEAM );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Inviter(sGUID=%X) invited dGUID=%X not in Leader's(lGUID=%X) team.", 
			sGuid, dGuid, lGuid );
	}

	//判断双方是否同一国家,不同国家不能组队
	INT SourCountry = pSourUser->GetCountry();
	INT DestCountry = pDestUser->GetCountry();

	//国家不同
	if ( SourCountry != DestCountry ) 
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...SourCountry %d != DestCountry %d. ", 
			SourCountry,DestCountry );
		return PACKET_EXE_CONTINUE;	
	}


	else if( pPacket->GetReturn()==FALSE )
	{//队长不同意
		WGTeamError sMsg;
		sMsg.SetPlayerID( pSourUser->GetPlayerID() );
		sMsg.SetErrorCode( TEAM_ERROR_INVITELEADERREFUSE );
		pSourServerPlayer->SendPacket( &sMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Invite Leader Refuse sGUID=%X dGUID=%X lGUID=%X", 
			sGuid, dGuid, lGuid );
	}
	else if( pDestUser->GetTeamID() != INVALID_ID )
	{//被邀请人已有队伍了
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
	{//队伍存在
		TeamID_t tid = pLeaderUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );
		if( pTeam->IsFull() )
		{//队伍人数已经满了
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
		//{//同意被邀请人加入队伍
		//	//将玩家加入队伍
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

		//	//设置玩家队伍信息
		//	pDestUser->SetTeamID( pTeam->GetTeamID() );
		//	WGTeamResult Msg1; // 发给新队员的
		//	Msg1.SetPlayerID( pDestUser->GetPlayerID() );
		//	Msg1.SetTeamID( tid );
		//	WGTeamResult Msg2; // 发给每个队员的
		//	Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
		//	Msg2.SetTeamID( tid );
		//	Msg2.SetGUID( dGuid );
		//	Msg2.SetGUIDEx( pDestUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
		//	Msg2.SetSceneID( pDestUser->GetSceneID() );
		//	Msg2.SetName( pDestUser->GetName() );
		//	Msg2.SetIcon( pDestUser->GetPortrait() );
		//	Msg2.SetDataID( pDestUser->GetSex() );
		//	Msg2.SetFamily( pDestUser->GetMenpai() );
		//	Msg2.SetLevel( pDestUser->GetLevel() );

		//	//通知所有组内的玩家
		//	for( int i=0; i<pTeam->MemberCount(); i++ )
		//	{
		//		TEAMMEMBER* pMember = pTeam->Member( i );
		//		if( pMember==NULL )
		//		{
		//			Assert(FALSE);
		//			continue;
		//		}

		//		if( pMember->m_Member != dGuid )
		//		{ // 将自己以外的玩家传给新玩家
		//			Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
		//			Msg1.SetName( pMember->m_Name );
		//			Msg1.SetIcon( pMember->m_nPortrait );
		//			Msg1.SetDataID( pMember->m_uDataID );
		//			Msg1.SetFamily( pMember->m_uFamily );

		//			Msg1.SetGUID( pMember->m_Member );
		//			Msg1.SetGUIDEx( pMember->m_GUIDEx ); // 将玩家的 PlayerID 传回
		//			Msg1.SetSceneID( pMember->m_SceneID );
		//			Msg1.SetLevel( pMember->m_Level );

		//			pDestServerPlayer->SendPacket( &Msg1 );
		//		}

		//		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
		//		if( pUser==NULL )
		//		{//如果队员离线,则用户数据是空
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
			//原先是不用被邀请者同意，直接添加到队伍中，现在得被邀请者同意才能添加到队伍中
			ID_t DestServerID = pDestUser->GetServerID();
			ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
			if( pDestServerPlayer )
			{
				//向被邀请人发送邀请消息
				WGTeamAskInvite Msg;
				Msg.SetPlayerID( pDestUser->GetPlayerID() );
				Msg.SetGUID( lGuid );//在这得改成队长的ID否则又是队员邀请，进入了死循环

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

			Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...AskInvite SourGUID=%X DestGUID=%X", 
				sGuid, dGuid );

		}
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
