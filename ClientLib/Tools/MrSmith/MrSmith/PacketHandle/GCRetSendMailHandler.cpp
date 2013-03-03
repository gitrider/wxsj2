#include "StdAfx.h"
#include "GCRetSendMail.h"

using namespace Packets;
UINT GCRetSendMailHandler::Execute(GCRetSendMail* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
