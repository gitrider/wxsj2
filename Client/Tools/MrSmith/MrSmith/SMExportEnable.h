/************************************\
*									*
*		ExportEnableInterface		*
*			         by fangyuanl.	*
*									*
\************************************/

#pragma once

#include "LuaPlus.h"

namespace SMITH
{

template< class T >
class LuaExport
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	LuaExport(const char* pszName, LuaPlus::LuaState* pState)
	{ 
		assert(pState);

		m_pMetaTable = new LuaPlus::LuaObject(pState);
		*m_pMetaTable = pState->GetGlobals().CreateTable(pszName);
		m_pMetaTable->SetObject("__index", *m_pMetaTable);

		LuaObject obj = pState->BoxPointer(this);
		obj.SetMetaTable(*m_pMetaTable);
		pState->GetGlobals().SetObject(pszName, obj);
	}

	virtual ~LuaExport()
	{
		delete m_pMetaTable; m_pMetaTable = 0;
	}

	/*!
	\brief
		Register function.
	*/
	template <class Callee>
	void RegisterFunction(const char* pszFuncName, int (Callee::*func)(LuaPlus::LuaState*))
	{
		m_pMetaTable->RegisterObjectFunctor(pszFuncName, func);
	}

protected:
	LuaPlus::LuaObject*	m_pMetaTable;
};

}
