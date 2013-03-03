#include "stdafx.h" 
#include "BLRetBillingKeep.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetBillingKeep.h"


UINT BLRetBillingKeepHandler::Execute(BLRetBillingKeep* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
	Assert(pPlayer);
	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);

		LWRetBillingKeep	Msg;

		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(pPacket->GetResult());
		Msg.SetLeftTime(pPacket->GetTimeLeft());
		Msg.SetStorePoint(pPacket->GetStorePoint());
		Msg.SetUserPoint(pPacket->GetUserPoint());

		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);
		}

		
		g_pLog->FastSaveLog(LOG_FILE_0,"BLRetBillingKeep::Execute()...Acc=%s BillingKeep Result = %d,lt=%d,sp=%d,up=%d",
		pPacket->GetAccount(),pPacket->GetResult(),pPacket->GetTimeLeft(),pPacket->GetStorePoint(),pPacket->GetUserPoint());
	}


		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}