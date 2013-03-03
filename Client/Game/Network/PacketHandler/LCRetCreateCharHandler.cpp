#include "StdAfx.h"
#include "LCRetCreateChar.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharCreate.h"

UINT	LCRetCreateCharHandler::Execute(LCRetCreateChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

			
		if(pPacket)
		{
			ASKCREATECHAR_RESULT	res = 	pPacket->GetResult();

			switch(res)
			{
			case ASKCREATECHAR_SUCCESS:			//成功
				{
					//CGameProcedure::s_pProcCharCreate->ChangeToRoleSel();

					CGameProcedure::s_pProcCharCreate->m_Status = CGamePro_CharCreate::CHAR_CREATE_CREATE_OK;
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CREATE_CLEAR_NAME );
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CREATE_ROLE_OK );
					break;
				}
			case ASKCREATECHAR_SERVER_BUSY:		//服务器忙，重试
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCreateCharHandler_Info_Server_Busy_Retry");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器忙，重试! ");	
					break;
				}
			case ASKCREATECHAR_OP_TIMES:		//操作过于频繁
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCreateCharHandler_Info_Operate_Busy");
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "再次创建一个角色需要15秒钟！");
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "操作过于频繁! ");	
					break;
				}
			case ASKCREATECHAR_FULL:			//角色已经满了
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCreateCharHandler_Info_Role_Is_Full");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色已经满了! ");	
					break;
				}
			case ASKCREATECHAR_OP_ERROR:		//错误操作流程
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCreateCharHandler_Info_Err_Operate_Produce");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "错误操作流程! ");	
					break;
				}
			case ASKCREATECHAR_INTERNAL_ERROR:  //内部错误
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCreateCharHandler_Info_Err");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "内部错误! ");	
					break;
				}
			case ASKCREATECHAR_SAME_NAME:
				{
					// 名字重复
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("Login_Create_Role_Name_Exist");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					break;
				}
			default:							// 未知错误
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCreateCharHandler_Info_Unknow_Err");
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