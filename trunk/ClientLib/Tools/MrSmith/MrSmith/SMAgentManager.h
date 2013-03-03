/****************************************\
*										*
* 			   AgentManager				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once
#include "SMIteratorBase.h"
#include "SMAgentTemplate.h"

namespace SMITH
{

//Forward reference declarations
class Agent;
struct AgentCreateParam;

class AgentManager
{
public:
	/*************************************************************************
		Public interface
	*************************************************************************/

	/*!
	\brief
		Create a new Agent.

	\param createParam
		Params to create

	\return
		Pointer to the newly created Agent object
	*/
	Agent* createAgent(const AgentCreateParam& createParam);

	/*!
	\brief
		Destroys the Agent with the specified id.

	\param id
		The identity of the agent to be destroyed.  If no such Object exists, nothing happens.

	\return
		Nothing.
	*/
	void destryoAgent(int id);

	/*!
	\brief
		Destroys the given Agent object

	\param agent
		Pointer to the Agent to be destroyed.  If no such Object exists, nothing happens.

	\return
		Nothing.
	*/
	void destroyAgent(Agent* agent);

	/*!
	\brief
		Destroys all Agent objects registered in the system

	\return
		Nothing
	*/
	void destroyAllAgents(void);

	/*!
	\brief
		Find the spec agent with the id.

	\return
		Pointer to the Agent with the id, return null if not exist.
	*/
	Agent* findAgent(int id);

	/*!
	\brief
		Get the agent buf num.

	\return
		The num of agent registry size.
	*/
	int getAgentNum(void) const { return (int)m_Agents.size(); }

	/*!
	\breif
		Load agent template from data fold

	\param szScriptsFold
		The fold that stores all agent script files.

	\return 
		The num of agent templates.
	*/
	int loadAgentTemplate(const char* szScriptsFold);

	/*!
	\breif
		Get the agent template by name.
	*/
	const AgentTemplate* getAgentTemplate(const char* szTemplateName) const;

	/*!
	\brief
		Create all agent create param to file.

	\param szFileName
		Filename to save
	*/
	bool saveAgentsCreateParam(const char* szFileName) const;

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef std::map< int, Agent* >	AgentRegistry;
	typedef std::map< std::string, AgentTemplate > AgentTemplatesRegistry;

	AgentTemplatesRegistry m_AgentTemplate;	//!< Templates of agents
	AgentRegistry m_Agents;					//!< All agent

public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	IteratorBrowser<AgentRegistry>	AgentIterator;
	typedef IteratorBrowser<AgentTemplatesRegistry>	AgentTemplateIterator;

	/*!
	\brief
		Return a AgentManager::AgentIterator object to iterate over the available agent objects.
	*/
	AgentIterator	getAgentIterator(void);

	/*!
	\breif
		Return a AgentManager::AgentIterator object to iterate over the available agent template.
	*/
	AgentTemplateIterator getAgentTemplateIterator(void);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	AgentManager();
	~AgentManager();

	/*!
	\brief
		Return pointer to singleton AgentManager object

	\return
		Pointer to singleton HelperSystem object
	*/
	static AgentManager* GetMe(void) { return s_pMe; }

private:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    /*!
    \brief
        Implementation method to generate a unique id to use for a Agent.
    */
    int generateUniqueId(void);

	/*!
	\brief
		Add a agent template.
	*/
	bool addAgentTemplate(const char* szFold);

private:
    // unimplemented constructors / assignment
    AgentManager(const AgentManager& obj);
    AgentManager& operator=(const AgentManager& obj);

	static AgentManager* s_pMe;
};


}