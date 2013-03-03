#include "StdAfx.h"
#include "GCMail.h"

using namespace Packets;
UINT GCMailHandler::Execute(GCMail* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
