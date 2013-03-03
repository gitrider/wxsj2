#include "stdafx.h" 
#include "BLRetAccCheck.h"
#include "LWNotifyUser.h"
#include "Log.h"
#include "LBBillingStart.h"
#include "TimeManager.h"
#include "BillingPlayer.h"
#include "LBBillingEnd.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "PlayerPool.h"
#include "BLRetAuth.h"
#include "PacketFactoryManager.h"
#include "LoginPlayerManager.h"


UINT BLRetAccCheckHandler::Execute(BLRetAccCheck* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
				
		TID CurrentThreadID = MyGetCurrentThreadID();

		if(CurrentThreadID != g_pServerManager->m_ThreadID)
		{
			Assert(FALSE);
		}

		Assert(pPlayer);

		if(pPlayer->IsBillingPlayer())
		{
			BillingPlayer* pBillPlayer = static_cast<BillingPlayer*>(pPlayer);
			Assert(pBillPlayer);
			Assert(pPacket);
			
			PlayerID_t	PlayerID = pPacket->GetPacketIndex();


			LoginPlayer*	pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
			if( pLoginPlayer==NULL )
			{
				Assert(FALSE) ;
				return PACKET_EXE_CONTINUE ;
			}

			if(pLoginPlayer->GetPlayerStatus() != PS_LOGIN_WAIT_AUTH)
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler::Execute() Acc = %s PlayerID=%d... Wrong logic!",
					pPacket->GetAccount(), PlayerID );
				return  PACKET_EXE_CONTINUE;
			}

			BYTE stats = pPacket->GetResult();
			
			BLRetAuth*	pMsg = (BLRetAuth*)g_pPacketFactoryManager->CreatePacket(PACKET_BL_RETAUTH);
			Assert(pMsg);
			
			pMsg->SetAccount(pPacket->GetAccount());

			switch(stats) 
			{
			case ACR_NO_NEW_LICENSE:
				{
					INT nEnable = g_Config.m_LoginInfo.EnableLicense;
					if(nEnable > 0)
					{
						g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s No New License!",pPacket->GetAccount());
						pMsg->SetResult(LOGINR_NO_NEW_LICENSE);
						pMsg->SetFatigueSign(pPacket->GetFatigueSign());
						pMsg->SetPhoneBind(pPacket->GetPhoneBind());
						pMsg->SetIPBind(pPacket->GetIPBind());
						pMsg->SetMiBaoBind(pPacket->GetMiBaoBind());
						pMsg->SetMacBind(pPacket->GetMacBind());
						pMsg->SetRealNameBind(pPacket->GetRealNameBind());
						pMsg->SetInputNameBind(pPacket->GetInputNameBind());
						pMsg->SetTotalOnlineTime(pPacket->GetTotalOnlineTime());
						g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
						break;
					}
					else
					{
						g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s No New License BUT IGNORE!",pPacket->GetAccount());
					}
				}
				//break;	//不用断开，不需输入许可协议时按正常登录流程走[2007/12/11 YangJun]
			case ACR_SUCCESS:
				{
					pMsg->SetResult(LOGINR_SUCCESS);
					pMsg->SetFatigueSign(pPacket->GetFatigueSign());
					pMsg->SetPhoneBind(pPacket->GetPhoneBind());
					pMsg->SetIPBind(pPacket->GetIPBind());
					pMsg->SetMiBaoBind(pPacket->GetMiBaoBind());
					pMsg->SetMacBind(pPacket->GetMacBind());
					pMsg->SetRealNameBind(pPacket->GetRealNameBind());
					pMsg->SetInputNameBind(pPacket->GetInputNameBind());
					pMsg->SetTotalOnlineTime(pPacket->GetTotalOnlineTime());
					//ServerManager 中执行
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Success!",pPacket->GetAccount());
				}
				break;
			case ACR_NO_ACCOUNT:
				{
					pMsg->SetResult(LOGINR_AUTH_FAIL);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Not Exist!",pPacket->GetAccount());
				}
				break;
			case ACR_WRONG_PASSWORD:
				{
					pMsg->SetResult(LOGINR_AUTH_FAIL);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Wrong password!",pPacket->GetAccount());
				}
				break;
			case ACR_USER_ONLINE:
				{
					pMsg->SetResult(LOGINR_USER_ONLINE);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					//要求World踢人
					{
						LWNotifyUser* ptMsg = (LWNotifyUser*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_NOTIFYUSER);
						ptMsg->SetAccount(pPacket->GetAccount());
						ptMsg->SetNotifyStatus(LNOTIFY_KICK_REQUEST);
						ptMsg->SetUserKey(0);

						g_pServerManager->SendPacket(ptMsg, WORLD_PLAYER_ID);
						g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s send kick request to world!",pPacket->GetAccount());
					}
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Char Online!",pPacket->GetAccount());
					return PACKET_EXE_CONTINUE;
				}
				break;
			case ACR_OTHER_ONLINE:
				{
					pMsg->SetResult(LOGINR_OTHER_ONLINE);
					pMsg->SetServerIP(pPacket->GetServerIP());
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Other Char Online!",pPacket->GetAccount());
				}
				break;
			case ACR_OTHER_ERROR:
				{
					pMsg->SetResult(LOGINR_OTHER_ERROR);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Other Error!",pPacket->GetAccount());
				}
				break;
			case ACR_FORBIT:
				{
					pMsg->SetResult(LOGINR_FORBIT);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Forbid logon!",pPacket->GetAccount());
				}
				break;
			case ACR_NO_POINT:
				{
					pMsg->SetResult(LOGINR_NOT_EN_POINT);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Insufficient Point!",pPacket->GetAccount());
				}
				break;
			case ACR_REG_PASSPORT:
				{
					pMsg->SetResult(LOGINR_NEED_REG);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Need Reg Passport!",pPacket->GetAccount());
				}
				break;
			case ACR_ACT_ACCOUNT:
				{
					pMsg->SetResult(LOGINR_PASSPORTNEEDACT);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Need Active acc!",pPacket->GetAccount());
				}
				break;
			case ACR_REG_WEB_PASSPORT:
				{
					pMsg->SetResult(LOGINR_NEED_WEB_REG);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Need Web Reg Passport!",pPacket->GetAccount());
				}
				break;
			case ACR_WRONG_IP_ADDR:
				{
					pMsg->SetResult(LOGINR_WRONG_IP_ADDR);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s IP Is Invalid!",pPacket->GetAccount());
				}
				break;
			case ACR_LOCK_BY_MOBILE:
				{
					pMsg->SetResult(LOGINR_LOCK_BY_MOBILE);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Lock By Mobile!",pPacket->GetAccount());
				}
				break;
			case ACR_NO_MIBAO:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"MIBAO:BLRetAccCheckHandler... Acc = %s Not Reg MIBAO!",pPacket->GetAccount());
					pMsg->SetResult(LOGINR_CHECK_MIBAO_STATUS);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
				}
				break;
			case ACR_MIBAO_NOT_USE:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"MIBAO:BLRetAccCheckHandler... Acc = %s Reg MIBAO but not Use!",pPacket->GetAccount());
					pMsg->SetResult(LOGINR_CHECK_MIBAO_STATUS);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
				}
				break;
			case ACR_MIBAO_ERR:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"MIBAO:BLRetAccCheckHandler... Acc = %s Wrong MIBAO value!",pPacket->GetAccount());
					pMsg->SetResult(LOGINR_MIBAO_ERROR);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
				}
				break;
			case ACR_FATIGUE_ONLYONE:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"MIBAO:BLRetAccCheckHandler... Acc = %s Fatigue Only One!",pPacket->GetAccount());
					pMsg->SetResult(LOGINR_FATIGUE_ONLYONE);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
				}
				break;
			case ACR_MAC_ERR:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"MIBAO:BLRetAccCheckHandler... Acc = %s Wrong MAC md5 value!",pPacket->GetAccount());
					pMsg->SetResult(LOGINR_MAC_ERROR);
					g_pLoginPlayerManager->SendPacket(pMsg,PlayerID);
				}
				break;
			default:
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"BLRetAccCheckHandler... Acc = %s Unknow Result(0x%02X)!",pPacket->GetAccount(),stats);
					g_pPacketFactoryManager->RemovePacket(pMsg);	// fix memory leak[11/24/2006]
				}
			}
		
				
			//DiskLog(LOG_FILE_0,"BLRetAccCheckHandler::Execute() ... OK!");
		}
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}