#include "StdAfx.h"
#include "CGStallClose.h"

using namespace Packets;
UINT CGStallCloseHandler::Execute(CGStallClose* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
