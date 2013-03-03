

#include "stdafx.h"
#include "Log.h"
#include "Scene.h"
#include "ServerManager.h"
#include "SceneManager.h"
#include "WGTopList.h"
#include "PacketFactoryManager.h"
#include "GCTopList.h"
#include "Server.h"
#include "PlayerPool.h"
#include "GamePlayer.h"

UINT	WGTopListHandler::Execute(WGTopList* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = NULL ;
	PlayerID_t PlayerID = pPacket->GetPlayerID();

	if( pPlayer->IsServerPlayer() )
	{
		pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
		if( pGamePlayer==NULL )
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}	
	}
	else if( pPlayer->IsGamePlayer() )
	{
		PlayerID = pPlayer->PlayerID() ;
		pGamePlayer = (GamePlayer*)pPlayer ;
	}
	else
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
		return PACKET_EXE_CONTINUE ;

	Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
	Assert(pPacket);
	
	GCTopList Msg  ;
	WORLD_TOP_LIST* pTop = Msg.GetTopList();
	memcpy(pTop,pPacket->GetTopList(),sizeof(WORLD_TOP_LIST));
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1,"WGTopListHandler::Execute()....OK!");

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

