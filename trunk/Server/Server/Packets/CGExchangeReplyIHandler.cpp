/*
�յ�����Ϣ֤�������뷽�Ѿ������˽������룬������˫���ͻ��˷��ͽ����������ӵ���Ϣ
*/

#include "stdafx.h"
#include "CGExchangeReplyI.h"
#include "GCExchangeError.h"
#include "GCExchangeEstablishI.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeReplyIHandler::Execute( CGExchangeReplyI* pPacket, Player* pPlayer )
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

	ObjID_t	SourceID = pPacket->GetObjID();
	Obj_Human* pDestHuman= pHuman;//�Լ������뷽
	Obj_Human* pSourceHuman = pScene->GetHumanManager()->GetHuman( SourceID );//���׷�����

	//��֤
	if( pSourceHuman == NULL )
	{//�����ߵ���
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	if(	pSourceHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA )
	{
		if( pSourceHuman->m_ExchangBox.m_ObjID == pDestHuman->GetID() )
		{//���ں��㽻�ױ�����
			return PACKET_EXE_CONTINUE;
		}
		else
		{	
			//�����Լ��Է��Ѿ��ڸ����˽�����,����״̬���	20100709 BLL
			if ( pSourceHuman->m_ExchangBox.m_ObjID != INVALID_ID )
			{
				GCExchangeError Msg;
				Msg.SetID(EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE);
				pHuman->GetPlayer()->SendPacket(&Msg);

				g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �Ѿ��ڽ����У� ���ܽ���[%s]������",	pSourceHuman->GetName(), pDestHuman->GetName() ) ;

				return PACKET_EXE_CONTINUE;
			}	
		}
	}

	if(pDestHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{
		if(	pDestHuman->m_ExchangBox.m_ObjID == pSourceHuman->GetID())
		{//���ǵ�ǰ����
			return PACKET_EXE_CONTINUE;
		}
		else
		{//�����Լ��Լ��Ѿ��ڸ����˽�����,����״̬���
			GCExchangeError Msg;
			Msg.SetID(EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE);
			pHuman->GetPlayer()->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �Ѿ��ڽ����У� ���ܽ���[%s]������",	pDestHuman->GetName(), pSourceHuman->GetName() ) ;
			return PACKET_EXE_CONTINUE;
		}
	}

	//�����ж�
	if(MySqrt( pSourceHuman->getWorldPos(), pDestHuman->getWorldPos() ) > EXCHANGE_MAX_DISTANCE )
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TOO_FAR);
		pHuman->GetPlayer()->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �������׷�Χ[%s]]",	pDestHuman->GetName(), pSourceHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE;
	}


	// ����˫���ƶ��в��ܽ���
    if ( TRUE == pSourceHuman->IsMoving() ) 
    {
        GCExchangeError Msg;
        Msg.SetID(EXCHANGE_MSG::ERR_SELF_MOVE);
        pHuman->GetPlayer()->SendPacket(&Msg);
        g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �����ƶ�", pSourceHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE;
    }

	if ( TRUE == pDestHuman->IsMoving() )
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_MOVE);
		pHuman->GetPlayer()->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �����ƶ�", pDestHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE;
	}

	// ��ʹ��ս������ʱ���ܽ���
	// ��ʹ�������ʱ���ܽ���
	if(pSourceHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || pSourceHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_ABILITY)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_USE_SKILL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeReplyIHandler:: [%s] ʹ�ü�����", pSourceHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pDestHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || pDestHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_ABILITY)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_USE_SKILL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeReplyIHandler::Ŀ�� [%s] ʹ�ü�����", pDestHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//�ڰ�̯ʱ���ܽ���
	if(pSourceHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_OPEN_STALL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeReplyIHandler:: [%s] ��̯��", pSourceHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pDestHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_OPEN_STALL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeReplyIHandler:: [%s] Ŀ���̯��", pDestHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	// �������ߵ��߲��ܽ���
	if(pSourceHuman->GetHP() <= 0 || pSourceHuman->GetPlayer()->IsDisconnect())
	{//�������������ߵ���
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_DEAD_OR_OFFLINE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeReplyIHandler: ID = %d ERROR:DEAD OR OFFLINE", pSourceHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}

	if(pDestHuman->GetHP() <= 0 || pDestHuman->GetPlayer()->IsDisconnect()) // ��֤������
	{//Ŀ���������ߵ���
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_DEAD_OR_OFFLINE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeReplyIHandler: ID = %d ERROR:TARGET DEAD OR OFFLINE ", pDestHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}


	//��֤ͨ������ʼ��ʼ��˫�����׺�,���벽��2
	pSourceHuman->m_ExchangBox.CleanUp();
	pSourceHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
	pSourceHuman->m_ExchangBox.m_Money = 0;
	pSourceHuman->m_ExchangBox.m_IsLocked = FALSE;
	pSourceHuman->m_ExchangBox.m_CanConform = FALSE;
	pSourceHuman->m_ExchangBox.m_ObjID = pDestHuman->GetID();
	//pSourceHuman->m_ExchangBox.

	pDestHuman->m_ExchangBox.CleanUp();
	pDestHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
	pDestHuman->m_ExchangBox.m_Money = 0;
	pDestHuman->m_ExchangBox.m_IsLocked = FALSE;
	pDestHuman->m_ExchangBox.m_CanConform = FALSE;
	pDestHuman->m_ExchangBox.m_ObjID = pSourceHuman->GetID();
	//pDestHuman->m_ExchangBox.
	g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �� [%s] ������������",	pSourceHuman->GetName(), pDestHuman->GetName() ) ;

	GCExchangeEstablishI MsgToSource;
	MsgToSource.SetObjID(pDestHuman->GetID());
	pSourceHuman->GetPlayer()->SendPacket(&MsgToSource);

	GCExchangeEstablishI MsgToDest;
	MsgToDest.SetObjID(pSourceHuman->GetID());
	pDestHuman->GetPlayer()->SendPacket(&MsgToDest);


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
