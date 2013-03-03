/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
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

		// �������������Ϣ
		WGDetailOtherOnlineAttrib_Human Msg;
		Msg.SetPlayerID(pPacket->GetPlayerID());//����鿴�ߵ�ID
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
