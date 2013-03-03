#include "StdAfx.h"
#include "GCDirectString.h"

using namespace Packets;
UINT GCDirectStringHandler::Execute(GCDirectString* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
