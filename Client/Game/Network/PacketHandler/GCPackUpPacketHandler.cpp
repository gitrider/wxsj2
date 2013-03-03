#include "StdAfx.h"
#include "GCPackUpPacket.h"
#include "Procedure/GameProcedure.h"
#include "Event/GMEventSystem.h"

uint GCPackUpPacketHandler::Execute(GCPackUpPacket* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CEventSystem::GetMe()->PushEvent(GE_END_PACKUP_PACKET);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}