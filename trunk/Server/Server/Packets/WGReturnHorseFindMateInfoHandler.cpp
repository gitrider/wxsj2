/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "WGReturnHorseFindMateInfo.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "HorseFindMateModule.h"
#include "PlayerPool.h"
#include "GCReturnHorseFindMateInfo.h"

namespace Packets
{

uint	WGReturnHorseFindMateInfoHandler::Execute(WGReturnHorseFindMateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID(); //发起查看者的ID
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	GCReturnHorseFindMateInfo packet;
	packet.SetSearch(pPacket->GetSearch());
	packet.SetReturnPostion(pPacket->GetReturnPostion());
	packet.SetContinue(pPacket->GetContinue());
	packet.SetHorseFindMateResult(pPacket->GetHorseFindMateResult());

	pGamePlayer->SendPacket(&packet);

	g_pLog->FastSaveLog(LOG_FILE_1, "WGReturnHorseFindMateInfoHandler: ok obj=%d,scene=%d",
		pGamePlayer->GetHuman()->GetID(), pGamePlayer->GetHuman()->getScene()->SceneID());

    return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
}
