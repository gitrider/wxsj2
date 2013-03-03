#include "StdAfx.h"
#include "GCEquipRefining.h"

using namespace Packets;

uint GCEquipRefiningHandler::Execute(GCEquipRefining* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}