#include "stdafx.h" 
#include "LBAskRegLicense.h"
#include "LBLRegNewLicense.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "Log.h"
#include "Config.h"

UINT	LBAskRegLicenseHandler::Execute(LBAskRegLicense* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		TID CurrentThreadID = MyGetCurrentThreadID();
		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			Assert(pPacket);
			BillingPlayer* pBillPlayer= (BillingPlayer*)pPlayer;
			Assert(pBillPlayer);
			
			LBLRegNewLicense	Msg;
			Msg.SetType(LBLRegNewLicense::LB_REGLICENSE);
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetPacketIndex(pPacket->GetPlayerID());
			Msg.SetClientResult(pPacket->GetUserAnswer());

			pBillPlayer->SendPacket(&Msg);
		}
		else
		{
			AssertEx(FALSE,"LBAskRegLicenseHandler 线程资源执行错误!");
		}
		
		g_pLog->FastSaveLog(LOG_FILE_0,"LBAskRegLicenseHandler::Execute() Acc=%s....OK", pPacket->GetAccount());

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}