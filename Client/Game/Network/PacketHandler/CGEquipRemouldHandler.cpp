#include "StdAfx.h"
#include "CGEquipRemould.h"

using namespace Packets;

uint CGEquipRemouldHandler::Execute( CGEquipRemould* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}