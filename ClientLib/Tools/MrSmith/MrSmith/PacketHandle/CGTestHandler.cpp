#include "StdAfx.h"
#include "CGTest.h"

using namespace Packets;
UINT CGTestHandler::Execute(CGTest* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
