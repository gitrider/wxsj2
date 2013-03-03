
/**	GamePro_CharSel.cpp
 *	
 *	功能：	处理登陆过程中的选择角色
 *			通过它联系到三个部分：角色的创建、进入游戏、返回到帐号输入界面
 *
 *	修改记录：
 *			080220	在进入游戏的事件 SendEnterGameMsg 中， 加入保存选择的角色的信息， 留给连接到 GameServer 时用
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
	case CHARSEL_CONNNECT_GAME_SERVER_READY:// 与游戏服务器的连接准备好, 可以连接.
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
	////按键 'A'
	//if(s_pInputSystem->IsKeyPress(KC_A))
	//{
	//	m_Status = CHARSEL_SELDONE;
	//}

	////-------------------------------------------------------
	////按键 '1' 创建角色流程
	//if(s_pInputSystem->IsKeyPress(KC_1))
	//{
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);
	//}

	////-------------------------------------------------------
	////按键 '2' 进入游戏流程.
	//if(s_pInputSystem->IsKeyPress(KC_2))
	//{
	//	m_bIsLoadScene = false;
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	//}//

	//分析事件队列
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


// 设置用户名字
void CGamePro_CharSel::SetUserName(const char* pUserName)
{
	if(pUserName)
	{
		m_strUserName = pUserName;
	}
}

// 切换到帐号输入界面
void CGamePro_CharSel::ChangeToAccountInput()
{
	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 打开帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// 关闭人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// 切换到登录界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	if(CGameProcedure::s_pProcLogIn)
	{
		CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING);
	}

	m_iCharacterCount = 0;
	//卸载场景,删除人物选择模型
	//s_pGfxSystem->Scene_UnLoad();
	// 断开网络连接
	CNetManager::GetMe()->Close();
}

// 切换到创建人物界面
void CGamePro_CharSel::ChangeToCreateRole()
{
	// 打开人物创建界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_CREATE_CHARACTOR);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// 切换到人物创建界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);

	if(CGameProcedure::s_pProcCharCreate)
	{
		CGameProcedure::s_pProcCharCreate->CallInit();
	}
}


// 添加一个人物
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
		UINT	iCountry	= m_CharacterInfo[m_iCharacterCount].m_Country;		// 国家
		UINT	iMenpai		= m_CharacterInfo[m_iCharacterCount].m_Menpai;		// 职业
//		GUID_t	guid		= m_CharacterInfo[m_iCharacterCount].m_GUID;


		// 名称
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_Name( cName );
		// 职业
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_MenPai( iMenpai );
		// 设置性别
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_RaceID( iRaceId );
		// 设置脸的模型
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_FaceMesh(iFaceMeshId);
		// 设置发型的模型.
//		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairMesh(iFaceHairId);
		// 头发颜色
//		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairColor(iHairColor);
		// 国家
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_Country(iCountry);

		// 设置装备.
		const DB_CHAR_EQUIP_LIST& equipInfo = m_CharacterInfo[m_iCharacterCount].m_EquipList;
		for( int iEquipPoint = HEQUIP_MAINHAND; iEquipPoint < HEQUIP_NUMBER - 1; iEquipPoint++ )
		{
			if( 0 != equipInfo.m_Equip[iEquipPoint] )
			{
				m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_Equip( (HUMAN_EQUIP)iEquipPoint, equipInfo.m_Equip[iEquipPoint] );
			}
		}

		// 模型记数
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


// 得到当前角色的个数
int CGamePro_CharSel::GetCurRoleCount()
{
	return m_iCharacterCount;
}

// 通过索引得到角色的信息
const DB_CHAR_BASE_INFO* CGamePro_CharSel::GetRoleInfo( int iIndex )
{
	if( (iIndex < MAX_SHOW_IN_UI) && (iIndex >= 0) )
	{
		return ( m_CharacterInfo + iIndex );
	}

	return NULL;
}


// 通过索引得到角色的UI 模型名字
const TCHAR* CGamePro_CharSel::GetRoleUIName( int iIndex )
{
	if((iIndex < MAX_SHOW_IN_UI)&&(iIndex >= 0))
	{
		return m_Character[iIndex].GetUIModelName();
	}

	return NULL;
}


// 删除一个角色
int CGamePro_CharSel::DelRole(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			CLAskDeleteChar msg;

			msg.SetCharGuid( m_CharacterInfo[iRoleIndex].m_GUID );

			// 删除角色
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


// 进入游戏
int CGamePro_CharSel::SendEnterGameMsg(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{

			// 保存选择角色的信息， 登陆到 GameServer 时要使用
			const CHAR* szSystemCfg = "./Game.cfg";
			CHAR szTemp[MAX_PATH];	// 260

			UINT dwFileAttr = GetFileAttributes(szSystemCfg);
			if(FILE_ATTRIBUTE_READONLY & dwFileAttr)
				SetFileAttributes(szSystemCfg, dwFileAttr&(~FILE_ATTRIBUTE_READONLY));

			LPCTSTR	cName	= m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_Name();
			INT		iRaceId = m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_RaceID();
			INT		iMenpai	= m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_MenPai();
			INT		iCountry= m_Character[iRoleIndex].m_pAvatar->GetCharacterData()->Get_Country();

			::WritePrivateProfileString("Variable", "User_NAME", cName, szSystemCfg);				// 名称
//			_snprintf(szTemp, MAX_PATH, "%u", iFaceMeshId);
//			::WritePrivateProfileString("Variable", "User_FACE_MODEL", szTemp, szSystemCfg);		// 脸形模型
			_snprintf(szTemp, MAX_PATH, "%u", iRaceId);
			::WritePrivateProfileString("Variable", "User_GENDER", szTemp, szSystemCfg);			// 性别
			_snprintf(szTemp, MAX_PATH, "%u", iMenpai);
			::WritePrivateProfileString("Variable", "User_MENPAI", szTemp, szSystemCfg);			// 角色职业
			_snprintf(szTemp, MAX_PATH, "%u", iCountry);
			::WritePrivateProfileString("Variable", "User_COUNTRY", szTemp, szSystemCfg);			// 国家

			if( FILE_ATTRIBUTE_READONLY & dwFileAttr )
				SetFileAttributes(szSystemCfg, dwFileAttr);



			CLAskCharLogin msg;

			m_EnterGameGUID = m_CharacterInfo[iRoleIndex].m_GUID;
			msg.SetCharGuid(m_EnterGameGUID);

			// 删除角色
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


// 切换到服务器连接流程
void CGamePro_CharSel::ChangeToServerConnect()
{
	// 切换到连接game server流程
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);

	// 设置game server流程为断开状态
	CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);


}

// 位置是否合法
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

// 删除选择的角色
int CGamePro_CharSel::DelSelRole()
{
	if( (m_iCurSelRoleDel < m_iCharacterCount) && (m_iCurSelRoleDel >= 0) )
	{
		if( m_iCurSelRoleDel < MAX_SHOW_IN_UI )
		{
			// 通过 GUID 删除角色
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

