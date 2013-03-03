

#include "stdafx.h"
#include "CopySceneManager.h"
#include "Config.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "SceneManager.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWAskSceneData.h"



CopySceneManager* g_pCopySceneManager = NULL ;

CopySceneManager::CopySceneManager( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		m_aLoadNode[i].CleanUp() ;
		m_aTotalCopyScene[i] = NULL ;
	}
	m_uHead = 0 ;
	m_uTail = 0 ;
	m_uTotalCount = 0 ;

__LEAVE_FUNCTION
}

CopySceneManager::~CopySceneManager( )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

BOOL CopySceneManager::Init( )
{
__ENTER_FUNCTION



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

SceneID_t CopySceneManager::SelectScene( COPYSCENE_SELECT* pSelect )
{
__ENTER_FUNCTION

	if( pSelect==NULL )
	{
		Assert(FALSE) ;
		return INVALID_ID ;
	}

	Scene* pScene = NULL ;

	//判断副本创建者是否已经在创建副本
	Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(pSelect->m_CopyData.m_TeamLeader)) ;
	if( pHuman==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "SelectScene::GUID=%X this human is null",pSelect->m_CopyData.m_TeamLeader );
		return INVALID_ID ;
	}
	Scene* pHumanScene = pHuman->getScene() ;
	if( pHumanScene==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "SelectScene::GUID=%X this human is not at scene",pSelect->m_CopyData.m_TeamLeader );
		return INVALID_ID ;
	}
	Assert( MyGetCurrentThreadID()==pHumanScene->m_ThreadID ) ;
	if( pHuman->m_CreateCopySceneID != INVALID_ID )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "SelectScene::GUID=%X SceneID=%d this human is creating copy scene",pSelect->m_CopyData.m_TeamLeader, pHuman->m_CreateCopySceneID );
		return 0 ;//如果返回INVALID_ID，则脚本会当成创建失败而向客户端发送回应
	}

	///////////////////////////////////
	SceneID_t retSceneID = INVALID_ID ;
	{
		AutoLock_T autolock(m_Lock);
		for(INT i=0; i<MAX_SCENE; i++ )
		{
			if( m_aTotalCopyScene[i]==NULL )
				continue ;

			//状态处理
			if( m_aTotalCopyScene[i]->GetSceneStatus() == SCENE_STATUS_SLEEP )
			{
				m_aTotalCopyScene[i]->SetSceneStatus( SCENE_STATUS_SELECT ) ;
				pScene = m_aTotalCopyScene[i] ;
			}
			else
			{
				continue ;
			}

			//设置装载参数
			pScene->m_SceneLoad = pSelect->m_Load ;
			pScene->m_SceneInitData.m_nDataType = pSelect->m_SceneDataType ;
			pScene->m_CopyData = pSelect->m_CopyData ;
			pScene->SetMapName(pSelect->m_Load.m_szMap);
			//if( g_Command_Print0 ) printf( "Select Success! SceneID=%d\n", pScene->SceneID()) ;

			break ;
		}

		if( pScene==NULL )
		{
			//if( g_Command_Print0 ) printf( "pScene==NULL\n") ;
			return INVALID_ID ;
		}

		retSceneID = pScene->SceneID() ;

		//将场景放入装载队列
		BOOL ret = PushScene( pScene ) ;
		if( ret==FALSE )
		{
			//if( g_Command_Print0 ) printf( "PushScene ret=FALSE\n") ;
			return INVALID_ID ;
		}

		//if( g_Command_Print0 ) printf( "PushScene Success! SceneID=%d\n", retSceneID ) ;
	}while(FALSE);

	pHuman->m_CreateCopySceneID = retSceneID ;

	return retSceneID ;

__LEAVE_FUNCTION

	return INVALID_ID ;
}

BOOL CopySceneManager::PushScene( Scene* pScene )
{
__ENTER_FUNCTION

	if( m_uTotalCount>=MAX_SCENE )
	{
		return FALSE ;
	}

	m_aLoadNode[m_uHead].m_pScene = pScene ;
	m_uHead ++ ;
	if( m_uHead>=MAX_SCENE )
		m_uHead = 0 ;

	m_uTotalCount ++ ;
	Assert( m_uTotalCount<=MAX_SCENE ) ;

	return TRUE ;

__LEAVE_FUNCTION
	
	return FALSE ;
}

Scene* CopySceneManager::PopScene( )
{
__ENTER_FUNCTION

	if( m_uTotalCount == 0 )
		return NULL ;

	Scene* pScene = m_aLoadNode[m_uTail].m_pScene ;
	m_uTail ++ ;
	if( m_uTail>=MAX_SCENE )
		m_uTail = 0 ;

	m_uTotalCount -- ;
	Assert( m_uTotalCount<=MAX_SCENE ) ;

	return pScene ;

__LEAVE_FUNCTION
	
	return NULL ;
}

BOOL CopySceneManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	Scene* pScene = NULL ;

	{
		AutoLock_T autolock(m_Lock);
		pScene = PopScene() ;
	}while(FALSE);

	if( pScene == NULL )
		return FALSE ;

	//读取场景的配置信息并装载
	SCENE_FILE* pSceneFile = g_pSceneManager->FindSceneFileByNavName( pScene->GetMapName() ) ;
	if( pSceneFile==NULL )
		return FALSE ;

	SCENE_LOAD load = pSceneFile->m_Load ;

	if( strlen(pScene->m_SceneLoad.m_szMonster)>4 )
	{
		strncpy( load.m_szMonster, pScene->m_SceneLoad.m_szMonster, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szArea)>4 )
	{
		strncpy( load.m_szArea, pScene->m_SceneLoad.m_szArea, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szPet)>4 )
	{
		strncpy( load.m_szPet, pScene->m_SceneLoad.m_szPet, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szPlatform)>4 )
	{
		strncpy( load.m_szPlatform, pScene->m_SceneLoad.m_szPlatform, _MAX_PATH-1 ) ;
		load.m_IsPlatformExist = TRUE ;
	}
	if( strlen(pScene->m_SceneLoad.m_szPatrolPointData)>4 )
	{
		strncpy( load.m_szPatrolPointData, pScene->m_SceneLoad.m_szPatrolPointData, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szGrowPointData)>4 )
	{
		strncpy( load.m_szGrowPointData, pScene->m_SceneLoad.m_szGrowPointData, _MAX_PATH-1 ) ;
		if( strlen(pScene->m_SceneLoad.m_szGrowPointSetup)<=4 )
		{
			AssertEx(FALSE,"GrowPointSetup数据设置错误") ;
		}
		strncpy( load.m_szGrowPointSetup, pScene->m_SceneLoad.m_szGrowPointSetup, _MAX_PATH-1 ) ;
		load.m_IsGrowPointExist = TRUE ;
	}
	if( strlen(pScene->m_SceneLoad.m_szBus)>4 )
	{
		strncpy( load.m_szBus, pScene->m_SceneLoad.m_szBus, sizeof(load.m_szBus) ) ;
		load.m_szBus[sizeof(load.m_szBus)-1] ='\0';
		load.m_IsBusExist = TRUE ;
	}

	pScene->Load( &load ) ;

	ID_t ClientRes = pScene->m_ClientRes ;
	if( ClientRes==INVALID_ID )
	{
		pScene->SetSceneStatus( SCENE_STATUS_SLEEP ) ;
		return FALSE ;
	}
	
	pScene->SetSceneStatus( SCENE_STATUS_LOAD ) ;
	
	//向世界数据服务器发送场景数据请求消息
	GWAskSceneData* pMsg = (GWAskSceneData*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKSCENEDATA));
	pMsg->SetSceneID( pScene->SceneID() ) ;
	pMsg->SetClientRes( ClientRes ) ;
	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CopySceneManager::AddCopyScene( SceneID_t SceneID, Scene* pScene )
{
__ENTER_FUNCTION

	if( SceneID<0 || SceneID>=MAX_SCENE )
		return FALSE ;

	Assert( m_aTotalCopyScene[SceneID]==NULL ) ;
	m_aTotalCopyScene[SceneID] = pScene ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

