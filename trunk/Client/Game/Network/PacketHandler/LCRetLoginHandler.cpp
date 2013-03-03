#include "StdAfx.h"
#include "LCRetLogin.h"
#include "Type.h"
#include "CLAskCharList.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\Gameprocedure.h"
#include "..\..\procedure\GamePro_Login.h"

UINT	LCRetLoginHandler::Execute(LCRetLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	
	if((pPacket)&&(pPacket->GetResult() == LOGINR_SUCCESS))
	{
		
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "�ȴ�������Ϸ...");
		
	}
	else
	{
		int iLoginRes = pPacket->GetResult();
		switch(iLoginRes)
		{
			
		case LOGINR_AUTH_FAIL:			//��֤ʧ��
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("LCRetLoginHandler_Info_UserId_Password_Err");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��֤�û����������!");
				break;
			}

		case LOGINR_VERSION_FAIL:		//�汾����
			{
		//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�汾����!");
				break;
			}
		case LOGINR_NOT_EN_POINT:		//��������
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("LCRetLoginHandler_Info_Point_Is_Not_Enough");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��������!");
				break;
			}
		case LOGINR_STOP_SERVICE:		//��ʱֹͣ����
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("LCRetLoginHandler_Info_Server_Is_Stop");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������ֹͣ����!");
				break;
			}
		default:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("LCRetLoginHandler_Info_Unknow_Err");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "δ֪����!");
				break;
			}

		}

		CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING);
		
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}