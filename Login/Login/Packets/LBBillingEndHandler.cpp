#include "stdafx.h" 
#include "LBBillingEnd.h"


UINT LBBillingEndHandler::Execute(LBBillingEnd* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}