/*
�ÿͻ�����̯
*/

#include "stdafx.h"
#include "CGStallClose.h"
#include "OResultDef.h"
#include "GCOperateResult.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "GCStallClose.h"

uint CGStallCloseHandler::Execute( CGStallClose* pPacket, Player* pPlayer )
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

	//��֤�Ƿ����ʸ��̯
	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallCloseHandler::ObjID=%d, != ServerStallBox::STALL_OPEN"
			,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}


	pHuman->m_StallBox.GetBBS()->SaveTitle();

	//��֤���,����̯λ��
	pHuman->m_StallBox.CleanUp();

	ORESULT oResult = pHuman->GetHumanAI()->PushCommand_Idle( );
	if ( OR_FAILED( oResult ) )
	{
		pHuman->SendOperateResultMsg(oResult);
	}

	GCStallClose MsgClose;
	pGamePlayer->SendPacket(&MsgClose);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallCloseHandler::ObjID=%d"
		,pHuman->GetID()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
