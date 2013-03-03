#include "StdAfx.h"
#include "GMHelperSystem.h"
#include "LuaPlus.h"
#include "..\Variable\GMVariable.h"
#include "..\Script\GMScriptSystem.h"
#include "..\dbc\gmdatabase.h"
#include "..\Procedure\GameProcedure.h"
KL_IMPLEMENT_DYNAMIC(CHelperSystem, GETCLASS(tHelperSystem));
CHelperSystem*			LuaExport< CHelperSystem >::s_pMe = 0;
LuaPlus::LuaObject*		LuaExport< CHelperSystem >::s_pMetaTable = 0;

CHelperSystem::CHelperSystem() : 
LuaExport< CHelperSystem >("Helper")
{
	m_hHelperProcess = 0;

	RegisterFunction("GotoHelper", &CHelperSystem::Lua_GotoHelper);
	RegisterFunction("GotoPaihang", &CHelperSystem::Lua_GotoPaihang);
}

CHelperSystem::~CHelperSystem()
{
}

VOID CHelperSystem::Initial(VOID*)
{
	//����Ψһ�Ĵ�����
	char szBuf[MAX_PATH] = {0};
	_snprintf(szBuf, MAX_PATH, 
		"3174F021-DBCD-432a-B1C5-83CD8CE627B9-%08X", GetCurrentProcessId());

	m_strCmdWnd = (const char*)szBuf;

	//-----------------------------------------------------------
	//��������
	//m_pMetaTable = new LuaPlus::LuaObject;
	//*m_pMetaTable = CScriptSystem::GetMe()->GetLuaState()->GetGlobals().CreateTable("Helper");
	//m_pMetaTable->SetObject("__index", *m_pMetaTable);

	//m_pMetaTable->RegisterObjectFunctor("GotoHelper", Lua_GotoHelper);

	//LuaObject obj_Variable = CScriptSystem::GetMe()->GetLuaState()->BoxPointer(this);
	//obj_Variable.SetMetaTable(*m_pMetaTable);
	//CScriptSystem::GetMe()->GetLuaState()->GetGlobals().SetObject("Helper", obj_Variable);
}

VOID CHelperSystem::Release(VOID)
{
	if(m_hHelperProcess)
	{
		TerminateProcess(m_hHelperProcess, 0);
		m_hHelperProcess = 0;
	}
}

VOID CHelperSystem::Tick(VOID)
{
}


void CHelperSystem::GotoAnchorHelp()
{
	char strTmp[MAX_PATH];
	UINT WorldID=CGameProcedure::s_pVariableSystem->GetAs_Int("WorldID");
	if(WorldID==0) //����
		return;
	_snprintf(strTmp,MAX_PATH, "WorldID%u", CGameProcedure::s_pVariableSystem->GetAs_Int("WorldID"));
	//-------------------------------
	//Ѱ��Ŀ�괰��
	HWND hCmdWnd = ::FindWindow(0, m_strCmdWnd.c_str());
	if(!hCmdWnd)
	{
		CreateHelperProcess(strTmp);
		return;
	}

	//-------------------------------
	//��Ŀ�괰�ڷ�������
	typedef struct
	{
		unsigned char	cCmdType;
		unsigned char	cReserve1;
		unsigned char	cReserve2;
		unsigned char	cReserve3;
		char			pContents[MAX_PATH];
	} HELPER_COMMAND, FAR *LPHELPER_COMMAND;

	static  HELPER_COMMAND cmd;
	ZeroMemory(&cmd, sizeof(HELPER_COMMAND));
	_snprintf(cmd.pContents, MAX_PATH, "%s", strTmp);

	static COPYDATASTRUCT   cCopyData;
	ZeroMemory(&cCopyData, sizeof(COPYDATASTRUCT));

	cCopyData.dwData = 0;
	cCopyData.cbData = sizeof(unsigned char)*4 + (DWORD)strlen(cmd.pContents) + 1;
	cCopyData.lpData = &cmd;

	SendMessage(hCmdWnd, WM_COPYDATA, 0, (LPARAM)&cCopyData);
}

void CHelperSystem::GotoAnchorHelp(const STRING& strAnchor)
{
	//-------------------------------
	//Ѱ��Ŀ�괰��
	HWND hCmdWnd = ::FindWindow(0, m_strCmdWnd.c_str());
	if(!hCmdWnd)
	{
		CreateHelperProcess(strAnchor);
		return;
	}

	//-------------------------------
	//��Ŀ�괰�ڷ�������
	typedef struct
	{
		unsigned char	cCmdType;
		unsigned char	cReserve1;
		unsigned char	cReserve2;
		unsigned char	cReserve3;
		char			pContents[MAX_PATH];
	} HELPER_COMMAND, FAR *LPHELPER_COMMAND;

	static  HELPER_COMMAND cmd;
	ZeroMemory(&cmd, sizeof(HELPER_COMMAND));
	_snprintf(cmd.pContents, MAX_PATH, "%s", strAnchor.c_str());

	static COPYDATASTRUCT   cCopyData;
	ZeroMemory(&cCopyData, sizeof(COPYDATASTRUCT));

	cCopyData.dwData = 0;
	cCopyData.cbData = sizeof(unsigned char)*4 + (DWORD)strlen(cmd.pContents) + 1;
	cCopyData.lpData = &cmd;

	SendMessage(hCmdWnd, WM_COPYDATA, 0, (LPARAM)&cCopyData);
}

void CHelperSystem::CreateHelperProcess(const STRING& strAnchor)
{
	CHAR szWorkDir[MAX_PATH] = {0};
	::GetModuleFileName(0, szWorkDir, MAX_PATH);
	::PathRemoveFileSpec(szWorkDir);

	CHAR szHelperExe[MAX_PATH] = {0};
	strncpy(szHelperExe, szWorkDir, MAX_PATH);
	::PathAppend(szHelperExe, "Helper.exe");

	CHAR szCmdLine[MAX_PATH] = {0};
	_snprintf(szCmdLine, MAX_PATH, "%s -w:\"%s\" -a:\"%s\"", 
		szHelperExe, m_strCmdWnd.c_str(), strAnchor.c_str());

	::GetCurrentDirectory(MAX_PATH, szWorkDir);

	//��������
	m_hHelperProcess = 0;

	STARTUPINFO			si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	if (CreateProcess(NULL, szCmdLine, NULL, NULL, 
		FALSE,CREATE_DEFAULT_ERROR_MODE, NULL,
		szWorkDir, &si, &pi) == FALSE)
	{
		return;
	}

	m_hHelperProcess = pi.hProcess;
}

INT CHelperSystem::Lua_GotoHelper(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsString()) return 0;
	std::string  str = args[2].GetString();
	STRING::size_type nRight = str.find( "*" );
	if( nRight == std::string::npos )
		GotoAnchorHelp(args[2].GetString());
	else
	{
		str.erase( nRight, 1);
		DBC_DEFINEHANDLE( pUIDBC, DBC_UI_LAYOUTDEFINE);
		INT nNum = (INT)pUIDBC->GetRecordsNum();
		for(INT i=0; i<nNum; i++)
		{
			const _DBC_UI_LAYOUTDEFINE* pWindow= (const _DBC_UI_LAYOUTDEFINE*)pUIDBC->Search_LineNum_EQU(i);
			if( pWindow->szName == str )
			{
				GotoAnchorHelp( pWindow->szHelp );
				return 0;
			}
		}
	}


	return 0;
}

INT CHelperSystem::Lua_GotoPaihang(LuaPlus::LuaState* state)
{
	GotoAnchorHelp();
	return 0;
}
