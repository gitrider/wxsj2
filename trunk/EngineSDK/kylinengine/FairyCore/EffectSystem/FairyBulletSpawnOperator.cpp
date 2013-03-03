#include "FairyBulletSpawnOperator.h"
#include "FairyBulletSystemManager.h"
#include "FairyBulletSystem.h"
#include "FairyBulletFlowSystem.h"
#include "FairyEffectBasic.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	BulletSpawnOperator::CmdBulletSystemTemplateName	BulletSpawnOperator::ms_bulletTemplateCmd;
	BulletSpawnOperator::CmdIsDeleteSource				BulletSpawnOperator::ms_isDeleteSourceCmd;


	BulletSpawnOperator::BulletSpawnOperator(BulletEventSystem* eventSystem) : BulletOperator()
	{
		m_type = "spawn";
		m_parent = eventSystem;
		
		m_spawnedTimes = 0;
		m_isOperated = false;

		if (createParamDictionary("BulletSpawnOperator"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("bulletsystemname", 
				"The mode of bullet speed operator.",
				Ogre::PT_STRING),&ms_bulletTemplateCmd);	  

			dict->addParameter(Ogre::ParameterDef("isdeletesource", 
				"The direction of bullet speed operator.",
				Ogre::PT_BOOL),&ms_isDeleteSourceCmd);	

		}
	}
	BulletSpawnOperator::~BulletSpawnOperator()
	{
	}
	void BulletSpawnOperator::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletSpawnOperator* newSpawnOperator =  dynamic_cast<BulletSpawnOperator*>(&newOperator);
		if(newSpawnOperator)
		{
			newSpawnOperator->m_bulletSystemName = m_bulletSystemName;
			newSpawnOperator->m_isDeleteSource = m_isDeleteSource;
		}
    }
	String BulletSpawnOperator::CmdBulletSystemTemplateName::doGet(const void* source) const
	{
		const BulletSpawnOperator* object = static_cast<const BulletSpawnOperator*>(source);
		return object->getBulletSystemName();
	}
	void BulletSpawnOperator::CmdBulletSystemTemplateName::doSet(void* target, const String& val)
	{
		BulletSpawnOperator* object = static_cast<BulletSpawnOperator*>(target);
		object->setBulletSystemName(val);
	}
	String BulletSpawnOperator::CmdIsDeleteSource::doGet(const void* source) const
	{
		const BulletSpawnOperator* object = static_cast<const BulletSpawnOperator*>(source);
		return Ogre::StringConverter::toString(object->getIsDeleteSource());
	}

	void BulletSpawnOperator::CmdIsDeleteSource::doSet(void* target, const String& val)
	{
		BulletSpawnOperator* object = static_cast<BulletSpawnOperator*>(target);
		object->setIsDeleteSource(Ogre::StringConverter::parseBool(val));
	}

	void BulletSpawnOperator::operateBulletEventSystem(Real timeElapsed)
	{
		if(!m_parent->getNewTransferredBulletSystem().empty())
		{
			
			BulletEventSystem::BulletSystemVecotor::iterator it
					= m_parent->getNewTransferredBulletSystem().begin();
			while(it != m_parent->getNewTransferredBulletSystem().end())
			{
				BulletSystem* pBulletSystem = *it;

				BulletSystem* pBulletSystemTemplate = BulletSystemManager::getSingleton().createBulletSystem(m_bulletSystemName);
				if(pBulletSystemTemplate)
				{
					BulletSystem* pNewBulletSystem = m_parent->getParent()->addBulletSystem(m_bulletSystemName);
					m_parent->addActiveBulletSystem(pNewBulletSystem);
					TransformInfo info= pBulletSystem->getTransformInfo();
					pNewBulletSystem->setTransformInfo(info);
				}
				if(m_isDeleteSource)
				{
					m_parent->removeActiveBulletSystem(pBulletSystem);
					m_parent->getParent()->removeBulletSystem(pBulletSystem);
					
				}
				it ++;
				
			}
			m_parent->getNewTransferredBulletSystem().clear();
			
		}

	}
}