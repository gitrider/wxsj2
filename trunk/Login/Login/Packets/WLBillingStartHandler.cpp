#include "stdafx.h" 
#include "WLBillingStart.h"
#include "LBBillingStart.h"
#include "BillingPlayer.h"
#include "ServerManager.h"

#include "Log.h"


UINT WLBillingStartHandler::Execute(WLBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		
		Assert(pPacket);
		

		BillingPlayer* pBillPlayer =	
			static_cast<BillingPlayer*>(g_pServerManager->GetServerPlayer(BILLING_PLAYER_ID));
		
		if(pBillPlayer->IsBillingPlayer() && pBillPlayer)
		{
			LBBillingStart	Msg;
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetCharName(pPacket->GetCharName());
			Msg.SetCharGUID((UINT)pPacket->GetCharGUID());
			Msg.SetCharLevel(pPacket->GetCharLevel());
			Msg.SetStartTime(pPacket->GetStartTime());
			Msg.SetPacketIndex(pPacket->GetPlayerID());
			pBillPlayer->SendPacket(&Msg);

			g_pLog->FastSaveLog(LOG_FILE_0,"WLBillingStartHandler::Execute ...OK acc=%s,charname=%s,charguid=%X,charlvl=%d,st=%d",
				pPacket->GetAccount(),pPacket->GetCharName(),pPacket->GetCharGUID(),pPacket->GetCharLevel(),pPacket->GetStartTime());
		}
		else
		{
			Assert(FALSE);
		}

		return PACKET_EXE_CONTINUE;

	}

	Assert(FALSE);
	
	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}