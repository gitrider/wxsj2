#include "StdAfx.h"
#include "CGAskChangeScene.h"

using namespace Packets;
UINT CGAskChangeSceneHandler::Execute(CGAskChangeScene* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
