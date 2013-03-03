#include "StdAfx.h"
#include "GCCharBaseAttrib.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMWorldManager.h"
#include "../SMObjectManager.h"
#include "../SMObject.h"
#include "../SMInfo.h"
#include "../SMLog.h"
#include "LuaPlus.h"

using namespace Packets;
using namespace SMITH;

UINT GCCharBaseAttribHandler::Execute(GCCharBaseAttrib* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

	return PACKET_EXE_CONTINUE;
}
