#include "StdAfx.h"
#include "CGEquipSuitExchange.h"

using namespace Packets;

uint CGEquipSuitExchangeHandler::Execute( CGEquipSuitExchange* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}