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
		if( pAgent->getLog() ) pAgent->getLog()->addLog("�ʺ���֤�ɹ�!");
	}
	else
	{
		const char* szErrorDesc[] = 
		{
			"�ɹ�",
			"��֤ʧ��",
			"�汾����",
			"��������,���ֵ",
			"��ʱֹͣ����",
			"��������Ϸ��������",
			"δ֪����",
			"�˺�ͣȨ",
			"�ж�Ӧ��ɫ����,��ʾ�Ƿ��߳�",
			"passport �д���,��Ҫע��"
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
