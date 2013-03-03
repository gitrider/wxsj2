#include "StdAfx.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "SMSystem.h"
#include "LegendWnd.h"
#include "LuaPlus.h"
#include "SMProcedureManager.h"
#include "SMLog.h"
#include "SMInfo.h"
#include "SMNetManager.h"
#include "SMVariable.h"
#include "SMWorldManager.h"
#include "SMUtil.h"
#include "SMObjectManager.h"
#include "DataPool\SMDataPool.h"

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

namespace SMITH
{


/*************************************************************************
	Constructor
*************************************************************************/
Agent::Agent(int id, int idLegend, const AgentCreateParam& param)
	: m_nID(id)
	, m_nLegendID(idLegend)
	, m_paramCreate(param)
	, m_state(Ready)
	, m_hWorkThread(0)
	, m_hStopSignal(0)
	, m_pProcedureManager(0)
	, m_pLog(0)
	, m_pInfo(0)
	, m_pWorldManager(0)
	, m_pNetManager(0)
	, m_pLuaState(0)
	, m_pObjectManager(0)
	, m_pVariable(0)
	, m_pUtil(0)
	, m_tLastTick(0)
	, m_tHeartBeatTick(0)
	, m_pDataPool(0)
{
}

/*************************************************************************
	Destructor
*************************************************************************/
Agent::~Agent()
{
	stop();

	//destroy stop control
	::CloseHandle(m_hStopSignal);
	m_hStopSignal = 0;
}

/*************************************************************************
	Agent working thread enter point.
*************************************************************************/
unsigned int Agent::_agentWorkingThread(void* pParam)
{
	return pParam ? ((Agent*)pParam)->WorkingThread() : 0;
}

/*************************************************************************
	Agent working thread
*************************************************************************/
unsigned int Agent::WorkingThread(void)
{
	//Init work
	initWork();

	while(true)
	{
		//Pause control
		::WaitForSingleObject(System::GetMe()->getResumeHandle(), INFINITE);

		//Stop control
		if(m_hStopSignal && WAIT_OBJECT_0 == ::WaitForSingleObject(m_hStopSignal, 0)) 
		{
			::CloseHandle(m_hStopSignal); m_hStopSignal = 0;
			break;
		}

		//Fault!
		if(Fault == getState()) 
		{
			//todo...
		}
		else
		{
			//do tick work
			doWork();
		}

		// release cpu.
		Sleep(5);
	};
	
	//Release work...
	releaseWork();

	//close thread handle
	::CloseHandle(m_hWorkThread);
	m_hWorkThread = 0;

	return 0;
}

/*************************************************************************
	Start working.
*************************************************************************/
bool Agent::start(void)
{
	if(Ready != getState()) return false;

	//create working thread.
	unsigned int nThreadID;
	m_hWorkThread = (HANDLE)::_beginthreadex(NULL, 0, _agentWorkingThread, this, CREATE_SUSPENDED|THREAD_QUERY_INFORMATION, &nThreadID );
	if (m_hWorkThread == NULL) return false;

	// work now!
	DWORD dwResumeCount = 0;
	DWORD dwLastError = 0;
	while( ResumeThread(m_hWorkThread) == (DWORD)-1 ) {
		if( dwLastError == 0 ) dwLastError = GetLastError();
		dwResumeCount ++;
		Sleep( 100 );
		if( dwResumeCount == 100 ) {
			try {
				::TerminateThread( m_hWorkThread, 0 );
				::CloseHandle(m_hWorkThread);
				m_hWorkThread = NULL;
			}
			catch( ... ) {}
#ifdef _DEBUG
			char szInfo[1024];
			sprintf( szInfo, "can't resume thread(%d), errorcode:%d", m_nID, dwLastError );
			OutputDebugString( szInfo );
#endif
			return false;
		}
	}
	m_tLastTick = timeGetTime();
	m_tHeartBeatTick = timeGetTime();
	
	return true;
}

/*************************************************************************
	Stop working.
*************************************************************************/
void Agent::stop(void)
{
	if(Working != getState() && Fault != getState()) return;

	if(m_hStopSignal == 0)
	{
		m_hStopSignal = ::CreateEvent(0, FALSE, FALSE, 0);
	}
	::SetEvent(m_hStopSignal);
}

/*************************************************************************
	do tick working.
*************************************************************************/
void Agent::doWork(void)
{
	if(Fault == getState()) return;

	try 
	{
		// time control 
		const float FPS = 30.0F;

		unsigned int timeNow = timeGetTime();
		if(timeNow - m_tLastTick < 1000.0f/FPS) return;
		
		float fElapsTime = (float)(timeNow-m_tLastTick);

		m_pNetManager->tick(fElapsTime);
		m_pProcedureManager->tick(fElapsTime);
		m_pObjectManager->tick(fElapsTime);
		m_pDataPool->tick(fElapsTime);

		m_tLastTick = timeNow;
	}
	catch(const std::exception& e)
	{
		setState(Fault);
		m_pLog->addLog(e.what());
	}
	catch(...)
	{
		setState(Fault);
		m_pLog->addLog("----UNKNOWN EXCEPTION----");
	}
}

/*************************************************************************
	Set the Agent state.
*************************************************************************/
void Agent::setState(State state)
{
	switch(state)
	{
	case Ready:
		CLegendWnd::GetMe()->SetLegendState(this->m_nLegendID, CLegendWnd::LS_DIE);
		break;

	case Working:
		CLegendWnd::GetMe()->SetLegendState(this->m_nLegendID, CLegendWnd::LS_LIVE_0);
		break;

	case Fault:
		CLegendWnd::GetMe()->SetLegendState(this->m_nLegendID, CLegendWnd::LS_FAULT);
		break;
	default:
		return;
	}
	m_state = state;
}

/*************************************************************************
	Init for this agent, create xxManager etc.
*************************************************************************/
void Agent::initWork(void)
{
	assert(Ready == getState());

	//set state
	setState(Working);

	//create
	m_pLuaState = LuaPlus::LuaState::Create(true, true);
	m_pProcedureManager = new ProcedureManager(this);
	m_pLog				= new Log(this);
	m_pInfo				= new Info(this);
	m_pNetManager		= new NetManager(this);
	m_pVariable			= new Variable(this);
	m_pWorldManager		= new WorldManager(this);
	m_pUtil				= new Util(this);
	m_pObjectManager	= new ObjectManager(this);
	m_pDataPool			= new DataPool(this);

	//init
	m_pProcedureManager->init();
	m_pLog->init();
	m_pInfo->init();
	m_pNetManager->init();
	m_pVariable->init();
	m_pWorldManager->init();
	m_pUtil->init();
	m_pObjectManager->init();
	m_pDataPool->init();

	m_pLog->addLog	 ("==========================");
	m_pLog->addLogFmt("        Agent:%d", getID());
	m_pLog->addLog	 ("==========================");

	//init variable
	for(int i=0; i<(int)m_paramCreate.vParams.size(); i++)
	{
		m_pVariable->setVariable(
			m_paramCreate.vParams[i].first.c_str(),
			m_paramCreate.vParams[i].second.c_str());
	}

	//get template
	const AgentTemplate* pTemplate = 
		AgentManager::GetMe()->getAgentTemplate(m_paramCreate.strTemplate.c_str());
	//should not empty
	assert(pTemplate);

	for(int i=0; i<(int)pTemplate->m_vScriptFiles.size(); i++)
	{
		char szScriptFile[MAX_PATH] = {0};
		strncpy(szScriptFile, pTemplate->m_strWorkFold.c_str(), MAX_PATH);
		::PathAppend(szScriptFile, pTemplate->m_vScriptFiles[i].c_str());

		m_pLuaState->DoFile(szScriptFile);
	}

	//do lua init
	m_pLuaState->DoString("Agent_Init()");
}

/*************************************************************************
	Release all work of this agent.
*************************************************************************/
void Agent::releaseWork(void)
{
	assert(Working == getState() || Fault == getState());

	m_pDataPool->release();
	m_pObjectManager->release();
	m_pUtil->release();
	m_pWorldManager->release();
	m_pVariable->release();
	m_pNetManager->release();
	m_pLog->release();
	m_pInfo->release();
	m_pProcedureManager->release();

	//release
	delete m_pDataPool; m_pDataPool = 0;
	delete m_pObjectManager; m_pObjectManager = 0;
	delete m_pUtil; m_pUtil = 0;
	delete m_pWorldManager; m_pWorldManager = 0;
	delete m_pVariable; m_pVariable = 0;
	delete m_pNetManager; m_pNetManager = 0;
	delete m_pLog; m_pLog = 0;
	delete m_pInfo; m_pInfo = 0;
	delete m_pProcedureManager; m_pProcedureManager = 0;
	LuaPlus::LuaState::Destroy(m_pLuaState); m_pLuaState = 0;

	// set state
	setState(Ready);
}

}
