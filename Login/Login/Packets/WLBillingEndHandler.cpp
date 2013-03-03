#include "stdafx.h" 
#include "WLBillingEnd.h"
#include "LBBillingEnd.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT WLBillingEndHandler::Execute(WLBillingEnd* pPacket, Player* pPlayer )
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
			LBBillingEnd	Msg;
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetCharLevel(pPacket->GetCharLevel());
			Msg.SetStartTime(pPacket->GetStartTime());
			Msg.SetEndTime(pPacket->GetEndTime());
			pBillPlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog(LOG_FILE_0,"WLBillingEndHandler::Execute ...OK acc=%s,charlvl=%d,st=%d",
				pPacket->GetAccount(),pPacket->GetCharLevel(),pPacket->GetStartTime());
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