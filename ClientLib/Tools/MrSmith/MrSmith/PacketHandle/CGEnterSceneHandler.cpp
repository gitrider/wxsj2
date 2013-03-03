#include "StdAfx.h"
#include "CGEnterScene.h"

using namespace Packets;
UINT CGEnterSceneHandler::Execute(CGEnterScene* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
