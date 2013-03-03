#include "StdAfx.h"
#include "CGEquipDecompound.h"

using namespace Packets;

uint CGEquipDecompoundHandler::Execute( CGEquipDecompound* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}