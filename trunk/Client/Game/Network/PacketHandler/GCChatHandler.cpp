#include "StdAfx.h"
#include "GCChat.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Manager\ObjectManager.h"
#include "../../DBC/GMDataBase.h"
#include "GIException.h"
#include "../../Global.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"

uint GCChatHandler::Execute( GCChat* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		SCRIPT_SANDBOX::Talk::s_Talk.HandleRecvTalkPacket(pPacket);
	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
