#include "StdAfx.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "Resource.h"
#include "LegendWnd.h"
#include "MainFrm.h"
#include "SMXMLHandler.h"
#include "SMXMLParser.h"

namespace SMITH
{

/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
AgentManager* AgentManager::s_pMe = 0;

/*************************************************************************
	Constructor
*************************************************************************/
AgentManager::AgentManager()
{
	s_pMe = this;
}

/*************************************************************************
	Destructor
*************************************************************************/
AgentManager::~AgentManager()
{
	s_pMe = 0;
}

/*************************************************************************
	Implementation method to generate a unique id to use for a Agent.
*************************************************************************/
int AgentManager::generateUniqueId(void)
{
	static int s_uid_counter = 0;
    int ret = ++s_uid_counter;

	if(ret < 0) ret = 0;
	return ret;
}

/*************************************************************************
	Create a new Agent.
*************************************************************************/
Agent* AgentManager::createAgent(const AgentCreateParam& createParam)
{
	Agent* newAgent = new Agent(generateUniqueId(), CLegendWnd::GetMe()->AllocaLegend(), createParam);

	//resouce error!
	if(!newAgent || newAgent->getID()==-1) return 0;

	m_Agents.insert(std::make_pair(newAgent->getID(), newAgent));

	return newAgent;
}

/*************************************************************************
	Destroys the Agent with the specified id.
*************************************************************************/
void AgentManager::destryoAgent(int id)
{
	AgentRegistry::iterator	pos = m_Agents.find(id);

	if (pos != m_Agents.end())
	{
		delete pos->second;
		m_Agents.erase(pos);
	}
}

/*************************************************************************
	Destroys the given Agent object
*************************************************************************/
void AgentManager::destroyAgent(Agent* agent)
{
	if (agent != 0)
	{
		destryoAgent(agent->getID());
	}
}

/*************************************************************************
	Destroys the given Agent object
*************************************************************************/
void AgentManager::destroyAllAgents(void)
{
	AgentRegistry::iterator it;
	for(it=m_Agents.begin(); it!=m_Agents.end(); it++)
	{
		destroyAgent(it->second);
	}
	m_Agents.clear();
}


/*************************************************************************
	Destroys the given Agent object
*************************************************************************/
Agent* AgentManager::findAgent(int id)
{
	AgentRegistry::iterator	pos = m_Agents.find(id);

	if (pos != m_Agents.end())
	{
		return pos->second;
	}

	return 0;
}

/*************************************************************************
	Return a AgentManager::AgentIterator object to iterate over 
	the available agent objects.
*************************************************************************/
AgentManager::AgentIterator AgentManager::getAgentIterator(void)
{
	return AgentIterator(m_Agents.begin(), m_Agents.end());
}

/*************************************************************************
	Return a AgentManager::AgentIterator object to iterate over 
	the available agent template.
*************************************************************************/
AgentManager::AgentTemplateIterator AgentManager::getAgentTemplateIterator(void)
{
	return AgentTemplateIterator(m_AgentTemplate.begin(), m_AgentTemplate.end());
}

/*************************************************************************
	Load agent template from data fold
*************************************************************************/
int AgentManager::loadAgentTemplate(const char* szScriptsFold)
{
	//invalid fold name
	if(!szScriptsFold || szScriptsFold[0]==0) return 0;

	WIN32_FIND_DATA theFindData;

	// TO Search...
	char szFileToSearch[MAX_PATH] = {0};
	strncpy(szFileToSearch, szScriptsFold, MAX_PATH);
	strcat(szFileToSearch, "*.*");

	// find first file
	HANDLE hFind = FindFirstFile(szFileToSearch, &theFindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		//empty !
		FindClose(hFind);
		return true;
	}

	BOOL bFind = FALSE;
	do
	{
		if(theFindData.cFileName[0] != '.')
		{
			//File in Disk
			CHAR szPathFileName[MAX_PATH];
			strncpy(szPathFileName, szScriptsFold, MAX_PATH);
			PathAppend(szPathFileName, theFindData.cFileName);

			//must a fold
			if(theFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				addAgentTemplate(szPathFileName);
			}
		}

		// Find Next file.
		bFind = FindNextFile(hFind, &theFindData);
	}while(bFind);

	FindClose(hFind);

	return (int)m_AgentTemplate.size();
}

/*************************************************************************
	Add a agent template.
*************************************************************************/
bool AgentManager::addAgentTemplate(const char* szFold)
{
	//parser xml file
	char szTemplateFile[MAX_PATH] = {0};
	strncpy(szTemplateFile, szFold, MAX_PATH);
	PathAppend(szTemplateFile, "Template.xml");

	if(!PathFileExists(szTemplateFile)) return false;

	AgentTemplate newTemplate;
	newTemplate.m_strWorkFold = szFold;
	newTemplate.m_strName = ::PathFindFileName(szFold);

	try
	{
		XMLHandler handler(&newTemplate);

		XMLParser xml_parser;
		xml_parser.parseXMLFile(handler, szTemplateFile);
	}
	catch(...)
	{
		return false;
	}

	m_AgentTemplate.insert(std::make_pair(newTemplate.m_strName, newTemplate));

	return true;
}

/*************************************************************************
	Get the agent template by name.
*************************************************************************/
const AgentTemplate* AgentManager::getAgentTemplate(const char* szTemplateName) const
{
	AgentTemplatesRegistry::const_iterator it = 
		m_AgentTemplate.find(szTemplateName);

	if(it==m_AgentTemplate.end()) return 0;

	return &(it->second);
}

/*************************************************************************
	Create all agent create param to file.
*************************************************************************/
bool AgentManager::saveAgentsCreateParam(const char* szFileName) const
{
	assert(szFileName);

	//create file
	FILE* fp = fopen(szFileName, "w");
	if(!fp) return false;

	fprintf(fp, "[Global]\nAgentCounts=%d\n", (int)m_Agents.size());

	int nIndex=0;
	AgentRegistry::const_iterator it;
	for(it=m_Agents.begin(); it!=m_Agents.end(); it++)
	{
		const Agent* pAgent = it->second;
		const AgentCreateParam& param = pAgent->getCreateParam();

		fprintf(fp, "\n[Agent%d]\n", nIndex);
		fprintf(fp, "Template=%s\n", param.strTemplate.c_str());
		fprintf(fp, "ParamNum=%d\n", (int)param.vParams.size());

		int nParamIndex=0;
		AgentCreateParam::Params::const_iterator itParam;
		for(itParam=param.vParams.begin(); itParam!=param.vParams.end(); itParam++)
		{
			fprintf(fp, "ParamName%d\t=%s\n", nParamIndex, itParam->first.c_str());
			fprintf(fp, "ParamValue%d\t=%s\n", nParamIndex, itParam->second.c_str());

			nParamIndex++;
		}
		nIndex++;
	}

	fclose(fp); fp=0;
	return true;
}

}