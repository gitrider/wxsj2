#include "StdAfx.h"
#include "GCDoubleExpInfo.h"

using namespace Packets;
UINT GCDoubleExpInfoHandler::Execute(GCDoubleExpInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
