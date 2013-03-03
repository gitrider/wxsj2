#include "stdafx.h" 
#include "LWRetBillingStart.h"
#include "Log.h"


UINT	LWRetBillingStartHandler::Execute(LWRetBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


