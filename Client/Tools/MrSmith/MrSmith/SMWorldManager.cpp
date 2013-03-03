#include "StdAfx.h"
#include "SMWorldManager.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "SMVariable.h"
#include "SMObjectManager.h"
#include "SMObject.h"

namespace SMITH
{

/*************************************************************************
	Static Data Definitions
*************************************************************************/

/*************************************************************************
	Constructor
*************************************************************************/
WorldManager::WorldManager(Agent* pAgent)
	: LuaExport< WorldManager >("WorldManager", pAgent->getLuaState()),
	m_pAgent(pAgent)
{
	assert(pAgent);
}

/*************************************************************************
	Destructor
*************************************************************************/
WorldManager::~WorldManager()
{

}

/*************************************************************************
	Init world manager
*************************************************************************/
void WorldManager::init(void)
{
	// register lua function
	RegisterFunction("EnterScene", &WorldManager::Lua_EnterScene);
	
}

/*************************************************************************
	Release world manager
*************************************************************************/
void WorldManager::release(void)
{

}

/*************************************************************************
	Enter a scene
*************************************************************************/
bool WorldManager::enterScene(int id)
{
	//
	//TODO: Load logic scene resouce.
	//
	//  ...
	//

	//Create and init myself
	Object* pMySelf = m_pAgent->getObjectManager()->createObject(OT_PLAYERMYSELF, 
		m_pAgent->getVariable()->getAs_Int("MySelf_ID"));

	//set position
	std::pair< float, float > fvPos = m_pAgent->getVariable()->getAs_Vector2("MySelf_Pos");
	pMySelf->setPosition(fvPos.first, fvPos.second);

	return true;
}

/*************************************************************************
	[LUA]Enter a scene
*************************************************************************/
int WorldManager::Lua_EnterScene(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	//enterScene(m_pAgent->getVariable()->getAs_Int("Scene_ID"));

	return 0;
}


}