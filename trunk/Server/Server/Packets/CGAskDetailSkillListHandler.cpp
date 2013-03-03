#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCDetailSkillList.h"
#include "Scene.h"
#include "HumanDB.h"

#include "CGAskDetailSkillList.h"

uint CGAskDetailSkillListHandler::Execute( CGAskDetailSkillList* pPacket, Player* pPlayer )
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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailSkillListHandler: faile obj=%d scene=%d", 
			ObjID, pScene->SceneID() ) ;
		return PACKET_EXE_CONTINUE;
	}

	GCDetailSkillList	Msg;

	Msg.setObjID( pTarget->GetID() );

	Msg.setSkillList( (WORD)(pHuman->Skill_GetSkillList().m_Count), pHuman->Skill_GetSkillList().m_aSkill );

	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailSkillListHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
