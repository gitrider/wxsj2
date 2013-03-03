/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "GWDetailOtherOnlineAttrib_Human.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGDetailOtherOnlineAttrib_Human.h"
#include "User.h"
#include "OnlineUser.h"

namespace Packets
{

uint	GWDetailOtherOnlineAttrib_HumanHandler::Execute(GWDetailOtherOnlineAttrib_Human* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	Assert( pServerPlayer );

	GUID_t guid = pPacket->GetPlayerGUID();
	USER* pUser = g_pOnlineUser->FindUser(guid);
	if(pUser){
		ID_t ServerID = pUser->GetServerID() ;
		ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
		if( pDestServerPlayer==NULL )
		{
			Assert(FALSE);
		}

		// 发送人物基本信息
		WGDetailOtherOnlineAttrib_Human Msg;
		Msg.SetPlayerID(pPacket->GetPlayerID());//发起查看者的ID
		Msg.SetLevel(pPacket->GetLevel());
		Msg.SetCountry(pPacket->GetCountry());
		Msg.SetMenpai(pPacket->GetMenpai());
		Msg.SetDataID(pPacket->GetDataID());


		pDestServerPlayer->SendPacket(&Msg);

		Log::SaveLog( WORLD_LOGFILE, "GWDetailOtherOnlineAttrib_HumanHandler: ok");
	}
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}
