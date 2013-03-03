/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 27 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "CGSearchOnlineUser.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWSearchOnlineUser.h"

namespace Packets
{

uint	CGSearchOnlineUserHandler::Execute(CGSearchOnlineUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	
	GWSearchOnlineUser* pMsg
		= (GWSearchOnlineUser*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_SEARCHONLINEUSER);
	pMsg->SetGUID(pHuman->GetGUID());
	pMsg->SetPostCode(pPacket->GetPostCode());
	pMsg->SetMaxPostion(pPacket->GetMaxPostion());
	pMsg->SetPlayerID(pHuman->GetPlayerID());

	g_pServerManager->SendPacket(pMsg, INVALID_ID);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGSearchOnlineUserHandler: ok obj=%d scene=%d", 
		pHuman->GetID(), pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}
