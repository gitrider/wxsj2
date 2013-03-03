#include "StdAfx.h"
#include "GCStallEstablish.h"

using namespace Packets;
UINT GCStallEstablishHandler::Execute(GCStallEstablish* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
