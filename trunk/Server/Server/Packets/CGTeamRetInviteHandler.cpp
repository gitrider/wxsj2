

#include "stdafx.h"
#include "CGTeamRetInvite.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamRetInvite.h"
#include "ServerManager.h"



uint CGTeamRetInviteHandler::Execute( CGTeamRetInvite* pPacket, Player* pPlayer )
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
			//Assert( FALSE&&"��ɫ���������������������" ) ;
			return PACKET_EXE_CONTINUE ;
	}

	GWTeamRetInvite* pMsg = (GWTeamRetInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMRETINVITE)) ;
	pMsg->SetReturn( pPacket->GetReturn() ) ;		//���ؽ��
	pMsg->SetSourGUID( pPacket->GetGUID() ) ;		//������
	pMsg->SetDestGUID( pGamePlayer->m_HumanGUID ) ;	//��������

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamRetInviteHandler: Ret=%d sGUID=%X dGUID=%X", 
		pPacket->GetReturn(), pPacket->GetGUID(), pGamePlayer->m_HumanGUID ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
