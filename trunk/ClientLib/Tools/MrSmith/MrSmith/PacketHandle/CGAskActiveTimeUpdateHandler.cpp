#include "StdAfx.h"
#include "CGAskActiveTimeUpdate.h"

using namespace Packets;
UINT CGAskActiveTimeUpdateHandler::Execute(CGAskActiveTimeUpdate* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
