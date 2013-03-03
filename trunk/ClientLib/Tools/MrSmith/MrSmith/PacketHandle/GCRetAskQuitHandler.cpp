#include "StdAfx.h"
#include "GCRetAskQuit.h"

using namespace Packets;
UINT GCRetAskQuitHandler::Execute(GCRetAskQuit* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
