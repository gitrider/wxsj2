#include "stdafx.h" 
#include "BLRetKeepLive.h"
#include "BillingPlayer.h"
#include "Log.h"


UINT BLRetKeepLiveHandler::Execute(BLRetKeepLive* pPacket, Player* pPlayer )
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
			
			pBillPlayer->ClearKeepLiveSendNum();
			g_pLog->FastSaveLog(LOG_FILE_0,"BLRetKeepLiveHandler::Execute() ... OK!");
		}
		else
		{
			Assert(FALSE); //ʲô�ط��߼�������
		}


		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}