#include "StdAfx.h"
#pragma warning(push)
#pragma warning(disable : 4996)

#include "GMVariable.h"
#include "GIException.h"
#include "GIUtil.h"
#include "..\Event\GMEventSystem.h"
#include "..\Script\GMScriptSystem.h"
#include "..\Global.h"
#include "..\Input\GMInputSystem.h"


const char* SYSTEM_CFG	= ".\\Game.cfg";
const char* USER_CFG	= "..\\Accounts\\Game.cfg";

INT ConvertSectionInVector(LPCTSTR szSectionBuf, INT nLen, std::vector< STRING >& vSectionBuf);

CVariableSystem* CVariableSystem::s_pMe = NULL;
KL_IMPLEMENT_DYNAMIC(CVariableSystem, GETCLASS(tVariableSystem));
CVariableSystem::CVariableSystem()
{
	s_pMe = this;
	m_pMetaTable = 0;
}

CVariableSystem::~CVariableSystem()
{
	if(m_pMetaTable) delete m_pMetaTable;
	m_pMetaTable = 0;
}

//从文件中读取变量
VOID CVariableSystem::LoadVariable(LPCTSTR szFileName, VARIABLE_MAP& mapBuf)
{
	KLAssert(szFileName);
	//	mapBuf.clear();

	//-----------------------------------------------------------
	//取得配置文件的大小
	HANDLE hFile = CreateFile(szFileName,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(INVALID_HANDLE_VALUE == hFile) return;

	DWORD dwHigh;
	DWORD dwFileSize = GetFileSize(hFile, &dwHigh);
	CloseHandle(hFile); hFile = NULL;
	if(0==dwFileSize) return;

	//-----------------------------------------------------------
	//分配足够的内存
	CHAR* pTempBuf = new CHAR[dwFileSize+32];
	if(!pTempBuf) return;

	//-----------------------------------------------------------
	//从配置文件中读取"Variable"节
	::GetPrivateProfileSection("Variable", pTempBuf, dwFileSize, szFileName);
	//分解
	std::vector< STRING > vRet;
	ConvertSectionInVector(pTempBuf, dwFileSize, vRet);

	delete[] pTempBuf; pTempBuf=NULL;

	//加入变量定义
	for(INT i=0; i<(INT)vRet.size(); i++)
	{
		STRING& strLine = vRet[i];

		STRING::size_type tEqu = strLine.find_first_of("= \t");
		if(tEqu == STRING::npos) continue;

		STRING strName = strLine.substr(0, tEqu);

		CHAR szTemp[1024];
		::GetPrivateProfileString("Variable", strName.c_str(), "", szTemp, 1024, szFileName);

		SetVariable(strName.c_str(), szTemp, FALSE);
	}

}

//将变量保存到文件中
VOID CVariableSystem::SaveVariable(LPCTSTR szFileName,  VARIABLE_MAP& mapBuf)
{
	const CHAR* szSystemCfg = szFileName;
	UINT dwFileAttr = GetFileAttributes(szSystemCfg);

	//去掉文件只读属性
	if(INVALID_FILE_ATTRIBUTES != dwFileAttr)
	{
		if(FILE_ATTRIBUTE_READONLY & dwFileAttr)
		{
			SetFileAttributes(szSystemCfg, dwFileAttr&(~FILE_ATTRIBUTE_READONLY));
		}
	}

	VARIABLE_MAP::iterator it;
	for(it=mapBuf.begin(); it!=mapBuf.end(); it++)
	{
		if(it->second.bTemp == FALSE)
		{
			::WritePrivateProfileString("Variable", it->first.c_str(), it->second.vValue.c_str(), szSystemCfg);
		}
	}
}

//得到某个变量的值，如果不存在，则返回FALSE
BOOL CVariableSystem::GetVariable(LPCTSTR szName, STRING& strValue)
{
	std::string strName = szName;
	if( strName == "View_Resoution" )
	{
		int z = 0;
	}

	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end()) return FALSE;

	strValue = itFind->second.vValue;
	return true;
}

//设置某个变量的值，如果不存在，则首先创建
VOID CVariableSystem::SetVariable(LPCTSTR szName, LPCTSTR szValue, BOOL bTemp, BOOL bFireEvent)
{
	//不接受非法值
	if(!szName || szName[0] == 0) return;
	if(!szValue) return;

	std::string strName = szName;
	if( strName == "View_Resoution" )
	{
		int z = 0;
	}

	VARIABLE varOld = m_mapCurrent[szName];
	VARIABLE new_Variable;

	new_Variable.bTemp = bTemp;
	new_Variable.vValue = szValue;

	m_mapCurrent[szName] = new_Variable;

	if(varOld.vValue != szValue && bFireEvent)
	{
		//产生事件
		std::vector< STRING > vParam;
		vParam.push_back(szName);
		vParam.push_back(szValue);
		CEventSystem::GetMe()->PushEvent(GE_VARIABLE_CHANGED, vParam);
	}
}

VOID CVariableSystem::SetAs_Float(LPCTSTR szName, FLOAT fValue, BOOL bTemp)
{
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%f", fValue);
	SetVariable(szName, szTemp, bTemp);
}

VOID CVariableSystem::SetAs_Int(LPCTSTR szName, INT nValue, BOOL bTemp)
{
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", nValue);
	SetVariable(szName, szTemp, bTemp);
}

VOID CVariableSystem::SetAs_Uint(LPCTSTR szName, UINT uValue, BOOL bTemp)
{
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%u", uValue);
	SetVariable(szName, szTemp, bTemp);
}

VOID CVariableSystem::SetAs_Vector2(LPCTSTR szName, FLOAT fX, FLOAT fY, BOOL bTemp)
{
	CHAR szTemp[64];
	_snprintf(szTemp, 32, "%f,%f", fX, fY);
	SetVariable(szName, szTemp, bTemp);
}


//设置某个变量的值，但并不马上生效, 下一次启动时才会生效
VOID CVariableSystem::SetVariableDelay(LPCTSTR szName, LPCTSTR szValue)
{
	m_mapNeedSave[szName].vValue = szValue;
}

VOID CVariableSystem::Initial(VOID*)
{
	//-----------------------------------------------------------
	//加载
	m_mapCurrent.clear();
	LoadVariable(SYSTEM_CFG, m_mapCurrent);
	if(GetAs_Int("GameServer_ConnectDirect")==0) LoadVariable(USER_CFG, m_mapCurrent);
	m_mapNeedSave.clear();

	//创建帐号配置文件目录
	::CreateDirectory("..\\Accounts", 0);

	//-----------------------------------------------------------
	//导出函数
	m_pMetaTable = new LuaPlus::LuaObject;
	*m_pMetaTable = CScriptSystem::GetMe()->GetLuaState()->GetGlobals().CreateTable("Variable");
	m_pMetaTable->SetObject("__index", *m_pMetaTable);

	m_pMetaTable->RegisterObjectFunctor("GetVariable", &CVariableSystem::Lua_GetGlobalVariable);
	m_pMetaTable->RegisterObjectFunctor("SetVariable", &CVariableSystem::Lua_SetGlobalVariable);

	LuaPlus::LuaObject obj_Variable = CScriptSystem::GetMe()->GetLuaState()->BoxPointer(this);
	obj_Variable.SetMetaTable(*m_pMetaTable);
	CScriptSystem::GetMe()->GetLuaState()->GetGlobals().SetObject("Variable", obj_Variable);

	SetAs_Int("DebugMode", g_bDebugMode);
}
VOID CVariableSystem::ReSet(VOID)
{
	//SaveAcountVariables();
	SetAs_Int("_Self_Data_Complete", 0);
	SetAs_Int( "_Display_Guild_Leave_Word", 0 );

	SetAs_Int("_FirstEnterScene_",0);


	SetAs_Int( "_Tab_Flashed_", 0 );

}


void CVariableSystem::SaveAcountVariables(void)
{
	if(GetAs_Int("GameServer_ConnectDirect") == 1)
		return;
	STRING szAccount,szUser,szGame_Connect;
	VARIABLE_MAP userCfgMap;
	VARIABLE_MAP::const_iterator it = m_mapCurrent.begin();
	for(;it != m_mapCurrent.end();it++)
	{
		STRING key = (*it).first;
		VARIABLE val  = (*it).second;
		userCfgMap[key] = val;
	}
	VARIABLE_MAP::iterator itFind = userCfgMap.find("View_Fanhunjiao");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("View_Fanhunjiao"));

	itFind = userCfgMap.find("View_Aplomb");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("View_Aplomb"));

	itFind = userCfgMap.find("Login_Area");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("Login_Area"));

	itFind = userCfgMap.find("Login_Server");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("Login_Server"));

	itFind = userCfgMap.find("Login_Provider");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("Login_Provider"));

	itFind = userCfgMap.find("View_Resoution");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("View_Resoution"));

	itFind = userCfgMap.find("View_PiFeng");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("View_PiFeng"));

	itFind = userCfgMap.find("View_FullScreen");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("View_FullScreen"));

	itFind = userCfgMap.find("View_MaxWindow");
	if(itFind != userCfgMap.end())
		userCfgMap.erase(userCfgMap.find("View_MaxWindow"));

	GetVariable("Game_Account",szAccount);
	GetVariable("Character_Name",szUser);
	if(szAccount.empty() || szUser.empty()) return;

	char szFullPath[MAX_PATH] = {0};

	STRING szAccEncrypt;
	StringEncrypt( szAccount, szAccEncrypt ); //加密帐号名
	_snprintf(szFullPath, MAX_PATH, "..\\Accounts\\%s", szAccEncrypt.c_str());
	//_snprintf(szFullPath, MAX_PATH, "..\\Accounts\\%s", szAccount.c_str());
	::CreateDirectory(szFullPath, NULL);

	char szFullFileName[MAX_PATH] = {0};
	STRING szUserEncrypt;
	StringEncrypt( szUser, szUserEncrypt ); //加密帐号名
	_snprintf(szFullFileName, MAX_PATH, "%s\\%s.pfc", szFullPath, szUserEncrypt.c_str());
	//_snprintf(szFullFileName, MAX_PATH, "%s\\%s.pfc", 
	//	szFullPath, szUser.c_str());
	::DeleteFile(szFullFileName);
	SaveVariable(szFullFileName, userCfgMap);
}

VOID CVariableSystem::Release(VOID)
{
	//should save m_mapNeedSave ...
	STRING szAccount,szUser,szGame_Connect;

	if(GetAs_Int("GameServer_ConnectDirect") != 1)
	{

		{
			VARIABLE_MAP userCfgMap;
			userCfgMap["View_Fanhunjiao"] = m_mapCurrent["View_Fanhunjiao"];
			userCfgMap["View_Aplomb"] = m_mapCurrent["View_Aplomb"];
			userCfgMap["Login_Area"] = m_mapCurrent["Login_Area"];
			userCfgMap["Login_Server"] = m_mapCurrent["Login_Server"];
			userCfgMap["Login_Provider"] = m_mapCurrent["Login_Provider"];
			userCfgMap["View_Resoution"] = m_mapCurrent["View_Resoution"];
			userCfgMap["View_PiFeng"] = m_mapCurrent["View_PiFeng"];
			userCfgMap["View_FullScreen"] = m_mapCurrent["View_FullScreen"];
			userCfgMap["View_MaxWindow"] = m_mapCurrent["View_MaxWindow"];
			SaveVariable(USER_CFG, userCfgMap);

		}

		{
			VARIABLE_MAP userCfgMap;
			m_mapCurrent.erase(m_mapCurrent.find("View_Fanhunjiao"));

			m_mapCurrent.erase(m_mapCurrent.find("View_Aplomb"));
			m_mapCurrent.erase(m_mapCurrent.find("Login_Area"));
			m_mapCurrent.erase(m_mapCurrent.find("Login_Server"));
			m_mapCurrent.erase(m_mapCurrent.find("Login_Provider"));
			m_mapCurrent.erase(m_mapCurrent.find("View_Resoution"));
			m_mapCurrent.erase(m_mapCurrent.find("View_PiFeng"));
			m_mapCurrent.erase(m_mapCurrent.find("View_FullScreen"));
			m_mapCurrent.erase(m_mapCurrent.find("View_MaxWindow"));
			//userCfgMap[""] = m_mapCurrent["View_Aplomb"];
			//userCfgMap[""] = m_mapCurrent["Login_Area"];
			//userCfgMap[""] = m_mapCurrent["Login_Server"];
			//userCfgMap[""] = m_mapCurrent["Login_Provider"];
			//userCfgMap[""] = m_mapCurrent["View_Resoution"];
			//userCfgMap[""] = m_mapCurrent["View_PiFeng"];
			//userCfgMap[""] = m_mapCurrent["View_FullScreen"];
			//userCfgMap[""] = m_mapCurrent["View_MaxWindow"];
			SaveVariable(USER_CFG, userCfgMap);

		}

		GetVariable("Game_Account",szAccount);
		GetVariable("Character_Name",szUser);
		if(szAccount.empty() || szUser.empty()) return;

		char szFullPath[MAX_PATH] = {0};

		STRING szAccEncrypt;
		StringEncrypt( szAccount, szAccEncrypt ); //加密帐号名
		_snprintf(szFullPath, MAX_PATH, "..\\Accounts\\%s", szAccEncrypt.c_str());
		//_snprintf(szFullPath, MAX_PATH, "..\\Accounts\\%s", szAccount.c_str());
		::CreateDirectory(szFullPath, NULL);

		char szFullFileName[MAX_PATH] = {0};
		STRING szUserEncrypt;
		StringEncrypt( szUser, szUserEncrypt ); //加密帐号名
		_snprintf(szFullFileName, MAX_PATH, "%s\\%s.pfc", szFullPath, szUserEncrypt.c_str());
		//_snprintf(szFullFileName, MAX_PATH, "%s\\%s.pfc", 
		//	szFullPath, szUser.c_str());
		::DeleteFile(szFullFileName);
		SaveVariable(szFullFileName, m_mapCurrent);

	}
	else
	{
		//-- FOR DEBUG
		SaveVariable(SYSTEM_CFG, m_mapCurrent);
		//-- FOR DEBUG
	}
}

//字符串, DEF=""
const STRING& CVariableSystem::GetAs_String(LPCTSTR szName, BOOL* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		static STRING s_strBlank = "";
		if(bHave) *bHave = FALSE;
		return s_strBlank;
	}

	if(bHave) *bHave = true;
	return itFind->second.vValue;
}

//整数, DEF=0
INT	CVariableSystem::GetAs_Int(LPCTSTR szName, BOOL* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = FALSE;
		return 0;
	}

	if(bHave) *bHave = true;
	return atoi(itFind->second.vValue.c_str());
}

//无符号整数, DEF=0
UINT	CVariableSystem::GetAs_Uint(LPCTSTR szName, BOOL* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = FALSE;
		return 0;
	}

	if(bHave) *bHave = true;
	UINT uValue = 0;
	sscanf(itFind->second.vValue.c_str(), "%u", &uValue);
	return uValue;
}

//浮点数, DEF=0.0f
FLOAT CVariableSystem::GetAs_Float(LPCTSTR szName, BOOL* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = FALSE;
		return 0.0f;
	}

	if(bHave) *bHave = true;
	return (FLOAT)atof(itFind->second.vValue.c_str());
}

fVector2 CVariableSystem::GetAs_Vector2(LPCTSTR szName, BOOL* bHave)
{
	static fVector2 fvNULL(0.0f, 0.0f);

	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = FALSE;
		return fvNULL;
	}

	if(bHave) *bHave = true;

	CHAR szTemp[64];
	strncpy(szTemp, itFind->second.vValue.c_str(), 64);

	char* pDot = strchr(szTemp, ',');
	if(!pDot) return fvNULL;
	*pDot = '\0';

	return fVector2((FLOAT)atof(szTemp), (FLOAT)atof(pDot+1));
}

INT CVariableSystem::Lua_SetGlobalVariable(LuaPlus::LuaState* state)	//设置全局变量
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsString()) 
	{
		KLThrow("LUA:CVariableSystem::Lua_SetGlobalVariable[2] param parameter error");
		return 0;
	}
	if(!args[3].IsString()) 
	{
		KLThrow("LUA:CVariableSystem::Lua_SetGlobalVariable[3] param parameter error");
		return 0;
	}
	if(!args[4].IsInteger())
	{
		KLThrow("LUA:CVariableSystem::Lua_SetGlobalVariable[4] param parameter error");
		return 0;
	}

	// 如果没有设置触发事件参数		20100714 BLL
	if ( !args[5].IsInteger() )
	{
		CVariableSystem::GetMe()->SetVariable(args[2].GetString(), args[3].GetString(), args[4].GetInteger());
	}
	else
	{
		CVariableSystem::GetMe()->SetVariable(args[2].GetString(), args[3].GetString(), args[4].GetInteger(), args[5].GetInteger());
	}

	return 0;
}

INT CVariableSystem::Lua_GetGlobalVariable(LuaPlus::LuaState* state)	//取得全局变量
{
	LuaPlus::LuaStack args(state);
	if(!args[2].IsString())
	{
		state->PushNil();
		return 1;
	}

	STRING strRet;
	BOOL bExist = CVariableSystem::GetMe()->GetVariable(args[2].GetString(), strRet);
	if(bExist)
	{
		state->PushString(strRet.c_str());
	}
	else
	{
		state->PushNil();
	}
	return 1;
}

void CVariableSystem::SetVariableDefault(LPCTSTR szName, LPCTSTR szValue)
{
	STRING strOld;
	BOOL bExist = GetVariable(szName, strOld);
	if(bExist) return;

	SetVariable(szName, szValue, FALSE);
}

void CVariableSystem::LoadPrivateConfig()
{
	STRING szAccount,szUser;
	GetVariable("Game_Account",szAccount);
	GetVariable("Character_Name",szUser);

	ConvertOldSaving_pfc( szAccount, szUser );//转换旧目录

	::WritePrivateProfileString("Variable", "Login_Area", GetAs_String("Login_Area").c_str(), USER_CFG );
	::WritePrivateProfileString("Variable", "Login_Server", GetAs_String("Login_Server").c_str(), USER_CFG);
	::WritePrivateProfileString("Variable", "Login_Provider", GetAs_String("Login_Provider").c_str(), USER_CFG);

	std::string srcAcc = szAccount;
	std::string srcUser = szUser;
	std::string dstAcc = "";
	std::string dstUser = "";

	StringEncrypt( srcAcc, dstAcc );
	StringEncrypt( srcUser, dstUser );

	CHAR szTemp[120];
	//_snprintf(szTemp, 120, "..\\Accounts\\%s\\%s.pfc", szAccount.c_str(), szUser.c_str());
	_snprintf(szTemp, 120, "..\\Accounts\\%s\\%s.pfc", dstAcc.c_str(), dstUser.c_str());
	LoadVariable(szTemp, m_mapCurrent);

	KLU_Log("Load private profile:%s,%s", szAccount.c_str(), szUser.c_str());
}

void CVariableSystem::GetVariableInFile( LPCTSTR szFileName, LPCTSTR szTitle, LPCTSTR szKey, LPTSTR szValue, INT nSize )
{
	ConvertOldSaving_cfg( szFileName );

	std::string strDst = "";
	std::string strSrc = szFileName;
	StringEncrypt( strSrc, strDst );

	CHAR szTemp[240];
	//_snprintf( szTemp, 120, "..\\Accounts\\%s\\%s.cfg", szFileName, szFileName );
	_snprintf( szTemp, 240, "..\\Accounts\\%s\\%s.cfg", strDst.c_str(), strDst.c_str() );

	GetPrivateProfileString( szTitle, szKey, "", szValue, nSize, szTemp );
}
void CVariableSystem::SetVariableInFile( LPCTSTR szFileName, LPCTSTR szTitle, LPCTSTR szKey, LPCTSTR szValue )
{
	std::string strDst = "";
	std::string strSrc = szFileName;
	StringEncrypt( strSrc, strDst );

	char szFullPath[MAX_PATH] = {0};
	//_snprintf(szFullPath, MAX_PATH, "..\\Accounts\\%s", szFileName );
	_snprintf(szFullPath, MAX_PATH, "..\\Accounts\\%s", strDst.c_str() );

	::CreateDirectory(szFullPath, NULL);//创建帐号目录

	CHAR szTemp[240];
	//_snprintf( szTemp, 120, "..\\Accounts\\%s\\%s.cfg", szFileName, szFileName );
	_snprintf( szTemp, 240, "..\\Accounts\\%s\\%s.cfg", strDst.c_str(), strDst.c_str() );

	WritePrivateProfileString( szTitle, szKey, szValue, szTemp );

	//读取快捷键设置
	//_snprintf(szTemp, 240, "..\\Accounts\\%s\\Acce_Custom.ini", strDst.c_str());
	//CInputSystem::GetMe()->SetIniPath(szTemp);
	//CInputSystem::GetMe()->InitAcceKey();
}

void CVariableSystem::StringEncrypt( std::string& strSrc, std::string& strDst )
{
	strDst.clear();
	int SrcLength = static_cast < int > ( strSrc.length() );
	strDst.append( "#" );

	for( int i=0; i<SrcLength; i++ )
	{
		const char* pStr = strSrc.c_str();
		unsigned char TempChar = pStr[ i ];
		char TempStr[ 8 ];

		itoa( TempChar, TempStr, 16 );
		strDst.append( TempStr );
	}
}
void CVariableSystem::ConvertOldSaving_cfg( const std::string& strAccount )
{
	char strTemp[ MAX_PATH ];
	std::string strDirFormat = "..\\Accounts\\%s";
	std::string strFileFormat = "..\\Accounts\\%s\\%s.cfg";

	//给目录改名
	std::string strSrcAccount = strAccount;
	std::string strDstAccount = "";
	StringEncrypt( strSrcAccount, strDstAccount );

	_snprintf( strTemp, MAX_PATH, strDirFormat.c_str(), strSrcAccount.c_str() );
	std::string strSrcDir = strTemp;
	_snprintf( strTemp, MAX_PATH, strDirFormat.c_str(), strDstAccount.c_str() );
	std::string strDstDir = strTemp;

	MoveFile( strSrcDir.c_str(), strDstDir.c_str() ); //给目录改名

	//给文件改名
	std::string strSrcName = strAccount;
	std::string strDstName = "";
	StringEncrypt( strSrcName, strDstName );

	_snprintf( strTemp, MAX_PATH, strFileFormat.c_str(), strDstAccount.c_str(), strSrcAccount.c_str() );
	std::string strSrcFile = strTemp;
	_snprintf( strTemp, MAX_PATH, strFileFormat.c_str(), strDstAccount.c_str(), strDstAccount.c_str() );
	std::string strDstFile = strTemp;

	MoveFile( strSrcFile.c_str(), strDstFile.c_str() );

}
void CVariableSystem::ConvertOldSaving_pfc( const std::string& strAccount, const std::string& strCharacter )
{

	char strTemp[ MAX_PATH ];
	std::string strDirFormat = "..\\Accounts\\%s";
	std::string strFileFormat = "..\\Accounts\\%s\\%s.pfc";

	//给目录改名
	std::string strSrcAccount = strAccount;
	std::string strDstAccount = "";
	StringEncrypt( strSrcAccount, strDstAccount );

	_snprintf( strTemp, MAX_PATH, strDirFormat.c_str(), strSrcAccount.c_str() );
	std::string strSrcDir = strTemp;
	_snprintf( strTemp, MAX_PATH, strDirFormat.c_str(), strDstAccount.c_str() );
	std::string strDstDir = strTemp;

	MoveFile( strSrcDir.c_str(), strDstDir.c_str() ); //给目录改名

	//给文件改名
	std::string strSrcName = strCharacter;
	std::string strDstName = "";
	StringEncrypt( strSrcName, strDstName );

	_snprintf( strTemp, MAX_PATH, strFileFormat.c_str(), strDstAccount.c_str(), strSrcName.c_str() );
	std::string strSrcFile = strTemp;
	_snprintf( strTemp, MAX_PATH, strFileFormat.c_str(), strDstAccount.c_str(), strDstName.c_str() );
	std::string strDstFile = strTemp;

	MoveFile( strSrcFile.c_str(), strDstFile.c_str() ); //给文件改名

}

//---------------------------------------------------------------------------------------------
INT ConvertSectionInVector(LPCTSTR szSectionBuf, INT nLen, std::vector< STRING >& vSectionBuf)
{
	register const char *pDot = szSectionBuf;

	vSectionBuf.clear();

	while(TRUE)
	{
		if( (*pDot == '\0' && *(pDot-1) == '\0') || (pDot-szSectionBuf >= nLen) )
		{
			break;
		}

		if(*pDot == '\0')
		{
			pDot++;
			continue;
		}

		STRING strTheSingle = pDot;
		vSectionBuf.push_back(strTheSingle);

		pDot += strlen(pDot)+1;
	}

	return (INT)vSectionBuf.size();
}

#pragma warning(pop)
