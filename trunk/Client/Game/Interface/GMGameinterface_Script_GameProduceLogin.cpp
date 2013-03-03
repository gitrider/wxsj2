
#include "stdafx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "Procedure/GamePro_Enter.h"
#include "Procedure/GamePro_CharSel.h"
#include "Procedure/GamePro_Login.h"
#include "Procedure/GameProcedure.h"
#include "Procedure/GamePro_CharCreate.h"

#include "NetWork/NetManager.h"
#include "LuaPlus.h"
#include "Global.h"
#include "Event/GMEventSystem.h"
#include "GIException.h"
#include "Action/GMActionSystem.h"
#include "DataPool/GMUIDataPool.h"
#include "DataPool/GMDataPool.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Input/GMInputSystem.h"
#include "world/worldmanager.h"
#include "object/Logic/character/obj_playermyself.h"
#include "ScriptCommandStruct.h"

#include "..\..\Common\database\WXSJ_DBC.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "GIDBC_Struct.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Sound\GMSoundSystem.h"

//#include "GameStruct_MapInfo.h"
#include "CGMapInfo.h"



namespace SCRIPT_SANDBOX
{

	// ���徲̬��Ա����.
	CGameProduce_Login CGameProduce_Login::s_GameProduceLogin;

	// ��̬lua������������.
	LuaPlus::LuaObject* CGameProduce_Login::s_pMetaTable = NULL;

	CGameProduce_Login::CGameProduce_Login()
	{

	}

	CGameProduce_Login::~CGameProduce_Login()
	{

	}

	//�õ�����������ĸ���.
	INT CGameProduce_Login::GetAreaCount(LuaPlus::LuaState* state)
	{

		int iAreaCount = 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			iAreaCount = CGameProcedure::s_pProcLogIn->GetAreaCount();
		}
		
		state->PushInteger(iAreaCount);
		return 1;
		
	}
	
	//�õ����������������.
	INT CGameProduce_Login::GetAreaName(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		PAREA_INFO pAreaInfo = NULL;
		if(CGameProcedure::s_pProcLogIn)
		{
			pAreaInfo = CGameProcedure::s_pProcLogIn->GetAreaInfo(nAreaIndex);
		}
		if (pAreaInfo)
		{
			state->PushString(pAreaInfo->szAreaName);
			state->PushInteger(pAreaInfo->iProvide);
		}
		else
		{
			state->PushString("");
			state->PushInteger(-1);
		}
		
		return 2;
	}

	//�õ��������������Ϣ
	INT CGameProduce_Login::GetAreaServerInfo(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		std::string strInfoName = "";
		int iAreaProvide		= 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			PAREA_INFO pInfo = CGameProcedure::s_pProcLogIn->GetAreaInfo(nAreaIndex);
			if(pInfo)
			{
				strInfoName  = pInfo->szAreaName;
				iAreaProvide = pInfo->iProvide;
			}
		}
	
		state->PushString(strInfoName.c_str());
		state->PushInteger(iAreaProvide);
		return 2;
	}


	//�õ������������, login server �ĸ���.
	INT CGameProduce_Login::GetAreaLoginServerCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		int iLoginServerCount = 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			iLoginServerCount = CGameProcedure::s_pProcLogIn->GetAreaLoginServerCount(nAreaIndex);
		}
	
		state->PushInteger(iLoginServerCount);
		
		return 1;
	}


	// �õ�login server ����Ϣ.
	INT CGameProduce_Login::GetAreaLoginServerInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		PLOGINSERVER_INOF pLoginServerInfo = NULL;
		if(CGameProcedure::s_pProcLogIn)
		{
			pLoginServerInfo = CGameProcedure::s_pProcLogIn->GetAreaLoginServerInfo(nAreaIndex, nLoginServerIndex);
		
		}
	
		if(pLoginServerInfo)
		{
			// �õ�����������
			state->PushString(pLoginServerInfo->szLoginServerName);

			// �õ�������״̬
			state->PushInteger(pLoginServerInfo->iLoginServerStatus);
		}
		else
		{
			state->PushString("");
			state->PushInteger(0);
		}
	
		return 2;

	}

	// ���ӵ�login server
	INT CGameProduce_Login::ConnectToLoginServer(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->ConnectToLoginServer(nAreaIndex, nLoginServerIndex);
		
		}
		return 0;
	}


	// �˵�������ѡ�����
	INT CGameProduce_Login::ExitToSelectServer(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->OpenSelectServerDlg();
		
		}
		return 0;
	}


	// ��֤�û���������
	INT CGameProduce_Login::CheckAccount(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsString()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		const char* pUserName = args[2].GetString();
		const char* pPassword = args[3].GetString();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->CheckAccount((CHAR*)pUserName, (CHAR*)pPassword);
		
		}
		return 0;
	}


	// ѡ��һ��������
	INT CGameProduce_Login::SelectLoginServer(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		if(CGameProcedure::s_pProcLogIn)
		{
			CGameProcedure::s_pProcLogIn->SelectLoginServer(nAreaIndex, nLoginServerIndex);
		}

		return 0;
	}

	// ������ѡ������л����ʺ��������
	INT CGameProduce_Login::ChangeToAccountInputDlgFromSelectRole(LuaPlus::LuaState* state)
	{
		
		if(CGameProcedure::s_pProcCharSel)
		{
			
			CGameProcedure::s_pProcCharSel->ChangeToAccountInput();
		
		}
		return 0;
	}

	// ������ѡ������л������ﴴ������
	INT CGameProduce_Login::ChangeToCreateRoleDlgFromSelectRole(LuaPlus::LuaState* state)
	{
		
		if(CGameProcedure::s_pProcCharSel)
		{
			if(CGameProcedure::s_pProcCharSel->GetCurRoleCount() >= 3)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("LCRetCreateCharHandler_Info_Role_Is_Full");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			CGameProcedure::s_pProcCharSel->ChangeToCreateRole();
		}
		return 0;
	}

	// �����ﴴ�������л�������ѡ�����
	INT CGameProduce_Login::ChangeToSelectRoleDlgFromCreateRole(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcCharCreate)
		{
			
			CGameProcedure::s_pProcCharCreate->ChangeToSelectRole();
		}//
		return 0;
	}

	// �õ���ɫ�ĸ���
	INT CGameProduce_Login::GetRoleCount(LuaPlus::LuaState* state)
	{
		LuaStack args( state );

		int iRoleCount = 0;
		if( CGameProcedure::s_pProcCharSel )
		{
			iRoleCount = CGameProcedure::s_pProcCharSel->GetCurRoleCount();
		}//	

		state->PushInteger( iRoleCount );

		return 1;
	}

	// �õ���ɫ����Ϣ
	INT CGameProduce_Login::GetRoleInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		const DB_CHAR_BASE_INFO* pInfo = NULL;
		if( CGameProcedure::s_pProcCharSel )
		{
			pInfo = CGameProcedure::s_pProcCharSel->GetRoleInfo(iRoleIndex);
			if( pInfo )
			{
				// ����
				state->PushString(pInfo->m_Name);
				// ���� Nick 2007.11.21
				// ���º���
				// ���ʺ���
				// ���򺹹�
				// ��Į����
				switch( pInfo->m_Country )
				{
				case 0:
					state->PushString("���º���");
					break;
				case 1:
					state->PushString("���ʺ���");
					break;
				case 2:
					state->PushString("���򺹹�");
					break;
				case 3:
					state->PushString("��Į����");
					break;
				}

				// ����
				state->PushInteger(pInfo->m_Menpai);
				// �ȼ�
				state->PushInteger(pInfo->m_Level);

				// ���� Nick 2007.11.21
				return 4;
			}

		}//	
		
		// ����
		state->PushString("");
		// ���� Nick 2007.11.21
		state->PushString("");	
		// ����
		state->PushInteger(0);
		// �ȼ�
		state->PushInteger(0);

		// ���� Nick 2007.11.21
		return 4;
	}


	// �õ���ɫ��uiģ������
	INT CGameProduce_Login::GetRoleUIModleName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		
		const TCHAR* pUIModleName = NULL;
		if(CGameProcedure::s_pProcCharSel)
		{
			pUIModleName = CGameProcedure::s_pProcCharSel->GetRoleUIName(iRoleIndex);
		}//	
		
		if(pUIModleName)
		{
			// ����
			state->PushString(pUIModleName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// ������ɫ
	INT CGameProduce_Login::CreateRole(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}
		if (!(args[4].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");

		}

		if (!(args[5].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");

		}

		if (!(args[6].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");

		}

		// �߻�����ȥ������ Nick 2007.11.24
		//if (!(args[7].IsInteger()))
		//{
		//	KLThrow("LUA:GetBuffIconNameByIndex param parameter error");

		//}


		if(CGameProcedure::s_pProcCharCreate)
		{
			const char* pChar = args[2].GetString();
			int iSex = args[3].GetInteger();
			int iCountry = args[4].GetInteger();
			int iProfression =args[5].GetInteger();
			int iFaceModelIndex = args[6].GetInteger();
			int iHairModelIndex = args[7].GetInteger();
			if(pChar)
			{
				int iStrLen = (int)::strlen(pChar);

				if(iStrLen > 12)
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("Login_Create_Role_Info_Name_Too_Len");
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
					return 0;
				}

				CGameProcedure::s_pProcCharCreate->m_iSex = iSex;
				CGameProcedure::s_pProcCharCreate->m_iCountry1 =iCountry;
		     	CGameProcedure::s_pProcCharCreate->m_iProfession1 = iProfression;
				CGameProcedure::s_pProcCharCreate->m_iFaceModle1 = iFaceModelIndex;
				//�߻�����ȥ������ Nick 2007.11.24
				CGameProcedure::s_pProcCharCreate->m_iHairModle1 = 0;	// iHairModelIndex;
				strcpy(CGameProcedure::s_pProcCharCreate->m_szCreateRoleName, pChar);
				CGameProcedure::s_pProcCharCreate->CreateRole();
			}
			else
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("Login_Create_Role_Info_Name_Is_Empty");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//strcpy(CGameProcedure::s_pProcCharCreate->m_szCreateRoleName, "");
				//CGameProcedure::s_pProcCharCreate->CreateRole();
			}
		}//	
		

		return 0;
	}

	// ɾ����ɫ
	INT CGameProduce_Login::DelRole(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if(CGameProcedure::s_pProcCharSel)
		{
			int iRoleIndex = args[2].GetInteger();
			CGameProcedure::s_pProcCharSel->DelRole(iRoleIndex);

			
		}//	

		return 0;
		
	}

	// ɾ��ѡ��Ľ�ɫ
	INT CGameProduce_Login::DelSelRole(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcCharSel)
		{

			CGameProcedure::s_pProcCharSel->DelSelRole();
			
		}//	

		return 0;
		
	}

	// ���ͽ�����Ϸ��Ϣ
	INT CGameProduce_Login::ChangeSceneMsg(LuaPlus::LuaState* state)
	{
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if(CGameProcedure::s_pProcEnter)
		{
			INT iSceneId = args[2].GetInteger();
			if( CGameProcedure::s_pProcEnter->GetSceneID() == iSceneId )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���Ѿ��ڸõ�ͼ��");
				return 0;
			}

			CGMapInfo msg;
			//msg.SetType( ASK_CHANGE_SCENE );
			msg.SetIndex( iSceneId );
			CNetManager::GetMe()->SendPacket( &msg );
			CGameProcedure::s_pEventSystem->PushEvent( GE_TOGLE_CLOSELARGEMAP );
		}

		return 0;
	}

	// ���ͽ�����Ϸ��Ϣ
	INT CGameProduce_Login::SendEnterGameMsg( LuaPlus::LuaState* state)
	{
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if(CGameProcedure::s_pProcCharSel)
		{
			int iRoleIndex = args[2].GetInteger();
			CGameProcedure::s_pProcCharSel->SendEnterGameMsg(iRoleIndex);
		}//	

		return 0;
	}

	// ѯ���Ƿ�ɾ����ɫ
	INT CGameProduce_Login::DelRole_YesNo(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		CGameProcedure::s_pProcCharSel->m_iCurSelRoleDel = iRoleIndex;
		std::vector< STRING > strParamInfo;

		if(CGameProcedure::s_pProcCharSel->IsValidIndex(iRoleIndex))
		{
			// 
			std::string strInfo = ""; 
			// ͨ�������õ���ɫ����Ϣ
			const DB_CHAR_BASE_INFO* pInfo = CGameProcedure::s_pProcCharSel->GetRoleInfo(iRoleIndex);
			if(pInfo)
			{
				//if(pInfo->m_Level < 9)
				{
					TCHAR szBuf[128];
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_GameLogin_Info_Del_Role_YesNo");
					_stprintf(szBuf, strTemp.c_str(), pInfo->m_Level, pInfo->m_Name);
					//_stprintf(szBuf, _T("��ȷ��Ҫ�� %d�� �Ľ�ɫ#c00ff00 %s #cffffffɾ����?"), pInfo->m_Level, pInfo->m_Name);
					strParamInfo.push_back(szBuf);
					strParamInfo.push_back("1");

					((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
				}
				//else
				//{
				//	STRING strTemp = "";
				//	strTemp = NOCOLORMSGFUNC("CGameProduce_Login_Can_Not_Del_Level_Over_9");
				//	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//}//
			}
			else
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_GameLogin_Info_Sel_Del_Role");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ѡ��һ��Ҫɾ���Ľ�ɫ.");
			}//
		}
		else
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_GameLogin_Info_Sel_Del_Role");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ѡ��һ��Ҫɾ���Ľ�ɫ.");
		}
		
		return 0;
	}

	// ѯ���Ƿ��˳���Ϸ
	INT CGameProduce_Login::ExitGame_YesNo(LuaPlus::LuaState* state)
	{
		std::vector< STRING > strParamInfo;

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_GameLogin_Info_Exit_Game_YesNo");
		strParamInfo.push_back(strTemp.c_str());
		//strParamInfo.push_back("ȷ��Ҫ�˳���Ϸ��?");
		strParamInfo.push_back("0");
		// 
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
		return 0;
	}

	// ѯ���Ƿ��˵��ʺ��������
	INT CGameProduce_Login::ExitToAccountInput_YesNo(LuaPlus::LuaState* state)
	{
		std::vector< STRING > strParamInfo;
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_GameLogin_Info_Exit_To_Account_Input_YesNo");
		strParamInfo.push_back(strTemp.c_str());

		//strParamInfo.push_back("ȷ���˵��ʺ����������?");
		strParamInfo.push_back("2");
		// 
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
		return 0;
	}

	// �Զ�ѡ��һ���뵱ǰѡ������繩Ӧ��ƥ���login server
	INT CGameProduce_Login::AutoSelLoginServer(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iIndex = args[2].GetInteger();
		CGameProcedure::s_pProcLogIn->AutoSelLoginServer(iIndex);
   		return 0;
	}

	
	// �õ�Ů����ͷ��ĸ���
	INT CGameProduce_Login::GetWomanFaceCount(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		
		int iFaceCount = 0;

		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			iFaceCount = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetWomanFaceCount();
		}
			
		// Ů�������εĸ���
		state->PushInteger(iFaceCount);
		return 1;
	}

	// �õ�������ͷ��ĸ���
	INT CGameProduce_Login::GetManFaceCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		
		int iFaceCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			iFaceCount = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetManFaceCount();
		}

		// ���������εĸ���
		state->PushInteger(iFaceCount);
		return 1;
	}

	// �õ�Ů����ͷ�������
	INT CGameProduce_Login::GetWomanFaceName(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iIndex = args[2].GetInteger();
		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			LPCSTR pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(0, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// �õ�������ͷ�������
	INT CGameProduce_Login::GetManFaceName(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iIndex = args[2].GetInteger();
		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			LPCSTR pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(1, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// �õ�����ͷ�������
	INT CGameProduce_Login::GetFaceName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(iRace, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// ����ͷ��id
	INT CGameProduce_Login::SetFaceId(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			CGameProcedure::s_pProcCharCreate->SetFaceByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �������β���.
	//
	
	// �õ�����model�ĸ���
	INT CGameProduce_Login::GetFaceModelCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// �Ա�
		int iRace  = args[2].GetInteger();

		int iModelCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			iModelCount = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModelCount(iRace);
		}

		state->PushInteger(iModelCount);
		return 1;
	}

	// �õ�����ģ�͵�����
	INT CGameProduce_Login::GetFaceModelName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			//pImageName = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModel(iRace, iIndex);
			//  
			pImageName = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModelShowName(iRace, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}
		return 1;
	}

	// ��������ģ��id
	INT CGameProduce_Login::SetFaceModelId(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		if( (CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel) )
		{
			CGameProcedure::s_pProcCharCreate->SetFaceModelByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}

	// ����ĳ������
	INT CGameProduce_Login::PlayAnimation(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:CGameProduce_Login::PlayAnimationparam parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:CGameProduce_Login::PlayAnimation parameter error");
		}

		// ͷ������
		int iGender  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		if( (CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel) )
		{
			CGameProcedure::s_pProcCharCreate->PlayAnimation(iGender, iIndex);
		}

		return 0;
	}
	


	INT CGameProduce_Login::SetGender( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow( "LUA: SetGender Parameter Error" );
		}

		int iGender  = args[2].GetInteger();

		if( CGameProcedure::s_pProcCharCreate )
		{
			CGameProcedure::s_pProcCharCreate->SetGender( iGender );
		}

		return 0;
	}
	
	INT CGameProduce_Login::SetProfession( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow( "LUA: SetProfession Parameter Error" );
		}

		int iProfession  = args[2].GetInteger();

		if( CGameProcedure::s_pProcCharCreate )
		{
			CGameProcedure::s_pProcCharCreate->SetProfession( iProfession );
		}

		return 0;
	}



	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���ڷ��β���.
	//
	
	// �õ�����model�ĸ���
	INT CGameProduce_Login::GetHairModelCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// �Ա�
		int iRace  = args[2].GetInteger();

		int iModelCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadHairModel))
		{
			iModelCount = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModelCount(iRace);
		}

		state->PushInteger(iModelCount);
		return 1;
	}

	// �õ�����ģ�͵�����
	INT CGameProduce_Login::GetHairModelName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadHairModel))
		{
			//pImageName = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModel(iRace, iIndex);
			// 
			pImageName = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModelShowName(iRace, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// ��������ģ��id
	INT CGameProduce_Login::SetHairModelId(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			CGameProcedure::s_pProcCharCreate->SetHairModelByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}

	// �ر��������ӡ� 
	INT CGameProduce_Login::CloseNetConnect(LuaPlus::LuaState* state)
	{

		CNetManager::GetMe()->Close();

		if(CGameProcedure::s_pProcCharSel)
		{
			CGameProcedure::s_pProcCharSel->ChangeToAccountInput();
		}
		return 0;
	}
 
	INT CGameProduce_Login::CreateRotateRole(LuaPlus::LuaState* state)
	{

		return 0;
	}

	INT CGameProduce_Login::ChangeSelectRole(LuaPlus::LuaState* state)
	{

		return 0;
	}

	INT CGameProduce_Login::SelRotateRole(LuaPlus::LuaState* state)
	{

		return 0;
	}

	INT CGameProduce_Login::GetLastLoginAreaAndServerIndex(LuaPlus::LuaState* state)
	{
		INT area =0, server = 0;
		STRING name;
		if((CGameProcedure::s_pProcLogIn))
		{
			CGameProcedure::s_pProcLogIn->GetLastLoginAreaAndServerIndex(area, server,name);
		}
		state->PushInteger(area);
		state->PushInteger(server);
		state->PushString(name.c_str());
		return 3;
	}
	

}