#include "stdafx.h"
#include "LWNotifyUser.h"
#include "WLNotifyUser.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"
//#include "WMCmd.h"
//#include "MotherClient.h"


UINT	LWNotifyUserHandler::Execute(LWNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	Assert(pPacket);
	ServerPlayer* pLoginPlayer = (ServerPlayer*)pPlayer ;
	Assert(pLoginPlayer);
	//MagicInt不同的处理[2007/11/12 YangJun]
	// CRC不同的处理[4/30/2007]
	//if(pPacket->GetNotifyStatus() == LNOTIFY_CRC_ERR || pPacket->GetNotifyStatus() == LNOTIFY_MAGICINT_ERR)
	//{
	//	WMCmd Msg2M ;
	//	Msg2M.GetCMDStruct()->SetStructType( CMD_STRUCT_NOTIFY ) ;
	//	Msg2M.GetCMDStruct()->SetControlID( -1 ) ;
	//	Msg2M.GetCMDStruct()->m_Notify.CleanUp() ;
	//	Msg2M.GetCMDStruct()->m_Notify.m_WorldID = g_Config.m_WorldInfo.m_ZoneID*1000+g_Config.m_WorldInfo.m_WorldID ;
	//	Msg2M.GetCMDStruct()->m_Notify.m_nParam0 = (pPacket->GetNotifyStatus() == LNOTIFY_CRC_ERR)?4:6 ;//CRC验证有错误(4),MagicInt验证有错误(6)
	//	Msg2M.GetCMDStruct()->m_Notify.m_nParam1 = pPacket->GetLoadCrc() ;
	//	Msg2M.GetCMDStruct()->m_Notify.m_nParam2 = pPacket->GetSaveCrc() ;
	//	sprintf( Msg2M.GetCMDStruct()->m_Notify.m_szNotify, 
	//		"%X,%s,%d,%d,%u",
	//		pPacket->GetCharGUID(),
	//		pPacket->GetAccount(),
	//		pPacket->GetLoadCrc(),
	//		pPacket->GetSaveCrc(),
	//		pPacket->GetUserKey()
	//		) ;
	//	Msg2M.GetCMDStruct()->m_Notify.m_nNotifySize = (INT)(strlen(Msg2M.GetCMDStruct()->m_Notify.m_szNotify)) ;
	//	g_pMotherClient->SendPacket( &Msg2M, INVALID_ID ) ;
	//	//酌情通知Mother
	//	g_pLog->FastSaveLog(LOG_FILE_4, "LWNotifyUserHandler...%s Error %s",
	//		(pPacket->GetNotifyStatus() == LNOTIFY_CRC_ERR)?"CRC":"MAGICINT",Msg2M.GetCMDStruct()->m_Notify.m_szNotify);
	//	return PACKET_EXE_CONTINUE;
	//}
	////////////////////////////////////////////////////////////////////////////
	USER* pUser = g_pOnlineUser->FindUserByAccount(pPacket->GetAccount());
	
	if(NULL == pUser)
	{//没有发现玩家的情况下
		WLNotifyUser nLMsg;
		nLMsg.SetAccName(pPacket->GetAccount());
		nLMsg.SetUserKey(pPacket->GetUserKey());

		if(pPacket->GetNotifyStatus() == LNOTIFY_ASK_REQUEST)
		{
			nLMsg.SetNotifyStatus(WNOTIFY_ASK_OK);
			g_pLog->FastSaveLog(LOG_FILE_4, "LWNotifyUserHandler...acc=%s ask status ok!",
				pPacket->GetAccount());
		}
		else if(pPacket->GetNotifyStatus() == LNOTIFY_KICK_NOUSR||
				pPacket->GetNotifyStatus() == LNOTIFY_KICK_OK)
		{
			g_pLog->FastSaveLog(LOG_FILE_4, "LWNotifyUserHandler...acc=%s Can't find Usr, Stop Kick!",
				pPacket->GetAccount());
			// World没有发现这个玩家的时候，通知Billing计费结束包[1/22/2007]
			//g_pOnlineUser->SendForceBillingEnd(pPacket->GetAccount(),1);
			return PACKET_EXE_CONTINUE;
		}
		else
		{
			nLMsg.SetNotifyStatus(WNOTIFY_KICK_NOUSR);
			g_pLog->FastSaveLog(LOG_FILE_4, "LWNotifyUserHandler...acc=%s Can't find Usr, Send BillingEnd to Billing!",
				pPacket->GetAccount());
			// World没有发现这个玩家的时候，通知Billing计费结束包[1/22/2007]
			g_pOnlineUser->SendBillingEnd(pPacket->GetAccount(),1);
		}
		pLoginPlayer->SendPacket(&nLMsg);

		return PACKET_EXE_CONTINUE;
	}
	else
	{//有玩家的情况下
		Assert(pPacket->GetNotifyStatus() != LNOTIFY_NONE);
		//是否是正在踢人中的玩家
		if(g_pOnlineUser->IsKickUser(pUser))
		{
			BYTE bKickRet = SK_NONE;
			//修改踢人状态
			if(pPacket->GetNotifyStatus() == LNOTIFY_KICK_OK)		bKickRet = SK_KICK_OK;
			if(pPacket->GetNotifyStatus() == LNOTIFY_KICK_NOUSR)	bKickRet = SK_KICK_NOUSR;
			if(SK_NONE != bKickRet)
			{
				if(g_pOnlineUser->EndKickUser(pUser, pLoginPlayer->PlayerID(),bKickRet))
				{
					g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWNotifyUserHandler...GUID=%X acc=%s kick ok!",
						pUser->GetGUID(), pPacket->GetAccount());
				}
				else
				{
					g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWNotifyUserHandler...GUID=%X acc=%s login kick ok!",
						pUser->GetGUID(), pPacket->GetAccount());
				}
				return PACKET_EXE_CONTINUE;
			}
			
			//其他提醒信息都需要重发踢人包给Login
			WLNotifyUser nLMsg;
			nLMsg.SetAccName(pPacket->GetAccount());
			nLMsg.SetUserKey(pPacket->GetUserKey());
			nLMsg.SetNotifyStatus(WNOTIFY_KICK_REQUEST);
			pLoginPlayer->SendPacket(&nLMsg);
			g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWNotifyUserHandler...GUID=%X acc=%s kick send to login ONLY!",
				pUser->GetGUID(), pPacket->GetAccount());

			return PACKET_EXE_CONTINUE;
		}
		
		//忽略无用的踢人结果消息包
		if(pPacket->GetNotifyStatus() == LNOTIFY_KICK_OK || pPacket->GetNotifyStatus() == LNOTIFY_KICK_NOUSR)
		{
			g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWNotifyUserHandler...GUID=%X acc=%s kick result IGNORE!",
				pUser->GetGUID(), pPacket->GetAccount());
			return PACKET_EXE_CONTINUE;
		}

		//开始踢人
		BOOL bKickAction = g_pOnlineUser->BeginKickUser(pUser);
		g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWNotifyUserHandler...GUID=%X acc=%s kick begin <%s>!",
			pUser->GetGUID(), pPacket->GetAccount(),(bKickAction)?"Success":"Fail");

		//为了确保有回应，踢人开始失败时直接发送一个踢人包
		if(FALSE == bKickAction)
		{
			WLNotifyUser nLMsg;
			nLMsg.SetAccName(pPacket->GetAccount());
			nLMsg.SetUserKey(pPacket->GetUserKey());
			nLMsg.SetNotifyStatus(WNOTIFY_KICK_REQUEST);
			pLoginPlayer->SendPacket(&nLMsg);
		}

		return PACKET_EXE_CONTINUE;
	}
__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}


