// CGRegisterTeamRecruitHandler.cpp

#include "stdafx.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"

#include "Obj_Human.h"
#include "HumanDB.h"
#include "Scene.h"
#include "Log.h"
#include "CGRegisterTeamRecruit.h"
#include "GWRegisterTeamRecruit.h"

uint CGRegisterTeamRecruitHandler::Execute( CGRegisterTeamRecruit* pPacket, Player* pPlayer )
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

	// 转发至GLServer
	GWRegisterTeamRecruit* pMsg = (GWRegisterTeamRecruit*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_REGISTER_TEAM_RECRUIT));

	pMsg->SetGUID( pPacket->GetGUID() );
	pMsg->SetType( pPacket->GetType() );
	pMsg->SetLeaderRecruit(*pPacket->GetLeaderRecruit());
	pMsg->SetMemberRecruit(*pPacket->GetMemberRecruit());

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGRegisterTeamRecruitHandler transmit to world: sObjID=%X dGUID=%X, wait get message from world.", 
		pHuman->GetID(), pPacket->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
