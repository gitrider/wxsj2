#include "StdAfx.h"
#include "GCByname.h"

using namespace Packets;
UINT GCBynameHandler::Execute(GCByname* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
