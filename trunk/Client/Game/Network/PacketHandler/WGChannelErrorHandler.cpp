#include "StdAfx.h"


#include "WGChannelError.h"




uint WGChannelErrorHandler::Execute( WGChannelError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
