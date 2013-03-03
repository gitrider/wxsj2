#pragma once

#include  <LuaPlus.h>

using namespace LuaPlus;

class CScriptModule
{
public:
	CScriptModule(void);
	~CScriptModule(void);
	void Initialize(const char* strGlobalFile);
	void DoString(const char* buf);
	void DoFile(const char* fileName);
	INT  DoFunction(const char* szFunctionName,const char* szParam1 = NULL,const char* szParam2 = NULL);
	const char*  DoFunctionAndRetString(const char* szFunctionName,const char* szParam1 = NULL,const char* szParam2 = NULL);
private:
	static  LuaStateAuto m_plusState;
};
