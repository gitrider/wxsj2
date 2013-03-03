/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
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

	PlayerID_t PlayerID = pPacket->GetPlayerID(); //����鿴�ߵ�ID
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
