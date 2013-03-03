#include "StdAfx.h"
#include "GCRetSetting.h"

using namespace Packets;
UINT GCRetSettingHandler::Execute(GCRetSetting* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
