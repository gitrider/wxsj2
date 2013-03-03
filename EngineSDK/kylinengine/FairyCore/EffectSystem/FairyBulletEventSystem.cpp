#include "FairyBulletEventSystem.h"
#include "FairyBulletFlowSystem.h"
#include "FairyBulletFlowSystemManager.h"
#include "FairyBulletOperator.h"
#include "FairyBulletSystem.h"

namespace Fairy
{
	BulletEventSystem::BulletEventSystem(const String& templateName,BulletFlowSystem* parent) :
		m_templateName(templateName),m_parent(parent)
	{
	}
	BulletEventSystem::~BulletEventSystem()
	{
		removeAllBulletOperators();
		removeAllActiveBulletSystem();
	}
	BulletEventSystem& BulletEventSystem::operator = (const BulletEventSystem& rhs)
	{
		this->m_templateName = rhs.m_templateName;
		int i =0;
		removeAllBulletOperators();

		for(i = 0;i<rhs.getNumBulletOperator();i++)
		{
			BulletOperator* rhsOperator = rhs.getBulletOperator(i);
			BulletOperator* newOperator = 
				BulletFlowSystemManager::getSingleton().createBulletOperator(rhsOperator->getType(),this);
			assert(rhsOperator && newOperator);
			rhsOperator->copyParameters(*newOperator);

			m_operators.push_back(newOperator);
		}
		return *this;
	}
	BulletOperator*	BulletEventSystem::addBulletOperator(const String& typeName,
			BulletOperatorAddMode addMode,
			unsigned int insertIndex)
	{

		BulletOperator* pOperator = 
			BulletFlowSystemManager::getSingleton().createBulletOperator(typeName,this);
		if(pOperator)
		{
			if(addMode == BulletEventSystem::BOAM_APPEND)
				m_operators.push_back(pOperator);
			else
			{
				if(insertIndex < m_operators.size())
					m_operators[insertIndex] = pOperator;
			}
		}
		return pOperator;
	}
	void BulletEventSystem::removeBulletOperator(unsigned int index)
	{
		if(index >= m_operators.size())
			return;
		std::vector<BulletOperator*>::iterator it = m_operators.begin() + index;
		if(*it)
		{
			BulletFlowSystemManager::getSingleton().destroyBulletOperator(*it);
			m_operators.erase(it);
		}
	}

	void BulletEventSystem::removeBulletOperator(BulletOperator* pOperator)
	{
		//BulletFlowSystemManager::getSingleton().destroyBulletOperator(pOperator);
		//m_operators.erase(pOperator);
	}
	void BulletEventSystem::removeAllBulletOperators()
	{
		std::vector<BulletOperator*>::iterator it = m_operators.begin();
		while(it != m_operators.end())
		{
			if(*it)
			{
				BulletFlowSystemManager::getSingleton().destroyBulletOperator(*it);
			}
			it++;
		}
		m_activeBulletSystem.clear();
	}
	int	BulletEventSystem::getNumBulletOperator() const
	{
		return m_operators.size();
	}
	BulletOperator*	BulletEventSystem::getBulletOperator(int index) const
	{
		if(index < m_operators.size())
			return m_operators[index];
		return NULL;
	}
	void BulletEventSystem::addNewTransferredBulletSystem(BulletSystem* bulletSystem)
	{
		m_newTransferredBulletSystem.push_back(bulletSystem);
	}
	void BulletEventSystem::addActiveBulletSystem(BulletSystem* bulletSystem)
	{
		assert(bulletSystem);
		m_activeBulletSystem.push_back(bulletSystem);
	}

	void BulletEventSystem::removeActiveBulletSystem(BulletSystem* bulletSystem)
	{
		std::vector<BulletSystem*>::iterator it = m_activeBulletSystem.begin();
		while(it != m_activeBulletSystem.end())
		{
			BulletSystem* pBulletSystem = *it;
			if(bulletSystem == pBulletSystem)
			{
				m_activeBulletSystem.erase(it);
				break;
			}
			it ++;
		}
	}
	void BulletEventSystem::clearActiveBulletSystem(BulletSystem* bulletSystem)
	{
		std::vector<BulletSystem*>::iterator it = m_activeBulletSystem.begin();
		while(it != m_activeBulletSystem.end())
		{
			BulletSystem* pBulletSystem = *it;
			if(bulletSystem == pBulletSystem)
			{
				m_activeBulletSystem.erase(it);
				break;
			}
			it ++;
		}
	}
	void BulletEventSystem::removeAllActiveBulletSystem()
	{
		/*std::vector<BulletSystem*>::const_iterator it = m_activeBulletSystem.begin();
		while(it != m_activeBulletSystem.end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem)
			{
				m_parent->removeBulletSystem(pBulletSystem);
			}
			it ++;
		}*/
		m_activeBulletSystem.clear();
	}
	void BulletEventSystem::update(Real time)
	{
		/*std::vector<BulletSystem*>::iterator newTrans_it = 
			m_newTransferredBulletSystem.begin();
		while(newTrans_it != m_newTransferredBulletSystem.end())
		{
			m_activeBulletSystem.push_back(*newTrans_it);
			newTrans_it ++;
		}
		m_newTransferredBulletSystem.clear();*/

		std::vector<BulletOperator*>::iterator operator_it = m_operators.begin();
		while(operator_it != m_operators.end())
		{
			if(*operator_it)
			{
				(*operator_it)->operateBulletEventSystem(time);
			}
			
			operator_it ++;
		}

		/*std::vector<BulletSystem*>::const_iterator system_it = m_activeBulletSystem.begin();
		while(system_it != m_activeBulletSystem.end())
		{
			if(*system_it)
				(*system_it)->update(time);

			system_it ++;
		}*/
		
	}
	void BulletEventSystem::addTransition(const String& eventName, BulletSystem* bulletSystem)
	{
		std::map<String,BulletSystemCollection*>::iterator it = m_transitionCache.find(eventName);
		if(it != m_transitionCache.end())
		{
			BulletSystemCollection* collection = (*it).second;
			std::vector<BulletSystem*>& vBulletSystems = collection->m_bulletSystemCollectition;
			vBulletSystems.push_back(bulletSystem);
		}
		else
		{
			BulletSystemCollection* pCollection = new BulletSystemCollection;
			std::vector<BulletSystem*>& vBulletSystems = pCollection->m_bulletSystemCollectition;
			vBulletSystems.push_back(bulletSystem);

			m_transitionCache.insert(TransitionMap::value_type(eventName,pCollection));
		}
	}
	BulletFlowSystem* BulletEventSystem::getParent()
	{
		return m_parent;
	}

	const Fairy::String& BulletEventSystem::getTemplateName() const
	{
		return m_templateName;
	}

	std::vector<BulletSystem*>&	BulletEventSystem::getActiveBulletSystem()
	{
		return m_activeBulletSystem;
	}
	std::vector<BulletSystem*>&	BulletEventSystem::getNewTransferredBulletSystem()
	{
		return m_newTransferredBulletSystem;
	}
	std::map<String,BulletSystemCollection*>& BulletEventSystem::getTransitionCache()
	{
		return m_transitionCache;
	}
}
