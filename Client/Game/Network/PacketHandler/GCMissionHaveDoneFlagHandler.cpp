#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"


#include "GCMissionHaveDoneFlag.h"



using namespace Packets;

uint GCMissionHaveDoneFlagHandler::Execute( GCMissionHaveDoneFlag* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);


		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_MISSION_HAVEDOWN_FALG;

		cmdTemp.m_anParam[0]	= pPacket->GetMissionID();
		cmdTemp.m_abParam[1]	= pPacket->IsHaveDown();
		pDataPool->OnCommand_( &cmdTemp );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
