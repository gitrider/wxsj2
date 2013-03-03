#include "StdAfx.h"
#include "LCRetDeleteChar.h"
#include "gamestruct.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"

UINT LCRetDeleteCharHandler::Execute(LCRetDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


		if(pPacket)
		{
			ASKDELETECHAR_RESULT res = pPacket->GetResult();
			switch(res)
			{
			case ASKDELETECHAR_SUCCESS:		//成功
				{
					CGameProcedure::s_pProcCharSel->SetCurSelDel(-1);
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "得到角色信息出错! ");	
					break;
				}
			case ASKDELETECHAR_SERVER_BUSY://服务器忙，重试
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Server_Busy_Retry");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器忙，重试! ");	
					break;
				}
			case ASKDELETECHAR_OP_TIMES:		//操作过于频繁
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Operate_Busy");
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "再次删除一个角色需要15秒钟！");
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "操作过于频繁! ");	
					break;
				}
			case ASKDELETECHARR_EMPTY:		//没有角色删除
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_No_Role_Del");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "没有角色删除! ");	
					break;
				}
			case ASKDELETECHAR_OP_ERROR:		//错误操作流程
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Err_Produce");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "错误操作流程! ");	
					break;
				}
			case ASKDELETECHAR_INTERNAL_ERROR: //内部错误
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Err");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "内部错误! ");	
					break;
				}
			default:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Unknow_err");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "未知错误! ");	
					break;
				}
			}
		}
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}