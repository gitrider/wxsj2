#include "StdAfx.h"
#include "CGPickDropItem.h"

using namespace Packets;


uint CGPickDropItemHandler::Execute( CGPickDropItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}