/****************************************\
*										*
* 				Agent					*
*										*
*						 by fangyuan		*
\****************************************/

#pragma once

namespace LuaPlus { class LuaState; }

namespace SMITH
{
//Forward reference declarations
class ProcedureManager;
class Log;
class Info;
class WorldManager;
class NetManager;
class ObjectManager;
class Variable;
class Util;
class DataPool;

//param for create agents
struct AgentCreateParam
{
	typedef std::vector< std::pair< std::string, std::string > >  Params;

	std::string strTemplate;	//!< template name
	Params vParams;				//!< Params buf to create
};

class Agent
{
public:
	/*************************************************************************
		Public enum
	*************************************************************************/
	enum State
	{
		Ready,		//!< Just create just now.
		Working,	//!< Working...
		Fault,		//!< Fault!
	};
	
	enum NotifyType
	{
		NT_ON_UNSELECTED,	//!< UnSelected at legend window
		NT_ON_SELECTED,		//!< Selected at legend window
	};

	/*************************************************************************
		Public struct
	*************************************************************************/
	struct NotifyMsg
	{
		NotifyType	type;		//!< Notify type
		DWORD		dwParam1;	//!< Param1
		DWORD		dwParam2;	//!< Param2
	};

public:
	/*************************************************************************
		Public interface
	*************************************************************************/

	/*!
	\brief
		Return the id of this agent object.
	*/
	int	getID(void) const { return m_nID; }

	/*!
	\brief
		Return the state of this agent object.
	*/
	State getState(void) const { return m_state; }
	
	/*!
	\brief
		Start working.
	*/
	bool start(void);
	/*!
	\brief
		Stop working.
	*/
	void stop(void);

	/*!
	\brief
		Get Luastate for this agent.
	*/
	LuaPlus::LuaState* getLuaState(void) { return m_pLuaState; }

	/*!
	\brief
		Get the log of this agent.
	*/
	Log* getLog(void) { return m_pLog; }

	/*!
	\brief
		Get the info of this agent.
	*/
	Info* getInfo(void) { return m_pInfo; }

	/*!
	\brief
		Get the netmanager of this agent.
	*/
	NetManager* getNetManager(void) { return m_pNetManager; }

	/*!
	\brief
		Get the variable manager
	*/
	Variable* getVariable(void) { return m_pVariable; }

	/*!
	\brief
		Get the World manager
	*/
	WorldManager* getWorldManager(void) { return m_pWorldManager; }

	/*!
	\brief
		Get the object manager
	*/
	ObjectManager* getObjectManager(void) { return m_pObjectManager; }

	/*!
	\brief
		Get the datapool
	*/
	DataPool* getDataPool(void) { return m_pDataPool; }

	/*!
	\brief
		Get work thread(called by system only)
	*/
	HANDLE getWorkThread(void) { return m_hWorkThread; }

	/*!
	\brief
		Get create params(const)
	*/
	const AgentCreateParam& getCreateParam(void) const { return m_paramCreate; }

	/*!
	\brief
		Get the procedure manager
	*/
	ProcedureManager* getProcedureManager(void) { return m_pProcedureManager; }

protected:
	/*************************************************************************
		Implementation Interface
	*************************************************************************/
	/*!
	\brief
		Init for this agent, create xxManager etc.
	\return
	*/
	void initWork(void);

	/*!
	\brief
		do tick work.
	\return
	*/
	void doWork(void);

	/*!
	\brief
		Release all work of this agent.
	\return
	*/
	void releaseWork(void);

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	int					m_nID;				//!< Identity for this agent.
	int					m_nLegendID;		//!< Identity of the legend.
	AgentCreateParam	m_paramCreate;		//!< Params to create
	State				m_state;			//!< State of this agent.
	HANDLE				m_hWorkThread;		//!< Work thread handle.
	HANDLE				m_hStopSignal;		//!< Stop control event.
	LuaPlus::LuaState*	m_pLuaState;		//!< LuaState.
	unsigned int		m_tLastTick;		//!< time of last tick work.
	unsigned int		m_tHeartBeatTick;	//!< time of last tick work.

	ProcedureManager*	m_pProcedureManager;	//!< Procedure Manager
	Log*				m_pLog;					//!< Log interface.
	Info*				m_pInfo;				//!< Info interface.
	NetManager*			m_pNetManager;			//!< Network manager.
	Variable*			m_pVariable;			//!< Variable manager.
	Util*				m_pUtil;				//!< Util function union.
	ObjectManager*		m_pObjectManager;		//!< LogicObject Manager.
	WorldManager*		m_pWorldManager;		//!< World data Manager.
	DataPool*			m_pDataPool;			//!< Logic Data Pool

private:
	/*************************************************************************
		Friends to allow access to constructors and destructors
	*************************************************************************/
	friend Agent*	AgentManager::createAgent(const AgentCreateParam& createParam);
	friend void		AgentManager::destryoAgent(int id);

	/*************************************************************************
		Construction and Destruction (private, only AgentManager can 
		create and destroy Agent objects).
	*************************************************************************/
	Agent(int id, int idLegend, const AgentCreateParam& param);
	virtual ~Agent();

private:
	/*!
	\brief
		Agent working thread enter point.
	*/
	static unsigned int __stdcall _agentWorkingThread(void* pParam);

	/*!
	\brief
		Agent working thread function
	*/
	unsigned int WorkingThread(void);

	/*!
	\brief
		Set the Agent state.
	*/
	void setState(State state);

private:
    // unimplemented constructors / assignment
    Agent(const Agent& obj);
    Agent& operator=(const Agent& obj);
};



}
