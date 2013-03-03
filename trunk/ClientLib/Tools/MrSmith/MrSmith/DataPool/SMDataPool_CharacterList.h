#pragma once

#include "..\SMExportEnable.h"

//Forward reference declarations
struct DB_CHAR_BASE_INFO;

namespace SMITH
{
//Forward reference declarations
class Agent;

// character data when login
class LoginCharacterList : public LuaExport< LoginCharacterList >
{
public:
	/** Insert a character data.
	*/
	void insertCharInfo(DB_CHAR_BASE_INFO* pCharInfo);

	/** get char counts
	*/
	int getCharCounts(void) const { return (int)m_vCharacterVector.size(); }

	/** get special char
	*/
	const DB_CHAR_BASE_INFO* getCharacter(int nIndex);

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/
	/*!
	\brief
		Get character counts

		return - character counts
	\return
		None.
	*/
	int Lua_GetCharacterCounts(LuaPlus::LuaState* state);

private:
	std::vector< DB_CHAR_BASE_INFO* > m_vCharacterVector;

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	LoginCharacterList(Agent* pAgent);
	~LoginCharacterList();
};


}