#include "StdAfx.h"
#include "CGEquipRefining.h"

using namespace Packets;

uint CGEquipRefiningHandler::Execute(CGEquipRefining* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}