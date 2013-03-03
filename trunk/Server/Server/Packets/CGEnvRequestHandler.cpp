

#include "stdafx.h"
#include "CGEnvRequest.h"
#include "SceneManager.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "PlayerPool.h"
#include "Log.h"
#include "GCWorldTime.h"
#include "TimeManager.h"
#include "GCTopList.h"
#include "PacketFactoryManager.h"
#include "Server.h"
#include "GameTable.h"
#include "Obj_Monster.h"
#include "MonsterManager.h"

uint CGEnvRequestHandler::Execute( CGEnvRequest* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	////���ڼ������λ����Χ�������Ϣ��������Ϣ����Ʒ��Ϣ��ȡ����
	////Ȼ���͸����

	//BOOL ret = pScene->ScanEnv( pHuman ) ;
	//Assert( ret ) ;

	//Assert( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITTING_FOR_ENVREQUEST ||
	//		pGamePlayer->GetPlayerStatus()==PS_SERVER_NORMAL ) ;
	//pGamePlayer->SetPlayerStatus( PS_SERVER_NORMAL ) ;

	//��ϵͳʱ�䷢�͸����
	GCWorldTime	Msg;
	Msg.SetWorldTime(g_WorldTime);
	Msg.SetSystemTime(g_pTimeManager->GetANSITime());
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGEnvRequest::GUID=%X OK",
		pHuman->GetGUID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
