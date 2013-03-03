#include "stdafx.h" 
#include "KeyExchange.h"

#include "Log.h"

namespace Packets
{
	uint KeyExchangeHandler::Execute(KeyExchange* pPacket, Player* pPlayer )
	{
		__ENTER_FUNCTION
		Assert( pPlayer ) ;
		Assert( pPacket);
		return PACKET_EXE_CONTINUE;
		__LEAVE_FUNCTION
		return PACKET_EXE_ERROR ;
	}
}