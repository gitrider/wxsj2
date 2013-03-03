/*
�ͻ���������ؽ��׺����ݣ��رս���,������Ϣ��
�������յ�����Ϣ�����˫�����׺���Ϣ������Ϣ15���Է�
*/

#include "stdafx.h"
#include "CGExchangeCancel.h"
#include "GCExchangeCancel.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeCancelHandler::Execute( CGExchangeCancel* pPacket, Player* pPlayer )
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

	//��֤
	EXCHANGE_CERTIFY_EACH_OTHER(pHuman)
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( pHuman->m_ExchangBox.m_ObjID );

	//���״̬
	pHuman->m_ExchangBox.CleanUp();
	pDestHuman->m_ExchangBox.CleanUp();

	g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] ������ȡ���˽���",	pHuman->GetName() ) ;

	//����ȡ����Ϣ��˫��
	GCExchangeCancel MsgToMe,MsgToOt;
	pHuman->GetPlayer()->SendPacket(&MsgToMe);
	pDestHuman->GetPlayer()->SendPacket(&MsgToOt);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
