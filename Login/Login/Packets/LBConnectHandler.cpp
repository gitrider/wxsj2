#include "stdafx.h" 
#include "LBConnect.h"



UINT LBConnectHandler::Execute(LBConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}