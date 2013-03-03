

#include "stdafx.h"
#include "CGTeamKick.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamKick.h"
#include "ServerManager.h"



uint CGTeamKickHandler::Execute( CGTeamKick* pPacket, Player* pPlayer )
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


	GWTeamKick* pMsg = (GWTeamKick*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMKICK)) ;
	pMsg->SetSourGUID( pPacket->GetSourGUID() ) ;//������
	pMsg->SetDestGUID( pPacket->GetDestGUID() ) ;//������

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamKickHandler: sGUID=%X dGUID=%X", 
		pPacket->GetSourGUID(), pPacket->GetDestGUID() ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
