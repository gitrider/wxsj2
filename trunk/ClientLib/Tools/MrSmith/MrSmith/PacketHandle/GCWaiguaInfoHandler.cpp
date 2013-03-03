#include "StdAfx.h"
#include "GCWaiguaInfo.h"

using namespace Packets;
UINT GCWaiguaInfoHandler::Execute(GCWaiguaInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
