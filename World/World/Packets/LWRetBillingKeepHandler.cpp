#include "stdafx.h"
#include "LWRetBillingKeep.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "OnlineUser.h"
#include "WGNotifyUser.h"
#include "ServerManager.h"


UINT	LWRetBillingKeepHandler::Execute(LWRetBillingKeep* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	

	enum BillingKeepResult
	{
		BKR_SUCCESS		=	0x01,	//计费成功
		BKR_NO_POINT	=	0x02,	//点数不够
		BKR_DB_ERROR	=	0x03,	//数据库错误
		BKR_NOT_START	=	0x04,	//没开始计费
	};
	
	
	Assert(pPacket);
	ServerPlayer* pLoginPlayer = (ServerPlayer*)pPlayer ;
	Assert(pLoginPlayer);
	USER* pUser = g_pOnlineUser->FindUserByAccount(pPacket->GetAccount());
	if(pUser)
	{
		BYTE bResult = pPacket->GetResult();

		switch(bResult) 
		{
		case BKR_SUCCESS:
			{
				////pUser->SetLeftTime(pPacket->GetLeftTime());
				////pUser->SetShopPoint(pPacket->GetStorePoint());
				////pUser->SetUserPoint(pPacket->GetUserPoint());
				////pUser->SetStartTime(pUser->GetEndTime());
				////pUser->SetBillingKeepTimes(0);
				////g_pLog->FastSaveLog(LOG_FILE_4, "LWRetBillingKeep...acc=%s Success,lt=%d,sp=%d,up=%d,GUID=%X",
				////	pPacket->GetAccount(),pUser->GetLeftTime(),pUser->GetShopPoint(),
				////	pUser->GetUserPoint(),pUser->GetGUID()) ;


			}
			break;
		case BKR_NO_POINT:
		case BKR_DB_ERROR:
		case BKR_NOT_START:
			{
				//WGNotifyUser MsgNotify ;
				//MsgNotify.SetGUID( pUser->GetGUID() ) ;
				//MsgNotify.SetStatus( WGNotifyUser::NUS_NEED_SERVER_KICK ) ;
				//ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );
				//if( pServerPlayer )
				//{
				//	pServerPlayer->SendPacket( &MsgNotify ) ;
				//}
				g_pOnlineUser->BeginKickUser(pUser);
				g_pLog->FastSaveLog(LOG_FILE_4, "KICK:LWRetBillingKeep...acc=%s Fail,Result=%d,GUID=%X",
					pPacket->GetAccount(),bResult,pUser->GetGUID()) ;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		//Assert(FALSE);
	}
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}


