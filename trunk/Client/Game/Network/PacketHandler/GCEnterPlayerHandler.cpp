#include "StdAfx.h"
#include "GCEnterPlayer.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Obj_PlayerOther.h"
#include "..\..\Procedure\GameProcedure.h"

uint GCEnterPlayerHandler::Execute( GCEnterPlayer* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	AxTrace(0, 2, "GCEnterPlayerHandler::Execute");

	//����ҽ���
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		const PLAYER_S& newPlayer = *(pPacket->GetPlayerInfo());
			
		//ͨ��ObjManager�����������
		TDObject_PlayerOther* pNewPlayer = (TDObject_PlayerOther*)TDObjectManager::GetMe()->NewObject(_T("TDObject_PlayerOther"), _T("TDObject_PlayerOther"));
		pNewPlayer->Build(newPlayer);
		pNewPlayer->Initial();
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
