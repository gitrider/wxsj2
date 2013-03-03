#include "StdAfx.h"
#include "CGSouXiaSew.h"

using namespace Packets;

uint CGSouXiaSewHandler::Execute(CGSouXiaSew* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}