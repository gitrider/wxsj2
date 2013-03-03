//PatchMaker�ӿ�

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

	//���
	int Lua_clear(LuaPlus::LuaState* state);
	//����Ŀ¼
	int Lua_analysePath(LuaPlus::LuaState* state);
	//��ð汾��
	int Lua_getPathVersion(LuaPlus::LuaState* state);
	//����Patch�ļ�
	int Lua_generatePatch(LuaPlus::LuaState* state);
	//������������
	int Lua_generateUpdateExe(LuaPlus::LuaState* state);

private:
	//PakMaker ʵ��ָ��
	IPatchMaker*		m_pPatchMakerImpl;

	//����Patchʱ�Ļص�����, ����Launch.exe
	static bool __stdcall _patchCallBack(const char *szPakName, const char* szFileName);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_PatchMaker(LuaPlus::LuaState* pLuaState);
	~Lua_PatchMaker();
};

}