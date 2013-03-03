#include "stdafx.h"
#include "Timer.h"
#include "GIException.h"
#include "Assertx.h"
#include "Script/GMScriptSystem.h"

KL_IMPLEMENT_DYNAMIC_VIRTUAL(tTimerSystem, GETCLASS(tNode));
KL_IMPLEMENT_DYNAMIC(CTimerManager, GETCLASS(tTimerSystem));

CTimerManager* CTimerManager::m_pMe = NULL;

CTimerManager::CTimerManager()
{
	m_pMe = this;
	m_TimerMap.clear();
	m_TimerMapS.clear();
}
CTimerManager::~CTimerManager()
{
	ClearAllTimer();
}
int CTimerManager::AddTimer(FnTimer  pTimerFn, UINT nElapse)
{
	UINT nID = (UINT)SetTimer(NULL,(UINT_PTR)0, nElapse, pTimerFn);
	AssertEx(nID > 0, "CreateTimer Failed");
	if (nID == 0)
	{
		return nID;
	}
	TIMERPARAM tm;
	tm.nTimerID = nID;
	tm.pfn =pTimerFn;
	m_TimerMap.insert(std::make_pair(nID, tm));
	return nID;
}

int CTimerManager::AddTimer(const std::string& env, const std::string& pfn, UINT nElpase)
{
	UINT nID = (UINT)SetTimer(NULL,(UINT_PTR)0, nElpase, CTimerManager::DefaultTimerFunc);
	AssertEx(nID > 0, "CreateTimer Failed");
	if (nID == 0)
	{
		return nID;
	}
	TIMERPARAM_S tm;
	tm.nTimerID = nID;
	tm.pfn =pfn;
	tm.env = env;
	m_TimerMapS.insert(std::make_pair(nID, tm));
	return nID;
}

void CTimerManager::DelTimer(UINT timerID)
{
	TimerMapIt it = m_TimerMap.find(timerID);
	if (it != m_TimerMap.end())
	{
		KillTimer(NULL, timerID);
		return;
	}

	TimerMapSIt its = m_TimerMapS.find(timerID);
	if (its != m_TimerMapS.end())
	{
		KillTimer(NULL, timerID);
		return;
	}
}
void CTimerManager::DelTimer(std::string pfn)
{
	for (TimerMapSIt it = m_TimerMapS.begin(); it != m_TimerMapS.end(); it++)
	{
		if (it->second.pfn == pfn)
		{
			KillTimer(NULL, it->first);
			it = m_TimerMapS.erase(it);
			return;
		}
	}
}
void CTimerManager::DelTimer(FnTimer fn)
{
	for (TimerMapIt it = m_TimerMap.begin(); it != m_TimerMap.end(); it++)
	{
		if (it->second.pfn == fn)
		{
			KillTimer(NULL, it->first);
			it = m_TimerMap.erase(it);
			return;
		}
	}
}
void CTimerManager::GetScriptEnv(UINT tID, std::string &env, std::string& script)
{
	for (TimerMapSIt it = m_TimerMapS.begin(); it != m_TimerMapS.end(); it++)
	{
		if (it->first == tID)
		{
			script = it->second.pfn;
			env = it->second.env;
			return;
		}
	}
	return;
}
void __stdcall CTimerManager::DefaultTimerFunc(HWND, UINT, UINT nID, DWORD)
{
	if (CTimerManager::GetMe())
	{
		std::string env, script;
		CTimerManager::GetMe()->GetScriptEnv(nID, env, script);
		if (script != "" && env != "")
		{
			env += "_Env";
			try{
				//保存旧环境名
				STRING strOldEnv = CScriptSystem::GetMe()->GetActiveEnvironment();
				//设定环境
				CScriptSystem::GetMe()->SetActiveEnvironment(env.c_str());
				//执行函数
				CScriptSystem::GetMe()->GetLuaState()->DoString(script.c_str());
				//恢复环境
				if(!strOldEnv.empty())
				{
					CScriptSystem::GetMe()->SetActiveEnvironment(strOldEnv.c_str());
				}
			}catch(...) 
			{
				STRING str = "执行环境 " + env + "执行函数: " + script;
				MessageBox(NULL, str.c_str(), "定时器使用错误" ,MB_ICONSTOP|MB_OK);
			}
			
		}
	}

}
void CTimerManager::ClearAllTimer()
{
	TimerMapIt end = m_TimerMap.end();
	for (TimerMapIt it = m_TimerMap.begin(); it != end; it++)
	{
		KillTimer(NULL, it->first);
	}
	m_TimerMap.clear();

	TimerMapSIt ends = m_TimerMapS.end();
	for (TimerMapSIt it = m_TimerMapS.begin(); it != ends; it++)
	{
		KillTimer(NULL, it->first);
	}
	m_TimerMapS.clear();
}

INT CTimerManager::LUA_SetTimer(LuaPlus::LuaState * state)
{
	LuaStack args(state);

	if(!(args[1].IsString()))
	{
		state->PushInteger(-1);
		return 1;
	}
	if(!(args[2].IsString()))
	{
		state->PushInteger(-1);
		return 1;
	}
	if(!(args[3].IsInteger()))
	{
		state->PushInteger(-1);
		return 1;
	}
	std::string env = args[1].GetString();
	std::string ES = args[2].GetString();
	UINT nElapse = args[3].GetInteger();
	int nID = AddTimer(env,ES, nElapse);
	state->PushInteger(nID);
	return 1;
}
INT CTimerManager::LUA_KillTimer(LuaPlus::LuaState *state)
{
	LuaStack args(state);

	if((args[1].IsInteger()))
	{
		UINT nID = args[1].GetInteger();
		DelTimer(nID);
		state->PushBoolean(true);
		return 1;
	}
	if((args[1].IsString()))
	{
		std::string pfn = args[1].GetString();
		DelTimer(pfn);
		state->PushBoolean(true);
		return 1;
	}
	state->PushBoolean(false);
	return 1;
}

namespace SCRIPT_SANDBOX
{
	INT LUA_SetTimer(LuaPlus::LuaState * L)
	{
		return CTimerManager::GetMe()->LUA_SetTimer(L);
	}
	INT LUA_KillTimer(LuaPlus::LuaState *L)
	{
		return CTimerManager::GetMe()->LUA_KillTimer(L);
	}
	INT LUA_ClearAllTimer(LuaPlus::LuaState *L)
	{
		CTimerManager::GetMe()->ClearAllTimer();
		return 0;
	}
};
/*VOID	 CTimerManager::Tick(VOID)
{
	for (TimerMapIt it = m_TimerMap.begin(); it != m_TimerMap.end(); it++)
	{
		if (it->second <= 0)
		{
			KillTimer(NULL, it->first);
			it = m_TimerMap.erase(it);
		}
		else
			it->second--;
	}

}*/	

void CTimerManager::Initial(void*)
{
	//id1 = AddTimer(TestFunc, 3099,1);
	//id2 = AddTimer(TestFunc, 5099,1);
}
