#include "StdAfx.h"
#include "GCMenpaiInfo.h"

using namespace Packets;
UINT GCMenpaiInfoHandler::Execute(GCMenpaiInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
