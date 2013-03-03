

#include "stdafx.h"
#include "CGBankMoney.h"
#include "GCBankMoney.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGBankMoneyHandler::Execute( CGBankMoney* pPacket, Player* pPlayer )
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

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	BYTE	IsSave = pPacket->GetIsSave();
	INT		Amount = pPacket->GetAmount();
	
	INT CurMoney = pHuman->__GetBankMoney();

	if(IsSave == CGBankMoney::SAVE_MONEY)
	{
		if (Amount<0)
		{
			return PACKET_EXE_ERROR;
		}
		if( Amount>(INT)(pHuman->GetMoney()) )
		{
			return PACKET_EXE_ERROR;
		}
		CurMoney += Amount;
		//将来要判一下是否大于银行能存的最大上限
		if(CurMoney>MAX_BANK_MONEY)
			CurMoney = MAX_BANK_MONEY;

		pHuman->__SetBankMoney(CurMoney);
		pHuman->SetMoney(pHuman->GetMoney() - Amount);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_BANK_SAVE;
		MoneyLogParam.Count		=	Amount;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

	}
	else if(IsSave == CGBankMoney::PUTOUT_MONEY)
	{
		if (Amount<0)
		{
			return PACKET_EXE_ERROR;
		}
		if(CurMoney - Amount < 0)
			Amount = CurMoney;

		pHuman->__SetBankMoney(CurMoney - Amount);
		pHuman->SetMoney(pHuman->GetMoney() + Amount);
		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_BANK_WITHDRAW;
		MoneyLogParam.Count		=	Amount;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

	}
	else if(IsSave == CGBankMoney::UPDATE_MONEY)
	{
		Amount = CurMoney;
	}
	else
	{
		return PACKET_EXE_ERROR;
	}
	GCBankMoney Msg;
	Msg.SetIsSave(IsSave);
	Msg.SetAmount(Amount);
	pPlayer = pHuman->GetPlayer();
	pPlayer->SendPacket( &Msg ) ;
	g_pLog->FastSaveLog( LOG_FILE_1, "CGBankMoneyHandler: Issave=%d, Amount=%d", (INT)IsSave, Amount ) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
