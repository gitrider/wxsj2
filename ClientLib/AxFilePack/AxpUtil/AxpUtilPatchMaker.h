//PatchMaker接口

#pragma once

#include "AxpUtilLuaExport.h"

namespace AXP
{

class Lua_PatchMaker : public LuaExport< Lua_PatchMaker >
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
	//分析目录
	int Lua_analysePath(LuaPlus::LuaState* state);
	//获得版本号
	int Lua_getPathVersion(LuaPlus::LuaState* state);
	//生成Patch文件
	int Lua_generatePatch(LuaPlus::LuaState* state);
	//生成升级补丁
	int Lua_generateUpdateExe(LuaPlus::LuaState* state);

private:
	//PakMaker 实例指针
	IPatchMaker*		m_pPatchMakerImpl;

	//生成Patch时的回调函数, 屏蔽Launch.exe
	static bool __stdcall _patchCallBack(const char *szPakName, const char* szFileName);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_PatchMaker(LuaPlus::LuaState* pLuaState);
	~Lua_PatchMaker();
};

}