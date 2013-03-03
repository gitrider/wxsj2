#include "StdAfx.h"
#include "CGUseExtraBag.h"

using namespace Packets;
uint	CGUseExtraBagHandler::Execute(CGUseExtraBag* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}