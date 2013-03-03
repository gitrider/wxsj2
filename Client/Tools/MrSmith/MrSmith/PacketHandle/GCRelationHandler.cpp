#include "StdAfx.h"
#include "GCRelation.h"

using namespace Packets;
UINT GCRelationHandler::Execute(GCRelation* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
