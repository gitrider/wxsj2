

#include "stdafx.h"
#include "CGChannelInvite.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "GWChannelInvite.h"





uint CGChannelInviteHandler::Execute( CGChannelInvite* pPacket, Player* pPlayer )
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

	GWChannelInvite* pMsg = (GWChannelInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHANNELINVITE)) ;
	pMsg->SetSourGUID( pPacket->GetSourGUID() ) ;//������
	pMsg->SetDestGUID( pPacket->GetDestGUID() ) ;//��������

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGChannelInviteHandler: sGUID=%X dGUID=%X", 
		pPacket->GetSourGUID(), pPacket->GetDestGUID() ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
