#include "StdAfx.h"
#include "CGStallEstablish.h"

using namespace Packets;
UINT CGStallEstablishHandler::Execute(CGStallEstablish* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
