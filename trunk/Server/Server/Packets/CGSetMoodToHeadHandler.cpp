//���������ķ�
#include "stdafx.h"
#include "Log.h"

#include "DataRecords.h"
#include "GameTable.h"
#include "GamePlayer.h"
#include "Scene.h"


#include "CGSetMoodToHead.h"


uint CGSetMoodToHeadHandler::Execute( CGSetMoodToHead* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	INT bView = pPacket->GetView();

	if(bView == 1)
	{
        //ModifyBy AMos  ���ܲ�������ע���ˣ�����������
		//��ʾ����
		//pHuman->SetTitleType(_TITLE::MOOD_TITLE);
		//pHuman->SetTitle(pHuman->GetHumanRelation()->GetMood());
		
	}
	else
	{
        //ModifyBy AMos  ���ܲ�������ע���ˣ�����������
		//pHuman->SetTitleType(_TITLE::NO_TITLE);
		//pHuman->SetTitle("");
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGSetMoodToHeadHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
