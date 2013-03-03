/*
��֤����ĺϷ��ԣ���ת��
*/

#include "stdafx.h"
#include "CGExchangeApplyI.h"
#include "GCExchangeApplyI.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeApplyIHandler::Execute( CGExchangeApplyI* pPacket, Player* pPlayer )
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

	ObjID_t		TargetID = pPacket->GetObjID();
	Obj_Human* pSourceHuman = pHuman;//���׷�����
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( TargetID );//���׶���

    if (pSourceHuman->IsInLoginProtectTime() || pDestHuman->IsInLoginProtectTime())
    {//�����߱���ʱ���ڣ����ɲ���
        GCExchangeError Msg;
        Msg.SetID(EXCHANGE_MSG::ERR_IN_PROTECT_TIME);
        pGamePlayer->SendPacket(&Msg);
        g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d or ID = %d ERR_IN_PROTECT_TIME", pSourceHuman->GetID(), pDestHuman->GetID()) ;

        return PACKET_EXE_CONTINUE;
    }

	//��֤
	if( pDestHuman == NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}
	INT iSettingData = pDestHuman->GetDB()->GetSetting(SETTING_TYPE_GAME)->m_SettingData;
	if(SETTINGFLAGISTRUE(iSettingData, GSF_REFUSE_TRADE))
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_REFUSE_TRADE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERR_REFUSE_TRADE", pSourceHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pSourceHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//���������ڽ�����
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pSourceHuman->GetID(), pSourceHuman->m_ExchangBox.m_Status) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pDestHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//Ŀ�����ڽ�����
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pDestHuman->GetID(),	pDestHuman->m_ExchangBox.m_Status ) ;
		return PACKET_EXE_CONTINUE;
	}
	//��ʹ��ս������ʱ���ܽ���
	//��ʹ�������ʱ���ܽ��� 
	//ս���в��ܽ���add by gh 2010/04/28
	if(pSourceHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || pSourceHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_ABILITY)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_USE_SKILL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::ʹ�ü�����") ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pDestHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || pDestHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_ABILITY)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_USE_SKILL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::Ŀ��ʹ�ü�����") ;
		return PACKET_EXE_CONTINUE ;
	}
	//�����в��ܽ���
	if(pSourceHuman->IsMoving())
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_MOVE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::�ƶ���") ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pDestHuman->IsMoving())
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_MOVE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::Ŀ���ƶ���") ;
		return PACKET_EXE_CONTINUE ;
	}
    //�ڰ�̯ʱ���ܽ���
	if(pSourceHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_OPEN_STALL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::��̯��") ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pDestHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_OPEN_STALL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::Ŀ���̯��") ;
		return PACKET_EXE_CONTINUE ;
	}
	// �������ߵ��߲��ܽ���
	if(pSourceHuman->GetHP() <= 0 || pSourceHuman->GetPlayer()->IsDisconnect())
	{//�������������ߵ���
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_DEAD_OR_OFFLINE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERROR:DEAD OR OFFLINE", pSourceHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pDestHuman->GetHP() <= 0 || pDestHuman->GetPlayer()->IsDisconnect()) // ��֤������
	{//Ŀ���������ߵ���
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_DEAD_OR_OFFLINE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERROR:TARGET DEAD OR OFFLINE ", pDestHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	// end of add
    //���׵����֮��ľ��볬��xx�ײ��ܽ���
    FLOAT fDistance = MySqrt( pDestHuman->getWorldPos(), pSourceHuman->getWorldPos() );
    if (fDistance > EXCHANGE_MAX_DISTANCE)
    {
        GCExchangeError Msg;
        Msg.SetID(EXCHANGE_MSG::ERR_TOO_FAR);
        pHuman->GetPlayer()->SendPacket(&Msg);
        g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �������׷�Χ[%s]]",	pDestHuman->GetName(), pSourceHuman->GetName() ) ;
        return PACKET_EXE_CONTINUE;
    }

	//����
	//������Ϣ��Ŀ������
	GCExchangeApplyI Msg;
	Msg.SetObjID(pSourceHuman->GetID());
	pDestHuman->GetPlayer()->SendPacket(&Msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: SOURCEID = %d DESTID = %d", pSourceHuman->GetID(), pDestHuman->GetID() ) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
