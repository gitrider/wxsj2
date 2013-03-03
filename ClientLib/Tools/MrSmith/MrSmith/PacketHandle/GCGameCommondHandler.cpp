#include "StdAfx.h"
#include "GCGameCommond.h"

using namespace Packets;
UINT GCGameCommondHandler::Execute(GCGameCommond* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
