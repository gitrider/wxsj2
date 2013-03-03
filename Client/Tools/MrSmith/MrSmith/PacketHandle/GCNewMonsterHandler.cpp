#include "StdAfx.h"
#include "GCNewMonster.h"

using namespace Packets;
UINT GCNewMonsterHandler::Execute(GCNewMonster* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
