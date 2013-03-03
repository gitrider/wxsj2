#include "stdafx.h"
#include "LBAskAuth.h"
#include "BLRetAuth.h"
#include "ServerPlayer.h"
#include "AccountTable.h"
#include "Log.h"

UINT	LBAskAuthHandler::Execute(LBAskAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		Assert(pPlayer);
		Assert(pPacket);

		//BLRetAuth	Msg;
		//
		//Msg.SetAccount(pPacket->GetAccount());
		//Msg.SetPlayerID(pPacket->GetPlayerID());


		//if(g_Account.Check(pPacket->GetAccount(),pPacket->GetPassWord()))
		//{
		//	
		//	Msg.SetResult(LOGINR_SUCCESS);
		//	g_pLog->SaveLog(BILLING_LOGFILE,"Account = %s Check Password SUCCESS!",pPacket->GetAccount());

		//}
		//else
		//{
		//	Msg.SetResult(LOGINR_AUTH_FAIL);
		//	g_pLog->SaveLog(BILLING_LOGFILE,"Account = %s Check Password FAIL!",pPacket->GetAccount());
		//}
		
		
		//((ServerPlayer*)pPlayer)->SendPacket(&Msg);

		//目前不做任何处理,直接返回成功
		BLRetAuth Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_SUCCESS );
		Msg.SetPlayerID(pPacket->GetPlayerID());
		Msg.SetFatigueSign(0);
		Msg.SetTotalOnlineTime(10);
		Msg.SetPhoneBind(0);
		Msg.SetIPBind(0);
		Msg.SetMiBaoBind(0);
		Msg.SetMacBind(0);
		Msg.SetRealNameBind(0);
		Msg.SetInputNameBind(0);

		//使用普通接口发送
		((ServerPlayer*)pPlayer)->SendPacket(&Msg);

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}