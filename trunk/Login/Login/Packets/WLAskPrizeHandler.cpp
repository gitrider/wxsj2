#include "stdafx.h" 
#include "WLAskPrize.h"
#include "LBPrize.h"
#include "LBAskPoint.h"
#include "LBAskBuy.h"
#include "LBLAskNewUserCard.h"
#include "LBLNewPrize.h"
#include "LBLNewCheckPrize.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "TimeManager.h"
#include "Log.h"


UINT WLAskPrizeHandler::Execute(WLAskPrize* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		Assert(pPacket);
		BillingPlayer* pBillPlayer =	
			static_cast<BillingPlayer*>(g_pServerManager->GetServerPlayer(BILLING_PLAYER_ID));

		if(pBillPlayer->IsBillingPlayer() && pBillPlayer)
		{
			//转发消息
			CHAR szSerialKey[MAX_PRIZE_SERIAL_LENGTH+1] = {0};
			GetSerialKey(szSerialKey,g_Config.m_WorldInfo.m_WorldID,(g_pServerManager->GetCurrentServerInfo())->m_ServerID);

			//1.抽CD-KEY
			if(pPacket->GetAskType() == ASK_PRIZE)
			{
				LBPrize	Msg;

				Msg.SetAccount(pPacket->GetAccName());
				Msg.SetCharName(pPacket->GetCharName());
				Msg.SetIPAddr(pPacket->GetHost());
				Msg.SetPrizeSerial(szSerialKey);
				Msg.SetPrizeType((USHORT)(pPacket->GetGoodsType()));

				Msg.SetPacketIndex(pPacket->GetPrizeUserID());
				pBillPlayer->SendPacket(&Msg);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLAskPrizeHandler...AskPrize acc=%s,char=%s,ip=%s,PUID=%d,key=%s",
					pPacket->GetAccName(),pPacket->GetCharName(),pPacket->GetHost(),pPacket->GetPrizeUserID(),szSerialKey);
			}
			//2.询问剩余点数
			else if(pPacket->GetAskType() == ASK_POINT)
			{
				LBAskPoint Msg;
				
				Msg.SetAccount(pPacket->GetAccName());
				Msg.SetCharName(pPacket->GetCharName());
				Msg.SetIPAddr(pPacket->GetHost());

				Msg.SetPacketIndex(pPacket->GetPrizeUserID());
				pBillPlayer->SendPacket(&Msg);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLAskPrizeHandler...AskPoint acc=%s,char=%s,ip=%s,PUID=%d",
					pPacket->GetAccName(),pPacket->GetCharName(),pPacket->GetHost(),pPacket->GetPrizeUserID());
			}
			//3.购买
			else if(pPacket->GetAskType() == ASK_BUY)
			{
				LBAskBuy Msg;

				Msg.SetAccount(pPacket->GetAccName());
				Msg.SetCharName(pPacket->GetCharName());
				Msg.SetIPAddr(pPacket->GetHost());
				Msg.SetPrizeSerial(szSerialKey);

				Msg.SetGoodsType(pPacket->GetGoodsType());
				Msg.SetGoodsNumber(pPacket->GetGoodsNumber());
				Msg.SetCostPoint(pPacket->GetCostPoint());
				
				Msg.SetPacketIndex(pPacket->GetPrizeUserID());
				pBillPlayer->SendPacket(&Msg);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLAskPrizeHandler...AskBuy acc=%s,char=%s,ip=%s,PUID=%d,key=%s,(%u:%u:%u)",
					pPacket->GetAccName(),pPacket->GetCharName(),pPacket->GetHost(),pPacket->GetPrizeUserID(),szSerialKey,
					pPacket->GetGoodsType(),pPacket->GetGoodsNumber(),pPacket->GetCostPoint());
			}
			//4.新手卡
			else if(pPacket->GetAskType() == ASK_NEWUSER_CARD)
			{
				LBLAskNewUserCard Msg;

				Msg.SetType(LBLAskNewUserCard::LB_NEWUSERCARD);
				Msg.SetAccount(pPacket->GetAccName());
				Msg.SetCharName(pPacket->GetCharName());
				Msg.SetIPAddr(pPacket->GetHost());
				Msg.SetNewUserCard(pPacket->GetNewUserCard());

				Msg.SetPacketIndex(pPacket->GetPrizeUserID());
				pBillPlayer->SendPacket(&Msg);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLAskPrizeHandler...ASK_NEWUSER_CARD acc=%s,char=%s,ip=%s,PUID=%d,card=%s",
					pPacket->GetAccName(),pPacket->GetCharName(),pPacket->GetHost(),pPacket->GetPrizeUserID(),pPacket->GetNewUserCard());
			}
			//5.新抽奖
			else if(pPacket->GetAskType() == ASK_NEWPRIZE)
			{
				LBLNewPrize Msg;

				Msg.SetType(LBLNewPrize::LB_NEW_PRIZE);
				Msg.SetAccount(pPacket->GetAccName());
				Msg.SetCharName(pPacket->GetCharName());
				Msg.SetIPAddr(pPacket->GetHost());
				Msg.SetPrizeSerial(szSerialKey);

				Msg.SetPacketIndex(pPacket->GetPrizeUserID());
				pBillPlayer->SendPacket(&Msg);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLAskPrizeHandler...ASK_NEWPRIZE acc=%s,char=%s,ip=%s,PUID=%d key=%s",
					pPacket->GetAccName(),pPacket->GetCharName(),pPacket->GetHost(),pPacket->GetPrizeUserID(),szSerialKey);
			}
			//6.新奖品查询
			else if(pPacket->GetAskType() == ASK_NEWCHECKPRIZE)
			{
				LBLNewCheckPrize Msg;

				Msg.SetType(LBLNewCheckPrize::LB_CHECK_PRIZE);
				Msg.SetAccount(pPacket->GetAccName());
				Msg.SetCharName(pPacket->GetCharName());
				Msg.SetIPAddr(pPacket->GetHost());
				
				Msg.SetPacketIndex(pPacket->GetPrizeUserID());
				pBillPlayer->SendPacket(&Msg);

				g_pLog->FastSaveLog(LOG_FILE_0,"WLAskPrizeHandler...ASK_NEWCHECKPRIZE acc=%s,char=%s,ip=%s,PUID=%d",
					pPacket->GetAccName(),pPacket->GetCharName(),pPacket->GetHost(),pPacket->GetPrizeUserID());
			}
			else
			{
				Assert(FALSE);
			}

			g_pLog->FastSaveLog(LOG_FILE_0,"WLAskPrizeHandler::Execute ...OK acc=%s,char=%s,ip=%s,PUID=%d",
				pPacket->GetAccName(),pPacket->GetCharName(),pPacket->GetHost(),pPacket->GetPrizeUserID());
		}
		else
		{
			Assert(FALSE);
		}

		return PACKET_EXE_CONTINUE;

	}

	Assert(FALSE);
	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}