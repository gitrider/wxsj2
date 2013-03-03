//PakMaker接口

#pragma once

#include "AxpUtilLuaExport.h"

namespace AXP
{

class Lua_PakMaker : public LuaExport< Lua_PakMaker >
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
	//加入目录
	int Lua_addDiskFold(LuaPlus::LuaState* state);
	//存储
	int Lua_savePakFile(LuaPlus::LuaState* state);
	//比较
	int Lua_comparePakFile(LuaPlus::LuaState* state);
	//移动Skeleton文件
	int Lua_moveSkeleton(LuaPlus::LuaState* state);

private:
	//PakMaker 实例指针
	IPakMaker*		m_pPakMakerImpl;

	typedef std::set< std::string > FILENAME_SET;
	FILENAME_SET m_strSkeletonToMove;

	//移动Skeleton文件
	void _moveSkeleton(const char* szPath, FILENAME_SET& setSkeletonToMove);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_PakMaker(LuaPlus::LuaState* pLuaState);
	~Lua_PakMaker();
};

}