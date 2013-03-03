#include "stdafx.h" 
#include "BLRetAskPoint.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetPrize.h"


UINT BLRetAskPointHandler::Execute(BLRetAskPoint* pPacket, Player* pPlayer )
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
        Msg.SetPrizeResult(BAPR_SUCCESS);
		Msg.SetRetType(ASK_POINT);
		Msg.SetLeftPoint(pPacket->GetPoint());

		g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAskPoint...Acc=%s PUID=%d,LeftPoint = %d",
			pPacket->GetAccount(),pPacket->GetPacketIndex(),pPacket->GetPoint());

		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);
		}

		
		g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAskPoint::Execute()...OK Acc=%s PUID=%d,LeftPoint = %d",
			pPacket->GetAccount(),pPacket->GetPacketIndex(),pPacket->GetPoint());
	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}