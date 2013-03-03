#include "stdafx.h" 
#include "BLRetAskBuy.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetPrize.h"


UINT BLRetAskBuyHandler::Execute(BLRetAskBuy* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID != g_pServerManager->m_ThreadID)
	{
		Assert(FALSE);
	}

	Assert(pPlayer);
	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);

		LWRetPrize	Msg;
		
		Msg.SetAccName(pPacket->GetAccount());
		Msg.SetPrizeUserID(pPacket->GetPacketIndex());
        Msg.SetPrizeResult(pPacket->GetBuyResult());
		UINT uLeftPoint = (pPacket->GetLeftPoint() >0)?pPacket->GetLeftPoint():0;
		Msg.SetLeftPoint(uLeftPoint);
		Msg.SetRetType(ASK_BUY);

		g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAskBuy...Acc=%s PUID=%d,Result = %d,LeftPoint=%u",
			pPacket->GetAccount(),pPacket->GetPacketIndex(),pPacket->GetBuyResult(),uLeftPoint);

		if(/*BABR_SUCCESS*/0x00 == pPacket->GetBuyResult())
		{
			for(INT i = 0; i < pPacket->GetGoodsNum(); ++i)
			{
				Msg.AddPrize(pPacket->GetGoods(i));

				g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAskBuy...Buy Acc=%s (%u:%u)",
					pPacket->GetAccount(), pPacket->GetGoods(i)->m_BuyInt, pPacket->GetGoods(i)->m_BuyNumber);
			}
		}
		else if(/*BABR_ALREADY_GET*/0x01 == pPacket->GetBuyResult())
		{
			for(INT i = 0; i < pPacket->GetGoodsNum(); ++i)
			{
				//已经购买过的商品就没啥意思了
				//Msg.AddPrize(pPacket->GetGoods(i));
				g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAskBuy...AlreadyBuy Acc=%s (%u:%u) Drop",
					pPacket->GetAccount(), pPacket->GetGoods(i)->m_BuyInt, pPacket->GetGoods(i)->m_BuyNumber);
			}
		}

		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);
		}

		
		g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAskBuy::Execute()...OK Acc=%s BuySerial=%s PUID=%d,Result = %d",
			pPacket->GetAccount(),pPacket->GetBuySerial(),pPacket->GetPacketIndex(),pPacket->GetBuyResult());
	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}