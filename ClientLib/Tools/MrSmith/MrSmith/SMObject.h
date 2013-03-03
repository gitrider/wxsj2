/****************************************\
*										*
*				Object					*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

namespace LuaPlus { class LuaState; }

namespace SMITH
{
//Forward reference declarations
class Agent;

/*************************************************************************
Public Enum
*************************************************************************/
enum ObjectType
{
	OT_NPC,				//!< Character(NPC)
	OT_PLAYEROTHER,		//!< Character(PlayerOther)
	OT_PLAYERMYSELF,	//!< Character(PlayerMySelf)
};


enum ObjectLogicMode
{
	LM_IDLE,		//!< Idle
	LM_MOVING,		//!< In moving
	LM_DEAD,		//!< Dead
};

class Object
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/
	/*!
	\brief
		Return the id of this object.
	*/
	int	getID(void) const { return m_nID; }

	/*!
	\brief
		Return the id of server object.
	*/
	int getServerID(void) const { return m_nServerID; }

	/*!
	\brief
		Return the type of this Object.
	*/
	ObjectType getType(void) const { return m_type; }

	/*!
	\brief
		Return the LogicMode of this Object.
	*/
	ObjectLogicMode getLogicMode(void) const { return m_logicMode; }

	/*!
	\brief
		Tick work
	*/
	void tick(float fElapsTime);

	/*!
	\brief
		Set Position
	*/
	void setPosition(float fX, float fZ);

	/*!
	\brief
		Goto Position
	*/
	void gotoPosition(float fX, float fZ);

	/*!
	\brief
		Set AI State
	*/
	void setAIState(const char* szAIState);

	/*!
	\brief
		Get AI State
	*/
	const std::string& getAIState(void) const { return m_strAIState; }

protected:
	/*************************************************************************
		Implementation Interface
	*************************************************************************/
	/*!
	\brief
		Set the current logic state
	*/
	void setLogicMode(ObjectLogicMode mode);

	/*!
	\brief
		tick***
	*/
	void tick_Idle(float fElapsTime);
	void tick_Moving(float fElapsTime);
	void tick_Dead(float fElapsTime);

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/

	/*!
	\brief
		Goto a position

		param0[int] - x axis
		param1[int] - z axis

	\return
		None.
	*/
	int Lua_Goto(LuaPlus::LuaState* state);	

	/*!
	\brief
		Get the player position

	\return
		return0[int] - x axis
		return1[int] - z axis
	*/
	int Lua_GetPosition(LuaPlus::LuaState* state);	

	/*!
	\brief
		Get the player logic mode

	\return
		return0[string] - logic mode
	*/
	int Lua_GetLogicMode(LuaPlus::LuaState* state);	

	/*!
	\brief
		Set the player AI state

		param0[string] - AI State name

	\return
	*/
	int Lua_SetAIState(LuaPlus::LuaState* state);	

	/*!
	\brief
		Get the player AI state

	\return
		return0[string] - AI State name
	*/
	int Lua_GetAIState(LuaPlus::LuaState* state);	

	int Lua_Chat(LuaPlus::LuaState* state);	

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;			//!< Owner.

	int						m_nID;				//!< the id of this object
	int						m_nServerID;		//!< server id
	ObjectType				m_type;				//!< the type of this object
	ObjectLogicMode			m_logicMode;		//!< Logic mode of this object
	float					m_fX, m_fZ;			//!< Current Position;
	float					m_fSpeed;			//!< Move speed.

	struct MovePath
	{
		int id;
		float fStartX, fStartZ;
		float fEndX, fEndZ;
		int state; //0-Not process 1-Processing
	};

	typedef std::list< MovePath > MovePathList;
	MovePathList			m_vMovePath;		//!< Move Path

	std::string				m_strAIState;		//!< Ai Mode(Userset)

private:
	/*************************************************************************
		Friends to allow access to constructors and destructors
	*************************************************************************/
	friend Object*	ObjectManager::createObject(ObjectType type, int serverId);
	friend void		ObjectManager::destroyObject(int id);

	/*************************************************************************
		Construction and Destruction (private, only ObjectManager can 
		create and destroy Agent objects).
	*************************************************************************/
	Object(int id, ObjectType type, Agent* pAgent, int serverId=-1);
	virtual ~Object();

private:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    /*!
    \brief
        Implementation method to generate a unique id to use for a PathNode.
    */
    int generateUniquePathId(void);

	int m_uid_counter;

private:
    // unimplemented constructors / assignment
    Object(const Agent& obj);
    Object& operator=(const Agent& obj);
};


};