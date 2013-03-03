#include "stdafx.h"
#include "CGAskTeamDistribMode.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamRetApply.h"
#include "ServerManager.h"
#include "GCRetTeamDistribMode.h"

uint CGAskTeamDistribModeHandler::Execute( CGAskTeamDistribMode* pPacket, Player* pPlayer )
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

	TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
	if (pTeamInfo == NULL)
	{
		return PACKET_EXE_ERROR;
	}

	GCRetTeamDistribMode Msg;
	Msg.setMode(pTeamInfo->GetDistribMode());

	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamDistribMode: success") ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}