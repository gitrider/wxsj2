#include "stdafx.h" 
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "Log.h"
#include "LBRegPassPort.h"
#include "Config.h"


UINT LBRegPassPortHandler::Execute(LBRegPassPort* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}