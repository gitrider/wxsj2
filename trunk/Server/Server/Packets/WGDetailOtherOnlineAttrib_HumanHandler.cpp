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
#include "WGDetailOtherOnlineAttrib_Human.h"
#include "PlayerPool.h"
#include "GCDetailAttrib.h"

namespace Packets
{

UINT WGDetailOtherOnlineAttrib_HumanHandler::Execute( WGDetailOtherOnlineAttrib_Human* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION
		
	PlayerID_t PlayerID = pPacket->GetPlayerID(); //����鿴�ߵ�ID
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	GCDetailAttrib Msg;
	Msg.SetLevel(pPacket->GetLevel());
	Msg.SetCountry(pPacket->GetCountry());
	Msg.SetMenpai(pPacket->GetMenpai());
	Msg.SetDataID(pPacket->GetDataID());
	
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "WGDetailOtherOnlineAttrib_HumanHandler: ok obj=%d,scene=%d",
		pGamePlayer->GetHuman()->GetID(), pGamePlayer->GetHuman()->getScene()->SceneID());

    return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}
}
