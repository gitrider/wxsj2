#include "StdAfx.h"
#include "SMObjectManager.h"
#include "SMObject.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "SMNetManager.h"
#include "SMUtil.h"

#include "CGCharMove.h"
#include "CGChat.h"

#include <math.h>

namespace SMITH
{

/*************************************************************************
	Constructor
*************************************************************************/
Object::Object(int id, ObjectType type, Agent* pAgent, int serverId) : 
	m_nID(id),
	m_type(type),
	m_pAgent(pAgent),
	m_fX(0.0f),
	m_fZ(0.0f),
	m_logicMode(LM_IDLE),
	m_nServerID(serverId),
	m_fSpeed(2.8f)
{
	assert(pAgent);

}

/*************************************************************************
	Destructor
*************************************************************************/
Object::~Object()
{
}

/*************************************************************************
	Set Position
*************************************************************************/
void Object::setPosition(float fX, float fZ)
{
	m_fX = fX;
	m_fZ = fZ;
}

/*************************************************************************
	Goto Position
*************************************************************************/
void Object::gotoPosition(float fX, float fZ)
{
	m_vMovePath.clear();

	// create a path
	//
	// TODO : Path find
	//
	MovePath newPath;
	newPath.id = generateUniquePathId();
	newPath.fStartX = m_fX;
	newPath.fStartZ = m_fZ;
	newPath.fEndX = fX;
	newPath.fEndZ = fZ;
	newPath.state = 0;
	m_vMovePath.push_back(newPath);

	setLogicMode(LM_MOVING);
}

/*************************************************************************
	Set logic mode
*************************************************************************/
void Object::setLogicMode(ObjectLogicMode mode)
{
	m_logicMode = mode;
}

/*************************************************************************
	Implementation method to generate a unique id to use for a PathNode.
*************************************************************************/
int Object::generateUniquePathId(void)
{
	m_uid_counter += 100;

	if(m_uid_counter < 0) m_uid_counter = 0;
	return m_uid_counter;
}

/*************************************************************************
	[LUA]Goto a position
*************************************************************************/
int Object::Lua_Goto(LuaPlus::LuaState* state)
{
	if(OT_PLAYERMYSELF != getType()) 
	{
		return 0;
	}

	LuaStack args(state);

	if(!(args[2].IsNumber()))
	{
		Util::throwException("Object::Goto[2] param parameter error");
	}

	if(!(args[3].IsNumber()))
	{
		Util::throwException("Object::Goto[3] param parameter error");
	}

	gotoPosition((float)args[2].GetNumber(), (float)args[3].GetNumber());
	return 0;
}

/*************************************************************************
	[LUA]Get the player position
*************************************************************************/
int Object::Lua_GetPosition(LuaPlus::LuaState* state)
{
	state->PushNumber((lua_Number)m_fX);
	state->PushNumber((lua_Number)m_fZ);
	
	return 2;
}

/*************************************************************************
	[LUA]Get the player logic mode
*************************************************************************/
int Object::Lua_GetLogicMode(LuaPlus::LuaState* state)
{
	switch(this->getLogicMode())
	{
	case LM_IDLE:
		state->PushString("Idle");
		break;

	case LM_MOVING:
		state->PushString("Moving");
		break;

	case LM_DEAD:
		state->PushString("Dead");
		break;

	default:
		state->PushString("UNKNOWN");
		break;
	}
	
	return 1;
}

/*************************************************************************
	[LUA]Set the player AI state
*************************************************************************/
int Object::Lua_SetAIState(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		Util::throwException("Object::SetAIState[2] param parameter error");
	}

	setAIState(args[2].GetString());
	return 0;
}

/*************************************************************************
	[LUA]Get the player AI state
*************************************************************************/
int Object::Lua_GetAIState(LuaPlus::LuaState* state)
{
	state->PushString(this->getAIState().c_str());
	return 1;
}

/*************************************************************************
	Set User AI state
*************************************************************************/
void Object::setAIState(const char* szAIState)
{
	m_strAIState = szAIState;
}

/*************************************************************************
	tick work
*************************************************************************/
void Object::tick(float fElapsTime)
{
	switch(this->getLogicMode())
	{
	case LM_IDLE:
		tick_Idle(fElapsTime);
		break;

	case LM_MOVING:
		tick_Moving(fElapsTime);
		break;

	case LM_DEAD:
		tick_Dead(fElapsTime);
		break;

	default:
		break;
	}
}

void Object::tick_Idle(float fElapsTime)
{
}

void Object::tick_Moving(float fElapsTime)
{
	if(m_vMovePath.empty())
	{
		setLogicMode(LM_IDLE);
		return;
	}

	//current path
	MovePath& currentPath = *(m_vMovePath.begin());

	//send move msg
	if(currentPath.state == 0)
	{
		//processing
		currentPath.state = 1;
		
		CGCharMove msg;
		msg.setObjID(m_nServerID);
		msg.setHandleID(currentPath.id);
		msg.setWorldPos(WORLD_POS(currentPath.fStartX, currentPath.fStartZ));

		float x, z;
		MovePathList::iterator it;
		for(it=m_vMovePath.begin(); it!=m_vMovePath.end(); it++)
		{
			WORLD_POS posTarget;
			x = posTarget.m_fX = it->fEndX;
			z = posTarget.m_fZ = it->fEndZ;
			
			msg.addTargetPos(&posTarget);
		}
		m_pAgent->getNetManager()->sendPacket(&msg);
	}

	//Move distance
	float fMoveDist = m_fSpeed*fElapsTime/1000.0f;
	float fMoveDistSq = fMoveDist*fMoveDist;

	//distance of currentpos and target
	float fDistSq = (currentPath.fEndX-m_fX)*(currentPath.fEndX-m_fX) +
					(currentPath.fEndZ-m_fZ)*(currentPath.fEndZ-m_fZ);

	// need move 
	if(fDistSq > fMoveDistSq)
	{
		float fTarDist = ::sqrt(fDistSq) ;
		float fDistX = (fMoveDist*(currentPath.fEndX-m_fX))/fTarDist;
		float fDistZ = (fMoveDist*(currentPath.fEndZ-m_fZ))/fTarDist;
		
		float fvNextPosX = m_fX + fDistX;
		float fvNextPosZ = m_fZ + fDistZ;

		//set new position
		this->setPosition(fvNextPosX, fvNextPosZ);		
	}
	else
	{
		//to next
		this->setPosition(currentPath.fEndX, currentPath.fEndZ);

		m_vMovePath.erase(m_vMovePath.begin());
	}

}

void Object::tick_Dead(float fElapsTime)
{
}

int Object::Lua_Chat(LuaPlus::LuaState* state)
{
	if(OT_PLAYERMYSELF != getType()) return 0;
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		Util::throwException("Object::Lua_Chat[2] param parameter error");
	}

	std::string str = args[2].GetString();

	//char szContex[1024];
	//sprintf( szContex, "ÎÒÈ¥%f,%f¿©~~", x, z );
	CGChat chat_msg;
	chat_msg.SetChannelID( 0 );
	chat_msg.SetContexSize( BYTE( str.length() ) );
	chat_msg.SetContex( str.c_str() );
	chat_msg.SetTargetName( "" );
	chat_msg.SetTeamID( 0 );
	chat_msg.SetChatType( 0 );
	m_pAgent->getNetManager()->sendPacket( &chat_msg );

	return 0;
}

}