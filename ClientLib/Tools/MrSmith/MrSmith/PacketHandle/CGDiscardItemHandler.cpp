#include "StdAfx.h"
#include "CGDiscardItem.h"

using namespace Packets;
UINT CGDiscardItemHandler::Execute(CGDiscardItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
