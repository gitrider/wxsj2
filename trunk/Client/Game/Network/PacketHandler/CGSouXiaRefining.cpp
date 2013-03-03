#include "StdAfx.h"
#include "CGSouXiaRefining.h"

using namespace Packets;

uint CGSouXiaRefiningHandler::Execute(CGSouXiaRefining* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}