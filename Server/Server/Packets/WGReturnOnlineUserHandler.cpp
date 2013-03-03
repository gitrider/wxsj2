/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "WGReturnOnlineUser.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "PlayerPool.h"
#include "GCReturnOnlineUser.h"

namespace Packets
{

uint	WGReturnOnlineUserHandler::Execute(WGReturnOnlineUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID() ;
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
	if( pGamePlayer==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	GCReturnOnlineUser Msg;
	Msg.SetReturnPostion(pPacket->GetReturnPostion());
	Msg.SetContinue(pPacket->GetContinue());
	Msg.SetResults(pPacket->GetResults());

	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "WGReturnOnlineUserHandler: ok obj=%d,scene=%d",
		pHuman->GetID(), pScene->SceneID());

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
}
