#include "StdAfx.h"
#include "CLConnect.h"

using namespace Packets;
UINT CLConnectHandler::Execute(CLConnect* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
