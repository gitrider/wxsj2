#include "StdAfx.h"
#include "GCDelObject.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMWorldManager.h"
#include "../SMObjectManager.h"
#include "../SMObject.h"
#include "LuaPlus.h"

using namespace Packets;
using namespace SMITH;

UINT GCDelObjectHandler::Execute(GCDelObject* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;
	if( pPacket )
	{
		int nObj = pAgent->getObjectManager()->findServerObject( (INT)pPacket->getObjID() );
		if( nObj != -1 )
		{
			pAgent->getObjectManager()->destroyObject( nObj );
		}
		else
		{
			nObj = pAgent->getObjectManager()->findServerObject( (INT)pPacket->getObjID() );
		}
		//pAgent->getObjectManager()->destroyObject( (INT)pPacket->getObjID() );
	}

	return PACKET_EXE_CONTINUE;
}
