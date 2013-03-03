#include "StdAfx.h"
#include "LCRetCreateChar.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "LuaPlus.h"

using namespace Packets;

UINT LCRetCreateCharHandler::Execute(LCRetCreateChar* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION

		SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

		if(pPacket)
		{
			ASKCREATECHAR_RESULT	res = 	pPacket->GetResult();

			switch(res)
			{
			case ASKCREATECHAR_SUCCESS:			//成功
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_success\")");
					break;
				}
			case ASKCREATECHAR_SERVER_BUSY:		//服务器忙，重试
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_repeat\")");
					break;
				}
			case ASKCREATECHAR_OP_TIMES:		//操作过于频繁
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_too_fast\")");
					break;
				}
			case ASKCREATECHAR_FULL:			//角色已经满了
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_full\")");
					break;
				}
			case ASKCREATECHAR_OP_ERROR:		//错误操作流程
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_proc\")");
					break;
				}
			case ASKCREATECHAR_INTERNAL_ERROR:  //内部错误
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_inner\")");
					break;
				}
			case ASKCREATECHAR_SAME_NAME:  // 名字重复
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_name\")");
					break;
				}
			default:							// 未知错误
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_unknow\")");
					break;
				}
			}
		}

		return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
