#include "StdAfx.h"
#include "CGStallDefaultPage.h"

using namespace Packets;
UINT CGStallDefaultPageHandler::Execute(CGStallDefaultPage* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
