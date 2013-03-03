#include "stdafx.h" 
#include "LBAccCheck.h"


UINT LBAccCheckHandler::Execute(LBAccCheck* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}