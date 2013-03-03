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
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR *pName = new CHAR[MAX_CHARACTER_NAME];
		strncpy(pName, CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name(), MAX_CHARACTER_NAME-1);
		CHAR *pContex= new CHAR[MAX_PATH];

		switch( pPacket->GetErrorCode() )
		{
		case TEAM_ERROR_INVITEDESTHASTEAM :		// 邀请对象已经属于某个组了
		case TEAM_ERROR_INVITEREFUSE :			// 被邀请人拒绝加入
		case TEAM_ERROR_INVITETEAMFULL :		// 邀请人的队伍人数已经满了
		case TEAM_ERROR_INVITELEADERREFUSE :	// 队长拒绝新成员被邀请加入
		case TEAM_ERROR_DISMISSNOTLEADER :		// 解散队伍的人不是队长
		case TEAM_ERROR_KICKNOTLEADER :			// 踢人者不是队长
		case TEAM_ERROR_APPLYSOURHASTEAM :		// 申请人已经属于某个组了
		case TEAM_ERROR_APPLYDESTHASNOTTEAM :	// 被申请者不属于某个组
		case TEAM_ERROR_APPLYLEADERREFUSE :		// 队长不同意申请人加入队伍
		case TEAM_ERROR_APPLYTEAMFULL :			// 被申请人的队伍人数已经满了
		case TEAM_ERROR_APPLYLEADERGUIDERROR :	// 被申请人所在队伍的队长GUID发生变化
		case TEAM_ERROR_APPOINTSOURNOTEAM :		// 旧队长不是队伍成员
		case TEAM_ERROR_APPOINTDESTNOTEAM :		// 新队长不是队伍成员
		case TEAM_ERROR_APPOINTNOTSAMETEAM :	// 任命时两个人不属于同一个队伍
		case TEAM_ERROR_APPOINTSOURNOLEADER :	// 旧队长不是队伍的队长了
		case TEAM_ERROR_APPLYLEADERCANTANSWER:	// 队长目前无法答复
		case TEAM_ERROR_INVITERNOTINTEAM:		// 邀请人不在队长的队伍中
		case TEAM_ERROR_APPLYWHENINTEAM:		// 申请人在有队伍的情况下申请入队
		case TEAM_ERROR_TEAMFULL:				// 队伍人数已满
		case TEAM_ERROR_REFUSEINVITESETTING:	// 被邀请人设置了拒绝邀请
		case TEAM_ERROR_TARGETNOTONLINE:		// 对方已经离线，加入失败。
		//case TEAM_ERROR_INVITEEOFFLINE:			// 被邀请人不在线，邀请失败
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC( pString[ pPacket->GetErrorCode() ] );
				strncpy(pContex, strTemp.c_str(), MAX_PATH-1);
				//strncpy(pContex, pString[ pPacket->GetErrorCode() ], MAX_PATH-1);
			}
			break;

		case TEAM_ERROR_LEAVETEAM:
			{
				STRING strTemp = "对方已经离开队伍了，请加入其它队伍。";
				strncpy( pContex, strTemp.c_str(), MAX_PATH - 1 );
			}
			break;

		case TEAM_ERROR_INVITEEOFFLINE:
			{
				STRING strTemp = "对方处于离线状态，执行操作失败。";
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
