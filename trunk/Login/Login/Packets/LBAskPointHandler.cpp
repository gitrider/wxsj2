#include "stdafx.h" 
#include "LBAskPoint.h"


UINT LBAskPointHandler::Execute(LBAskPoint* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}