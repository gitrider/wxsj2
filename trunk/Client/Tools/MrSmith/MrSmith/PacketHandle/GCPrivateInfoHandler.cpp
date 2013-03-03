#include "StdAfx.h"
#include "GCPrivateInfo.h"

using namespace Packets;
UINT GCPrivateInfoHandler::Execute(GCPrivateInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
