#include "StdAfx.h"
#include "CGRetQuestion.h"

using namespace Packets;
UINT CGRetQuestionHandler::Execute(CGRetQuestion* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
