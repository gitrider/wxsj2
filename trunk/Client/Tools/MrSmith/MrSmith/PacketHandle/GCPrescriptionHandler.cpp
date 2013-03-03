#include "StdAfx.h"
#include "GCPrescription.h"

using namespace Packets;
UINT GCPrescriptionHandler::Execute(GCPrescription* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
