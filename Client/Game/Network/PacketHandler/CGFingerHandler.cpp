#include "StdAfx.h"
#include "CGFinger.h"


uint CGFingerHandler::Execute( CGFinger* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
