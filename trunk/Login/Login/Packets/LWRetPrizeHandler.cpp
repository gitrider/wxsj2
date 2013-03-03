#include "stdafx.h" 
#include "LWRetPrize.h"
#include "Log.h"


UINT	LWRetPrizeHandler::Execute(LWRetPrize* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


