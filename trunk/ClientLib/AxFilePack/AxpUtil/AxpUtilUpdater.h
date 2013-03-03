//PatchMaker�ӿ�

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

	//���
	int Lua_clear(LuaPlus::LuaState* state);
	//����Patch��
	int Lua_addPatchFile(LuaPlus::LuaState* state);
	//��ָ��Ŀ¼����
	int Lua_updateVersion(LuaPlus::LuaState* state);
	//���patch�ļ�crc
	int Lua_checkPatchCRC(LuaPlus::LuaState* state);
	//����/���ܸ��°�
	int Lua_encryptPatchFile(LuaPlus::LuaState* state);

private:
	//Updater ʵ��ָ��
	IUpdater*		m_pUpdaterImpl;

	//�����ص�����
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