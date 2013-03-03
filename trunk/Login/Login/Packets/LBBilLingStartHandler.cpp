#include "stdafx.h" 
#include "LBBillingStart.h"


UINT LBBillingStartHandler::Execute(LBBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}