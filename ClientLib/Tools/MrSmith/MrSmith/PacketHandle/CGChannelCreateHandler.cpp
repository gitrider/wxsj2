#include "StdAfx.h"
#include "CGChannelCreate.h"

using namespace Packets;
UINT CGChannelCreateHandler::Execute(CGChannelCreate* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
