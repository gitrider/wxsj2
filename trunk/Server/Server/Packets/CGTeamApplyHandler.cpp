

#include "stdafx.h"
#include "CGTeamApply.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamApply.h"
#include "ServerManager.h"




uint CGTeamApplyHandler::Execute( CGTeamApply* pPacket, Player* pPlayer )
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

	GWTeamApply* pMsg = (GWTeamApply*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMAPPLY)) ;
	pMsg->SetSourGUID( pPacket->GetSourGUID() ) ;//������
	pMsg->SetDestName( pPacket->GetDestName() ) ;//��������

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamApplyHandler: sGUID=%X dName=%s", 
		pPacket->GetSourGUID(), pPacket->GetDestName() ) ;




	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
