//PatchMaker�ӿ�

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

	//���
	int Lua_clear(LuaPlus::LuaState* state);
	//��Pak�ļ�
	int Lua_openPakFile(LuaPlus::LuaState* state);
	//ȫ����ѹ��ָ��Ŀ¼
	int Lua_extractAllToPath(LuaPlus::LuaState* state);
	//��ѹĳ�ļ���ָ��Ŀ¼
	int Lua_extractFileToPath(LuaPlus::LuaState* state);
	//��ĳ�ļ��������
	int Lua_updateFile(LuaPlus::LuaState* state);
	//��ĳ�ļ��Ӱ���ɾ��
	int Lua_removeFile(LuaPlus::LuaState* state);


	int Lua_test(LuaPlus::LuaState* state);
private:
	//Updater ʵ��ָ��
	IPakFile*		m_pPakImpl;

	//��ѹָ���ļ�
	bool _extractFile(const char* szFileName, const char* szTargetPath);
	//����Ŀ¼
	bool _forceCreatePath(const char* szPath);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_PakFile(LuaPlus::LuaState* pLuaState);
	~Lua_PakFile();
};


}