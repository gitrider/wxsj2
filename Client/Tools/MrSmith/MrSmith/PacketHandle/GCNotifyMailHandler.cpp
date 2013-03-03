#include "StdAfx.h"
#include "GCNotifyMail.h"

using namespace Packets;
UINT GCNotifyMailHandler::Execute(GCNotifyMail* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
