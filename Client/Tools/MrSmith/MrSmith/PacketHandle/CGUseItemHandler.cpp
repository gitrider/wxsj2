#include "StdAfx.h"
#include "CGUseItem.h"

using namespace Packets;
UINT CGUseItemHandler::Execute(CGUseItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
