#include "stdafx.h" 
#include "LBAskRegAuth.h"
#include "LBRegPassPort.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "Log.h"
#include "Config.h"

UINT	LBAskRegAuthHandler::Execute(LBAskRegAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		TID CurrentThreadID = MyGetCurrentThreadID();
		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			Assert(pPacket);
			BillingPlayer* pBillPlayer= (BillingPlayer*)pPlayer;
			Assert(pBillPlayer);
			
			LBRegPassPort	Msg;
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetSuperPassWord(pPacket->GetSuperPassWord());
			Msg.SetPassWord(pPacket->GetPassWord());
			Msg.SetPacketIndex(pPacket->GetPlayerID());
			Msg.SetIPAddr(pPacket->GetIP());
			Msg.SetEmail(pPacket->GetEmail());

			pBillPlayer->SendPacket(&Msg);
		}
		else
		{
			AssertEx(FALSE,"LBAskRegAuthHandler 线程资源执行错误!");
		}
		
		g_pLog->FastSaveLog(LOG_FILE_0,"LBAskRegAuthHandler::Execute() Acc=%s IP=%s....OK", pPacket->GetAccount(),pPacket->GetIP() );

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}