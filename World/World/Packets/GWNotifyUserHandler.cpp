

#include "stdafx.h"
#include "GWNotifyUser.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"
#include "ServerPlayer.h"




uint GWNotifyUserHandler::Execute( GWNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() ) ;
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWNotifyUserHandler...User GUID=%X not find!", 
			pPacket->GetGUID(), pPacket->GetStatus() ) ;

		return PACKET_EXE_CONTINUE ;
	}

	//被Server锁定
	if(pPacket->GetStatus()==GWNotifyUser::NUS_LOCK_BY_SHM)
	{
		if(g_pOnlineUser->IsKickUser(pUser))
		{
			//被Server的ShareMem锁定的玩家，会在User的HeartBeat中重新发送删人请求
			g_pOnlineUser->EndKickUser(pUser,pServerPlayer->PlayerID(),SK_KICK_WAIT_SHM);

			Log::SaveLog( WORLD_LOGFILE, "KICK:GWNotifyUserHandler...GUID=%X acc=%s serverid=%d user not in lock by sharemem!",
				pUser->GetGUID(), pUser->GetAccount(), pServerPlayer->GetServerData()->m_ServerID);
		}
		else
		{
			Log::SaveLog( WORLD_LOGFILE, "KICK:GWNotifyUserHandler...GUID=%X acc=%s serverid=%d user not in lock status NUS_LOCK_BY_SHM IGNORE!",
				pUser->GetGUID(), pUser->GetAccount(), pServerPlayer->GetServerData()->m_ServerID);
		}
		return PACKET_EXE_CONTINUE;
	}

	//Server里没有此玩家
	if(pPacket->GetStatus()==GWNotifyUser::NUS_NOUSR)
	{
		if(g_pOnlineUser->IsKickUser(pUser))
		{
			//被Server的ShareMem锁定的玩家，会在User的HeartBeat中重新发送删人请求
			g_pOnlineUser->EndKickUser(pUser,pServerPlayer->PlayerID(),SK_KICK_OK);

			Log::SaveLog( WORLD_LOGFILE, "KICK:GWNotifyUserHandler...GUID=%X acc=%s serverid=%d user kick ok!",
				pUser->GetGUID(), pUser->GetAccount(), pServerPlayer->GetServerData()->m_ServerID);
		}
		else
		{
			Log::SaveLog( WORLD_LOGFILE, "KICK:GWNotifyUserHandler...GUID=%X acc=%s serverid=%d user not in lock status NUS_NOUSR IGNORE!",
				pUser->GetGUID(), pUser->GetAccount(), pServerPlayer->GetServerData()->m_ServerID);
		}
		return PACKET_EXE_CONTINUE;
	}

	if( pPacket->GetStatus()==GWNotifyUser::NUS_DISCONNECT )
	{
		pUser->SetUserStatus( US_OFFLINE );
		pUser->ResetKickTime();
		g_pOnlineUser->OnUserOffLine( pUser );
        g_pCountryManager->RemoveCountryUser((COUNTRY_ATTRIBUTE)pUser->GetCountry(), pUser->GetGUID());
	}
	
	if( pPacket->GetStatus()==GWNotifyUser::NUS_REMOVE )
	{
		g_pOnlineUser->OnUserRemove( pUser );
        g_pCountryManager->RemoveCountryUser((COUNTRY_ATTRIBUTE)pUser->GetCountry(), pUser->GetGUID());
		g_pOnlineUser->DelUser( pUser ) ;
	}
	

	Log::SaveLog( WORLD_LOGFILE, "GWNotifyUserHandler...User GUID=%X Status=%d", 
		pPacket->GetGUID(), pPacket->GetStatus() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
