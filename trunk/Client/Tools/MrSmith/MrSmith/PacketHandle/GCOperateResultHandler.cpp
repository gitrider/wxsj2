#include "StdAfx.h"
#include "GCOperateResult.h"

using namespace Packets;
UINT GCOperateResultHandler::Execute(GCOperateResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
