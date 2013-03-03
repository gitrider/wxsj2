/****************************************\
*										*
* 				AgentLog				*
*										*
*						 by fangyuan		*
\****************************************/

#pragma once

#include "SMExportEnable.h"

//Forward reference declarations
class CAgentInfo_LogDlg;

namespace SMITH
{

const size_t MAX_LOG_NUM = 200;
//Forward reference declarations
class Agent;

class Log : public LuaExport< Log >
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
		add a log as string
	\return
	*/
	void addLog(const char* pszContents);

	/*!
	\brief
		add a log as formt
	\return
	*/
	void addLogFmt(const char* pszFmt, ...);

	/*!
	\brief
		dump all log to window
	\return
	*/
	void dumpLog(CAgentInfo_LogDlg* pLogDlg);

	void dumpLog();

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/

	/*!
	\brief
		Add a log of this agent

		param0[string] - the contents of log.

	\return
		None.
	*/
	int Lua_Log(LuaPlus::LuaState* state);
	

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;			//!< Owner.

	struct Log_t
	{
		SYSTEMTIME	m_time;
		std::string	m_contents;
	};
	std::list< Log_t >			m_vLogBuf;	//< Queue of log msg

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Log(Agent* pAgent);
	~Log();
};

}