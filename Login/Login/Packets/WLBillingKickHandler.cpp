#include "stdafx.h" 
#include "WLBillingKick.h"
#include "LBKickAll.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT WLBillingKickHandler::Execute(WLBillingKick* pPacket, Player* pPlayer )
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
			
			LBKickAll		Msg;
			Msg.SetZoneID(g_Config.m_WorldInfo.m_ZoneID);
			Msg.SetWorldID(pPacket->GetWorldID());
			Msg.SetReason("Login StartUpKick");
			pBillPlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog(LOG_FILE_0,"WLBillingKickHandler::Execute ...OK WorldID=%d",pPacket->GetWorldID());
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