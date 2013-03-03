#include "StdAfx.h"
#include "GCTest.h"

using namespace Packets;
UINT GCTestHandler::Execute(GCTest* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
