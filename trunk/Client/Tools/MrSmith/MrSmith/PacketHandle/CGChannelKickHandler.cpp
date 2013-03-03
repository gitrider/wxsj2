#include "StdAfx.h"
#include "CGChannelKick.h"

using namespace Packets;
UINT CGChannelKickHandler::Execute(CGChannelKick* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
