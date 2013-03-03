#include "StdAfx.h"
#include "GCHorseLevelUp.h"


using namespace Packets;

UINT GCHorseLevelUpHandler::Execute(GCHorseLevelUp* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}