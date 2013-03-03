//数据文件整理接口

#pragma once

#include "AxpUtilLuaExport.h"


namespace AXP
{

class Lua_DataFile : public LuaExport< Lua_DataFile >
{
public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/
	//转化DBC文件
	int Lua_convertDBCFile(LuaPlus::LuaState* state);
	//转化Lua文件
	int Lua_convertLUAFile(LuaPlus::LuaState* state);

	//一般文件保存回调接口
	static bool __stdcall _standSaveCallBack(const char *szNameInDiskOrg, 
							const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	//DBC和任务脚本文件保存回调接口
	static bool __stdcall _configSaveCallBack(const char *szNameInDiskOrg, 
							const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	//DBC和任务脚本文件保存回调接口(Hack)
	static bool __stdcall _configHackSaveCallBack(const char *szNameInDiskOrg, 
							const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	//一般文件比较回调函数
	static bool __stdcall _standCompareCallBack(const char* szFileNameInDisk, 
							IStream* pDataStream);

	//DBC和任务脚本文件保存回调接口
	static bool __stdcall _configCompareCallBack(const char* szFileNameInDisk, 
							IStream* pDataStream);

private:
	static std::string		m_strTempPath;	//!< 临时目录

	//将一个dbc文件转化
	static void convertDBCFile(const char* szSource, const char* szTarget);
	//将一个任务脚本转化
	static bool convertLUAFile(const char* szSource, const char* szTarget);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_DataFile(LuaPlus::LuaState* pLuaState);
	~Lua_DataFile();
};

}