#include "StdAfx.h"
#include "CGSouXiaDiscard.h"

using namespace Packets;

uint CGSouXiaDiscardHandler::Execute(CGSouXiaDiscard* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}