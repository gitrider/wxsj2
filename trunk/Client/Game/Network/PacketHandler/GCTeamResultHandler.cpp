
#include "StdAfx.h"
#include "GCTeamResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "GIEventDefine.h"
#include "..\..\event\GMEventSystem.h"


uint GCTeamResultHandler::Execute( GCTeamResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	// 确定是在主流程
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);

		SCommand_DPC cmdTemp;

		cmdTemp.m_wID			= DPC_UPDATE_TEAM_OR_GROUP;	// 组队信息更新
		cmdTemp.m_adwParam[0]	= pPacket->GetReturn();
		cmdTemp.m_adwParam[1]	= pPacket->GetGUID();
		cmdTemp.m_adwParam[2]	= pPacket->GetTeamID();
		cmdTemp.m_adwParam[3]	= pPacket->GetGUIDEx();
		cmdTemp.m_adwParam[4]	= pPacket->GetSceneID();
		cmdTemp.m_adwParam[5]	= pPacket->GetIcon();
		cmdTemp.m_adwParam[6]	= pPacket->GetDataID();
		cmdTemp.m_adwParam[7]	= pPacket->GetFamily();

		strncpy( (char*)&cmdTemp.m_apParam[8], pPacket->GetName(), strlen(pPacket->GetName())+1 );
		
		pDataPool->OnCommand_( &cmdTemp );
		
		// 队友改变场景了
		CEventSystem::GetMe()->PushEvent(GE_TEAM_CHANG_WORLD);
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
