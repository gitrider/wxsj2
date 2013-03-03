

#include "stdafx.h"
#include "CGTeamRetApply.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamRetApply.h"
#include "ServerManager.h"

uint CGTeamRetApplyHandler::Execute( CGTeamRetApply* pPacket, Player* pPlayer )
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

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	if ( pHuman->GetHP() <= 0 )
	{
			//Assert( FALSE&&"��ɫ���������������" ) ;
			return PACKET_EXE_CONTINUE ;
	}

	GWTeamRetApply* pMsg = (GWTeamRetApply*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMRETAPPLY)) ;
	pMsg->SetReturn( pPacket->GetReturn() ) ;
	pMsg->SetSourGUID( pPacket->GetSourGUID() ) ;
	pMsg->SetLeaderGUID( pGamePlayer->m_HumanGUID ) ;

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamRetApplyHandler: Ret=%d sGUID=%X lGUID=%X", 
		pPacket->GetReturn(), pPacket->GetSourGUID(), pGamePlayer->m_HumanGUID ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
