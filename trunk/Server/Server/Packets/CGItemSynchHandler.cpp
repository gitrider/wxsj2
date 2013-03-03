/*
同步客户端物品变化
*/
#include "stdafx.h"
#include "CGItemSynch.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"

#include "GCItemSynch.h"

#include "GCPlayerShopError.h"
#include "LogDefine.h"
#include "Obj_Pet.h"
#include "RecordOpt.h"

using namespace ITEM_SYNCH_STRUCT;

uint CGItemSynchHandler::Execute( CGItemSynch* pPacket, Player* pPlayer )
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

	g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::ObjName=%s"
		,pHuman->GetName()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
