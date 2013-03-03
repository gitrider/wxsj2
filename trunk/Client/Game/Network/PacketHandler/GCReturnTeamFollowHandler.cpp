
/** GCReturnTeamFollowHandler.cpp
 */

#include "StdAfx.h"
#include "GCReturnTeamFollow.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"

#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GIUISystem.h"
#include "../../Interface/GMInterface_Script_Talk.h"

uint GCReturnTeamFollowHandler::Execute( GCReturnTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);
		CTeamOrGroup* team = pDataPool->GetTeamOrGroup();
		TeamMemberInfo* pTMInfo = team->GetMember( pPacket->GetGUID() );

		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		STRING strTemp = "";

		if( pTMInfo == NULL )
		{
			Assert(FALSE && "没有发现如上队员。");
			return PACKET_EXE_CONTINUE;
		}

		CObject_PlayerMySelf *pMe = CObjectManager::GetMe()->GetMySelf();
		CHAR szText[_MAX_PATH];

		switch( pPacket->GetReturn() )
		{
		// 队员拒绝跟随队长
		case TF_RESULT_REFUSE_FOLLOW:
			{
				STRING strTemp = "";
				strTemp = NOPARAMMSGFUNC("GCReturnTeamFollowHandler_Info_Reject_Follow");
				_snprintf(szText, _MAX_PATH, strTemp.c_str(), pTMInfo->m_szNick);	// "%s拒绝跟随。"

				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
			}
			break;

		// 队员进入组队跟随状态
		case TF_RESULT_ENTER_FOLLOW:
			{
				GUID_t myGUID = pMe->GetServerGUID();

				if( pPacket->GetGUID() == myGUID )
				{
					SCommand_Object cmdTemp;

					cmdTemp.m_wID				= OC_UPDATE_TEAM_FOLLOW_FLAG;
					cmdTemp.m_abParam[0]		= TRUE;

					pMe->PushCommand( &cmdTemp );

					//msg
					if( bLog )
					{
						TeamMemberInfo* pTMInfo = team->GetMember(team->GetLeaderGUID());
						strTemp = COLORMSGFUNC("team_follow_other2", pTMInfo->m_szNick);
						ADDTALKMSG(strTemp);
						
						strTemp = NOCOLORMSGFUNC("team_follow_other2", pTMInfo->m_szNick);
						ADDNEWDEBUGMSG(strTemp);
					}
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCReturnTeamFollowHandler_Info_Join_Follow");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pTMInfo->m_szNick);	// "%s加入跟随队列。"

					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
				}
			}
			break;

		// 队员退出组队跟随状态
		case TF_RESULT_STOP_FOLLOW:
			{
				GUID_t myGUID = pMe->GetServerGUID();

				if( pPacket->GetGUID() == myGUID )
				{
					SCommand_Object cmdTemp;

					cmdTemp.m_wID				= OC_UPDATE_TEAM_FOLLOW_FLAG;
					cmdTemp.m_abParam[0]		= FALSE;

					pMe->PushCommand( &cmdTemp );

					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCReturnTeamFollowHandler_Info_Exit_Follow");
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );	// "退出组队跟随状态。"
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GCReturnTeamFollowHandler_Info_Someone_Exit_Follow");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pTMInfo->m_szNick);	// "%s退出跟随队列。"

					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
				}
			}
			break;

		// 通知客户端进入组队跟随状态的标记（ENTER_FOLLOW 的 quiet 模式）
		case TF_RESULT_FOLLOW_FLAG:
			{
				GUID_t myGUID = pMe->GetServerGUID();

				if( pPacket->GetGUID() == myGUID )
				{
					SCommand_Object cmdTemp;

					cmdTemp.m_wID				= OC_UPDATE_TEAM_FOLLOW_FLAG;
					cmdTemp.m_abParam[0]		= TRUE;

					pMe->PushCommand( &cmdTemp );
				}
			}
			break;

		default:
			{
				Assert(FALSE && "组队跟随不明返回类型。");
				return PACKET_EXE_CONTINUE;
			}
		}

	//	CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "GCReturnTeamFollow Received." );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
