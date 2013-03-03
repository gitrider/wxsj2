#include "stdafx.h" 
#include "LBLAskNewUserCard.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetPrize.h"


UINT LBLAskNewUserCardHandler::Execute(LBLAskNewUserCard* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID != g_pServerManager->m_ThreadID)
	{
		Assert(FALSE);
	}

	Assert(pPlayer);
	Assert(pPacket->GetType() == LBLAskNewUserCard::BL_NEWUSERCARD);

	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);

		LWRetPrize	Msg;
		
		Msg.SetAccName(pPacket->GetAccount());
		Msg.SetPrizeUserID(pPacket->GetPacketIndex());
        Msg.SetPrizeResult(pPacket->GetPrizeResult());
		Msg.SetRetType(ASK_NEWUSER_CARD);

		if(/*BPR_SUCCESS*/0x00 == pPacket->GetPrizeResult())
		{
			for(INT i = 0; i < pPacket->GetPrizeNum(); ++i)
			{
				Msg.AddPrize(pPacket->GetPrize(i));

				g_pLog->FastSaveLog(LOG_FILE_0,"LBLAskNewUserCard...GetPrize Acc=%s price=%s,num=%d",
					pPacket->GetAccount(), pPacket->GetPrize(i)->m_PrizeString, pPacket->GetPrize(i)->m_PrizeNum);
			}
		}
		else if(/*BPR_ALREADY_GET*/0x02 == pPacket->GetPrizeResult())
		{
			for(INT i = 0; i < pPacket->GetPrizeNum(); ++i)
			{
				//已经领过的奖品就没啥意思了
				//Msg.AddPrize(pPacket->GetPrize(i));
				g_pLog->FastSaveLog(LOG_FILE_0,"LBLAskNewUserCard...AlreadyGetPrize Acc=%s price=%s,num=%d Drop",
					pPacket->GetAccount(), pPacket->GetPrize(i)->m_PrizeString, pPacket->GetPrize(i)->m_PrizeNum);
			}
		}

		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);
		}

		
		g_pLog->FastSaveLog(LOG_FILE_0,"LBLAskNewUserCard::Execute()...OK Acc=%s PUID=%d,Result = %d",
			pPacket->GetAccount(),pPacket->GetPacketIndex(),pPacket->GetPrizeResult());
	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}