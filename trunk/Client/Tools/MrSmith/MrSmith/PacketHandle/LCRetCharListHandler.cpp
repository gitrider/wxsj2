#include "StdAfx.h"
#include "LCRetCharList.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMLog.h"
#include "LuaPlus.h"

#include "../DataPool/SMDataPool.h"
#include "../DataPool/SMDataPool_CharacterList.h"

using namespace Packets;
UINT LCRetCharListHandler::Execute(LCRetCharList* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

	if(pPacket->GetResult() == ASKCHARLIST_SUCCESS)
	{
		int iCharacterCount = pPacket->GetCharNumber();

		if(iCharacterCount > 0)
		{
			for(int i = 0; i < iCharacterCount; i++)
			{
				DB_CHAR_BASE_INFO* pInfo = pPacket->GetCharBaseInfo(i);
				if(pInfo)
				{
					pAgent->getDataPool()->m_pLoginCharacterList->insertCharInfo(pInfo);
					//pAgent->getVariable()->setAs_CharBaseInfo( i, pInfo );
				}
			}
		}
		pAgent->getLuaState()->DoString("Procedure_CharSel_SetState(\"char_list\")");
	}
	else
	{
		int iLoginRes = pPacket->GetResult();
		switch(iLoginRes)
		{
		case ASKCHARLIST_OP_FAILS:
			pAgent->getVariable()->setVariable("CharListErrorDesc", "����ʧ��");
			break;
		case ASKCHARLIST_SERVER_BUSY:
			pAgent->getVariable()->setVariable("CharListErrorDesc", "������æ������");
			break;
		case ASKCHARLIST_OP_TIMES:
			pAgent->getVariable()->setVariable("CharListErrorDesc", "��������Ƶ��");
			break;
		default:
			pAgent->getVariable()->setVariable("CharListErrorDesc", "δ֪����");
			break;
		}
		pAgent->getLuaState()->DoString("Procedure_CharSel_SetState(\"char_list_err\")");
	}

	return PACKET_EXE_CONTINUE;
}
