#include "StdAfx.h"
#include "GCMinorPasswd.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"
#include "..\..\procedure\GamePro_CharCreate.h"

uint GCMinorPasswdHandler::Execute( GCMinorPasswd* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GC_MINORPASSWD* pRecv;
	pRecv = pPacket->GetMinorPassword();

	switch( pRecv->m_Type )
	{
	// �����Ƿ��Ѿ�����
	case MRETT_PASSWDSETUP:
		{
			
			if(1 == pRecv->m_ReturnSetup.m_uFlag)
			{// �Ѿ����ù�������
				
				// ��������ɹ�
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(TRUE);
				// ����δ��������״̬����
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "True" );
			}
			else
			{// û�����ù�������
				
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "True" );
			}
			break;
		}

	// ����������ʣ��ʱ�䣨���߲��ڽ���׶Σ�
	case MRETT_DELETEPASSWDTIME:
		{
			
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Apply_Force_Unlock_Password");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	

			TCHAR bufInfo[512];
			memset(bufInfo, 0, sizeof(bufInfo));
			
			strTemp = NOPARAMMSGFUNC("MinorPassword_Apply_Force_Unlock_Password_Time");
			_stprintf(bufInfo, strTemp.c_str(), (pRecv->m_ReturnDeleteTime.m_uTime / (1000 * 60 *60)));
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, bufInfo);	
			break;
		}

	// �����������óɹ�
	case MRETT_SETPASSWDSUCC:
		{
			// ��������ɹ�
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup( TRUE );

			// ����δ��������״̬����
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Set_Password_Ok");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "True" );
			
			break;
		}

	// ���������޸ĳɹ�
	case MRETT_MODIFYPASSWDSUCC:
		{
			// �޸�����ɹ�
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Modify_Password_OK");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// ������������ɹ�
	case MRETT_UNLOCKPASSWDSUCC:
		{
			// ��������ɹ�
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(TRUE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Unlock_Password_Ok");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// ���ñ������ͳɹ�
	case MRETT_SETPASSWDPROTECT:
		{
			INT flag = pRecv->m_ReturnSetup.GetSetupFlag();
			CDataPool::GetMe()->Rich_SetPasswordData( flag );

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���ñ������ͳɹ�" );
			break;
		}
	// �������߱���ʱ��ɹ�
	case MRETT_SETPROTECTTIMESUCC:
		{
			UINT time = pRecv->m_ReturnDeleteTime.GetDeleteTime();

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�������߱���ʱ��ɹ�" );
			break;
		}


	// ǿ�ƽ����������ʧЧ
	case MRETT_DELETEPASSWDCANCEL:
		{
			break;
		}

	// ��������ǿ�ƽ���ɹ�
	case MRETT_DELETEPASSWDSUCC:
		{
			// ǿ�ƽӴ�����ɹ���
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Force_Unlock_Password_OK");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// ������������ʧ��
	case MRETT_ERR_SETPASSWDFAIL:
		{
			// ���ö�������ʧ��
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Set_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	

			break;
		}

	// ���������޸�ʧ��
	case MRETT_ERR_MODIFYPASSWDFAIL:
		{// �޸�����ʧ��
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Modify_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// �����������ʧ��
	case MRETT_ERR_UNLOCKPASSWDFAIL:
		{
			// ����ʧ�ܡ�
			//CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Unlock_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// ��������ǿ�ƽ��ʧ��
	case MRETT_ERR_DELETEPASSWDFAIL:
		{
			// ǿ�ƽӴ�����ʧ�ܡ�
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Force_Unlock_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// ��ն�������ɹ�
	case MRETT_CLEANPASSWDSUCC:
		{
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�����Ѿ����");	
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "False" );
			break;
		}

	// ��ն�������ʧ��
	case MRETT_ERR_CLEANPASSWDFAIL:
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Modify_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// У������ʧ�� 
	case MRETT_ERR_PASSWD:
		{
//			CDataPool::GetMe()->Rich_SetInputPasswdState( nType, FALSE );

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "У������ʧ��!");	
			break;
		}

	// �������߱���ʱ��ʧ��
	case MRETT_ERR_SETPROTECTTIMEFAIL:
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�������߱���ʱ��ʧ��!");	
			break;
		}

	default :
		Assert(FALSE);
		return 0;
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

