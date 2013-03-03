#include "StdAfx.h"
#include "CGCharMoodState.h"

using namespace Packets;
UINT CGCharMoodStateHandler::Execute(CGCharMoodState* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
