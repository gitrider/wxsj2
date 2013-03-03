#include "StdAfx.h"
#include "CGChallenge.h"

using namespace Packets;
UINT CGChallengeHandler::Execute(CGChallenge* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
