#include "StdAfx.h"
#include "CGCommand.h"

using namespace Packets;
UINT CGCommandHandler::Execute(CGCommand* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
