#include "stdafx.h" 
#include "LBKeepLive.h"



UINT LBKeepLiveHandler::Execute(LBKeepLive* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}