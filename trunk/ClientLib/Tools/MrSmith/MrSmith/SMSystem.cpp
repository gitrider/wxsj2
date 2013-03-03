#include "StdAfx.h"
#include "SMSystem.h"
#include "Resource.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "LegendWnd.h"
#include "AgentInfo_LogDlg.h"
#include "AgentInfo_InfoDlg.h"
#include "SMLog.h"
#include "SMInfo.h"

namespace SMITH
{

/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
System* System::s_pMe = 0;

const int AGENT_AROUSE_TIME_INTERVAL = 100;		//100ms

/*************************************************************************
	Constructor
*************************************************************************/
System::System() :
	m_state(Ready),
	m_pAgentManager(0),
	m_hResumeHandle(0),
	m_idSelectAgent(-1)
{
	s_pMe = this;

	// create resume control handle
	m_hResumeHandle = ::CreateEvent(0, TRUE, TRUE, 0);
	::SetEvent(m_hResumeHandle);

	// create critical section for Legend wnd and agents
	InitializeCriticalSection(&m_csLegendShare);

	// create critical section for lord cmd queue
	InitializeCriticalSection(&m_csLordCmdQueue);
	
	// create sign for lord queue.
	m_hLordCmdSign = ::CreateEvent(0, TRUE, FALSE, 0);
	::ResetEvent(m_hLordCmdSign);
}

/*************************************************************************
	Destructor
*************************************************************************/
System::~System()
{
	delete m_pAgentManager;
	
	//close sign for lord queue.
	::CloseHandle(m_hLordCmdSign);
	m_hLordCmdSign = 0;

	//close critical section for lord cmd queue
	DeleteCriticalSection(&m_csLordCmdQueue);

	//close resume control handle
	::CloseHandle(m_hResumeHandle);
	m_hResumeHandle = 0;

	//release critical section for Legend wnd and agents
	DeleteCriticalSection(&m_csLegendShare);

	s_pMe = 0;
}

/*************************************************************************
	Init Agent System
*************************************************************************/
void System::init(HWND hNotifyWnd)
{
	//keep notify win handle
	assert(hNotifyWnd);
	m_hNotifyWnd = hNotifyWnd;

	//Create lord thread
	unsigned int nThreadID;
	m_hLordThread = (HANDLE)::_beginthreadex(NULL, 0, _lordWorkingThread, this, CREATE_SUSPENDED|THREAD_QUERY_INFORMATION, &nThreadID );
	if (m_hLordThread == NULL)
	{
		throw std::string("Create lord thread error!");
		return;
	}
	// create agent manater
	char szWorkDir[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szWorkDir);
	::PathAppend(szWorkDir, "..\\Smith\\");
	m_pAgentManager = new AgentManager;
	m_pAgentManager->loadAgentTemplate(szWorkDir);

	// work now!
	ResumeThread(m_hLordThread);

	// set init state.
	setState(Ready);
}

/*************************************************************************
	Add new agent to Agent System
*************************************************************************/
void System::addAgent(int nCounts, AgentCreateParam* pCreateParam)
{
	bool bWorking = Working==getState() || Pause==getState();

	// Create init agent
	pushLordCommand(LCT_CreateAgent, (int)(INT_PTR)pCreateParam, 
		MAKELONG(bWorking ? 1 : 0, (short)nCounts));
}

/*************************************************************************
	System Power on!
*************************************************************************/
void System::start(void)
{
	if(Ready != getState()) return;

	//Start all agent
	pushLordCommand(LCT_ArouseAll);
	setState(Working);
}

/*************************************************************************
	System Power off!
*************************************************************************/
void System::stop(void)
{
	if(Working != getState() && Pause != getState()) return;

	//Start all agent
	pushLordCommand(LCT_StopAll);
	setState(StopPedding);
}

/*************************************************************************
	System Pause
*************************************************************************/
void System::pause(void)
{
	if(Working != getState()) return;

	::ResetEvent(m_hResumeHandle);
	setState(Pause);
}

/*************************************************************************
	System Resume
*************************************************************************/
void System::resume(void)
{
	if(Pause != getState()) return;

	::SetEvent(m_hResumeHandle);
	setState(Working);
}

/*************************************************************************
	Set the current state.
*************************************************************************/
void System::setState(State state)
{
	m_state = state;

	::SendMessage(m_hNotifyWnd, WMA_STATE_CHANGED, 0, 0);
}

/*************************************************************************
	Legend window selected agent changed!
*************************************************************************/
void System::onLegendSelectChanged(int idSelectAgent, CAgentInfo_LogDlg* pLogDlg, CAgentInfo_InfoDlg* pInfoDlg)
{
	//check 
	Agent* pAgent = m_pAgentManager->findAgent(idSelectAgent);
	if(!pAgent) return;

	//------------------------------------------
	//multi thread lock
	lockLog();

	//------------------------------------------
	//clean old log
	pLogDlg->clearAllLog();
	pInfoDlg->clearAllLog();

	m_idSelectAgent = idSelectAgent;

	//dump all log of this agent
	if(pAgent->getLog())
	{
		pAgent->getLog()->dumpLog(pLogDlg);
	}

	//------------------------------------------
	//multi thread Unlock
	unlockLog();
}

/*************************************************************************
	Push a command to lord thread
*************************************************************************/
void System::pushLordCommand(LordCmdType type, int nParam1, int nParam2)
{
	// enter critical section
	::EnterCriticalSection(&m_csLordCmdQueue);

	// push cmd
	LordCmd theCmd;
	theCmd.type = type;
	theCmd.nParam1 = nParam1;
	theCmd.nParam2 = nParam2;
	
	m_qLordCmd.push(theCmd);

	// leave critical section
	::LeaveCriticalSection(&m_csLordCmdQueue);

	// set sign 
	::SetEvent(m_hLordCmdSign);
}

/*************************************************************************
	Agent lord working thread enter point.
*************************************************************************/
unsigned int System::_lordWorkingThread(void* pParam)
{
	return pParam ? ((System*)pParam)->lordWorkingThread() : 0;
}

/*************************************************************************
	Agent lord working thread
*************************************************************************/
unsigned int System::lordWorkingThread(void)
{
	while(true)
	{
		//----------------------------------------
		// wait cmd sign
		::WaitForSingleObject(m_hLordCmdSign, INFINITE);

		// enter critical section
		::EnterCriticalSection(&m_csLordCmdQueue);
		if(m_qLordCmd.empty())
		{
			// leave critical section
			::LeaveCriticalSection(&m_csLordCmdQueue);
			continue;
		}
		// get cmd
		LordCmd theCmd = m_qLordCmd.front();
		m_qLordCmd.pop();

		if(m_qLordCmd.empty()) ::ResetEvent(m_hLordCmdSign);
		// leave critical section
		::LeaveCriticalSection(&m_csLordCmdQueue);

		//----------------------------------------
		//Process cmd
		switch(theCmd.type)
		{
		// create agents...
		case LCT_CreateAgent:
			{
				bool	bStartNow = LOWORD(theCmd.nParam2)==1;
				int		nCounts = (int)HIWORD(theCmd.nParam2);
				AgentCreateParam* pCreateparam = (AgentCreateParam*)(INT_PTR)theCmd.nParam1;

				// 保存登录用户名
				char		buf[128];
				int			iParam	= -1;
				std::string	sParam;
				for( int j=0; j<(int)pCreateparam->vParams.size(); j++ )
				{
					if( pCreateparam->vParams[j].first == "AccountName" )
					{
						iParam		= j;
						sParam		= pCreateparam->vParams[j].second;
						break;
					}
				}

				// 形如000X时，特殊处理
				int			iNum	= atoi( sParam.substr( sParam.length()-4 ).c_str() );

				for( int i=0; i<nCounts; i++ )
				{
					if( i == 1 )
					{
						sParam		= sParam.substr( 0, sParam.length()-4 );
					}

					// 一组多用户命名规则
					if( i > 0 && iParam >= 0 )
					{
						sprintf( buf, "%04d@game.sohu.com", i + iNum );
						pCreateparam->vParams[iParam].second	= sParam + buf;
					}
					else
					{
						sprintf( buf, "@game.sohu.com" );
						pCreateparam->vParams[iParam].second	= sParam + buf;
					}

					Agent* pAgent = m_pAgentManager->createAgent(*pCreateparam);

					if(bStartNow) 
					{
						if( pAgent->start() == false )
							m_hUnStartAgents.push_back( pAgent->getID() );
						::Sleep(AGENT_AROUSE_TIME_INTERVAL);
					}

					::SendMessage(m_hNotifyWnd, WMA_AGENT_NUM, m_pAgentManager->getAgentNum(), 0);
				}
				//delete org createparam
				delete pCreateparam; pCreateparam=0;

				if( m_hUnStartAgents.size() > 0 ) pushLordCommand( LCT_ArouseStartFail );
			}
			break;
		// arouse agent...
		case LCT_ArouseAll:
			{
				//Start all agent
				AgentManager::AgentIterator agent = m_pAgentManager->getAgentIterator();
				while (!agent.isAtEnd())
				{
					agent.getCurrentValue()->start();
					++agent;
					::Sleep(AGENT_AROUSE_TIME_INTERVAL);
				}
				if( m_hUnStartAgents.size() > 0 ) pushLordCommand( LCT_ArouseStartFail );
			}
			break;
		// stop all agent...
		case LCT_StopAll:
			{
				// resume all first
				::SetEvent(m_hResumeHandle);

				// send stop sign
				int nAgentNum = m_pAgentManager->getAgentNum();
				int nAgentTeam = nAgentNum / MAXIMUM_WAIT_OBJECTS + 1;
				HANDLE (*hAllWorkThread)[MAXIMUM_WAIT_OBJECTS] = new HANDLE[nAgentTeam][MAXIMUM_WAIT_OBJECTS];
				AgentManager::AgentIterator agent = m_pAgentManager->getAgentIterator();
				int nIndex = 0;
				int nOffset = 0;
				while (!agent.isAtEnd())
				{
					hAllWorkThread[nOffset][nIndex++] = agent.getCurrentValue()->getWorkThread();
					if( nIndex == MAXIMUM_WAIT_OBJECTS ) { nIndex = 0; nOffset ++; }
					agent.getCurrentValue()->stop();
					++agent;
				}

				//wait for all thread stop(10 sec)...
				for( int i = 0; i < nAgentTeam; i ++ )
				{
					int nWait = i == nAgentTeam - 1 ? nIndex : MAXIMUM_WAIT_OBJECTS;
					if( nWait == 0 ) break;
					if(WAIT_TIMEOUT == ::WaitForMultipleObjects(nWait, hAllWorkThread[i], TRUE, 10*1000)) {
						//TODO ...
						// Terminate all thread!
						int a=0;
					}
				}

				delete [] hAllWorkThread;
				setState(Ready);

				break;
			}
			break;
		case LCT_ArouseStartFail:
			{
				for( std::list<int>::iterator it = m_hUnStartAgents.begin(); it != m_hUnStartAgents.end(); ) {
					Agent* pAgent = m_pAgentManager->findAgent( *it );
					if( pAgent ) {
						if( pAgent->start() == true ) {
							m_hUnStartAgents.pop_front();
						}
						else {
							++ it;
						}
						Sleep( 100 );
					}
					else {
						m_hUnStartAgents.pop_front();
					}
				}
				if( m_hUnStartAgents.size() > 0 ) pushLordCommand( LCT_ArouseStartFail );
			}
			break;

		default:
			break;
		}

	};

	return 0;
}

/*************************************************************************
	Create all agent create param to file.
*************************************************************************/
bool System::saveAgentsCreateParam(const char* szFileName) const
{
	return m_pAgentManager->saveAgentsCreateParam(szFileName);
}

/*************************************************************************
	Create all agent create param to file.
*************************************************************************/
bool System::loadAgentsCreateParam(const char* szFileName)
{
	if(!::PathFileExists(szFileName)) return false;

	//get agent counts
	int nCounts = ::GetPrivateProfileInt("Global", "AgentCounts", 0, szFileName);
	if(0==nCounts) return false;

	for(int i=0; i<nCounts; i++)
	{
		//read param
		AgentCreateParam& theParam = *(new AgentCreateParam);

		char szAgentName[32] = {0};
		_snprintf(szAgentName, 32, "Agent%d", i);

		char szTemp[1024] = {0};

		::GetPrivateProfileString(szAgentName, "Template", "", szTemp, 1024, szFileName);
		theParam.strTemplate = szTemp;

		int nParamNum = ::GetPrivateProfileInt(szAgentName, "ParamNum", 0, szFileName);
		theParam.vParams.resize(nParamNum);
		for(int j=0; j<nParamNum; j++)
		{
			char szParam[32];
			
			_snprintf(szParam, 32, "ParamName%d", j);
			::GetPrivateProfileString(szAgentName, szParam, "", szTemp, 1024, szFileName);
			theParam.vParams[j].first = szTemp;

			_snprintf(szParam, 32, "ParamValue%d", j);
			::GetPrivateProfileString(szAgentName, szParam, "", szTemp, 1024, szFileName);
			theParam.vParams[j].second = szTemp;
		}

		//add a agent 
		addAgent(1, &theParam);
	}

	return true;
}

}