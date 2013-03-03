//CGCharAllTitlesHandler.cpp
#include "stdafx.h"
#include "CGCharAllTitles.h"
#include "GCCharAllTitles.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
uint CGCharAllTitlesHandler::Execute( CGCharAllTitles* pPacket, Player* pPlayer )
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

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t ObjID = pPacket->getTargetID() ;
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: not find obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( !IsCharacterObj( pObj->GetObjType() ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: not character obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(Obj::OBJ_TYPE_HUMAN == pObj->GetObjType() )
	{
		Obj_Human* phum = (Obj_Human*)pObj ;

        phum->UpdateTitlesToClient();
	}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
