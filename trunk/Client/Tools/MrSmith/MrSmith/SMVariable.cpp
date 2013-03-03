#include "StdAfx.h"
#include "SMVariable.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "SMUtil.h"

namespace SMITH
{

/*************************************************************************
	Constructor
*************************************************************************/
Variable::Variable(Agent* pAgent)
	: LuaExport< Variable >("Variable", pAgent->getLuaState()),
	m_pAgent(pAgent)
{
	assert(pAgent);
}

/*************************************************************************
	Destructor
*************************************************************************/
Variable::~Variable()
{
}

/*************************************************************************
	Init Variable Manager
*************************************************************************/
void Variable::init(void)
{
	// register lua function
	RegisterFunction("SetVariable", &Variable::Lua_SetVariable);
	RegisterFunction("GetVariable", &Variable::Lua_GetVariable);
}

/*************************************************************************
	Release variable manager
*************************************************************************/
void Variable::release(void)
{
}

/*************************************************************************
	tickwork variable manager
*************************************************************************/
void Variable::tick(void)
{


}

/*************************************************************************
	Get variable
*************************************************************************/
bool Variable::getVariable(const char* szName, std::string& strValue)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end()) return false;

	strValue = itFind->second;
	return true;
}


/*************************************************************************
	Set variable
*************************************************************************/
void Variable::setVariable(const char* szName, const char* szValue)
{
	m_mapCurrent[szName] = szValue;
}

/*************************************************************************
	Set variable as int
*************************************************************************/
void Variable::setAs_Int(const char* szName, int nValue)
{
	char szTemp[32];
	_snprintf(szTemp, 32, "%d", nValue);
	setVariable(szName, szTemp);
}

/*************************************************************************
	Set variable as float
*************************************************************************/
void Variable::setAs_Float(const char* szName, float fValue)
{
	char szTemp[32];
	_snprintf(szTemp, 32, "%f", fValue);
	setVariable(szName, szTemp);
}

/*************************************************************************
		Set variable as vector2 (float,float)
*************************************************************************/
void Variable::setAs_Vector2(const char* szName, float fX, float fY)
{
	char szTemp[64];
	_snprintf(szTemp, 32, "%f,%f", fX, fY);
	setVariable(szName, szTemp);
}


/*************************************************************************
		Get variable as string(def="")
*************************************************************************/
const std::string& Variable::getAs_String(const char* szName, bool* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		static std::string s_strBlank = "";
		if(bHave) *bHave = false;
		return s_strBlank;
	}

	if(bHave) *bHave = true;
	return itFind->second;
}

/*************************************************************************
		Get variable as int(def=0)
*************************************************************************/
int	Variable::getAs_Int(const char* szName, bool* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = false;
		return 0;
	}

	if(bHave) *bHave = true;
	return atoi(itFind->second.c_str());
}

/*************************************************************************
		Get variable as float(def=0.0f)
*************************************************************************/
float Variable::getAs_Float(const char*  szName, bool* bHave)
{
	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = false;
		return 0.0f;
	}

	if(bHave) *bHave = true;
	return (float)atof(itFind->second.c_str());
}

/*************************************************************************
		Get variable as vector2(def=0.0f,0.0f)
*************************************************************************/
std::pair< float, float > Variable::getAs_Vector2(const char* szName, bool* bHave)
{
	static std::pair< float, float > fvNULL(0.0f, 0.0f);

	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(szName);
	if(itFind == m_mapCurrent.end())
	{
		if(bHave) *bHave = false;
		return fvNULL;
	}

	if(bHave) *bHave = true;

	char szTemp[64];
	strncpy(szTemp, itFind->second.c_str(), 64);

	char* pDot = strchr(szTemp, ',');
	if(!pDot) return fvNULL;
	*pDot = '\0';

	return std::make_pair((float)atof(szTemp), (float)atof(pDot+1));
}


/*************************************************************************
	Set variable
*************************************************************************/
int Variable::Lua_SetVariable(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		Util::throwException("Variable::SetVariable[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		Util::throwException("Variable::SetVariable[3] param parameter error");
	}

	setVariable(args[2].GetString(), args[3].GetString());
	return 0;
}

/*************************************************************************
	Set variable
*************************************************************************/
int Variable::Lua_GetVariable(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		Util::throwException("Variable::SetVariable[2] param parameter error");
	}

	VARIABLE_MAP::iterator itFind = m_mapCurrent.find(args[2].GetString());
	if(itFind == m_mapCurrent.end()) 
	{
		state->PushString("");
		state->PushBoolean(false);
		return 2;
	}


	state->PushString(itFind->second.c_str());
	state->PushBoolean(true);
	return 2;
}

}
