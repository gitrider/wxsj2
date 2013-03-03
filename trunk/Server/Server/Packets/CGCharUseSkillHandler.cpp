

#include "stdafx.h"
#include "GamePlayer.h"
#include "Log.h"
#include "Scene.h"
#include "OResultDef.h"
#include "GCOperateResult.h"

#include "Obj_Human.h"
#include "Obj_Pet.h"
#include "AI_Human.h"
#include "AI_Pet.h"

#include "CGCharUseSkill.h"



uint CGCharUseSkillHandler::Execute( CGCharUseSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	if ( MyGetCurrentThreadID() != pScene->m_ThreadID )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error Obj = %d (MyGetCurrentThreadID() != pScene->m_ThreadID) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	ObjID_t		ObjID			= pHuman->GetID() ;
	SkillID_t	idSkill			= pPacket->getSkillDataID();
	ObjID_t		idTarget		= pPacket->getTargetID();
	WORLD_POS	posTarget		= pPacket->getTargetPos() ;
	FLOAT		fDir			= pPacket->getDir() ;
	//now the target guid only use to call of human(拉人去指定位置的操作)
	GUID_t		guidTarget		= pPacket->getTargetGUID();
	INT			iHurtDelayTime	= pPacket->GetHurtDelayTime();

	//如果角色是人类
	if ( pPacket->getObjID() == pHuman->GetID() )
	{
		//将使用技能交给角色状态机处理
		ORESULT oResult = pHuman->GetHumanAI()->PushCommand_UseSkill( idSkill, idTarget, posTarget.m_fX, posTarget.m_fZ, fDir, guidTarget, iHurtDelayTime );
		if ( OR_FAILED( oResult ) )
		{
			pHuman->SendOperateResultMsg(oResult);
		}

	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
