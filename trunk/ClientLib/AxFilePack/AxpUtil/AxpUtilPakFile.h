//PatchMaker接口

#pragma once

#include "AxpUtilLuaExport.h"

namespace AXP
{

class Lua_PakFile : public LuaExport< Lua_PakFile >
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
	//打开Pak文件
	int Lua_openPakFile(LuaPlus::LuaState* state);
	//全部解压到指定目录
	int Lua_extractAllToPath(LuaPlus::LuaState* state);
	//解压某文件到指定目录
	int Lua_extractFileToPath(LuaPlus::LuaState* state);
	//将某文件更新入包
	int Lua_updateFile(LuaPlus::LuaState* state);
	//将某文件从包中删除
	int Lua_removeFile(LuaPlus::LuaState* state);


	int Lua_test(LuaPlus::LuaState* state);
private:
	//Updater 实例指针
	IPakFile*		m_pPakImpl;

	//解压指定文件
	bool _extractFile(const char* szFileName, const char* szTargetPath);
	//创建目录
	bool _forceCreatePath(const char* szPath);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_PakFile(LuaPlus::LuaState* pLuaState);
	~Lua_PakFile();
};


}