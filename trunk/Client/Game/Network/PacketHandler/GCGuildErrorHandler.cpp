
/** GCGuildErrorHandler.cpp
 *	帮会操作的错误信息反馈
 */

#include "StdAfx.h"
#include "GCGuildError.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

uint GCGuildErrorHandler::Execute( GCGuildError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR szText[_MAX_PATH];

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Other_Guild_Err");
		_snprintf(szText, _MAX_PATH, strTemp.c_str());
		//_snprintf(szText, _MAX_PATH, "其他帮会错误。");

		switch( pPacket->GetErrorCode() )
		{
		// 不符合创建条件，只给出错误符，具体的错误描述由脚本发送
		case GUILD_ERROR_WANTING			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Condition_Is_Not_Enough");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "不满足创建条件。");
			}
			break;

		// 名字非法
		case GUILD_ERROR_INVALID_NAME		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Name_Err");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "帮会名称非法。");
			}
			break;

		// 重名
		case GUILD_ERROR_DUPLICATED_NAME	:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Name_Exist");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "帮会名称与现有名称重复。");
			}
			break;

		// 不能创建更多帮会
		case GUILD_ERROR_GUILD_FULL			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Can_Not_Create_More_Guild");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "无法创建更多帮会。");
			}
			break;

		// 帮会满员了
		case GUILD_ERROR_MEMBER_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Member_Is_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "帮会满员了。");
			}
			break;

		// 申请人已满
		case GUILD_ERROR_PROPOSER_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Apply_Is_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "申请人已满。");
			}
			break;

		// 玩家已加入帮会
		case GUILD_ERROR_IN_GUILD			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_You_Have_Joined");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "你已加入帮会。");
			}
			break;

		// 帮会不存在
		case GUILD_ERROR_NOT_EXIST			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Not_Exist");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "帮会不存在。");
			}
			break;

		// 没有权限
		case GUILD_ERROR_UNAUTHORIZED		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_No_Right");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "没有权限。");
			}
			break;

		// 任免职务，没有职位空缺
		case GUILD_ERROR_NO_VACANCY			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_No_Job");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "没有职位空缺。");
			}
			break;

		// 调整权限，被调整人员（职位）不够资格
		case GUILD_ERROR_NO_QUALIFICATION	:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Member_Is_Not_Enough");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "被调整人员（职位）不够资格。");
			}
			break;

		// 存入金额，帮会金库已满
		case GUILD_ERROR_CASHBOX_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Money_Is_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "帮会金库已满。");
			}
			break;

		// 这个职位人数已满，不能任命
		case GUILD_ERROR_POS_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "这个职位人数已满，不能任命。");
			}
			break;

		// 不明错误
		case GUILD_ERROR					:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Unknow_err");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "未知帮会错误。");
			}
			break;

		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_SYSTEM_MESSAGE, szText );
//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "帮会", szText, 0 );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
