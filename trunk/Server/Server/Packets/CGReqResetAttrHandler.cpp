#include "stdafx.h"
#include "CGReqResetAttr.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCReqResetAttrResult.h"


uint	CGReqResetAttrHandler::Execute(CGReqResetAttr* pPacket,Player* pPlayer)
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
		
	//Ȩ���ж�
	
	GCReqResetAttrResult	Msg;

	if(pHuman->GetLevel()>=DEFAULT_WASHPOINT_LEVEL)
	{
		//ִ��
		pHuman->WashPoints();
		Msg.SetAttrPoint(pHuman->Get_RemainPoints());
		Msg.SetResult(RATTR_SUCCESS);
	}
	else
	{
		Msg.SetResult(RATTR_NO_LEVEL);
	}
	


	//��Ϣ

	pGamePlayer->SendPacket(&Msg);



	g_pLog->FastSaveLog( LOG_FILE_1, "CGReqResetAttrHandler: Obj=%d Level=%d ", 
		pHuman->GetID(), pHuman->GetLevel() ) ;

	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}