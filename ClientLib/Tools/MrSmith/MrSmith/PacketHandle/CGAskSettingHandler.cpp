#include "StdAfx.h"
#include "CGAskSetting.h"

using namespace Packets;
UINT CGAskSettingHandler::Execute(CGAskSetting* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
