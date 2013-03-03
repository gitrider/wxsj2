#include "StdAfx.h"
#include "LCRetCreateCode.h"

using namespace Packets;
UINT LCRetCreateCodeHandler::Execute(LCRetCreateCode* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
