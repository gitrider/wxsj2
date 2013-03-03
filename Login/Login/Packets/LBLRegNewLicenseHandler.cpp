#include "stdafx.h" 
#include "LBLRegNewLicense.h"
#include "BLRetAuth.h"

#include "Log.h"
#include "TimeManager.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "LoginPlayerManager.h"


UINT LBLRegNewLicenseHandler::Execute(LBLRegNewLicense* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	enum RegNewLicenseResult
	{
		RNLR_FAIL				= 0x00,		//协议注册通过
		RNLR_SUCCESS			= 0x01,		//协议注册失败
	};

	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID != g_pServerManager->m_ThreadID)
	{
		Assert(FALSE);
	}

	Assert(pPlayer);
	Assert(pPacket->GetType() == LBLRegNewLicense::BL_REGLICENSE);

	if(pPlayer->IsBillingPlayer())
	{
		BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);

		Assert(pBillPlayer);
		Assert(pPacket);

		PlayerID_t		PlayerID = pPacket->GetPacketIndex();
		LoginPlayer*	pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID) ;

		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE;
		}

		if(pLoginPlayer->GetPlayerStatus() != PS_LOGIN_WAIT_LICENSE_RESULT)
		{
			g_pLog->FastSaveLog(LOG_FILE_0,"LBLRegNewLicenseHandler::Execute() Acc = %s PlayerID=%d... Wrong logic!",
				pPacket->GetAccount(), PlayerID );
			return  PACKET_EXE_CONTINUE;
		}

		pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_AUTH);
		//ServerManager线程，通知LoginPlayerManager线程
		BLRetAuth*	pMsg = (BLRetAuth*)g_pPacketFactoryManager->CreatePacket(PACKET_BL_RETAUTH);
		Assert(pMsg);

		//玩家名称
		pMsg->SetAccount(pPacket->GetAccount());
		//返回值类型
		switch(pPacket->GetServerResult())
		{
		case RNLR_SUCCESS:	//注册协议成功
			{
				pMsg->SetResult(LOGINR_ONLY_SUCCESS);
				g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
				g_pLog->FastSaveLog(LOG_FILE_0,"LBLRegNewLicenseHandler... Acc = %s Reg New License Success!",pPacket->GetAccount());
			}
			break;
		case RNLR_FAIL:
		default:
			{
				pMsg->SetResult(LOGINR_NEW_LICENSE_FAIL);
				g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
				g_pLog->FastSaveLog(LOG_FILE_0,"LBLRegNewLicenseHandler... Acc = %s Reg New License Fail!",pPacket->GetAccount());
			}
			break;
		}

		g_pLog->FastSaveLog(LOG_FILE_0,"LBLRegNewLicense::Execute()...OK POS=%d ServerResult=%d",
			pPacket->GetPacketIndex(),pPacket->GetServerResult());
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}