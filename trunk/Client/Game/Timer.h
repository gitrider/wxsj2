

#ifndef __TIMER_HEADER__
#define __TIMER_HEADER__

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};

#include "GINode.h"
//#include "GMInterface_Lua.h"

typedef void ( __stdcall * FnTimer )(HWND, UINT, UINT, DWORD);

class tTimerSystem : public tNode
{
public:
	//������ʱ��
	virtual int AddTimer(FnTimer  pTimerFn, UINT nElpase ) = 0;
	virtual int AddTimer(const std::string& env, const std::string& pfn, UINT nElpase) = 0;
	//ɾ����ʱ��
	virtual void DelTimer(UINT timerID) = 0;
	virtual void DelTimer(FnTimer fn) =0;
	virtual void DelTimer(std::string pfn) = 0;
	//ɾ�����м�ʱ��
	virtual void ClearAllTimer() = 0;
protected:
	KL_DECLARE_DYNAMIC(tTimerSystem);
private:
};
struct TIMERPARAM 
{
	FnTimer pfn;
	UINT nTimerID;
};
struct TIMERPARAM_S 
{
	std::string pfn;
	std::string env;
	UINT nTimerID;
};

class CTimerManager : public tTimerSystem
{
public:
	CTimerManager();
	~CTimerManager();
	virtual int AddTimer(FnTimer  pTimerFn, UINT nElpase);
	virtual int AddTimer(const std::string& env, const std::string& pfn, UINT nElpase);
	virtual void DelTimer(UINT timerID);
	virtual void DelTimer(FnTimer fn);
	virtual void DelTimer(std::string pfn);
	void ClearAllTimer();
	void Initial(void*);
	//��ȡ�ű�ִ�л��������ݶ�ʱ��ID
	void GetScriptEnv(UINT tID, std::string &env, std::string& script);
	static CTimerManager* GetMe() { return m_pMe; }

public:
	INT LUA_SetTimer(LuaPlus::LuaState * L);
	INT LUA_KillTimer(LuaPlus::LuaState *L);
protected:
	KL_DECLARE_DYNAMIC(CTimerManager);
	typedef  std::map<UINT, TIMERPARAM>::iterator TimerMapIt;
	typedef std::map<UINT, TIMERPARAM_S>::iterator TimerMapSIt;
private:
	std::map<UINT, TIMERPARAM> m_TimerMap;
	std::map<UINT, TIMERPARAM_S> m_TimerMapS;
	static CTimerManager* m_pMe;
	
	//Ĭ�ϵĽű���ʱ���¼�������
	static void __stdcall DefaultTimerFunc(HWND, UINT, UINT, DWORD);
};
//�ű�ɳ��
namespace SCRIPT_SANDBOX
{
	INT LUA_SetTimer(LuaPlus::LuaState * L);
	INT LUA_KillTimer(LuaPlus::LuaState *L);
	INT LUA_ClearAllTimer(LuaPlus::LuaState *L);
};


void __stdcall TestFunc(HWND, UINT, UINT, DWORD);
#endif