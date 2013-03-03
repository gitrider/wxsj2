/****************************************\
*										*
*		   VariableManager				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once

#include "SMExportEnable.h"

namespace SMITH
{
//Forward reference declarations
class Agent;

class Variable : public LuaExport< Variable >
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/
	/*!
	\brief
		Init manager
	\return
	*/
	void init(void);
	/*!
	\brief
		Release manager
	\return
	*/
	void release(void);
	/*!
	\brief
		Tickwork
	\return
	*/
	void tick(void);
	/*!
	\brief
		Get variable
	\return
	*/
	bool getVariable(const char* szName, std::string& strValue);

	/*!
	\brief
		Set variable
	\return
	*/
	void setVariable(const char* szName, const char* szValue);

	/*************************************************************************
		Public Interface(Set direct)
	*************************************************************************/
	/*!
	\brief
		Set variable as int
	\return
	*/
	void setAs_Int(const char* szName, int nValue);

	/*!
	\brief
		Set variable as float
	\return
	*/
	void setAs_Float(const char* szName, float fValue);
	/*!
	\brief
		Set variable as vector2 (float,float)
	\return
	*/
	void setAs_Vector2(const char* szName, float fX, float fY);

	/*************************************************************************
		Public Interface(Get direct)
	*************************************************************************/
	/*!
	\brief
		Get variable as String(def="")
	\return
	*/
	const std::string& getAs_String(const char* szName, bool* bHave = 0);

	/*!
	\brief
		Get variable as int(def=0)
	\return
	*/
	int getAs_Int(const char* szName, bool* bHave = 0);

	/*!
	\brief
		Get variable as float(def=0.0f)
	\return
	*/
	float getAs_Float(const char* szName, bool* bHave = 0);

	/*!
	\brief
		Get variable as vector2(def=0.0f,0.0f)
	\return
	*/
	std::pair< float, float > getAs_Vector2(const char* szName, bool* bHave = 0);

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/

	/*!
	\brief
		Set a variable

		param0[string] - the name of variable
		param1[string] - the value of variable

	\return
		None.
	*/
	int Lua_SetVariable(LuaPlus::LuaState* state);	

	/*!
	\brief
		Get a variable

		param0[string] - the name of variable

	\return
		return0[string] - Value of variable(string)
		return1[bool]	- is variable exist.
	*/
	int Lua_GetVariable(LuaPlus::LuaState* state);	

protected:
	/*************************************************************************
		Implementation Interface
	*************************************************************************/

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef std::map< std::string, std::string >	VARIABLE_MAP;

	Agent*					m_pAgent;			//!< Owner.
	VARIABLE_MAP			m_mapCurrent;		//!< Current Variables

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Variable(Agent* pAgent);
	~Variable();
};


}