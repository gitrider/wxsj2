
/** �ܹ�������Lua��ģ���� 
 */

#pragma once


#include "..\Script\GMScriptSystem.h"
#include "GIException.h"


template< class T >
class LuaExport
{
public:
	LuaExport(const char* pszName, LuaPlus::LuaState* pState = 0)
	{ 
		KLAssert(s_pMe == 0);
		s_pMe = static_cast< T* >(this);

		LuaPlus::LuaState* theState = pState ? pState : CScriptSystem::_GetLuaState();

		s_pMetaTable = new LuaPlus::LuaObject;
		*s_pMetaTable = theState->GetGlobals().CreateTable(pszName);
		s_pMetaTable->SetObject("__index", *s_pMetaTable);

		LuaObject obj = theState->BoxPointer(s_pMe);
		obj.SetMetaTable(*s_pMetaTable);
		theState->GetGlobals().SetObject(pszName, obj);
	}

	virtual ~LuaExport()
	{
		delete s_pMetaTable; s_pMetaTable = 0;
		s_pMe = 0;
	}

	template <class Callee>
	void RegisterFunction(const char* pszFuncName, int (Callee::*func)(LuaPlus::LuaState*))
	{
		s_pMetaTable->RegisterObjectFunctor(pszFuncName, func);
	}

	//ȡ���Լ�
	static T* GetMe(void)  { return s_pMe; }

protected:
	static T*					s_pMe;			//����ʵ����ȫ�ֱ���
	static LuaPlus::LuaObject*	s_pMetaTable;	//MetaTable

private:
    // unimplemented constructors / assignment
    LuaExport(const LuaExport& obj);
    LuaExport& operator=(const LuaExport& obj);
};

