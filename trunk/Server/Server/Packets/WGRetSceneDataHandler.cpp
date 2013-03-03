

#include "stdafx.h"
#include "WGRetSceneData.h"
#include "Log.h"
#include "ServerManager.h"
#include "Scene.h"
#include "SceneManager.h"



uint WGRetSceneDataHandler::Execute( WGRetSceneData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		SceneID_t SceneID = pPacket->GetSceneID() ;
		ID_t ClientRes	= pPacket->GetClientRes() ;

		if( ClientRes!=INVALID_ID && SceneID!=INVALID_ID )
		{
			g_pSceneManager->SetSceneResID( SceneID, ClientRes ) ;
		}

		Scene* pScene = g_pSceneManager->GetScene( SceneID )  ;
		if( pScene == NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}

		pScene->m_SceneInitData = *pPacket->GetSceneInitData() ;
		pScene->Init( ) ;
		//֪ͨ���д�����,�����Ѿ��ɹ�����
		pScene->SetSceneStatus( SCENE_STATUS_INIT ) ;
	}
	else
	{
		Assert(FALSE) ;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "WGRetSceneDataHandler: SceneID=%d",
			pPacket->GetSceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
