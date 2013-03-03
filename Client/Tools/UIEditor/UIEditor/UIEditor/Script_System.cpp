#include "StdAfx.h"
#include "Script_System.h"
#include "global.h"
#include "RightEventList.h"
#include "RightPropList.h"
#include "LuaDlg.h"

CScript_System* CScript_System::m_instance = NULL;
//LuaObject CScript_System::m_pObj;

CScript_System::CScript_System(void)
{
	m_instance = this;
}

CScript_System::~CScript_System(void)
{
}

int CScript_System::print(LuaState* L)
{
	LuaStack  arg(L);
	printf(arg[2].GetString());
	return 0;
}

int CScript_System::addEventToList(LuaState* L)
{
	LuaStack  arg(L);
	if (arg[2].IsString() && GetRightEventList())
	{
		GetRightEventList()->InsertItemS(arg[2].GetString(),"SpecialWindowEvent");
	}
	return 0;
}

int CScript_System::addPropertyToList(LuaState* L)
{
	LuaStack  arg(L);
	if (arg[2].IsString() && arg[3].IsString() && GetRightEventList())
	{
		GetRightPropList()->InsertItemS(arg[2].GetString(),arg[3].GetString());
	}
	return 0;
}

int CScript_System::setLuaKeyWord(LuaState* L)
{
	LuaStack  arg(L);
	if (arg[2].IsString() && g_luaDlg)
	{
		g_luaDlg->AddKeywords(arg[2].GetString());
	}
	return 0;
}
int CScript_System::setLuaConstantWord(LuaState* L)
{
	LuaStack  arg(L);
	if (arg[2].IsString() && g_luaDlg)
	{
		g_luaDlg->AddConstants(arg[2].GetString());
	}
	return 0;
}

int CScript_System::insertEditItem(LuaState* L)
{
	LuaStack  arg(L);

	if (arg[2].IsString() && arg[3].IsString() && arg[4].IsString())
	{
		CRightPropList* pList = GetRightPropList();

		pList->InsertEditItem(arg[2].GetString(), arg[3].GetString(), "",
			OTES_NORMAL, arg[2].GetString(), arg[4].GetString());
	}
	return 0;
}


int CScript_System::insertComboboxItem(LuaState* L)
{
	LuaStack  arg(L);

	if (arg[2].IsString() && arg[3].IsString() && arg[4].IsString())
	{

		StringVector vec;
		int i=5;

		vec.push_back("");

		while (arg[i].IsString())
		{
			vec.push_back(arg[i++].GetString());
		}

		CRightPropList* pList = GetRightPropList();

		pList->InsertComboItem(arg[2].GetString(), arg[3].GetString(),vec,
			OTCS_NORMAL, arg[2].GetString(), arg[4].GetString());

	}

	return 0;
}

int CScript_System::insertColorDlgItem(LuaState* L)
{
	LuaStack  arg(L);

	if (arg[2].IsString() && arg[3].IsString() && arg[4].IsString())
	{

		CRightPropList* pList = GetRightPropList();

		pList->InsertColorItem(arg[2].GetString(), arg[3].GetString(),"",
			OTCOLS_NORMAL, arg[2].GetString(), arg[4].GetString());

	}

	return 0;
}