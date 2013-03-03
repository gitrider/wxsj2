#include "stdafx.h"
#include "WGNotifyUser.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "GUIDManager.h"
#include "ServerManager.h"
#include "Scene.h"
#include "GWNotifyUser.h"
#include "PacketFactoryManager.h"
#include "GWChat.h"
#include "GCChat.h"


uint WGNotifyUserHandler::Execute( WGNotifyUser* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetGUID() ;
	Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(sGuid)) ;
	if( pHuman==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: pHuman==NULL GUID=%X",
			sGuid ) ;
		goto CHECK_RECYCLE_GUID;
	}

	GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer()) ;
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: pGamePlayer==NULL GUID=%X",
			sGuid ) ;
		goto CHECK_RECYCLE_GUID;
	}

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: pScene==NULL GUID=%X",
			sGuid ) ;
		goto CHECK_RECYCLE_GUID;
	}

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		PlayerID_t PlayerID = pHuman->GetPlayerID() ;
		pScene->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: ServerPlayer GUID=%X",
			sGuid ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		if( MyGetCurrentThreadID() != pScene->m_ThreadID ) return PACKET_EXE_CONTINUE ;

		if( pPacket->GetStatus() == WGNotifyUser::NUS_WORLD_KICK_REQUEST||
			pPacket->GetStatus() == WGNotifyUser::NUS_REMOVE)
		{
			GWNotifyUser* pGMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
			pGMsg->SetGUID(pPacket->GetGUID());
			pGMsg->SetPlayerID(-1);
			pGMsg->SetStatus(GWNotifyUser::NUS_NEED_WORLD_KICK);
			g_pServerManager->SendPacket( pGMsg, INVALID_ID ) ;

			g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: GamePlayer GUID=%X WGNotifyUser::NUS_WORLD_KICK_REQUEST kick receive!",
				sGuid ) ;
			return PACKET_EXE_ERROR ;
		}
		else if( pPacket->GetStatus() == WGNotifyUser::NUS_CANNOTSAY )
		{//����
			//pHuman->CannotSay( pPacket->GetTime() ) ;

			//CHAR szNotify[MAX_CHAT_SIZE] ;
			//if( pPacket->GetTime() > 0 )
			//{
			//	// 10��(��)������ҽ��Բ�������Ƶ��������....
			//	if ( pHuman->GetLevel() > 10 )
			//	{
			//		sprintf( szNotify, _TXT2((148),"#{_INFOUSR%s}�������Υ����Ϸ�����Ѿ�������%d����"), 
			//			pHuman->GetName(), pPacket->GetTime()/60000 );
			//		GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;
			//		pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			//		pChatPacket->SetChatType( CHAT_TYPE_SYSTEM ) ;
			//		pChatPacket->SetContexSize( (BYTE)(strlen(szNotify)) ) ;
			//		pChatPacket->SetContex( szNotify ) ;
			//		g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
			//		g_pLog->FastSaveLog( LOG_FILE_12, "ChatType=4 4 Contex=%s", szNotify ) ;
			//	}
		//	}
			//else
			//{
			//	sprintf( szNotify, _TXT2((149),"��ɫ���Թ����Ѿ��ظ������Ծ�������Ϸ����ף����Ϸ��죡"));
			//	GCChat* pChatPacket = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
			//	pChatPacket->SetChatType( CHAT_TYPE_SYSTEM ) ;
			//	pChatPacket->SetContexSize( (BYTE)(strlen(szNotify)) ) ;
			//	pChatPacket->SetContex( szNotify ) ;
			//	pChatPacket->SetSourNameSize( 0 ) ;
			//	pGamePlayer->SendPacket( pChatPacket ) ;
			//}
			//g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: GamePlayer GUID=%X WGNotifyUser::NUS_CANNOTSAY!",
			//	sGuid ) ;
	}
	}

	return PACKET_EXE_CONTINUE ;

CHECK_RECYCLE_GUID:
	//��World ����һ�����˵Ļ�Ӧ������������GUIDManager
	GWNotifyUser* nGMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
	nGMsg->SetGUID(pPacket->GetGUID());
	nGMsg->SetPlayerID(-1);

	if(g_pGUIDManager->Get(sGuid) == NULL/* && g_pGUIDManager->IsRecycle(sGuid) == FALSE*/)
	{
		nGMsg->SetStatus(GWNotifyUser::NUS_NOUSR);
		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: ServerPlayer GUID=%X GWNotifyUser::NUS_NOUSR send!",
			sGuid );
	}
	else
	{
		nGMsg->SetStatus(GWNotifyUser::NUS_LOCK_BY_SHM);
		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: ServerPlayer GUID=%X GWNotifyUser::NUS_LOCK_BY_SHM send!",
			sGuid );
	}
	g_pServerManager->SendPacket( nGMsg, INVALID_ID ) ;
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
