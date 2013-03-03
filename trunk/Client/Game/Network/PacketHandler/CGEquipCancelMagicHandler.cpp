#include "StdAfx.h"
#include "CGEquipCancelMagic.h"

using namespace Packets;

uint CGEquipCancelMagicHandler::Execute( CGEquipCancelMagic* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}