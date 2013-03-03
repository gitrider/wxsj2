
#include "stdafx.h"
#include "GWAskChangeScene.h"
#include "Log.h"
#include "OnlineUser.h"

#include "ServerPlayer.h"
#include "WGRetChangeScene.h"

uint GWAskChangeSceneHandler::Execute( GWAskChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t guid = pPacket->GetGUID() ;
	USER* pUser = g_pOnlineUser->FindUser( guid ) ;
	if( pUser )
	{
		//����ת�Ƴ�����֤��
		pUser->SetKey( pPacket->GetKey() );

		WGRetChangeScene Msg ;

		Msg.SetKey(pPacket->GetKey()) ;
		Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
		if( pPacket->GetStatus() == GWAskChangeScene::CSS_SAMESCENE )
		{//��ǰ������Ŀ�ĳ���������ͬ����Ϸ��������
			SceneID_t OldSceneID = pUser->GetSceneID() ;
			pUser->SetSceneID( pPacket->GetDestSceneID() );

			g_pOnlineUser->OnUserChangeScene( pUser, OldSceneID, pPacket->GetDestSceneID() ) ;

			Msg.SetReturn( WGRetChangeScene::CSR_SUCCESS ) ;
		}
		else if( pPacket->GetStatus() == GWAskChangeScene::CSS_DIFFSERVER )
		{//��ǰ������Ŀ�ĳ������ڲ�ͬ����Ϸ��������
			FULLUSERDATA* pFullUserData = g_pOnlineUser->NewData() ;
			if( pFullUserData==NULL )
			{
				Msg.SetReturn( WGRetChangeScene::CSR_ERROR ) ;
			}
			else
			{
				memcpy( pFullUserData, pPacket->GetUserData(), sizeof(FULLUSERDATA) ) ;
				pUser->SetFullUserData( pFullUserData );
				pUser->SetUserStatus( US_HOLDUSERDATA );
				pUser->ResetKickTime();

				//�޸��û���������
				SceneID_t OldSceneID = pUser->GetSceneID() ;
				pUser->GetFullUserData()->m_Human.m_StartScene = pPacket->GetDestSceneID() ;
				pUser->SetSceneID( pPacket->GetDestSceneID() );
				g_pOnlineUser->OnUserChangeScene( pUser, OldSceneID, pPacket->GetDestSceneID() ) ;

				Msg.SetSceneID( pPacket->GetDestSceneID() ) ;
				Msg.SetReturn( WGRetChangeScene::CSR_SUCCESS_DIFFSERVER ) ;
			}
		}
		else 
		{
			Assert(FALSE) ;
		}

		pServerPlayer->SendPacket( &Msg ) ;
	}
	else
	{
		Log::SaveLog( WORLD_LOGFILE, "ERROR:GWAskChangeSceneHandler...GUID=%X,S=%d,D=%d!", 
			guid, pPacket->GetSourSceneID(), pPacket->GetDestSceneID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Log::SaveLog( WORLD_LOGFILE, "GWAskChangeSceneHandler...GUID=%X,S=%d,D=%d!", 
		guid, pPacket->GetSourSceneID(), pPacket->GetDestSceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
