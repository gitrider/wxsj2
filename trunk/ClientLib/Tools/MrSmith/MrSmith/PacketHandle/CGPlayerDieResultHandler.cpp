#include "StdAfx.h"
#include "CGPlayerDieResult.h"

using namespace Packets;
UINT CGPlayerDieResultHandler::Execute(CGPlayerDieResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
