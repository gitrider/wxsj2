#include "StdAfx.h"
#include "SMProcedureManager.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "SMLog.h"
#include "SMUtil.h"

namespace SMITH
{

/*************************************************************************
	Static Data Definitions
*************************************************************************/

/*************************************************************************
	Constructor
*************************************************************************/
ProcedureManager::ProcedureManager(Agent* pAgent)
	: LuaExport< ProcedureManager >("Procedure", pAgent->getLuaState()),
	m_pAgent(pAgent)
{
	assert(pAgent);
}

/*************************************************************************
	Destructor
*************************************************************************/
ProcedureManager::~ProcedureManager()
{
}

/*************************************************************************
	Init manager
*************************************************************************/
void ProcedureManager::init(void)
{
	// register lua function
	RegisterFunction("SetCurrentProcedure", Lua_SetCurrentProcedure);

}

/*************************************************************************
	Tick
*************************************************************************/
void ProcedureManager::tick(float fElapsTime)
{
	if(m_strPrevProcedure != m_strCurrentProcedure)
	{
		char szReleasePrvProcedure[MAX_PATH];
		_snprintf(szReleasePrvProcedure, MAX_PATH, "Robot_ExcuteProcedure(\"%s\", \"Release\");", 
			m_strPrevProcedure.c_str());
		m_pAgent->getLuaState()->DoString(szReleasePrvProcedure);

		char szInitProcedure[MAX_PATH];
		_snprintf(szInitProcedure, MAX_PATH, "Robot_ExcuteProcedure(\"%s\", \"Init\");", 
			m_strCurrentProcedure.c_str());
		m_pAgent->getLuaState()->DoString(szInitProcedure);

		m_strPrevProcedure = m_strCurrentProcedure;
	}

	char szTickProcedure[MAX_PATH];
	_snprintf(szTickProcedure, MAX_PATH, "Robot_ExcuteProcedure(\"%s\", \"Tick\");", 
			m_strCurrentProcedure.c_str());
	m_pAgent->getLuaState()->DoString(szTickProcedure);
}

/*************************************************************************
	SetCurrentProcedure
*************************************************************************/
void ProcedureManager::SetCurrentProcedure(const char* szProName)
{
	std::string	strProName	= szProName;
	if( strProName.empty() || strProName == m_strCurrentProcedure )
		return;

	// set current procedure name.
	m_strPrevProcedure		= m_strCurrentProcedure;
	m_strCurrentProcedure	= strProName;
	
	m_pAgent->getLog()->addLogFmt("ChangeProcedure: %s", strProName.c_str());
}

/*************************************************************************
	Release procedure manager
*************************************************************************/
void ProcedureManager::release(void)
{

}

/*************************************************************************
	set current procedure
*************************************************************************/
int ProcedureManager::Lua_SetCurrentProcedure(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		Util::throwException("ProcedureManager::SetCurrentProcedure[2] param parameter error");
	}

	SetCurrentProcedure( args[2].GetString() );
	return 0;
}

}