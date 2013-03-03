#include "StdAfx.h"
#include "GCAddLockObj.h"


uint GCAddLockObjHandler::Execute( GCAddLockObj* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
