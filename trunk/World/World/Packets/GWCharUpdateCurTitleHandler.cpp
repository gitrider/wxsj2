
#include "stdafx.h"
#include "GWCharUpdateCurTitle.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerManager.h"

using namespace Packets;

UINT GWCharUpdateCurTitleHandler::Execute(GWCharUpdateCurTitle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->getGuid() );

    ID_t ServerID = g_Config.SceneID2ServerID(pUser->GetSceneID());
    ServerPlayer *pSPlayer = g_pServerManager->GetServerPlayer(ServerID);

	if( pUser==NULL )
	{//没有发现在“在线用户列表”里对应此GUID的用户
		Log::SaveLog( WORLD_LOGFILE, "GWCharUpdateCurTitleHandler...User GUID=%X not find!",
			pPacket->getGuid() );

		Assert( FALSE && "找不到此人" );
		return PACKET_EXE_CONTINUE;
	}

	Log::SaveLog( WORLD_LOGFILE, "GWCharUpdateCurTitleHandler...User GUID=%X Changed Title to Title=%s", 
		pPacket->getGuid(), pPacket->getTitleName() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
