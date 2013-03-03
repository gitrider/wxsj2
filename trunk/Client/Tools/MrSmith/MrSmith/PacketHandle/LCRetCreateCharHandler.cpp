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
			case ASKCREATECHAR_SUCCESS:			//�ɹ�
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_success\")");
					break;
				}
			case ASKCREATECHAR_SERVER_BUSY:		//������æ������
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_repeat\")");
					break;
				}
			case ASKCREATECHAR_OP_TIMES:		//��������Ƶ��
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_too_fast\")");
					break;
				}
			case ASKCREATECHAR_FULL:			//��ɫ�Ѿ�����
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_full\")");
					break;
				}
			case ASKCREATECHAR_OP_ERROR:		//�����������
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_proc\")");
					break;
				}
			case ASKCREATECHAR_INTERNAL_ERROR:  //�ڲ�����
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_inner\")");
					break;
				}
			case ASKCREATECHAR_SAME_NAME:  // �����ظ�
				{
					pAgent->getLuaState()->DoString("Procedure_CharCreate_SetState(\"create_char_name\")");
					break;
				}
			default:							// δ֪����
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
