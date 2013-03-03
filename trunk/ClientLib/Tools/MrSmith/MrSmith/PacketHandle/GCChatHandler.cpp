#include "StdAfx.h"
#include "GCChat.h"
#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMLog.h"

using namespace Packets;
UINT GCChatHandler::Execute(GCChat* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;
	pAgent->getLog()->addLogFmt( "%d:%s:%s",
		pPacket->GetChatType(),
		pPacket->GetSourName(),
		pPacket->GetContex() );
	return PACKET_EXE_CONTINUE;
}
