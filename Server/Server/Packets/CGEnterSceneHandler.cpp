
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
	{//当前玩家是刚接入的
	 //当前代码由 IncomingPlayerManager来执行

		//检查线程执行资源是否正确
		Assert( MyGetCurrentThreadID()==g_pIncomingPlayerManager->m_ThreadID ) ;

		SceneID_t SceneID = pPacket->getSceneID() ;
		Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
		WORLD_POS enterPos = *(pPacket->getEnterPos()) ;
		PlayerID_t PlayerID = pGamePlayer->PlayerID() ;

		//非法的场景ID
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

		//当前玩家没有权限进入此场景（等级不够，场景没开放，等）
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
		{//启动备份场景
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

		//如果需要加入的场景已经处于饱和状态，则返回加入场景失败消息
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

		//删除接入模块中数据
		PlayerID_t pid = pGamePlayer->PlayerID() ;
		BOOL ret = g_pIncomingPlayerManager->DelPlayer( pid ) ;
		if( !ret )
		{
			Assert(FALSE) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR I: CGEnterScene::DelPlayer GUID=%X SceneID=%d PlayerID=%d",
				pGamePlayer->m_HumanGUID, SceneID, pid ) ;
			return PACKET_EXE_ERROR ;
		}

		//向目的场景发送转移消息
		CGEnterScene* pEnter = (CGEnterScene*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_ENTERSCENE)) ;
		pEnter->setEnterType( pPacket->getEnterType() ) ;
		pEnter->setSceneID( SceneID ) ;
		pEnter->setEnterPos( enterPos ) ;


		//将玩家状态设置为PS_SERVER_READY_TO_ENTER
		pGamePlayer->SetPlayerStatus( PS_SERVER_READY_TO_ENTER ) ;


		pScene->SendPacket( pEnter, PlayerID ) ;
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEnterScene I GUID=%X To:%d Pos=(%.2f,%.2f)...Ready ",
			pGamePlayer->m_HumanGUID, SceneID, enterPos.m_fX, enterPos.m_fZ ) ;

		//**注意**
		//必须返回PACKET_EXE_BREAK ；
		return PACKET_EXE_BREAK ;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_READY_TO_ENTER )
	{//场景收到消息，处理添加
	 //当前代码由 pScene线程来执行
	 //收到原场景发送过来的消息，由目的场景执行
	 //功能:
 	 //1.添加此玩家到本场景中
	 //2.玩家发送一个消息

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

		//检查线程执行资源是否正确
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		//将客户端连接加入目的场景玩家管理器
		BOOL ret = pScene->GetScenePlayerManager()->AddPlayer( pPlayer ) ;
		if( !ret )
		{//如果加入场景失败，由于当前玩家已经从原先的场景里脱离，
		 //所以只能断开此玩家的网络连接
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
		{//不能进入一个不可走的地方
			SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;
			BOOL boo = pScene->GetScenePlayerManager()->DelPlayerSocket( fd ) ;
			boo = pScene->GetScenePlayerManager()->DelPlayer( pGamePlayer->PlayerID() ) ;
			boo = pGamePlayer->ChooseFreeOwn(pScene->GetRecyclePlayerManager()) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR D: CGEnterScene::FindEmptyPos GUID=%X SceneID=%d Pos=(%.2f,%.2f)",
				pGamePlayer->m_HumanGUID, SceneID, EnterPos.m_fX, EnterPos.m_fZ ) ;

			return PACKET_EXE_ERROR ;
		}

		//将玩家加入到场景
		ret = pScene->ObjectEnterScene(	pGamePlayer->GetHuman() ) ;
		if( !ret )
		{//如果加入场景失败，由于当前玩家已经从原先的场景里脱离，
		 //所以返回错误值断开此玩家的网络连接
			SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;
			BOOL boo = pScene->GetScenePlayerManager()->DelPlayerSocket( fd ) ;
			boo = pScene->GetScenePlayerManager()->DelPlayer( pGamePlayer->PlayerID() ) ;
			boo = pGamePlayer->ChooseFreeOwn(pScene->GetRecyclePlayerManager()) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR D: CGEnterScene::pScene->ObjectEnterScene GUID=%X SceneID=%d",
				pGamePlayer->m_HumanGUID, SceneID ) ;

			return PACKET_EXE_ERROR ;
		}
		//场景进入玩家事件
		pScene->OnScenePlayerEnter( pGamePlayer ) ;

		// 下一版要改
		//向客户端发送进入场景成功的消息
		GCEnterScene Msg0 ;
		Msg0.setReturn( 0 ) ;
		Msg0.setSceneID( SceneID ) ;
		Msg0.setResID( g_pSceneManager->GetSceneResID(SceneID) ) ;
		Msg0.setEnterPos( EnterPos ) ;
		Msg0.setObjID( pGamePlayer->GetHuman()->GetID() ) ;

		//如果是城市场景，还要发送成城市级别
		if(pScene->m_SceneInitData.m_nDataType == SCENE_TYPE_CIT)
		{
			Msg0.setIsCity(TRUE);
			Msg0.setCityLevel(pScene->m_SceneInitData.m_CityData.m_Level);
			/*08TL 方案
			//INT iCityRes = g_CitySceneTbl.GetResLvByScene(pScene->SceneID(), pScene->m_SceneInitData.m_CityData.m_Level);
			//Msg0.setResID( iCityRes ) ;
			*/
		}

		pGamePlayer->SendPacket( &Msg0 ) ;
		//发送时间同步的消息

		pHuman->setWorldPos( &EnterPos );
		pHuman->UpdateZone( ) ;
		pHuman->SetActiveFlag( TRUE );
		pHuman->SetChangeSceneFlag( FALSE );

		pHuman->GetDB()->SetDBStartScene( SceneID );
		pHuman->GetDB()->SetDBPosition(*pHuman->getWorldPos());

		//成功进入场景，将玩家状态设置为：PS_SERVER_NORMAL
		pGamePlayer->SetPlayerStatus( PS_SERVER_NORMAL ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEnterScene D GUID=%X To:%d Pos=(%.2f,%.2f)...OK ",
			pGamePlayer->m_HumanGUID, SceneID, EnterPos.m_fX, EnterPos.m_fZ ) ;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_NORMAL ||
			 pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_CHANGESCENE_SCENE	
			 )
	{//在场景中时收到进入场景，说明是要在同一个服务器端程序内转移场景
	 //当前代码由原场景线程来执行, 目的是要清除原场景中的玩家数据
	 //由原场景执行
	 //功能:
	 //1.在本场景中清空此玩家
	 //2.向目的场景发送一个消息

		Obj_Human* pHuman = pGamePlayer->GetHuman() ;

		Scene* pScene = pHuman->getScene() ;
		if( pScene==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_ERROR ;
		}

		//检查线程执行资源是否正确
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		SceneID_t DestSceneID = pPacket->getSceneID() ;
		if( DestSceneID==pScene->SceneID() )
		{//同一个场景
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
		WORLD_POS enterPos =  *(pPacket->getEnterPos()) ;

		Scene* pDestScene = g_pSceneManager->GetScene( DestSceneID ) ;
		if( DestSceneID==INVALID_ID || pDestScene==NULL )
		{//没有目的场景数据，可能场景号有错，或者场景不是在此服务器端程序
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

		//当前玩家没有权限进入此场景（等级不够，场景没开放，等）
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
		{//不能直接进入，需要进入备份场景
			DestSceneID = pHuman->GetDB()->GetDBBakScene() ;
			enterPos = *pHuman->GetDB()->GetDBBakPosition() ;
			pDestScene = g_pSceneManager->GetScene( DestSceneID ) ;
			if( pDestScene==NULL )
			{//没有目的场景数据，可能场景号有错，或者场景不是在此服务器端程序
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

		//如果需要加入的场景已经处于饱和状态，则返回加入场景失败消息
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


		//第一步：清除socket数据
		SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;
		BOOL ret = pScene->GetScenePlayerManager()->DelPlayerSocket( fd ) ;
		if( !ret )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::DelPlayerSocket GUID=%X DestScene=%d Socket=%d",
				pGamePlayer->m_HumanGUID, DestSceneID, fd ) ;

			return PACKET_EXE_ERROR ;
		}

		//第二步：删除场景接入模块中数据
		PlayerID_t pid = pGamePlayer->PlayerID() ;
		ret = pScene->GetScenePlayerManager()->DelPlayer( pid ) ;
		if( !ret )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR S: CGEnterScene::DelPlayer GUID=%X PlayerID=%d",
				pGamePlayer->m_HumanGUID, pid ) ;

			return PACKET_EXE_ERROR ;
		}

		//第三步：删除场景中的数据
		pScene->OnScenePlayerLeave( pGamePlayer ) ;
		pScene->ObjectLeaveScene( pHuman ) ;

		//向目的场景发送转移消息
		CGEnterScene* pEnter = (CGEnterScene*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_ENTERSCENE)) ;
		pEnter->setEnterType( pPacket->getEnterType() ) ;
		pEnter->setSceneID( DestSceneID ) ;
		pEnter->setEnterPos( enterPos ) ;

		//将玩家状态设置为PS_SERVER_READY_TO_ENTER
		pGamePlayer->SetPlayerStatus( PS_SERVER_READY_TO_ENTER ) ;

		//向目标场景发送异步的场景消息
		pDestScene->SendPacket( pEnter, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEnterScene S GUID=%X From:%d To:%d Pos=(%.2f,%.2f)...Ready ",
			pGamePlayer->m_HumanGUID, pScene->SceneID(), DestSceneID, enterPos.m_fX, enterPos.m_fZ ) ;

		//**注意**
		//必须返回PACKET_EXE_BREAK ；
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
