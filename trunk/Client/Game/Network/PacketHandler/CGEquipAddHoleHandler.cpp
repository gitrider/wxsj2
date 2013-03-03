#include "StdAfx.h"
#include "CGEquipAddHole.h"

using namespace Packets;

uint CGEquipAddHoleHandler::Execute( CGEquipAddHole* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}