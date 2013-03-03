#include "StdAfx.h"
#include "GCBankItemInfo.h"

using namespace Packets;
UINT GCBankItemInfoHandler::Execute(GCBankItemInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
