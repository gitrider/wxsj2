#include "StdAfx.h"
#include "CGEquipLevelup.h"

using namespace Packets;

uint CGEquipLevelupHandler::Execute( CGEquipLevelup* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}