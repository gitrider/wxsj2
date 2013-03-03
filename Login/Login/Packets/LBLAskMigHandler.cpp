#include "stdafx.h" 
#include "LBLAskMig.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetPrize.h"


UINT LBLAskMigHandler::Execute(LBLAskMig* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID != g_pServerManager->m_ThreadID)
	{
		Assert(FALSE);
	}

	Assert(pPlayer);
	Assert(pPacket->GetType() == LBLAskMig::BL_ASKMIG);

	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);

		LWRetPrize	Msg;
		
		//Msg.SetAccName(pPacket->GetAccount());
		//Msg.SetPrizeUserID(pPacket->GetPacketIndex());
  //      Msg.SetPrizeResult(pPacket->GetPrizeResult());
		//Msg.SetRetType(ASK_NEWPRIZE);

		//if(/*BNPR_SUCCESS*/0x00 == pPacket->GetPrizeResult())
		//{
		//	for(INT i = 0; i < pPacket->GetPrizeNum(); ++i)
		//	{
		//		Msg.AddPrize(pPacket->GetPrize(i));

		//		g_pLog->FastSaveLog(LOG_FILE_0,"LBLAskMig...GetPrize Acc=%s price=%s,num=%d",
		//			pPacket->GetAccount(), pPacket->GetPrize(i)->m_PrizeString, pPacket->GetPrize(i)->m_PrizeNum);
		//	}
		//}
		//else if(/*BNPR_ALREADY_GET*/0x02 == pPacket->GetPrizeResult())
		//{
		//	for(INT i = 0; i < pPacket->GetPrizeNum(); ++i)
		//	{
		//		//已经领过的奖品就没啥意思了
		//		//Msg.AddPrize(pPacket->GetPrize(i));
		//		g_pLog->FastSaveLog(LOG_FILE_0,"LBLAskMig...AlreadyGetPrize Acc=%s price=%s,num=%d Drop",
		//			pPacket->GetAccount(), pPacket->GetPrize(i)->m_PrizeString, pPacket->GetPrize(i)->m_PrizeNum);
		//	}
		//}

		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);
		}

		
		g_pLog->FastSaveLog(LOG_FILE_0,"LBLAskMig::Execute()...OK Acc=%s PUID=%d,Result = %d",
			pPacket->GetAccount(),pPacket->GetPacketIndex(),pPacket->GetMigResult());
	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}