

#include "stdafx.h"
#include "DynamicSceneManager.h"
#include "Config.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "SceneManager.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWAskSceneData.h"
#include "GameTable.h"
#include "GUIDManager.h"


DynamicSceneManager* g_pDynamicSceneManager = NULL ;

DynamicSceneManager::DynamicSceneManager( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		m_aLoadNode[i].CleanUp() ;
	}
	m_uHead = 0 ;
	m_uTail = 0 ;
	m_uTotalCount = 0 ;

__LEAVE_FUNCTION
}

DynamicSceneManager::~DynamicSceneManager( )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

BOOL DynamicSceneManager::Init( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

SceneID_t DynamicSceneManager::SelectScene( DYNAMICSCENE_SELECT* pSelect )
{
__ENTER_FUNCTION

	if( pSelect==NULL )
	{
		Assert(FALSE) ;
		return INVALID_ID ;
	}

	Scene* pScene = NULL ;

	Scene* aTemp[MAX_SCENE];
	INT nCount=0 ;
	for( INT i=0; i<MAX_SCENE; i++ )
	{
		pScene = g_pSceneManager->GetScene( (SceneID_t)i ) ;
		if( pScene==NULL )
			continue ;

		//ѡ�񳡾�����
		//��SceneInfo.ini��ѡ��Type = 4��
		//�����ļ���=CityInfo.txt�ж�Ӧ��ڳ�����scene�ļ����ĳ���
		if(pSelect->m_SceneDataType == pScene->GetSceneType()&& 
			!tstricmp(g_Config.m_SceneInfo.m_pScene[i].m_szFileName, g_CitySceneTbl.GetszFileNameByPort(pSelect->m_CitySelect.m_PortSceneID)))
		{
			aTemp[nCount] = pScene ;
			nCount ++ ;
		}
	}

	pScene = NULL ;

	m_Lock.Lock() ;
	for(INT i=0; i<nCount; i++ )
	{
		pScene = aTemp[i] ;

		//״̬����
		if( pScene->GetSceneStatus() == SCENE_STATUS_SLEEP )
		{
			pScene->SetSceneStatus( SCENE_STATUS_SELECT ) ;
			break ;
		}
		else
		{
			pScene = NULL ;
		}
	}

	if( pScene==NULL )
	{
		m_Lock.Unlock() ;//����
		return INVALID_ID ;
	}

	//����������װ�ض���
	DynamicSceneManager::LOAD_NODE NewNode;
	NewNode.m_SceneDataType	= pSelect->m_SceneDataType;
	if(NewNode.m_SceneDataType	== SCENE_TYPE_CIT )
	{
		NewNode.m_CityData.m_pScene			=	pScene;
		NewNode.m_CityData.m_PortSceneID	=	pSelect->m_CitySelect.m_PortSceneID;
		NewNode.m_CityData.m_PlayerGuid		=	pSelect->m_CitySelect.m_PlayerGuid;
	}

	BOOL ret = PushNode( NewNode ) ;
	if( ret==FALSE )
	{
		Assert( FALSE ) ;
		m_Lock.Unlock() ;//����
		return INVALID_ID ;
	}


__LEAVE_FUNCTION

	m_Lock.Unlock() ;//����
	return INVALID_ID ;
}

BOOL DynamicSceneManager::PushNode( DynamicSceneManager::LOAD_NODE Node )
{
__ENTER_FUNCTION

	if( m_uTotalCount>=MAX_SCENE )
	{
		return FALSE ;
	}

	m_aLoadNode[m_uHead] = Node;
	m_uHead ++ ;
	if( m_uHead>=MAX_SCENE )
		m_uHead = 0 ;

	m_uTotalCount ++ ;
	Assert( m_uTotalCount<=MAX_SCENE ) ;

	return TRUE ;

__LEAVE_FUNCTION
	
	return FALSE ;
}

DynamicSceneManager::LOAD_NODE DynamicSceneManager::PopNode( )
{
__ENTER_FUNCTION

	DynamicSceneManager::LOAD_NODE node;
	if( m_uTotalCount == 0 )
		return node ;

	DynamicSceneManager::LOAD_NODE Node = m_aLoadNode[m_uTail] ;
	m_uTail ++ ;
	if( m_uTail>=MAX_SCENE )
		m_uTail = 0 ;

	m_uTotalCount -- ;
	Assert( m_uTotalCount<=MAX_SCENE ) ;

	return Node;

__LEAVE_FUNCTION
	
	DynamicSceneManager::LOAD_NODE node;
	return node ;
}

BOOL DynamicSceneManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	Scene* pScene = NULL ;

	m_Lock.Lock() ;
	LOAD_NODE CurNode = PopNode() ;
	m_Lock.Unlock();

	if(CurNode.m_SceneDataType == SCENE_TYPE_CIT)
	{
		pScene = CurNode.m_CityData.m_pScene;
		//����װ�ز���
		pScene->m_SceneInitData.m_nDataType = SCENE_TYPE_CIT;
		pScene->m_SceneInitData.m_CityData.m_PortSceneID	= CurNode.m_CityData.m_PortSceneID;
	}

	if( pScene == NULL )
		return FALSE ;

	//��ȡ������������Ϣ��װ��
	SCENE_LOAD load ;
	if(CurNode.m_SceneDataType == SCENE_TYPE_CIT)
	{
		//δ�깤��:������Ҫ�ĵ�������
		g_pSceneManager->SetLoadData( g_CitySceneTbl.GetszFileNameByPort(CurNode.m_CityData.m_PortSceneID),load ) ;
	}

	//װ��
	pScene->Load( &load ) ;
	pScene->SetSceneStatus( SCENE_STATUS_LOAD ) ;
	pScene->GetCityData()->m_Creator = CurNode.m_CityData.m_PlayerGuid;//��¼���Լ��ĳ�����Ϣ�У���֪ͨ�����߳��н����ɹ���
	Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(CurNode.m_CityData.m_PlayerGuid)) ;
	
	//���������ݷ��������ͳ�������������Ϣ
	GWAskSceneData* pMsg = (GWAskSceneData*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKSCENEDATA));

	GWAskSceneData::CITY_DATA CityData;
	CityData.m_PortSceneID	=	CurNode.m_CityData.m_PortSceneID;
	CityData.m_GuildID		=	pHuman->GetGuildID();
	pMsg->SetType( GWAskSceneData::TYPE_CITY );
	pMsg->SetSceneID( pScene->SceneID() ) ;
	pMsg->SetExtraDataLength( CityData.GetSize() );
	pMsg->SetExtraData( (CHAR*)&CityData );
	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
