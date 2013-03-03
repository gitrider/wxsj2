#include "stdafx.h"
#include "GWSetTeamDistribMode.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"
#include "WGRetTeamDistribMode.h"


uint GWSetTeamDistribModeHandler::Execute( GWSetTeamDistribMode* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	BYTE nMode = pPacket->getMode();//mode
	TeamID_t nTeamID = pPacket->getTeamID();//TeamID
	GUID_t LeaderGuid = pPacket->getLeaderID();//Leader的GUID

	USER* pLeaderUser = g_pOnlineUser->FindUser( LeaderGuid );
	if( pLeaderUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWSetTeamDistribMode...User Leader GUID=%X not find!", LeaderGuid );

		return PACKET_EXE_CONTINUE;
	}
	
	if (nMode>=ITEM_DISTRIB_NUM)
	{
		Log::SaveLog( WORLD_LOGFILE, "GWSetTeamDistribMode...MODE=%X error!", nMode );
		return PACKET_EXE_CONTINUE;
	}
	ServerPlayer* pLeaderServerPlayer = (ServerPlayer*)pPlayer;

	if( pLeaderUser->GetTeamID() == INVALID_ID )
	{
		//无队伍
		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetErrorCode( TEAM_ERROR_DISTRIBNOLEADER );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWSetTeamDistribMode... LeaderGuid=%X!", LeaderGuid );
	}
	else
	{//队伍存在
		TeamID_t tid = pLeaderUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

		if( pTeam->Leader()->m_Member != LeaderGuid )
		{
			//队长GUID和队伍中记录的队长GUID不符
			WGTeamError sMsg;
			sMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			sMsg.SetErrorCode( TEAM_ERROR_DISTRIBNOLEADER );
			pLeaderServerPlayer->SendPacket( &sMsg );

			Log::SaveLog( WORLD_LOGFILE, "GWSetTeamDistribMode... LeaderGuid=%X!", LeaderGuid );
		}
		else 
		{//
			pTeam->SetDistribMode(nMode);
			WGRetTeamDistribMode Msg;
			Msg.setLeaderID( pLeaderUser->GetPlayerID() );
			Msg.setTeamID(pLeaderUser->GetTeamID());
			Msg.setMode(nMode);

			pLeaderServerPlayer->SendPacket( &Msg );

		}//end for

		Log::SaveLog( WORLD_LOGFILE, "GWSetTeamDistribMode...Success!  GUID=%X", LeaderGuid );

	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_CONTINUE;	
}