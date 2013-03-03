#include "stdafx.h" 
#include "LCRetCreateCode.h"


UINT LCRetCreateCodeHandler::Execute(LCRetCreateCode* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}