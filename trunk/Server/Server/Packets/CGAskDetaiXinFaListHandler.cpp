#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCDetailXinFaList.h"
#include "Scene.h"
#include "HumanDB.h"
#include "CGAskDetailXinFaList.h"


uint CGAskDetailXinFaListHandler::Execute( CGAskDetailXinFaList* pPacket, Player* pPlayer )
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

	ObjID_t ObjID = pHuman->GetID();
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj(pPacket->getTargetID()));
	if( pTarget==NULL )
	{
		Assert( pTarget ) ;
		return PACKET_EXE_ERROR ;
	}

	// Ȩ���ж�
	// to be continue...
	if ( pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN
		&& pTarget->GetObjType() != Obj::OBJ_TYPE_PET )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailXinFaListHandler: faile obj=%d scene=%d", 
			ObjID, pScene->SceneID() ) ;
		return PACKET_EXE_CONTINUE;
	}

	GCDetailXinFaList	Msg;

	Msg.setObjID( pTarget->GetID() );

	Msg.setXinFaList(pHuman->Skill_GetXinFaList().m_Count, pHuman->Skill_GetXinFaList().m_aXinFa);
	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailXinFaListHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
