

#include "stdafx.h"
#include "WGRetChangeScene.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCRetChangeScene.h"
#include "SceneManager.h"




uint WGRetChangeSceneHandler::Execute( WGRetChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	Assert( pScene ) ;

	if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_CHANGESCENE )
	{//�������յ��������������������
		//����߳�ִ����Դ�Ƿ���ȷ
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_CHANGESCENE_SCENE );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetChangeSceneHandler::Execute(ret=%d) GUID=%X ServerPlayer...OK ",
			pPacket->GetReturn(), pGamePlayer->m_HumanGUID );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_CHANGESCENE_SCENE )
	{//�����յ�Cache�����Ϣ
		//����߳�ִ����Դ�Ƿ���ȷ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCRetChangeScene Msg;

		Msg.SetKey( pPacket->GetKey() ) ;

		switch( pPacket->GetReturn() )
		{
		case WGRetChangeScene::CSR_SUCCESS :
			{
				Msg.SetReturn( GCRetChangeScene::CSR_SUCCESS );
			}
			break;
		case WGRetChangeScene::CSR_SUCCESS_DIFFSERVER :
			{
				Msg.SetReturn( GCRetChangeScene::CSR_SUCCESS_DIFFSERVER );

				SceneID_t SceneID = pPacket->GetSceneID() ;
				Assert( SceneID!=INVALID_ID );
				ID_t ServerID = g_pSceneManager->GetSceneInfo(SceneID)->m_ServerID;

				_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );

				Msg.SetIP( pData->m_IP0 );
				Msg.SetPort( pData->m_Port0 );

				//�ȴ��ͻ��˶Ͽ�����
				pGamePlayer->SetDirty( TRUE );

				g_pLog->FastSaveLog( LOG_FILE_1, "WGRetChangeSceneHandler::Diff Server(GUID=%X SceneID=%d IP=%s Port=%d)",
					pGamePlayer->m_HumanGUID, SceneID, pData->m_IP0, pData->m_Port0 );
			}
			break;
		case WGRetChangeScene::CSR_ERROR :
			{
				Msg.SetReturn( GCRetChangeScene::CSR_ERROR );

				//ZoneID_t zid = pScene->CalcZoneID( pHuman->getWorldPos() );
				//Assert( zid!=INVALID_ID );
				//pScene->Register( pHuman, zid );
				pHuman->SetActiveFlag( TRUE );
				pHuman->SetChangeSceneFlag( FALSE );

				pGamePlayer->SetPlayerStatus( PS_SERVER_NORMAL );
			}
			break;
		default :
			{
				Assert(FALSE);
			}
			break;
		};

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetChangeSceneHandler::Execute(GUID=%X player=%d,ret=%d) Scene..OK ",
			pGamePlayer->m_HumanGUID, PlayerID, pPacket->GetReturn() );

		return PACKET_EXE_CONTINUE;
	}
	else
	{
		Assert(FALSE);
	}

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
