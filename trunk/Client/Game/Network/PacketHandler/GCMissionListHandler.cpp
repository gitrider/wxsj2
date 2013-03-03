
#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"

#include "GCMissionList.h"



using namespace Packets;

uint GCMissionListHandler::Execute( GCMissionList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);


		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_MISSION_LIST;

		cmdTemp.m_adwParam[0]	= (UINT)(pPacket->GetObjID());
		cmdTemp.m_adwParam[1]	= (UINT)(pPacket->GetMissionListFlags());
		cmdTemp.m_apParam[2]	= (VOID*)(pPacket->GetMissionBuf());
		pDataPool->OnCommand_( &cmdTemp );

		CDetailAttrib_Player* pAttrib = (CDetailAttrib_Player*)CUIDataPool::GetMe()->GetMySelfDetailAttrib();
		pAttrib->SetMissionHaveDone(pPacket->GetMissionHaveDone());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
