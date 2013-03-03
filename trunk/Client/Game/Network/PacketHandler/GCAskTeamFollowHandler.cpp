
/** GCAskTeamFollowHandler.cpp
 */

#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"

#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\event\GMEventSystem.h"

#include "GCAskTeamFollow.h"

uint GCAskTeamFollowHandler::Execute( GCAskTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObject_PlayerMySelf* pMe;

		pMe = CObjectManager::GetMe()->GetMySelf();
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC( "GCAskTeamFollowHandler_Info_Join_Team_Model_YesNo" );
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );	// "��ѡ���Ƿ�������ģʽ��"


		GUID_t LeaderGUID;
		LeaderGUID = CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID();

		CHAR LeaderName[MAX_CHARACTER_NAME];
		strncpy( LeaderName, CUIDataPool::GetMe()->GetTeamOrGroup()->GetMember(LeaderGUID)->m_szNick, MAX_CHARACTER_NAME );

		CEventSystem::GetMe()->PushEvent( GE_TEAM_FOLLOW_INVITE, LeaderName );

	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
