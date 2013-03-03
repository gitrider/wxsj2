

#include "stdafx.h"
#include "CGStallEstablish.h"
#include "OResultDef.h"
#include "GCOperateResult.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "GCStallEstablish.h"
#include "GCStallApply.h"

uint CGStallEstablishHandler::Execute( CGStallEstablish* pPacket, Player* pPlayer )
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
	//����״̬���ɲ���
	if(pHuman->m_ExchangBox.m_Status > 0)
	{//����
		pHuman->m_StallBox.CleanUp();
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	// ͨ��AI�жϵ�ǰ״̬���ܷ��̯
	AI_Human* pAIHuman = (AI_Human*)(pHuman->GetAIObj());
	if (!pAIHuman) {
		return PACKET_EXE_ERROR;
	}

	ORESULT oResult = pAIHuman->PushCommand_Stall();
	if (OR_OK != oResult) {
		pHuman->SendOperateResultMsg(oResult);
		return PACKET_EXE_CONTINUE;
	}
	
	UINT	PosTax		= 0;
	BYTE	TradeTax	= 0;
	StallInfoManager* pStallInfoMgr = pScene->GetStallInfoManager();
	if(pStallInfoMgr)
	{
		UINT PosX = (UINT)pHuman->getWorldPos()->m_fX;
		UINT PosZ = (UINT)pHuman->getWorldPos()->m_fZ;

		BOOL bCanStall = pStallInfoMgr->CanStall(PosX, PosZ);
		if(bCanStall == FALSE)
		{
			GCStallApply	Msg;
			Msg.SetIsCanStall(FALSE);

			pHuman->m_StallBox.CleanUp();
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallEstablishHandler::ObjID=%d, error pos"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE ;
		}

		//��֤�Ƿ����ʸ��̯
		if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_READY)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_ILLEGAL);
			pHuman->m_StallBox.CleanUp();
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallEstablishHandler::ObjID=%d, ERR_ILLEGAL:!= ServerStallBox::STALL_READY"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE ;
		}

		PosTax	  = pStallInfoMgr->StallPosPayment(PosX, PosZ);
		TradeTax  = pStallInfoMgr->StallExchangeTax(PosX, PosZ);

		if(pHuman->GetMoney() < PosTax)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_NOT_ENOUGH_MONEY_TO_OPEN);
			pHuman->m_StallBox.CleanUp();
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallEstablishHandler::ObjID=%d, ERR_NOT_ENOUGH_MONEY_TO_OPEN"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE ;
		}
		else
		{
			pHuman->SetMoney(pHuman->GetMoney() - PosTax);
			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_EXCHANGE_STALL_TAX;	
			MoneyLogParam.Count		=	PosTax;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);

		}

		//��֤���,����̯λ��
		pHuman->m_StallBox.SetStallStatus(ServerStallBox::STALL_OPEN);
		pHuman->m_StallBox.SetStallIsOpen(TRUE);//�����������ͻ��˵���ʾ����
		pHuman->m_StallBox.SetPosTax(PosTax);
		pHuman->m_StallBox.SetTradeTax(TradeTax);
		//pHuman->m_StallBox.SetFirstPage(0);

		//ռ����񣬲�׼�����ٰ�̯��
		pStallInfoMgr->SetCanStall(PosX, PosZ, FALSE);
		pHuman->m_StallBox.SetStallPos(PosX, PosZ);

		//֪ͨ�Լ���ʼ���Լ���̯λ��
		GCStallEstablish Msg;
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallEstablishHandler::ObjID=%d"
			,pHuman->GetID()) ;
	}
	else
	{
		Assert(0);
	}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
