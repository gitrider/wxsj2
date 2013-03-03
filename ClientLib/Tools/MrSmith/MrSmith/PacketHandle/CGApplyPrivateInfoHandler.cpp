#include "StdAfx.h"
#include "CGApplyPrivateInfo.h"

using namespace Packets;
UINT CGApplyPrivateInfoHandler::Execute(CGApplyPrivateInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
