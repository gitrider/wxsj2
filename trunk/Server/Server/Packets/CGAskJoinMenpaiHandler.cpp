// CGAskJoinMenpaiHandler.cpp


#include "stdafx.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GameTable.h"

#include "CGAskJoinMenpai.h"
#include "GCJoinMenpai.h"

uint CGAskJoinMenpaiHandler::Execute( CGAskJoinMenpai* pPacket, Player* pPlayer )
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

	
	
	//��������
	pHuman->GetDB()->SetMenPai(pPacket->GetMenpaiID());

	//�����ķ�
	

	//���ü���
	

	//֪ͨ�ͻ��˼���Ľ��,�ɿͻ����Լ�ȥ��ģ���ڵ����ݸı��Լ���ֵ
	GCJoinMenpai Msg;
	Msg.SetIsSucceedJoin(JOINMENPAI_SUCCEED);

	pPlayer = pHuman->GetPlayer();
	pPlayer->SendPacket( &Msg ) ;

	//INT nTemp = pPacket->GetMenpaiID();
	//INT nNum = g_XinFaDataMgr.m_nSize;

	//_XINFA_LIST   listMenpaiXinfa;

	//for(INT i=0,k=0; i<nNum; i++)
	//{
	//	if( g_XinFaDataMgr.m_vTemplateList[i].MenPai == nTemp)
	//		listMenpaiXinfa.m_aXinFa
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
