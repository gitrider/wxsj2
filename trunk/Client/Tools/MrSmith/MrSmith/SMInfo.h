/****************************************\
*										*
* 				AgentLog				*
*										*
*						 by fangyuan		*
\****************************************/

#pragma once

#include "SMExportEnable.h"

//Forward reference declarations
class CAgentInfo_InfoDlg;

namespace SMITH
{

const size_t MAX_INFO_NUM = 200;
//Forward reference declarations
class Agent;

class Info : public LuaExport< Info >
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

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/


protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;			//!< Owner.

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Info(Agent* pAgent);
	~Info();
};

}