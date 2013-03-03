/*
�ͻ��˵Ľ�Ǯ�仯֪ͨ������
*/

#include "stdafx.h"
#include "CGExchangeSynchMoneyII.h"
#include "GCExchangeSynchII.h"
#include "GCExchangeError.h"
#include "GCExchangeSynchLock.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"


uint CGExchangeSynchMoneyIIHandler::Execute( CGExchangeSynchMoneyII* pPacket, Player* pPlayer )
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
	EXCHANGE_CERTIFY_ISLOCK(pHuman)

	ObjID_t	DestID = pHuman->m_ExchangBox.m_ObjID;
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( DestID );

	EXCHANGE_CERTIFY_STATUS(pHuman, EXCHANGE_SYNCH_DATA)
	EXCHANGE_CERTIFY_STATUS(pDestHuman, EXCHANGE_SYNCH_DATA)
	

	//����
	UINT	Money	 = pPacket->GetMoney();		

	if( Money<= pHuman->GetMoney() )
	{
		pHuman->m_ExchangBox.m_Money = Money;
		g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] ���ý��׽�Ǯ [%d]",	pHuman->GetName(), Money ) ;

        //�ı��Ǯ    �Լ�����
        pHuman->m_ExchangBox.m_IsLocked = FALSE;
        if(pDestHuman->m_ExchangBox.m_IsLocked == TRUE)
        {//����Է��Ѿ�����
            pDestHuman->m_ExchangBox.m_IsLocked = FALSE;
            GCExchangeSynchLock MsgToDes, MsgToSelf;

            //֪ͨ�Է�����˫����
            MsgToDes.SetIsBoth(TRUE);
            MsgToDes.SetIsLocked(FALSE);
            pDestHuman->GetPlayer()->SendPacket(&MsgToDes);

            //֪ͨ�Լ���Է���
            MsgToSelf.SetIsMyself(FALSE);
            MsgToSelf.SetIsLocked(FALSE);
            pHuman->GetPlayer()->SendPacket(&MsgToSelf);

        }
        else
        {//�Է�û�м�����ֱ��ͬ��
            GCExchangeSynchLock MsgToDes;
            MsgToDes.SetIsMyself (FALSE);
            MsgToDes.SetIsLocked(FALSE);
            pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
        }

		//��Ǯ�ı䲻�����Լ��ˣ�ʡ����Ϣ
		GCExchangeSynchII MsgToTarget;
		MsgToTarget.SetIsMyself(FALSE);
		MsgToTarget.SetOpt(EXCHANGE_MSG::OPT_MONEY);
		MsgToTarget.SetMoney(Money);
		pDestHuman->GetPlayer()->SendPacket(&MsgToTarget);
		return PACKET_EXE_CONTINUE;
	}
	else
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
		pHuman->GetPlayer()->SendPacket(&Msg);
		pHuman->m_ExchangBox.CleanUp();
		return PACKET_EXE_CONTINUE;
	}
	
	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
