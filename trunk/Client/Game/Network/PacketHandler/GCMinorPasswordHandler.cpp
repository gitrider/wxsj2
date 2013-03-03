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
	// 密码是否已经设置
	case MRETT_PASSWDSETUP:
		{
			
			if(1 == pRecv->m_ReturnSetup.m_uFlag)
			{// 已经设置过的密码
				
				// 设置密码成功
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(TRUE);
				// 设置未解锁密码状态。。
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "True" );
			}
			else
			{// 没有设置过的密码
				
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "True" );
			}
			break;
		}

	// 二级密码解除剩余时间（或者不在解除阶段）
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

	// 二级密码设置成功
	case MRETT_SETPASSWDSUCC:
		{
			// 设置密码成功
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup( TRUE );

			// 设置未解锁密码状态。。
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Set_Password_Ok");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "True" );
			
			break;
		}

	// 二级密码修改成功
	case MRETT_MODIFYPASSWDSUCC:
		{
			// 修改密码成功
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Modify_Password_OK");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// 二级密码解锁成功
	case MRETT_UNLOCKPASSWDSUCC:
		{
			// 解锁密码成功
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(TRUE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Unlock_Password_Ok");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// 设置保护类型成功
	case MRETT_SETPASSWDPROTECT:
		{
			INT flag = pRecv->m_ReturnSetup.GetSetupFlag();
			CDataPool::GetMe()->Rich_SetPasswordData( flag );

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "设置保护类型成功" );
			break;
		}
	// 设置上线保护时间成功
	case MRETT_SETPROTECTTIMESUCC:
		{
			UINT time = pRecv->m_ReturnDeleteTime.GetDeleteTime();

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "设置上线保护时间成功" );
			break;
		}


	// 强制解除二级密码失效
	case MRETT_DELETEPASSWDCANCEL:
		{
			break;
		}

	// 二级密码强制解除成功
	case MRETT_DELETEPASSWDSUCC:
		{
			// 强制接触密码成功。
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Force_Unlock_Password_OK");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// 二级密码设置失败
	case MRETT_ERR_SETPASSWDFAIL:
		{
			// 设置二级密码失败
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Set_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	

			break;
		}

	// 二级密码修改失败
	case MRETT_ERR_MODIFYPASSWDFAIL:
		{// 修改密码失败
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Modify_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// 二级密码解锁失败
	case MRETT_ERR_UNLOCKPASSWDFAIL:
		{
			// 解锁失败。
			//CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Unlock_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// 二级密码强制解除失败
	case MRETT_ERR_DELETEPASSWDFAIL:
		{
			// 强制接触密码失败。
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Force_Unlock_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// 清空二级密码成功
	case MRETT_CLEANPASSWDSUCC:
		{
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "密码已经清除");	
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK, "password", "False" );
			break;
		}

	// 清空二级密码失败
	case MRETT_ERR_CLEANPASSWDFAIL:
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("MinorPassword_Modify_Password_Fail");
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	
			break;
		}

	// 校验密码失败 
	case MRETT_ERR_PASSWD:
		{
//			CDataPool::GetMe()->Rich_SetInputPasswdState( nType, FALSE );

			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "校验密码失败!");	
			break;
		}

	// 设置上线保护时间失败
	case MRETT_ERR_SETPROTECTTIMEFAIL:
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "设置上线保护时间失败!");	
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

