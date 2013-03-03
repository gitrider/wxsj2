#include "stdafx.h" 
#include "LBKickAll.h"

UINT LBKickAllHandler::Execute(LBKickAll* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}