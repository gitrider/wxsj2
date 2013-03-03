#include "StdAfx.h"
#include "CGGameCommond.h"

using namespace Packets;
UINT CGGameCommondHandler::Execute(CGGameCommond* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
