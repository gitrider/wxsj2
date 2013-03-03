

#include "stdafx.h"
#include "WGRetTeamRecruitInfo.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GCRetTeamRecruitInfo.h"

uint WGRetTeamRecruitInfoHandler::Execute( WGRetTeamRecruitInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();

	if ( pScene==NULL )
	{ // 主要是玩家断线
		// Assert( pScene );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetTeamRecruitInfoHandler: OK " );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCRetTeamRecruitInfo Msg;
		for (int i=0;i<pPacket->GetMemberCount();i++)
		{
			Msg.AddMember(*pPacket->GetMember(i));
		}

		pGamePlayer->SendPacket( &Msg );

		//
		g_pLog->FastSaveLog( LOG_FILE_1,"WGRetTeamRecruitInfoHandler::Execute()....OK!");
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
