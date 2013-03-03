/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "GWHorseChangeState.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "HorseFindMateManager.h"
#include "WGOperateResult.h"

namespace Packets
{
	uint	GWHorseChangeStateHandler::Execute(GWHorseChangeState* pPacket, Player* pPlayer )
	{
		__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
		Assert( pServerPlayer );

		INT OResult;
		if(HorseFindMateInfoManager::GetInstance().OnHorseChangeState(pPacket->GetGUID(), 
			pPacket->GetStateChangeType()) ){
			OResult = OR_CANCELHORSEFINDMATE_SUCC;

			WGOperateResult packet;
			packet.SetPlayerID(pPacket->GetPlayerID());
			packet.setResult(OResult);
			pServerPlayer->SendPacket(&packet);
		}
		//else{
		//	OResult = OR_CANCELHORSEFINDMATE_FAIL;
		//}

		Log::SaveLog( WORLD_LOGFILE, "GWHorseChangeStateHandler: ok guid=%d,changeType=%d",
			pPacket->GetGUID(), pPacket->GetStateChangeType());

		return PACKET_EXE_CONTINUE;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
	}
}

