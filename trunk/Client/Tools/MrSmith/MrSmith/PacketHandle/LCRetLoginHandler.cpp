#include "StdAfx.h"
#include "LCRetLogin.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMLog.h"
#include "LuaPlus.h"

using namespace Packets;
UINT LCRetLoginHandler::Execute(LCRetLogin* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

	if(pPacket->GetResult() == LOGINR_SUCCESS)
	{
		if( pAgent->getLog() ) pAgent->getLog()->addLog("帐号验证成功!");
	}
	else
	{
		const char* szErrorDesc[] = 
		{
			"成功",
			"验证失败",
			"版本错误",
			"点数不够,请冲值",
			"暂时停止服务",
			"在其他游戏世界在线",
			"未知错误",
			"账号停权",
			"有对应角色在线,提示是否踢出",
			"passport 中存在,需要注册"
		};

		int iLoginRes = pPacket->GetResult();
		switch(iLoginRes)
		{
		case LOGINR_AUTH_FAIL:
		case LOGINR_VERSION_FAIL:
		case LOGINR_NOT_EN_POINT:
		case LOGINR_STOP_SERVICE:
		case LOGINR_OTHER_ONLINE:
		case LOGINR_OTHER_ERROR:	
		case LOGINR_FORBIT:		
		case LOGINR_USER_ONLINE:	
		case LOGINR_NEED_REG:
			if( pAgent->getVariable() )
			{
				pAgent->getVariable()->setVariable("LoginErrorDesc", szErrorDesc[iLoginRes]);
			}
			break;
		default:
			if( pAgent->getVariable() ) pAgent->getVariable()->setVariable("LoginErrorDesc", "UNKNOWN");
			break;
		}

		if( pAgent->getLuaState() ) pAgent->getLuaState()->DoString("Procedure_Login_SetState(\"accnt_val_failed\")");
	}

	return PACKET_EXE_CONTINUE;
}
