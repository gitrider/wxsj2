#pragma once
#include "LuaPlus.h"

using namespace LuaPlus;

class CScript_System
{
public:
	CScript_System(void);
	~CScript_System(void);

public:
	int print(LuaState* L);
	int addEventToList(LuaState* L);
	int addPropertyToList(LuaState* L);
	int setLuaKeyWord(LuaState* L);
	int setLuaConstantWord(LuaState* L);

	int insertEditItem(LuaState* L);
	int insertComboboxItem(LuaState* L);

	int insertColorDlgItem(LuaState* L);

public:
	static CScript_System* Instance()
	{
		return m_instance;
	}
	//static LuaObject m_pObj;
private:
	static CScript_System* m_instance;
};
