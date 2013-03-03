#include "StdAfx.h"
#include "GCNewPlatform.h"

using namespace Packets;
UINT GCNewPlatformHandler::Execute(GCNewPlatform* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
