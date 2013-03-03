#include "StdAfx.h"
#include "GCNewItem.h"

using namespace Packets;
UINT GCNewItemHandler::Execute(GCNewItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
