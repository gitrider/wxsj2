
#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "ServerManager.h"
#include "WGDetailOtherOnlineEquipList_Human.h"
#include "PlayerPool.h"
#include "GCDetailEquipList.h"

namespace Packets
{

uint WGDetailOtherOnlineEquipList_HumanHandler::Execute( WGDetailOtherOnlineEquipList_Human* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID(); //发起查看者的ID
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	GCDetailEquipList Msg;
	Msg.setObjID(INVALID_ID);	//因为该玩家在Client或Client所在的当前Server根本就不存在
	Msg.SetAskMode(ASK_EQUIP_ALL);

	UINT dwEquipMask = pPacket->GetPartFlags();
	for(INT i=0; i<HEQUIP_NUMBER; i++){
		if(dwEquipMask&(1<<i)){
			Msg.SetEquipData((HUMAN_EQUIP)i, pPacket->GetEquipData((HUMAN_EQUIP)i));
		}
	}			
	
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "WGDetailOtherOnlineEquipList_HumanHandler: ok  obj=%d,scene=%d",
		pGamePlayer->GetHuman()->GetID(), pGamePlayer->GetHuman()->getScene()->SceneID());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

}
