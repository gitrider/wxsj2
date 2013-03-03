#include "stdafx.h" 
#include "LCRetLoginCode.h"


UINT LCRetLoginCodeHandler::Execute(LCRetLoginCode* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}