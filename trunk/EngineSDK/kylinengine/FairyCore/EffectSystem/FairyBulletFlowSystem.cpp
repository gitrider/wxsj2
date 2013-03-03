
#include "FairyBulletFlowSystem.h"
#include "FairyBulletEventSystem.h"
#include "FairyBulletSystemManager.h"
#include "FairyBulletSystem.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyBulletObject.h"
#include <OgreStringConverter.h>
 namespace Fairy
{

	BulletFlowSystem::CmdMaxTime	BulletFlowSystem::ms_maxTimeCmd;

	BulletFlowSystem::BulletFlowSystem(System* system) : m_system(system),m_sceneNode(NULL),m_userData(NULL),
		m_age(0.0f),m_maxTime(0.0f),m_isCreated(false),m_targetModel(NULL),m_casterModel(NULL),
		m_isActiveEventSystemListInitialised(false),m_birthBulletTimesPerCluster(0)
	{
		initParamDictionary();
	}
	BulletFlowSystem::~BulletFlowSystem()
	{
		removeAllBulletEventSystem();
		removeAllBulletSystem();
	}
	bool BulletFlowSystem::initParamDictionary(void)
	{
		if (createParamDictionary("BulletFlow"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();			

			dict->addParameter(Ogre::ParameterDef("maxtime", 
				"bullet flow max time",
				Ogre::PT_REAL),&ms_maxTimeCmd);	  

			return true;
		}

		return false;
	}
	String BulletFlowSystem::CmdMaxTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const BulletFlowSystem *>(target)->getMaxTime() );
	}
	void BulletFlowSystem::CmdMaxTime::doSet(void* target, const String& val)
	{
		static_cast<BulletFlowSystem *>(target)->setMaxTime(Ogre::StringConverter::parseReal(val));
	}		

	BulletFlowSystem& BulletFlowSystem::operator = (const BulletFlowSystem& rhs)
	{
		this->m_maxTime = rhs.m_maxTime;
		this->m_templateName = rhs.m_templateName;
		this->m_birthBulletTimesPerCluster = rhs.m_birthBulletTimesPerCluster;
		removeAllBulletEventSystem();
		std::map<String,BulletEventSystem*>::const_iterator it = rhs.getBulletEventSystemsMap().begin();
		while(it != rhs.getBulletEventSystemsMap().end())
		{
			BulletEventSystem *rhsBES = it->second;
			BulletEventSystem *newBES = new BulletEventSystem(rhsBES->getTemplateName(),this);
			assert ( rhsBES && rhsBES );
			*newBES = *rhsBES;
			m_bulletEventSystemMap.insert(BulletEventMap::value_type(rhsBES->getTemplateName(),newBES));
			it++;
		}
		/*removeAllBulletSystem();
		for(int i = 0; i < rhs.getNumBulletSystem(); ++i)
		{
			BulletSystem *rhsBS = rhs.getBulletSystem(i);
			BulletSystem *newBS = this->addBulletSystem(rhsBS->getTemplateName());
			assert ( rhsBS && rhsBS );

		}*/

		return *this;
	}
	void BulletFlowSystem::update(Real time)
	{

		BulletEventMap::iterator event_it = m_bulletEventSystemMap.begin();
		while( event_it != m_bulletEventSystemMap.end())
		{
			if(event_it->second)
				event_it->second->update(time);
			event_it++;
		}
		BulletSystemVector::iterator it = m_bulletSystems.begin();
		while( it != m_bulletSystems.end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem)
			{
				pBulletSystem->update(time);
			}
			it++;
		}
		integrateBulletSystemTransform(time);
		m_age += time;
	}
	BulletEventSystem*	BulletFlowSystem::addBulletEventSystem(const String& name)
	{
		BulletEventSystem* pEvent = new BulletEventSystem(name,this);
		m_bulletEventSystemMap.insert(std::pair<String,BulletEventSystem*>(name,pEvent));
		return pEvent;
	}
	int	BulletFlowSystem::getNumBulletEventSystem() const
	{
		return m_bulletEventSystemMap.size();
	}
	BulletEventSystem*	BulletFlowSystem::getBulletEventSystem(const String& name)
	{
		BulletEventMap::iterator it = m_bulletEventSystemMap.find(name);
		if(it != m_bulletEventSystemMap.end())
			return it->second;
		return NULL;
	}
	void BulletFlowSystem::removeBulletEventSystem(const String& eventSystemName)
	{
		BulletEventMap::iterator it = m_bulletEventSystemMap.find(eventSystemName);
		if( it != m_bulletEventSystemMap.end())
		if (it->second)
		{
			delete it->second;
			m_bulletEventSystemMap.erase(it);
	    }
	}
	void BulletFlowSystem::removeBulletEventSystem(BulletEventSystem* eventSystem)
	{
		BulletEventMap::iterator it = m_bulletEventSystemMap.begin();
		while( it != m_bulletEventSystemMap.end())
		{
			if (it->second == eventSystem)
			{
					delete it->second;
					m_bulletEventSystemMap.erase(it);
					return;
			}
			it++;
		}
		
	}
	const std::map<String,BulletEventSystem*>& BulletFlowSystem::getBulletEventSystemsMap() const
	{
		return m_bulletEventSystemMap;
	}
	void  BulletFlowSystem::removeAllBulletEventSystem()
	{
		BulletEventMap::iterator it = m_bulletEventSystemMap.begin();
		while( it != m_bulletEventSystemMap.end())
		{
			BulletEventSystem* pBulletEventSystem = it->second;
			if(pBulletEventSystem)
				delete pBulletEventSystem;

			it++;
		}
		m_bulletEventSystemMap.clear();
	}
	// 此函数调用BulletSystemManager::createBulletSystem函数,另外BulletSystem
	// 由单独的BulletSystemManager管理，而不在BulletFlowSystemManager里面管理[1/23/2008 JiangWei]
	BulletSystem* BulletFlowSystem::addBulletSystem(const String& name)
	{
		if(m_targetModel == NULL || m_casterModel == NULL)
			return NULL;
		BulletSystem* pBulletSystemTemplate = 
			BulletSystemManager::getSingleton().getBulletSystemTemplate(name);
		if(pBulletSystemTemplate)
		{
			BulletSystem* pNewBulletSystem = new BulletSystem(name,m_system);

			*pNewBulletSystem = *pBulletSystemTemplate;
			pNewBulletSystem->Initial(m_targetModel, m_casterModel,m_AttachPointName,m_TargetPointName,
				m_OffsetPos,m_OffsetRoation);
			m_bulletSystems.push_back(pNewBulletSystem);
			return pNewBulletSystem;
		}
		return NULL;
	}
	int BulletFlowSystem::getNumBulletSystem() const
	{
		return m_bulletSystems.size();
	}
	BulletSystem* BulletFlowSystem::getBulletSystem(int index) const
	{
		if(index < m_bulletSystems.size())
		{
			return m_bulletSystems[index];
		}
		return NULL;
	}
	void BulletFlowSystem::removeBulletSystem(unsigned int index)
	{
		if(index >= m_bulletSystems.size())
			return;
		BulletSystemVector::iterator it = m_bulletSystems.begin()  + index;
		if (*it)
		{
			delete *it;
			*it = NULL;
		}
		m_bulletSystems.erase(it);
	}
	void BulletFlowSystem::removeBulletSystem(BulletSystem* bulletSystem)
	{
		BulletSystemVector::iterator it = m_bulletSystems.begin();
		while( it != m_bulletSystems.end())
		{
			BulletSystem* pBulletSystem = *it;
			if (pBulletSystem == bulletSystem)
			{
				BulletSystemManager::getSingleton().destroyBulletSystem(*it);
				m_bulletSystems.erase(it);
				return;
			}
			it++;
		}
	}
	void  BulletFlowSystem::removeAllBulletSystem()
	{
		BulletSystemVector::iterator it = m_bulletSystems.begin();
		while( it != m_bulletSystems.end())
		{
			BulletSystem* pBulletSystem = *it;
			if (pBulletSystem)
			{
				delete pBulletSystem;
				pBulletSystem = NULL;
			}
			it++;
		}
		m_bulletSystems.clear();
	}

	Fairy::Real	BulletFlowSystem::getMaxTime() const
	{
		return m_maxTime;
	}
	void BulletFlowSystem::setMaxTime(Fairy::Real val)
	{
		m_maxTime = val;
	}
	Fairy::Real BulletFlowSystem::getAge() const
	{
		return m_age;
	}
	void BulletFlowSystem::setAge(Fairy::Real val)
	{
		m_age = val;
	}
	int BulletFlowSystem::getBirthBulletTimesPerCluster() const
	{
		return m_birthBulletTimesPerCluster;
	}
	void BulletFlowSystem::setBirthBulletTimesPerCluster(int val)
	{
		m_birthBulletTimesPerCluster = val;
	}
	void BulletFlowSystem::integrateBulletSystemTransform(Real time)
	{
		BulletEventMap::iterator it = m_bulletEventSystemMap.begin();
		while( it != m_bulletEventSystemMap.end())
		{
			BulletEventSystem* pBulletEventSystem = it->second;
			BulletEventSystem::TransitionMap::iterator trans_it = 
				pBulletEventSystem->getTransitionCache().begin();
			while(trans_it != pBulletEventSystem->getTransitionCache().end())
			{
				BulletSystemCollection* pBulletSystemCollection = trans_it->second;
				std::vector<BulletSystem*>::iterator system_it = 
					pBulletSystemCollection->m_bulletSystemCollectition.begin();
				while(system_it != pBulletSystemCollection->m_bulletSystemCollectition.end())
				{
					pBulletEventSystem->clearActiveBulletSystem(*system_it);
				
					BulletEventSystem* pTransToBulletEventSystem = getBulletEventSystem(trans_it->first);
					if(pTransToBulletEventSystem)
						pTransToBulletEventSystem->addNewTransferredBulletSystem(*system_it);

					system_it ++;
				}
				delete pBulletSystemCollection;
				pBulletSystemCollection = NULL;

				trans_it ++;
			}
			pBulletEventSystem->getTransitionCache().clear();

			it ++;

		}
	}
};