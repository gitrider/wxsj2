//TestUtil�ӿ�

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
	//���
	int Lua_clear(LuaPlus::LuaState* state);
	//�ļ����
	int Lua_selfCheck(LuaPlus::LuaState* state);
	//Ŀ¼�Ŷ�
	int Lua_distrubPath(LuaPlus::LuaState* state);
	//Ŀ¼����
	int Lua_copyPath(LuaPlus::LuaState* state);
	//����Ŀ¼
	int Lua_createPath(LuaPlus::LuaState* state);
	//ɾ��Ŀ¼
	int Lua_deletePath(LuaPlus::LuaState* state);
	//��־
	int Lua_log(LuaPlus::LuaState* state);

private:
	//�ݹ��Ŷ�Ŀ¼
	void _distrubPath(const char* szPath);
	//�Ŷ��ļ�
	bool _distrubFile(const char* szFileName);
	//��־
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