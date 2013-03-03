#include "stdafx.h" 
#include "BLRetConnect.h"
#include "BillingPlayer.h"
#include "Log.h"


UINT BLRetConnectHandler::Execute(BLRetConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		Assert(pPlayer);
		
		if(pPlayer->IsBillingPlayer())
		{
			
			BillingPlayer * pBillPlayer = static_cast<BillingPlayer*>(pPlayer);
			if(pBillPlayer==NULL)
			{
				Assert(FALSE);
				return PACKET_EXE_CONTINUE ;
			}
			pBillPlayer->SetPlayerStatus(PS_BILLING_CONNECT);
			
			g_pLog->FastSaveLog(LOG_FILE_0,"BLRetConnectHandler::Execute() ... OK!");
		}
		else
		{
			Assert(FALSE); //什么地方逻辑错误了
		}


		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}