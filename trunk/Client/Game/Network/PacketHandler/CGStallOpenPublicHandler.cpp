#include "stdafx.h"

#include "CGStallOpenPublic.h"


uint CGStallOpenPublicHandler::Execute( CGStallOpenPublic* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
