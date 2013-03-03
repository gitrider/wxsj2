#include "StdAfx.h"
#include "LCRetCharLogin.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"
#include "..\..\procedure\GamePro_CharCreate.h"
#include "..\..\procedure\Gamepro_ChangeScene.h"
#include "GIVariable.h"
UINT LCRetCharLoginHandler::Execute(LCRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


		if(pPacket)
		{
			ASKCHARLOGIN_RESULT res;
			res = pPacket->GetResult();
			switch(res)
			{
			case ASKCHARLOGIN_SUCCESS:		//成功
				{
					UINT	iPort	  = pPacket->GetServerPort();
					CHAR*	pServerIP = pPacket->GetServerIP();
					UINT	iUserKey  = pPacket->GetUserKey();

					// 记录服务器ip地址在全局变量
					CGameProcedure::s_pVariableSystem->SetVariable("GameServer_Address", pServerIP);
					// 记录服务器端口号在全局变量
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Port", iPort);
					// 记录进入游戏角色的guid
					CGameProcedure::s_pVariableSystem->SetAs_Int("User_GUID", CGameProcedure::s_pProcCharSel->m_EnterGameGUID);
					// 进入场景的user key
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Key", iUserKey);

					// 切换到服务器连接流程
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLEAR_ACCOUNT );
					CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);
					CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);

					break;
				}
			case ASKCHARLIST_WORLD_FULL:		//世界满了
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Server_Is_Full");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器满了! ");	
					break;
				}
			case ASKCHARLOGIN_SERVER_BUSY:	//服务器忙，重试
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Server_Is_Busy_Retry");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器忙，重试! ");	
					break;
				}
			case ASKCHARLOGIN_OP_TIMES:		//操作过于频繁
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Server_Operate_Busy");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "操作过于频繁 ");	
					break;
				}
			default:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Unknow_Err");
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
