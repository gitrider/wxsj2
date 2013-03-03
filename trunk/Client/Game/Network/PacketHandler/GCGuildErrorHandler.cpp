
/** GCGuildErrorHandler.cpp
 *	�������Ĵ�����Ϣ����
 */

#include "StdAfx.h"
#include "GCGuildError.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

uint GCGuildErrorHandler::Execute( GCGuildError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR szText[_MAX_PATH];

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Other_Guild_Err");
		_snprintf(szText, _MAX_PATH, strTemp.c_str());
		//_snprintf(szText, _MAX_PATH, "����������");

		switch( pPacket->GetErrorCode() )
		{
		// �����ϴ���������ֻ���������������Ĵ��������ɽű�����
		case GUILD_ERROR_WANTING			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Condition_Is_Not_Enough");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "�����㴴��������");
			}
			break;

		// ���ַǷ�
		case GUILD_ERROR_INVALID_NAME		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Name_Err");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "������ƷǷ���");
			}
			break;

		// ����
		case GUILD_ERROR_DUPLICATED_NAME	:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Name_Exist");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "������������������ظ���");
			}
			break;

		// ���ܴ���������
		case GUILD_ERROR_GUILD_FULL			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Can_Not_Create_More_Guild");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "�޷����������ᡣ");
			}
			break;

		// �����Ա��
		case GUILD_ERROR_MEMBER_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Member_Is_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "�����Ա�ˡ�");
			}
			break;

		// ����������
		case GUILD_ERROR_PROPOSER_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Apply_Is_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "������������");
			}
			break;

		// ����Ѽ�����
		case GUILD_ERROR_IN_GUILD			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_You_Have_Joined");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "���Ѽ����ᡣ");
			}
			break;

		// ��᲻����
		case GUILD_ERROR_NOT_EXIST			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Not_Exist");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "��᲻���ڡ�");
			}
			break;

		// û��Ȩ��
		case GUILD_ERROR_UNAUTHORIZED		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_No_Right");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "û��Ȩ�ޡ�");
			}
			break;

		// ����ְ��û��ְλ��ȱ
		case GUILD_ERROR_NO_VACANCY			:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_No_Job");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "û��ְλ��ȱ��");
			}
			break;

		// ����Ȩ�ޣ���������Ա��ְλ�������ʸ�
		case GUILD_ERROR_NO_QUALIFICATION	:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Member_Is_Not_Enough");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "��������Ա��ְλ�������ʸ�");
			}
			break;

		// ��������������
		case GUILD_ERROR_CASHBOX_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Money_Is_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "�����������");
			}
			break;

		// ���ְλ������������������
		case GUILD_ERROR_POS_FULL		:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Full");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "���ְλ��������������������");
			}
			break;

		// ��������
		case GUILD_ERROR					:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildErrorHander_Info_Guild_Unknow_err");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
				//_snprintf(szText, _MAX_PATH, "δ֪������");
			}
			break;

		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_SYSTEM_MESSAGE, szText );
//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "���", szText, 0 );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
