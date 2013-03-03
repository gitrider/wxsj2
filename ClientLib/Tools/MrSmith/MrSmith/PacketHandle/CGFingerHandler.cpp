#include "StdAfx.h"
#include "CGFinger.h"

using namespace Packets;
UINT CGFingerHandler::Execute(CGFinger* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
