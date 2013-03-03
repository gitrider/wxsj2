
#include "stdafx.h"
#include "CGTopList.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWTopList.h"

uint	CGTopListHandler::Execute(CGTopList* pPacket, Player* pPlayer )
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
	
	GWTopList* pMsg = (GWTopList*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_TOP_LIST);
	pMsg->SetGUID(pHuman->GetGUID());
	pMsg->SetType(pPacket->GetType());
	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGTopListHandler: GUID=%X", pHuman->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}