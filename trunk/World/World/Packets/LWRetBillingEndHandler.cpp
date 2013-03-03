#include "stdafx.h"
#include "LWRetBillingEnd.h"
#include "Log.h"


UINT	LWRetBillingEndHandler::Execute(LWRetBillingEnd* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


		g_pLog->FastSaveLog(LOG_FILE_4, "LWRetBillingEnd...acc=%s Result=%d",
		pPacket->GetAccount(),pPacket->GetResult()) ;

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


