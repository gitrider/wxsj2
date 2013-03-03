#include "StdAfx.h"
#include "CGEquipBind.h"

using namespace Packets;

uint CGEquipBindHandler::Execute( CGEquipBind* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}