#include "StdAfx.h"
#include "ScriptModule.h"
#include "Script_System.h"

LuaStateAuto  CScriptModule::m_plusState;

CScriptModule::CScriptModule(void)
{
}

CScriptModule::~CScriptModule(void)
{

}

void CScriptModule::DoString(const char* buf)
{
	try
	{
		if( m_plusState->DoString(buf) != 0)
		{
			printf("%s\n", m_plusState->Stack(-1).GetString());
		}
	}
	 catch (LuaException& e)
	{
		printf("DoString Error Msg = %s\n", e.GetMessage());
	}
	
}

void CScriptModule::DoFile(const char* fileName)
{
	try
	{
		if( m_plusState->DoFile(fileName) != 0)
		{
			printf("%s\n", m_plusState->Stack(-1).GetString());
		}
	}
	catch (LuaException& e)
	{
		printf("DoString Error Msg = %s\n", e.GetMessage());
	}

}

INT  CScriptModule::DoFunction(const char* szFunctionName,const char* szParam1,const char* szParam2)
{
	if (szFunctionName && m_plusState)
	{
		LuaFunction<int> func(m_plusState, szFunctionName);
		if(szParam1 && szParam2)
		{
			return func(szParam1, szParam2);
		}
		else if (szParam1)
		{
			return func(szParam1);
		}
		else if (szParam2)
		{
			return func(szParam2);
		}
		else
		{
			return func();
		}
	}
	return 0;
}


int printss(LuaState* L)
{
	LuaStack  arg(L);
	if (arg[1].IsString())
	{
		printf(arg[1].GetString());
	}
	else
	{
		printf("error call printss \n");
	}
	
	return 0;
}
void CScriptModule::Initialize(const char* strGlobalFile)
{
	m_plusState = LuaState::Create();
	
	if (strGlobalFile && strlen(strGlobalFile) > 0)
	{
		DoString(strGlobalFile);
	}
	LuaObject objGlobal = m_plusState->GetGlobals();
	LuaObject metaTblSystem = m_plusState->GetGlobals().CreateTable("metaTblSystem");
	metaTblSystem.SetObject("__index", metaTblSystem);
	metaTblSystem.RegisterObjectFunctor("print", &CScript_System::print);

	metaTblSystem.RegisterObjectFunctor("addEventToList", &CScript_System::addEventToList);
	metaTblSystem.RegisterObjectFunctor("addPropertyToList", &CScript_System::addPropertyToList);

	metaTblSystem.RegisterObjectFunctor("setLuaKeyWord", &CScript_System::setLuaKeyWord);
	metaTblSystem.RegisterObjectFunctor("setLuaConstantWord", &CScript_System::setLuaConstantWord);

	metaTblSystem.RegisterObjectFunctor("insertEditItem", &CScript_System::insertEditItem);
	metaTblSystem.RegisterObjectFunctor("insertComboboxItem", &CScript_System::insertComboboxItem);
	metaTblSystem.RegisterObjectFunctor("insertColorDlgItem", &CScript_System::insertColorDlgItem);

	
	LuaObject objSys = m_plusState->BoxPointer(CScript_System::Instance());
	objSys.SetMetaTable(metaTblSystem);
	m_plusState->GetGlobals().SetObject("System", objSys);
	
}


const char* CScriptModule::DoFunctionAndRetString(const char* szFunctionName,const char* szParam1 ,const char* szParam2 )
{
	if (szFunctionName && m_plusState)
	{
		LuaFunction<const char*> func(m_plusState, szFunctionName);
		if(szParam1 && szParam2)
		{
			return func(szParam1, szParam2);
		}
		else if (szParam1)
		{
			return func(szParam1);
		}
		else if (szParam2)
		{
			return func(szParam2);
		}
		else
		{
			return func();
		}
	}
	return "";
}