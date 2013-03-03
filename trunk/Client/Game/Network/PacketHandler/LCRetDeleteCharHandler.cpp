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
			case ASKDELETECHAR_SUCCESS:		//�ɹ�
				{
					CGameProcedure::s_pProcCharSel->SetCurSelDel(-1);
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�õ���ɫ��Ϣ����! ");	
					break;
				}
			case ASKDELETECHAR_SERVER_BUSY://������æ������
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Server_Busy_Retry");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������æ������! ");	
					break;
				}
			case ASKDELETECHAR_OP_TIMES:		//��������Ƶ��
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Operate_Busy");
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�ٴ�ɾ��һ����ɫ��Ҫ15���ӣ�");
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��������Ƶ��! ");	
					break;
				}
			case ASKDELETECHARR_EMPTY:		//û�н�ɫɾ��
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_No_Role_Del");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "û�н�ɫɾ��! ");	
					break;
				}
			case ASKDELETECHAR_OP_ERROR:		//�����������
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Err_Produce");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�����������! ");	
					break;
				}
			case ASKDELETECHAR_INTERNAL_ERROR: //�ڲ�����
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Err");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�ڲ�����! ");	
					break;
				}
			default:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("LCRetDeleteCharHandler_Info_Unknow_err");
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