// [7/8/2010 ³Â¾üÁú]
#include "StdAfx.h"
#include "CGManipulateMount.h"

using namespace Packets;

uint CGManipulateMountHandler::Execute(CGManipulateMount* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}