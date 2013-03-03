#include "stdafx.h" 
#include "BLRetBillingEnd.h"
#include "BillingPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "LWRetBillingEnd.h"



UINT BLRetBillingEndHandler::Execute(BLRetBillingEnd* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	enum BillingEndResult
	{
		BER_FAILURE		= 0x00,			//�Ʒѽ���ʧ��
		BER_SUCCESS		= 0x01,			//�Ʒѽ����ɹ�
	};
	
	Assert(pPlayer);
	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);
		
		BYTE bResult = pPacket->GetResult();
		
		
		LWRetBillingEnd	Msg;
		
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(pPacket->GetResult());
		ServerPlayer* pServerPlayer =	
			static_cast<ServerPlayer*>(g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID));
		if(pServerPlayer)
		{
			pServerPlayer->SendPacket(&Msg);
		}

		//�Ʒѽ����ɹ���Ϣ
		g_pLog->FastSaveLog(LOG_FILE_0,"BLRetBillingEnd::Execute()...Acc=%s Billing End Result = %d ",
		pPacket->GetAccount(),pPacket->GetResult());
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}