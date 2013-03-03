

#include "stdafx.h"
#include "WGChannelError.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"

#include "GCChannelError.h"



uint WGChannelErrorHandler::Execute( WGChannelError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

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

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		pScene->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGChannelErrorHandler: ServerPlayer (ErrorCode=%d) ",
			pPacket->GetErrorCode() ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		GCChannelError Msg ;
		Msg.SetErrorCode( pPacket->GetErrorCode() ) ;

		pGamePlayer->SendPacket( &Msg ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGChannelErrorHandler: GamePlayer (GUID=%X ErrorCode=%d) ",
			pGamePlayer->m_HumanGUID, pPacket->GetErrorCode() ) ;
	}
	else
	{
		Assert(FALSE) ;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
