#include "StdAfx.h"
#include "CGStallOpen.h"

using namespace Packets;
UINT CGStallOpenHandler::Execute(CGStallOpen* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
