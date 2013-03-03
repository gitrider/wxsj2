#include "stdafx.h"
#include "BLRetAuth.h"
#include "Log.h"
#include "BillingPlayer.h"

#include "BLRetBillingKeep.h"
#include "BLRetBillingStart.h"
#include "BLRetAccCheck.h"
#include "BLRetAskBuy.h"
#include "BLRetAskPoint.h"
#include "BLRetBillingEnd.h"
#include "BLRetKeepLive.h"
#include "BLRetConnect.h"
#include "BLRetKickAll.h"
#include "BLRetPrize.h"
#include "BLRetRegPassPort.h"
#include "LBAccCheck.h"
#include "LBAskBuy.h"
#include "LBAskPoint.h"
#include "LBAskRegAuth.h"
#include "LBAskRegLicense.h"
#include "LBBillingEnd.h"
#include "LBBillingKeep.h"
#include "LBBillingStart.h"
#include "LBConnect.h"
#include "LBKeepLive.h"
#include "LBKickAll.h"
#include "LBLAskMig.h"
#include "LBLAskNewUserCard.h"
#include "LBLCostLog.h"
#include "LBLNewCheckPrize.h"
#include "LBLNewPrize.h"
#include "LBLRegNewLicense.h"
#include "LBPrize.h"
#include "LBRegPassPort.h"


UINT BLRetAuthHandler::Execute(BLRetAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

UINT BLRetAccCheckHandler::Execute(BLRetAccCheck* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetAskBuyHandler::Execute(BLRetAskBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetAskPointHandler::Execute(BLRetAskPoint* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


UINT BLRetBillingEndHandler::Execute(BLRetBillingEnd* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetBillingKeepHandler::Execute(BLRetBillingKeep* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetBillingStartHandler::Execute(BLRetBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetConnectHandler::Execute(BLRetConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetKeepLiveHandler::Execute(BLRetKeepLive* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetKickALLHandler::Execute(BLRetKickALL* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetPrizeHandler::Execute(BLRetPrize* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT BLRetRegPassPortHandler::Execute(BLRetRegPassPort* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBAccCheckHandler::Execute( LBAccCheck* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		//目前不做任何处理,直接返回成功
		BLRetAccCheck Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(ACR_SUCCESS );
		Msg.SetFatigueSign(0);
		Msg.SetTotalOnlineTime(10);
		Msg.SetPhoneBind(0);
		Msg.SetIPBind(0);
		Msg.SetMiBaoBind(0);
		Msg.SetMacBind(0);
		Msg.SetRealNameBind(0);
		Msg.SetInputNameBind(0);
		Msg.SetPacketIndex(pPacket->GetPacketIndex());

		//使用普通接口发送
		((BillingPlayer*)pPlayer)->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBAskBuyHandler::Execute(LBAskBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBAskPointHandler::Execute(LBAskPoint* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBAskRegAuthHandler::Execute(LBAskRegAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBAskRegLicenseHandler::Execute(LBAskRegLicense* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBBillingEndHandler::Execute(LBBillingEnd* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBBillingKeepHandler::Execute(LBBillingKeep* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
UINT LBBillingStartHandler::Execute(LBBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		BLRetBillingStart Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetCode(0x00); 
		Msg.SetFeeType(0);
		Msg.SetResult(0x01);//succ
		Msg.SetStorePoint(1);
		Msg.SetUserPoint(1);
		Msg.SetLeftTime(10000);
		Msg.SetPacketIndex(pPacket->GetPacketIndex());
		
		((BillingPlayer*)pPlayer)->SendPacket(&Msg);

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
UINT LBConnectHandler::Execute(LBConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		BLRetConnect Msg;
		((BillingPlayer*)pPlayer)->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBKeepLiveHandler::Execute(LBKeepLive* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		BLRetKeepLive Msg;
		((BillingPlayer*)pPlayer)->SendPacket(&Msg);
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBKickAllHandler::Execute(LBKickAll* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBLAskMigHandler::Execute(LBLAskMig* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBLAskNewUserCardHandler::Execute(LBLAskNewUserCard* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBLCostLogHandler::Execute(LBLCostLog* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


UINT LBLNewCheckPrizeHandler::Execute(LBLNewCheckPrize* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


UINT LBLNewPrizeHandler::Execute(LBLNewPrize* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBLRegNewLicenseHandler::Execute(LBLRegNewLicense* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBPrizeHandler::Execute(LBPrize* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT LBRegPassPortHandler::Execute(LBRegPassPort* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}








