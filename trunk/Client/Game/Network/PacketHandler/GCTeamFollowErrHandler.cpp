
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
		case TF_ERROR_TOO_FAR:				// ��ӳ�̫Զ�ˣ������ܸ��棩
		case TF_ERROR_IN_TEAM_FOLLOW:		// �Ѿ�������Ӹ���״̬����������ĳЩ������
		case TF_ERROR_STALL_OPEN:			// ���ڰ�̯
		case TF_ERROR_NOT_IN_FOLLOW_MODE:	// ����Ŀǰ�����ڸ���״̬
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
