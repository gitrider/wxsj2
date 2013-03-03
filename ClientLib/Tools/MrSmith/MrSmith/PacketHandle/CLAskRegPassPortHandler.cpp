#include "StdAfx.h"
#include "CLAskRegPassPort.h"

using namespace Packets;
UINT CLAskRegPassPortHandler::Execute(CLAskRegPassPort* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
