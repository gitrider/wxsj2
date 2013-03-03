#include "StdAfx.h"
#include "SMDataPool_CharacterList.h"
#include "..\SMAgentManager.h"
#include "..\SMAgent.h"

#include "Type.h"
#include "GameStruct.h"

namespace SMITH
{

LoginCharacterList::LoginCharacterList(Agent* pAgent)
	: LuaExport< LoginCharacterList >("LoginCharacterList", pAgent->getLuaState())
{
	RegisterFunction("GetCharacterCounts", Lua_GetCharacterCounts);
}

LoginCharacterList::~LoginCharacterList()
{
	for(int i=0; i<(int)m_vCharacterVector.size(); i++)
	{
		delete m_vCharacterVector[i];
		m_vCharacterVector[i]=0;
	}

	m_vCharacterVector.clear();
}

void LoginCharacterList::insertCharInfo(DB_CHAR_BASE_INFO* pCharInfo)
{
	DB_CHAR_BASE_INFO* pNew = new DB_CHAR_BASE_INFO;
	*pNew = *pCharInfo;
	m_vCharacterVector.push_back(pNew);
}

const DB_CHAR_BASE_INFO* LoginCharacterList::getCharacter(int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vCharacterVector.size()) return 0;

	return m_vCharacterVector[nIndex];
}

int LoginCharacterList::Lua_GetCharacterCounts(LuaPlus::LuaState* state)
{
	state->PushInteger(getCharCounts());
	return 1;
}


}