/*
Server���Ƚ�ִ��Ȩת�������̣߳�
�����߳�ǿ�н��˳�������������Ƶ��˳��е���ڳ����У�����ʱ��������ô�࣬ǿ���ƶ���
�����ڳ�����CityPortList��ָ���Լ�����
����Լ���SCENE_INT_DATA,
����m_DynamicSceneQuitTimer,�ȴ��ر�,�߼����Զ��ѳ������ó�˯��״̬
������Ϣ16, GWCityCloseSuccess��World

*/

#include "stdafx.h"
#include "WGCityClose.h"
#include "PlayerPool.h"
#include "Log.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GamePlayer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServerManager.h"
#include "GWCityCloseSuccess.h"
#include "PacketFactoryManager.h"

uint WGCityCloseHandler::Execute( WGCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		PlayerID_t	PlayerID	= pPacket->GetPlayerID();
		SceneID_t	SceneID		= pPacket->GetSceneID();
		GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
		if( pGamePlayer==NULL ) 
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		Obj_Human* pHuman = pGamePlayer->GetHuman();
		Assert( pHuman );

		Scene*	pScene		= pHuman->getScene();
		Scene*	pCityScene	= g_pSceneManager->GetScene( SceneID ) ;

		if( pScene==NULL ) return PACKET_EXE_CONTINUE;

		if( pPlayer->IsServerPlayer() )
		{//�������յ��������������������
			Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
			pCityScene->SendPacket(pPacket,PlayerID);
			return PACKET_EXE_NOTREMOVE ;
		}
		else if( pPlayer->IsGamePlayer() )
		{
			Assert( MyGetCurrentThreadID()==pCityScene->m_ThreadID ) ;

			//1.�����������������
			ScenePlayerManager* pCurScenePlayerManager = pCityScene->GetScenePlayerManager();
			PlayerID_t PlayerInTheCity[MAX_PLAYER];
			UINT PlayerNum = 0;
			for(UINT i =0 ;i<pCurScenePlayerManager->GetPlayerNumber();i++ )
			{
				PlayerInTheCity[PlayerNum++] = pCurScenePlayerManager->GetPlayers()[i];
			}

			SceneID_t	PortSceneID = pCityScene->m_SceneInitData.m_CityData.m_PortSceneID;
			Scene*		pPortScene	= g_pSceneManager->GetScene( PortSceneID ) ;
			WORLD_POS pos(150,100);
			for(UINT i =0 ;i<PlayerNum;i++ )
			{
				GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerInTheCity[i]);
				if( pGamePlayer==NULL )
				{
					Assert(FALSE);
					return PACKET_EXE_CONTINUE;
				}

				Obj_Human* pHuman = pGamePlayer->GetHuman();

				pHuman->ChangeScene( SceneID, PortSceneID, pos, 9 ) ;
			}

			//2.�����ڳ�����CityPortList��ָ���Լ�����
			if(!pPortScene->m_CityData.RemovePort(SceneID))
			{
				Assert(FALSE);
				return PACKET_EXE_CONTINUE;
			}

			//3.����Լ���SCENE_INT_DATA
			_CITY_GUID CityGuid = pCityScene->m_SceneInitData.m_CityData.m_Guid;
			pCityScene->m_SceneInitData.m_CityData.CleanUp(); 

			//֪ͨworld�����Ѿ�ɾ�����
			GWCityCloseSuccess* pMsgToWorld = (GWCityCloseSuccess*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_CITYCLOSESUCCESS);
			pMsgToWorld->SetCityGuid(CityGuid);
			g_pServerManager->SendPacket( pMsgToWorld, INVALID_ID ) ;
		}


		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
