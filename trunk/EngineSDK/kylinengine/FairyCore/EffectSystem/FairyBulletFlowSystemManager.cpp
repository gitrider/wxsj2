#include "FairyBulletFlowSystem.h"
#include "FairyBulletFlowSystemManager.h"
#include "FairyBulletSystemManager.h"
#include "FairyBulletSystem.h"
#include "FairyBulletEventSystem.h"
#include "FairyBulletOperatorFactory.h"
#include "FairyBulletAgeTesterFactory.h"
#include "FairyBulletBirthOperatorFactory.h"
#include "FairyBulletCameraShakeOperatorFactory.h"
#include "FairyBulletDeleterOperatorFactory.h"
#include "FairyBulletFindTargetTesterFactory.h"
#include "FairyBulletForceOperatorFactory.h"
#include "FairyBulletHeightFieldCollisionTesterFactory.h"
#include "FairyBulletScaleTesterFactory.h"
#include "FairyBulletScriptTesterFactory.h"
#include "FairyBulletSpawnOperatorFactory.h"
#include "FairyBulletSpeedOperatorFactory.h"
#include "FairyBulletSwayOperatorFactory.h"
#include "FairyBulletHelixOperatorFactory.h"
#include "FairyBulletOperator.h"
#include <OgreResourceGroupManager.h>
 template<> Fairy::BulletFlowSystemManager* Ogre::Singleton<Fairy::BulletFlowSystemManager>::ms_Singleton = 0;

namespace Fairy
{

	BulletFlowSystemManager::BulletFlowSystemManager(System* system) : m_system(system),
		m_createBulletFlowCallback(NULL),m_afterCreatedBulletSystemCallback(NULL),
		m_beforeDeleteBulletSystemCallback(NULL),m_bulletSystemHitTargetCallback(NULL)
	{
		m_scriptPatterns.push_back("*.bullet");

		Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

		registerOperatorFactories();
	}
	BulletFlowSystemManager::~BulletFlowSystemManager()
	{
		clearOperatorFactories();
	}
    /*************************************************************************
	Return singleton object
	*************************************************************************/
	BulletFlowSystemManager&	BulletFlowSystemManager::getSingleton(void)
	{
		return Ogre::Singleton<BulletFlowSystemManager>::getSingleton();
	}

	/*
	*	重载ScriptLoader纯虚函数
	*/
	const StringVector& BulletFlowSystemManager::getScriptPatterns(void) const
	{
		return m_scriptPatterns;
	}
	void BulletFlowSystemManager::parseScript(Ogre::DataStreamPtr& stream, const String& groupName)
	{
		m_ParsingFileName = stream->getName();
		String line;
        StringVector vecparams;
		mWrongLineNum = 0;

        while(!stream->eof())
        {
            line = stream->getLine();
            ++mWrongLineNum;

			if (!(line.length() == 0 || line.substr(0,2) == "//"))
            {
     
				vecparams = Ogre::StringUtil::split(line, "\t ");
				//assert (vecparams[0] == "effect");
				//assert (vecparams.size() == 2);
				if(vecparams[0] == "bulletflow" && vecparams.size() == 2)
				{
					// No current system
					// So first valid data should be a system name
					//pSys = Ogre::ParticleSystemManager::getSingleton().createTemplate(vecparams[1], mGroupName);
					//pSys->_notifyOrigin(stream->getName());
					// Skip to and over next {
					skipToNextOpenBrace(stream);

					parseBulletFlowTemplateSegment(vecparams[1],stream,groupName);

					continue;
				}
				else if (vecparams[0] == "bulletsystem" && vecparams.size() == 2)
				{
					skipToNextOpenBrace(stream);
					BulletSystemManager::getSingleton().parseBulletSystemTemplateSegment(vecparams[1],stream,groupName);

					continue;
				}
				
                skipToNextOpenBrace(stream);
			}
		}

	}
	Real BulletFlowSystemManager::getLoadingOrder(void) const
	{
		return 0;
	}
	void BulletFlowSystemManager::parseBulletFlowTemplateSegment(const String& name,
	Ogre::DataStreamPtr& stream, const String& groupName)
	{

		BulletFlowSystem *pBulletFlow = createBulletFlowSystemTemplate(name);

		assert (pBulletFlow);
		addToBulletFlowTemplateScriptFileMap(pBulletFlow->getTemplateName(),stream->getName());

		// Parse emitter details
		String line;

		mWrongLineNum = 0;

		while(!stream->eof())
		{
			line = stream->getLine();
            ++mWrongLineNum;

			// Ignore comments & blanks
			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{				
				if (line == "}")
				{
					// Finished emitter
					break;
				}
				else
				{
					StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);
					if(vecparams[0] == "eventsystem" || vecparams.size() != 2)
					{
						skipToNextOpenBrace(stream);
						parseNewBulletEventSystem(vecparams[1],stream,pBulletFlow);
					}
					else
					{
						parseAttrib(line, pBulletFlow);
					}
				}
			}
		}

	}
	BulletFlowSystem* BulletFlowSystemManager::createBulletFlowSystem(const String& templateName)
	{
		BulletFlowSystem *newFlowSystem = NULL;
	
		// Look up template
		BulletFlowSystem *pTemplate = getBulletFlowSystemTemplate(templateName);
		if (!pTemplate)
		{
            // 如果找不到所需特效模板，先用错误模板代替，如果连错误模板都找不到，就异常报错
            _logErrorInfo("can't find the required BulletFlowSystem template '" + templateName,
                "", "BulletFlowSystemManager::createBulletFlowSystem");


            if (!pTemplate)
			    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Cannot find required template '"
                "BulletFlowSystem_Error'",
				"BulletFlowSystemManager::createBulletFlowSystem");
		}
	
		newFlowSystem = new BulletFlowSystem(m_system);		

		// 复制参数，包括创建父子节点的连接关系
		*newFlowSystem = *pTemplate;

		//创建BulletSystem
		/*BulletFlowSystem::BulletEventMap::const_iterator it = 
			pTemplate->getBulletEventSystemsMap().begin();
		while(it != pTemplate->getBulletEventSystemsMap().end())
		{
			BulletEventSystem* pBulletEvent = it->second;
			if(pBulletEvent)
			{
				for(int i = 0;i<pBulletEvent->getNumBulletOperator();i++)
				{
					Ogre::String value = pBulletEvent->getBulletOperator(i)->getParameter("bulletsystemname");
					if(value != Ogre::StringUtil::BLANK)
					{
						newFlowSystem->addBulletSystem(value);
					}
				}
			}
			it ++;
		}*/

		assert (newFlowSystem);

		m_ActiveFlowSystemList.push_back(newFlowSystem);
	
		return newFlowSystem;
	}
	void	BulletFlowSystemManager::destroyBulletFlowSystem(BulletFlowSystem* bulletFlowSystem)
	{
		assert (bulletFlowSystem);

        ActiveFlowSystemList::iterator i = m_ActiveFlowSystemList.begin();

        while ( i != m_ActiveFlowSystemList.end() )
		{
			if (*i == bulletFlowSystem)
			{
				BulletFlowSystem* pBulletFlowSystem = *i;
				delete bulletFlowSystem;
				bulletFlowSystem = NULL;
				i = m_ActiveFlowSystemList.erase(i);
                break;
			}

            i++;
		}
	}
	void BulletFlowSystemManager::updateBulletFlowSystem(Ogre::Real time)
	{
		ActiveFlowSystemList::iterator i = m_ActiveFlowSystemList.begin();

        while ( i != m_ActiveFlowSystemList.end() )
		{
			if(*i)
				(*i)->update(time);
		}
	}
	BulletFlowSystem* BulletFlowSystemManager::createBulletFlowSystemTemplate(const Ogre::String& name)
	{
		BulletFlowSystem* ptr = NULL;
        BulletFlowSystemMap::iterator i = m_bulletFlowSystemTemplate.find(name);
        if (i == m_bulletFlowSystemTemplate.end())
        {
			ptr = new BulletFlowSystem(m_system);
			ptr->setTemplaterName(name);
			assert (ptr);

            m_bulletFlowSystemTemplate[ name ] = ptr;
        }
        else
        {
			OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
                "bulletFlowSystem template with name '" + name + "' already exists!",
                "BulletFlowSystemManager::createBulletFlowSystemTemplate");
        }

        return ptr;
	}
	void BulletFlowSystemManager::destroyBulletFlowSystemTemplate(const String& name)
	{
		BulletFlowSystem* ptr = NULL;
		BulletFlowSystemMap::iterator i = m_bulletFlowSystemTemplate.find(name);
		if (i != m_bulletFlowSystemTemplate.end())
		{
			ptr = i->second;
			delete ptr;
			m_bulletFlowSystemTemplate.erase(i);

		}
	}
	void BulletFlowSystemManager::addBulletFlowSystemTemplate(const String& name,BulletFlowSystem* bulletFlowSystem)
	{
		assert (bulletFlowSystem);

		std::pair<BulletFlowSystemMap::iterator, bool> inserted;

		inserted = m_bulletFlowSystemTemplate.insert(BulletFlowSystemMap::value_type(name, bulletFlowSystem) );

		assert (inserted.second);

	}
	BulletFlowSystem* BulletFlowSystemManager::getBulletFlowSystemTemplate(const String& name)
	{
		
		BulletFlowSystemMap::iterator it = m_bulletFlowSystemTemplate.find ( name );
		if( it != m_bulletFlowSystemTemplate.end() )
			return it->second;
		
		return NULL;
	}
	// 此函数由BulletFlowSystem::addBulletEventSystem函数调用 [1/23/2008 JiangWei]
	BulletEventSystem* BulletFlowSystemManager::createBulletEventSystem(const String& name,BulletFlowSystem* flowSystem)
	{
		return NULL;
	}
	void BulletFlowSystemManager::destroyBulletEventSystem(BulletEventSystem* bulletEventSystem)
	{

	}
	/*
	*	放弃BulletEventSystemManager，因为EventSystem不需要模板文件，所以原本
	*	BulletEventSystemManeger的功能全部放在此Manager内实现
	*/
	// 此函数由BulletEventSystem::addBulletOperator函数调用 [1/23/2008 JiangWei]
	BulletOperator*	BulletFlowSystemManager::createBulletOperator(const String& typeName,BulletEventSystem* eventSystem)
	{
		BulletOperatorFactoryMap::const_iterator i = m_operatorFactories.find(typeName);

		if ( i == m_operatorFactories.end() )
		{
			// exception
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
                "the bullet operator factory type '" + typeName + "' didn't exist!",
                "BulletFlowSystemManager::createBulletOperator");
		}

		BulletOperatorFactory *factory = i->second;
		assert (factory);
		
		BulletOperator *oprator = factory->createOperator(eventSystem);

		assert (oprator);

		return oprator;
	}
	void BulletFlowSystemManager::destroyBulletOperator(BulletOperator* bulletOperator)
	{
		if(bulletOperator)
		{
			delete bulletOperator;
			bulletOperator = NULL;
		}
	}
	void BulletFlowSystemManager::addBulletOperatorFactory(BulletOperatorFactory* operatorFactory)
	{
		assert (operatorFactory);

		std::pair<BulletOperatorFactoryMap::iterator, bool> inserted;

		inserted = m_operatorFactories.insert(BulletOperatorFactoryMap::value_type(operatorFactory->getName(),operatorFactory));

		assert (inserted.second);
	}
	void BulletFlowSystemManager::registerOperatorFactories()
	{
		BulletAgeTesterFactory* pAgeTesterFactory = new BulletAgeTesterFactory();
		addBulletOperatorFactory(pAgeTesterFactory);

		BulletBirthOperatorFactory* pBirthOperatorFactory = new BulletBirthOperatorFactory();
		addBulletOperatorFactory(pBirthOperatorFactory);

		BulletCameraShakeOperatorFactory* pCameraShakeOperatorFactory = new BulletCameraShakeOperatorFactory();
		addBulletOperatorFactory(pCameraShakeOperatorFactory);

		BulletDeleterOperatorFactory* pDeleterOperatorFactory = new BulletDeleterOperatorFactory();
		addBulletOperatorFactory(pDeleterOperatorFactory);

		BulletFindTargetTesterFactory* pFindTargetTesterFactory = new BulletFindTargetTesterFactory();
		addBulletOperatorFactory(pFindTargetTesterFactory);

		BulletForceOperatorFactory* pForceOperatorFactory = new BulletForceOperatorFactory();
		addBulletOperatorFactory(pForceOperatorFactory);

		BulletHeightFieldTesterFactory* pHeightFieldTesterFactory = new BulletHeightFieldTesterFactory();
		addBulletOperatorFactory(pHeightFieldTesterFactory);

		BulletScaleTesterFactory* pScaleTesterFactory = new BulletScaleTesterFactory();
		addBulletOperatorFactory(pScaleTesterFactory);

		BulletScriptTesterFactory* pScriptTesterFactory = new BulletScriptTesterFactory();
		addBulletOperatorFactory(pScriptTesterFactory);

		BulletSpawnOperatorFactory* pSpawnOperatorFactory = new BulletSpawnOperatorFactory();
		addBulletOperatorFactory(pSpawnOperatorFactory);

		BulletSpeedOperatorFactory* pSpeedOperatorFactory = new BulletSpeedOperatorFactory();
		addBulletOperatorFactory(pSpeedOperatorFactory);

		BulletSwayOperatorFactory* pShakeOperatorFactory = new BulletSwayOperatorFactory();
		addBulletOperatorFactory(pShakeOperatorFactory);

		BulletHelixOperatorFactory* pHelixOperatorFactory = new BulletHelixOperatorFactory();
		addBulletOperatorFactory(pHelixOperatorFactory);

	}
	void BulletFlowSystemManager::clearOperatorFactories()
	{
		BulletOperatorFactoryMap::iterator it = m_operatorFactories.begin();
		while(it != m_operatorFactories.end())
		{
			delete (*it).second;
			(*it).second = NULL;
			it ++;
		}
		m_operatorFactories.clear();
	}

	BulletFlowSystemManager::BulletFlowTemplateIterator 
		BulletFlowSystemManager::getBulletFlowTemplateIterator()
	{
		return BulletFlowTemplateIterator(
			m_bulletFlowSystemTemplate.begin(), m_bulletFlowSystemTemplate.end());
	}
	// 解析BulletFlowSystem属性 [1/19/2008 JiangWei]
	void BulletFlowSystemManager::parseAttrib(const String& line,BulletFlowSystem* bulletFlowSystem)
	{
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);
		if ( vecparams.size() != 2 || false == bulletFlowSystem->setParameter(vecparams[0], vecparams[1]) )
		{
			_logErrorInfo("Bad Anim BulletFlowSystem attribute line", line, "BulletFlowSystemManager::parseAttrib");
		}
	}
	// 解析新BulletEventSystem [1/19/2008 JiangWei]
	void BulletFlowSystemManager::parseNewBulletEventSystem(const String& name,Ogre::DataStreamPtr& stream,
	BulletFlowSystem* bulletFlowSystem)
	{
		BulletEventSystem* pEventSystem = bulletFlowSystem->addBulletEventSystem(name);

		assert(pEventSystem);

		// Parse emitter details
		String line;

		while(!stream->eof())
		{
			line = stream->getLine();
            ++mWrongLineNum;

			// Ignore comments & blanks
			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{				
				if (line == "}")
				{
					// Finished emitter
					break;
				}
				else
				{
					StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);
					if(vecparams[0] == "operator" || vecparams[0] == "tester"|| vecparams.size() != 2)
					{
						skipToNextOpenBrace(stream);
						parseNewBulletOperator(vecparams[1],stream,pEventSystem);
					}
					else
					{
						parseBulletEventSystemAttrib(line, pEventSystem);
					}
				}
			}
		}

	}
	void BulletFlowSystemManager::parseBulletEventSystemAttrib(const String& line,BulletEventSystem* bulletEventSystem)
	{
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);
		if ( vecparams.size() != 2 || false == bulletEventSystem->setParameter(vecparams[0], vecparams[1]) )
		{
			_logErrorInfo("Bad Anim BulletEventSystem attribute line", line, "BulletFlowSystemManager::parseBulletEventSystemAttrib");
		}
	}
	// 解析行BulletOperator [1/19/2008 JiangWei]
	void BulletFlowSystemManager::parseNewBulletOperator(const String& name,Ogre::DataStreamPtr& stream,BulletEventSystem* eventSystem)
	{
		BulletOperator*	pOperator = eventSystem->addBulletOperator(name);
		assert(pOperator);

		// Parse emitter details
		String line;

		while(!stream->eof())
		{
			line = stream->getLine();
            ++mWrongLineNum;

			// Ignore comments & blanks
			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{				
				if (line == "}")
				{
					// Finished emitter
					break;
				}
				else
				{
					parseBulletOperatorAttrib(line, pOperator);
					
				}
			}
		}
	}
	void BulletFlowSystemManager::parseBulletOperatorAttrib(const String& line,BulletOperator* bulletOperator)
	{
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);
		if ( vecparams.size() != 2)
		{
			_logErrorInfo("Bad Anim BulletOperator attribute line", line, "BulletFlowSystemManager::parseBulletOperatorAttrib");
			bulletOperator->setParameter(vecparams[0], "");
			return;
		}
		bulletOperator->setParameter(vecparams[0], vecparams[1]);
	}
	void BulletFlowSystemManager::skipToNextCloseBrace(Ogre::DataStreamPtr& stream)
    {
       Ogre::String line = "";
        while (!stream->eof() && line != "}")
        {
            line = stream->getLine();
            ++mWrongLineNum;
        }

    }
    //-----------------------------------------------------------------------
    void BulletFlowSystemManager::skipToNextOpenBrace(Ogre::DataStreamPtr& stream)
    {
       Ogre::String line = "";
        while (!stream->eof() && line != "{")
        {
            line = stream->getLine();
            ++mWrongLineNum;
        }

    }
	/// 输出错误信息
	void BulletFlowSystemManager::_logErrorInfo(const String& errorInfo, const String& lineContent, 
							const String& functionName)
	{

	}

	//---------------------------------------------------------------------
	void BulletFlowSystemManager::addToBulletFlowTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName )
	{
		assert ( false == templateName.empty() && false == fileName.empty() );

		std::pair<BulletTemplateScriptFileMap::iterator, bool> inserted = 
			m_BulletTemplateScriptFileMap.insert( BulletTemplateScriptFileMap::value_type(templateName, fileName) );

		assert (inserted.second);
	}
	void BulletFlowSystemManager::removeBulletFlowTemplateScriptFileMap( const Ogre::String &templateName)
	{
		BulletTemplateScriptFileMap::iterator i = m_BulletTemplateScriptFileMap.find(templateName);

		if( i!= m_BulletTemplateScriptFileMap.end() )
			m_BulletTemplateScriptFileMap.erase(i);
	}
	//---------------------------------------------------------------------
	const Ogre::String & BulletFlowSystemManager::getFileNameByTemplateName( const Ogre::String &templateName )
	{
		BulletTemplateScriptFileMap::iterator i = m_BulletTemplateScriptFileMap.find(templateName);

		assert ( i!= m_BulletTemplateScriptFileMap.end() );

		return i->second;
	}
	//---------------------------------------------------------------------
	void BulletFlowSystemManager::getTemplatesFromScriptFile( const Ogre::String &fileName, StringVector &templates )
	{
		for ( BulletTemplateScriptFileMap::iterator i = m_BulletTemplateScriptFileMap.begin();
			i != m_BulletTemplateScriptFileMap.end(); ++i )
		{
			if ( i->second == fileName )
			{
				templates.push_back(i->first);
			}
		}
	}

}