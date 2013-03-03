#include "StdAfx.h"
#include "GCSpecialObj_FadeOut.h"

using namespace Packets;
UINT GCSpecialObj_FadeOutHandler::Execute(GCSpecialObj_FadeOut* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
