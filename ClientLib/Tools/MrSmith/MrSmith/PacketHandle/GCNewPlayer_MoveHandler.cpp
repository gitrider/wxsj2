#include "StdAfx.h"
#include "GCNewPlayer_Move.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMWorldManager.h"
#include "../SMObjectManager.h"
#include "../SMObject.h"
#include "../SMNetManager.h"
#include "CGCharAskBaseAttrib.h"
#include "LuaPlus.h"

using namespace Packets;
using namespace SMITH;

UINT GCNewPlayer_MoveHandler::Execute(GCNewPlayer_Move* pPacket, Player* pPlayer)
{
	//return PACKET_EXE_CONTINUE;
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;
	if( pPacket )
	{
		pAgent->getObjectManager()->createObject( OT_PLAYEROTHER, (INT)pPacket->GetObjID() );

		//char szCommand[1024];
		//sprintf( szCommand, "New_Player_IN(\"%d\")", (INT)pPacket->getObjID() );
		//pAgent->getLuaState()->DoString(szCommand);

		//CGCharAskBaseAttrib msg;
		//msg.setTargetID((INT)pPacket->getObjID());
		//pAgent->getNetManager()->sendPacket(&msg);
	}

	return PACKET_EXE_CONTINUE;
}
