#include "stdafx.h" 
#include "WLBillingKeep.h"
#include "LBBillingKeep.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT WLBillingKeepHandler::Execute(WLBillingKeep* pPacket, Player* pPlayer )
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
			//转发消息
			LBBillingKeep	Msg;
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetCharLevel(pPacket->GetCharLevel());
			Msg.SetStartTime(pPacket->GetStartTime());
			Msg.SetEndTime(pPacket->GetEndTime());
			pBillPlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog(LOG_FILE_0,"WLBillingKeepHandler::Execute ...OK acc=%s,charlvl=%d,st=%d,et=%d",
			pPacket->GetAccount(),pPacket->GetCharLevel(),pPacket->GetStartTime(),pPacket->GetEndTime());

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