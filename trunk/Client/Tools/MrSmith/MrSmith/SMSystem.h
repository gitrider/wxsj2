/****************************************\
*										*
* 			   SmithSystem				*
*										*
*						 by fangyuan		*
\****************************************/

#pragma once


//Forward reference declarations
class CAgentInfo_LogDlg;
class CAgentInfo_InfoDlg;

namespace SMITH
{

//Forward reference declarations
class AgentManager;
struct AgentCreateParam;

class System
{
public:
	/*************************************************************************
		Public enum
	*************************************************************************/

	// State of system
	enum State
	{
		Ready,			//!< Create just now.
		Working,		//!< Working...
		Pause,			//!< Pause!
		StopPedding,	//!< Stop pedding...
	};

	// Notify command to outside
	enum NotifyMsg
	{
		WMA_AGENT_NUM = WM_USER+1001,		//!< Agent num changed		wParam=Agent num
		WMA_STATE_CHANGED,					//!< System state changed
	};

public:
	/*************************************************************************
		Public interface
	*************************************************************************/
	/*!
	\brief
		Return the state of this agent object.

	\return
		
	*/
	State getState(void) const { return m_state; }

	/*!
	\brief
		Return the pause control handle(called by agent!)

	\return
		
	*/
	HANDLE getResumeHandle(void) const { return m_hResumeHandle; }

	/*!
	\brief
		Init Agent System

	\param hNotifyWnd
		The window handle that accept notity msg.

	\return
		None
	*/
	void init(HWND hNotifyWnd);

	/*!
	\brief
		Add new Agent to System

	\param nCounts
		the count of agents to create.

	\param pCreateParam
		a point to Agent::CreateParam struct

	\return
		None
	*/
	void addAgent(int nCounts, AgentCreateParam* pCreateParam);

	/*!
	\brief
		System Power on!

	\return
		None
	*/
	void start(void);

	/*!
	\brief
		System Power off!

	\return
		None
	*/
	void stop(void);

	/*!
	\brief
		System Pause

	\return
		None
	*/
	void pause(void);
	/*!
	\brief
		System resume

	\return
		None
	*/
	void resume(void);

	/*!
	\brief
		Legend window selected agent changed!

	\return
		None
	*/
	void onLegendSelectChanged(int idSelectAgent, CAgentInfo_LogDlg* pLogDlg, CAgentInfo_InfoDlg* pInfoDlg);

	/*!
	\brief
		Lock log thread lock

	\return
		None
	*/
	void lockLog(void) { EnterCriticalSection(&m_csLegendShare); }

	/*!
	\brief
		UnLock log thread lock

	\return
		None
	*/
	void unlockLog(void) { LeaveCriticalSection(&m_csLegendShare); }

	/*!
	\brief
		Get Selected agent by legend window

	\return
		None
	*/
	int getSelectedAgent(void) const { return m_idSelectAgent; }

	/*!
	\brief
		Create all agent create param to file.

	\param szFileName
		Filename to save
	*/
	bool saveAgentsCreateParam(const char* szFileName) const;

	/*!
	\brief
		Load agent create param from file

	\param szFileName
		Filename to load
	*/
	bool loadAgentsCreateParam(const char* szFileName);

private:
	/*************************************************************************
		Implementation Struct
	*************************************************************************/
	// Command type to lord
	enum LordCmdType
	{
		LCT_Nop,

		LCT_CreateAgent,		//!< Create agent		param1= Agent num to create param2= Is start
		LCT_ArouseAll,			//!< Arouse all agent	
		LCT_StopAll,			//!< Stop all agent
		LCT_ArouseStartFail,
	};

	// Command to lord
	struct LordCmd
	{
		LordCmdType type;			// Command Type 
		int			nParam1;		// Param1
		int			nParam2;		// Param2

		LordCmd():type(LCT_Nop), nParam1(0), nParam2(0)
		{
		}
		LordCmd(const LordCmd& ref) 
		{
			type = ref.type; nParam1 = ref.nParam1; nParam2 = ref.nParam2; 
		}
		LordCmd& operator=(const LordCmd& ref) 
		{ 
			type = ref.type; nParam1 = ref.nParam1; nParam2 = ref.nParam2; 
		}
	};
	typedef std::queue< LordCmd > LordCmdQueue;

	/*************************************************************************
		Implementation interface
	*************************************************************************/
	/*!
	\brief
		Set the current state.
	
	\return
		None
	*/
	void setState(State state);

	/*!
	\brief
		Push a command to lord thread
	
	\return
		None
	*/
	void pushLordCommand(LordCmdType type, int nParam1=0, int nParam2=0);

	/*!
	\brief
		Agent lord working thread enter point.
	*/
	static unsigned int __stdcall _lordWorkingThread(void* pParam);

	/*!
	\brief
		Agent lord working thread function
	*/
	unsigned int lordWorkingThread(void);

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	State				m_state;			//!< The main state of system.
	HWND				m_hNotifyWnd;		//!< The handle that accept notify msg.
	AgentManager*		m_pAgentManager;	//!< Agent Manager
	HANDLE				m_hResumeHandle;	//!< Handle to control resume state(s:resume n:pause);

	int					m_idSelectAgent;	//!< id of the selected agent(by legend window)
	CRITICAL_SECTION	m_csLegendShare;	//!< Legend wnd and agents share critical section.

	HANDLE				m_hLordThread;		//!< The lord thread(agent monitor)
	LordCmdQueue		m_qLordCmd;			//!< Queue of lord command
	CRITICAL_SECTION	m_csLordCmdQueue;	//!< Critical section for lord command queue.
	HANDLE				m_hLordCmdSign;		//!< The sign for cmd queue not empty(s:queue not empty n:empty)

	std::list<int>		m_hUnStartAgents;

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	System();
	~System();

	/*!
	\brief
		Return pointer to singleton System object

	\return
		Pointer to singleton System object
	*/
	static System* GetMe(void) { return s_pMe; }

private:
    // unimplemented constructors / assignment
    System(const System& obj);
    System& operator=(const System& obj);

	static System* s_pMe;
};

}