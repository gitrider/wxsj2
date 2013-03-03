/****************************************\
*										*
* 			AgentTemplate				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

namespace SMITH
{

class AgentTemplate
{

public:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef std::vector< std::string > ScriptFiles;
	typedef std::vector< std::pair< std::string, std::string > > VariableDefine;

	std::string		m_strWorkFold;	//!< directoy name stores data files
	std::string		m_strName;		//!< name of agent template
	std::string		m_strDesc;		//!< desc of template(GBC)
	ScriptFiles		m_vScriptFiles;	//!< scripts to run
	VariableDefine	m_vVariables;	//!< variables defined
};



}
