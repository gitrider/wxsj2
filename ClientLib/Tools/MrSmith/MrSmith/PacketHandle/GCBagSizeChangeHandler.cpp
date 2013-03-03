#include "StdAfx.h"
#include "GCBagSizeChange.h"

using namespace Packets;
UINT GCBagSizeChangeHandler::Execute(GCBagSizeChange* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
