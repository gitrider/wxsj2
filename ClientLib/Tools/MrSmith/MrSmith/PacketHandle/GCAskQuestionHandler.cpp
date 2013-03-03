#include "StdAfx.h"
#include "GCAskQuestion.h"

using namespace Packets;
UINT GCAskQuestionHandler::Execute(GCAskQuestion* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
