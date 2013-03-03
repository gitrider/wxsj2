#include "StdAfx.h"
#include "GCEnterScene.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "LuaPlus.h"
#include "../SMVariable.h"
#include "../SMWorldManager.h"
#include "../SMObjectManager.h"

using namespace Packets;
UINT GCEnterSceneHandler::Execute(GCEnterScene* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

	//Enter success!
	if(pPacket->getReturn() == 0)
	{
		//------------------------------------------------------------------
		//Save myself data
		pAgent->getVariable()->setAs_Int("MySelf_ID", (int)pPacket->getObjID());
		pAgent->getVariable()->setAs_Vector2("MySelf_Pos", pPacket->getEnterPos().m_fX, pPacket->getEnterPos().m_fZ);
		pAgent->getVariable()->setAs_Int("Scene_ID", (int)pPacket->getSceneID());

		//Enter scene
		pAgent->getWorldManager()->enterScene((int)pPacket->getSceneID());

		//clear objs
		//pAgent->getObjectManager()->destroyAllObjectsExceptme();

		//Enter success!
		pAgent->getLuaState()->DoString("Procedure_EnterScene_SetState(\"enter_ok\")");
	}
	else
	{
		//Enter failed!
		pAgent->getLuaState()->DoString("Procedure_EnterScene_SetState(\"enter_failed\")");
	}

	return PACKET_EXE_CONTINUE;
}
