
#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"

#include "GCNotifyGoodBad.h"

const char* pNotifyGoodBadStr[] = {
	"NOTIFY_GOODBAD_HELPNEWBIE",		// NOTIFY_GOODBAD_HELPNEWBIE
	"NOTIFY_GOODBAD_NEWBIEDIE",			// NOTIFY_GOODBAD_NEWBIEDIE
	"NOTIFY_GOODBAD_GMCOMMAND",			// NOTIFY_GOODBAD_GMCOMMAND
};

uint GCNotifyGoodBadHandler::Execute( GCNotifyGoodBad* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		STRING strTemp = COLORMSGFUNC(pNotifyGoodBadStr[pPacket->GetNotifyMode()], pPacket->GetValue());
		ADDTALKMSG(strTemp);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
