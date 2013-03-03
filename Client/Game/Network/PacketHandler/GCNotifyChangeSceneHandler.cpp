#include "StdAfx.h"
#include "GCNotifyChangeScene.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\World\WorldManager.h"
#include "GIDBC_Struct.h"

uint GCNotifyChangeSceneHandler::Execute( GCNotifyChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNotifyChangeSceneHandle[%.1f,%.1f]r::Execute", 
	//	pPacket->getTargetPos()->m_fX, pPacket->getTargetPos()->m_fZ);
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		INT nSourceID = CWorldManager::GetMe()->GetActiveDataSceneID();
		if(nSourceID != pPacket->getCurrentSceneID()) return PACKET_EXE_CONTINUE;

		CWorldManager::GetMe()->ChangeScene(
			pPacket->getTargetSceneID(),
			pPacket->getResID(),
			fVector2(pPacket->getTargetPos()->m_fX, pPacket->getTargetPos()->m_fZ),
			pPacket->getTargetDir());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
