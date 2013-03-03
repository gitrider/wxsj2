#include "StdAfx.h"
#include "GCNewPortal.h"

using namespace Packets;
UINT GCNewPortalHandler::Execute(GCNewPortal* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
