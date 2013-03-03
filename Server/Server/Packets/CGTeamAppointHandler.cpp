

#include "stdafx.h"
#include "CGTeamAppoint.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamAppoint.h"
#include "ServerManager.h"
#include "GUIDManager.h"
#include "GCTeamError.h"


uint CGTeamAppointHandler::Execute( CGTeamAppoint* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;
	
	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Obj_Human* pCharacter = (Obj_Human*)(g_pGUIDManager->Get(pPacket->GetDestGUID())) ;
	if( pCharacter )
	{
		if( pCharacter->GetObjType()==Obj::OBJ_TYPE_HUMAN )
		{
			INT objId = pCharacter->GetID();
			if ( pCharacter->GetPlayer()->IsDisconnect() )
			{
				//对方离线，返回离线提示信息
				GCTeamError tMsg ;
				tMsg.SetErrorCode( TEAM_ERROR_INVITEEOFFLINE ) ;
				pGamePlayer->SendPacket( &tMsg ) ;


				//return PACKET_EXE_ERROR 
				return PACKET_EXE_CONTINUE;
			}
			
		}
	}
	else
	{
		//对方离线,返回离线提示信息
		GCTeamError tMsg ;
		tMsg.SetErrorCode( TEAM_ERROR_INVITEEOFFLINE ) ;
		pGamePlayer->SendPacket( &tMsg ) ;

		//return PACKET_EXE_ERROR ;
		return PACKET_EXE_CONTINUE;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;


	GWTeamAppoint* pMsg = (GWTeamAppoint*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMAPPOINT)) ;
	pMsg->SetSourGUID( pPacket->GetSourGUID() ) ;//旧队长
	pMsg->SetDestGUID( pPacket->GetDestGUID() ) ;//新队长

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamAppointHandler: sGUID=%X dGUID=%X", 
		pPacket->GetSourGUID(), pPacket->GetDestGUID() ) ;



	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
