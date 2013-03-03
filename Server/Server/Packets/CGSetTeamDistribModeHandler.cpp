#include "stdafx.h"
#include "CGSetTeamDistribMode.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamRetApply.h"
#include "ServerManager.h"
#include "GWSetTeamDistribMode.h"

uint CGSetTeamDistribModeHandler::Execute( CGSetTeamDistribMode* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	
	BYTE nMode = pPacket->getMode();
	if (nMode>=ITEM_DISTRIB_NUM)
	{
		return PACKET_EXE_ERROR;
	}
	TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
	if (pTeamInfo == NULL)
	{
		return PACKET_EXE_ERROR;
	}
	if (!pTeamInfo->IsLeader())
	{
		return PACKET_EXE_CONTINUE;
	}
	
	GWSetTeamDistribMode* pMsg = (GWSetTeamDistribMode*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_SETTEAMDISTRIBMODE);
	pMsg->setMode(nMode);
	pMsg->setLeaderID(pHuman->GetGUID());
	pMsg->setTeamID(pTeamInfo->GetTeamID());

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGSetTeamDistribModeHandler: nMode=%d success", nMode) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
