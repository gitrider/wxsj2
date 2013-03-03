/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "GWIssueHorseFindMateInfo.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "HorseFindMateManager.h"
#include "WGOperateResult.h"

namespace Packets
{

uint	GWIssueHorseFindMateInfoHandler::Execute(GWIssueHorseFindMateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	Assert( pServerPlayer );

	INT OResult;
	if(HorseFindMateInfoManager::GetInstance().IssueHorseFindMateInfo(pPacket->GetHorseFindMateInfo())){
		OResult = OR_ISSUEHORSE_SUCC;
	}
	else{
		OResult = OR_ISSUEHORSE_FAIL;
	}

	WGOperateResult packet;
	packet.SetPlayerID(pPacket->GetPlayerID());
	packet.setResult(OResult);
	pServerPlayer->SendPacket(&packet);

	Log::SaveLog( WORLD_LOGFILE, "GWIssueHorseFindMateInfoHandler: ok petguid_h=%u, petguid_l=%u",
		pPacket->GetHorseFindMateInfo().m_HorseGuid.GetHighSection(),
		pPacket->GetHorseFindMateInfo().m_HorseGuid.GetLowSection());

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

}
