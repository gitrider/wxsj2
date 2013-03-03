

#include "stdafx.h"
#include "CGTeamInvite.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "GWTeamInvite.h"



uint CGTeamInviteHandler::Execute( CGTeamInvite* pPacket, Player* pPlayer )
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

	if ( pHuman->GetHP() <= 0 )
	{
			//Assert( FALSE&&"��ɫ�������������������" ) ;
			return PACKET_EXE_CONTINUE ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	GWTeamInvite* pMsg = (GWTeamInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMINVITE)) ;
	pMsg->SetSourGUID( pHuman->GetGUID() ) ;//������
	pMsg->SetDestName( pPacket->GetDestName() ) ;//��������

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamInviteHandler: sGUID=%X dGUID=%s", 
		pPacket->GetSourObjID(), pPacket->GetDestName() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
