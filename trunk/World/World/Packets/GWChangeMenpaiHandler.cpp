
#include "stdafx.h"
#include "GWChangeMenpai.h"
#include "Log.h"
#include "OnlineUser.h"

using namespace Packets;

uint GWChangeMenpaiHandler::Execute(GWChangeMenpai* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if( pUser==NULL )
	{//û�з����ڡ������û��б����Ӧ��GUID���û�
		Log::SaveLog( WORLD_LOGFILE, "GWChangeMenpaiHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "�Ҳ�������" );
		return PACKET_EXE_CONTINUE;
	}

	pUser->SetMenpai( pPacket->GetMenpai() );

	Log::SaveLog( WORLD_LOGFILE, "GWChangeMenpaiHandler...User GUID=%X Changed Menpai to Menpai=%d", 
		pPacket->GetGUID(), pPacket->GetMenpai() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
