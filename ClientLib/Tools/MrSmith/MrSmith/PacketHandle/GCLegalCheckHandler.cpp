#include "StdAfx.h"
#include "GCLegalCheck.h"

using namespace Packets;
UINT GCLegalCheckHandler::Execute(GCLegalCheck* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
