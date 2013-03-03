
#include "stdafx.h"
#include "CGPlayerShopEstablish.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "PacketFactoryManager.h"
#include "GCPlayerShopEstablish.h"
#include "TimeManager.h"//g_pTimeManager
#include "GWChat.h"
#include "ServerManager.h"

UINT CGPlayerShopEstablishHandler::Execute( CGPlayerShopEstablish* pPacket, Player* pPlayer )
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
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopEstablishHandler::Name=%s"
		,pHuman->GetName()) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
