#include "StdAfx.h"
#include "SMXMLHandler.h"
#include "SMXMLAttributes.h"

#include "SMAgentTemplate.h"

namespace SMITH
{

// xml file elements and attributes
const std::string XMLHandler::AgentElement("Agent");
const std::string XMLHandler::ScriptsElement("Scripts");
const std::string XMLHandler::ScriptElement("Script");
const std::string XMLHandler::VariablesElement("Variables");
const std::string XMLHandler::VariableElement("Variable");
const std::string XMLHandler::NameAttribute("name");
const std::string XMLHandler::DescAttribute("desc");
const std::string XMLHandler::DefaultAttribute("default");

void XMLHandler::elementStart(const std::string& element, const XMLAttributes& attributes)
{
	if(_stricmp(element.c_str(), AgentElement.c_str()) == 0)			// <Agent ...
	{
		m_stackWork.push(WS_Agent);
		m_bAgentSpace = true;
		m_pTemplate->m_strDesc = attributes.getValueAsString(DescAttribute).c_str();
	}
	else if(_stricmp(element.c_str(), ScriptsElement.c_str()) == 0 && m_bAgentSpace)	// <Scripts ...
	{
		m_stackWork.push(WS_Scripts);
	}
	else if(_stricmp(element.c_str(), ScriptElement.c_str()) == 0 && m_bAgentSpace)	// <Script ...
	{
		m_pTemplate->m_vScriptFiles.push_back(attributes.getValueAsString(NameAttribute));
		m_stackWork.push(WS_Script);
	}
	else if(_stricmp(element.c_str(), VariablesElement.c_str()) == 0 && m_bAgentSpace)	// <Variables ...
	{
		m_stackWork.push(WS_Variables);
	}
	else if(_stricmp(element.c_str(), VariableElement.c_str()) == 0 && m_bAgentSpace)	// <Variable ...
	{
		m_pTemplate->m_vVariables.push_back(
			std::make_pair( attributes.getValueAsString(NameAttribute), 
							attributes.getValueAsString(DefaultAttribute)));
		m_stackWork.push(WS_Variable);
	}
	else
	{
		m_stackWork.push(WS_Unknown);
	}
}

void XMLHandler::elementEnd(const std::string& element)
{
	if(_stricmp(element.c_str(), AgentElement.c_str()) == 0)			// <Agent ...
	{
		m_bAgentSpace = false;
		m_stackWork.pop();
	}
	else if(_stricmp(element.c_str(), ScriptsElement.c_str()) == 0)	// <Scripts ...
	{
		m_stackWork.pop();
	}
	else if(_stricmp(element.c_str(), ScriptElement.c_str()) == 0)	// <Script ...
	{
		m_stackWork.pop();
	}
	else if(_stricmp(element.c_str(), VariablesElement.c_str()) == 0)	// <Variables ...
	{
		m_stackWork.pop();
	}
	else if(_stricmp(element.c_str(), VariableElement.c_str()) == 0)	// <Variable ...
	{
		m_stackWork.pop();
	}
	else
	{
		m_stackWork.pop();
	}
}

}