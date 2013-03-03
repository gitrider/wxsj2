#include "StdAfx.h"
#include "GCLeaveScene.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"

uint GCLeaveSceneHandler::Execute( GCLeaveScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCLeaveSceneHandler");
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//Ѱ�Ҹ����
		CObject_Character* pChar = NULL;
		if(!pChar) pChar = (CObject_Character* )pObjectManager->FindServerObject((INT)pPacket->getObjID());
		if(!pChar)	return PACKET_EXE_CONTINUE;

		//ɾ�����
		pChar->Release();
		CGameProcedure::s_pObjectManager->DestroyObject(pChar);
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
