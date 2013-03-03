#include "StdAfx.h"
#include "CGPickBoxItem.h"

using namespace Packets;
UINT CGPickBoxItemHandler::Execute(CGPickBoxItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
