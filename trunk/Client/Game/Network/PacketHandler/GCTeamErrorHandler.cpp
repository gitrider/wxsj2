#include "StdAfx.h"

#include "GCTeamError.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\Datapool\GMDP_CharacterData.h"
#include "GIException.h"

const char* pString[] = {
	"GCTeamErrorHandler_Info_Target_Join_Team",
	"GCTeamErrorHandler_Info_Target_Reject_Invite",
	"GCTeamErrorHandler_Info_You_Team_Is_Full",
	"GCTeamErrorHandler_Info_Leader_Reject_Inviter_Join",
	"GCTeamErrorHandler_Info_Leader_Can_Dismiss_Team",
	"GCTeamErrorHandler_Info_Leader_Can_Kick_Member",
	"GCTeamErrorHandler_Info_Target_Join_Other_Team",
	"GCTeamErrorHandler_Info_Target_No_Team",
	"GCTeamErrorHandler_Info_Leader_Reject_Apply",
	"GCTeamErrorHandler_Info_Target_Team_Is_Full",
	"GCTeamErrorHandler_Info_Leader_Change",
	"GCTeamErrorHandler_Info_You_Are_Not_Leader_Can_Not_Appoint",
	"GCTeamErrorHandler_Info_Can_Not_Appoint_Not_Member",
	"GCTeamErrorHandler_Info_Can_Not_Appoint_Member_Leave",
	"GCTeamErrorHandler_Info_Can_Not_Appoint_You_Not_Leader",
	"GCTeamErrorHandler_Info_Target_Leader_Not_Respone",
	"GCTeamErrorHandler_Info_Invitor_Not_In_Team",
	"GCTeamErrorHandler_Info_You_Leave_Old_Then_Join_New",
	"GCTeamErrorHandler_Info_Team_Is_Full",
	"GCTeamErrorHandler_Info_Target_Set_Refuse_Invite",
	"GCTeamErrorHandler_Info_Target_Is_Not_Online",
};

uint GCTeamErrorHandler::Execute( GCTeamError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR *pName = new CHAR[MAX_CHARACTER_NAME];
		strncpy(pName, CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name(), MAX_CHARACTER_NAME-1);
		CHAR *pContex= new CHAR[MAX_PATH];

		switch( pPacket->GetErrorCode() )
		{
		case TEAM_ERROR_INVITEDESTHASTEAM :		// ��������Ѿ�����ĳ������
		case TEAM_ERROR_INVITEREFUSE :			// �������˾ܾ�����
		case TEAM_ERROR_INVITETEAMFULL :		// �����˵Ķ��������Ѿ�����
		case TEAM_ERROR_INVITELEADERREFUSE :	// �ӳ��ܾ��³�Ա���������
		case TEAM_ERROR_DISMISSNOTLEADER :		// ��ɢ������˲��Ƕӳ�
		case TEAM_ERROR_KICKNOTLEADER :			// �����߲��Ƕӳ�
		case TEAM_ERROR_APPLYSOURHASTEAM :		// �������Ѿ�����ĳ������
		case TEAM_ERROR_APPLYDESTHASNOTTEAM :	// �������߲�����ĳ����
		case TEAM_ERROR_APPLYLEADERREFUSE :		// �ӳ���ͬ�������˼������
		case TEAM_ERROR_APPLYTEAMFULL :			// �������˵Ķ��������Ѿ�����
		case TEAM_ERROR_APPLYLEADERGUIDERROR :	// �����������ڶ���Ķӳ�GUID�����仯
		case TEAM_ERROR_APPOINTSOURNOTEAM :		// �ɶӳ����Ƕ����Ա
		case TEAM_ERROR_APPOINTDESTNOTEAM :		// �¶ӳ����Ƕ����Ա
		case TEAM_ERROR_APPOINTNOTSAMETEAM :	// ����ʱ�����˲�����ͬһ������
		case TEAM_ERROR_APPOINTSOURNOLEADER :	// �ɶӳ����Ƕ���Ķӳ���
		case TEAM_ERROR_APPLYLEADERCANTANSWER:	// �ӳ�Ŀǰ�޷���
		case TEAM_ERROR_INVITERNOTINTEAM:		// �����˲��ڶӳ��Ķ�����
		case TEAM_ERROR_APPLYWHENINTEAM:		// ���������ж����������������
		case TEAM_ERROR_TEAMFULL:				// ������������
		case TEAM_ERROR_REFUSEINVITESETTING:	// �������������˾ܾ�����
		case TEAM_ERROR_TARGETNOTONLINE:		// �Է��Ѿ����ߣ�����ʧ�ܡ�
		//case TEAM_ERROR_INVITEEOFFLINE:			// �������˲����ߣ�����ʧ��
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC( pString[ pPacket->GetErrorCode() ] );
				strncpy(pContex, strTemp.c_str(), MAX_PATH-1);
				//strncpy(pContex, pString[ pPacket->GetErrorCode() ], MAX_PATH-1);
			}
			break;

		case TEAM_ERROR_LEAVETEAM:
			{
				STRING strTemp = "�Է��Ѿ��뿪�����ˣ�������������顣";
				strncpy( pContex, strTemp.c_str(), MAX_PATH - 1 );
			}
			break;

		case TEAM_ERROR_INVITEEOFFLINE:
			{
				STRING strTemp = "�Է���������״̬��ִ�в���ʧ�ܡ�";
				strncpy( pContex, strTemp.c_str(), MAX_PATH - 1 );
			}
			break;

		default:
			KLAssert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, pContex );
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, pContex );

//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, pName, pContex, 0 );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
