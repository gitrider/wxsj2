#include "StdAfx.h"
#include "SMLog.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "Resource.h"
#include "AgentInfo_LogDlg.h"
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
Log::Log(Agent* pAgent)
	: LuaExport< Log >("Loger", pAgent->getLuaState()),
	m_pAgent(pAgent)
{
	assert(pAgent);
}

/*************************************************************************
	Destructor
*************************************************************************/
Log::~Log()
{
}

/*************************************************************************
	Init log manager
*************************************************************************/
void Log::init(void)
{
	// register lua function
	RegisterFunction("AddLog", &Log::Lua_Log);
	
}

/*************************************************************************
	Release log manager
*************************************************************************/
void Log::release(void)
{

}

/*************************************************************************
	Add a formated log
*************************************************************************/
void Log::addLog(const char* pszContents)
{
	if(!pszContents) return;

	Log_t newLog;
	::GetLocalTime(&(newLog.m_time));

	newLog.m_contents = pszContents;

	//-----------------
	System::GetMe()->lockLog();

	m_vLogBuf.push_back(newLog);
	while( m_vLogBuf.size() > MAX_LOG_NUM ) m_vLogBuf.pop_front();
	
	//check is selected
	if(System::GetMe()->getSelectedAgent() == m_pAgent->getID())
	{
		CMainWnd::GetMe()->getLogDlg()->addLog(newLog.m_time, newLog.m_contents);
	}

	System::GetMe()->unlockLog();
	//-----------------
}

/*************************************************************************
	Add a formated log
*************************************************************************/
void Log::addLogFmt(const char* pszFmt, ...)
{
	char szTemp[1024] = {0};
	va_list ptr; va_start(ptr, pszFmt);
	_vsnprintf((char *)szTemp, 1024, pszFmt, ptr);
	va_end(ptr);

	addLog(szTemp);
}

/*************************************************************************
	Add a log(Lua interface)
*************************************************************************/
int Log::Lua_Log(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		Util::throwException("Log::addLog[2] param parameter error");
	}

	addLog(args[2].GetString());
	return 0;
}

/*************************************************************************
	Dump all log to log window
*************************************************************************/
void Log::dumpLog(CAgentInfo_LogDlg* pLogDlg)
{
	std::list< Log_t >::iterator it;
	for(it=m_vLogBuf.begin(); it!=m_vLogBuf.end(); it++)
	{
		const Log_t& theLog = *it;

		pLogDlg->addLog(theLog.m_time, theLog.m_contents);
	}
}

void Log::dumpLog()
{
	char szTemp[512];
	sprintf( szTemp, "agent%04d.log", m_pAgent->getID() );
	FILE* fp=fopen( szTemp, "a" );
	if( !fp ) return;

	std::list< Log_t >::iterator it;
	for(it=m_vLogBuf.begin(); it!=m_vLogBuf.end(); it++)
	{
		const Log_t& theLog = *it;
		sprintf(szTemp, "%02d_%02d:%02d:%02d %s\r\n", 
			theLog.m_time.wDay, theLog.m_time.wHour, theLog.m_time.wMinute, theLog.m_time.wSecond, theLog.m_contents.c_str() );
		fwrite( szTemp, 1, strlen( szTemp ), fp );
		Sleep( 1 );
	}

	fclose( fp );
}

}