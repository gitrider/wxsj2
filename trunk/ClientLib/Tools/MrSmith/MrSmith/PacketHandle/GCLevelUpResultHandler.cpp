#include "StdAfx.h"
#include "GCLevelUpResult.h"

using namespace Packets;
UINT GCLevelUpResultHandler::Execute(GCLevelUpResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
