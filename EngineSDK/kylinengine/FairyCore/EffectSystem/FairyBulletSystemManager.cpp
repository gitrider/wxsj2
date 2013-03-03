
#include "FairyBulletSystemManager.h"
#include "FairyBulletSystem.h"
#include "FairyAnimationEffectInfo.h"
#include "FairyAnimationRibbon.h"
#include "FairyAnimationSceneLightInfo.h"
#include "FairyAnimationSound.h"
#include "FairyAnimationBulletFlow.h"
#include "FairyAnimationCameraShake.h"
#include "FairyAnimationLine.h"
#include <OgreException.h>
template<> Fairy::BulletSystemManager* Ogre::Singleton<Fairy::BulletSystemManager>::ms_Singleton = 0;

namespace Fairy
{
	BulletSystemManager::BulletSystemManager(System* system)
	{
		m_system = system;
	}
	BulletSystemManager::~BulletSystemManager()
	{

	}
	BulletSystemManager&	BulletSystemManager::getSingleton(void)
	{
		return Ogre::Singleton<BulletSystemManager>::getSingleton();
	}

	const StringVector& BulletSystemManager::getScriptPatterns(void) const
	{
		return m_scriptPatterns;
	}
	void BulletSystemManager::parseScript(Ogre::DataStreamPtr& stream, const String& groupName)
	{

	}
	Real BulletSystemManager::getLoadingOrder(void) const
	{
		return 0;
	}
	void BulletSystemManager::parseBulletSystemTemplateSegment(const String& templateName,
		Ogre::DataStreamPtr& stream, const String& groupName)
	{
		BulletSystem *pBulletSystem = createBulletSystemTemplate(templateName);

		assert (pBulletSystem);

		addToBulletSystemTemplateScriptFileMap(pBulletSystem->getTemplateName(),stream->getName());

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
					if(vecparams[0] == "AnimEffect" /*|| vecparams.size() != 2*/)
					{
						skipToNextOpenBrace(stream);
						parseAnimEffectInfo(stream, pBulletSystem);
					}
					else if(vecparams[0] == "Ribbon" /*|| vecparams.size() != 2*/)
					{
						skipToNextOpenBrace(stream);
						parseAnimRibbon(stream, pBulletSystem);
					}
					else if(vecparams[0] == "Sound" /*|| vecparams.size() != 2*/)
					{
						skipToNextOpenBrace(stream);
						parseAnimSound(stream, pBulletSystem);
					}
					else if(vecparams[0] == "SceneLight" /*|| vecparams.size() != 2*/)
					{
						skipToNextOpenBrace(stream);
						parseAnimSceneLightInfo(stream, pBulletSystem);
					}
					else if(vecparams[0] == "CameraShake" /*|| vecparams.size() != 2*/)
					{
						skipToNextOpenBrace(stream);
						parseAnimSceneCamerShake(stream, pBulletSystem);
					}
					else if(vecparams[0] == "Line")
					{
						skipToNextOpenBrace(stream);
						parseAnimLine(stream, pBulletSystem);
					}
					else
					{
						parseAttrib(line, pBulletSystem);
					}

				}
			}
		}

	}
	// 此函数由BulletFlowSystem::addBulletSystem函数调用 [1/23/2008 JiangWei]
	BulletSystem* BulletSystemManager::createBulletSystem(const String& name)
	{
		BulletSystem *newBulletSystem = NULL;
	
		// Look up template
		BulletSystem *pTemplate = getBulletSystemTemplate(name);
		if (!pTemplate)
		{
            // 如果找不到所需特效模板，先用错误模板代替，如果连错误模板都找不到，就异常报错
            _logErrorInfo("can't find the required BulleSystem template '" + name,
                "", "BulletSystemManager::createBulletSystem");


            if (!pTemplate)
			    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Cannot find required template '"
                "BulletSystem_Error'",
				"BulletSystemManager::createBulletSystem");
		}			

		newBulletSystem = new BulletSystem(name,m_system);		

		// 复制参数，包括创建父子节点的连接关系
		*newBulletSystem = *pTemplate;

		assert (newBulletSystem);
	
		return newBulletSystem;
	}
	void BulletSystemManager::destroyBulletSystem(BulletSystem* bulletSystem)
	{
		if(bulletSystem)
		{
			delete bulletSystem;
			bulletSystem = NULL;
		}
	}
	BulletSystem* BulletSystemManager::createBulletSystemTemplate(const String& name)
	{
		BulletSystem* ptr = NULL;
        BulletSystemMap::iterator i = m_systemTemplate.find(name);
        if (i == m_systemTemplate.end())
        {
			ptr = new BulletSystem(name,m_system);
			assert (ptr);

            m_systemTemplate[ name ] = ptr;
        }
        else
        {
			OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
                "BulletSystem template with name '" + name + "' already exists!",
                "BulletSystemManager::createBulletSystemTemplate");
        }

        return ptr;
	}
	void BulletSystemManager::destroyBulletSystemTemplate(BulletSystem* bulletSystem)
	{
		BulletSystemMap::iterator it = m_systemTemplate.find(bulletSystem->getTemplateName());
		if (it != m_systemTemplate.end())
		{
			BulletSystem* pSystem = it->second;
			if(pSystem)
			{
				BulletSytemTemplateScriptFileMap::iterator scriptFile_it = 
					m_BulletSytemTemplateScriptFileMap.find(it->first);
				if(scriptFile_it != m_BulletSytemTemplateScriptFileMap.end())
				{
					m_BulletSytemTemplateScriptFileMap.erase(scriptFile_it);
				}
				m_systemTemplate.erase(it);
				delete pSystem;
				pSystem = NULL;
			}
		}
	}
	void BulletSystemManager::addBulletSystemTemplate(const String& name,BulletSystem* bulletSystem)	
	{
		m_systemTemplate.insert(std::pair<String,BulletSystem*>(name,bulletSystem));

	}
	BulletSystem*	BulletSystemManager::getBulletSystemTemplate(const String& name)
	{
		std::map<String,BulletSystem*>::iterator it = m_systemTemplate.find(name);
		if(it != m_systemTemplate.end())
		{
			return (*it).second;
		}
		return NULL;
	}

	BulletSystemManager::BulletSystemTemplateIterator BulletSystemManager::getBulletSystemTemplateIterator()
	{
		return BulletSystemTemplateIterator(
			m_systemTemplate.begin(), m_systemTemplate.end());
	}

	void BulletSystemManager::skipToNextCloseBrace(Ogre::DataStreamPtr& stream)
    {
       Ogre::String line = "";
        while (!stream->eof() && line != "}")
        {
            line = stream->getLine();
            ++mWrongLineNum;
        }

    }
    //-----------------------------------------------------------------------
    void BulletSystemManager::skipToNextOpenBrace(Ogre::DataStreamPtr& stream)
    {
       Ogre::String line = "";
        while (!stream->eof() && line != "{")
        {
            line = stream->getLine();
            ++mWrongLineNum;
        }

    }
	void BulletSystemManager::parseAttrib(const String& line, BulletSystem* bulletSystem)
	{
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);
		if ( vecparams.size() != 2)
		{
			_logErrorInfo("Bad Anim BulletSystem attribute line", line, "BulletSystemManager::parseAttrib");
		}
		bulletSystem->setParameter(vecparams[0], vecparams[1]);
	}
	void BulletSystemManager::parseAnimEffectInfo(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem)
	{
		assert (bulletSystem);

		AnimationEffectInfo *effectInfo = bulletSystem->addAnimationEffectInfo();

		assert (effectInfo);

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
					// Attribute
					//	Ogre::StringUtil::toLowerCase(line);
					parseAnimEffectInfoAttrib(line, effectInfo);
				}
			}
		}
	}
	void BulletSystemManager::parseAnimEffectInfoAttrib(const String& line, AnimationEffectInfo *effectInfo)
	{
		// 设置element的属性
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

        if (vecparams.size() != 2)
        {
            _logErrorInfo("the number of parameters must be 2!", line, "BulletSystemManager::parseAnimEffectInfoAttrib");
            return;
        }

		if ( false == effectInfo->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad Anim Effect Info attribute line", line, "BulletSystemManager::parseAnimEffectInfoAttrib");
		}
	}
	void BulletSystemManager::parseAnimRibbon(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem)	
	{
		assert (bulletSystem);

		AnimationRibbon *ribbon = bulletSystem->addAnimationRibbon();

		assert (ribbon);

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
					// Attribute
					//	Ogre::StringUtil::toLowerCase(line);
					parseAnimRibbonAttrib(line, ribbon);
				}
			}
		}
	}
	void BulletSystemManager::parseAnimRibbonAttrib(const String& line, AnimationRibbon *ribbon)
	{
		// 设置element的属性
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (  vecparams.size() != 2 || false == ribbon->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad Anim ribbon attribute line", line, "BulletSystemManager::parseAnimRibbonAttrib");
		}

	}
	void BulletSystemManager::parseAnimSound(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem)	
	{
		assert (bulletSystem);

		AnimationSound *sound = bulletSystem->addAnimationSound();

		assert (sound);

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
					// Attribute
					//	Ogre::StringUtil::toLowerCase(line);
					parseAnimSoundAttrib(line, sound);
				}
			}
		}
	}
	void BulletSystemManager::parseAnimSoundAttrib(const String& line, AnimationSound *sound)
	{
		// 设置element的属性
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (  vecparams.size() != 2 || false == sound->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad Anim sound attribute line", line, "BulletSystemManager::parseAnimSoundAttrib");
		}
	}
	void BulletSystemManager::parseAnimSceneLightInfo(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem)
	{
		assert (bulletSystem);

        AnimationSceneLightInfo *sceneLightInfo = bulletSystem->addAnimationSceneLightInfo();

        assert (sceneLightInfo);

        // Parse emitter details
       Ogre::String line;

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
                    // Attribute
                    //	Ogre::StringUtil::toLowerCase(line);
                    parseAnimSceneLightInfoAttrib(line, sceneLightInfo);
                }
            }
        }
	}
	void BulletSystemManager::parseAnimSceneLightInfoAttrib(const String& line, AnimationSceneLightInfo *sceneLightInfo)
	{
		 // 设置element的属性
        StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

        if (vecparams.size() != 2)
        {
            _logErrorInfo("the number of parameters must be 2!", line, "BulletSystemManager::parseAnimSceneLightInfoAttrib");
            return;
        }

        if ( false == sceneLightInfo->setParameter(vecparams[0], vecparams[1]) )
        {
            _logErrorInfo("Bad Anim Scene Light Info attribute line", line, "BulletSystemManager::parseAnimSceneLightInfoAttrib");
        }
	}
	void BulletSystemManager::parseAnimSceneCamerShake(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem)
	{
		assert (bulletSystem);

		AnimationCameraShake *cameraShake = bulletSystem->addAnimationCameraShake();

		assert (cameraShake);

		// Parse emitter details
		Ogre::String line;

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
					// Attribute
					//	Ogre::StringUtil::toLowerCase(line);
					parseAnimSceneCamerShakeAttrib(line, cameraShake);
				}
			}
		}
	}
	void BulletSystemManager::parseAnimSceneCamerShakeAttrib(const String& line,AnimationCameraShake *cameraShake)
	{
		// 设置element的属性
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (vecparams.size() != 2)
		{
			_logErrorInfo("the number of parameters must be 2!", line, "BulletSystemManager::parseAnimSceneCamerShakeAttrib");
			return;
		}

		if ( false == cameraShake->setParameter(vecparams[0], vecparams[1]) )
		{
			_logErrorInfo("Bad Anim Scene Light Info attribute line", line, "BulletSystemManager::parseAnimSceneCamerShakeAttrib");
		}
	}
	void BulletSystemManager::parseAnimLine(Ogre::DataStreamPtr &stream, BulletSystem *bulletSystem)
	{
		assert (bulletSystem);

		AnimationLine *pAnimiationline = bulletSystem->addAnimationLine();

		assert (pAnimiationline);

		// Parse emitter details
		Ogre::String line;

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
					// Attribute
					//	Ogre::StringUtil::toLowerCase(line);
					parseAnimLineAttrib(line, pAnimiationline);
				}
			}
		}

	}
	void BulletSystemManager::parseAnimLineAttrib(const String& line, AnimationLine *animationLine)
	{
		// 设置element的属性
		StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (vecparams.size() != 2)
		{
			_logErrorInfo("the number of parameters must be 2!", line, "BulletSystemManager::parseAnimLineAttrib");
			return;
		}

		if ( false == animationLine->setParameter(vecparams[0], vecparams[1]) )
		{
			_logErrorInfo("Bad Anim line attribute line", line, "BulletSystemManager::parseAnimLineAttrib");
		}

	}

	/// 输出错误信息
	void BulletSystemManager::_logErrorInfo(const String& errorInfo, const String& lineContent, 
		const String& functionName)
	{

	}

	//---------------------------------------------------------------------
	void BulletSystemManager::addToBulletSystemTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName )
	{
		assert ( false == templateName.empty() && false == fileName.empty() );

		std::pair<BulletSytemTemplateScriptFileMap::iterator, bool> inserted = 
			m_BulletSytemTemplateScriptFileMap.insert( BulletSytemTemplateScriptFileMap::value_type(templateName, fileName) );

		assert (inserted.second);
	}
	//---------------------------------------------------------------------
	const Ogre::String & BulletSystemManager::getFileNameByTemplateName( const Ogre::String &templateName )
	{
		BulletSytemTemplateScriptFileMap::iterator i = m_BulletSytemTemplateScriptFileMap.find(templateName);

		assert ( i!= m_BulletSytemTemplateScriptFileMap.end() );

		return i->second;
	}
	//---------------------------------------------------------------------
	void BulletSystemManager::getTemplatesFromScriptFile( const Ogre::String &fileName, StringVector &templates )
	{
		for ( BulletSytemTemplateScriptFileMap::iterator i = m_BulletSytemTemplateScriptFileMap.begin();
			i != m_BulletSytemTemplateScriptFileMap.end(); ++i )
		{
			if ( i->second == fileName )
			{
				templates.push_back(i->first);
			}
		}
	}
	void BulletSystemManager::removeBulletSystemTemplateScriptFileMap( const Ogre::String &templateName)
	{
		BulletSytemTemplateScriptFileMap::iterator i = m_BulletSytemTemplateScriptFileMap.find(templateName);

		if( i!= m_BulletSytemTemplateScriptFileMap.end() )
		{
			m_BulletSytemTemplateScriptFileMap.erase(i);
		}
	}



}