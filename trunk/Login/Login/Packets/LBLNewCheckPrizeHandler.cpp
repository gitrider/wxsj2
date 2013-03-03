#include "stdafx.h" 
#include "LBLNewCheckPrize.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetPrize.h"


UINT LBLNewCheckPrizeHandler::Execute(LBLNewCheckPrize* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID != g_pServerManager->m_ThreadID)
	{
		Assert(FALSE);
	}

	Assert(pPlayer);
	Assert(pPacket->GetType() == LBLNewCheckPrize::BL_CHECK_PRIZE);

	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);

		LWRetPrize	Msg;
		
		Msg.SetAccName(pPacket->GetAccount());
		Msg.SetPrizeUserID(pPacket->GetPacketIndex());
        Msg.SetPrizeResult(pPacket->GetPrizeResult());
		Msg.SetRetType(ASK_NEWCHECKPRIZE);

		if(/*BNCPR_SUCCESS*/0x00 == pPacket->GetPrizeResult())
		{
			for(INT i = 0; i < pPacket->GetPrizeNum(); ++i)
			{
				Msg.AddPrize(pPacket->GetPrize(i));

				//g_pLog->FastSaveLog(LOG_FILE_0,"LBLNewCheckPrize...GetPrize Acc=%s price=%s,num=%d",
				//	pPacket->GetAccount(), pPacket->GetPrize(i)->m_PrizeString, pPacket->GetPrize(i)->m_PrizeNum);
			}
		}

		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);
		}

		
		g_pLog->FastSaveLog(LOG_FILE_0,"LBLNewCheckPrize::Execute()...OK Acc=%s PUID=%d,Result = %d",
			pPacket->GetAccount(),pPacket->GetPacketIndex(),pPacket->GetPrizeResult());
	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}