/****************************************\
*										*
*		   Util Function				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

#include "SMExportEnable.h"

namespace SMITH
{

//Forward reference declarations
class Agent;

class Util : public LuaExport< Util >
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/
	/*!
	\brief
		Init Util
	\return
	*/
	void init(void);
	/*!
	\brief
		release Util
	\return
	*/
	void release(void);

	/*************************************************************************
		Util function
	*************************************************************************/
	/*!
	\brief
		Hash a string to unsigned int.
	\return
	*/
	static unsigned int hashString2CRC(const char* szName);

	/*!
	\brief
		Create a rand name.
	\return
	*/
	static std::string generateName(void);

	/*!
	\brief
		Throw exception
	\return
	*/
	static void throwException(const char* szFmt, ...);

	/*!
	\brief
		hash string
	*/
	static std::string md5String(const char* pStr);

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/

	/*!
	\brief
		Hash a string to unsigned int.

		param0[string] - the contents of log.

	\return
		return0[int]	- the hash value of string.
	*/
	int Lua_HashString2CRC(LuaPlus::LuaState* state);

	/*!
	\brief
		Create a rand name.

	\return
		return0[string]	- rand name
	*/
	int Lua_CreateRandName(LuaPlus::LuaState* state);

	/*!
	\brief
		Throw a exception

		param0[string] - the contents of log.

	\return
	*/
	int Lua_ThrowException(LuaPlus::LuaState* state);

	int Lua_Sleep(LuaPlus::LuaState* state);

	int Lua_GetTickCount(LuaPlus::LuaState* state);

	int Lua_GetSmithCount(LuaPlus::LuaState* state);

	int Lua_MsgBox(LuaPlus::LuaState* state);

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;			//!< Owner.

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Util(Agent* pAgent);
	~Util();
};


}