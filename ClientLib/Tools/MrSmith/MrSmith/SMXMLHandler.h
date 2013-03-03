/*

Agent Template XML Handle

by jinchao.
ref from CEGUI
*/

#pragma once

#include <stack>

namespace SMITH
{
class AgentTemplate;
class XMLAttributes;
class XMLHandler
{
public:
	/*!
	\brief
	XMLParser base class constructor.
	*/
	XMLHandler(AgentTemplate* pTemplate) : m_pTemplate(pTemplate), m_bAgentSpace(false)
	{ }

	/*!
	\brief
	XMLHandler base class destructor.
	*/
	virtual ~XMLHandler(void)
	{ }

	/*!
	\brief
	Method called to notify the handler at the start of each XML element encountered.

	\param element
	std::string object holding the name of the element that is starting.

	\param attributes
	An XMLAttributes object holding the collection of attributes specified for the element.

	\return
	Nothing.
	*/
	virtual void elementStart(const std::string& element, const XMLAttributes& attributes);

	/*!
	\brief
	Method called to notify the handler at the end of each XML element encountered.

	\param element
	std::string object holding the name of the element that is ending.

	\return
	Nothing.
	*/
	virtual void elementEnd(const std::string& element);

protected:
	AgentTemplate* m_pTemplate;		//!< Template object that we are helping to build

private:
	/*************************************************************************
		Implementation Constants
	*************************************************************************/

	// XML related strings
	static const std::string AgentElement;			//!< Root RSS element.
	static const std::string ScriptsElement;		//!< Element specifying an Scripts
	static const std::string ScriptElement;			//!< Element specifying an Script
	static const std::string VariablesElement;		//!< Element specifying an Variables
	static const std::string VariableElement;		//!< Element specifying an Variable
	static const std::string NameAttribute;			//!< Attribute name that stores the name of a element
	static const std::string DescAttribute;			//!< Attribute name that stores the desc of a element
	static const std::string DefaultAttribute;		//!< Attribute name that stores the default value of a element

	enum WorkStation
	{
		WS_Unknown = 0,

		WS_Agent,
		WS_Scripts,
		WS_Script,
		WS_Variables,
		WS_Variable,
	};
	typedef std::stack< WorkStation > WorkStack;

	WorkStack m_stackWork;	//!< Work Stack
	bool m_bAgentSpace;		//!< In agent tempplate space
//	RSSParser::Item m_tempItem;		//!< Temp RSS Item
};

}