#include "StdAfx.h"
#include "CGPlayerCallOfResult.h"

using namespace Packets;
UINT CGPlayerCallOfResultHandler::Execute(CGPlayerCallOfResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
