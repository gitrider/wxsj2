
/**	GamePro_CharSel.cpp
 *	
 *	���ܣ�	�����½�����е�ѡ���ɫ
 *			ͨ������ϵ���������֣���ɫ�Ĵ�����������Ϸ�����ص��ʺ��������
 *
 *	�޸ļ�¼��
 *			080220	�ڽ�����Ϸ���¼� SendEnterGameMsg �У� ���뱣��ѡ��Ľ�ɫ����Ϣ�� �������ӵ� GameServer ʱ��
 *
 */


#include "StdAfx.h"
#include "Global.h"
#include "GameCommand.h"
#include "Network\NetManager.h"
#include "GamePro_CharSel.h"
#include "Gamepro_login.h"
#include "GamePro_Enter.h"
#include "GamePro_CharCreate.h"
#include "Gamepro_ChangeScene.h"
#include "Input\GMInputSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "Object\ObjectCommandDef.h"
#include "Object\ObjectDef.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Event\GMEventSystem.h"
#include "DBC\GMDataBase.h"
#include "CGEnterScene.h"
#include "CLAskDeleteChar.h"
#include "CLAskCharLogin.h"
#include "GameStruct.h"
#include "GIGfxSystem.h"
#include "GIException.h"
#include "GIVariable.h"



CGamePro_CharSel::CGamePro_CharSel()
{
	m_Status = CHARSEL_CHARSHOW;
	m_bIsLoadScene		= false;
	m_iCharacterCount	= 0;

	m_bClearUIModel = true;

	m_bInitUiModel = false;
}

CGamePro_CharSel::~CGamePro_CharSel()
{
	ClearUIModel();
}


VOID CGamePro_CharSel::Init(VOID)
{
	if(m_bClearUIModel)
	{
		ClearUIModel();
	}
}

VOID CGamePro_CharSel::Tick(VOID)
{
	CGameProcedure::Tick();
	//CGameProcedure::ProcessActiveInput();

	switch(m_Status)
	{
	case CHARSEL_CHARSHOW:
		{
			break;
		}

	case CHARSEL_SELDONE:
		{
			break;
		}
	case CHARSEL_CONNNECT_GAME_SERVER_READY:// ����Ϸ������������׼����, ��������.
		{
			break;
		}
	}

}

VOID CGamePro_CharSel::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_CharSel::Release(VOID)
{
}

VOID CGamePro_CharSel::CloseRequest(VOID)
{
}

VOID CGamePro_CharSel::SetStatus(PLAYER_CHARSEL_STATUS status)
{
	m_Status = status;
}

VOID CGamePro_CharSel::ProcessInput(VOID)
{
	////-------------------------------------------------------
	////���� 'A'
	//if(s_pInputSystem->IsKeyPress(KC_A))
	//{
	//	m_Status = CHARSEL_SELDONE;
	//}

	////-------------------------------------------------------
	////���� '1' ������ɫ����
	//if(s_pInputSystem->IsKeyPress(KC_1))
	//{
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);
	//}

	////-------------------------------------------------------
	////���� '2' ������Ϸ����.
	//if(s_pInputSystem->IsKeyPress(KC_2))
	//{
	//	m_bIsLoadScene = false;
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	//}//

	//�����¼�����
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;
		if(event.nEvent != EVENT_KEY_UP) continue;
		switch(event.kIndex)
		{
		case KC_GRAVE:
			{
			}
			break;
		case KC_1:
			{
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);
			}
			break;
		case KC_2:
			{
				m_Status = CHARSEL_SELDONE;
				m_bIsLoadScene = false;
			}
		
		default:
			break;
		}
	}//

}


// �����û�����
void CGamePro_CharSel::SetUserName(const char* pUserName)
{
	if(pUserName)
	{
		m_strUserName = pUserName;
	}
}

// �л����ʺ��������
void CGamePro_CharSel::ChangeToAccountInput()
{
	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// ���ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// �ر�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// �л�����¼����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	if(CGameProcedure::s_pProcLogIn)
	{
		CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING);
	}

	m_iCharacterCount = 0;
	//ж�س���,ɾ������ѡ��ģ��
	//s_pGfxSystem->Scene_UnLoad();
	// �Ͽ���������
	CNetManager::GetMe()->Close();
}

// �л��������������
void CGamePro_CharSel::ChangeToCreateRole()
{
	// �����ﴴ������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_CREATE_CHARACTOR);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �ر�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// �л������ﴴ������
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);

	if(CGameProcedure::s_pProcCharCreate)
	{
		CGameProcedure::s_pProcCharCreate->CallInit();
	}
}


// ���һ������
int CGamePro_CharSel::AddCharacter( const DB_CHAR_BASE_INFO& CharacterInfo )
{
	if( m_iCharacterCount < MAX_SHOW_IN_UI )
	{
		memcpy( m_CharacterInfo + m_iCharacterCount, &CharacterInfo, sizeof(DB_CHAR_BASE_INFO) );
		char szName[64]  = {0};
		sprintf(szName, "CharSel_Model_%d", m_iCharacterCount+1);
		m_Character[m_iCharacterCount].CreateNewUIModel(szName);

		LPCTSTR	cName		= m_CharacterInfo[m_iCharacterCount].m_Name;
		int		iRaceId		= m_CharacterInfo[m_iCharacterCount].m_Sex;
		int		iFaceMeshId = m_CharacterInfo[m_iCharacterCount].m_FaceModel;
		int		iFaceHairId = m_CharacterInfo[m_iCharacterCount].m_HairModel;
		UINT	iHairColor	= m_CharacterInfo[m_iCharacterCount].m_HairColor;
		UINT	iCountry	= m_CharacterInfo[m_iCharacterCount].m_Country;		// ����
		UINT	iMenpai		= m_CharacterInfo[m_iCharacterCount].m_Menpai;		// ְҵ
//		GUID_t	guid		= m_CharacterInfo[m_iCharacterCount].m_GUID;


		// ����
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_Name( cName );
		// ְҵ
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_MenPai( iMenpai );
		// �����Ա�
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_RaceID( iRaceId );
		// ��������ģ��
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_FaceMesh(iFaceMeshId);
		// ���÷��͵�ģ��.
//		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairMesh(iFaceHairId);
		// ͷ����ɫ
//		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairColor(iHairColor);
		// ����
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_Country(iCountry);

		// ����װ��.
		const DB_CHAR_EQUIP_LIST& equipInfo = m_CharacterInfo[m_iCharacterCount].m_EquipList;
		for( int iEquipPoint = HEQUIP_MAINHAND; iEquipPoint < HEQUIP_NUMBER - 1; iEquipPoint++ )
		{
			if( 0 != equipInfo.m_Equip[iEquipPoint] )
			{
				m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_Equip( (HUMAN_EQUIP)iEquipPoint, equipInfo.m_Equip[iEquipPoint] );
			}
		}

		// ģ�ͼ���
		m_iCharacterCount++;

		return 0;
	}
	else
	{
		return -1;
	}

}

/*
VOID UpdateUIModel( INT index )
{
	if( index >= m_iCharacterCount || index < 0 )
		return ;

	INT iRaceId = m_CharacterInfo[m_iCharacterCount].m_Sex;
}
*/


// �õ���ǰ��ɫ�ĸ���
int CGamePro_CharSel::GetCurRoleCount()
{
	return m_iCharacterCount;
}

// ͨ�������õ���ɫ����Ϣ
const DB_CHAR_BASE_INFO* CGamePro_CharSel::GetRoleInfo( int iIndex )
{
	if( (iIndex < MAX_SHOW_IN_UI) && (iIndex >= 0) )
	{
		return ( m_CharacterInfo + iIndex );
	}

	return NULL;
}


// ͨ�������õ���ɫ��UI ģ������
const TCHAR* CGamePro_CharSel::GetRoleUIName( int iIndex )
{
	if((iIndex < MAX_SHOW_IN_UI)&&(iIndex >= 0))
	{
		return m_Character[iIndex].GetUIModelName();
	}

	return NULL;
}


// ɾ��һ����ɫ
int CGamePro_CharSel::DelRole(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			CLAskDeleteChar msg;

			msg.SetCharGuid( m_CharacterInfo[iRoleIndex].m_GUID );

			// ɾ����ɫ
			CNetManager::GetMe()->SendPacket(&msg);

			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("produce_char_sel_Info_Deling_Role");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());	
			//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	

			return 0;
		}
	}


	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_char_sel_Info_Please_Sel_Role");
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	

	return 1;
}


// ������Ϸ
int CGamePro_CharSel::SendEnterGameMsg(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{

			// ����ѡ���ɫ����Ϣ�� ��½�� GameServer ʱҪʹ��
			const CHAR* szSystemCfg = "./Game.cfg";
			CHAR szTemp[MAX_PATH];	// 260

			UINT dwFileAttr = GetFileAttributes(szSystemCfg);
			if(FILE_ATTRIBUTE_READONLY & dwFileAttr)
				SetFileAttributes(szSystemCfg, dwFileAttr&(~FILE_ATTRIBUTE_READONLY));

			LPCTSTR	cName	= m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_Name();
			INT		iRaceId = m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_RaceID();
			INT		iMenpai	= m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_MenPai();
			INT		iCountry= m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_Country();

			::WritePrivateProfileString("Variable", "User_NAME", cName, szSystemCfg);				// ����
//			_snprintf(szTemp, MAX_PATH, "%u", iFaceMeshId);
//			::WritePrivateProfileString("Variable", "User_FACE_MODEL", szTemp, szSystemCfg);		// ����ģ��
			_snprintf(szTemp, MAX_PATH, "%u", iRaceId);
			::WritePrivateProfileString("Variable", "User_GENDER", szTemp, szSystemCfg);			// �Ա�
			_snprintf(szTemp, MAX_PATH, "%u", iMenpai);
			::WritePrivateProfileString("Variable", "User_MENPAI", szTemp, szSystemCfg);			// ��ɫְҵ
			_snprintf(szTemp, MAX_PATH, "%u", iCountry);
			::WritePrivateProfileString("Variable", "User_COUNTRY", szTemp, szSystemCfg);			// ����

			if( FILE_ATTRIBUTE_READONLY & dwFileAttr )
				SetFileAttributes(szSystemCfg, dwFileAttr);



			CLAskCharLogin msg;

			m_EnterGameGUID = m_CharacterInfo[iRoleIndex].m_GUID;
			msg.SetCharGuid(m_EnterGameGUID);

			// ɾ����ɫ
			CNetManager::GetMe()->SendPacket(&msg);

			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("produce_char_sel_Info_Readying_To_Enter_Game");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());	
			//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	

			return 0;
		}
	}

	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_char_sel_Info_Please_Sel_Role");
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	

	return 1;
}


// �л�����������������
void CGamePro_CharSel::ChangeToServerConnect()
{
	// �л�������game server����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);

	// ����game server����Ϊ�Ͽ�״̬
	CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);


}

// λ���Ƿ�Ϸ�
bool CGamePro_CharSel::IsValidIndex(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			return true;
		}
	}

	return false;
}

// ɾ��ѡ��Ľ�ɫ
int CGamePro_CharSel::DelSelRole()
{
	if( (m_iCurSelRoleDel < m_iCharacterCount) && (m_iCurSelRoleDel >= 0) )
	{
		if( m_iCurSelRoleDel < MAX_SHOW_IN_UI )
		{
			// ͨ�� GUID ɾ����ɫ
			CLAskDeleteChar msg;
			msg.SetCharGuid( m_CharacterInfo[m_iCurSelRoleDel].m_GUID );
			CNetManager::GetMe()->SendPacket(&msg);

			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("produce_char_sel_Info_Deling_Role");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());

			return 0;
		}
	}

	STRING strTemp = "";
	strTemp = NOCOLORMSGFUNC("produce_char_sel_Info_Please_Sel_Role");
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());

	return 1;
}

void CGamePro_CharSel::SetCurSelDel(int iIndex)
{
	m_iCurSelRoleDel = -1;
}

void CGamePro_CharSel::ClearUIModel()
{
	for(int i = 0; i < MAX_SHOW_IN_UI; i++)
	{
		m_Character[i].DestroyUIModel();
	}

	m_iCharacterCount = 0;
}

