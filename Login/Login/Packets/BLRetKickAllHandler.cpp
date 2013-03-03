#include "stdafx.h" 
#include "BLRetKickAll.h"
#include "BillingPlayer.h"
#include "Log.h"


UINT BLRetKickALLHandler::Execute(BLRetKickALL* pPacket, Player* pPlayer )
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
			
			Assert(pPacket);

			switch(pPacket->GetResult())
			{
			case 0x00:
				{

					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetKickAllHandler::Execute() ... Fail!");
				}
				break;
			case 0x01:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetKickAllHandler::Execute() ... Success!");
				}
				break;
			default:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetKickAllHandler::Execute() ... UnKnow Result!");
				}
				break;
			}
		}
		else
		{
			Assert(FALSE); //什么地方逻辑错误了
		}


		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}