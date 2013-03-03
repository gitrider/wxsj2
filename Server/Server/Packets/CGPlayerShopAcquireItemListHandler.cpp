/*
客户端申请商店的商品列表
*/

#include "stdafx.h"
#include "CGPlayerShopAcquireItemList.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCItemList.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"

#include "RecordOpt.h"
#include "GCPlayerShopUpdatePartners.h"

using namespace ITEM_SYNCH_STRUCT;
UINT CGPlayerShopAcquireItemListHandler::Execute( CGPlayerShopAcquireItemList* pPacket, Player* pPlayer )
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
	
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopAcquireItemListHandler::ObjName=%s",pHuman->GetName()) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
