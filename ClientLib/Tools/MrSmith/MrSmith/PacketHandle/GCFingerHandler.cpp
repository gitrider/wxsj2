#include "StdAfx.h"
#include "GCFinger.h"

using namespace Packets;
UINT GCFingerHandler::Execute(GCFinger* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
