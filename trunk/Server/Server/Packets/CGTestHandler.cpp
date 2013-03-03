

#include "stdafx.h"
#include "CGTest.h"
#include "GamePlayer.h"
#include "Log.h"
#include "SceneManager.h"
#include "Server.h"
#include "Config.h"
#include "IncomingPlayerManager.h"
#include "PacketFactoryManager.h"
#include "GCTest.h"
#include "Obj_ItemBox.h"
#include "ItemTypes.h"
//#include  "GCNewItemBox.h"
#include "GCEnterScene.h"
#include "TimeManager.h"
#include "Obj_Monster.h"
#include "AI_Character.h"



uint CGTestHandler::Execute( CGTest* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	Obj_Human* pHuman = ((GamePlayer*)pPlayer)->GetHuman() ;

	switch( pPacket->GetType() )
	{
	case CGTest::TYPE_CG_ASK_ENTER_SCENE:
		{//�ͻ��˷���������Ϣ������֮

			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
			PlayerID_t PlayerID = pPlayer->PlayerID( ) ;

			//�����һ������
			INT index = rand()%g_Config.m_SceneInfo.m_SceneCount ;
			SceneID_t SceneID = g_Config.m_SceneInfo.m_pScene[index].m_SceneID ;
			Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
			if( pScene==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_ERROR ;
			}

			//ɾ������ģ��������
			BOOL ret = g_pIncomingPlayerManager->DelPlayer( pGamePlayer->PlayerID() ) ;
			if( !ret )
			{
				Assert(FALSE) ;
				return PACKET_EXE_ERROR ;
			}

			//��Ŀ�ĳ�������ת����Ϣ
			CGTest* pTest = (CGTest*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_TEST)) ;
			pTest->SetType( CGTest::TYPE_CG_DO_ENTER_SCENE ) ;
			pTest->GetTest()->SceneID = SceneID ;
			pScene->SendPacket( pTest, PlayerID ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "CGTest::TYPE_CG_ASK_ENTER_SCENE PID:%d To:%d...OK ",
				PlayerID, SceneID ) ;

			return PACKET_EXE_BREAK ;
		}
		break ;
	case CGTest::TYPE_CG_DO_ENTER_SCENE:
		{//���յ��������������Ϣ������֮

			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
			SceneID_t SceneID = pPacket->GetTest()->SceneID ;

			//ȡ��Ŀ�ĳ���
			Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
			if( pScene==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_ERROR ;
			}

			//���ͻ��˼���Ŀ�ĳ���
			BOOL ret = pScene->GetScenePlayerManager()->AddPlayer( pPlayer ) ;
			if( !ret )
			{//������볡��ʧ�ܣ����ڵ�ǰ����Ѿ���ԭ�ȵĳ��������룬
			 //����ֻ�ܶϿ�����ҵ���������
				
				BOOL boo = pGamePlayer->ChooseFreeOwn(pScene->GetRecyclePlayerManager()) ;
				Assert( boo ) ;
				return PACKET_EXE_ERROR ;
			}

			GCTest Msg ;
			Msg.SetType( GCTest::TYPE_GC_ENTER ) ;
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "CGTest::TYPE_CG_DO_ENTER_SCENE To:%d...OK ",
				SceneID ) ;
		}
		break ;
	case CGTest::TYPE_CG_ASK_CHANGE_SCENE :
		{
			if( pPlayer==NULL )
				return FALSE ;

			SceneID_t SceneID = pPacket->GetTest()->SceneID ;
			PlayerID_t PlayerID = pPlayer->PlayerID( ) ;

			if( SceneID == INVALID_ID )
			{//�����һ������
				INT index = rand()%g_Config.m_SceneInfo.m_SceneCount ;
				SceneID = g_Config.m_SceneInfo.m_pScene[index].m_SceneID ;
			}

			Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
			if( pScene==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_ERROR ;
			}

			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
			SceneID_t CurSceneID = pHuman->getScene()->SceneID() ;
			if( CurSceneID == INVALID_ID )
			{//???
				Log::SaveLog( SERVER_ERRORFILE, "CGTest::TYPE_CG_ASK_CHANGE_SCENE" ) ;
				Log::SaveLog( SERVER_ERRORFILE, "\tPlayer=Status:%d SOCKET:%d PlayerID:%d IsEmpty:%d PlayerManagerID:%d",
					pGamePlayer->GetPlayerStatus(),
					pGamePlayer->GetSocket()->getSOCKET(),
					pGamePlayer->PlayerID(),
					pGamePlayer->IsEmpty(),
					pGamePlayer->PlayerManagerID()) ;
				Log::SaveLog( SERVER_ERRORFILE, "\tPacket=Status:%d SOCKET:%d SceneID:%d Valid:%d PlayerID:%d PlayerManagerID:%d",
					pPacket->GetTest()->Status,
					pPacket->GetTest()->s,
					pPacket->GetTest()->SceneID,
					pPacket->GetTest()->Valid,
					pPacket->GetTest()->PlayerID,
					pPacket->GetTest()->PlayerManagerID) ;
				return PACKET_EXE_ERROR ;
			}

			if( CurSceneID == SceneID )
			{//��ͬ����������Ҫת��
				return PACKET_EXE_CONTINUE ;
			}

			Scene* pCurScene = g_pSceneManager->GetScene( CurSceneID ) ;
			if( pCurScene==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_ERROR ;
			}

			ScenePlayerManager* pSPM = pCurScene->GetScenePlayerManager() ;
			pSPM->DelPlayer( pGamePlayer->PlayerID() ) ;

			pCurScene->ObjectLeaveScene( pGamePlayer->GetHuman() ) ;


			CGTest* pTest = (CGTest*)(g_pPacketFactoryManager->CreatePacket(PACKET_CG_TEST)) ;
			pTest->SetType( CGTest::TYPE_CG_DO_CHANGE_SCENE ) ;
			pTest->GetTest()->SceneID = SceneID ;
			pScene->SendPacket( pTest, PlayerID ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "CGTest::TYPE_CG_ASK_CHANGE_SCENE PID:%d From:%d To:%d...OK ",
				PlayerID, CurSceneID, SceneID ) ;

			return PACKET_EXE_BREAK ;
		}
		break ;
	case CGTest::TYPE_CG_DO_CHANGE_SCENE:
		{
			if( pPlayer==NULL )
				return PACKET_EXE_ERROR ;

			SceneID_t SceneID = pPacket->GetTest()->SceneID ;
			PlayerID_t PlayerID = pPlayer->PlayerID( ) ;
			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

			Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
			ScenePlayerManager* pSPM = pScene->GetScenePlayerManager() ;
			BOOL ret = pSPM->AddPlayer( pGamePlayer ) ;
			if( !ret )
			{
				RecyclePlayerManager* pRPM = pScene->GetRecyclePlayerManager();

				ret = pGamePlayer->ChooseFreeOwn(pRPM) ;
				Assert( ret ) ;
				return PACKET_EXE_ERROR ;
			}

			pScene->ObjectEnterScene( pGamePlayer->GetHuman() );
			//pScene->HumanEnterScene(	pGamePlayer->GetHuman(), 
			//							0,
			//							NULL ) ;
			pGamePlayer->GetHuman()->SetActiveFlag( TRUE );
			pGamePlayer->GetHuman()->SetChangeSceneFlag( FALSE );

			Obj_Human *pHuman = pGamePlayer->GetHuman();
			pHuman->GetDB()->SetDBStartScene(pScene->SceneID());

			GCEnterScene Msg0 ;
			Msg0.setReturn( 0 ) ;
			Msg0.setSceneID( pScene->SceneID( ) ) ;
			Msg0.setEnterPos( *pHuman->getWorldPos() ) ;
			Msg0.setObjID( pHuman->GetID() ) ;

			pHuman->GetPlayer()->SendPacket( &Msg0 ) ;

			GCTest Msg ;
			Msg.SetType( GCTest::TYPE_GC_CHANGE ) ;
			pGamePlayer->SendPacket( &Msg ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "CGTest::TYPE_CG_DO_CHANGE_SCENE PID:%d To:%d...OK ", PlayerID, SceneID ) ;
		}
		break ;
	case CGTest::TYPE_CG_SHUTDOWN_SERVER:
		{
			UINT Now = g_pTimeManager->CurrentTime() ;
			g_Server.m_TimeToQuit.BeginTimer( 5000, Now ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGTest::TYPE_CG_SHUTDOWN_SERVER ...OK " ) ;
		}
		break ;
	case CGTest::TYPE_CG_SAY_ALLSCENE:
		{
			if( pPlayer==NULL )
				return PACKET_EXE_ERROR ;

			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

			SceneID_t SceneID = pHuman->getScene()->SceneID() ;
			if( SceneID == INVALID_ID )
				return PACKET_EXE_ERROR ;

			Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
			if( pScene==NULL )
				return PACKET_EXE_ERROR ;

			GCTest* pTest = (GCTest*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_TEST)) ;
			pTest->SetType( GCTest::TYPE_GC_SAY ) ;
			sprintf( pTest->GetTest()->szSay, "SceneID:%d %s", SceneID, pPacket->GetTest()->szSay ) ;
			//pScene->BroadCast( pTest, 0, 0, -1 ) ;
			g_pPacketFactoryManager->RemovePacket( pTest ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "CGTest::TYPE_CG_SAY_ALLSCENE %s...OK ",
				pPacket->GetTest()->szSay ) ;
		}
		break ;
	case CGTest::TYPE_CG_DROP_ITEM_BOX:
		{
			
			if( pPlayer==NULL )
				return PACKET_EXE_ERROR ;


			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

			SceneID_t SceneID = pHuman->getScene()->SceneID() ;
			if( SceneID == INVALID_ID )
				return PACKET_EXE_ERROR ;

			Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
			if( pScene==NULL )
				return PACKET_EXE_ERROR ;

			//���������Ʒ
			uint	iMonsterLevel	=	10;
			uint	iKillerLevel	=	10;
			INT		iMonsterType	=	0;

			

			////Obj_ItemBox������Ϣ����

			//GCNewItemBox ItemMsg ;

			//ItemMsg.setObjID(pIB->GetID());
			//ItemMsg.setOwner(pHuman->GetID());
			//pPlayer->SendPacket(&ItemMsg);

			break;
		}

	case  CGTest::TYPE_CG_MONSTER_DIE:
		{

			if( pPlayer==NULL )
				return PACKET_EXE_ERROR ;


			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

			SceneID_t SceneID = pHuman->getScene()->SceneID() ;
			if( SceneID == INVALID_ID )
				return PACKET_EXE_ERROR ;

			Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
			if( pScene==NULL )
				return PACKET_EXE_ERROR ;

			

			Obj* pObj = pScene->GetObjManager()->GetObj(pPacket->GetTest()->MonsterID);
			if(pObj)
			{
				Obj_Monster* pMonster = (Obj_Monster*)pObj;
				pMonster->GM_ForceDie(pGamePlayer->GetHuman());
			}

			break;
		}
	default :
		{
		}
		break ;
	}



	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
