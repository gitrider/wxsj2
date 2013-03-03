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
			case ASKCHARLOGIN_SUCCESS:		//�ɹ�
				{
					UINT	iPort	  = pPacket->GetServerPort();
					CHAR*	pServerIP = pPacket->GetServerIP();
					UINT	iUserKey  = pPacket->GetUserKey();

					// ��¼������ip��ַ��ȫ�ֱ���
					CGameProcedure::s_pVariableSystem->SetVariable("GameServer_Address", pServerIP);
					// ��¼�������˿ں���ȫ�ֱ���
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Port", iPort);
					// ��¼������Ϸ��ɫ��guid
					CGameProcedure::s_pVariableSystem->SetAs_Int("User_GUID", CGameProcedure::s_pProcCharSel->m_EnterGameGUID);
					// ���볡����user key
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Key", iUserKey);

					// �л�����������������
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLEAR_ACCOUNT );
					CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);
					CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);

					break;
				}
			case ASKCHARLIST_WORLD_FULL:		//��������
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Server_Is_Full");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "����������! ");	
					break;
				}
			case ASKCHARLOGIN_SERVER_BUSY:	//������æ������
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Server_Is_Busy_Retry");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������æ������! ");	
					break;
				}
			case ASKCHARLOGIN_OP_TIMES:		//��������Ƶ��
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Server_Operate_Busy");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��������Ƶ�� ");	
					break;
				}
			default:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetCharLoginHandler_Info_Unknow_Err");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "δ֪����! ");	
					break;
				}
			}
		}
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
