/*
通知服务器此商店已经被盘出
*/

#include "stdafx.h"
#include "CGPlayerShopSaleOut.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopSaleOut.h"
#include "GCPlayerShopError.h"

using namespace PLAYER_SHOP;
UINT CGPlayerShopSaleOutHandler::Execute( CGPlayerShopSaleOut* pPacket, Player* pPlayer )
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

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::ObjName=%s "
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
