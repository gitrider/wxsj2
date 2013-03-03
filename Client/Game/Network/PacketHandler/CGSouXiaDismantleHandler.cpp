#include "StdAfx.h"
#include "CGSouXiaDismantle.h"

using namespace Packets;

uint CGSouXiaDismantleHandler::Execute(CGSouXiaDismantle* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}