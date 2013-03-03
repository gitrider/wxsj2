
/** GCTeamFollowErrHandler.cpp
 */

#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

#include "GCTeamFollowErr.h"

const char* pErrString[] = {
	"GCTeamFollowErrorHandler_Info_Too_Far_To_Leader",
	"GCTeamFollowErrorHandler_Info_Can_Not_Move_Self",
	"GCTeamFollowErrorHandler_Info_Stall_Can_Not_Follow_Team",
	"GCTeamFollowErrorHandler_Info_In_Follow"
};

uint GCTeamFollowErrHandler::Execute( GCTeamFollowErr* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR *pErrContent = new CHAR[MAX_PATH];

		switch( pPacket->GetError() )
		{
		case TF_ERROR_TOO_FAR:				// 离队长太远了（而不能跟随）
		case TF_ERROR_IN_TEAM_FOLLOW:		// 已经处于组队跟随状态（而不能做某些操作）
		case TF_ERROR_STALL_OPEN:			// 正在摆摊
		case TF_ERROR_NOT_IN_FOLLOW_MODE:	// 队伍目前不处于跟随状态
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC(pErrString[pPacket->GetError()]);
				_snprintf(pErrContent, MAX_PATH, strTemp.c_str());
				//_snprintf(pErrContent, MAX_PATH, pErrString[pPacket->GetError()]);
			}
			break;

		default:
			return PACKET_EXE_CONTINUE;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, pErrContent );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
