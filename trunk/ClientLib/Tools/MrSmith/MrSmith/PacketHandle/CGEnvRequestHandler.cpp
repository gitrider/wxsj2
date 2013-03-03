#include "StdAfx.h"
#include "CGEnvRequest.h"

using namespace Packets;
UINT CGEnvRequestHandler::Execute(CGEnvRequest* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
