#include "stdafx.h" 
#include "BLRetBillingStart.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetBillingStart.h"


UINT BLRetBillingStartHandler::Execute(BLRetBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	Assert(pPlayer);

	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);


		LWRetBillingStart	Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(pPacket->GetResult());
		Msg.SetPlayerID(pPacket->GetPacketIndex());

		BYTE bResult =	pPacket->GetResult();		
		switch(bResult)
		{
		case 0x00:
			{
				Msg.SetCode(pPacket->GetCode());
				//计费不成功消息
				g_pLog->FastSaveLog(LOG_FILE_0,"BLRetBillingStart::Execute()...Acc=%s Billing Fail Code = %d ",pPacket->GetAccount(),pPacket->GetCode());
			}
			break;
		case 0x01:
			{
				Msg.SetFeeType(pPacket->GetFeeType());
				Msg.SetLeftTime(pPacket->GetLeftTime());
				Msg.SetStorePoint(pPacket->GetStorePoint());
				Msg.SetUserPoint(pPacket->GetUserPoint());

				//计费成功
				g_pLog->FastSaveLog(LOG_FILE_0,"BLRetBillingStart::Execute()...Acc=%s Billing Start ft=%d,lt=%d",
					pPacket->GetAccount(),pPacket->GetFeeType(),pPacket->GetLeftTime());
			}
			break;
		default:
			Assert(FALSE);
		}
		//g_pServerManager->SendPacket(&Msg,WORLD_PLAYER_ID);
		ServerPlayer* pServerPlayer =	
		static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);

			g_pLog->FastSaveLog(LOG_FILE_0,"BLRetBillingStart::Execute()...Send Packet LWRetBillingStart");
				
		}

	}

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}