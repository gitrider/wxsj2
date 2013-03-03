#include "StdAfx.h"
#include "GCWorldTime.h"
#include "Procedure/GameProcedure.h"
#include "GITimeSystem.h"

uint GCWorldTimeHandler::Execute( GCWorldTime* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	CGameProcedure::GetPreProcedure()->s_pTimeSystem->SetServerTime(pPacket->GetSystemTime());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
