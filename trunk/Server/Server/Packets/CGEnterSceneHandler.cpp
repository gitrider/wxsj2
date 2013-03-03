
#include "stdafx.h"
#include "CGEnterScene.h"
#include "GamePlayer.h"
#include "IncomingPlayerManager.h"
#include "PacketFactoryManager.h"
#include "SceneManager.h"
#include "Log.h"
#include "GCEnterScene.h"
#include "Obj_Human.h"
#include "GCOperateResult.h"
#include "ServerManager.h"


uint CGEnterSceneHandler::Execute( CGEnterScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_ENTER )
	{//��ǰ����Ǹս����
	 //��ǰ������ IncomingPlayerManager��ִ��

		//����߳�ִ����Դ�Ƿ���ȷ
		Assert( MyGetCurrentThreadID()==g_pIncomingPlayerManager->m_ThreadID ) ;

		SceneID_t SceneID = pPacket->getSceneID() ;
		Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
		WORLD_POS enterPos = *(pPacket->getEnterPos()) ;
		PlayerID_t PlayerID = pGamePlayer->PlayerID() ;

		//�Ƿ��ĳ���ID
		if( SceneID==INVALID_ID || pScene==NULL )
		{
			GCEnterScene Msg ;
			Msg.setReturn( 2 ) ;
			Msg.setSceneID( SceneID ) ;
			Msg.setResID( g_pSceneManager->GetSceneResID(SceneID) ) ;
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR I: CGEnterScene::INVALID_ID GUID=%X SceneID=%d",
				pGamePlayer->m_HumanGUID, SceneID ) ;

			return PACKET_EXE_CONTINUE ;
		}

		//��ǰ���û��Ȩ�޽���˳������ȼ�����������û���ţ��ȣ�
		INT nRet = pScene->CheckEnter( PlayerID ) ;
		if( nRet==0 )
		{
			GCEnterScene Msg ;
			Msg.setReturn( 1 ) ;
			Msg.setSceneID( SceneID ) ;
			Msg.setResID( g_pSceneManager->GetSceneResID(SceneID) ) ;
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR I: CGEnterScene::pScene->CheckEnter GUID=%X SceneID=%d",
				pGamePlayer->m_HumanGUID, SceneID ) ;

			return PACKET_EXE_CONTINUE ;
		}
		else if( nRet==-1 )
		{//�������ݳ���
			SceneID = pGamePlayer->GetHuman()->GetDB()->GetDBBakScene() ;
			enterPos = *pGamePlayer->GetHuman()->GetDB()->GetDBBakPosition() ;
			pScene = g_pSceneManager->GetScene( SceneID ) ;
			if( pScene==NULL )
			{
				GCEnterScene Msg ;
				Msg.setReturn( 2 ) ;
				Msg.setSceneID( SceneID ) ;
				Msg.setResID( g_pSceneManager->GetSceneResID(SceneID) ) ;
				pGamePlayer->SendPacket( &Msg ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR I: CGEnterScene::INVALID_ID GUID=%X SceneID=%d (%.2f,%.2f)",
					pGamePlayer->m_HumanGUID, SceneID, enterPos.m_fX, enterPos.m_fZ ) ;

				return PACKET_EXE_CONTINUE ;
			}
		}

		//�����Ҫ����ĳ����Ѿ����ڱ���״̬���򷵻ؼ��볡��ʧ����Ϣ
		if( !pScene->IsCanEnter() )
		{
			GCEnterScene Msg ;
			Msg.setReturn( 3 ) ;
			Msg.setSceneID( SceneID ) ;
			Msg.setResID( g_pSceneManager->GetSceneResID(SceneID) ) ;
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR I: CGEnterScene::pScene->IsCanEnter GUID=%X SceneID=%d",
				pGamePlayer->m_HumanGUID, SceneID ) ;

			return PACKET_EXE_CONTINUE ;
		}

		//ɾ������ģ��������
		PlayerID_t pid = pGamePlayer->PlayerID() ;
		BOOL ret = g_pIncomingPlayerManager->DelPlayer( pid ) ;
		if( !ret )
		{
			Assert(FALSE) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR I: CGEnterScene::DelPlayer GUID=%X SceneID=%d PlayerID=%d",
				pGamePlayer->m_HumanGUID, SceneID, pid ) ;
			return PACKET_EXE_ERROR ;
		}

		//��Ŀ�ĳ�������ת����Ϣ
		CGEnterScene* pEnter = (CGEnterScene*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_ENTERSCENE)) ;
		pEnter->setEnterType( pPacket->getEnterType() ) ;
		pEnter->setSceneID( SceneID ) ;
		pEnter->setEnterPos( enterPos ) ;


		//�����״̬����ΪPS_SERVER_READY_TO_ENTER
		pGamePlayer->SetPlayerStatus( PS_SERVER_READY_TO_ENTER ) ;


		pScene->SendPacket( pEnter, PlayerID ) ;
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEnterScene I GUID=%X To:%d Pos=(%.2f,%.2f)...Ready ",
			pGamePlayer->m_HumanGUID, SceneID, enterPos.m_fX, enterPos.m_fZ ) ;

		//**ע��**
		//���뷵��PACKET_EXE_BREAK ��
		return PACKET_EXE_BREAK ;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_READY_TO_ENTER )
	{//�����յ���Ϣ���������
	 //��ǰ������ pScene�߳���ִ��
	 //�յ�ԭ�������͹�������Ϣ����Ŀ�ĳ���ִ��
	 //����:
 	 //1.��Ӵ���ҵ���������
	 //2.��ҷ���һ����Ϣ

		Obj_Human *pHuman = pGamePlayer->GetHuman();
		if (!pHuman)
		{
			Assert(FALSE) ;
			return PACKET_EXE_ERROR;
		}

		SceneID_t SceneID = pPacket->getSceneID() ;
		Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
		if( pScene==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_ERROR ;
		}

		PlayerID_t PlayerID = pGamePlayer->PlayerID() ;

		//����߳�ִ����Դ�Ƿ���ȷ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		//���ͻ������Ӽ���Ŀ�ĳ�����ҹ�����
		BOOL ret = pScene->GetScenePlayerManager()->AddPlayer( pPlayer ) ;
		if( !ret )
		{//������볡��ʧ�ܣ����ڵ�ǰ����Ѿ���ԭ�ȵĳ��������룬
		 //����ֻ�ܶϿ�����ҵ���������
			SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;
			BOOL boo = pScene->GetScenePlayerManager()->DelPlayerSocket( fd ) ;
			boo = pGamePlayer->ChooseFreeOwn(pScene->GetRecyclePlayerManager()) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR D: CGEnterScene::AddPlayer GUID=%X SceneID=%d Socket=%d",
				pGamePlayer->m_HumanGUID, SceneID, fd ) ;

			return PACKET_EXE_ERROR ;
		}

		WORLD_POS EnterPos = *pPacket->getEnterPos() ;
		ret = pScene->GetMap()->FindEmptyPos( &EnterPos ) ;
		if( ret == FALSE )
		{//���ܽ���һ�������ߵĵط�
			SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;
			BOOL boo = pScene->GetScenePlayerManager()->DelPlayerSocket( fd ) ;
			boo = pScene->GetScenePlayerManager()->DelPlayer( pGamePlayer->PlayerID() ) ;
			boo = pGamePlayer->ChooseFreeOwn(pScene->GetRecyclePlayerManager()) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR D: CGEnterScene::FindEmptyPos GUID=%X SceneID=%d Pos=(%.2f,%.2f)",
				pGamePlayer->m_HumanGUID, SceneID, EnterPos.m_fX, EnterPos.m_fZ ) ;

			return PACKET_EXE_ERROR ;
		}

		//����Ҽ��뵽����
		ret = pScene->ObjectEnterScene(	pGamePlayer->GetHuman() ) ;
		if( !ret )
		{//������볡��ʧ�ܣ����ڵ�ǰ����Ѿ���ԭ�ȵĳ��������룬
		 //���Է��ش���ֵ�Ͽ�����ҵ���������
			SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;
			BOOL boo = pScene->GetScenePlayerManager()->DelPlayerSocket( fd ) ;
			boo = pScene->GetScenePlayerManager()->DelPlayer( pGamePlayer->PlayerID() ) ;
			boo = pGamePlayer->ChooseFreeOwn(pScene->GetRecyclePlayerManager()) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR D: CGEnterScene::pScene->ObjectEnterScene GUID=%X SceneID=%d",
				pGamePlayer->m_HumanGUID, SceneID ) ;

			return PACKET_EXE_ERROR ;
		}
		//������������¼�
		pScene->OnScenePlayerEnter( pGamePlayer ) ;

		// ��һ��Ҫ��
		//��ͻ��˷��ͽ��볡���ɹ�����Ϣ
		GCEnterScene Msg0 ;
		Msg0.setReturn( 0 ) ;
		Msg0.setSceneID( SceneID ) ;
		Msg0.setResID( g_pSceneManager->GetSceneResID(SceneID) ) ;
		Msg0.setEnterPos( EnterPos ) ;
		Msg0.setObjID( pGamePlayer->GetHuman()->GetID() ) ;

		//����ǳ��г�������Ҫ���ͳɳ��м���
		if(pScene->m_SceneInitData.m_nDataType == SCENE_TYPE_CIT)
		{
			Msg0.setIsCity(TRUE);
			Msg0.setCityLevel(pScene->m_SceneInitData.m_CityData.m_Level);
			/*08TL ����
			//INT iCityRes = g_CitySceneTbl.GetResLvByScene(pScene->SceneID(), pScene->m_SceneInitData.m_CityData.m_Level);
			//Msg0.setResID( iCityRes ) ;
			*/
		}

		pGamePlayer->SendPacket( &Msg0 ) ;
		//����ʱ��ͬ������Ϣ

		pHuman->setWorldPos( &EnterPos );
		pHuman->UpdateZone( ) ;
		pHuman->SetActiveFlag( TRUE );
		pHuman->SetChangeSceneFlag( FALSE );

		pHuman->GetDB()->SetDBStartScene( SceneID );
		pHuman->GetDB()->SetDBPosition(*pHuman->getWorldPos());

		//�ɹ����볡���������״̬����Ϊ��PS_SERVER_NORMAL
		pGamePlayer->SetPlayerStatus( PS_SERVER_NORMAL ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEnterScene D GUID=%X To:%d Pos=(%.2f,%.2f)...OK ",
			pGamePlayer->m_HumanGUID, SceneID, EnterPos.m_fX, EnterPos.m_fZ ) ;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_NORMAL ||
			 pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_CHANGESCENE_SCENE	
			 )
	{//�ڳ�����ʱ�յ����볡����˵����Ҫ��ͬһ���������˳�����ת�Ƴ���
	 //��ǰ������ԭ�����߳���ִ��, Ŀ����Ҫ���ԭ�����е��������
	 //��ԭ����ִ��
	 //����:
	 //1.�ڱ���������մ����
	 //2.��Ŀ�ĳ�������һ����Ϣ

		Obj_Human* pHuman = pGamePlayer->GetHuman() ;

		Scene* pScene = pHuman->getScene() ;
		if( pScene==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_ERROR ;
		}

		//����߳�ִ����Դ�Ƿ���ȷ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		SceneID_t DestSceneID = pPacket->getSceneID() ;
		if( DestSceneID==pScene->SceneID() )
		{//ͬһ������
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
		WORLD_POS enterPos =  *(pPacket->getEnterPos()) ;

		Scene* pDestScene = g_pSceneManager->GetScene( DestSceneID ) ;
		if( DestSceneID==INVALID_ID || pDestScene==NULL )
		{//û��Ŀ�ĳ������ݣ����ܳ������д����߳��������ڴ˷������˳���
			GCEnterScene Msg ;
			Msg.setReturn( 2 ) ;
			Msg.setSceneID( DestSceneID ) ;
			Msg.setResID( g_pSceneManager->GetSceneResID(DestSceneID) ) ;
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::invalid dest scene GUID=%X DestScene=%d",
				pGamePlayer->m_HumanGUID, DestSceneID ) ;

			return PACKET_EXE_CONTINUE ;
		}

		PlayerID_t PlayerID = pGamePlayer->PlayerID() ;

		//��ǰ���û��Ȩ�޽���˳������ȼ�����������û���ţ��ȣ�
		INT nRet = pDestScene->CheckEnter( PlayerID ) ;
		if( nRet==0 )
		{
			GCEnterScene Msg ;
			Msg.setReturn( 1 ) ;
			Msg.setSceneID( DestSceneID ) ;
			Msg.setResID( g_pSceneManager->GetSceneResID(DestSceneID) ) ;
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::pScene->CheckEnter Error GUID=%X DestScene=%d",
				pGamePlayer->m_HumanGUID, DestSceneID ) ;
			
			return PACKET_EXE_CONTINUE ;
		}
		else if( nRet==-1 )
		{//����ֱ�ӽ��룬��Ҫ���뱸�ݳ���
			DestSceneID = pHuman->GetDB()->GetDBBakScene() ;
			enterPos = *pHuman->GetDB()->GetDBBakPosition() ;
			pDestScene = g_pSceneManager->GetScene( DestSceneID ) ;
			if( pDestScene==NULL )
			{//û��Ŀ�ĳ������ݣ����ܳ������д����߳��������ڴ˷������˳���
				GCEnterScene Msg ;
				Msg.setReturn( 2 ) ;
				Msg.setSceneID( DestSceneID ) ;
				Msg.setResID( g_pSceneManager->GetSceneResID(DestSceneID) ) ;
				pGamePlayer->SendPacket( &Msg ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::dest scene error GUID=%X DestScene=%d Pos=(%.2f,%.2f)",
					pGamePlayer->m_HumanGUID, DestSceneID, enterPos.m_fX, enterPos.m_fZ ) ;

				return PACKET_EXE_CONTINUE ;
			}
		}

		//�����Ҫ����ĳ����Ѿ����ڱ���״̬���򷵻ؼ��볡��ʧ����Ϣ
		if( !pDestScene->IsCanEnter() )
		{
			GCEnterScene Msg ;
			Msg.setReturn( 3 ) ;
			Msg.setSceneID( DestSceneID ) ;
			Msg.setResID( g_pSceneManager->GetSceneResID(DestSceneID) ) ;			
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::pScene->IsCanEnter Error GUID=%X DestScene=%d Pos=(%.2f,%.2f)",
				pGamePlayer->m_HumanGUID, DestSceneID, enterPos.m_fX, enterPos.m_fZ ) ;

			return PACKET_EXE_CONTINUE ;
		}


		//��һ�������socket����
		SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;
		BOOL ret = pScene->GetScenePlayerManager()->DelPlayerSocket( fd ) ;
		if( !ret )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::DelPlayerSocket GUID=%X DestScene=%d Socket=%d",
				pGamePlayer->m_HumanGUID, DestSceneID, fd ) ;

			return PACKET_EXE_ERROR ;
		}

		//�ڶ�����ɾ����������ģ��������
		PlayerID_t pid = pGamePlayer->PlayerID() ;
		ret = pScene->GetScenePlayerManager()->DelPlayer( pid ) ;
		if( !ret )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::DelPlayer GUID=%X PlayerID=%d",
				pGamePlayer->m_HumanGUID, pid ) ;

			return PACKET_EXE_ERROR ;
		}

		//��������ɾ�������е�����
		pScene->OnScenePlayerLeave( pGamePlayer ) ;
		pScene->ObjectLeaveScene( pHuman ) ;

		//��Ŀ�ĳ�������ת����Ϣ
		CGEnterScene* pEnter = (CGEnterScene*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_ENTERSCENE)) ;
		pEnter->setEnterType( pPacket->getEnterType() ) ;
		pEnter->setSceneID( DestSceneID ) ;
		pEnter->setEnterPos( enterPos ) ;

		//�����״̬����ΪPS_SERVER_READY_TO_ENTER
		pGamePlayer->SetPlayerStatus( PS_SERVER_READY_TO_ENTER ) ;

		//��Ŀ�곡�������첽�ĳ�����Ϣ
		pDestScene->SendPacket( pEnter, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEnterScene S GUID=%X From:%d To:%d Pos=(%.2f,%.2f)...Ready ",
			pGamePlayer->m_HumanGUID, pScene->SceneID(), DestSceneID, enterPos.m_fX, enterPos.m_fZ ) ;

		//**ע��**
		//���뷵��PACKET_EXE_BREAK ��
		return PACKET_EXE_BREAK ;

	}
	else
	{
		Assert(FALSE) ;
	}


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
