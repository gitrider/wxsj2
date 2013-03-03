#include "stdafx.h" 
#include "LBAskAuth.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "Log.h"
#include "LBAccCheck.h"
#include "Config.h"

UINT	LBAskAuthHandler::Execute(LBAskAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		TID CurrentThreadID = MyGetCurrentThreadID();
		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			Assert(pPacket);
			BillingPlayer* pBillPlayer= (BillingPlayer*)pPlayer;
			Assert(pBillPlayer);
			
			LBAccCheck	Msg;
			Msg.SetAccount(pPacket->GetAccount());
			
			Msg.SetPassWord(pPacket->GetPassWord());
			Msg.SetUserLevel(1);
			Msg.SetIPAddr(pPacket->GetIPAddr());
			Msg.SetPacketIndex(pPacket->GetPlayerID());
			for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
			{
				Msg.SetMiBaoKey(i,pPacket->GetMiBaoKey(i));
				Msg.SetMiBaoValue(i,pPacket->GetMiBaoValue(i));
			}
			Msg.SetMacAddr(pPacket->GetMacAddr());
			pBillPlayer->SendPacket(&Msg);
		}
		else
		{
			AssertEx(FALSE,"LBAskAuthHandler 线程资源执行错误!");
		}
		
		g_pLog->FastSaveLog(LOG_FILE_0,"LBAskAuthHandler::Execute() Acc=%s....OK", pPacket->GetAccount() );

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}