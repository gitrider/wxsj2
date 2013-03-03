#include "StdAfx.h"
#include "CGSaveEquipSuit.h"

using namespace Packets;

uint CGSaveEquipSuitHandler::Execute( CGSaveEquipSuit* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}