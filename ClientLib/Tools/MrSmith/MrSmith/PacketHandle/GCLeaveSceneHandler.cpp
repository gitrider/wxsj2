#include "StdAfx.h"
#include "GCLeaveScene.h"

using namespace Packets;
UINT GCLeaveSceneHandler::Execute(GCLeaveScene* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
