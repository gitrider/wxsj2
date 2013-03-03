#include "stdafx.h"
#include "LWRetPrize.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"
#include "TimeManager.h"
//#include "WGRetPrize.h"
//#include "WMCmd.h"
//#include "MotherClient.h"


UINT	LWRetPrizeHandler::Execute(LWRetPrize* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
		
	Assert(pPacket);
//	ServerPlayer* pLoginPlayer = (ServerPlayer*)pPlayer ;
//	Assert(pLoginPlayer);
//	
//	CHAR	charname[MAX_CHARACTER_NAME+1] = {0};
//	WGRetPrize retMsg;
//	retMsg.SetRetType(pPacket->GetRetType());
//
//	PRIZE_USER* pPrizeUser = NULL;
//	USER* pUser = NULL;
//	if( pPacket->GetRetType() == ASK_PRIZE || pPacket->GetRetType() == ASK_BUY ||
//		pPacket->GetRetType() == ASK_NEWUSER_CARD || pPacket->GetRetType() == ASK_NEWPRIZE)
//	{
//		pPrizeUser = g_pOnlineUser->FindPrizeUserByID(pPacket->GetPrizeUserID());
//		//查找抽奖信息
//		if(NULL == pPrizeUser)
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...PUID=%d,acc=%s,result=%d Can't find Prize User",
//				pPacket->GetPrizeUserID(),pPacket->GetAccName(),pPacket->GetPrizeResult()) ;
//			goto LOST_PRIZE_LOG;
//		}
//		
//		//获得玩家名称
//		strncpy(charname,pPrizeUser->GetName(),MAX_CHARACTER_NAME);
//	    
//		//检查抽奖状态
//		if(PUS_SEND_TO_LOGIN != pPrizeUser->GetStatus())
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Prize User should abandon",
//				pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//			g_pOnlineUser->DelPrizeUser(pPrizeUser);
//			goto LOST_PRIZE_LOG;
//		}
//	}
//
//	//获得玩家在线信息
//	//USER* pUser = NULL;
//	if( pPacket->GetRetType() == ASK_PRIZE || pPacket->GetRetType() == ASK_BUY ||
//		pPacket->GetRetType() == ASK_NEWUSER_CARD || pPacket->GetRetType() == ASK_NEWPRIZE)
//	{
//		pUser = g_pOnlineUser->FindUser(pPrizeUser->GetGUID());
//	}
//	else if(pPacket->GetRetType() == ASK_POINT || pPacket->GetRetType() == ASK_NEWCHECKPRIZE)
//	{
//		pUser = g_pOnlineUser->FindUserByAccount(pPacket->GetAccName());
//	}
//	else
//	{
//		Assert(FALSE);
//	}
//
//	if(NULL == pUser)
//	{
//		if( pPacket->GetRetType() == ASK_PRIZE || pPacket->GetRetType() == ASK_BUY ||
//			pPacket->GetRetType() == ASK_NEWUSER_CARD || pPacket->GetRetType() == ASK_NEWPRIZE)
//		{
//
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Can't find User",
//				pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//			//抽奖和购买，需要去除额外信息
//			g_pOnlineUser->DelPrizeUser(pPrizeUser);
//		}
//		else if(pPacket->GetRetType() == ASK_POINT)
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...ASK_POINT...acc=%s,PUID=%d,result=%d,LeftPoin=%u Can't find User",
//				pPacket->GetAccName(),pPacket->GetPrizeUserID(),pPacket->GetPrizeResult(),pPacket->GetLeftPoint());
//		}
//		else if(pPacket->GetRetType() == ASK_NEWCHECKPRIZE)
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...ASK_NEWCHECKPRIZE...acc=%s,PUID=%d,result=%d Can't find User",
//				pPacket->GetAccName(),pPacket->GetPrizeUserID(),pPacket->GetPrizeResult());
//		}
//		goto LOST_PRIZE_LOG;
//	}
//
//	//获得玩家所在Server
//	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer(pUser->GetServerID());
//	if(NULL == pServerPlayer)
//	{
//		if( pPacket->GetRetType() == ASK_PRIZE || pPacket->GetRetType() == ASK_BUY ||
//			pPacket->GetRetType() == ASK_NEWUSER_CARD || pPacket->GetRetType() == ASK_NEWPRIZE)
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Can't find User's Server",
//				pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//			//抽奖和购买，需要去除额外信息
//			g_pOnlineUser->DelPrizeUser(pPrizeUser);
//		}
//		else if(pPacket->GetRetType() == ASK_POINT)
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...ASK_POINT...acc=%s,PUID=%d,result=%d,LeftPoin=%u Can't find User's Server",
//				pPacket->GetAccName(),pPacket->GetPrizeUserID(),pPacket->GetPrizeResult(),pPacket->GetLeftPoint());
//		}
//		else if(pPacket->GetRetType() == ASK_NEWCHECKPRIZE)
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...ASK_NEWCHECKPRIZE...acc=%s,PUID=%d,result=%d Can't find User's Server",
//				pPacket->GetAccName(),pPacket->GetPrizeUserID(),pPacket->GetPrizeResult());
//		}
//
//		goto LOST_PRIZE_LOG;
//	}
//
//	//玩家状态处理
//	if( pPacket->GetRetType() == ASK_PRIZE || pPacket->GetRetType() == ASK_BUY ||
//		pPacket->GetRetType() == ASK_NEWUSER_CARD || pPacket->GetRetType() == ASK_NEWPRIZE)
//	{
//		retMsg.SetGUID(pPrizeUser->GetGUID());
//		//检查当前玩家的状态是否还安全
//		if( US_NORMAL != pUser->UserStatus() ||								//玩家当前状态
//			pUser->GetSceneID() != pPrizeUser->GetSceneID() ||				//玩家当前场景
//			pUser->GetPlayerID() != pPrizeUser->GetPlayerID() ||			//玩家当前服务器ID
//			0 != strcmp(pUser->GetName(),pPrizeUser->GetName()) ||			//玩家角色名称
//			pUser->GetGUID() != pPrizeUser->GetGUID() ||					//玩家GUID
//			0 != strcmp(pUser->GetAccount(),pPrizeUser->GetAccName()) )		//玩家帐号名称
//		{
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...INF0:PUID=%d,GUID=%X,ip=%s,result=%d Prize User and User Info Diff",
//				pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...INF1:PUSR,GUID=%X,SceneId=%d,PlayerId=%d,Acc=%s,Char=%s Prize User Info",
//				pPrizeUser->GetGUID(),pPrizeUser->GetSceneID(),pPrizeUser->GetPlayerID(),pPrizeUser->GetAccName(),pPrizeUser->GetName());
//
//			g_pLog->SaveLog(LOG_FILE_4, "ERROR:LWRetPrize...INF2:USR,GUID=%X,SceneId=%d,PlayerId=%d,Acc=%s,Char=%s,Stat=%d User Info",
//				pUser->GetGUID(),pUser->GetSceneID(),pUser->GetPlayerID(),pUser->GetAccount(),pUser->GetName(),pUser->UserStatus());
//
//			retMsg.SetRetResult(UPR_ERR_CANT_NOW);
//			pServerPlayer->SendPacket(&retMsg);
//
//			g_pOnlineUser->DelPrizeUser(pPrizeUser);
//			goto LOST_PRIZE_LOG;
//		}
//	}
//	
//	//根据pPacket里的状态来决定行为
//	//1.抽CD-KEY
//	if(pPacket->GetRetType() == ASK_PRIZE)
//	{
//		switch(pPacket->GetPrizeResult()) 
//		{
//			case BPR_SUCCESS:			//0x00,成功
//				{
//					retMsg.SetRetResult(UPR_SUCCESS);
//					g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Success",
//						pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//					for(UINT i = 0; i < pPacket->GetPrizeNum(); ++i)
//					{
//						_BUY_DATA* pData = pPacket->GetPrize(i);
//						if(pData)
//						{
//							retMsg.AddPrize(pData);
//							g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...Success acc=%s char=%s prize=%s,num=%d",
//								pPrizeUser->GetAccName(),pPrizeUser->GetName(), pData->m_BuyString, pData->m_BuyNumber);
//						}
//					}
//				}
//				break;
//			case BPR_ALREADY_GET:		//0x02,已经领奖
//				{
//					retMsg.SetRetResult(UPR_ERR_ALREADYGET_PRIZE);
//
//					g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d already get prize",
//						pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//				}
//				break;
//			case BPR_NO_PRIZE:			//0x05,没有中奖
//				{
//					retMsg.SetRetResult(UPR_ERR_NO_PRIZE);
//
//					g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d no prize",
//						pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//				}
//				break;
//			case BPR_EXPIRE_PRIZE:		//0x06,抽奖过期
//				{
//					retMsg.SetRetResult(UPR_ERR_EXPIRE_PRIZE);
//
//					g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d expire prize",
//						pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//				}
//				break;
//			case BPR_OTHER_ERR:			//0x03,其他错误
//			case BPR_INVALID_ACC:		//0x04,帐号非法
//			default:
//				{
//					retMsg.SetRetResult(UPR_ERR_CANT_NOW);
//
//					g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d other err",
//						pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//				}
//				break;
//		}
//	}
//	//2.查询人物点数
//	else if(pPacket->GetRetType() == ASK_POINT)
//	{
//		retMsg.SetGUID(pUser->GetGUID());
//		switch(pPacket->GetPrizeResult()) 
//		{
//		case BAPR_SUCCESS:		//0x00,查询点数成功
//			{
//				retMsg.SetLeftPoint(pPacket->GetLeftPoint());
//				retMsg.SetRetResult(UPR_ASKPOINT_SUCCESS);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d leftpoint=%u Success",
//					pPacket->GetPrizeUserID(),pUser->GetGUID(),pUser->GetHost(),pPacket->GetPrizeResult(),pPacket->GetLeftPoint());
//			}
//			break;
//		default:
//			{
//				retMsg.SetRetResult(UPR_ERR_CANT_NOW);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d other err",
//					pPacket->GetPrizeUserID(),pUser->GetGUID(),pUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		}
//	}
//	//3.购买
//	else if(pPacket->GetRetType() == ASK_BUY)
//	{
//		switch(pPacket->GetPrizeResult()) 
//		{
//		case BABR_SUCCESS:			//0x00,成功
//			{
//				retMsg.SetLeftPoint(pPacket->GetLeftPoint());
//				retMsg.SetRetResult(UPR_ASKBUY_SUCCESS);
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Success",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//				for(UINT i = 0; i < pPacket->GetPrizeNum(); ++i)
//				{
//					_BUY_DATA* pData = pPacket->GetPrize(i);
//					if(pData)
//					{
//						retMsg.AddPrize(pData);
//						g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...Success acc=%s char=%s buytype=%u,buynum=%u",
//							pPrizeUser->GetAccName(),pPrizeUser->GetName(), pData->m_BuyInt, pData->m_BuyNumber);
//
//						WMCmd Msg2M ;
//						Msg2M.GetCMDStruct()->SetStructType( CMD_STRUCT_NOTIFY ) ;
//						Msg2M.GetCMDStruct()->SetControlID( -1 ) ;
//						Msg2M.GetCMDStruct()->m_Notify.CleanUp() ;
//						Msg2M.GetCMDStruct()->m_Notify.m_WorldID = g_Config.m_WorldInfo.m_ZoneID*1000+g_Config.m_WorldInfo.m_WorldID ;
//						Msg2M.GetCMDStruct()->m_Notify.m_nParam0 = 3 ;//用游戏点购买元宝的结果
//						sprintf( Msg2M.GetCMDStruct()->m_Notify.m_szNotify, 
//								"%X,%d,%d,%s,%s,%s",
//								pPrizeUser->GetGUID(),
//								pPacket->GetLeftPoint(),
//								pData->m_BuyNumber,
//								pPrizeUser->GetAccName(),
//								pPrizeUser->GetName(),
//								pPrizeUser->GetHost()
//								) ;
//						Msg2M.GetCMDStruct()->m_Notify.m_nNotifySize = (INT)(strlen(Msg2M.GetCMDStruct()->m_Notify.m_szNotify)) ;
//						g_pMotherClient->SendPacket( &Msg2M, INVALID_ID ) ;
//					}
//				}
//			}
//			break;
//		case BABR_NOENOUGH_POINT:		//0x02,点数不足
//			{
//				retMsg.SetRetResult(UPR_ERR_NOENOUGH_POINT);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d not enough point",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BABR_INVALID_GOODSCODE:	//0x03,物品代码非法
//			{
//				retMsg.SetRetResult(UPR_ERR_GOODSCODE_ERR);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d invalid goodscode",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BABR_ALREADY_GET:			//0x01,已经购买
//		case BABR_OTHER_ERR:			//0x09,其他错误
//		default:
//			{
//				retMsg.SetRetResult(UPR_ERR_CANT_NOW);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d other err",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		}
//	}
//	//4.新手卡
//	else if(pPacket->GetRetType() == ASK_NEWUSER_CARD)
//	{
//		switch(pPacket->GetPrizeResult()) 
//		{
//		case BNCR_SUCCESS:				//0x00,成功
//			{
//				retMsg.SetRetResult(UPR_NEWUSERCARD_SUCCESS);
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Success",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//				for(UINT i = 0; i < pPacket->GetPrizeNum(); ++i)
//				{
//					_BUY_DATA* pData = pPacket->GetPrize(i);
//					if(pData)
//					{
//						retMsg.AddPrize(pData);
//						g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...Success acc=%s char=%s prize=%s,num=%d",
//							pPrizeUser->GetAccName(),pPrizeUser->GetName(), pData->m_BuyString, pData->m_BuyNumber);
//					}
//				}
//			}
//			break;
//		case BNCR_WRONG_CARDNUMBER:		//0x01,卡号错
//			{
//				retMsg.SetRetResult(UPR_ERR_WRONGCARDNUMBER);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d wrong card number",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCR_ALREADY_GET:			//0x02,已经领奖
//			{
//				retMsg.SetRetResult(UPR_ERR_ALREADYGET_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d already get prize",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;			
//		case BNCR_NO_NEWUSERCARD:		//0x05,没有中奖
//			{
//				retMsg.SetRetResult(UPR_ERR_NO_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d no prize",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCR_EXPIRE_CARD:			//0x06,抽奖过期
//			{
//				retMsg.SetRetResult(UPR_ERR_EXPIRE_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d expire prize",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCR_OTHER_USER_ALREADY:	//0x09,其他玩家已经领取
//			{
//				retMsg.SetRetResult(UPR_ERR_OTHERUSERUSE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d card already use by other",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCR_OVER_LEVEL10:			//0x0A,用户已10级，不能再使用新手卡
//			{
//				retMsg.SetRetResult(UPR_ERR_OVERLEVEL10);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d card over level 10 err",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCR_OTHER_ERR:			//0x03,其他错误
//		default:
//			{
//				retMsg.SetRetResult(UPR_ERR_CANT_NOW);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d other err",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		}
//	}
//	//5.新抽奖
//	else if(pPacket->GetRetType() == ASK_NEWPRIZE)
//	{
//		switch(pPacket->GetPrizeResult()) 
//		{
//		case BNPR_SUCCESS:				//0x00,成功
//			{
//				retMsg.SetRetResult(UPR_NEWPRIZE_SUCCESS);
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Success",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//
//				for(UINT i = 0; i < pPacket->GetPrizeNum(); ++i)
//				{
//					_BUY_DATA* pData = pPacket->GetPrize(i);
//					if(pData)
//					{
//						retMsg.AddPrize(pData);
//						g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...Success acc=%s char=%s prize=%s,num=%d",
//							pPrizeUser->GetAccName(),pPrizeUser->GetName(), pData->m_BuyString, pData->m_BuyNumber);
//					}
//				}
//			}
//			break;
//		case BNPR_ALREADY_GET:			//0x02,已经领奖
//			{
//				retMsg.SetRetResult(UPR_ERR_ALREADYGET_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d already get prize",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;			
//		case BNPR_NO_PRIZE:				//0x05,没有中奖
//			{
//				retMsg.SetRetResult(UPR_ERR_NO_ITEM_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d no prize",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNPR_EXPIRE_PRIZE:			//0x06,抽奖过期
//			{
//				retMsg.SetRetResult(UPR_ERR_EXPIRE_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d expire prize",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNPR_INVALID_SERVER:		//0x07,不是规定的领奖服务器
//			{
//				retMsg.SetRetResult(UPR_ERR_INVALID_SERVER);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d not valid server",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNPR_OTHER_ERR:			//0x03,其他错误
//		case BNPR_INVALID_ACC:			//0x04,帐号非法
//		default:
//			{
//				retMsg.SetRetResult(UPR_ERR_CANT_NOW);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d other err",
//					pPacket->GetPrizeUserID(),pPrizeUser->GetGUID(),pPrizeUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		}
//	}
//	//6.新奖品查询
//	else if(pPacket->GetRetType() == ASK_NEWCHECKPRIZE)
//	{
//		retMsg.SetGUID(pUser->GetGUID());
//		switch(pPacket->GetPrizeResult()) 
//		{
//		case BNCPR_SUCCESS:				//0x00,成功
//			{
//				retMsg.SetRetResult(UPR_NEWCHECKPRIZE_SUCCESS);
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d Success",
//					pPacket->GetPrizeUserID(),pUser->GetGUID(),pUser->GetHost(),pPacket->GetPrizeResult());
//
//				for(UINT i = 0; i < pPacket->GetPrizeNum(); ++i)
//				{
//					_BUY_DATA* pData = pPacket->GetPrize(i);
//					if(pData)
//					{
//						retMsg.AddPrize(pData);
//						g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...Success acc=%s char=%s prize=%s,num=%d",
//							pUser->GetAccount(),pUser->GetName(), pData->m_BuyString, pData->m_BuyNumber);
//					}
//				}
//			}
//			break;
//		case BNCPR_NO_PRIZE:			//0x05,没有中奖
//			{
//				retMsg.SetRetResult(UPR_ERR_NO_ITEM_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d no prize",
//					pPacket->GetPrizeUserID(),pUser->GetGUID(),pUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCPR_EXPIRE_PRIZE:		//0x06,抽奖过期
//			{
//				retMsg.SetRetResult(UPR_ERR_EXPIRE_PRIZE);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d expire prize",
//					pPacket->GetPrizeUserID(),pUser->GetGUID(),pUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCPR_INVALID_SERVER:		//0x07,不是规定的领奖服务器
//			{
//				retMsg.SetRetResult(UPR_ERR_INVALID_SERVER);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d not valid server",
//					pPacket->GetPrizeUserID(),pUser->GetGUID(),pUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		case BNCPR_OTHER_ERR:			//0x03,其他错误
//		case BNCPR_INVALID_ACC:			//0x04,帐号非法
//		default:
//			{
//				retMsg.SetRetResult(UPR_ERR_CANT_NOW);
//
//				g_pLog->SaveLog(LOG_FILE_4, "LWRetPrize...PUID=%d,GUID=%X,ip=%s,result=%d other err",
//					pPacket->GetPrizeUserID(),pUser->GetGUID(),pUser->GetHost(),pPacket->GetPrizeResult());
//			}
//			break;
//		}
//	}
//	else
//	{
//		Assert(FALSE);
//	}
//
//	//将信息发送给Server
//	pServerPlayer->SendPacket(&retMsg);
//	if( pPacket->GetRetType() == ASK_PRIZE || pPacket->GetRetType() == ASK_BUY ||
//		pPacket->GetRetType() == ASK_NEWUSER_CARD || pPacket->GetRetType() == ASK_NEWPRIZE)
//	{
//		//抽奖和购买，需要去除额外信息
//		g_pOnlineUser->DelPrizeUser(pPrizeUser);
//	}
//
//	//记录发送成功日志
//	g_pLog->SaveLog(LOG_FILE_4, "LWRetPrizeHandler::Execute()...GUID=%X Scene=%d send to server OK",
//		pUser->GetGUID(),pUser->GetSceneID());
//
//	return PACKET_EXE_CONTINUE;
//
//LOST_PRIZE_LOG:
//
//	//丢失的奖品需要记录下来
//	if( pPacket->GetRetType() == ASK_PRIZE || pPacket->GetRetType() == ASK_BUY ||
//		pPacket->GetRetType() == ASK_NEWUSER_CARD || pPacket->GetRetType() == ASK_NEWPRIZE)
//	{
//		if( BPR_SUCCESS == pPacket->GetPrizeResult() || BABR_SUCCESS == pPacket->GetPrizeResult() ||
//			BNCR_SUCCESS == pPacket->GetPrizeResult())
//		{
//			for(UINT i = 0; i < pPacket->GetPrizeNum(); ++i)
//			{
//				_BUY_DATA* pData = pPacket->GetPrize(i);
//				if(pData)
//				{
//					g_pLog->SaveLog(LOG_FILE_4, "ERROR:LostPrize...GUID=%X acc=%s char=%s (%s:%u:%u:%u)!!!!!",
//						(pUser)?pUser->GetGUID():0xFFFFFF, pPacket->GetAccName(),charname,
//						pData->m_BuyString,pData->m_BuyPoint,pData->m_BuyInt,pData->m_BuyNumber);
//				}
//			}
//		}
//	}
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}























