#include "stdafx.h" 
#include "GWLWGCostLog.h"
#include "LBLCostLog.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "Log.h"


uint GWLWGCostLogHandler::Execute( GWLWGCostLog* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		Assert(pPacket);
		switch(pPacket->GetType()) 
		{
		case GWLWGCostLog::WL_COST_LOG:
			{
				BillingPlayer* pBillPlayer =	
					static_cast<BillingPlayer*>(g_pServerManager->GetServerPlayer(BILLING_PLAYER_ID));

				if(pBillPlayer->IsBillingPlayer() && pBillPlayer)
				{
					LBLCostLog	Msg;
					Msg.SetZoneID(g_Config.m_WorldInfo.m_ZoneID);
					Msg.SetPacketIndex(pPacket->GetLogPos());
					Msg.SetCostInfo(pPacket->GetLogInfo());

					pBillPlayer->SendPacket(&Msg);
					g_pLog->FastSaveLog(LOG_FILE_0,"GWLWGCostLogHandler::Execute ...Send to Billing OK key=%s",
						pPacket->GetLogInfo()->m_SerialKey);
				}
				else
				{
					Assert(FALSE);
				}
			}
			break;
		default:
			{
				g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:GWLWGCostLogHandler::Execute()...Invalid Type Find!");
			}
			break;
		}

		return PACKET_EXE_CONTINUE ;
	}

	Assert(FALSE);
__LEAVE_FUNCTION
	return PACKET_EXE_ERROR ;
}
