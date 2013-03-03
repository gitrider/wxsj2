//PakMaker�ӿ�

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

	//���
	int Lua_clear(LuaPlus::LuaState* state);
	//����Ŀ¼
	int Lua_addDiskFold(LuaPlus::LuaState* state);
	//�洢
	int Lua_savePakFile(LuaPlus::LuaState* state);
	//�Ƚ�
	int Lua_comparePakFile(LuaPlus::LuaState* state);
	//�ƶ�Skeleton�ļ�
	int Lua_moveSkeleton(LuaPlus::LuaState* state);

private:
	//PakMaker ʵ��ָ��
	IPakMaker*		m_pPakMakerImpl;

	typedef std::set< std::string > FILENAME_SET;
	FILENAME_SET m_strSkeletonToMove;

	//�ƶ�Skeleton�ļ�
	void _moveSkeleton(const char* szPath, FILENAME_SET& setSkeletonToMove);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_PakMaker(LuaPlus::LuaState* pLuaState);
	~Lua_PakMaker();
};

}