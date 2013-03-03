#include "StdAfx.h"
#include "GCUICommand.h"

using namespace Packets;
UINT GCUICommandHandler::Execute(GCUICommand* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
