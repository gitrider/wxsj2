//* 			程序外接帮助系统			*

#pragma once
#include "GIHelperSystem.h"
#include "..\Interface\GMinterface_Lua.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CHelperSystem : public tHelperSystem, public LuaExport< CHelperSystem >
{
	//-----------------------------------------------------------
	//命令接口
	//-----------------------------------------------------------
public:
	// 跳转到指定帮助界面
	void	GotoAnchorHelp(const STRING& strAnchor);

	//重载此方法，排行榜专用 tongxi
	void	GotoAnchorHelp();

	//-----------------------------------------------------------
	// 导出到Lua的函数
	//-----------------------------------------------------------
public:
	INT Lua_GotoHelper(LuaPlus::LuaState* state);


	INT Lua_GotoPaihang(LuaPlus::LuaState* state);
protected:
	//创建帮助进程
	void	CreateHelperProcess(const STRING& strAnchor);

	//-----------------------------------------------------------
	//核心继承
	//-----------------------------------------------------------
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

protected:
	//帮助程序命令接受窗口名
	STRING	m_strCmdWnd;
	//帮助进程
	HANDLE	m_hHelperProcess;

public:
	CHelperSystem();
	virtual ~CHelperSystem();

protected:
	KL_DECLARE_DYNAMIC(CHelperSystem);
};
