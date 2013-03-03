#include "StdAfx.h"
#include "CGModifySetting.h"

using namespace Packets;
UINT CGModifySettingHandler::Execute(CGModifySetting* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
