#include "StdAfx.h"
#include "GCScriptCommand.h"

using namespace Packets;
UINT GCScriptCommandHandler::Execute(GCScriptCommand* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
