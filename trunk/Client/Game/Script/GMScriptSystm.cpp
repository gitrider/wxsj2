
/**	GMScriptSystem.cpp
 *	
 *	���ܣ�	UI��صĽӿ�
 *	
 *	�޸ļ�¼��
 *			080304	��Ӱ����ص�ע�� ע��
 *					��ӻ�ȡ���촰���Ҽ�ѡ�еĽ�ɫ����	GetContexMenuObjectName
 *					��� AdjustMemberTitle ���ĳ�Ա�ƺ�
 *			080305	DestoryFamily
 *					AgreeInvite
 *					ShowInputDialog
 *			080307	CreateFamily
 *					CreateFamilyConfirm
 *					DestoryFamily
 *					GetFamilyMembersInfo
 *					AdjustFamilyMember
 *					TransferMemberToFamily
 *					KickMemberFormFamily
 *					InviteMemberToFamily
 *					FamilyInviteConfirm
 *			080311	GetFamilyMemberIndex
 *					SetLuaFamilyNum
 *					GetLuaFamilyNum
 *			080314	Guild OpenMenu  �����б�Ĳ˵�
 *					FriendSearcher ���Ѳ�����صĽӿ�
 *			080317	Friend::ThrowToEnemyList
 *					Friend::OpenSearcher
 *			080318	FriendSearchByNameAndId
 *					ShowFriendInputDlg
 *			080320	GetItemPrice
 *					NpcShopSell
 *					NpcShopBuy
 *			080331	GetAgnameType
 *			080402	GetNewGroupInfo
 *					LuaShowFriendInfo
 *			080409  OpenEquipAll    // �򿪸���װ�� �������� ���˳ƺ�  һ����װ �������  �������� ����
 *			080415	Lua_PasswordGetData
 *					Lua_SavePasswordSetup
 *			080417	���ӹ�����ؽӿ�
 *			080418	CheckInputPassword
 *					SetInputPasswordState
 *					GetInputPasswordState
 *			080418	Lua_PasswordTypeGetState
 *			080421	CheckPasswdProcedure
 *					InputPasswdProcedure
 *					AppointFunctionary
 *					DismissFunctionary
 *			
 *			
 */					

#include "StdAfx.h"
#include "Global.h"
#include "GMScriptSystem.h"

#include "GIException.h"
#include "GITimeSystem.h"
#include "GIUtil.h"
#include "GIResourceProvider.h"

#include "Action/GMActionSystem.h"
#include "NetWork/NetManager.h"
#include "Variable/GMVariable.h"
#include "Procedure/Gameprocedure.h"
#include "Procedure/GameProcedure.h"

#include "Interface/GMGameInterface_Script.h"
#include "Interface/GMInterface_Script_Tooltip.h"
#include "Interface/GMGameInterface_Script_Package.h"
#include "Interface/GMInterface_Script_Talk.h"
#include "Interface/GMInterface_Script_PetInviteFriend.h"
#include "Interface/GMInterface_Script_Guild.h"
#include "Interface/GMInterface_Script_SystemSetup.h"
#include "Interface/GMGameInterface_Script_Skill_Study.h"
#include "Interface/GMGameInterface_Script_Mount.h"
#include "Interface/GMInterface_Script_Country.h"
#include "Interface/GMGameInterface_Script_SouXia.h"
#include "../Timer.h"



#define FILE_SCRIPT		"Script.dat"


//=========================================================================

CScriptEnvironment::CScriptEnvironment(LPCTSTR szName, LPCTSTR szFileName)
{
	KLAssert(szName);

	//��������Table
	m_strName = szName;
	m_strFileName = szFileName;
	m_strNameInLua = m_strName+"_Env";
	m_theEnvObj = CScriptSystem::GetMe()->GetLuaState()->GetGlobals().CreateTable(m_strNameInLua.c_str());
}

CScriptEnvironment::~CScriptEnvironment()
{
}


INT CScriptEnvironment::DoFile(LPCTSTR szScriptFile)
{
	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource( szScriptFile, lpAddress, "General" );
	if( dwSize > 0 )
	{
 		INT result = CScriptSystem::GetMe()->GetLuaState()->DoString(lpAddress);
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
		return result;
	}
	return 0;
}

void CScriptEnvironment::DoString_WithCPPException(LPCTSTR szString)
{
	try
	{
		//����ɻ�����
		STRING strOldEnv = CScriptSystem::GetMe()->GetActiveEnvironment();
		//�趨����
		CScriptSystem::GetMe()->SetActiveEnvironment(m_strNameInLua.c_str());
		//ִ�к���
		CScriptSystem::GetMe()->GetLuaState()->DoString(szString);
		//�ָ�����
		if(!strOldEnv.empty())
		{
			CScriptSystem::GetMe()->SetActiveEnvironment(strOldEnv.c_str());
		}
	}
	catch(const std::exception& e)
	{
		static STRING strCPPException;
		strCPPException = e.what();
		
		//ת��Ϊ�ṹ���쳣
		LPVOID pException = &strCPPException;
		::RaiseException(
			0XE000C0DE, 
			EXCEPTION_NONCONTINUABLE,				// ���ɼ��������ش���
			1,										// 1������
			(CONST ULONG_PTR *)&pException);		// ָ��̬�쳣�ڴ�
	}
	catch(...)
	{
		throw;
	}
}

void CScriptEnvironment::DoString_WithGenException(LPCTSTR szString)
{
	CHAR szTitle[MAX_PATH];
	_snprintf(szTitle, MAX_PATH, "LUA: %s", m_strFileName.c_str());

	__try 
	{
		DoString_WithCPPException(szString);
	}
	__except (tProcessInnerException(GetExceptionInformation(), g_hMainWnd, szTitle), EXCEPTION_EXECUTE_HANDLER) {}
}

VOID CScriptEnvironment::DoString(LPCTSTR szString)
{
	//�����κ��쳣
	if(!CScriptSystem::GetMe()->IsEnableLuaCrash())
	{
		try{
		//����ɻ�����
		STRING strOldEnv = CScriptSystem::GetMe()->GetActiveEnvironment();
		//�趨����
		CScriptSystem::GetMe()->SetActiveEnvironment(m_strNameInLua.c_str());
		//ִ�к���
		CScriptSystem::GetMe()->GetLuaState()->DoString(szString);
		//�ָ�����
		if(!strOldEnv.empty())
		{
			CScriptSystem::GetMe()->SetActiveEnvironment(strOldEnv.c_str());
		}
		}catch(...) {}
		return;
	}

	//����ṹ���쳣��ִ��
	DoString_WithGenException(szString);
}

VOID CScriptEnvironment::DoFunction(LPCTSTR szFunction, LPCTSTR szParam)
{
	//if( strcmpi(szFunction, "SelectRole_GetRoleInfo") == 0 )
	//	MessageBox( 0, "SelectRole_GetRoleInfo", "Test", 0 );
	CHAR szDoFunction[MAX_PATH];
	_snprintf(szDoFunction, MAX_PATH, "%s(%s);", szFunction, szParam ? szParam : "");
	DoString(szDoFunction);
}

//=========================================================================
LuaPlus::LuaState* CScriptSystem::_GetLuaState(void)
{
	// ��֤���Ա���ʼ��
	static LuaPlus::LuaStateOwner s_theLuaState;
	return s_theLuaState.Get();
}



KL_IMPLEMENT_DYNAMIC(CScriptSystem, GETCLASS(tScriptSystem));
CScriptSystem* CScriptSystem::s_pMe = NULL;


CScriptSystem::CScriptSystem()
{
	s_pMe = this;
}

CScriptSystem::~CScriptSystem()
{
	KLAssert(m_listScriptEnv.empty());
}

tScriptEnvironment* CScriptSystem::NewEnvironment(LPCTSTR szName, LPCTSTR szFileName)
{
	CScriptEnvironment* pNewEnv = new CScriptEnvironment(szName, szFileName);
	m_listScriptEnv.push_back(pNewEnv);

	return (tScriptEnvironment*)pNewEnv;
}

VOID CScriptSystem::DestroyEnvironment(tScriptEnvironment* pEnv)
{
	std::list< CScriptEnvironment* >::iterator it;
	for(it=m_listScriptEnv.begin(); it!=m_listScriptEnv.end(); it++)
	{
		if((*it) == pEnv)
		{
			delete pEnv;
			m_listScriptEnv.erase(it);

			return;
		}
	}
}

const STRING& CScriptSystem::GetQuestFile(int nID)
{
	const static STRING strEmpty = "";

	std::map< int, STRING >::iterator itFind;

	itFind = m_mapQuestFile.find(nID);
	if(itFind == m_mapQuestFile.end())
		return strEmpty;

	return itFind->second;
}

VOID CScriptSystem::Initial(VOID*)
{
	// ��������ű��б�
	_loadQuestFile();

	// �Ƿ��׳��쳣
	BOOL bHave;
	m_bEnableLuaCrash = CGameProcedure::s_pVariableSystem->GetAs_Int("EnableLuaCrash", &bHave);
	if(!bHave)
		m_bEnableLuaCrash = TRUE;

	LuaPlus::LuaState* pState = _GetLuaState();
	LuaPlus::LuaObject objGlobal = _GetLuaState()->GetGlobals();

	//================
	// ע��ȫ�ֺ���
	//================

	// �ڵ�½������ȷ����Ĳ���
	objGlobal.Register( "ServerLogin",				SCRIPT_SANDBOX::Lua_ServerLogin);
	// Ҫ���˳�����
	objGlobal.Register( "QuitApplication",			SCRIPT_SANDBOX::Lua_QuitApplication);
	// Ҫ���/�رռ�����
	objGlobal.Register( "ToggleSkillBook",			SCRIPT_SANDBOX::Lua_ToggleSkillBook);
	// Ҫ��򿪼�����
	objGlobal.Register( "OpenSkillBook",			SCRIPT_SANDBOX::Lua_OpenSkillBook);
	// Ҫ���/�رճ������
	objGlobal.Register( "TogglePetPage",			SCRIPT_SANDBOX::Lua_TogglePetPage);
	// Ҫ�������ܽ���
	objGlobal.Register( "OpenLifePage",				SCRIPT_SANDBOX::Lua_OpenLifePage);
	// Ҫ�����ͨ���ܽ���
	objGlobal.Register( "OpenCommonSkillPage",		SCRIPT_SANDBOX::Lua_OpenCommonSkillPage);
	// �򿪰���
	objGlobal.Register( "ToggleContainer",			SCRIPT_SANDBOX::Lua_ToggleContainer);
	// ������
	objGlobal.Register( "ToggleMission",			SCRIPT_SANDBOX::Lua_ToggleMission);
	// �ر����ж����˵�
	objGlobal.Register( "CloseSecondMenu",			SCRIPT_SANDBOX::Lua_CloseSecondMenu);

	// �õ�ĳ������������Ŀ
	objGlobal.Register( "GetActionNum",				SCRIPT_SANDBOX::Lua_GetActionNum);
	// �õ���������
	objGlobal.Register( "EnumAction",				SCRIPT_SANDBOX::Lua_EnumAction);
	// �õ���������
	objGlobal.Register( "EnumActionForEquip",		SCRIPT_SANDBOX::Lua_EnumActionForEquip);
	// �Ӽ���id �õ�CActionItem
	objGlobal.Register( "GetAction_SkillID",		SCRIPT_SANDBOX::Lua_GetAction_SkillID);
	// ����Ʒid �õ�CActionItem
	objGlobal.Register( "GetAction_ItemID",		SCRIPT_SANDBOX::Lua_GetAction_ItemID);	
	// ��ʾ�Ҽ��˵�
	objGlobal.Register( "ShowContexMenu",			SCRIPT_SANDBOX::Lua_ShowContexMenu);
	// ���񴰿�ѡ���
	objGlobal.Register( "QuestFrameOptionClicked",	SCRIPT_SANDBOX::Lua_QuestFrameOptionClicked);
	// ��ʼ�ϳ�
	objGlobal.Register( "ComposeItem_Begin",		SCRIPT_SANDBOX::Lua_ComposeItem_Begin);
	// ȡ���ϳ�
	objGlobal.Register( "ComposeItem_Cancel",		SCRIPT_SANDBOX::Lua_ComposeItem_Cancel);
	// ��������
	objGlobal.Register( "QuestFrameAcceptClicked",	SCRIPT_SANDBOX::Lua_QuestFrameAcceptClicked);
	// л������
	objGlobal.Register( "QuestFrameRefuseClicked",	SCRIPT_SANDBOX::Lua_QuestFrameRefuseClicked);
	// ��������
	objGlobal.Register( "QuestFrameMissionContinue",SCRIPT_SANDBOX::Lua_QuestFrameMissionContinue);
	// �������
	objGlobal.Register( "QuestFrameMissionComplete",SCRIPT_SANDBOX::Lua_QuestFrameMissionComplete);
	// �������
	objGlobal.Register( "QuestFrameMissionAbnegate",SCRIPT_SANDBOX::Lua_QuestFrameMissionAbnegate);
	// �����ʾ��Ϣ
	objGlobal.Register( "PushDebugMessage",			SCRIPT_SANDBOX::Lua_PushDebugMessage);
	objGlobal.Register( "UpdateChangeSuit",			SCRIPT_SANDBOX::Lua_UpdateChangeSuit);

	objGlobal.Register( "Get_XParam_INT",			SCRIPT_SANDBOX::Lua_Get_XParam_INT);
	objGlobal.Register( "Get_XParam_STR",			SCRIPT_SANDBOX::Lua_Get_XParam_STR);
	objGlobal.Register( "Clear_XSCRIPT",			SCRIPT_SANDBOX::Lua_Clear_XSCRIPT);
	objGlobal.Register( "Send_XSCRIPT",				SCRIPT_SANDBOX::Lua_Send_XSCRIPT);
	objGlobal.Register( "Set_XSCRIPT_Parameter",	SCRIPT_SANDBOX::Lua_Set_XSCRIPT_Parameter);
	objGlobal.Register( "Set_XSCRIPT_Function_Name",SCRIPT_SANDBOX::Lua_Set_XSCRIPT_Function_Name);
	objGlobal.Register( "Set_XSCRIPT_ScriptID",		SCRIPT_SANDBOX::Lua_Set_XSCRIPT_ScriptID);
	objGlobal.Register( "Set_XSCRIPT_ParamCount",	SCRIPT_SANDBOX::Lua_Set_XSCRIPT_ParamCount);

	// �����Ƿ�Show��״̬
	objGlobal.Register( "IsWindowShow",				SCRIPT_SANDBOX::Lua_IsWindowShow);
	// �ر����д���
	objGlobal.Register( "CloseAllWindow",			SCRIPT_SANDBOX::Lua_CloseAllWindow );
	//// �򿪻�ر�ĳ������
	//objGlobal.Register( "CloseOneWindow",			SCRIPT_SANDBOX::Lua_CloseOneWindow );
	// ѧϰ�����ķ�
	objGlobal.Register( "SkillsStudyFrame_study",	SCRIPT_SANDBOX::lua_SkillsStudyFrame_study);
	// �����ķ�������Ҫ�Ľ�Ǯ
	objGlobal.Register( "GetUplevelXinfaSpendMoney",SCRIPT_SANDBOX::Lua_GetUplevelXinfaSpendMoney);
	// �����ķ�������Ҫ�ľ���ֵ
	objGlobal.Register( "GetUplevelXinfaSpendExp",	SCRIPT_SANDBOX::Lua_GetUplevelXinfaSpendExp);
	// ������ǰ�ķ��ȼ�
	objGlobal.Register( "GetXinfaLevel",			SCRIPT_SANDBOX::Lua_GetXinfaLevel);

	// ����UI�����
	objGlobal.Register( "SetUICamera",				SCRIPT_SANDBOX::Lua_Set_UICamera);

	objGlobal.Register( "SaveAutoAttributePoint",	SCRIPT_SANDBOX::Lua_SaveAutoAttributePoint);
	objGlobal.Register( "GetAutoAttributePoint",			SCRIPT_SANDBOX::Lua_GetAutoAttributePoint);

	//======$
//	objGlobal.Register( "ClickTest",				SCRIPT_SANDBOX::Lua_ClickTest);


	//********************************************************************************************
	// �ϳɽ���
	objGlobal.Register( "ToggleComposeWnd",			SCRIPT_SANDBOX::Lua_ToggleComposeWnd);
	// ö�����е��䷽
	objGlobal.Register( "EnumMethod",				SCRIPT_SANDBOX::Lua_EnumMethod);
	// �õ�ĳһ���䷽������
	objGlobal.Register( "GetMethodNumber",			SCRIPT_SANDBOX::Lua_GetMethodNumber);
	// ����������Ϣ
	objGlobal.Register( "SendChatMessage",			SCRIPT_SANDBOX::Lua_SendChatMessage);
	// ����gmָ��
	objGlobal.Register( "SendGMCommand",			SCRIPT_SANDBOX::Lua_SendGMCommand);
	// ������Ϣ���
	objGlobal.Register( "AxTrace",					SCRIPT_SANDBOX::Lua_AxTrace);
	
	// �ر�|�� ��ͼ
	objGlobal.Register( "ToggleLargeMap",			SCRIPT_SANDBOX::Lua_ToggleLargeMap);	// ���ͼ
	objGlobal.Register( "ToggleSceneMap",			SCRIPT_SANDBOX::Lua_ToggleSceneMap);	// ������ͼ
	objGlobal.Register( "UpdateMinimap",			SCRIPT_SANDBOX::Lua_UpdateMinimap);		// ����С��ͼ
	objGlobal.Register( "OpenMinimap",				SCRIPT_SANDBOX::Lua_OpenMinimap);		// ��С��ͼ

	//	�رյ�ͼ
	objGlobal.Register( "CloseSceneMap",			SCRIPT_SANDBOX::Lua_CloseSceneMap );	// �رճ�����ͼ
	objGlobal.Register( "CloseLargeMap",			SCRIPT_SANDBOX::Lua_CloseLargeMap );	// �رմ��ͼ

	// ���³�����ͼ�������ͼ�б�(_WX@7)
	objGlobal.Register( "UpdateLargeMapList",		SCRIPT_SANDBOX::Lua_UpdateLargeMapList );	//���������ͼ�е��б�
	objGlobal.Register( "UpdateSceneMapList",		SCRIPT_SANDBOX::Lua_UpdateSceneMapList );	//���³�����ͼ�е��б�
	
	objGlobal.Register( "LargeMapList",				SCRIPT_SANDBOX::Lua_LargeMapList );			//�������ͼ�е��б����������
	objGlobal.Register( "LargeMapListNum",			SCRIPT_SANDBOX::Lua_LargeMapListNum );	//�������ͼ�е��б����������

	objGlobal.Register( "SceneMapList",				SCRIPT_SANDBOX::Lua_SceneMapList );		//ö�ٳ����б��е�����
	objGlobal.Register( "SceneMapListNum",			SCRIPT_SANDBOX::Lua_SceneMapListNum );	//ö�ٳ����б��е�ITEM������


	// ��|�ر� ���˽��׽���
//	objGlobal.Register( "OpenBooth",				SCRIPT_SANDBOX::Lua_OpenBooth);
	objGlobal.Register( "CloseBooth",				SCRIPT_SANDBOX::Lua_CloseBooth);
	objGlobal.Register( "RepairAll",				SCRIPT_SANDBOX::Lua_RepairAll);			// ����ȫ��
	objGlobal.Register( "RepairOne",				SCRIPT_SANDBOX::Lua_RepairOne);			// ����һ��
	objGlobal.Register( "GetOperationType",			SCRIPT_SANDBOX::Lua_GetOperationType);	// ��������
	objGlobal.Register( "GetShopType",				SCRIPT_SANDBOX::Lua_GetShopType);		// ��������

	//���������״̬��ΪĬ��
	objGlobal.Register( "CancelRepairState",		SCRIPT_SANDBOX::Lua_CancelRepairState);			// ����һ��

	// ��ȡ��ǰ��������
	objGlobal.Register( "GetCurrentSceneName",		SCRIPT_SANDBOX::Lua_GetCurrentSceneName);
	// ��ȡ�����ߴ�
	objGlobal.Register( "GetSceneSize",				SCRIPT_SANDBOX::Lua_GetSceneSize);
	//	����ID��ó�������
	objGlobal.Register( "GetSceneNameFromID",				SCRIPT_SANDBOX::Lua_GetSceneNameFromID);
	// �õ���ǰ����id
	objGlobal.Register( "GetSceneID",				SCRIPT_SANDBOX::Lua_GetSceneID);
	// ���������
	objGlobal.Register( "ResetCamera",				SCRIPT_SANDBOX::Lua_ResetCamera);
	// ������������
	objGlobal.Register( "OpenCharacter",			SCRIPT_SANDBOX::Lua_OpenCharacter);
	// ��������
	objGlobal.Register( "AskLevelUp",				SCRIPT_SANDBOX::Lua_AskLevelUp);
	
	// ��װ����
	objGlobal.Register( "OpenEquip",				SCRIPT_SANDBOX::Lua_OpenEquip);

	// �򿪸���װ�� �������� ���˳ƺ�  һ����װ �������  ��������
	objGlobal.Register( "OpenEquipAll",				SCRIPT_SANDBOX::Lua_OpenEquipAll);

	//  �رո���װ�� �������� ���˳ƺ�  һ����װ �������  ��������
	objGlobal.Register( "CloseEquipAll",				SCRIPT_SANDBOX::Lua_CloseEquipAll);
	// ��װ������
//	objGlobal.Register( "OpenEquip",				SCRIPT_SANDBOX::Lua_OpenSelfEquip);

	// ����װ������ϸ��Ϣ
	objGlobal.Register( "AskEquipDetial",			SCRIPT_SANDBOX::Lua_AskEquipDetial);

	// ��|�ر� �ƺŽ���
	objGlobal.Register( "OpenTitleList",			SCRIPT_SANDBOX::Lua_OpenTitleList);
	objGlobal.Register( "CloseTitleList",			SCRIPT_SANDBOX::Lua_CloseTitleList);

	//��,�ر�,�л�����
	objGlobal.Register( "OpenWindow",			SCRIPT_SANDBOX::Lua_OpenWindow);
	objGlobal.Register( "CloseWindow",			SCRIPT_SANDBOX::Lua_CloseWindow);
	objGlobal.Register( "ToggleWindow",			SCRIPT_SANDBOX::Lua_ToggleWindow);


	objGlobal.Register( "SplitUnifiedString",			SCRIPT_SANDBOX::Lua_SplitUnifiedString);


//---------------------------------------------------------------------------------------------------------------------------------------------
// ȫ�ֺ���������.
//

	// ע������Ҽ��˵���
	objGlobal.Register( "Show_Team_Func_Menu",		SCRIPT_SANDBOX::Lua_ShowTeamFuncMenu);
	// ���õ�ǰѡ��Ķ�Ա
	objGlobal.Register( "SetCurSelMember",			SCRIPT_SANDBOX::Lua_SetCurSelMember);
	// ��ʾ����Ի���
	objGlobal.Register( "ShowTeamInfoDlg",			SCRIPT_SANDBOX::Lua_ShowTeamInfoDlg);
	// ��ʾ�������ȷ�϶Ի���
	objGlobal.Register( "ShowGuildInfoDlg",			SCRIPT_SANDBOX::Lua_ShowGuildInfoDlg);
	// ��˸���鰴ť
	objGlobal.Register( "FlashTeamButton",			SCRIPT_SANDBOX::Lua_FlashTeamButton);
	// ����һ����Ʒ
	objGlobal.Register( "DiscardItem",				SCRIPT_SANDBOX::Lua_DiscardItem);
	// ȡ������һ����Ʒ
	objGlobal.Register( "DiscardItemCancelLocked",	SCRIPT_SANDBOX::Lua_DiscardItemCancelLocked);
	// ����һ����Ч
	objGlobal.Register( "PlaySound",				SCRIPT_SANDBOX::Lua_PlaySound);
	// ��Lua�ű�����Ϣ
	objGlobal.Register( "PushEvent",				SCRIPT_SANDBOX::Lua_PushEvent);
	// ��������������а�
	objGlobal.Register( "SendTopList",				SCRIPT_SANDBOX::Lua_SendTopList);
	// �õ�ٽ�������
	//objGlobal.Register( "GetTopListName",				SCRIPT_SANDBOX::Lua_GetTopListName);

	objGlobal.Register( "CleanItemForTip",				SCRIPT_SANDBOX::CleanItemForTip);
	// �õ�ٽ�������
	objGlobal.Register( "CreatItemForTip",				SCRIPT_SANDBOX::CreatItemForTip);


//
// ȫ�ֺ���������
//---------------------------------------------------------------------------------------------------------------------------------------------

	// ����һ��ȫ�ֱ���
	objGlobal.Register( "SetGlobalInteger",			SCRIPT_SANDBOX::Lua_SetGlobalInteger);
	// ���һ��ȫ�ֱ���
	objGlobal.Register( "GetGlobalInteger",			SCRIPT_SANDBOX::Lua_GetGlobalInteger);
	
	// ���target����, �Ҽ��򿪲˵�
	objGlobal.Register( "OpenTargetMenu",			SCRIPT_SANDBOX::Lua_OpenTargetMenu);
	// ��ʾϵͳ��ʾ��Ϣ��
	objGlobal.Register( "ShowSystemTipInfo",		SCRIPT_SANDBOX::Lua_ShowSystemTipInfo);
	
	// �򿪸��Ķ�������Ի��� 
//	objGlobal.Register( "OpenChangeMinorPasswordDlg",	SCRIPT_SANDBOX::Lua_OpenChangeMinorPasswordDlg);
	// �򿪽������롣 
//	objGlobal.Register( "OpenUnLockeMinorPasswordDlg",	SCRIPT_SANDBOX::Lua_OpenUnLockeMinorPasswordDlg);

	// ������������档 
//	objGlobal.Register( "OpenSetMinorPasswordDlg",	SCRIPT_SANDBOX::Lua_OpenSetMinorPasswordDlg);
	// �������롣 
	objGlobal.Register( "SendSetMinorPassword",     SCRIPT_SANDBOX::Lua_SendSetMinorPassword);
	// �޸����롣 
	objGlobal.Register( "SendModifyMinorPassword",	SCRIPT_SANDBOX::Lua_SendModifyMinorPassword);
	// �������롣 
	objGlobal.Register( "UnLockMinorPassword",		SCRIPT_SANDBOX::Lua_UnLockMinorPassword);
	// ǿ�ƽ������롣 
	objGlobal.Register( "ForceUnLockMinorPassword",	SCRIPT_SANDBOX::Lua_ForceUnLockMinorPassword);
	// �޸����롣 
	objGlobal.Register( "ModifyMinorPassword",		SCRIPT_SANDBOX::Lua_ModifyMinorPassword);

	objGlobal.Register( "GetCurrentTime",			SCRIPT_SANDBOX::Lua_GetCurrentTime);

	// ���ð�ȫʱ��
	objGlobal.Register( "SetPasswordProtectTime",	SCRIPT_SANDBOX::Lua_SetPasswordProtectTime);

	// �����������
//	objGlobal.Register( "CheckInputPassword",		SCRIPT_SANDBOX::Lua_CheckInputPassword );	
//	objGlobal.Register( "SetInputPasswordState",	SCRIPT_SANDBOX::Lua_SetInputPasswordState );
//	objGlobal.Register( "GetInputPasswordState",	SCRIPT_SANDBOX::Lua_GetInputPasswordState );



	// ��ȡ���λ��
	objGlobal.Register( "GetMousePos",				SCRIPT_SANDBOX::Lua_GetMousePos);
	// ������λ����Ļ���꣩
	objGlobal.Register( "GetCurMousePos",			SCRIPT_SANDBOX::Lua_GetCurMousePos);

	// �򿪹ر�һ����װ����
	objGlobal.Register( "OpenQuickChangeEquip",		SCRIPT_SANDBOX::Lua_OpenQuickChangeEquip);

	// ��8�������ܰ�ť Nick 2007.11.22
	objGlobal.Register( "OpenMainFuncBtn",			SCRIPT_SANDBOX::Lua_OpenMainFuncBtn);


	//װ����� �����ж�
	objGlobal.Register( "GetAddHoleCondition",		SCRIPT_SANDBOX::Lua_GetAddHoleCondition);


	//��������
	objGlobal.Register( "AddStart_Equip_Info",		SCRIPT_SANDBOX::Lua_AddStart_Equip_Info);

	//��������
	objGlobal.Register( "AddStartAutoMatic_Attr_Info",		SCRIPT_SANDBOX::Lua_AddStartAutoMatic_Attr_Info);

	objGlobal.Register( "Get_Equip_StarInfo",		SCRIPT_SANDBOX::Lua_Get_Equip_StarInfo);

	//װ������
	objGlobal.Register( "Equip_Refining",		SCRIPT_SANDBOX::Lua_Equip_Refining);

	//װ����ħ
	objGlobal.Register( "Equip_Fumo",		SCRIPT_SANDBOX::Lua_Equip_Fumo);

	//װ����ħ
	objGlobal.Register( "Equip_Tuimo",		SCRIPT_SANDBOX::Lua_Equip_Tuimo);

	//��������
	objGlobal.Register( "SpecialRepair_Info",		SCRIPT_SANDBOX::Lua_SpecialRepair_Info);


	// �õ�װ���Ŀ�����
	 
	objGlobal.Register( "GetEquipHoleNumForAddHole",SCRIPT_SANDBOX::Lua_GetEquipHoleNumForAddHole);

	// �õ�װ���Ŀ�����

	objGlobal.Register( "PopRepairSureWnd",SCRIPT_SANDBOX::Lua_PopRepairSureWnd);


	// ��ñ�����䷽����
	objGlobal.Register( "EnumPackFormulationNum",SCRIPT_SANDBOX::Lua_EnumPackFormulationNum);


	//ö����ҿ����䷽�䷽ 
	 objGlobal.Register( "EnumPackProCanUse",SCRIPT_SANDBOX::Lua_EnumPackProCanUse);       


	//ȡ�ô���ͺϳ�װ�� �䷽�Ĳ��ϡ������ȡ�á� 
	  objGlobal.Register( "GetPackFormulation",SCRIPT_SANDBOX::Lua_GetPackFormulation);




	  //ȡ����� �Ƿ�����Ӧ�ȼ��Ĳ���  ���ںϳ�                        ����ͺϳ�װ��  
	  objGlobal.Register( "GetPackForMatOnGrade",SCRIPT_SANDBOX::Lua_GetPackForMatOnGrade);

	  //ȡ����� ��Ӧ�ȼ��Ĳ��ϵ�icon   
	  objGlobal.Register( "GetPackForMatIconOnGrade",SCRIPT_SANDBOX::Lua_GetPackForMatIconOnGrade);

	  //ȡ����� �Ƿ�����Ӧ�ȼ��Ĳ���  ���ڸ���                         
	  objGlobal.Register( "GetPackForMatOnGradeReWork",SCRIPT_SANDBOX::Lua_GetPackForMatOnGradeReWork);

	   

	  //ö���䷽���ֺ�������  ���ڴ���ͺϳ�װ������
	 objGlobal.Register( "EnumPackFormulation",SCRIPT_SANDBOX::Lua_EnumPackFormulation);


	 //�򿪸������
	 objGlobal.Register( "Open_Rework_Equip",SCRIPT_SANDBOX::Lua_Open_Rework_Equip);


	 //�򿪸������
	 objGlobal.Register( "GetReWorkMatInfo",SCRIPT_SANDBOX::Lua_GetReWorkMatInfo);


	 //  ---����װ������Ĳ��������͵ȼ�
	 objGlobal.Register( "EquipDismantle_Material_Info",SCRIPT_SANDBOX::Lua_EquipDismantle_Material_Info);
 

	 //�ر�װ�� ��� ��װ ���� �ƺ� ����ui
	 objGlobal.Register( "EquipAllCloseUI",SCRIPT_SANDBOX::Lua_EquipAllCloseUI);
	 //openװ�� ��� ��װ ���� �ƺ� ����ui
	 objGlobal.Register( "EquipAllOpenUI",SCRIPT_SANDBOX::Lua_EquipAllOpenUI);

	 //������ݼ�֮���ж��Ƿ���Խ���һ����װ
	 objGlobal.Register( "IsEquipExChange",SCRIPT_SANDBOX::Lua_IsEquipExChange);

	 //����װ�� ��� ��װ ���� �ƺ� ����λ��
	 objGlobal.Register( "UpdataEquipAllPos",SCRIPT_SANDBOX::Lua_UpdataEquipAllPos);

	 //Timer by fangyuan
	 objGlobal.Register("KillTimer", SCRIPT_SANDBOX::LUA_KillTimer);
	 objGlobal.Register("SetTimer", SCRIPT_SANDBOX::LUA_SetTimer);
	 objGlobal.Register("ClearAllTimer", SCRIPT_SANDBOX::LUA_ClearAllTimer);
	 
	//RMB�̳� by gh 
	 objGlobal.Register( "IB_Shop",		SCRIPT_SANDBOX::Lua_IB_Shop);

	//---------------------------------------------------------------------------------------------------------------------------------------------
	// ע���ɫ Target
	LuaObject metatable_Character = objGlobal.CreateTable("CharacterMetaTable");
	metatable_Character.SetObject("__index", metatable_Character);

	// �Ҽ��˵����
	metatable_Character.RegisterObjectFunctor( "ShowTargetContexMenu",		&SCRIPT_SANDBOX::Character::ShowTargetContexMenu);

	// �����Ƿ����
	metatable_Character.RegisterObjectFunctor( "IsPresent",						&SCRIPT_SANDBOX::Character::IsPresent);
	// ȡ������
	metatable_Character.RegisterObjectFunctor( "GetName",						&SCRIPT_SANDBOX::Character::GetName);
	// ȡ��HP
	metatable_Character.RegisterObjectFunctor( "GetHPPercent",					&SCRIPT_SANDBOX::Character::GetHPPercent);
	// ȡ��MP
	metatable_Character.RegisterObjectFunctor( "GetMPPercent",					&SCRIPT_SANDBOX::Character::GetMPPercent);
	// ȡ�÷�ŭ
	metatable_Character.RegisterObjectFunctor( "GetRagePercent",				&SCRIPT_SANDBOX::Character::GetRagePercent);
	// ����
	metatable_Character.RegisterObjectFunctor( "Follow",						&SCRIPT_SANDBOX::Character::Follow );
	// �õ���ɫ��λ��
	metatable_Character.RegisterObjectFunctor( "GetPos",						&SCRIPT_SANDBOX::Character::GetPos );

	// �����ȼ�
	metatable_Character.RegisterObjectFunctor( "GetLevel",						&SCRIPT_SANDBOX::Character::GetLevel );
	// ��ȡ�������������
	metatable_Character.RegisterObjectFunctor( "GetData",						&SCRIPT_SANDBOX::Character::GetData );
	// ѯ����ϸ����
	metatable_Character.RegisterObjectFunctor( "SendAskDetail",					&SCRIPT_SANDBOX::Character::SendAskDetail );
	// �޸������е��������
	metatable_Character.RegisterObjectFunctor( "Set_To_Private",				&SCRIPT_SANDBOX::Character::Set_To_Private );
	// �رղ鿴�Է���ɫ����UI
	metatable_Character.RegisterObjectFunctor( "Close_Before_TargetEquip_UI",	&SCRIPT_SANDBOX::Character::Close_Before_TargetEquip_UI );

	metatable_Character.RegisterObjectFunctor( "TargetEquip_ChangeModel",		&SCRIPT_SANDBOX::Character::TargetEquip_ChangeModel );	

	
//---------------------------------------------------------------------------------------------------------------------------------------------
// Target ������
//

	// �����������
	metatable_Character.RegisterObjectFunctor( "SendTeamRequest",				&SCRIPT_SANDBOX::Character::SendTeamRequest );
	// ������������������
	metatable_Character.RegisterObjectFunctor( "SendTeamApply",					&SCRIPT_SANDBOX::Character::SendTeamApply );

//
// Target ������
//---------------------------------------------------------------------------------------------------------------------------------------------


	// �õ���ɫ���ϵ�buff������
	metatable_Character.RegisterObjectFunctor( "GetBuffNumber",					&SCRIPT_SANDBOX::Character::GetBuffNumber );
	// ���Buff��ͼ�������
	metatable_Character.RegisterObjectFunctor( "GetBuffIconNameByIndex",		&SCRIPT_SANDBOX::Character::GetBuffIconNameByIndex );
	metatable_Character.RegisterObjectFunctor( "GetDialogNpcName",				&SCRIPT_SANDBOX::Character::GetDialogNpcName );
	metatable_Character.RegisterObjectFunctor( "GetShopNpcName",				&SCRIPT_SANDBOX::Character::GetShopNpcName );
	metatable_Character.RegisterObjectFunctor( "GetXinfaNpcName",				&SCRIPT_SANDBOX::Character::GetXinfaNpcName );
	metatable_Character.RegisterObjectFunctor( "GetDialogNpcSeverID",			&SCRIPT_SANDBOX::Character::GetDialogNpcSeverID );



	//---------------------------------------------------------------------------------------------------------------------------------------------
	//
	// ��ǰ��target �Ƿ��Ƕ���
	//
	metatable_Character.RegisterObjectFunctor( "IsTargetTeamMember",			&SCRIPT_SANDBOX::Character::IsTargetTeamMember );

	// ��ǰ��target �Ƿ�Ϸ� 
	metatable_Character.RegisterObjectFunctor( "IsTargetValide",				&SCRIPT_SANDBOX::Character::IsTargetValide );

	// ��ǰ��target Team����  
	metatable_Character.RegisterObjectFunctor( "TargetFrame_Update_Name_Team",	&SCRIPT_SANDBOX::Character::TargetFrame_Update_Name_Team );

	// ��ǰ��target Team hp  
	metatable_Character.RegisterObjectFunctor( "TargetFrame_Update_HP_Team",	&SCRIPT_SANDBOX::Character::TargetFrame_Update_HP_Team );

	// ��ǰ��target Team mp  
	metatable_Character.RegisterObjectFunctor( "TargetFrame_Update_MP_Team",	&SCRIPT_SANDBOX::Character::TargetFrame_Update_MP_Team );

	// ��ǰ��target Team iRange  
	metatable_Character.RegisterObjectFunctor( "TargetFrame_Update_Rage_Team",	&SCRIPT_SANDBOX::Character::TargetFrame_Update_Rage_Team );

	// ��ǰ��target Team iLevel  
	metatable_Character.RegisterObjectFunctor( "TargetFrame_Update_Level_Team", &SCRIPT_SANDBOX::Character::TargetFrame_Update_Level_Team );

	// ��ǰ��target Team ͷ��  
	metatable_Character.RegisterObjectFunctor( "TargetFrame_Update_Icon_Team",	&SCRIPT_SANDBOX::Character::TargetFrame_Update_Icon_Team );

	


	LuaObject obj_Target = pState->BoxPointer(&(SCRIPT_SANDBOX::Character::s_Target));
	obj_Target.SetMetaTable(metatable_Character);
	objGlobal.SetObject("Target", obj_Target);

	//---------------------------------------------------------------------------------
	//ע����� Player
	LuaObject metatable_PlayerMySelf = objGlobal.CreateTable("PlayerMySelfMetaTable");
	metatable_PlayerMySelf.SetObject("__index", metatable_PlayerMySelf);

	metatable_PlayerMySelf.RegisterObjectFunctor( "GetData",					&SCRIPT_SANDBOX::PlayerMySelf::GetData);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetPos",						&SCRIPT_SANDBOX::PlayerMySelf::GetPos );
	metatable_PlayerMySelf.RegisterObjectFunctor( "IsPresent",					&SCRIPT_SANDBOX::PlayerMySelf::IsPresent);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetName",					&SCRIPT_SANDBOX::PlayerMySelf::GetName);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetBuffNumber",				&SCRIPT_SANDBOX::PlayerMySelf::GetBuffNumber);

	metatable_PlayerMySelf.RegisterObjectFunctor( "GetBuffIconNameByIndex",		&SCRIPT_SANDBOX::PlayerMySelf::GetBuffIconNameByIndex);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetBuffToolTipsByIndex",		&SCRIPT_SANDBOX::PlayerMySelf::GetBuffToolTipsByIndex);
	metatable_PlayerMySelf.RegisterObjectFunctor( "DispelBuffByIndex",			&SCRIPT_SANDBOX::PlayerMySelf::DispelBuffByIndex);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetBuffTimeTextByIndex",		&SCRIPT_SANDBOX::PlayerMySelf::GetBuffTimeTextByIndex);

	// ѧϰ��������
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetAbilityStudy",				&SCRIPT_SANDBOX::PlayerMySelf::GetAbilityStudy);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetAbilityInfo",				&SCRIPT_SANDBOX::PlayerMySelf::GetAbilityInfo);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetSkillInfo",				&SCRIPT_SANDBOX::PlayerMySelf::GetSkillInfo);
	// Nick 2007.11.23
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetIndexSkillInfo",			&SCRIPT_SANDBOX::PlayerMySelf::GetIndexSkillInfo);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetSkillNum",				&SCRIPT_SANDBOX::PlayerMySelf::GetSkillNum);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetXinfaInfo",				&SCRIPT_SANDBOX::PlayerMySelf::GetXinfaInfo);

	metatable_PlayerMySelf.RegisterObjectFunctor( "AskLeanAbility",				&SCRIPT_SANDBOX::PlayerMySelf::AskLeanAbility);

	// �ƺ����
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetAgnameNum",				&SCRIPT_SANDBOX::PlayerMySelf::GetAgnameNum);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetAgnameType",				&SCRIPT_SANDBOX::PlayerMySelf::GetAgnameType);
	metatable_PlayerMySelf.RegisterObjectFunctor( "EnumAgname",					&SCRIPT_SANDBOX::PlayerMySelf::EnumAgname);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetCurrentAgname",			&SCRIPT_SANDBOX::PlayerMySelf::GetCurrentAgname);
	metatable_PlayerMySelf.RegisterObjectFunctor( "AskChangeCurrentAgname",		&SCRIPT_SANDBOX::PlayerMySelf::AskChangeCurrentAgname);
	metatable_PlayerMySelf.RegisterObjectFunctor( "SetNullAgname",				&SCRIPT_SANDBOX::PlayerMySelf::SetNullAgname);		// ��ѡ��ƺ�
	metatable_PlayerMySelf.RegisterObjectFunctor( "UpdateTitleState",			&SCRIPT_SANDBOX::PlayerMySelf::UpdateTitleState );
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetAgnameInfo",				&SCRIPT_SANDBOX::PlayerMySelf::GetAgnameInfo );
	
	// ����
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetAllAgnameNum",			&SCRIPT_SANDBOX::PlayerMySelf::GetAllAgnameNum);
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetTableAgnameInfo",			&SCRIPT_SANDBOX::PlayerMySelf::GetTableAgnameInfo);




//---------------------------------------------------------------------------------------------------------------------------------------------------------
// װ�����Խ���
//

	// �����������Ե�����Ϣ
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendAskManualAttr",			&SCRIPT_SANDBOX::PlayerMySelf::SendAskManualAttr);

	// �õ���ǰ�ƺ�
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetCurTitle",				&SCRIPT_SANDBOX::PlayerMySelf::GetCurTitle);

	//��ý�ɫ��ǰ�Ի���NPC��ID
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetCurDialogNpcId",				&SCRIPT_SANDBOX::PlayerMySelf::GetCurDialogNpcId);

//
// װ�����Խ���
//---------------------------------------------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------------------------------------
// ���������������
//

	// ͬ��������
	metatable_PlayerMySelf.RegisterObjectFunctor( "AgreeJoinTeam",				&SCRIPT_SANDBOX::PlayerMySelf::SendAgreeJoinTeam);
	// �ܾ��������
	metatable_PlayerMySelf.RegisterObjectFunctor( "RejectJoinTeam",				&SCRIPT_SANDBOX::PlayerMySelf::SendRejectJoinTeam);

	// ͬ��������(��������������)
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendAgreeJoinTeam_Apply",	&SCRIPT_SANDBOX::PlayerMySelf::SendAgreeJoinTeam_Apply);
	// �ܾ��������(��������������)
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendRejectJoinTeam_Apply",	&SCRIPT_SANDBOX::PlayerMySelf::SendRejectJoinTeam_Apply);

	// ͬ��������(��Ա����ĳ�˼������)
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendAgreeJoinTeam_TeamMemberInvite",		&SCRIPT_SANDBOX::PlayerMySelf::SendAgreeJoinTeam_TeamMemberInvite);
	// �ܾ��������(��Ա����ĳ�˼������)
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendRejectJoinTeam_TeamMemberInvite",	&SCRIPT_SANDBOX::PlayerMySelf::SendRejectJoinTeam_TeamMemberInvite);
	
	// ͬ�������Ӹ���ģʽ
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendAgreeTeamFollow",		&SCRIPT_SANDBOX::PlayerMySelf::SendAgreeTeamFollow);
	// �ܾ�������Ӹ���ģʽ
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendRefuseTeamFollow",		&SCRIPT_SANDBOX::PlayerMySelf::SendRefuseTeamFollow);

	// �뿪����
	metatable_PlayerMySelf.RegisterObjectFunctor( "LeaveTeam",					&SCRIPT_SANDBOX::PlayerMySelf::LeaveTeam);
	// ��ɢ����
	metatable_PlayerMySelf.RegisterObjectFunctor( "DismissTeam",				&SCRIPT_SANDBOX::PlayerMySelf::DismissTeam);
	// �߳���ǰѡ�еĶ�Ա.
	metatable_PlayerMySelf.RegisterObjectFunctor( "KickTeamMember",				&SCRIPT_SANDBOX::PlayerMySelf::KickTeamMember);
	// �Խ�����.
	metatable_PlayerMySelf.RegisterObjectFunctor( "CreateTeamSelf",				&SCRIPT_SANDBOX::PlayerMySelf::CreateTeamSelf);
	// �����ӳ�.
	metatable_PlayerMySelf.RegisterObjectFunctor( "AppointLeader",				&SCRIPT_SANDBOX::PlayerMySelf::AppointLeader);

	// �Ƿ�����Ӹ���״̬
	metatable_PlayerMySelf.RegisterObjectFunctor( "InTeamFollowMode",			&SCRIPT_SANDBOX::PlayerMySelf::InTeamFollowMode);
	// ȡ������
	metatable_PlayerMySelf.RegisterObjectFunctor( "StopFollow",					&SCRIPT_SANDBOX::PlayerMySelf::StopFollow);
	// ��Ӹ���
	metatable_PlayerMySelf.RegisterObjectFunctor( "TeamFrame_AskTeamFollow",	&SCRIPT_SANDBOX::PlayerMySelf::TeamFrame_AskTeamFollow);

	// �õ�team member guid
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetTeamMemberGUID",			&SCRIPT_SANDBOX::PlayerMySelf::GetTeamMemberGUID);

	//�޸����״̬����Ʒʰȡģʽ
	metatable_PlayerMySelf.RegisterObjectFunctor( "ModifyTeamAllocationMode",	&SCRIPT_SANDBOX::PlayerMySelf::ModifyTeamAllocationMode);
	//������״̬����Ʒʰȡģʽ
	metatable_PlayerMySelf.RegisterObjectFunctor( "GetTeamAllocationMode",		&SCRIPT_SANDBOX::PlayerMySelf::GetTeamAllocationMode);

	// ����
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendReliveMessage_OutGhost", &SCRIPT_SANDBOX::PlayerMySelf::SendReliveMessage_OutGhost);
	// ����
	metatable_PlayerMySelf.RegisterObjectFunctor( "SendReliveMessage_Relive",	&SCRIPT_SANDBOX::PlayerMySelf::SendReliveMessage_Relive);

	// ���ö�������
	metatable_PlayerMySelf.RegisterObjectFunctor( "SetSupperPassword",			&SCRIPT_SANDBOX::PlayerMySelf::SetSupperPassword);
	metatable_PlayerMySelf.RegisterObjectFunctor( "PasswordGetData",			&SCRIPT_SANDBOX::PlayerMySelf::Lua_PasswordGetData);
	metatable_PlayerMySelf.RegisterObjectFunctor( "SavePasswordSetup",			&SCRIPT_SANDBOX::PlayerMySelf::Lua_SavePasswordSetup);
	metatable_PlayerMySelf.RegisterObjectFunctor( "PasswordTypeGetState",		&SCRIPT_SANDBOX::PlayerMySelf::Lua_PasswordTypeGetState );

	// ����������
	metatable_PlayerMySelf.RegisterObjectFunctor( "CheckPasswdProcedure",		&SCRIPT_SANDBOX::PlayerMySelf::Lua_CheckPasswdProcedure );
	// ������������������
	metatable_PlayerMySelf.RegisterObjectFunctor( "InputPasswdProcedure",		&SCRIPT_SANDBOX::PlayerMySelf::Lua_InputPasswdProcedure );
	
	
//
// ���������
//-----------------------------------------------------------------------------------------------------------------------------------------------------------


	// �Ҽ��˵����
	metatable_PlayerMySelf.RegisterObjectFunctor( "ShowMySelfContexMenu",		&SCRIPT_SANDBOX::PlayerMySelf::ShowMySelfContexMenu);
	// ���ѡ���Լ�
	metatable_PlayerMySelf.RegisterObjectFunctor( "SelectMyselfAsTarget",		&SCRIPT_SANDBOX::PlayerMySelf::SelectMyselfAsTarget);


	LuaObject obj_PlayerMySelf = pState->BoxPointer(&(SCRIPT_SANDBOX::PlayerMySelf::s_MySelf));

	obj_PlayerMySelf.SetMetaTable(metatable_PlayerMySelf);
	objGlobal.SetObject("Player", obj_PlayerMySelf);
	objGlobal.SetObject("Player2", obj_PlayerMySelf);
	objGlobal.SetObject("PlayerArm", obj_PlayerMySelf);

	//---------------------------------------------------------------------------------
	//ע�� ���ư�ť����
	CActionItem::s_pMetaTable = new LuaPlus::LuaObject;
	*CActionItem::s_pMetaTable = objGlobal.CreateTable("ActionButtonMetaTable");
	CActionItem::s_pMetaTable->SetObject("__index", *CActionItem::s_pMetaTable);

	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetID",					&CActionItem::Lua_GetID);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetType",				&CActionItem::Lua_GetType);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetName",				&CActionItem::Lua_GetName);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetOwnerXinfa",			&CActionItem::Lua_GetOwnerXinfa);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetPetSkillOwner",		&CActionItem::Lua_GetPetSkillOwner);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetDefineID",			&CActionItem::Lua_GetDefineID);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetPrice",				&CActionItem::Lua_GetPrice);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetNum",					&CActionItem::Lua_GetNum);
	CActionItem::s_pMetaTable->RegisterObjectFunctor( "GetDesc",				&CActionItem::Lua_GetDesc);

	//---------------------------------------------------------------------------------
	//ע�����ݳص�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::DataPool::s_pMetaTable = objGlobal.CreateTable("DataPoolMetaTable");
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::DataPool::s_pMetaTable);

	//������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfo_Num",			&SCRIPT_SANDBOX::DataPool::GetMissionInfo_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfo_Text",			&SCRIPT_SANDBOX::DataPool::GetMissionInfo_Text);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfo_Bonus",			&SCRIPT_SANDBOX::DataPool::GetMissionInfo_Bonus);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfo_Kind",			&SCRIPT_SANDBOX::DataPool::GetMissionInfo_Kind);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfo_IsDayTip",		&SCRIPT_SANDBOX::DataPool::GetMissionInfo_IsDayTip);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfo_DayTipCount",	&SCRIPT_SANDBOX::DataPool::GetMissionInfo_DayTipCount);	

	//����������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionDemand_Num",			&SCRIPT_SANDBOX::DataPool::GetMissionDemand_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "CloseMissionFrame",				&SCRIPT_SANDBOX::DataPool::CloseMissionFrame);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionDemand_Text",			&SCRIPT_SANDBOX::DataPool::GetMissionDemand_Text);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionDemand_Item",			&SCRIPT_SANDBOX::DataPool::GetMissionDemand_Item);
	//Continue
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionContinue_Num",		&SCRIPT_SANDBOX::DataPool::GetMissionContinue_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionContinue_Text",		&SCRIPT_SANDBOX::DataPool::GetMissionContinue_Text);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMissionContinue_Bonus",		&SCRIPT_SANDBOX::DataPool::GetMissionContinue_Bonus);

	//ȡ��������б�
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetScoutMission",			&SCRIPT_SANDBOX::DataPool::GetScoutMission);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SetScoutMission",		&SCRIPT_SANDBOX::DataPool::SetScoutMission);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetScoutMission_Num",		&SCRIPT_SANDBOX::DataPool::GetScoutMission_Num);
	
	//Npc�Զ���·
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "NpcAutoFindPath",		&SCRIPT_SANDBOX::DataPool::NpcAutoFindPath);

	// �ڳ�����ͼ���ڵ������ƶ�������λ��ʱ�������ǰ��Npc�Զ�Ѱ·״̬���������Զ�Ѱ·�Ĳ���ֵ	20100531 BLL
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ResetNpcAutoFindPath",		&SCRIPT_SANDBOX::DataPool::ResetNpcAutoFindPath);	

	//NPC�糡���Զ�Ѱ·
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "NpcOutSceneAutoFindPath",		&SCRIPT_SANDBOX::DataPool::NpcOutSceneAutoFindPath);

	//��ҵ�ǰ��������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Num",			&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_InUse",		&SCRIPT_SANDBOX::DataPool::GetPlayerMission_InUse);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Description",	&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Description);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionCustom_Num",	&SCRIPT_SANDBOX::DataPool::GetPlayerMissionCustom_Num);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionCustom",		&SCRIPT_SANDBOX::DataPool::GetPlayerMissionCustom);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_DataRound",	&SCRIPT_SANDBOX::DataPool::GetPlayerMission_DataRound);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Mission_Abnegate_Popup",		&SCRIPT_SANDBOX::DataPool::Mission_Abnegate_Popup);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "EnumPlayerMission_ItemAction",	&SCRIPT_SANDBOX::DataPool::EnumPlayerMission_ItemAction);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Mission_Abnegate_Special_Quest_Popup",	&SCRIPT_SANDBOX::DataPool::Mission_Abnegate_Special_Quest_Popup);


	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_ItemCountNow",	&SCRIPT_SANDBOX::DataPool::GetPlayerMission_ItemCountNow);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_BillName",		&SCRIPT_SANDBOX::DataPool::GetPlayerMission_BillName);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Name",			&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Name);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Info",			&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Info);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Variable",		&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Variable);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Display",		&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Display);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_RemainTime",	&SCRIPT_SANDBOX::DataPool::GetPlayerMission_RemainTime);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionDemand_Num",	&SCRIPT_SANDBOX::DataPool::GetPlayerMissionDemand_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionDemand_Item",	&SCRIPT_SANDBOX::DataPool::GetPlayerMissionDemand_Item);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionDemandKill_Num",&SCRIPT_SANDBOX::DataPool::GetPlayerMissionDemandKill_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionDemand_NPC",	&SCRIPT_SANDBOX::DataPool::GetPlayerMissionDemand_NPC);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionBonus_Num",		&SCRIPT_SANDBOX::DataPool::GetPlayerMissionBonus_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMissionBonus_Item",	&SCRIPT_SANDBOX::DataPool::GetPlayerMissionBonus_Item);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Memo",			&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Memo);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Level",		&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Level);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_Kind",			&SCRIPT_SANDBOX::DataPool::GetPlayerMission_Kind);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_IsElite",		&SCRIPT_SANDBOX::DataPool::GetPlayerMission_IsElite);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPlayerMission_SceneID",		&SCRIPT_SANDBOX::DataPool::GetPlayerMission_SceneID);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetNPCEventList_Num",			&SCRIPT_SANDBOX::DataPool::GetNPCEventList_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetNPCEventList_Item",			&SCRIPT_SANDBOX::DataPool::GetNPCEventList_Item);
	//����������أ��й��䷽���֡�
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Num",				&SCRIPT_SANDBOX::DataPool::GetPrescrList_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Item",			&SCRIPT_SANDBOX::DataPool::GetPrescrList_Item);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Item_LifeAbility",&SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_LifeAbility);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Item_Requirement",&SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_Requirement);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Item_Result",		&SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_Result);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Item_NeedTool",		&SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_NeedTool);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Item_Desc",		&SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_Desc);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPrescrList_Item_ListPos",		&SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_ListPos);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetBoothList_Num",				&SCRIPT_SANDBOX::DataPool::GetBoothList_Num );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetBoothGood_ID",				&SCRIPT_SANDBOX::DataPool::GetBoothGood_ID );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetBoothGood_Name",				&SCRIPT_SANDBOX::DataPool::GetBoothGood_Name );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetBoothGood_Price",			&SCRIPT_SANDBOX::DataPool::GetBoothGood_Price );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ReloadWindowScript",			&SCRIPT_SANDBOX::DataPool::Lua_ReloadWindowScript );
	

//-----------------------------------------------------------------------------------------------------------------------------------------------------
// ���ݳ�������
//

	// �õ��Լ���GUID
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetSelfGUID",				&SCRIPT_SANDBOX::DataPool::GetSelfGUID);
	// �õ������ж�Ա��λ����Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetTeamMemPosByUIIndex",				&SCRIPT_SANDBOX::DataPool::GetTeamMemPosByUIIndex);
	// �õ������ж�Ա����Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetTeamMemberInfo",				&SCRIPT_SANDBOX::DataPool::GetTeamMemInfo);
	// �õ������ж�Ա��GUID
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetTeamMemGUIDByUIIndex",		&SCRIPT_SANDBOX::DataPool::GetTeamMemGUIDByUIIndex);
	// ͨ�������õ������ж�Ա����Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetTeamMemInfoByIndex",			&SCRIPT_SANDBOX::DataPool::GetTeamMemInfoByIndex);
	// �õ������ж�Ա�ĸ���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetTeamMemberCount",			&SCRIPT_SANDBOX::DataPool::GetTeamMemCount);

	// �Ƿ��Ƕӳ�
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "IsTeamLeader",					&SCRIPT_SANDBOX::DataPool::IsTeamLeader);

	//	�Լ��Ƿ��Ƕӳ�(yyz_add)
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMyTeamLeaderFlag",			&SCRIPT_SANDBOX::DataPool::GetMyTeamLeaderFlag);

	// �õ��������ĸ���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetInviteTeamCount",			&SCRIPT_SANDBOX::DataPool::GetInviteTeamCount);

	// �õ�ĳ���������Ķ�Ա�ĸ���.
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetInviteTeamMemberCount",		&SCRIPT_SANDBOX::DataPool::GetInviteTeamMemberCount);
	// �õ��������Ķ�Ա��Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetInviteTeamMemberInfo",		&SCRIPT_SANDBOX::DataPool::GetInviteTeamMemberInfo);

	// �ӳ��򿪶�����Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SetSelectTeamMember_Leader",	&SCRIPT_SANDBOX::DataPool::SetSelectTeamMember_Leader);

	// �õ������˵ĸ���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetApplyMemberCount",			&SCRIPT_SANDBOX::DataPool::GetApplyMemberCount);
	// �õ���������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetApplyMemberInfo",			&SCRIPT_SANDBOX::DataPool::GetApplyMemberInfo);
	// ����ѡ�е�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SetCurSelApply",				&SCRIPT_SANDBOX::DataPool::SetCurSelApply);
	// ����ѡ�е�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "EraseApply",					&SCRIPT_SANDBOX::DataPool::EraseApply);
	// ���������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ClearAllApply",					&SCRIPT_SANDBOX::DataPool::ClearAllApply);

	// ���ö��������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SetTeamFrameOpenFlag",			&SCRIPT_SANDBOX::DataPool::SetTeamFrameOpenFlag);

	// �õ��������ģ������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetInviteTeamMemberUIModelName",&SCRIPT_SANDBOX::DataPool::GetInviteTeamMemberUIModelName);

	// �õ������Աģ������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetTeamMemUIModelName",			&SCRIPT_SANDBOX::DataPool::GetTeamMemUIModelName);

	// �õ�������ģ������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetApplyMemberUIModelName",     &SCRIPT_SANDBOX::DataPool::GetApplyMemberUIModelName);

	// �������е��Լ���Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SetSelfInfo",					&SCRIPT_SANDBOX::DataPool::SetSelfInfo);

	// ��Ա�Ƿ��ڳ�����
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "IsTeamMemberInScene",			&SCRIPT_SANDBOX::DataPool::IsTeamMemberInScene);

	// ͨ�����ѽ���ѡ�����, ��Ϊtarget
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SelectAsTargetByUIIndex",		&SCRIPT_SANDBOX::DataPool::SelectAsTargetByUIIndex);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMyTeamFlag",					&SCRIPT_SANDBOX::DataPool::GetMyTeamFlag);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMyTeamMemberCount",			&SCRIPT_SANDBOX::DataPool::GetMyTeamMemberCount);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetDistanceBySeverID",			&SCRIPT_SANDBOX::DataPool::GetDistanceBySeverID);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetAnotherTeamMemberSeverID",	&SCRIPT_SANDBOX::DataPool::GetAnotherTeamMemberSeverID);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetClientIDBySeverID",			&SCRIPT_SANDBOX::DataPool::GetClientIDBySeverID);

	//
// ���ݳ����������
//------------------------------------------------------------------------------------------------------------------------------------------------------



	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPacketItem",					&SCRIPT_SANDBOX::DataPool::GetPacketItem );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPacketItemMaxNumber",		&SCRIPT_SANDBOX::DataPool::GetPacketItemMaxNumber );

    SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPacketItemFroId",			&SCRIPT_SANDBOX::DataPool::GetPacketItemFroId );


	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetPacketItemAllNum",			&SCRIPT_SANDBOX::DataPool::GetPacketItemAllNum );
	// �����б����

	// �õ�ĳһ��ĺ�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetFriendNumber",				&SCRIPT_SANDBOX::DataPool::GetFriendNumber );
	// �õ�ĳһ��ĺ��ѵľ�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetFriend",						&SCRIPT_SANDBOX::DataPool::GetFriend );
	// ���һ�����ѵ�ĳһ��
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "AddFriend",						&SCRIPT_SANDBOX::DataPool::AddFriend );
	// �õ�һ������Ƿ����Լ��������ϵ��
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "IsTempFriendByName",			&SCRIPT_SANDBOX::DataPool::IsTempFriendByName );

	// �õ�һ������Ƿ��Ѿ���ӵ��������б���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "IsInBlackListByName",			&SCRIPT_SANDBOX::DataPool::IsInBlackListByName );

	// ͨ��GUID�õ�һ������Ƿ��Ѿ���ӵ��������б���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "IsInBlackListByGuid",			&SCRIPT_SANDBOX::DataPool::IsInBlackListByGuid );

	// �õ�һ������Ƿ��Ѿ���ӵ������б���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "IsInEnemyListByName",			&SCRIPT_SANDBOX::DataPool::IsInEnemyListByName );

	// ��������ͨ���Ҽ������Ҷ����Ϊ����
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "AddFriendByModel",				&SCRIPT_SANDBOX::DataPool::AddFriendByModel );

	// ��ϵͳ���ɾ����������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "AskDelFriend",					&SCRIPT_SANDBOX::DataPool::AskDelFriend );

	// ȷ��ɾ��һ������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "DelFriend",						&SCRIPT_SANDBOX::DataPool::DelFriend );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "LuaShowFriendInfo",				&SCRIPT_SANDBOX::DataPool::LuaShowFriendInfo );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ShowFriendInfo",				&SCRIPT_SANDBOX::DataPool::ShowFriendInfo );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ShowChatInfo",					&SCRIPT_SANDBOX::DataPool::ShowChatInfo );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SetMood",						&SCRIPT_SANDBOX::DataPool::SetMood );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMood",						&SCRIPT_SANDBOX::DataPool::GetMood );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "FriendSeting",					&SCRIPT_SANDBOX::DataPool::FriendSeting );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetFriendByName",				&SCRIPT_SANDBOX::DataPool::GetFriendByName );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "AddChatHistoryMessage",					&SCRIPT_SANDBOX::DataPool::AddChatHistoryMessage );
	// ȷ�����������ҵ������б�
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SureAddFriend",					&SCRIPT_SANDBOX::DataPool::SureAddFriend );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "RejectAddFriend",				&SCRIPT_SANDBOX::DataPool::RejectAddFriend );
	
	/*
		LAIM ����3����ʵ��
	*/
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "MailAutoRestoreGet",					&SCRIPT_SANDBOX::DataPool::MailAutoRestoreGet );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ChatAutoRestoreGet",					&SCRIPT_SANDBOX::DataPool::ChatAutoRestoreGet );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "AutoRestoreGetContent",					&SCRIPT_SANDBOX::DataPool::AutoRestoreGetContent );

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMailNumber",					&SCRIPT_SANDBOX::DataPool::GetMailNumber );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetMail",						&SCRIPT_SANDBOX::DataPool::GetMail );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "OpenMailBrowser",				&SCRIPT_SANDBOX::DataPool::OpenMailBrowser );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "OpenMailEdit",					&SCRIPT_SANDBOX::DataPool::OpenMailEdit );
	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SendMail",						&SCRIPT_SANDBOX::DataPool::SendMail );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "GetNextMail",					&SCRIPT_SANDBOX::DataPool::GetNextMail );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "OpenFriendList",				&SCRIPT_SANDBOX::DataPool::OpenFriendList );

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ThrowToBlackList",				&SCRIPT_SANDBOX::DataPool::ThrowToBlackList );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ThrowToFriendList",				&SCRIPT_SANDBOX::DataPool::ThrowToFriendList );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ThrowToEnemyList",				&SCRIPT_SANDBOX::DataPool::ThrowToEnemyList );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "ThrowToList",					&SCRIPT_SANDBOX::DataPool::ThrowToList );

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "OpenHistroy",					&SCRIPT_SANDBOX::DataPool::OpenHistroy );

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Change_MyHairColor",			&SCRIPT_SANDBOX::DataPool::Change_MyHairColor );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Change_MyHairStyle",			&SCRIPT_SANDBOX::DataPool::Change_MyHairStyle );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Change_MyHairStyle_Item",		&SCRIPT_SANDBOX::DataPool::Change_MyHairStyle_Item );


	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Get_MyHairColor",				&SCRIPT_SANDBOX::DataPool::Get_MyHairColor );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Get_MyHairStyle",				&SCRIPT_SANDBOX::DataPool::Get_MyHairStyle );


	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Change_RectifyColor",			&SCRIPT_SANDBOX::DataPool::Change_RectifyColor );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Change_GetColorLumination",		&SCRIPT_SANDBOX::DataPool::Change_GetColorLumination );

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "IsCanDoAction",					&SCRIPT_SANDBOX::DataPool::IsCanDoAction );

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "Check_StringCode",				&SCRIPT_SANDBOX::DataPool::Check_StringCode) ;



	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	// target ѡ�п��ǲ��ڵ�ǰ�Ķ���ʱ
	//

	// ��ǰ��target Team����  
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Name_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Name_Team);

	// ��ǰ��target Team hp  
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_HP_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_HP_Team);

	// ��ǰ��target Team mp  
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_MP_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_MP_Team);
	
	// ��ǰ��target Team iRange  
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Rage_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Rage_Team);

	// ��ǰ��target Team iLevel  
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Level_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Level_Team);

	// ��ǰ��target Team ͷ��  
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Icon_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Icon_Team);

//	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("NetClose", &SCRIPT_SANDBOX::DataPool::NetClose);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("ToggleShowPetList", &SCRIPT_SANDBOX::DataPool::ToggleShowPetList);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetNPCIDByServerID", &SCRIPT_SANDBOX::DataPool::GetNPCIDByServerID);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetUIColor", &SCRIPT_SANDBOX::DataPool::GetUIColor);

	//lua�е�����������������ֻ��ռλ������Ӧ������ʾ���ѵĸ���������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_BuffNum_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_BuffNum_Team);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_BuffInfo_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_BuffInfo_Team);

	

	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	// ϵͳ��ʾ

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SystemTip_GetEventNum",			&SCRIPT_SANDBOX::DataPool::SystemTip_GetEventNum );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SystemTip_GetEventInfo",		&SCRIPT_SANDBOX::DataPool::SystemTip_GetEventInfo );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor( "SystemTip_SetEventState",		&SCRIPT_SANDBOX::DataPool::SystemTip_SetEventState );
	




	LuaObject obj_DataPool = pState->BoxPointer(&(SCRIPT_SANDBOX::DataPool::s_DataPool));


	obj_DataPool.SetMetaTable(*SCRIPT_SANDBOX::DataPool::s_pMetaTable);
	objGlobal.SetObject("DataPool", obj_DataPool);


	//---------------------------------------------------------------------------------
	//ע���������ʦ��������
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable = objGlobal.CreateTable("AbilityTeacherTable");
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable);

	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::AbilityTeacher::IsPresent);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetAbilityID", &SCRIPT_SANDBOX::AbilityTeacher::GetAbilityID);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetNeedExp", &SCRIPT_SANDBOX::AbilityTeacher::GetNeedExp);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetNeedMoney", &SCRIPT_SANDBOX::AbilityTeacher::GetNeedMoney);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetNeedSkillExp", &SCRIPT_SANDBOX::AbilityTeacher::GetNeedSkillExp);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetServerData", &SCRIPT_SANDBOX::AbilityTeacher::GetServerData);

	LuaObject obj_Ability = pState->BoxPointer(&(SCRIPT_SANDBOX::AbilityTeacher::s_AbilityTeacher));
	obj_Ability.SetMetaTable(*SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable);
	objGlobal.SetObject("AbilityTeacher", obj_Ability);

	//---------------------------------------------------------------------------------
	//ע����ﵼ������
	SCRIPT_SANDBOX::Pet::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Pet::s_pMetaTable = objGlobal.CreateTable("PetTable");
	SCRIPT_SANDBOX::Pet::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Pet::s_pMetaTable);
	
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::Pet::IsPresent);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPet_Count", &SCRIPT_SANDBOX::Pet::GetPet_Count);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetList_Appoint", &SCRIPT_SANDBOX::Pet::GetPetList_Appoint);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Go_Fight", &SCRIPT_SANDBOX::Pet::Go_Fight);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Go_Relax", &SCRIPT_SANDBOX::Pet::Go_Relax);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Go_Free", &SCRIPT_SANDBOX::Pet::Go_Free);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Feed", &SCRIPT_SANDBOX::Pet::Feed);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Dome", &SCRIPT_SANDBOX::Pet::Dome);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetName", &SCRIPT_SANDBOX::Pet::GetName);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetID", &SCRIPT_SANDBOX::Pet::GetID);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetNaturalLife", &SCRIPT_SANDBOX::Pet::GetNaturalLife);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMaxLife", &SCRIPT_SANDBOX::Pet::GetMaxLife);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetHappy", &SCRIPT_SANDBOX::Pet::GetHappy);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPotential", &SCRIPT_SANDBOX::Pet::GetPotential);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetConsort", &SCRIPT_SANDBOX::Pet::GetConsort);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetSex", &SCRIPT_SANDBOX::Pet::GetSex);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetBasic", &SCRIPT_SANDBOX::Pet::GetBasic);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetLoyalgGade", &SCRIPT_SANDBOX::Pet::GetLoyalgGade);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetLevel", &SCRIPT_SANDBOX::Pet::GetLevel);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetType", &SCRIPT_SANDBOX::Pet::GetType);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetHP", &SCRIPT_SANDBOX::Pet::GetHP);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMaxHP", &SCRIPT_SANDBOX::Pet::GetMaxHP);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMP", &SCRIPT_SANDBOX::Pet::GetMP);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMaxMP", &SCRIPT_SANDBOX::Pet::GetMaxMP);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetStrAptitude", &SCRIPT_SANDBOX::Pet::GetStrAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPFAptitude", &SCRIPT_SANDBOX::Pet::GetPFAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetDexAptitude", &SCRIPT_SANDBOX::Pet::GetDexAptitude);
//	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetIntAptitude", &SCRIPT_SANDBOX::Pet::GetIntAptitude);
//	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetStaAptitude", &SCRIPT_SANDBOX::Pet::GetStaAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetStr", &SCRIPT_SANDBOX::Pet::GetStr);
//	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetInt", &SCRIPT_SANDBOX::Pet::GetInt);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetDex", &SCRIPT_SANDBOX::Pet::GetDex);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPF", &SCRIPT_SANDBOX::Pet::GetPF);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetSta", &SCRIPT_SANDBOX::Pet::GetSta);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPotential", &SCRIPT_SANDBOX::Pet::GetPotential);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetExp", &SCRIPT_SANDBOX::Pet::GetExp);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetNearAttack", &SCRIPT_SANDBOX::Pet::GetNearAttack);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetNearRecovery", &SCRIPT_SANDBOX::Pet::GetNearRecovery);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetFarAttack", &SCRIPT_SANDBOX::Pet::GetFarAttack);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetFarRecovery", &SCRIPT_SANDBOX::Pet::GetFarRecovery);

//	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsAttack", &SCRIPT_SANDBOX::Pet::GetPhysicsAttack);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMagicAttack", &SCRIPT_SANDBOX::Pet::GetMagicAttack);
//	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsRecovery", &SCRIPT_SANDBOX::Pet::GetPhysicsRecovery);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMagicRecovery", &SCRIPT_SANDBOX::Pet::GetMagicRecovery);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMiss", &SCRIPT_SANDBOX::Pet::GetMiss);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetShootProbability", &SCRIPT_SANDBOX::Pet::GetShootProbability);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Add_Attribute", &SCRIPT_SANDBOX::Pet::Add_Attribute);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Change_Name", &SCRIPT_SANDBOX::Pet::Change_Name);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetIsFighting", &SCRIPT_SANDBOX::Pet::GetIsFighting);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SetModel", &SCRIPT_SANDBOX::Pet::SetModel);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetDeathWarrant", &SCRIPT_SANDBOX::Pet::GetDeathWarrant);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetSkillID", &SCRIPT_SANDBOX::Pet::GetSkillID);
	//add for petskillstudy
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SetSkillStudyModel", &SCRIPT_SANDBOX::Pet::SetSkillStudyModel);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("ShowTargetPet", &SCRIPT_SANDBOX::Pet::ShowTargetPet);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudy_Do", &SCRIPT_SANDBOX::Pet::SkillStudy_Do);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudyUnlock", &SCRIPT_SANDBOX::Pet::SkillStudyUnlock);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudy_MenPaiSkill_Created", &SCRIPT_SANDBOX::Pet::SkillStudy_MenPaiSkill_Created);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudy_MenPaiSkill_Clear", &SCRIPT_SANDBOX::Pet::SkillStudy_MenPaiSkill_Clear);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetGUID", &SCRIPT_SANDBOX::Pet::GetGUID);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("ShowPetList", &SCRIPT_SANDBOX::Pet::ShowPetList);
	//add for contexmenu
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("ShowMyPetContexMenu", &SCRIPT_SANDBOX::Pet::ShowMyPetContexMenu);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("HandlePetMenuItem", &SCRIPT_SANDBOX::Pet::HandlePetMenuItem);
	//add for impact
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetPortraitByIndex", &SCRIPT_SANDBOX::Pet::GetPetPortraitByIndex);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetImpactNum", &SCRIPT_SANDBOX::Pet::GetPetImpactNum);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetImpactIconNameByIndex", &SCRIPT_SANDBOX::Pet::GetPetImpactIconNameByIndex);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("EnumPetSkill", &SCRIPT_SANDBOX::Pet::Lua_EnumPetSkill);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Select_Pet", &SCRIPT_SANDBOX::Pet::Select_Pet);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetAIType", &SCRIPT_SANDBOX::Pet::GetAIType);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetSkillPassive", &SCRIPT_SANDBOX::Pet::GetSkillPassive);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Free_Confirm", &SCRIPT_SANDBOX::Pet::Free_Confirm);

	
	
	
	LuaObject obj_Pet = pState->BoxPointer(&(SCRIPT_SANDBOX::Pet::s_Pet));
	obj_Pet.SetMetaTable(*SCRIPT_SANDBOX::Pet::s_pMetaTable);
	objGlobal.SetObject("Pet", obj_Pet);

	//---------------------------------------------------------------------------------
	//ע��������Ѱ嵼������
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable = objGlobal.CreateTable("PetInviteFriendTable");
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable);

	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetInviteNum", &SCRIPT_SANDBOX::PetInviteFriend::GetInviteNum);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetHumanINFO", &SCRIPT_SANDBOX::PetInviteFriend::GetHumanINFO);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetInviteMsg", &SCRIPT_SANDBOX::PetInviteFriend::GetInviteMsg);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("SetPetModel", &SCRIPT_SANDBOX::PetInviteFriend::SetPetModel);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("ShowTargetPet", &SCRIPT_SANDBOX::PetInviteFriend::ShowTargetPet);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetPetINFO", &SCRIPT_SANDBOX::PetInviteFriend::GetPetINFO);

	LuaObject obj_PetInviteFriend = pState->BoxPointer(&(SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend));
	obj_PetInviteFriend.SetMetaTable(*SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable);
	objGlobal.SetObject("PetInviteFriend", obj_PetInviteFriend);

	//---------------------------------------------------------------------------------
	//ע��Է����ﵼ������
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::TargetPet::s_pMetaTable = objGlobal.CreateTable("TargetPetTable");
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::TargetPet::s_pMetaTable);
	
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::TargetPet::IsPresent);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPet_Count", &SCRIPT_SANDBOX::TargetPet::GetPet_Count);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPetList_Appoint", &SCRIPT_SANDBOX::TargetPet::GetPetList_Appoint);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Go_Fight", &SCRIPT_SANDBOX::TargetPet::Go_Fight);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Go_Relax", &SCRIPT_SANDBOX::TargetPet::Go_Relax);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetName", &SCRIPT_SANDBOX::TargetPet::GetName);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetID", &SCRIPT_SANDBOX::TargetPet::GetID);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetNaturalLife", &SCRIPT_SANDBOX::TargetPet::GetNaturalLife);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMaxLife", &SCRIPT_SANDBOX::TargetPet::GetMaxLife);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetHappy", &SCRIPT_SANDBOX::TargetPet::GetHappy);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetConsort", &SCRIPT_SANDBOX::TargetPet::GetConsort);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetSex", &SCRIPT_SANDBOX::TargetPet::GetSex);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetBasic", &SCRIPT_SANDBOX::TargetPet::GetBasic);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetLoyalgGade", &SCRIPT_SANDBOX::TargetPet::GetLoyalgGade);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetLevel", &SCRIPT_SANDBOX::TargetPet::GetLevel);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetType", &SCRIPT_SANDBOX::TargetPet::GetType);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetHP", &SCRIPT_SANDBOX::TargetPet::GetHP);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMaxHP", &SCRIPT_SANDBOX::TargetPet::GetMaxHP);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMP", &SCRIPT_SANDBOX::TargetPet::GetMP);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMaxMP", &SCRIPT_SANDBOX::TargetPet::GetMaxMP);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetStrAptitude", &SCRIPT_SANDBOX::TargetPet::GetStrAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPFAptitude", &SCRIPT_SANDBOX::TargetPet::GetPFAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetDexAptitude", &SCRIPT_SANDBOX::TargetPet::GetDexAptitude);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetIntAptitude", &SCRIPT_SANDBOX::TargetPet::GetIntAptitude);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetStaAptitude", &SCRIPT_SANDBOX::TargetPet::GetStaAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetStr", &SCRIPT_SANDBOX::TargetPet::GetStr);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetInt", &SCRIPT_SANDBOX::TargetPet::GetInt);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetDex", &SCRIPT_SANDBOX::TargetPet::GetDex);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPF", &SCRIPT_SANDBOX::TargetPet::GetPF);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetSta", &SCRIPT_SANDBOX::TargetPet::GetSta);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPotential", &SCRIPT_SANDBOX::TargetPet::GetPotential);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetExp", &SCRIPT_SANDBOX::TargetPet::GetExp);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetNearAttack", &SCRIPT_SANDBOX::TargetPet::GetNearAttack);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetNearRecovery", &SCRIPT_SANDBOX::TargetPet::GetNearRecovery);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetFarAttack", &SCRIPT_SANDBOX::TargetPet::GetFarAttack);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetFarRecovery", &SCRIPT_SANDBOX::TargetPet::GetFarRecovery);

//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsAttack", &SCRIPT_SANDBOX::TargetPet::GetPhysicsAttack);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMagicAttack", &SCRIPT_SANDBOX::TargetPet::GetMagicAttack);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsRecovery", &SCRIPT_SANDBOX::TargetPet::GetPhysicsRecovery);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMagicRecovery", &SCRIPT_SANDBOX::TargetPet::GetMagicRecovery);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMiss", &SCRIPT_SANDBOX::TargetPet::GetMiss);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetShootProbability", &SCRIPT_SANDBOX::TargetPet::GetShootProbability);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Add_Attribute", &SCRIPT_SANDBOX::TargetPet::Add_Attribute);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Change_Name", &SCRIPT_SANDBOX::TargetPet::Change_Name);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetIsFighting", &SCRIPT_SANDBOX::TargetPet::GetIsFighting);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("SetModel", &SCRIPT_SANDBOX::TargetPet::SetModel);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("CopyMyPet", &SCRIPT_SANDBOX::TargetPet::CopyMyPet);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetDeathWarrant", &SCRIPT_SANDBOX::TargetPet::GetDeathWarrant);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetAIType", &SCRIPT_SANDBOX::TargetPet::GetAIType);



	LuaObject obj_TargetPet = pState->BoxPointer(&(SCRIPT_SANDBOX::TargetPet::s_TargetPet));
	obj_TargetPet.SetMetaTable(*SCRIPT_SANDBOX::TargetPet::s_pMetaTable);
	objGlobal.SetObject("TargetPet", obj_TargetPet);

	//---------------------------------------------------------------------------------
	//ע������Լ�������
	SCRIPT_SANDBOX::Bank::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Bank::s_pMetaTable = objGlobal.CreateTable("BankTable");
	SCRIPT_SANDBOX::Bank::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Bank::s_pMetaTable);

	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::Bank::IsPresent);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("OpenSaveFrame", &SCRIPT_SANDBOX::Bank::OpenSaveFrame);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("OpenGetFrame", &SCRIPT_SANDBOX::Bank::OpenGetFrame);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetInputMoney", &SCRIPT_SANDBOX::Bank::GetInputMoney);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("SaveMoneyToBank", &SCRIPT_SANDBOX::Bank::SaveMoneyToBank);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetMoneyFromBank", &SCRIPT_SANDBOX::Bank::GetMoneyFromBank);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetBankMoney", &SCRIPT_SANDBOX::Bank::GetBankMoney);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("MoveItemToPacket", &SCRIPT_SANDBOX::Bank::MoveItemToPacket);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetRentBoxNum", &SCRIPT_SANDBOX::Bank::GetRentBoxNum);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetRentBoxInfo", &SCRIPT_SANDBOX::Bank::GetRentBoxInfo);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("SetCurRentIndex", &SCRIPT_SANDBOX::Bank::SetCurRentIndex);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("TransformCoin", &SCRIPT_SANDBOX::Bank::TransformCoin);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("Close", &SCRIPT_SANDBOX::Bank::Close);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::Bank::EnumItem);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("OpenInputFrame", &SCRIPT_SANDBOX::Bank::OpenInputFrame);

	LuaObject obj_Bank = pState->BoxPointer(&(SCRIPT_SANDBOX::Bank::s_Bank));
	obj_Bank.SetMetaTable(*SCRIPT_SANDBOX::Bank::s_pMetaTable);
	objGlobal.SetObject("Bank", obj_Bank);


	//---------------------------------------------------------------------------------
	//ע�����֮��Ľ���
	SCRIPT_SANDBOX::Exchange::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Exchange::s_pMetaTable = objGlobal.CreateTable("ExchangeTable");
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Exchange::s_pMetaTable);

	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::Exchange::IsPresent);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("SendExchangeApply", &SCRIPT_SANDBOX::Exchange::SendExchangeApply);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("OpenExchangeFrame", &SCRIPT_SANDBOX::Exchange::OpenExchangeFrame);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetItemNum", &SCRIPT_SANDBOX::Exchange::GetItemNum);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetItemAction", &SCRIPT_SANDBOX::Exchange::GetItemAction);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetMoney", &SCRIPT_SANDBOX::Exchange::GetMoney);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("LockExchange", &SCRIPT_SANDBOX::Exchange::LockExchange);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("IsLocked", &SCRIPT_SANDBOX::Exchange::IsLocked);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("AcceptExchange", &SCRIPT_SANDBOX::Exchange::AcceptExchange);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("ExchangeCancel", &SCRIPT_SANDBOX::Exchange::ExchangeCancel);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("OpenPetFrame", &SCRIPT_SANDBOX::Exchange::OpenPetFrame);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetMoneyFromInput", &SCRIPT_SANDBOX::Exchange::GetMoneyFromInput);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("IsStillAnyAppInList", &SCRIPT_SANDBOX::Exchange::IsStillAnyAppInList);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetOthersName", &SCRIPT_SANDBOX::Exchange::GetOthersName);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("AddPet", &SCRIPT_SANDBOX::Exchange::AddPet);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetPetNum", &SCRIPT_SANDBOX::Exchange::GetPetNum);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::Exchange::EnumPet);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("DelSelectPet", &SCRIPT_SANDBOX::Exchange::DelSelectPet);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("ViewPetDesc", &SCRIPT_SANDBOX::Exchange::ViewPetDesc);

	LuaObject obj_Exchange = pState->BoxPointer(&(SCRIPT_SANDBOX::Exchange::s_Exchange));
	obj_Exchange.SetMetaTable(*SCRIPT_SANDBOX::Exchange::s_pMetaTable);
	objGlobal.SetObject("Exchange", obj_Exchange);

	//---------------------------------------------------------------------------------
	// ע�������
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::LifeAbility::s_pMetaTable = objGlobal.CreateTable("LifeAbilityTable");
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::LifeAbility::s_pMetaTable);

//	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetEquip_GemBore_Count", &SCRIPT_SANDBOX::LifeAbility::GetEquip_GemBore_Count);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetLifeAbility_Data", &SCRIPT_SANDBOX::LifeAbility::GetLifeAbility_Data);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetActionID_ByAbilityID", &SCRIPT_SANDBOX::LifeAbility::GetActionID_ByAbilityID);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetLifeAbility_Number", &SCRIPT_SANDBOX::LifeAbility::GetLifeAbility_Number);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Material_Number", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Material_Number);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Material", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Material);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Item_FromNum", &SCRIPT_SANDBOX::LifeAbility::GetPrescrList_Item_FromNum);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Item_Maximum", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Item_Maximum);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Material_Hold_Count", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Material_Hold_Count);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Update_Synthesize", &SCRIPT_SANDBOX::LifeAbility::Update_Synthesize);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Open_Compose_Gem_Page", &SCRIPT_SANDBOX::LifeAbility::Open_Compose_Gem_Page);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Do_Enchase", &SCRIPT_SANDBOX::LifeAbility::Do_Enchase);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Do_Combine", &SCRIPT_SANDBOX::LifeAbility::Do_Combine);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetEquip_Gem", &SCRIPT_SANDBOX::LifeAbility::GetEquip_Gem);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetEquip_GemCount", &SCRIPT_SANDBOX::LifeAbility::GetEquip_GemCount);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Enchase_Preparation", &SCRIPT_SANDBOX::LifeAbility::Enchase_Preparation);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Compound_Preparation", &SCRIPT_SANDBOX::LifeAbility::Compound_Preparation);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Can_Enchase", &SCRIPT_SANDBOX::LifeAbility::Can_Enchase);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Can_Combine", &SCRIPT_SANDBOX::LifeAbility::Can_Combine);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Lock_Packet_Item", &SCRIPT_SANDBOX::LifeAbility::Lock_Packet_Item);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Explain", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Explain);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetNpcId", &SCRIPT_SANDBOX::LifeAbility::GetNpcId);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetGemActionInfo", &SCRIPT_SANDBOX::LifeAbility::GetGemActionInfo);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("SetPrecalcInfo", &SCRIPT_SANDBOX::LifeAbility::SetPrecalcInfo);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Item_RequirementData", &SCRIPT_SANDBOX::LifeAbility::GetPrescrList_Item_RequirementData);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetRequirementNum_ById", &SCRIPT_SANDBOX::LifeAbility::GetRequirementNum_ById);

	
	LuaObject obj_LifeAbility = pState->BoxPointer(&(SCRIPT_SANDBOX::LifeAbility::s_LifeAbility));
	obj_LifeAbility.SetMetaTable(*SCRIPT_SANDBOX::LifeAbility::s_pMetaTable);
	objGlobal.SetObject("LifeAbility", obj_LifeAbility);

	//---------------------------------------------------------------------------------
	// ������
	SCRIPT_SANDBOX::Guild::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Guild::s_pMetaTable = objGlobal.CreateTable("GuildTable");
	SCRIPT_SANDBOX::Guild::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Guild::s_pMetaTable);

	// �������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("CreateGuild",					&SCRIPT_SANDBOX::Guild::CreateGuild);
	// �������ȷ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("CreateGuildConfirm",			&SCRIPT_SANDBOX::Guild::CreateGuildConfirm);
	// ��World��������ϸ��Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AskGuildDetailInfo",			&SCRIPT_SANDBOX::Guild::AskGuildDetailInfo);
	// ��World�������Ա��Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AskGuildMembersInfo",			&SCRIPT_SANDBOX::Guild::AskGuildMembersInfo);
	// ��World������ְλ��Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AskGuildAppointPosInfo",		&SCRIPT_SANDBOX::Guild::AskGuildAppointPosInfo);
	// ������а�������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetGuildNum",					&SCRIPT_SANDBOX::Guild::GetGuildNum);
	// �������������Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetGuildInfo",					&SCRIPT_SANDBOX::Guild::GetGuildInfo);
	// ������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("JoinGuild",						&SCRIPT_SANDBOX::Guild::JoinGuild);
	// ����Լ��İ�����Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMembersNum",					&SCRIPT_SANDBOX::Guild::GetMembersNum);
	// ����Լ��İ�����Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMembersInfo",				&SCRIPT_SANDBOX::Guild::GetMembersInfo);
	// ����Լ��İ�����Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMyGuildInfo",				&SCRIPT_SANDBOX::Guild::GetMyGuildInfo);
	// ����Լ����ɵ���ϸ��Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMyGuildDetailInfo",			&SCRIPT_SANDBOX::Guild::GetMyGuildDetailInfo);
	// ����Լ��İ���Ȩ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMyGuildPower",				&SCRIPT_SANDBOX::Guild::GetMyGuildPower);
	// �߳����,�ܾ�����
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("KickGuild",						&SCRIPT_SANDBOX::Guild::KickGuild);
	// ���ɻ�Ա
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("RecruitGuild",					&SCRIPT_SANDBOX::Guild::RecruitGuild);
	// �˳����
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("QuitGuild",						&SCRIPT_SANDBOX::Guild::QuitGuild);
	// �޸����а���ְλ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AdjustMemberAuth",				&SCRIPT_SANDBOX::Guild::AdjustMemberAuth);
	// �������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("DemisGuild",					&SCRIPT_SANDBOX::Guild::DemisGuild);
	// ���ɾ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("DestoryGuild",					&SCRIPT_SANDBOX::Guild::DestoryGuild);
	// �޸İ����Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("FixGuildInfo",					&SCRIPT_SANDBOX::Guild::FixGuildInfo);
	// �������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GuildInvite",					&SCRIPT_SANDBOX::Guild::GuildInvite);
	// �޸ĳ�Ա�ĳƺ�
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AdjustMemberTitle",				&SCRIPT_SANDBOX::Guild::AdjustMemberTitle);
	// ɾ������
//	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("DestoryFamily",					&SCRIPT_SANDBOX::Guild::DestoryFamily);
	// ͬ������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AgreeInvite",					&SCRIPT_SANDBOX::Guild::AgreeInvite);
	// ��ʾ����Ի���
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("ShowInputDialog",				&SCRIPT_SANDBOX::Guild::ShowInputDialog);

	// ��Ա�б��ѡ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("SetMemberListSelect",			&SCRIPT_SANDBOX::Guild::SetMemberListSelect);

	// �����صĲ˵�
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("OpenMenu",						&SCRIPT_SANDBOX::Guild::OpenMenu);


	// ��������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("CreateFamily",					&SCRIPT_SANDBOX::Guild::CreateFamily);
	// ��������ȷ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("CreateFamilyConfirm",			&SCRIPT_SANDBOX::Guild::CreateFamilyConfirm);
	// ����ɾ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("DestoryFamilyConfirm",			&SCRIPT_SANDBOX::Guild::DestoryFamilyConfirm );
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("DestoryFamily",					&SCRIPT_SANDBOX::Guild::DestoryFamily);
	// ��ü���ĳ�Ա��ϸ��Ϣ
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetFamilyMembersInfo",			&SCRIPT_SANDBOX::Guild::GetFamilyMembersInfo);
	// �Լ����Ա���в�������ӻ�ɾ����
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AdjustFamilyMember",			&SCRIPT_SANDBOX::Guild::AdjustFamilyMember);
	// �ƶ���Ա������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("TransferMemberToFamily",		&SCRIPT_SANDBOX::Guild::TransferMemberToFamily);
	// �Ӽ������߳�
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("KickMemberFormFamily",			&SCRIPT_SANDBOX::Guild::KickMemberFormFamily);
	// ��������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("InviteMemberToFamily",			&SCRIPT_SANDBOX::Guild::InviteMemberToFamily);
	// ��������ȷ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("FamilyInviteConfirm",			&SCRIPT_SANDBOX::Guild::FamilyInviteConfirm);
	// ��ȡ��������
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetFamilyMemberNum",			&SCRIPT_SANDBOX::Guild::GetFamilyMemberNum);
	// ��ȡ�����Ա�ڳ�Ա�б��е�����
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetFamilyMemberIndex",			&SCRIPT_SANDBOX::Guild::GetFamilyMemberIndex);
	// 
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("SetLuaFamilyNum",				&SCRIPT_SANDBOX::Guild::SetLuaFamilyNum);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetLuaFamilyNum",				&SCRIPT_SANDBOX::Guild::GetLuaFamilyNum);

	// ׼������Ա����
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("PrepareMembersInfomation",		&SCRIPT_SANDBOX::Guild::PrepareMembersInfomation);
	// Lua��ʾ��list�ؼ���λ��
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetShowMembersIdx",				&SCRIPT_SANDBOX::Guild::GetShowMembersIdx);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetShowTraineesIdx",			&SCRIPT_SANDBOX::Guild::GetShowTraineesIdx);

	LuaObject obj_Guild = pState->BoxPointer(&(SCRIPT_SANDBOX::Guild::s_Guild));
	obj_Guild.SetMetaTable(*SCRIPT_SANDBOX::Guild::s_pMetaTable);
	objGlobal.SetObject("Guild", obj_Guild);



	//---------------------------------------------------------------------------------
	// ����

	SCRIPT_SANDBOX::Country::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Country::s_pMetaTable = objGlobal.CreateTable("CountryTable");
	SCRIPT_SANDBOX::Country::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Country::s_pMetaTable);


	// ��������
	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("CreateCountryKing",			&SCRIPT_SANDBOX::Country::CreateCountryKing );
	// ȷ����������
	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("CreateCountryKingConfirm",	&SCRIPT_SANDBOX::Country::CreateCountryKingConfirm );
	// ������ҹ�Ա��Ϣ
	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("RequestCountryFunctionary",	&SCRIPT_SANDBOX::Country::RequestCountryFunctionary );
	// ��ȡ���ҹ�Ա��Ϣ
	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("GetCountryFunctionary",		&SCRIPT_SANDBOX::Country::GetCountryFunctionary );
	// �޸����й��ҹ�Աְλ
//	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("AdjustFunctionaryAuth",		&SCRIPT_SANDBOX::Country::AdjustFunctionaryAuth );
	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("AppointFunctionary",			&SCRIPT_SANDBOX::Country::AppointFunctionary );
	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("DismissFunctionary",			&SCRIPT_SANDBOX::Country::DismissFunctionary );
	// �����ʵ�
	SCRIPT_SANDBOX::Country::s_pMetaTable->RegisterObjectFunctor("CreateMonarch",				&SCRIPT_SANDBOX::Country::CreateMonarch );


	LuaObject obj_Country = pState->BoxPointer( &(SCRIPT_SANDBOX::Country::s_Country) );
	obj_Country.SetMetaTable( *SCRIPT_SANDBOX::Country::s_pMetaTable );
	objGlobal.SetObject("Country", obj_Country);




	//---------------------------------------------------------------------------------
	//��Ұ���
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable = objGlobal.CreateTable("PlayerPackage");
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable);

	// ��ѯ���������е���Ʒ
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::PlayerPackage::EnumItem);
	// �򿪰�̯�����ۣ�����
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("OpenStallSaleFrame", &SCRIPT_SANDBOX::PlayerPackage::OpenStallSaleFrame);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("GetSplitSum", &SCRIPT_SANDBOX::PlayerPackage::GetSplitSum);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("SplitItem", &SCRIPT_SANDBOX::PlayerPackage::SplitItem);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("CancelSplitItem", &SCRIPT_SANDBOX::PlayerPackage::CancelSplitItem);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("PackUpPacket", &SCRIPT_SANDBOX::PlayerPackage::PackUpPacket);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("GetSplitName", &SCRIPT_SANDBOX::PlayerPackage::GetSplitName);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("OpenPetList", &SCRIPT_SANDBOX::PlayerPackage::OpenPetList);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("OpenLockFrame", &SCRIPT_SANDBOX::PlayerPackage::OpenLockFrame);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("Lock", &SCRIPT_SANDBOX::PlayerPackage::Lock);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("IsLock", &SCRIPT_SANDBOX::PlayerPackage::IsLock);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("ItemLink", &SCRIPT_SANDBOX::PlayerPackage::ItemLink);
 

	//ȷ�����
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("AddHoleItem", &SCRIPT_SANDBOX::PlayerPackage::AddHoleItem);
	//ȡ�����
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("CancelAddHoleItem", &SCRIPT_SANDBOX::PlayerPackage::CancelAddHoleItem);
	//ȷ���� 
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("Bind_Equip", &SCRIPT_SANDBOX::PlayerPackage::Bind_Equip);
	
	//ȡ����
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("CancelBindItem", &SCRIPT_SANDBOX::PlayerPackage::CancelBindItem);
   
	 //ȷ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("AddStar_Equip", &SCRIPT_SANDBOX::PlayerPackage::AddStar_Equip);

	//ȡ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("Cancel_AddStart_Equip", &SCRIPT_SANDBOX::PlayerPackage::Cancel_AddStart_Equip);


	//ȷ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("SpecialRepair_Equip", &SCRIPT_SANDBOX::PlayerPackage::SpecialRepair_Equip);
	//ȡ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("Cancel_SpecialRepair_Equip", &SCRIPT_SANDBOX::PlayerPackage::Cancel_SpecialRepair_Equip);

	//ȷ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("SureRepair_Equip", &SCRIPT_SANDBOX::PlayerPackage::SureRepair_Equip);


	//ȷ���ϳ�װ��
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("EquipSynthesize_Sure", &SCRIPT_SANDBOX::PlayerPackage::EquipSynthesize_Sure);



	//ȷ������װ��
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("ReWorkEquip", &SCRIPT_SANDBOX::PlayerPackage::ReWorkEquip);

	//ȡ������װ��
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("ReWorkEquipCannel", &SCRIPT_SANDBOX::PlayerPackage::ReWorkEquipCannel);

	//ȷ�����װ��
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("EquipDismantleSure", &SCRIPT_SANDBOX::PlayerPackage::EquipDismantleSure);

	//ȡ�����װ��
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("EquipDismantleCannel", &SCRIPT_SANDBOX::PlayerPackage::EquipDismantleCannel);

	//ȡ������װ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("CancelSwapCarditem", &SCRIPT_SANDBOX::PlayerPackage::CancelSwapCarditem);

	//ȡ������װ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("CancelChangeCarditem", &SCRIPT_SANDBOX::PlayerPackage::CancelChangeCarditem);

	//ȷ������װ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("SureSwapCarditem", &SCRIPT_SANDBOX::PlayerPackage::SureSwapCarditem);

	//ȷ������װ������
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("SureChangeCarditem", &SCRIPT_SANDBOX::PlayerPackage::SureChangeCarditem);




	LuaObject obj_PlayerPackage = pState->BoxPointer(&(SCRIPT_SANDBOX::PlayerPackage::s_PlayerPackage));
	obj_PlayerPackage.SetMetaTable(*SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable);
	objGlobal.SetObject("PlayerPackage", obj_PlayerPackage);

	//---------------------------------------------------------------------------------
	//Tooltip
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable = objGlobal.CreateTable("SuperTooltips");
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable);

	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetTitle",	   &SCRIPT_SANDBOX::SuperTooltips::GetTitle);

	// �õ�����ͼ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetIcon",	   &SCRIPT_SANDBOX::SuperTooltips::GetIcon);

	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemDesc",  &SCRIPT_SANDBOX::SuperTooltips::GetItemDesc);
	
	// �õ�ʹ����Ʒ��Ҫ�ĵȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemLevel", &SCRIPT_SANDBOX::SuperTooltips::GetItemLevel);

	// �õ���Ʒ�;�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemDur", &SCRIPT_SANDBOX::SuperTooltips::GetItemDur);

	// �õ���Ʒ�ļ۸�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemPrice", &SCRIPT_SANDBOX::SuperTooltips::GetItemPrice);

	// �õ�װ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipCard", &SCRIPT_SANDBOX::SuperTooltips::GetEquipCard);


	// �õ���Ʒ��ְҵ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemProfession", &SCRIPT_SANDBOX::SuperTooltips::GetItemProfession);

	// �õ���Ʒ�Ĺ�����Ƿ�Ψһ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemRuleOnly", &SCRIPT_SANDBOX::SuperTooltips::GetItemRuleOnly);

	// �õ���Ʒ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemManufacturer", &SCRIPT_SANDBOX::SuperTooltips::GetItemManufacturer);

	// �õ���Ʒ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemRepairCount", &SCRIPT_SANDBOX::SuperTooltips::GetItemRepairCount);

	// �õ�װ���Ŀ�����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipHoleNum", &SCRIPT_SANDBOX::SuperTooltips::GetEquipHoleNum);

	// �õ�װ������װ��Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipSetInfo", &SCRIPT_SANDBOX::SuperTooltips::GetEquipSetInfo);

	//�õ�װ�����ͼ������Ϣ 
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipSpritePaiInfo", &SCRIPT_SANDBOX::SuperTooltips::GetEquipSpritePaiInfo);


	//�õ�װ��С�������Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipSpriteChainInfo", &SCRIPT_SANDBOX::SuperTooltips::GetEquipSpriteChainInfo);

	//�õ��������װ��С�������Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetOtherEquipSpriteChainInfo", &SCRIPT_SANDBOX::SuperTooltips::GetOtherEquipSpriteChainInfo);

	//�õ��������װ���󼤻����Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetOtherEquipSpriteAllActiveInfo", &SCRIPT_SANDBOX::SuperTooltips::GetOtherEquipSpriteAllActiveInfo);

	//�õ�װ���󼤻����Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipSpriteAllActiveInfo", &SCRIPT_SANDBOX::SuperTooltips::GetEquipSpriteAllActiveInfo);
 
	// �õ��������װ������װ��Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetOtherEquipSetInfo", &SCRIPT_SANDBOX::SuperTooltips::GetOtherEquipSetInfo);


	// �õ���װ������װ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipSetActiveAttr", &SCRIPT_SANDBOX::SuperTooltips::GetEquipSetActiveAttr);

	// �õ��������װ������װ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetOtherEquipSetActiveAttr", &SCRIPT_SANDBOX::SuperTooltips::GetOtherEquipSetActiveAttr);


	// �õ�װ����������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetStartLevel", &SCRIPT_SANDBOX::SuperTooltips::GetStartLevel);

	//�õ�����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetAttributeValue", &SCRIPT_SANDBOX::SuperTooltips::GetAttributeValue);

	//�õ��������� 
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetTypeDesc", &SCRIPT_SANDBOX::SuperTooltips::GetTypeDesc);
	
	// �õ���Ʒ�İ���Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemBindInfo", &SCRIPT_SANDBOX::SuperTooltips::GetItemBindInfo);

	// �õ�װ����2�ΰ���Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemSecLevelBindInfo", &SCRIPT_SANDBOX::SuperTooltips::GetItemSecLevelBindInfo);

	// �õ�װ����2�ΰ󶨼ӳ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemBindAddInfo", &SCRIPT_SANDBOX::SuperTooltips::GetItemBindAddInfo);
	// �õ�װ������ɫ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetDropVisColor", &SCRIPT_SANDBOX::SuperTooltips::GetDropVisColor);

	// �õ�װ����Ʒ�Ļ������� 
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipBaseAttrInfo", &SCRIPT_SANDBOX::SuperTooltips::GetEquipBaseAttrInfo);

	// �õ�װ����Ʒ�ĸ������� 
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipAddAttrInfo", &SCRIPT_SANDBOX::SuperTooltips::GetEquipAddAttrInfo);
	
	// �õ�װ���ļ�������    

	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipSkillInfo", &SCRIPT_SANDBOX::SuperTooltips::GetEquipSkillInfo);

	// �õ���Ʒ������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemType", &SCRIPT_SANDBOX::SuperTooltips::GetItemType);


	// �õ���Ʒ����������(��װ��������������Ʒ)
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemClassType", &SCRIPT_SANDBOX::SuperTooltips::GetItemClassType);

	// �õ�װ���ϵı�ʯ��Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetGemInEquipInfo", &SCRIPT_SANDBOX::SuperTooltips::GetGemInEquipInfo);

	// �õ��õ��ķ��ȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetXinfaLevel", &SCRIPT_SANDBOX::SuperTooltips::GetXinfaLevel);

	// �Ƿ�ѧ�Ἴ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetIsLearnedSkill", &SCRIPT_SANDBOX::SuperTooltips::GetIsLearnedSkill);


	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLevel", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLevel);

	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsPassiveSkill", &SCRIPT_SANDBOX::SuperTooltips::IsPassiveSkill);
	

	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillDesc", &SCRIPT_SANDBOX::SuperTooltips::GetSkillDesc);

	// �õ���ʯ�ȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetGemLevel", &SCRIPT_SANDBOX::SuperTooltips::GetGemLevel);

	// �õ���ʯ����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetGemArribInfo", &SCRIPT_SANDBOX::SuperTooltips::GetGemArribInfo);

	// �õ���ʯ����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetNeedGemInfo", &SCRIPT_SANDBOX::SuperTooltips::GetNeedGemInfo);

	// �õ�������������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetFecthHeadInfo_TypeDesc", &SCRIPT_SANDBOX::SuperTooltips::GetFecthHeadInfo_TypeDesc);

	// �õ����������ֵ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetFecthHeadInfo_Value", &SCRIPT_SANDBOX::SuperTooltips::GetFecthHeadInfo_Value);

	// �õ��������������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetFecthHeadInfo_Num", &SCRIPT_SANDBOX::SuperTooltips::GetFecthHeadInfo_Num);


	// �õ�װ���Ƿ���ͨ���߸߼�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemEquipQua", &SCRIPT_SANDBOX::SuperTooltips::GetItemEquipQua);

	//ҩƷ��ʯƷ�� ����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemQuality", &SCRIPT_SANDBOX::SuperTooltips::GetItemQuality);

	// �õ�װ��������������
	//SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipMaxRepair", &SCRIPT_SANDBOX::SuperTooltips::GetEquipMaxRepair);

	// �õ������ķ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillXinfaLevel", &SCRIPT_SANDBOX::SuperTooltips::GetSkillXinfaLevel);

	// �õ�����Ʒ���е����͡�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemTableType", &SCRIPT_SANDBOX::SuperTooltips::GetItemTableType);

	// �õ��䷽��Ҫ�ļ��ܵ����ơ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetPrescrSkillName", &SCRIPT_SANDBOX::SuperTooltips::GetPrescrSkillName);

	// �õ��䷽��Ҫ�ļ��ܵĵȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetPrescrNeedSkillLevel", &SCRIPT_SANDBOX::SuperTooltips::GetPrescrNeedSkillLevel);

	// �õ��䷽��Ҫ�ļ��ܵĵ�ǰ�ĵȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetPrescrNeedSkillCurLevel", &SCRIPT_SANDBOX::SuperTooltips::GetPrescrNeedSkillCurLevel);

	// ����������Ʒ����ϸ��Ϣ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("SendAskItemInfoMsg", &SCRIPT_SANDBOX::SuperTooltips::SendAskItemInfoMsg);

	// �õ�װ���ļ�����Ϣ  
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsEquipIdentify", &SCRIPT_SANDBOX::SuperTooltips::IsEquipIdentify);

	// NPC �̵��Ƿ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsNpcShopOpen", &SCRIPT_SANDBOX::SuperTooltips::IsNpcShopOpen);
	
	//npc�Ƿ����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::SuperTooltips::IsPresent);


	//�õ���Ʒ�Ƿ��ڱ�����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsItemInPacket", &SCRIPT_SANDBOX::SuperTooltips::IsItemInPacket);

	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemIsOnStall", &SCRIPT_SANDBOX::SuperTooltips::GetItemIsOnStall);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemPriceOnStall", &SCRIPT_SANDBOX::SuperTooltips::GetItemPriceOnStall);


	//��д���պ�������֤��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillMaxLevel", &SCRIPT_SANDBOX::SuperTooltips::GetSkillMaxLevel);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillType", &SCRIPT_SANDBOX::SuperTooltips::GetSkillType);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillCoolDown", &SCRIPT_SANDBOX::SuperTooltips::GetSkillCoolDown);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillAttackRange", &SCRIPT_SANDBOX::SuperTooltips::GetSkillAttackRange);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillGatherTime", &SCRIPT_SANDBOX::SuperTooltips::GetSkillGatherTime);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLeadTime", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLeadTime);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetNextLvSkillDesc", &SCRIPT_SANDBOX::SuperTooltips::GetNextLvSkillDesc);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetNextLvSkillGatherTime", &SCRIPT_SANDBOX::SuperTooltips::GetNextLvSkillGatherTime);

	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetNextLvSkillLeadTime", &SCRIPT_SANDBOX::SuperTooltips::GetNextLvSkillLeadTime);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLvUpLevelQua", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLvUpLevelQua);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLvUpPointQua", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLvUpPointQua);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLvUpFahterSkillQua", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLvUpFahterSkillQua);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLvUpMoneyQua", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLvUpMoneyQua);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLeadTime", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLeadTime);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLvUpXpQua", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLvUpXpQua);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLvUpItemQua", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLvUpItemQua);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsSouXiaItem", &SCRIPT_SANDBOX::SuperTooltips::IsSouXiaItem);
	
	//�õ����������ٶ�          [7/21/2010 �¾���]
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetMountAddtionSpeed", &SCRIPT_SANDBOX::SuperTooltips::GetMountAddtionSpeed);
	//�õ��������ʱ��          [7/21/2010 �¾���]
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetMountLife", &SCRIPT_SANDBOX::SuperTooltips::GetMountLife);
	//�õ������ȡʱ��          [7/21/2010 �¾���]
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetMountObtainDate", &SCRIPT_SANDBOX::SuperTooltips::GetMountObtainDate);
	//�õ�����ʣ��ʹ��ʱ��      [7/21/2010 �¾���]
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetMountRemainTime", &SCRIPT_SANDBOX::SuperTooltips::GetMountRemainTime);
	//�õ����ﵱǰ�Ƿ��Ѱ�    [7/21/2010 �¾���]
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetMountIsBinded", &SCRIPT_SANDBOX::SuperTooltips::GetMountIsBinded);
	// �õ����������         [7/21/2010 �¾���]
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetMountBindType", &SCRIPT_SANDBOX::SuperTooltips::GetMountBindType);


	
	LuaObject obj_SuperTooltips = pState->BoxPointer(&(SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip));
	obj_SuperTooltips.SetMetaTable(*SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable);
	objGlobal.SetObject("SuperTooltips", obj_SuperTooltips);

	//��Ʒ����ϵͳ�õ�lua����
	LuaObject obj_SuperTooltips2 = pState->BoxPointer(&(SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip2));
	obj_SuperTooltips2.SetMetaTable(*SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable);
	objGlobal.SetObject("SuperTooltips2", obj_SuperTooltips2);	
	
	//---------------------------------------------------------------------------------
	//��̯���(Sale)
	SCRIPT_SANDBOX::StallSale::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::StallSale::s_pMetaTable = objGlobal.CreateTable("StallSale");
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::StallSale::s_pMetaTable);

	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::StallSale::IsPresent);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetPosTax", &SCRIPT_SANDBOX::StallSale::GetPosTax);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetTradeTax", &SCRIPT_SANDBOX::StallSale::GetTradeTax);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ReferItemPrice", &SCRIPT_SANDBOX::StallSale::ReferItemPrice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ModifStallName", &SCRIPT_SANDBOX::StallSale::ModifStallName);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ModifItemPrice", &SCRIPT_SANDBOX::StallSale::ModifItemPrice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ApplyCloseStall", &SCRIPT_SANDBOX::StallSale::ApplyCloseStall);	// add by gh 2010/04/15
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("SetSaveStates", &SCRIPT_SANDBOX::StallSale::SetSaveStates);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("CloseStall", &SCRIPT_SANDBOX::StallSale::CloseStall);

	// ���̯λ����		20100706 BLL
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("StallOpen", &SCRIPT_SANDBOX::StallSale::StallOpen);

	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ItemReprice", &SCRIPT_SANDBOX::StallSale::ItemReprice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("AgreeBeginStall", &SCRIPT_SANDBOX::StallSale::AgreeBeginStall);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("DeleteItem", &SCRIPT_SANDBOX::StallSale::DeleteItem);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetPrice", &SCRIPT_SANDBOX::StallSale::GetPrice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("OpenMessageSale", &SCRIPT_SANDBOX::StallSale::OpenMessageSale);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ApplyAd", &SCRIPT_SANDBOX::StallSale::ApplyAd);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("CloseStallMessage", &SCRIPT_SANDBOX::StallSale::CloseStallMessage);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetStallName", &SCRIPT_SANDBOX::StallSale::GetStallName);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetAdvertise", &SCRIPT_SANDBOX::StallSale::GetAdvertise);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetGuid", &SCRIPT_SANDBOX::StallSale::GetGuid);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetPetNum", &SCRIPT_SANDBOX::StallSale::GetPetNum);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::StallSale::EnumPet);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("PetUpStall", &SCRIPT_SANDBOX::StallSale::PetUpStall);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("PetReprice", &SCRIPT_SANDBOX::StallSale::PetReprice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetDefaultPage", &SCRIPT_SANDBOX::StallSale::GetDefaultPage);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("SetDefaultPage", &SCRIPT_SANDBOX::StallSale::SetDefaultPage);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ViewPetDesc", &SCRIPT_SANDBOX::StallSale::ViewPetDesc);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("SetSelectPet", &SCRIPT_SANDBOX::StallSale::SetSelectPet);


	LuaObject obj_StallSale = pState->BoxPointer(&(SCRIPT_SANDBOX::StallSale::s_StallSale));
	obj_StallSale.SetMetaTable(*SCRIPT_SANDBOX::StallSale::s_pMetaTable);
	objGlobal.SetObject("StallSale", obj_StallSale);

	//---------------------------------------------------------------------------------
	//��̯���(Buy)
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::StallBuy::s_pMetaTable = objGlobal.CreateTable("StallBuy");
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::StallBuy::s_pMetaTable);

	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::StallBuy::IsPresent);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetTradeTax", &SCRIPT_SANDBOX::StallBuy::GetTradeTax);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("OpenStall", &SCRIPT_SANDBOX::StallBuy::OpenStall);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetPrice", &SCRIPT_SANDBOX::StallBuy::GetPrice);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("BuyItem", &SCRIPT_SANDBOX::StallBuy::BuyItem);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("OpenMessageBuy", &SCRIPT_SANDBOX::StallBuy::OpenMessageBuy);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetStallName", &SCRIPT_SANDBOX::StallBuy::GetStallName);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetAdvertise", &SCRIPT_SANDBOX::StallBuy::GetAdvertise);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("CloseStallMessage", &SCRIPT_SANDBOX::StallBuy::CloseStallMessage);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetGuid", &SCRIPT_SANDBOX::StallBuy::GetGuid);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetStallerName", &SCRIPT_SANDBOX::StallBuy::GetStallerName);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetPetNum", &SCRIPT_SANDBOX::StallBuy::GetPetNum);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::StallBuy::EnumPet);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetDefaultPage", &SCRIPT_SANDBOX::StallBuy::GetDefaultPage);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("BuyPet", &SCRIPT_SANDBOX::StallBuy::BuyPet);

	LuaObject obj_StallBuy = pState->BoxPointer(&(SCRIPT_SANDBOX::StallBuy::s_StallBuy));
	obj_StallBuy.SetMetaTable(*SCRIPT_SANDBOX::StallBuy::s_pMetaTable);
	objGlobal.SetObject("StallBuy", obj_StallBuy);

	//---------------------------------------------------------------------------------
	//��̯��Ϣ
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::StallBbs::s_pMetaTable = objGlobal.CreateTable("StallBbs");
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::StallBbs::s_pMetaTable);

	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("GetMessageNum", &SCRIPT_SANDBOX::StallBbs::GetMessageNum);
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("EnumMessage", &SCRIPT_SANDBOX::StallBbs::EnumMessage);
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("AddMessage", &SCRIPT_SANDBOX::StallBbs::AddMessage);
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("ReplyMessage", &SCRIPT_SANDBOX::StallBbs::ReplyMessage);

	LuaObject obj_StallBbs = pState->BoxPointer(&(SCRIPT_SANDBOX::StallBbs::s_StallBbs));
	obj_StallBbs.SetMetaTable(*SCRIPT_SANDBOX::StallBbs::s_pMetaTable);
	objGlobal.SetObject("StallBbs", obj_StallBbs);

	//---------------------------------------------------------------------------------
	//�����ύ
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::MissionReply::s_pMetaTable = objGlobal.CreateTable("MissionReply");
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::MissionReply::s_pMetaTable);

	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::MissionReply::IsPresent);
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("OpenPetFrame", &SCRIPT_SANDBOX::MissionReply::OpenPetFrame);
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("OnContinue", &SCRIPT_SANDBOX::MissionReply::OnContinue);
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::MissionReply::EnumItem);

	LuaObject obj_MissionReply = pState->BoxPointer(&(SCRIPT_SANDBOX::MissionReply::s_MissionReply));
	obj_MissionReply.SetMetaTable(*SCRIPT_SANDBOX::MissionReply::s_pMetaTable);
	objGlobal.SetObject("MissionReply", obj_MissionReply);


	//---------------------------------------------------------------------------------
	//����ӿ�
	SCRIPT_SANDBOX::Talk::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Talk::s_pMetaTable = objGlobal.CreateTable("Talk");
	SCRIPT_SANDBOX::Talk::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Talk::s_pMetaTable);

	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannelNumber", &SCRIPT_SANDBOX::Talk::GetChannelNumber);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannel", &SCRIPT_SANDBOX::Talk::GetChannel);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannelHeader", &SCRIPT_SANDBOX::Talk::GetChannelHeader);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SendChatMessage", &SCRIPT_SANDBOX::Talk::SendChatMessage);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("InsertHistory", &SCRIPT_SANDBOX::Talk::InsertHistory);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SetMaxSaveNumber", &SCRIPT_SANDBOX::Talk::SetMaxSaveNumber);

	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("CreateTab", &SCRIPT_SANDBOX::Talk::CreateTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ConfigTab", &SCRIPT_SANDBOX::Talk::ConfigTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("CreateTabFinish", &SCRIPT_SANDBOX::Talk::CreateTabFinish);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ConfigTabFinish", &SCRIPT_SANDBOX::Talk::ConfigTabFinish);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SaveTab", &SCRIPT_SANDBOX::Talk::SaveTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ClearTab", &SCRIPT_SANDBOX::Talk::ClearTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannelType", &SCRIPT_SANDBOX::Talk::GetChannelType);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SelectFaceMotion", &SCRIPT_SANDBOX::Talk::SelectFaceMotion);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SelectTextColor", &SCRIPT_SANDBOX::Talk::SelectTextColor);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SaveOldTalkMsg", &SCRIPT_SANDBOX::Talk::SaveOldTalkMsg);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ChangeCurrentChannel", &SCRIPT_SANDBOX::Talk::ChangeCurrentChannel);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ModifyChatTxt", &SCRIPT_SANDBOX::Talk::ModifyChatTxt);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ShowContexMenu", &SCRIPT_SANDBOX::Talk::ShowContexMenu);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ContexMenuTalk", &SCRIPT_SANDBOX::Talk::ContexMenuTalk);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ContexMenuCopy", &SCRIPT_SANDBOX::Talk::ContexMenuCopy);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ChangeClipboardTxt", &SCRIPT_SANDBOX::Talk::ChangeClipboardTxt);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetCurrentSelectName", &SCRIPT_SANDBOX::Talk::GetCurrentSelectName);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetTalkTemplateString", &SCRIPT_SANDBOX::Talk::GetTalkTemplateString_Lua);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("EnumChatMood", &SCRIPT_SANDBOX::Talk::EnumChatMood);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ShowChatMood", &SCRIPT_SANDBOX::Talk::ShowChatMood);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("HideChatMood", &SCRIPT_SANDBOX::Talk::HideChatMood);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("HandleMenuAction", &SCRIPT_SANDBOX::Talk::HandleMenuAction);

	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ShowPingBi", &SCRIPT_SANDBOX::Talk::ShowPingBi);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetPingBiNum", &SCRIPT_SANDBOX::Talk::GetPingBiNum);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetPingBiName", &SCRIPT_SANDBOX::Talk::GetPingBiName);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("DelPingBi", &SCRIPT_SANDBOX::Talk::DelPingBi);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetCurInputLanguage", &SCRIPT_SANDBOX::Talk::GetCurInputLanguage);
	//SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetSendHistoryText", &SCRIPT_SANDBOX::Talk::GetSendHistoryText );
	// 
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetContexMenuObjectName", &SCRIPT_SANDBOX::Talk::GetContexMenuObjectName);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("clearAllHistoryFrame",&SCRIPT_SANDBOX::Talk::ClearAllHistoryFrame);

	LuaObject obj_Talk = pState->BoxPointer(&(SCRIPT_SANDBOX::Talk::s_Talk));
	obj_Talk.SetMetaTable(*SCRIPT_SANDBOX::Talk::s_pMetaTable);
	objGlobal.SetObject("Talk", obj_Talk);

	//---------------------------------------------------------------------------------
	//Npc Shop
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::NpcShop::s_pMetaTable = objGlobal.CreateTable("NpcShop");
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::NpcShop::s_pMetaTable);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("Close", &SCRIPT_SANDBOX::NpcShop::Close);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetNpcId", &SCRIPT_SANDBOX::NpcShop::GetNpcId);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetItemPrice", &SCRIPT_SANDBOX::NpcShop::GetItemPrice);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("NpcShopBuy", &SCRIPT_SANDBOX::NpcShop::NpcShopBuy);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("NpcShopSell", &SCRIPT_SANDBOX::NpcShop::NpcShopSell);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetItemPriceType", &SCRIPT_SANDBOX::NpcShop::GetItemPriceType);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetItemPrice", &SCRIPT_SANDBOX::NpcShop::GetItemPrice);

	LuaObject obj_NpcShop = pState->BoxPointer(&(SCRIPT_SANDBOX::NpcShop::s_NpcShop));
	obj_NpcShop.SetMetaTable(*SCRIPT_SANDBOX::NpcShop::s_pMetaTable);
	objGlobal.SetObject("NpcShop", obj_NpcShop);

	// add by gh for IBShop
	SCRIPT_SANDBOX::IBShop::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::IBShop::s_pMetaTable = objGlobal.CreateTable("IBShop");
	SCRIPT_SANDBOX::IBShop::s_pMetaTable->RegisterObjectFunctor("GetIBItemPrice", &SCRIPT_SANDBOX::IBShop::Lua_GetIBItemPrice);
	SCRIPT_SANDBOX::IBShop::s_pMetaTable->RegisterObjectFunctor("EnumActionFoIBShop", &SCRIPT_SANDBOX::IBShop::Lua_EnumActionFoIBShop);
	SCRIPT_SANDBOX::IBShop::s_pMetaTable->RegisterObjectFunctor("GetIBShopAccordNum", &SCRIPT_SANDBOX::IBShop::Lua_GetIBShopAccordNum);
	
	LuaObject obj_IBShop = pState->BoxPointer(&(SCRIPT_SANDBOX::IBShop::s_IBShop));
	obj_IBShop.SetMetaTable(*SCRIPT_SANDBOX::IBShop::s_pMetaTable);
	objGlobal.SetObject("IBShop", obj_IBShop);

	//---------------------------------------------------------------------------------
	//һ����װ
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::QuickChange::s_pMetaTable = objGlobal.CreateTable("QuickChange");
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::QuickChange::s_pMetaTable);
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable->RegisterObjectFunctor("ChangeSuit", &SCRIPT_SANDBOX::QuickChange::Lua_ChangeSuit);
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable->RegisterObjectFunctor("SaveSuitName", &SCRIPT_SANDBOX::QuickChange::Lua_SaveSuitName);
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable->RegisterObjectFunctor("SaveSuit", &SCRIPT_SANDBOX::QuickChange::Lua_SaveSuit);
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable->RegisterObjectFunctor("ReadSuit", &SCRIPT_SANDBOX::QuickChange::Lua_ReadSuit);
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable->RegisterObjectFunctor("SetCurrentSuit", &SCRIPT_SANDBOX::QuickChange::Lua_SetCurrentSuit);
	SCRIPT_SANDBOX::QuickChange::s_pMetaTable->RegisterObjectFunctor("GetSuitName", &SCRIPT_SANDBOX::QuickChange::Lua_GetSuitName);

	LuaObject obj_QuickChange = pState->BoxPointer(&(SCRIPT_SANDBOX::QuickChange::s_QuickChange));
	obj_QuickChange.SetMetaTable(*SCRIPT_SANDBOX::QuickChange::s_pMetaTable);
	objGlobal.SetObject("QuickChange", obj_QuickChange);

	//---------------------------------------------------------------------------------
	//System Setup
	SCRIPT_SANDBOX::SystemSetup* pSystemSetup = new SCRIPT_SANDBOX::SystemSetup();
	pSystemSetup->RegisterFunction("Texture", &SCRIPT_SANDBOX::SystemSetup::Lua_Texture);
	//ִ����Ӧһ���˵�����
	pSystemSetup->RegisterFunction("Texture", &SCRIPT_SANDBOX::SystemSetup::Lua_Texture);
	pSystemSetup->RegisterFunction("Display", &SCRIPT_SANDBOX::SystemSetup::Lua_Display);
	pSystemSetup->RegisterFunction("Sound", &SCRIPT_SANDBOX::SystemSetup::Lua_Sound);
	pSystemSetup->RegisterFunction("OpenSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenSetup);
	pSystemSetup->RegisterFunction("ViewSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_ViewSetup);
	pSystemSetup->RegisterFunction("SoundSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_SoundSetup);
	pSystemSetup->RegisterFunction("UISetup", &SCRIPT_SANDBOX::SystemSetup::Lua_UISetup);
	pSystemSetup->RegisterFunction("InputSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_InputSetup);
	pSystemSetup->RegisterFunction("GameSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_GameSetup);
	pSystemSetup->RegisterFunction("GameHelp", &SCRIPT_SANDBOX::SystemSetup::Lua_GameHelp);
	pSystemSetup->RegisterFunction("BackGame", &SCRIPT_SANDBOX::SystemSetup::Lua_BackGame);
	//��Ƶ���
	pSystemSetup->RegisterFunction("View_GetData", &SCRIPT_SANDBOX::SystemSetup::Lua_View_GetData);
	pSystemSetup->RegisterFunction("View_SetData", &SCRIPT_SANDBOX::SystemSetup::Lua_View_SetData);
	pSystemSetup->RegisterFunction("View_SaveToFile", &SCRIPT_SANDBOX::SystemSetup::Lua_View_SaveToFile);
	//��Ϸ����
	pSystemSetup->RegisterFunction("GameGetData", &SCRIPT_SANDBOX::SystemSetup::Lua_GameGetData);
	pSystemSetup->RegisterFunction("SaveGameSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_SaveGameSetup);
	//�����Ϣ
	pSystemSetup->RegisterFunction("AskPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_AskPrivateInfo);
	pSystemSetup->RegisterFunction("ApplyPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_ApplyPrivateInfo);
	pSystemSetup->RegisterFunction("GetPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_GetPrivateInfo);
	pSystemSetup->RegisterFunction("OpenPrivatePage", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenPrivatePage);
	pSystemSetup->RegisterFunction("SetPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_SetPrivateInfo);
	pSystemSetup->RegisterFunction("OpenEquipFrame", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenEquipFrame);
	pSystemSetup->RegisterFunction("OpenPetFrame", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenPetFrame);
	pSystemSetup->RegisterFunction("GetCaredObjId", &SCRIPT_SANDBOX::SystemSetup::Lua_GetCaredObjId);

	// �ı䴰��ƫ����
	pSystemSetup->RegisterFunction( "ChangeViewSize", &SCRIPT_SANDBOX::SystemSetup::Lua_ChangeViewSize );


	//---------------------------------------------------------------------------------
	//PlayerShop
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::PlayerShop::s_pMetaTable = objGlobal.CreateTable("PlayerShop");
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::PlayerShop::s_pMetaTable);

	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("CreateShop", &SCRIPT_SANDBOX::PlayerShop::CreateShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetShopNum", &SCRIPT_SANDBOX::PlayerShop::GetShopNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumShop", &SCRIPT_SANDBOX::PlayerShop::EnumShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("AskOpenShop", &SCRIPT_SANDBOX::PlayerShop::AskOpenShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::PlayerShop::EnumItem);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumShopInfo", &SCRIPT_SANDBOX::PlayerShop::EnumShopInfo);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetSelectIndex", &SCRIPT_SANDBOX::PlayerShop::GetSelectIndex);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("RetackItem", &SCRIPT_SANDBOX::PlayerShop::RetackItem);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("InputMoney", &SCRIPT_SANDBOX::PlayerShop::InputMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("UpStall", &SCRIPT_SANDBOX::PlayerShop::UpStall);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("SetCurSelectPage", &SCRIPT_SANDBOX::PlayerShop::SetCurSelectPage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCurSelectPage", &SCRIPT_SANDBOX::PlayerShop::GetCurSelectPage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("DealMoney", &SCRIPT_SANDBOX::PlayerShop::DealMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ApplyMoney", &SCRIPT_SANDBOX::PlayerShop::ApplyMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("InfoMoney", &SCRIPT_SANDBOX::PlayerShop::InfoMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetStallNum", &SCRIPT_SANDBOX::PlayerShop::GetStallNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("BuyItem", &SCRIPT_SANDBOX::PlayerShop::BuyItem);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("IsSelectOnSale", &SCRIPT_SANDBOX::PlayerShop::IsSelectOnSale);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("DownSale", &SCRIPT_SANDBOX::PlayerShop::DownSale);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ClearSelect", &SCRIPT_SANDBOX::PlayerShop::ClearSelect);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetMoney", &SCRIPT_SANDBOX::PlayerShop::GetMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCommercialFactor", &SCRIPT_SANDBOX::PlayerShop::GetCommercialFactor);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetShopInfo", &SCRIPT_SANDBOX::PlayerShop::GetShopInfo);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("IsOpenStall", &SCRIPT_SANDBOX::PlayerShop::IsOpenStall);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("OpenStall", &SCRIPT_SANDBOX::PlayerShop::OpenStall);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("AskStallData", &SCRIPT_SANDBOX::PlayerShop::AskStallData);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::PlayerShop::EnumPet);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("SetSelectPet", &SCRIPT_SANDBOX::PlayerShop::SetSelectPet);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCanOpenShopType", &SCRIPT_SANDBOX::PlayerShop::GetCanOpenShopType);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("SetCurSelectPetIndex", &SCRIPT_SANDBOX::PlayerShop::SetCurSelectPetIndex);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetObjPrice", &SCRIPT_SANDBOX::PlayerShop::GetObjPrice);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("Modify", &SCRIPT_SANDBOX::PlayerShop::Modify);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetObjName", &SCRIPT_SANDBOX::PlayerShop::GetObjName);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ViewPetDesc", &SCRIPT_SANDBOX::PlayerShop::ViewPetDesc);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ClearSelectPos", &SCRIPT_SANDBOX::PlayerShop::ClearSelectPos);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("Transfer", &SCRIPT_SANDBOX::PlayerShop::Transfer);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("IsSaleOut", &SCRIPT_SANDBOX::PlayerShop::IsSaleOut);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("BuyShop", &SCRIPT_SANDBOX::PlayerShop::BuyShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("CloseShop", &SCRIPT_SANDBOX::PlayerShop::CloseShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("OpenMessage", &SCRIPT_SANDBOX::PlayerShop::OpenMessage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetMessageNum", &SCRIPT_SANDBOX::PlayerShop::GetMessageNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCurPageMessageNum", &SCRIPT_SANDBOX::PlayerShop::GetCurPageMessageNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumMessage", &SCRIPT_SANDBOX::PlayerShop::EnumMessage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("DealFriend", &SCRIPT_SANDBOX::PlayerShop::DealFriend);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetFriendNum", &SCRIPT_SANDBOX::PlayerShop::GetFriendNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumFriend", &SCRIPT_SANDBOX::PlayerShop::EnumFriend);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ChangeShopNum", &SCRIPT_SANDBOX::PlayerShop::ChangeShopNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetNpcId", &SCRIPT_SANDBOX::PlayerShop::GetNpcId);

	LuaObject obj_PlayerShop = pState->BoxPointer(&(SCRIPT_SANDBOX::PlayerShop::s_PlayerShop));
	obj_PlayerShop.SetMetaTable(*SCRIPT_SANDBOX::PlayerShop::s_pMetaTable);
	objGlobal.SetObject("PlayerShop", obj_PlayerShop);

	//---------------------------------------------------------------------------------
	//Friend
	SCRIPT_SANDBOX::Friend::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Friend::s_pMetaTable = objGlobal.CreateTable("Friend");
	SCRIPT_SANDBOX::Friend::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Friend::s_pMetaTable);

	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "Close",							&SCRIPT_SANDBOX::Friend::Close);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "EnumName",						&SCRIPT_SANDBOX::Friend::EnumName);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "OpenGrouping",					&SCRIPT_SANDBOX::Friend::OpenGrouping);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "OpenMenu",						&SCRIPT_SANDBOX::Friend::OpenMenu);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "AskTeam",							&SCRIPT_SANDBOX::Friend::AskTeam);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "GetHistroyNumber",				&SCRIPT_SANDBOX::Friend::GetHistroyNumber);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "GetHistroyData",					&SCRIPT_SANDBOX::Friend::GetHistroyData);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "SetCurrentTeam",					&SCRIPT_SANDBOX::Friend::SetCurrentTeam);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "GetCurrentTeam",					&SCRIPT_SANDBOX::Friend::GetCurrentTeam);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "CallOf",							&SCRIPT_SANDBOX::Friend::CallOf);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "ViewFeel",						&SCRIPT_SANDBOX::Friend::ViewFeel);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "IsMoodInHead",					&SCRIPT_SANDBOX::Friend::IsMoodInHead);
	// ��������
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "CreateGrouping",					&SCRIPT_SANDBOX::Friend::CreateGrouping);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "ShowFriendInputDlg",				&SCRIPT_SANDBOX::Friend::ShowFriendInputDlg);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "ShowAddNewFriendDlg",				&SCRIPT_SANDBOX::Friend::ShowAddNewFriendDlg);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "GetNewGroupInfo",					&SCRIPT_SANDBOX::Friend::GetNewGroupInfo);

	// ���������ж��Ƿ��Ǻ���
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "IsPlayerIsFriend",				&SCRIPT_SANDBOX::Friend::IsPlayerIsFriend);
	// ���õ�ǰѡ��
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "SetCurrentSelect",				&SCRIPT_SANDBOX::Friend::SetCurrentSelect);

	// ������
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor( "OpenSearcher",					&SCRIPT_SANDBOX::Friend::OpenSearcher);
	

	LuaObject obj_Friend = pState->BoxPointer(&(SCRIPT_SANDBOX::Friend::s_Friend));
	obj_Friend.SetMetaTable(*SCRIPT_SANDBOX::Friend::s_pMetaTable);
	objGlobal.SetObject("Friend", obj_Friend);


	//FriendSearcher
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable = objGlobal.CreateTable("FriendSearcher");
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable);

	// ��պ����б�
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "ClearFingerList",			&SCRIPT_SANDBOX::FriendSearcher::ClearFingerList);
	// ��ȡҳ��
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "GetFriendPageNumber",		&SCRIPT_SANDBOX::FriendSearcher::GetFriendPageNumber);
	// ��ȡһҳ�еĺ�������
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "GetFriendNumberInPage",	&SCRIPT_SANDBOX::FriendSearcher::GetFriendNumberInPage);
	// ��ȡһҳ��ѡ��ĺ��Ѳ���
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "GetFriendFromPage",		&SCRIPT_SANDBOX::FriendSearcher::GetFriendFromPage);
	// ͨ��id����
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "FriendSearchByID",		&SCRIPT_SANDBOX::FriendSearcher::FriendSearchByID);
	// ͨ����������
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "FriendSearchByName",		&SCRIPT_SANDBOX::FriendSearcher::FriendSearchByName);
	// ͨ�����ֺ�id��ȷ�Ĳ���
//	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "FriendSearchByNameAndId",	&SCRIPT_SANDBOX::FriendSearcher::FriendSearchByNameAndId);
	// �߼�����
	SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable->RegisterObjectFunctor( "FriendSearchAdvance",		&SCRIPT_SANDBOX::FriendSearcher::FriendSearchAdvance);

	LuaObject obj_FriendSearcher = pState->BoxPointer(&(SCRIPT_SANDBOX::FriendSearcher::s_FriendSearcher));
	obj_FriendSearcher.SetMetaTable(*SCRIPT_SANDBOX::FriendSearcher::s_pMetaTable);
	objGlobal.SetObject("FriendSearcher", obj_FriendSearcher);

	//-----------------------------------------------------------------------------------------------------
	// ÿ�չ�������
	// DaytipsData
	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::DaytipsData::s_pMetaTable = objGlobal.CreateTable("DaytipsData");
	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::DaytipsData::s_pMetaTable);

	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable->RegisterObjectFunctor( "FillCycleMissionList",	&SCRIPT_SANDBOX::DaytipsData::FillCycleMissionList);
	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable->RegisterObjectFunctor( "FillFixedMissionList",	&SCRIPT_SANDBOX::DaytipsData::FillFixedMissionList);
	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable->RegisterObjectFunctor( "RecommendMission",	&SCRIPT_SANDBOX::DaytipsData::RecommendMission);
	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable->RegisterObjectFunctor( "GetWeek",	&SCRIPT_SANDBOX::DaytipsData::GetWeek);
	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable->RegisterObjectFunctor( "GetMissionByIndex",	&SCRIPT_SANDBOX::DaytipsData::GetMissionByIndex);
	SCRIPT_SANDBOX::DaytipsData::s_pMetaTable->RegisterObjectFunctor( "GetMissionCount",	&SCRIPT_SANDBOX::DaytipsData::GetMissionCount);
	
	LuaObject obj_DaytipsData = pState->BoxPointer(&(SCRIPT_SANDBOX::DaytipsData::s_DaytipsData));
	obj_DaytipsData.SetMetaTable(*SCRIPT_SANDBOX::DaytipsData::s_pMetaTable);
	objGlobal.SetObject("DaytipsData", obj_DaytipsData);

	//-----------------------------------------------------------------------------------------------------
	// �������
	// QuestLogData
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::QuestLogData::s_pMetaTable = objGlobal.CreateTable("QuestLogData");
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::QuestLogData::s_pMetaTable);

	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "GetMissionList",	&SCRIPT_SANDBOX::QuestLogData::GetMissionList);
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "CanDoMissionNum",	&SCRIPT_SANDBOX::QuestLogData::CanDoMissionNum);
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "DoneMissionNum_CurScene",	&SCRIPT_SANDBOX::QuestLogData::DoneMissionNum_CurScene);
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "CanDoMissionNum_CurScene",	&SCRIPT_SANDBOX::QuestLogData::CanDoMissionNum_CurScene);
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfoByMissionID",	&SCRIPT_SANDBOX::QuestLogData::GetMissionInfoByMissionID);
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "GetMissionInfoByClientIndex",	&SCRIPT_SANDBOX::QuestLogData::GetMissionInfoByClientIndex);
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "GetMissionRewordNumByIndex",	&SCRIPT_SANDBOX::QuestLogData::GetMissionRewordNumByIndex);
	SCRIPT_SANDBOX::QuestLogData::s_pMetaTable->RegisterObjectFunctor( "GetMissionRewordInfo",	&SCRIPT_SANDBOX::QuestLogData::GetMissionRewordInfo);

	LuaObject obj_QuestLogData = pState->BoxPointer(&(SCRIPT_SANDBOX::QuestLogData::s_QuestLogData));
	obj_QuestLogData.SetMetaTable(*SCRIPT_SANDBOX::QuestLogData::s_pMetaTable);
	objGlobal.SetObject("QuestLogData", obj_QuestLogData);

	//-----------------------------------------------------------------------------------------------------
	//
	// ��¼���̵�������
	//
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable = objGlobal.CreateTable("GameProduceLogin");

	// �õ�����ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetServerAreaCount",		&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaCount);

	// �õ����������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetServerAreaName",		&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaName);

	// �õ�������login server�ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetAreaLoginServerCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaLoginServerCount);

	// �õ�login server����Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetAreaLoginServerInfo",	&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaLoginServerInfo);
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetLastLoginAreaAndServerIndex",	&SCRIPT_SANDBOX::CGameProduce_Login::GetLastLoginAreaAndServerIndex);

	// ���ӵ�login server
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ConnectToLoginServer",	&SCRIPT_SANDBOX::CGameProduce_Login::ConnectToLoginServer);

	// �򿪷�����ѡ�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ExitToSelectServer",	&SCRIPT_SANDBOX::CGameProduce_Login::ExitToSelectServer);

	// ��֤�û���������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("CheckAccount",	&SCRIPT_SANDBOX::CGameProduce_Login::CheckAccount);

	// ѡ��һ��login server������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SelectLoginServer",	&SCRIPT_SANDBOX::CGameProduce_Login::SelectLoginServer);

	// ������ѡ������л����ʺ��������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeToAccountInputDlgFromSelectRole",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeToAccountInputDlgFromSelectRole);

	// ������ѡ������л������ﴴ������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeToCreateRoleDlgFromSelectRole",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeToCreateRoleDlgFromSelectRole);

	// �����ﴴ�������л�������ѡ�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeToSelectRoleDlgFromCreateRole",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeToSelectRoleDlgFromCreateRole);
	
	// �õ���ǰ��ɫ�ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetRoleCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetRoleCount);
	
	// �õ���ɫ����Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetRoleInfo",	&SCRIPT_SANDBOX::CGameProduce_Login::GetRoleInfo);

	// �õ���ɫUI modle ����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetRoleUIModleName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetRoleUIModleName);

	// ����һ����ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("CreateRole",	&SCRIPT_SANDBOX::CGameProduce_Login::CreateRole);

	//ɾ��һ����ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("DelRole",	&SCRIPT_SANDBOX::CGameProduce_Login::DelRole);

	//���ͽ�����Ϸ��Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SendEnterGameMsg",	&SCRIPT_SANDBOX::CGameProduce_Login::SendEnterGameMsg);
	// ���ͻ�������Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeSceneMsg",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeSceneMsg);


	// ѯ���Ƿ�ɾ����ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("DelRole_YesNo",	&SCRIPT_SANDBOX::CGameProduce_Login::DelRole_YesNo);

	// ѯ���Ƿ��˳���Ϸ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ExitGame_YesNo",	&SCRIPT_SANDBOX::CGameProduce_Login::ExitGame_YesNo);

	// ѯ���Ƿ��˵��ʺ��������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ExitToAccountInput_YesNo",	&SCRIPT_SANDBOX::CGameProduce_Login::ExitToAccountInput_YesNo);

	// ɾ��ѡ�еĽ�ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("DelSelRole",	&SCRIPT_SANDBOX::CGameProduce_Login::DelSelRole);

	//�õ��������������Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetAreaServerInfo",	&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaServerInfo);

	// �Զ�ѡ��һ���뵱ǰѡ������繩Ӧ��ƥ���login server
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("AutoSelLoginServer",	&SCRIPT_SANDBOX::CGameProduce_Login::AutoSelLoginServer);

	// �õ�Ů����ͷ��ĸ���.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetWomanFaceCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetWomanFaceCount);

	// �õ�������ͷ��ĸ���.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetManFaceCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetManFaceCount);

	// �õ�Ů����ͷ�����Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetWomanFaceName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetWomanFaceName);

	// �õ�������ͷ�����Ϣ.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetManFaceName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetManFaceName);

	// �õ�����ͷ�����Ϣ.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetFaceName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetFaceName);

	// ����ͷ��id
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetFaceId",	&SCRIPT_SANDBOX::CGameProduce_Login::SetFaceId);

	// �õ����εĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetFaceModelCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetFaceModelCount);

	// �õ�����ģ�͵�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetFaceModelName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetFaceModelName);

	// ��������ģ��id
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetFaceModelId",	&SCRIPT_SANDBOX::CGameProduce_Login::SetFaceModelId);

	// ��������ģ��id
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("PlayAnimation",	&SCRIPT_SANDBOX::CGameProduce_Login::PlayAnimation);

	// �õ����͵ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetHairModelCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetHairModelCount);

	// �õ�����ģ�͵�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetHairModelName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetHairModelName);

	// ���÷���ģ��id
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetHairModelId",	&SCRIPT_SANDBOX::CGameProduce_Login::SetHairModelId);


	// �Ա�
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor( "SetGender",	&SCRIPT_SANDBOX::CGameProduce_Login::SetGender );
	// ְҵ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor( "SetProfession",	&SCRIPT_SANDBOX::CGameProduce_Login::SetProfession );


	// �ر�����״̬��
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("CloseNetConnect",	&SCRIPT_SANDBOX::CGameProduce_Login::CloseNetConnect);
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("CreateRotateRole",	&SCRIPT_SANDBOX::CGameProduce_Login::CreateRotateRole);
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeSelectRole",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeSelectRole);
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SelRotateRole",	&SCRIPT_SANDBOX::CGameProduce_Login::SelRotateRole);

	LuaObject obj_GameProduceLogin = pState->BoxPointer(&(SCRIPT_SANDBOX::CGameProduce_Login::s_GameProduceLogin));
	obj_GameProduceLogin.SetMetaTable(*SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable);
	objGlobal.SetObject("GameProduceLogin", obj_GameProduceLogin);

	// ---------------------------------------------------------------------------------------------------------
	// ����ѧϰ
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::SkillStudy::s_pMetaTable = objGlobal.CreateTable("SkillStudy");
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::SkillStudy::s_pMetaTable);

	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "StudySkill",		 &SCRIPT_SANDBOX::SkillStudy::StudySkill );
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetStudyState",	 &SCRIPT_SANDBOX::SkillStudy::GetStudyState );
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetSkillLevel",	 &SCRIPT_SANDBOX::SkillStudy::GetSkillLevel );
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetSkillMaxLevel", &SCRIPT_SANDBOX::SkillStudy::GetSkillMaxLevel );
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetSeriesPoint",	 &SCRIPT_SANDBOX::SkillStudy::GetSeriesPoint );
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetFatherSkillID", &SCRIPT_SANDBOX::SkillStudy::GetFatherSkillID );
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetSkillPoint",    &SCRIPT_SANDBOX::SkillStudy::GetSkillPoint );
	//laim
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetSkillType",    &SCRIPT_SANDBOX::SkillStudy::GetSkillType );

	// 20100421 AddCodeBegin
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetSkillName",    &SCRIPT_SANDBOX::SkillStudy::GetSkillName );
	// 20100421 AddCodeEnd

	// 20100421 AddCodeBegin
	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "GetSkillPassiveFlag",    &SCRIPT_SANDBOX::SkillStudy::GetSkillPassiveFlag );
	// 20100421 AddCodeEnd

	SCRIPT_SANDBOX::SkillStudy::s_pMetaTable->RegisterObjectFunctor( "RequestIsStudySkill",    &SCRIPT_SANDBOX::SkillStudy::RequestIsStudySkill );

	LuaObject obj_SkillStudy = pState->BoxPointer( &(SCRIPT_SANDBOX::SkillStudy::s_SkillStudy) );
	obj_SkillStudy.SetMetaTable( *SCRIPT_SANDBOX::SkillStudy::s_pMetaTable );
	objGlobal.SetObject( "SkillStudy", obj_SkillStudy );


	// ---------------------------------------------------------------------------------------------------------
	// ������
	SCRIPT_SANDBOX::Mount::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Mount::s_pMetaTable = objGlobal.CreateTable("Mount");
	SCRIPT_SANDBOX::Mount::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Mount::s_pMetaTable);

	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountName",			&SCRIPT_SANDBOX::Mount::GetMountName );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountLevel",		&SCRIPT_SANDBOX::Mount::GetMountLevel );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountGender",		&SCRIPT_SANDBOX::Mount::GetMountGender );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountEraCount",		&SCRIPT_SANDBOX::Mount::GetMountEraCount );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "IsCanBeMating",		&SCRIPT_SANDBOX::Mount::IsCanBeMating );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountGUID",			&SCRIPT_SANDBOX::Mount::GetMountGUID );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetSpouseID",			&SCRIPT_SANDBOX::Mount::GetSpouseID );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountList",			&SCRIPT_SANDBOX::Mount::GetMountList );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountHappiness",	&SCRIPT_SANDBOX::Mount::GetMountHappiness );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountGrowRate",		&SCRIPT_SANDBOX::Mount::GetMountGrowRate );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountBasic",		&SCRIPT_SANDBOX::Mount::GetMountBasic );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountSavvy",		&SCRIPT_SANDBOX::Mount::GetMountSavvy );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountStr",			&SCRIPT_SANDBOX::Mount::GetMountStr );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountDex",			&SCRIPT_SANDBOX::Mount::GetMountDex );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountCon",			&SCRIPT_SANDBOX::Mount::GetMountCon );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountInt",			&SCRIPT_SANDBOX::Mount::GetMountInt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountPotential",	&SCRIPT_SANDBOX::Mount::GetMountPotential );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountStrApt",		&SCRIPT_SANDBOX::Mount::GetMountStrApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountDexApt",		&SCRIPT_SANDBOX::Mount::GetMountDexApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountConApt",		&SCRIPT_SANDBOX::Mount::GetMountConApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountIntApt",		&SCRIPT_SANDBOX::Mount::GetMountIntApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetUIModelName",		&SCRIPT_SANDBOX::Mount::GetUIModelName );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountUIModelName",	&SCRIPT_SANDBOX::Mount::GetMountUIModelName );  // [7/14/2010 �¾���]
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "SetAttrAddRequest",	&SCRIPT_SANDBOX::Mount::SetAttrAddRequest );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountSkill",		&SCRIPT_SANDBOX::Mount::GetMountSkill );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountAction",		&SCRIPT_SANDBOX::Mount::GetMountAction );       // [7/14/2010 �¾���]
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "IsMounting",			&SCRIPT_SANDBOX::Mount::IsMounting );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "Mounting",				&SCRIPT_SANDBOX::Mount::Mounting );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "Dismount",				&SCRIPT_SANDBOX::Mount::Dismount );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "FreeMount",			&SCRIPT_SANDBOX::Mount::FreeMount );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "CallMount",			&SCRIPT_SANDBOX::Mount::CallMount );            // [7/14/2010 �¾���]
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "ReCallMount",			&SCRIPT_SANDBOX::Mount::ReCallMount );          // [7/14/2010 �¾���]
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "DiscardMount",			&SCRIPT_SANDBOX::Mount::DiscardMount );         // [7/14/2010 �¾���]
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetBattleMountIndex",	&SCRIPT_SANDBOX::Mount::GetBattleMountIndex );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetSelectMount",		&SCRIPT_SANDBOX::Mount::GetSelectMount );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "SetSelectMount",		&SCRIPT_SANDBOX::Mount::SetSelectMount );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "ItemUnLock",			&SCRIPT_SANDBOX::Mount::ItemUnLock );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "UseMountItem",			&SCRIPT_SANDBOX::Mount::UseMountItem );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "ChangeMountName",		&SCRIPT_SANDBOX::Mount::ChangeMountName );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "IdentifyMount",		&SCRIPT_SANDBOX::Mount::RequestIdentifyMount );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "CanMating",			&SCRIPT_SANDBOX::Mount::CanMating );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "IsLocked",				&SCRIPT_SANDBOX::Mount::IsLocked );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "RequestMating",		&SCRIPT_SANDBOX::Mount::RequestMating );

	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "RequestMyMountMatingLocked",	&SCRIPT_SANDBOX::Mount::RequestMyMountMatingLocked );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "RequestTogleMatingPage",		&SCRIPT_SANDBOX::Mount::RequestTogleMatingPage );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetMountNickName",				&SCRIPT_SANDBOX::Mount::GetMountNickName );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "RequestGainMatingMount",		&SCRIPT_SANDBOX::Mount::RequestGainMatingMount );

	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountName",		&SCRIPT_SANDBOX::Mount::GetTargetMountName );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountNickName",	&SCRIPT_SANDBOX::Mount::GetTargetMountNickName );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountLevel",		&SCRIPT_SANDBOX::Mount::GetTargetMountLevel );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountGender",		&SCRIPT_SANDBOX::Mount::GetTargetMountGender );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountID",			&SCRIPT_SANDBOX::Mount::GetTargetMountID );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetSpouseID",		&SCRIPT_SANDBOX::Mount::GetTargetSpouseID );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountStr",		&SCRIPT_SANDBOX::Mount::GetTargetMountStr );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountCon",		&SCRIPT_SANDBOX::Mount::GetTargetMountCon );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountDex",		&SCRIPT_SANDBOX::Mount::GetTargetMountDex );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountInt",		&SCRIPT_SANDBOX::Mount::GetTargetMountInt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountStrApt",		&SCRIPT_SANDBOX::Mount::GetTargetMountStrApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountDexApt",		&SCRIPT_SANDBOX::Mount::GetTargetMountDexApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountConApt",		&SCRIPT_SANDBOX::Mount::GetTargetMountConApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountIntApt",		&SCRIPT_SANDBOX::Mount::GetTargetMountIntApt );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountBasic",		&SCRIPT_SANDBOX::Mount::GetTargetMountBasic );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountSavvy",		&SCRIPT_SANDBOX::Mount::GetTargetMountSavvy );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountGrowRate",	&SCRIPT_SANDBOX::Mount::GetTargetMountGrowRate );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetUIModelName",		&SCRIPT_SANDBOX::Mount::GetTargetUIModelName );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "TargetMountAttrRequest",	&SCRIPT_SANDBOX::Mount::TargetMountAttrRequest );
	SCRIPT_SANDBOX::Mount::s_pMetaTable->RegisterObjectFunctor( "GetTargetMountSkill",		&SCRIPT_SANDBOX::Mount::GetTargetMountSkill );

	LuaObject obj_Mount = pState->BoxPointer( &(SCRIPT_SANDBOX::Mount::s_Mount) );
	obj_Mount.SetMetaTable( *SCRIPT_SANDBOX::Mount::s_pMetaTable );
	objGlobal.SetObject( "Mount", obj_Mount );


	// ---------------------------------------------------------------------------------------------------------
	// �������
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable = objGlobal.CreateTable("SouXiaLu");
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable);

	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "IsEquipSoiuXia",			&SCRIPT_SANDBOX::SouXiaLu::Lua_IsEquipSoiuXia );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaUsbNum",			&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaUsbNum );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaUsbItem",			&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaUsbItem );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "StudyByUSBItem",			&SCRIPT_SANDBOX::SouXiaLu::Lua_StudyByUSBItem );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaPageNum_Type",			&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaPageNum_Type );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaLevel",			&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaLevel );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaIndex",			&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaIndex );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaPageNum",			&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaPageNum );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaSkillID",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaSkillID );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaActionIDBySkillID",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaActionIDBySkillID );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaSkillName",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaSkillName );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaArmID",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaArmID );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaArmData",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaArmData );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaArmIcon",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaArmIcon );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXiaArmMatData",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXiaArmMatData );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSouXia_Data",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSouXia_Data );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SetPrewSkill",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SetPrewSkill );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SetPrewArm",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SetPrewArm );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SetPrewPet",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SetPrewPet );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SetPrewZuoJi",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SetPrewZuoJi );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "ClearPrewArm",		&SCRIPT_SANDBOX::SouXiaLu::Lua_ClearPrewArm );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "InitModelAvart",		&SCRIPT_SANDBOX::SouXiaLu::Lua_InitModelAvart );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SouXiaLu_ArmMake",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SouXiaLu_ArmMake );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "IsBlankSouXiaItem",		&SCRIPT_SANDBOX::SouXiaLu::Lua_IsBlankSouXiaItem );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetSkillIdByActionID",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetSkillIdByActionID );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "GetModelIDbySkillID",		&SCRIPT_SANDBOX::SouXiaLu::Lua_GetModelIDbySkillID );

	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SouXiaLu_Dismantle_Accept",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SouXiaLu_Dismantle_Accept );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SouXiaLu_Destory_Accept",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SouXiaLu_Destory_Accept );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SouXiaLu_Complex_Accept",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SouXiaLu_Complex_Accept );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SouXiaLu_Sew_Accept",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SouXiaLu_Sew_Accept );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SouXia_SetLock",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SouXia_SetLock );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "Clear_SouXiaSkill",		&SCRIPT_SANDBOX::SouXiaLu::Lua_Clear_SouXiaSkill );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SouXia_NpcId",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SouXia_NpcId );
	SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable->RegisterObjectFunctor( "SetMainMenuBarSouXiaSkillNum",		&SCRIPT_SANDBOX::SouXiaLu::Lua_SetMainMenuBarSouXiaSkillNum );

	LuaObject obj_SouXialu = pState->BoxPointer( &(SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu) );
	obj_SouXialu.SetMetaTable( *SCRIPT_SANDBOX::SouXiaLu::s_pMetaTable );
	objGlobal.SetObject( "SouXiaLu", obj_SouXialu );

}



VOID CScriptSystem::Tick(void)
{
	const UINT COLLECT_TIME = 3*1000; //3m
	static INT s_nLastCollectTime = 0;

	UINT nTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	if(CGameProcedure::s_pTimeSystem->CalSubTime(s_nLastCollectTime, nTimeNow) >= COLLECT_TIME)
	{
		//�ͷ������ڴ�
		CScriptSystem::GetMe()->GetLuaState()->CollectGarbage();

		s_nLastCollectTime = nTimeNow;
	}
}

VOID CScriptSystem::Release(VOID)
{
	//����֤��
	delete SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable;
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable = NULL;

	//������
	delete SCRIPT_SANDBOX::Guild::s_pMetaTable;
	SCRIPT_SANDBOX::Guild::s_pMetaTable = NULL;

	//ϵͳ����
	delete SCRIPT_SANDBOX::SystemSetup::GetMe();

	//�����ύ
	delete SCRIPT_SANDBOX::Talk::s_pMetaTable;
	SCRIPT_SANDBOX::Talk::s_pMetaTable = NULL;

	//�����ύ
	delete SCRIPT_SANDBOX::MissionReply::s_pMetaTable;
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable = NULL;

	//��̯���(Buy)
	delete SCRIPT_SANDBOX::StallBuy::s_pMetaTable;
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable = NULL;

	//MissionReply
	delete SCRIPT_SANDBOX::MissionReply::s_pMetaTable;
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable = NULL;

	//NpcShop
	delete SCRIPT_SANDBOX::NpcShop::s_pMetaTable;
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable = NULL;

	//Friend
	delete SCRIPT_SANDBOX::Friend::s_pMetaTable;
	SCRIPT_SANDBOX::Friend::s_pMetaTable = NULL;

	//PlayerShop
	delete SCRIPT_SANDBOX::PlayerShop::s_pMetaTable;
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable = NULL;

	//��̯���(Sale)
	delete SCRIPT_SANDBOX::StallSale::s_pMetaTable;
	SCRIPT_SANDBOX::StallSale::s_pMetaTable = NULL;

	//��̯��Ϣ
	delete SCRIPT_SANDBOX::StallBbs::s_pMetaTable;
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable = NULL;

	//Tooltip
	delete SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable;
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable = NULL;

	//��Ұ���
	delete SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable;
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable = NULL;

	//���֮��Ľ���
	delete SCRIPT_SANDBOX::Exchange::s_pMetaTable;
	SCRIPT_SANDBOX::Exchange::s_pMetaTable = NULL;

	//����Լ�������
	delete SCRIPT_SANDBOX::Bank::s_pMetaTable;
	SCRIPT_SANDBOX::Bank::s_pMetaTable = NULL;

	//���ﵼ������
	delete SCRIPT_SANDBOX::Pet::s_pMetaTable;
	SCRIPT_SANDBOX::Pet::s_pMetaTable = NULL;

	delete SCRIPT_SANDBOX::TargetPet::s_pMetaTable;
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable = NULL;

	//�����
	delete SCRIPT_SANDBOX::LifeAbility::s_pMetaTable;
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable = NULL;

	//�������ʦ��������
	delete SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable;
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable = NULL;

	//��¼���浼������.
	delete SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable;
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable = NULL;

	//���� ���ݳؽӿ�
	delete SCRIPT_SANDBOX::DataPool::s_pMetaTable;
	SCRIPT_SANDBOX::DataPool::s_pMetaTable = NULL;

	//���� ���ư�ť����
	delete CActionItem::s_pMetaTable;
	CActionItem::s_pMetaTable = NULL;

	std::list< CScriptEnvironment* >::iterator it;
	for(it=m_listScriptEnv.begin(); it!=m_listScriptEnv.end(); it++)
	{
		if(*it)
		{
			delete (CScriptEnvironment*)(*it);
			*it = NULL;
		}
	}

	m_listScriptEnv.clear();
}

void CScriptSystem::SetActiveEnvironment(LPCTSTR szEnvName)
{
	m_strEnvName = szEnvName;

	CHAR szSetEnvScript[MAX_PATH];
	_snprintf(szSetEnvScript, MAX_PATH, "setmetatable(_G, {__index = %s});", 
		szEnvName);
	GetLuaState()->DoString(szSetEnvScript);
}

struct _LOAD_QUEST 
{
	int nId;
	const char* szString;
};

void CScriptSystem::_loadQuestFile(void)
{
	const char* szQuestFileIndexFile = "Script.tab";
	const char* szBlankString = " \t";

	//-----------------------------
	// Open file
	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szQuestFileIndexFile, lpAddress, "General");
	if(dwSize > 0)
	{
		DBC::DBCFile dbFile(0);
		dbFile.OpenFromMemory( lpAddress,lpAddress + dwSize + 1);
		INT nCount = dbFile.GetRecordsNum();
		for (int i=0; i<nCount; i++)
		{
			_LOAD_QUEST* pContent = (_LOAD_QUEST*) dbFile.Search_Posistion(i, 0);
			if (pContent)
			{
				m_mapQuestFile.insert(std::make_pair(pContent->nId , pContent->szString));
			}
		}
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
	}
	else
	{
		KLThrow("Can't open quest file index file: %s", szQuestFileIndexFile);
		return;
	}

}

tScriptEnvironment* CScriptSystem::GetEnvironmentByName(const LPCTSTR szName )
{
	std::list< CScriptEnvironment* >::iterator it;
	for(it = m_listScriptEnv.begin(); it != m_listScriptEnv.end(); it++)
	{
		CScriptEnvironment* pEnv = *it;
		if (strncmp(szName, pEnv->GetName(), 255) == 0)
		{
			return pEnv;
		}
	}
	return NULL;
}