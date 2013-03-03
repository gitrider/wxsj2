
#include "stdafx.h"
#include "Scene.h"
#include "GamePlayer.h"
#include "SceneManager.h"
#include "PacketFactoryManager.h"
#include "GGSceneNotify.h"
#include "GUIDManager.h"
#include "Obj_Human.h"
#include "GamePlayer.h"
#include "GCNotifyChangeScene.h"
#include "TimeManager.h"
#include "GCExchangeError.h"
#include "ScenePlayerManager.h"
#include "MonsterManager.h"
#include "PetManager.h"
#include "GCCityNotify.h"
#include "ItemBoxManager.h"

VOID Scene::OnSceneInit( )
{
__ENTER_FUNCTION

	UINT uTime = g_pTimeManager->CurrentTime() ;

	if( m_CopyData.m_SourSceneID != INVALID_ID )
	{
		Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(m_CopyData.m_TeamLeader)) ;
		if( pHuman )
		{//���ݸ��������ߵ�GUID�����ô˸����Ķ������Ϣ
			m_CopyData.m_TeamID = pHuman->GetTeamInfo()->GetTeamID() ;
		}
		else
		{
			Assert(FALSE) ;
		}

		Scene* pScene = g_pSceneManager->GetScene(m_CopyData.m_SourSceneID) ;
		if( pScene )
		{
			GGSceneNotify* pMsg = (GGSceneNotify*)(g_pPacketFactoryManager->CreatePacket(PACKET_GG_SCENENOTIFY));
			pMsg->SetSourSceneID( m_CopyData.m_SourSceneID ) ;
			pMsg->SetDestSceneID( SceneID() ) ;

			pScene->SendPacket( pMsg, INVALID_ID_EX ) ;
		}
	}

	if( m_CopyData.m_SceneTimer > 0 )
	{
		BeginSceneTimer( m_CopyData.m_SceneTimer, uTime ) ;
	}

	if(m_SceneInitData.m_nDataType == SCENE_TYPE_CIT)
	{//�˳����ǳ��г��������⴦��
		//������ڳ���������
		GCCityNotify MsgNotifySuccess;
		Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(m_CityData.m_Creator)) ;
		pHuman->GetPlayer()->SendPacket(&MsgNotifySuccess);

		Scene* pScene = g_pSceneManager->GetScene( m_SceneInitData.m_CityData.m_PortSceneID )  ;
		if( pScene == NULL )
		{
			Assert(FALSE);
			return;
		}

		CITYSCENE_DATA* pCitySceneData = pScene->GetCityData();
		if(!pCitySceneData->AddPort(SceneID()))
		{
			Assert(FALSE);
			return;
		}
	}

	m_pLuaInterface->ExeScript_D( SCENE_SCRIPTID,"OnSceneInit", (INT)SceneID() ) ;

__LEAVE_FUNCTION
}

VOID Scene::OnSceneTimer( UINT uTime )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DD( SCENE_SCRIPTID,"OnSceneTimer", (INT)SceneID(), (INT)uTime );

__LEAVE_FUNCTION
}

VOID Scene::OnSceneQuit( )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_D( SCENE_SCRIPTID,"OnSceneQuit", (INT)SceneID() ) ;

	GetScenePlayerManager()->RemoveAllPlayer() ;
	GetMonsterManager()->RemoveAllMonster() ;
	m_pPlatformManager->RemoveAllPlatform() ;
	GetPetManager()->RemoveAllPet() ;
	GetItemBoxManager()->RemoveAllItemBox() ;


__LEAVE_FUNCTION
}

VOID Scene::OnScenePlayerEnter( GamePlayer* pGamePlayer )
{
__ENTER_FUNCTION
	Obj_Human* pHuman = pGamePlayer->GetHuman();
	if(NULL!=pHuman)
	{
		m_pLuaInterface->ExeScript_DD( SCENE_SCRIPTID,"OnScenePlayerEnter", (INT)SceneID(), (INT)pHuman->GetID() );
	}
__LEAVE_FUNCTION
}

VOID Scene::OnSceneHumanLevelUp( Obj_Human* pHuman )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DDD( SCENE_SCRIPTID,"OnSceneHumanLevelUp", (INT)SceneID(), (INT)pHuman->GetID(), (INT)pHuman->GetLevel() );

__LEAVE_FUNCTION
}

VOID Scene::OnSceneHumanDie(Obj_Human* pHuman, ObjID_t idKiller)
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DDD( SCENE_SCRIPTID,"OnSceneHumanDie", (INT)SceneID(), (INT)pHuman->GetID(), (INT)idKiller );

__LEAVE_FUNCTION
}

VOID Scene::OnSceneHumanRelive(Obj_Human* pHuman,INT ReliveType)
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DDD( SCENE_SCRIPTID,"OnSceneHumanRelive", (INT)SceneID(), (INT)pHuman->GetID(), (INT)ReliveType );

__LEAVE_FUNCTION
}

VOID Scene::OnScenePlayerDisconnect( GamePlayer* pGamePlayer )
{
}

VOID Scene::OnScenePlayerLeave( GamePlayer* pGamePlayer ) 
{
__ENTER_FUNCTION

	Obj_Human* pHuman = pGamePlayer->GetHuman();

	//���������������Ĵ��û���Ϣ
	MovePacket( pGamePlayer->PlayerID() ) ;


	//����һ���뿪���������
	if(pHuman->m_ExchangBox.m_ObjID != INVALID_ID)
	{//���ڸ����˽�����
		ObjID_t	TargetID = pHuman->m_ExchangBox.m_ObjID;
		Obj_Human* pTargetHuman = GetHumanManager()->GetHuman( TargetID );
		if( pTargetHuman != NULL )
		{//�Է�û����
			if( pTargetHuman->m_ExchangBox.m_Status > 0
				&& pTargetHuman->m_ExchangBox.m_ObjID == pHuman->GetID()
				)
			{//�Է�Ҳȷʵ���ڽ���״̬,����ͬ�Լ�����
				GCExchangeError Msg;
				Msg.SetID(EXCHANGE_MSG::ERR_DROP);
				pTargetHuman->GetPlayer()->SendPacket(&Msg);
				pTargetHuman->m_ExchangBox.CleanUp();
			}
		}
		pHuman->m_ExchangBox.CleanUp();
	}


	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_CLOSE)
	{
		pHuman->m_StallBox.CleanUp();
	}

__LEAVE_FUNCTION
}

VOID Scene::OnSceneNotify( SceneID_t DestSceneID )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DD( SCENE_SCRIPTID,"OnSceneNotify", (INT)SceneID(), (INT)DestSceneID );
//	m_pLuaInterface->ExeFile_DD( "scene.lua","OnSceneNotify", (INT)SceneID(), (INT)DestSceneID );

__LEAVE_FUNCTION

	return ;

	////��������
	//Scene* pScene = g_pSceneManager->GetScene( DestSceneID ) ;
	//GUID_t TeamLeader = pScene->m_CopyData.m_TeamLeader ;
	//Obj_Human* pHuman = ((Obj_Human*)(g_pGUIDManager->Get(TeamLeader))) ;
	//if( !pHuman )
	//{
	//	Assert(FALSE) ;
	//	return ;
	//}
	//GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer()) ;


	//WORLD_POS New ;
	//New.m_fX = 11.0f ;
	//New.m_fZ = 11.0f ;

	//pHuman->ChangeScene( SceneID(), DestSceneID, New, 9 ) ;

	//GCNotifyChangeScene Msg ;
	//Msg.setCurrentSceneID( SceneID() ) ;
	//Msg.setTargetSceneID( DestSceneID ) ;
	//Msg.setTargetPos( &New ) ;
	//Msg.setTargetDir( 9 ) ;
	//				
	//pGamePlayer->SendPacket( &Msg ) ;
	//��������
}

BOOL Scene::OnAcceptMissionCheck(Obj_Human* pHuman,ScriptID_t idMissionScript)
{
__ENTER_FUNCTION

	BOOL bRet = FALSE ;
	bRet = m_pLuaInterface->ExeScript_DDD(	SCENE_SCRIPTID,
											"OnAcceptMissionCheck", 
											(INT)SceneID(), 
											(INT)pHuman->GetID(),
											idMissionScript ) ;
	
	return bRet ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL	Scene::OnNpcDefaultDialog(Obj_Human* pHuman,ObjID_t idNpc) 
{
	__ENTER_FUNCTION

	BOOL bRet = FALSE;
	bRet = m_pLuaInterface->ExeScript_DDD(	SCENE_SCRIPTID,
											"OnNpcDefaultDialog", 
											(INT)SceneID(), 
											(INT)pHuman->GetID(),
											idNpc);		
		

	__LEAVE_FUNCTION

		return FALSE;

}

BOOL	Scene::OnNpcEventList(Obj_Human* pHuman,ObjID_t idNpc) 
{
	__ENTER_FUNCTION

	BOOL bRet = FALSE;
	bRet = m_pLuaInterface->ExeScript_DDD(	SCENE_SCRIPTID,
		"OnNpcDefaultEventList", 
		(INT)SceneID(), 
		(INT)pHuman->GetID(),
		idNpc);		


	__LEAVE_FUNCTION

		return FALSE;

}


