/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "GWDetailOtherOnlineEquipList_Human.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGDetailOtherOnlineEquipList_Human.h"
#include "User.h"
#include "OnlineUser.h"

namespace Packets
{

uint	GWDetailOtherOnlineEquipList_HumanHandler::Execute(GWDetailOtherOnlineEquipList_Human* pPacket, Player* pPlayer )
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

		// 发送人物装备信息
		WGDetailOtherOnlineEquipList_Human Msg;
		Msg.SetPlayerID(pPacket->GetPlayerID()); //设置发起查看者的ID
		Msg.SetAskMode(ASK_EQUIP_ALL);

		for(INT i=0; i<HEQUIP_NUMBER; i++){
			if(pPacket->GetPartFlags() & (1<<i))
				Msg.SetEquipData((HUMAN_EQUIP)i, pPacket->GetEquipData((HUMAN_EQUIP)i) );
		}			

		pDestServerPlayer->SendPacket(&Msg);

		Log::SaveLog( WORLD_LOGFILE, "GWDetailOtherOnlineEquipList_HumanHandler: ok ");
	}
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}
