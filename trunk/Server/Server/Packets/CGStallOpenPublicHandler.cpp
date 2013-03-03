/*
玩家摊位开市
*/

#include "stdafx.h"
#include "CGStallOpenPublic.h"

#include "Type.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCStallOpenPublic.h"


uint CGStallOpenPublicHandler::Execute( CGStallOpenPublic* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene == NULL )
	{
		Assert( FALSE ) ;
		return PACKET_EXE_ERROR;
	}
	 
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	if(pHuman->__IsPasswordProtect(MINORPASSWD_INFO::PROTECT_STALL))
	{
		if (!pHuman->__IsPasswordUnlock())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d  Password lock", pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE ;
		}
	}

	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN )
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_ILLEGAL);
		pGamePlayer->SendPacket(&Msg);

		return PACKET_EXE_CONTINUE;
	}

	pHuman->m_StallBox.SetStallOpenPublic( pPacket->GetIsOpen() );

	GCStallOpenPublic msg;
	msg.SetIsOpen( pPacket->GetIsOpen() );
	pGamePlayer->SendPacket( &msg );

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
