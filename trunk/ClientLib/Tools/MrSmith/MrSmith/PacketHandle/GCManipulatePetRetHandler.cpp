#include "StdAfx.h"
#include "GCManipulatePetRet.h"

using namespace Packets;
UINT GCManipulatePetRetHandler::Execute(GCManipulatePetRet* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
