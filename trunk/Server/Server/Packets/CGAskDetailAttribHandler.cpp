
#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "ServerManager.h"
#include "CGAskDetailAttrib.h"
#include "GWAskOtherOnlineAttrib_Human.h"
#include "PacketFactoryManager.h"

using namespace Packets;

uint CGAskDetailAttribHandler::Execute( CGAskDetailAttrib* pPacket, Player* pPlayer )
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

	ObjID_t ObjID = pHuman->GetID();
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR ;
	}

	Obj *pObj = pScene->GetObjManager()->GetObj(pPacket->getTargetID());
	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailAttribHandler: Target is NULL TargetID=%d", pPacket->getTargetID() ) ;
		return PACKET_EXE_ERROR;
	}
	
	// 权限判断
	// to be continue...
	if(pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN)
	{
		Obj_Human* pTarget = (Obj_Human*)pObj;
		if ( fabsf( pHuman->getWorldPos()->m_fX - pTarget->getWorldPos()->m_fX ) <= 10.f
			&& fabsf( pHuman->getWorldPos()->m_fZ - pTarget->getWorldPos()->m_fZ ) <= 10.f )
		{
			pTarget->AskMsg_DetailAttrib(pHuman);
		}
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailAttribHandler: No Rights to Target DetailAttr obj=%d", ObjID ) ;
			return PACKET_EXE_CONTINUE;
		}
	}
	else if(pObj->GetObjType() == Obj::OBJ_TYPE_PET)
	{
		// 宠物不通过此方法得详细属性
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailAttribHandler: Not Implement Obj_Pet Target obj=%d", ObjID ) ;
		return	PACKET_EXE_CONTINUE;
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailAttribHandler: Warring No Rights to ask other obj's detailattribute obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailAttribHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
