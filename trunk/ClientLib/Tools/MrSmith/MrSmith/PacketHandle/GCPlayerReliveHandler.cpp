#include "StdAfx.h"
#include "GCPlayerRelive.h"

using namespace Packets;
UINT GCPlayerReliveHandler::Execute(GCPlayerRelive* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
