//PatchMaker接口

#pragma once

#include "AxpUtilLuaExport.h"

namespace AXP
{

class Lua_Updater : public LuaExport< Lua_Updater >
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
	//加入Patch包
	int Lua_addPatchFile(LuaPlus::LuaState* state);
	//对指定目录升级
	int Lua_updateVersion(LuaPlus::LuaState* state);
	//检查patch文件crc
	int Lua_checkPatchCRC(LuaPlus::LuaState* state);
	//加密/解密更新包
	int Lua_encryptPatchFile(LuaPlus::LuaState* state);

private:
	//Updater 实例指针
	IUpdater*		m_pUpdaterImpl;

	//升级回调函数
	static void __stdcall _updateCallBack(const char *szPakName, const char* szFileName, 
									UPDATE_PROCESS process);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_Updater(LuaPlus::LuaState* pLuaState);
	~Lua_Updater();
};

}