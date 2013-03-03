#include "StdAfx.h"
#include "CGChannelDismiss.h"

using namespace Packets;
UINT CGChannelDismissHandler::Execute(CGChannelDismiss* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
