#include "stdafx.h"
#include "LWRetBillingStart.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"
#include "TimeManager.h"
#include "WGNotifyUser.h"
#include "WLRetCharLogin.h"
#include "WLBillingEnd.h"

UINT	LWRetBillingStartHandler::Execute(LWRetBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		enum	BillStartResult
		{
				BSR_FAIL		=	0x00,
				BSR_SUCCESSFUL	=	0x01,
		};

		enum	BillStartFailCode
		{
				BSFC_NOT_LOGIN		=	0x01,
				BSFC_NO_POINT		=	0x02,
				BSFC_END_TIME		=	0x03,
				BSFC_USER_ONLINE	=	0x04,
		};		


		Assert(pPacket);
		ServerPlayer* pLoginPlayer = (ServerPlayer*)pPlayer ;
		Assert(pLoginPlayer);
		
		USER* pUser = g_pOnlineUser->FindUserByAccount(pPacket->GetAccount());
		if(pUser)
		{
			BYTE bResult = pPacket->GetResult();
			
			if(bResult ==BSR_FAIL)	//开始失败
			{
				WLRetCharLogin	Msg;
				Msg.SetResult(ASKCHARLOGIN_NOT_LOGON);	// 设置一个缺省的错误[1/26/2007]

				BYTE bCode	=		pPacket->GetCode();

				switch(bCode) 
				{
				case BSFC_NOT_LOGIN:  //没有验证密码成功
					{
						Msg.SetResult(ASKCHARLOGIN_NOT_LOGON);
						break;
					}
				case BSFC_NO_POINT:   //没有足够的点数
					{
						Msg.SetResult(ASKCHARLOGIN_NO_POINT);
						break;
					}
				case BSFC_END_TIME:   //月卡时间到期
					{
						Msg.SetResult(ASKCHARLOGIN_END_TIME);
						break;
					}
				case BSFC_USER_ONLINE://重发了还是没有billingend成功？
					{
						Msg.SetResult(ASKCHARLOGIN_CHARLIVING);
						break;
					}
				default:
					break;
				}	

				g_pLog->FastSaveLog(LOG_FILE_4, "LWRetBillingStart...acc=%s Fail,guid=%X",pPacket->GetAccount(),pUser->GetGUID()) ;

				//////////////////////////////////////////////////////////////////////////
				//应当把玩家踢掉
				//if( pUser->UserStatus()==US_HOLDUSERDATA )
				//{

				//	WLBillingEnd	BillEndMsg;
				//	BillEndMsg.SetAccount(pPacket->GetAccount());
				//	BillEndMsg.SetCharLevel(pUser->GetLevel());
				//	BillEndMsg.SetEndTime((UINT)g_pTimeManager->GetANSITime());
				//	BillEndMsg.SetStartTime((UINT)g_pTimeManager->GetANSITime());
				//	BillEndMsg.SetFeeType(BET_FORCE);//强制结束

				//	g_pOnlineUser->OnUserRemove( pUser);
				//	g_pOnlineUser->DelUser( pUser );
				//	pLoginPlayer->SendPacket(&BillEndMsg);
				//
				//}
				//else
				//{
				//	WGNotifyUser MsgNotify ;
				//	MsgNotify.SetGUID( pUser->GetGUID() ) ;
				//	MsgNotify.SetStatus( WGNotifyUser::NUS_NEED_SERVER_KICK ) ;
				//	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );
				//	if( pServerPlayer )
				//	{
				//		pServerPlayer->SendPacket( &MsgNotify ) ;
				//	}
				//}	
				
				//新踢人机制
				g_pOnlineUser->BeginKickUser(pUser);
				g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWRetBillingStart...acc=%s should kick",pPacket->GetAccount()) ;
							
				Msg.SetAccount(pPacket->GetAccount());
				Msg.SetPlayerID(pPacket->GetPlayerID());
				Msg.SetPlayerGUID(pUser->GetGUID());
				Msg.SetHoldStatus(TRUE);
				pLoginPlayer->SendPacket(&Msg);
				
			}
			
			if(bResult ==BSR_SUCCESSFUL)	//计费开始成功
			{
				BYTE	 bFeeType = pPacket->GetFeeType();
				UINT	 LeftTime = pPacket->GetLeftTime();
				UINT	 ShopPoint = pPacket->GetStorePoint();
				UINT	 UserPoint = pPacket->GetUserPoint();

				//pUser->SetFeeType(bFeeType);
				//pUser->SetLeftTime(LeftTime);
				//pUser->SetShopPoint(ShopPoint);
				//pUser->SetUserPoint(UserPoint);
				
				WLRetCharLogin Msg;
				Msg.SetResult(ASKCHARLOGIN_SUCCESS);
				Msg.SetAccount(pPacket->GetAccount());
				Msg.SetPlayerID(pPacket->GetPlayerID());
				Msg.SetPlayerGUID(pUser->GetGUID());
				Msg.SetHoldStatus(TRUE);
				Msg.SetPlayerServerID(pUser->GetServerID());
				pLoginPlayer->SendPacket(&Msg);


				g_pLog->FastSaveLog(LOG_FILE_4, "LWRetBillingStart...acc=%s Success,GUID=%X",pPacket->GetAccount(),pUser->GetGUID()) ;
			}
		}
		else
		{
			g_pLog->FastSaveLog(LOG_FILE_4, "LWRetBillingStart...acc=%s Can't find User",pPacket->GetAccount()) ;
		}


		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


