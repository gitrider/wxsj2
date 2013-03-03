#include "stdafx.h"
#include "GCEquipCancelMagicResult.h"

using namespace Packets;

uint GCEquipCancelMagicResultHandler::Execute( GCEquipCancelMagicResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}