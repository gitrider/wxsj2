/****************************************\
*										*
*		   WorldDataManager				*
*										*
*						 by fangyuan		*
\****************************************/

#pragma once

#include "SMExportEnable.h"

namespace SMITH
{

//Forward reference declarations
class Agent;

class WorldManager : public LuaExport< WorldManager >
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/

	/*!
	\brief
		Init manager
	\return
	*/
	void init(void);

	/*!
	\brief
		Release manager
	\return
	*/
	void release(void);

	/*!
	\brief
		Tickwork manager
	\return
	*/
	void tick(void);

	/*!
	\brief
		Enter a scene
	\param id
		the id of scene.

	\return
		Is success
	*/
	bool enterScene(int id);

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/
	/*!
	\brief
		Enter a scene

		param0[int] - the id of scene

	\return
		None.
	*/
	int Lua_EnterScene(LuaPlus::LuaState* state);

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;			//!< Owner.

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WorldManager(Agent* pAgent);
	~WorldManager();
};

}