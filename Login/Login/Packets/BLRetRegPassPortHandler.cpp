#include "stdafx.h" 
#include "BLRetAuth.h"
#include "BLRetRegPassPort.h"

#include "Log.h"
#include "TimeManager.h"
#include "BillingPlayer.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "LoginPlayerManager.h"


UINT BLRetRegPassPortHandler::Execute(BLRetRegPassPort* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		enum	RegResult
		{
			REGR_SUCCESS			=	0x01,			//成功
			REGR_EXIST_ACCOUNT		=	0x02,			//账户已存在
			REGR_USER_ONLINE		=	0x04,			//用户在本组在线
			REGR_OTHER_USER_ONLINE	=	0x05,			//用户在其他组在线
			REGR_OTHER_ERROR		=	0x06,			//其他错误
			REGR_FORBIT_LOGIN		=	0x07,			//禁止登录
			REGR_NOT_ENOUGH_POINT	=	0x08,			//点数不足
			REGR_NOT_ACTIVE			=	0xA0,			//用户没有激活
		};
		
		TID CurrentThreadID = MyGetCurrentThreadID();

		if(CurrentThreadID != g_pServerManager->m_ThreadID)
		{
			Assert(FALSE);
		}

		Assert(pPlayer);
		if(pPlayer->IsBillingPlayer())
		{
			BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);
			Assert(pPacket);
			
			PlayerID_t		PlayerID = pPacket->GetPacketIndex();
			LoginPlayer*	pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID) ;

			if( pLoginPlayer==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_CONTINUE;
			}

			if(pLoginPlayer->GetPlayerStatus() != PS_LOGIN_WAIT_REG_RESULT)
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"BLRetRegPassPortHandler::Execute() Acc = %s PlayerID=%d... Wrong logic!",
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
			BYTE stats = pPacket->GetAccResult();
			switch(stats)
			{
			case REGR_SUCCESS:				//成功
				{
					pMsg->SetResult(LOGINR_REG_SUCCESS);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s Reg Success!",pPacket->GetAccount());
				}
				break;
			case REGR_EXIST_ACCOUNT:		//账户已存在
				{
					pMsg->SetResult(LOGINR_REG_ALREADYEXIST);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s Already Reg acc!",pPacket->GetAccount());
				}
				break;
			case REGR_USER_ONLINE:			//用户在本组在线
				{
					pMsg->SetResult(LOGINR_REG_FAIL);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s User Online!",pPacket->GetAccount());
				}
				break;
			case REGR_OTHER_USER_ONLINE:	//用户在其他组在线
				{
					pMsg->SetResult(LOGINR_REG_FAIL);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s User Other Online!",pPacket->GetAccount());
				}
				break;
			case REGR_OTHER_ERROR:			//其他错误
				{
					pMsg->SetResult(LOGINR_REG_FAIL);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s Reg Failed Other Reason!",pPacket->GetAccount());
				}
				break;
			case REGR_FORBIT_LOGIN:			//禁止登录
				{
					pMsg->SetResult(LOGINR_REG_FAIL);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s Forbit login acc!",pPacket->GetAccount());
				}
				break;
			case REGR_NOT_ENOUGH_POINT:		//点数不足
				{
					pMsg->SetResult(LOGINR_REG_FAIL);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s No point acc!",pPacket->GetAccount());
				}
				break;
			case REGR_NOT_ACTIVE:			//用户没有激活
				{
					pMsg->SetResult(LOGINR_PASSPORTNEEDACT);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s Need Active acc!",pPacket->GetAccount());
				}
				break;
			default:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler... Acc = %s Unknow Result(0x%02X)!",pPacket->GetAccount(),stats);
					g_pPacketFactoryManager->RemovePacket(pMsg);
				}
				break;
			}

			g_pLog->FastSaveLog(LOG_FILE_0,"REG:BLRetRegPassPortHandler::Execute() Acc = %s... OK!", pPacket->GetAccount());
		}
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}