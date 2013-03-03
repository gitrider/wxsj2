#include "StdAfx.h"
#include "GCNewItemBox.h"

using namespace Packets;
UINT GCNewItemBoxHandler::Execute(GCNewItemBox* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
