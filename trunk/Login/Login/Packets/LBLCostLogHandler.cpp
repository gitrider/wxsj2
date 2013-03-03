#include "stdafx.h" 
#include "LBLCostLog.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "GWLWGCostLog.h"


UINT LBLCostLogHandler::Execute(LBLCostLog* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID != g_pServerManager->m_ThreadID)
	{
		Assert(FALSE);
	}

	Assert(pPlayer);
	Assert(pPacket->GetType() == LBLCostLog::BL_COST_LOG);

	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);

		GWLWGCostLog	costMsg;
		costMsg.SetType(GWLWGCostLog::LW_COST_LOG);
        costMsg.SetLogPos(pPacket->GetPacketIndex());
		costMsg.SetResult(pPacket->GetResult());
		costMsg.SetBillingRetKey(pPacket->GetSerial());

		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&costMsg);
		}

		
		g_pLog->FastSaveLog(LOG_FILE_0,"LBLCostLog::Execute()...OK POS=%d Result=%d key=%s",
			pPacket->GetPacketIndex(),pPacket->GetResult(),pPacket->GetSerial());
	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}