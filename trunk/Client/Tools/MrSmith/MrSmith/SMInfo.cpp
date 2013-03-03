#include "StdAfx.h"
#include "SMInfo.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "Resource.h"
#include "AgentInfo_InfoDlg.h"
#include "SMSystem.h"
#include "MainWnd.h"
#include "SMUtil.h"

namespace SMITH
{

/*************************************************************************
	Static Data Definitions
*************************************************************************/

/*************************************************************************
	Constructor
*************************************************************************/
Info::Info(Agent* pAgent)
	: LuaExport< Info >("Info", pAgent->getLuaState()),
	m_pAgent(pAgent)
{
	assert(pAgent);
}

/*************************************************************************
	Destructor
*************************************************************************/
Info::~Info()
{
}

/*************************************************************************
	Init log manager
*************************************************************************/
void Info::init(void)
{
	
}

/*************************************************************************
	Release log manager
*************************************************************************/
void Info::release(void)
{

}

}