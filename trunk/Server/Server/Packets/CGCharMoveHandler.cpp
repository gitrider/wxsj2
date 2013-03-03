

#include "stdafx.h"
#include "CGCharMove.h"
#include "OResultDef.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Log.h"
#include "GCOperateResult.h"
#include "GCCharMoveResult.h"
//#include "GCNewPlayer_Move.h"




uint CGCharMoveHandler::Execute( CGCharMove* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharMoveHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharMoveHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharMoveHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	if ( MyGetCurrentThreadID() != pScene->m_ThreadID )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharMoveHandler: Error Obj = %d (MyGetCurrentThreadID() != pScene->m_ThreadID) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//��̯��
		GCCharMoveResult msgResult;
		msgResult.setResult(OR_CAN_NOT_MOVE_STALL_OPEN);
		msgResult.setHandleID(pPacket->getHandleID());
		pGamePlayer->SendPacket( &msgResult ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharMoveHandler::��̯��") ;
		return PACKET_EXE_CONTINUE ;
	}

	ObjID_t ObjID = pHuman->GetID() ;
	INT nHandleID = pPacket->getHandleID();
	WORLD_POS CurPos = *pHuman->getWorldPos() ;
	WORD wNumPathNode = (WORD)(pPacket->getNumTargetPos());
	const WORLD_POS *paTarPos = pPacket->getTargetPos() ;
	ORESULT oResult = pHuman->GetHumanAI()->PushCommand_Move( nHandleID, wNumPathNode, paTarPos );
	if ( OR_FAILED( oResult ) )
	{
		GCCharMoveResult msgResult;
		msgResult.setResult(oResult);
		msgResult.setHandleID(pPacket->getHandleID());
		pGamePlayer->SendPacket( &msgResult ) ;
	}

	if ( wNumPathNode > 0 )
	{
		//Ӱ��Ч��ע�͵�%95 ��Ϣ�������Ϣ
		//g_pLog->FastSaveLog( LOG_FILE_1, "CGCharMoveHandler::ObjID=%d X=%f Y=%f", 
		//	ObjID, paTarPos[wNumPathNode-1].m_fX, paTarPos[wNumPathNode-1].m_fZ ) ;
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharMoveHandler::ObjID=%d , no target pos", ObjID ) ;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
