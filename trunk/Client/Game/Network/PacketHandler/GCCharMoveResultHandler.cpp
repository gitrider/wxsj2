// GCCharMoveResultHandler.cpp

#include "StdAfx.h"

#include "GCCharMoveResult.h"

uint GCCharMoveResultHandler::Execute( GCCharMoveResult* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

