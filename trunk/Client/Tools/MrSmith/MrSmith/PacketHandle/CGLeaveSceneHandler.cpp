#include "StdAfx.h"
#include "CGLeaveScene.h"

using namespace Packets;
UINT CGLeaveSceneHandler::Execute(CGLeaveScene* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
