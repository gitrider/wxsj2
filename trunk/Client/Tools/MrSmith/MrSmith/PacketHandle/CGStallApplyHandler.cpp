#include "StdAfx.h"
#include "CGStallApply.h"

using namespace Packets;
UINT CGStallApplyHandler::Execute(CGStallApply* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
