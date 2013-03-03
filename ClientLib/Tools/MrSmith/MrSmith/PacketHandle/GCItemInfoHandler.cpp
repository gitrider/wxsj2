#include "StdAfx.h"
#include "GCItemInfo.h"

using namespace Packets;
UINT GCItemInfoHandler::Execute(GCItemInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
