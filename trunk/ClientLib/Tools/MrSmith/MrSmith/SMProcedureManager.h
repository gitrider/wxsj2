/****************************************\
*										*
* 			ProcedureManager			*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

#include "SMExportEnable.h"

namespace SMITH
{

//Forward reference declarations
class Agent;

class ProcedureManager : public LuaExport< ProcedureManager >
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
		tick
	\return
	*/
	void tick(float fElapsTime);

	/*!
	\brief
		SetCurrentProcedure
	\return
	*/
	void SetCurrentProcedure(const char* szProName);

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/

	/*!
	\brief
		set current procedure

		param0[string] - the name of procedure.

	\return
		None.
	*/
	int Lua_SetCurrentProcedure(LuaPlus::LuaState* state);
	

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*		m_pAgent;				//!< Owner.

	std::string m_strPrevProcedure;		//!< Prev procedure name.
	std::string	m_strCurrentProcedure;	//!< Current procedure name.

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	ProcedureManager(Agent* pAgent);
	~ProcedureManager();
};

}