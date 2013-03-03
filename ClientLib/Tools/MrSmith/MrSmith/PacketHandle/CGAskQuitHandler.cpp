#include "StdAfx.h"
#include "CGAskQuit.h"

using namespace Packets;
UINT CGAskQuitHandler::Execute(CGAskQuit* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
