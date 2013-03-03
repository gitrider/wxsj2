//TestUtil接口

#pragma once

#include "AxpUtilLuaExport.h"

namespace AXP
{

class Lua_Test : public LuaExport< Lua_Test >
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/
	//清空
	int Lua_clear(LuaPlus::LuaState* state);
	//文件检查
	int Lua_selfCheck(LuaPlus::LuaState* state);
	//目录扰动
	int Lua_distrubPath(LuaPlus::LuaState* state);
	//目录拷贝
	int Lua_copyPath(LuaPlus::LuaState* state);
	//创建目录
	int Lua_createPath(LuaPlus::LuaState* state);
	//删除目录
	int Lua_deletePath(LuaPlus::LuaState* state);
	//日志
	int Lua_log(LuaPlus::LuaState* state);

private:
	//递归扰动目录
	void _distrubPath(const char* szPath);
	//扰动文件
	bool _distrubFile(const char* szFileName);
	//日志
	void _log(const char* pszFmt, ...);


	std::string m_strLogFile;
	void _recreateLog(void);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_Test(LuaPlus::LuaState* pLuaState);
	~Lua_Test();
};

}