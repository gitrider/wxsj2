#include "StdAfx.h"
#include "CGChannelInvite.h"

using namespace Packets;
UINT CGChannelInviteHandler::Execute(CGChannelInvite* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
