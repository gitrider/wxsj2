#include "StdAfx.h"
#include "LCRetCharLogin.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMLog.h"
#include "LuaPlus.h"

using namespace Packets;
UINT LCRetCharLoginHandler::Execute(LCRetCharLogin* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION

		SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

		const char * szErrorDesc[] = 
		{
			"成功",
			"世界满了",
			"服务器忙，重试",
			"角色载入出错",
			"操作过于频繁",
			"不是角色的所有者",
			"服务器维护",
			"非法的SceneID",
			"角色在线稍后重进",
			"无足够点数",
			"月卡到期",
			"非法流程",
		};

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

					pAgent->getVariable()->setVariable("GameServer_Address", pServerIP );
					pAgent->getVariable()->setAs_Int("GameServer_Port", iPort );
					pAgent->getVariable()->setAs_Int("GameServer_Key", iUserKey );
					pAgent->getLuaState()->DoString("Procedure_CharSel_SetState(\"enter_scene\")");
					break;
				}

			case ASKCHARLIST_WORLD_FULL:
			case ASKCHARLOGIN_SERVER_BUSY:
			case ASKCHARLOGIN_LOADDB_ERROR:
			case ASKCHARLOGIN_OP_TIMES:	
			case ASKCHARLOGIN_NOT_OWNER:
			case ASKCHARLOGIN_SERVER_STOP:
			case ASKCHARLOGIN_INVALID_SCENE:
			case ASKCHARLOGIN_CHARLIVING:
			case ASKCHARLOGIN_NO_POINT:	
			case ASKCHARLOGIN_END_TIME:		
			case ASKCHARLOGIN_NOT_LOGON:
				{
					pAgent->getVariable()->setVariable("CharLogin_ErrorDesc", szErrorDesc[res]);
					pAgent->getLuaState()->DoString("Procedure_CharSel_SetState(\"enter_scene_error\")");
					break;
				}

			default:
				{
					char szErrorDesc[32];
					_snprintf(szErrorDesc, 32, "未知错误%d", res);

					pAgent->getVariable()->setVariable("CharLogin_ErrorDesc", szErrorDesc);
					pAgent->getLuaState()->DoString("Procedure_CharSel_SetState(\"enter_scene_error\")");
					break;
				}
			}
		}
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
