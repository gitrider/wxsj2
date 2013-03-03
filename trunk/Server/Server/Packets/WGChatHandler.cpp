

#include "stdafx.h"
#include "WGChat.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GCChat.h"
#include "ChatPipe.h"
#include "PacketFactoryManager.h"
#include "SceneManager.h"




uint WGChatHandler::Execute( WGChat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

	switch( pPacket->GetChatType() )
	{
	case CHAT_TYPE_TEAM:
	case CHAT_TYPE_USER:
	case CHAT_TYPE_SELF:
		{
			PlayerID_t PlayerID = pPacket->GetPlayerID() ;
			GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
			if( pGamePlayer==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_CONTINUE ;
			}

			Obj_Human* pHuman = pGamePlayer->GetHuman() ;
			Assert( pHuman ) ;

			Scene* pScene = pHuman->getScene() ;
			if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

			GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
			pMsg->SetChatShowPos(pPacket->GetChatShowPos());
			pMsg->SetChatType( pPacket->GetChatType() ) ;
			pMsg->SetContexSize( pPacket->GetContexSize() ) ;
			pMsg->SetContex( pPacket->GetContex() ) ;
			pMsg->SetSourNameSize( pPacket->GetSourNameSize() ) ;
			pMsg->SetSourName( pPacket->GetSourName() ) ;

			pScene->GetChatPipe()->SendPacket( pMsg, INVALID_ID, pHuman->GetID() ) ;

		}
		break ;
	case CHAT_TYPE_SYSTEM:
	case CHAT_TYPE_WORLD:
		{
			for( INT i=0; i<MAX_SCENE; i++ )
			{
				Scene* pScene = g_pSceneManager->GetScene( (SceneID_t)i ) ;
				if( pScene==NULL )
					continue ;

				if( pScene->GetPlayerManager()->GetCount()<=0 )
					continue ;

				GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
				pMsg->SetChatShowPos(pPacket->GetChatShowPos());
				pMsg->SetChatType( pPacket->GetChatType() ) ;
				pMsg->SetContexSize( pPacket->GetContexSize() ) ;
				pMsg->SetContex( pPacket->GetContex() ) ;
				pMsg->SetSourNameSize( pPacket->GetSourNameSize() ) ;
				pMsg->SetSourName( pPacket->GetSourName() ) ;

				pScene->GetChatPipe()->SendPacket( pMsg, INVALID_ID, INVALID_ID ) ;

			}
		}
		break ;
	case CHAT_TYPE_GUILD:
		{
			for( INT i=0; i<MAX_SCENE; i++ )
			{
				Scene* pScene = g_pSceneManager->GetScene( (SceneID_t)i ) ;
				if( pScene==NULL )
					continue ;

				if( pScene->GetPlayerManager()->GetCount()<=0 )
					continue ;

				GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
				pMsg->SetChatShowPos(pPacket->GetChatShowPos());
				pMsg->SetChatType( pPacket->GetChatType() ) ;
				pMsg->SetContexSize( pPacket->GetContexSize() ) ;
				pMsg->SetContex( pPacket->GetContex() ) ;
				pMsg->SetSourNameSize( pPacket->GetSourNameSize() ) ;
				pMsg->SetSourName( pPacket->GetSourName() ) ;

				pScene->GetChatPipe()->SendPacket( pMsg,  INVALID_ID , pPacket->GetGuildID()) ; // modify by gh 2010/04/30 ���ݺ���ע�͵�֪���Ĳ���Ϊ���ID

			}
		}
		break ;

	case CHAT_TYPE_JIAZU:
		{
			for( INT i=0; i<MAX_SCENE; i++ )
			{
				Scene* pScene = g_pSceneManager->GetScene( (SceneID_t)i ) ;
				if( pScene==NULL )
					continue ;

				if( pScene->GetPlayerManager()->GetCount()<=0 )
					continue ;

				GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
				pMsg->SetChatShowPos(pPacket->GetChatShowPos());
				pMsg->SetChatType( pPacket->GetChatType() ) ;
				pMsg->SetContexSize( pPacket->GetContexSize() ) ;
				pMsg->SetContex( pPacket->GetContex() ) ;
				pMsg->SetSourNameSize( pPacket->GetSourNameSize() ) ;
				pMsg->SetSourName( pPacket->GetSourName() ) ;

				//�ڶ��������ð��ID�ǲ����Ѷ�Ϊ֮����������ģ��
				pScene->GetChatPipe()->SendPacket( pMsg, pPacket->GetJiaZuID(), pPacket->GetGuildID() ) ;

			}
		}
		break ;
	case CHAT_TYPE_COUNTRY:
		{
			for( INT i=0; i<MAX_SCENE; i++ )
			{
				Scene* pScene = g_pSceneManager->GetScene( (SceneID_t)i ) ;
				if( pScene==NULL )
					continue ;

				if( pScene->GetPlayerManager()->GetCount()<=0 )
					continue ;

				GCChat* pMsg = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;
				pMsg->SetChatShowPos(pPacket->GetChatShowPos());
				pMsg->SetChatType( pPacket->GetChatType() ) ;
				pMsg->SetContexSize( pPacket->GetContexSize() ) ;
				pMsg->SetContex( pPacket->GetContex() ) ;
				pMsg->SetSourNameSize( pPacket->GetSourNameSize() ) ;
				pMsg->SetSourName( pPacket->GetSourName() ) ;

				pScene->GetChatPipe()->SendPacket( pMsg, pPacket->GetCountryID(), INVALID_ID ) ;

			}
		}
		break ;
	default :
		{
			Assert( FALSE ) ;
		}
		break ;
	};


	g_pLog->FastSaveLog( LOG_FILE_1, "WGChatHandler:: ChatType=%d Contex=%s...OK ", 
		pPacket->GetChatType(), pPacket->GetContex() ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
