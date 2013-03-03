#include "StdAfx.h"
#include "GCChallenge.h"

using namespace Packets;
UINT GCChallengeHandler::Execute(GCChallenge* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
