/********************************************************************
filename:   FairyEffectManager.cpp

purpose:    effect manager manage all effects, skills and path files.
*********************************************************************/

#include "FairyEffectManager.h"
#include "FairyEffect.h"
#include "FairyEffectElementFactory.h"
#include "FairyEffectElement.h"
#include "FairySkill.h"
#include "FairyAnimationRibbon.h"
#include "FairyAnimationEffectInfo.h"
#include "FairyAnimationSound.h"
#include "FairyAnimationSceneLightInfo.h"
#include "FairyAnimationBulletFlow.h"
#include "FairyAnimationCameraShake.h"
#include "FairyMainSceneLight.h"
#include "FairyCylinderElement.h"

#include "Core/FairySystem.h"

// ogre header
#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreLogManager.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreControllerManager.h>
#include <OgreController.h>
#include <OgrePredefinedControllers.h>
#include <OgreParticleSystemManager.h>
#include <OgreParticleSystem.h>
#include <OgreParticleAffector.h>
#include <OgreParticleEmitter.h>
#include <OgreBillboardParticleRenderer.h>

template<> Fairy::EffectManager * Ogre::Singleton<Fairy::EffectManager>::ms_Singleton = 0;

namespace Fairy	{

/// �Ƿ�ʹ��effect pool
#define USE_EFFECT_POOL 1

/// �Ƿ������Ч������Ϣ
#define OUTPUT_DEBUG_INFO 1

#if OUTPUT_DEBUG_INFO

	unsigned int succeed = 0;
	unsigned int fail_nolist = 0;
	unsigned int fail_nomap = 0;
	unsigned int large = 0;
	unsigned int newMap = 0;

#endif
	//////////////////////////////////////////////////////////////////////////
	namespace
	{
        class Fairy::EffectManager;

		class EffectControllerValue : public Ogre::ControllerValue<float>
		{
		protected:
			EffectManager *mEffectMgr;

		public:

			EffectControllerValue(EffectManager *effectMgr)
				: mEffectMgr(effectMgr)
			{
			}

			float getValue(void) const
			{
				return 0;
			}

			void setValue(float value)
			{
				mEffectMgr->updateActiveEffects(value);

                // ������Ч�ƹ�
                mEffectMgr->getMainSceneLight()->updateLight(value);
			}
		};
	}

	//////////////////////////////////////////////////////////////////////////
	
    EffectManager *EffectManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
	//---------------------------------------------------------------------
    EffectManager & EffectManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
	//---------------------------------------------------------------------
	EffectManager::EffectManager( System *system )	:
	mSystem(system), 
	mController(NULL),
	mMaxNumFreeEffectPerList(10),
    mWrongLineNum(0),
    mParsingFileName(""),
    mMainSceneLight(NULL),
    mEffectLevel(EL_HIGH)
	{
		assert (mSystem);

		mScriptPatterns.push_back("*.effect");
		mScriptPatterns.push_back("*.skill");

		Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

		mActiveEffectList.clear();
		mFreeEffectMap.clear();
		mEffectTemplateMap.clear();
		mEffectTemplateScriptFileMap.clear();
		mSkillTemplateScriptFileMap.clear();
		mParticleTemplateScriptFileMap.clear();
		mLoadedScripts.clear();
		mLoadedSkillScripts.clear();

		registerElementFactory();
		createController();

        mMainSceneLight = new MainSceneLight(mSystem);
	}
	//---------------------------------------------------------------------
	EffectManager::~EffectManager()
	{
        delete mMainSceneLight;

		Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);

		// ɾ��Effect map
		for ( ActiveEffectList::iterator i = mActiveEffectList.begin();
			i != mActiveEffectList.end();	++i	)
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mActiveEffectList.clear();

        clearAllFreeEffects();

		_destroyEffectTemplates();

		// ɾ��EffectFactory map
		for ( ElementFactoryMap::iterator i = mElementFactoryMap.begin();
			i != mElementFactoryMap.end();	++i	)
		{
			if (i->second)
			{
				delete i->second;
				i->second = NULL;
			}
		}
		mElementFactoryMap.clear();	

		_destroySkillTemplates();

		if (mController)
		{
			Ogre::ControllerManager &controllerManager = Ogre::ControllerManager::getSingleton();
			controllerManager.destroyController(mController);
			mController = NULL;
		}

#if OUTPUT_DEBUG_INFO

		Ogre::LogManager::getSingleton().logMessage("getFromFreeMap failed nomap : " + Ogre::StringConverter::toString(fail_nomap));
		Ogre::LogManager::getSingleton().logMessage("getFromFreeMap succeed : " + Ogre::StringConverter::toString(succeed));
		Ogre::LogManager::getSingleton().logMessage("getFromFreeMap failed nolist : " + Ogre::StringConverter::toString(fail_nolist));
		Ogre::LogManager::getSingleton().logMessage("larger than mMaxNumFreeEffectPerList : " + Ogre::StringConverter::toString(large));
		Ogre::LogManager::getSingleton().logMessage("new freemap : " + Ogre::StringConverter::toString(newMap));
#endif
	}
	//---------------------------------------------------------------------
	const Ogre::StringVector& EffectManager::getScriptPatterns(void) const
	{
		return mScriptPatterns;
	}

	//---------------------------------------------------------------------
    void EffectManager::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
	{
		mParsingFileName = stream->getName();
		mGroupName = groupName;

        // �ڽ���ÿ���ļ�ʱ���к�����
        mWrongLineNum = 0;

        if (Ogre::StringUtil::endsWith(mParsingFileName, ".effect", false))
		{
			// ������Ч�ļ�
			parseEffectFile(stream);
		}
		else if (Ogre::StringUtil::endsWith(mParsingFileName, ".skill", false))
		{
			//������·���ļ�
			parseSkillFile(stream);
		}
		else
		{
			// �쳣
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "wrong file name " + mParsingFileName, "EffectManager::parseScript");
		}		
	}
	//---------------------------------------------------------------------
    float EffectManager::getLoadingOrder(void) const
	{
		return 1000.0f;
	}
	//-----------------------------------------------------------------------
    void EffectManager::skipToNextCloseBrace(Ogre::DataStreamPtr& stream)
    {
       Ogre::String line = "";
        while (!stream->eof() && line != "}")
        {
            line = stream->getLine();
            ++mWrongLineNum;
        }

    }
    //-----------------------------------------------------------------------
    void EffectManager::skipToNextOpenBrace(Ogre::DataStreamPtr& stream)
    {
       Ogre::String line = "";
        while (!stream->eof() && line != "{")
        {
            line = stream->getLine();
            ++mWrongLineNum;
        }

    }
	//---------------------------------------------------------------------
	Effect * EffectManager::createEffectTemplate(const Ogre::String &name)
	{
		Effect *ret = NULL;
        EffectTemplateMap::iterator i = mEffectTemplateMap.find(name);
        if (i == mEffectTemplateMap.end())
        {
			assert (mSystem);
			ret = new Effect(name, mSystem);
			assert (ret);

            mEffectTemplateMap[name] = ret;
        }
        else
        {
			OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
                "Effect template with name '" + name + "' already exists!",
                "EffectManager::createEffectTemplate");
        }

        return ret;
	}
	//---------------------------------------------------------------------
	Skill * EffectManager::createSkillTemplate(const Ogre::String &name)
	{
		Skill *ret = NULL;
		SkillMap::iterator i = mSkillMap.find(name);
		if (i == mSkillMap.end())
		{
			assert (mSystem);
			ret = new Skill(name, mSystem);
			assert (ret);

			mSkillMap[name] = ret;
		}
		else
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
				"skill template with name '" + name + "' already exists!",
				"EffectManager::createSkillTemplate");
		}

		return ret;
	}
	//---------------------------------------------------------------------
	void EffectManager::parseNewElement(const Ogre::String &type, Ogre::DataStreamPtr &stream, Effect *effectParent)
	{
		// Create new emitter
       // ParticleEmitter* pEmit = sys->addEmitter(type);
		EffectElement *element = createElement(type);
		assert (element);

		effectParent->addElement(element);

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
                    parseElementAttrib(line, element);
                }
            }
        }
	}
	//---------------------------------------------------------------------
	void EffectManager::registerElementFactory(void)
	{
		addElementFactory( new ParticleElementFactory() );
		addElementFactory( new MeshElementFactory() );
		addElementFactory( new BillboardSetElementFactory() );
		addElementFactory( new ProjectorElementFactory() );
		addElementFactory( new BeamElementFactory() ); 
		addElementFactory( new PointLightElementFactory() );
		addElementFactory( new CylinderEffectElementFactory() );
	}
	//---------------------------------------------------------------------
	void EffectManager::addElementFactory( EffectElementFactory *factory )
	{
		assert (factory);

		std::pair<ElementFactoryMap::iterator, bool> inserted;

		inserted = mElementFactoryMap.insert( ElementFactoryMap::value_type( factory->getName(), factory) );

		assert (inserted.second);
	}
	//---------------------------------------------------------------------
	EffectElement * EffectManager::createElement(const Ogre::String &type)
	{
		ElementFactoryMap::const_iterator i = mElementFactoryMap.find(type);

		if ( i == mElementFactoryMap.end() )
		{
			// exception
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
                "the element type '" + type + "' didn't exist!",
                "EffectManager::createElement");
		}

		EffectElementFactory *factory = i->second;
		assert (factory);
		
		EffectElement *element = factory->createElement(mSystem);

		assert (element);

		return element;
	}
	//---------------------------------------------------------------------
	void EffectManager::parseElementAttrib(const Ogre::String& line, EffectElement *element)
	{
        // ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

        if (vecparams.size() != 2)
        {
            _logErrorInfo("the number of parameters must be 2!", line, "EffectManager::parseElementAttrib");
            return;
        }

        if ( false == element->setParameter(vecparams[0], vecparams[1]) )
        {
            _logErrorInfo("Bad effect element attribute line", line, "EffectManager::parseElementAttrib");
        }
	}	
	//---------------------------------------------------------------------
	void EffectManager::parseSkillAttrib(const Ogre::String& line, Skill *skill)
	{
		// ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

        if (vecparams.size() != 2)
        {
            _logErrorInfo("the number of parameters must be 2!", line, "EffectManager::parseSkillAttrib");
            return;
        }

		if ( false == skill->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad skill attribute line", line, "EffectManager::parseSkillAttrib");
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::parseAnimEffectInfoAttrib(const Ogre::String& line, AnimationEffectInfo *effectInfo)
	{
		// ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

        if (vecparams.size() != 2)
        {
            _logErrorInfo("the number of parameters must be 2!", line, "EffectManager::parseAnimEffectInfoAttrib");
            return;
        }

		if ( false == effectInfo->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad Anim Effect Info attribute line", line, "EffectManager::parseAnimEffectInfoAttrib");
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::dump(const Ogre::String &fileName)
	{
		// ���ÿ����Ч����Ϣ������element�ĸ���������
		std::ofstream of(fileName.c_str());
		of << "Effect System debug info " << std::endl;
		of << "Number of effect: " << mEffectTemplateMap.size() << std::endl;
		of << "Number of effect element factory: " << mElementFactoryMap.size() << std::endl;

		for ( EffectTemplateMap::iterator i = mEffectTemplateMap.begin();
			i != mEffectTemplateMap.end();	++i )
		{
			of << "-------------------------------------------------" << std::endl;

			const Effect *effect = i->second;
			effect->dump(of);
		}		

		of << "-------------------------------------------------" << std::endl;
		of << "Number of effect: " << mActiveEffectList.size() << std::endl;

		for ( ActiveEffectList::iterator i = mActiveEffectList.begin();
			i != mActiveEffectList.end();	++i )
		{
			of << "-------------------------------------------------" << std::endl;

			const Effect *effect = *i;
			effect->dump(of);
		}
	}
	//---------------------------------------------------------------------
	Effect * EffectManager::createEffect( const Ogre::String &templateName )
	{
		Effect *newEffect = NULL;
		newEffect = getFromFreeMap(templateName);

		if ( NULL == newEffect )
		{		
			// Look up template
			Effect *pTemplate = getTemplate(templateName);
			if (!pTemplate)
			{
                // ����Ҳ���������Чģ�壬���ô���ģ����棬���������ģ�嶼�Ҳ��������쳣����
                _logErrorInfo("can't find the required effect template '" + templateName,
                    "", "EffectManager::createEffect");

                pTemplate = getTemplate("Effect_Error");

                if (!pTemplate)
				    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Cannot find required template '"
                    "Effect_Error'",
					"EffectManager::createEffect");
			}			

			newEffect = new Effect(templateName, mSystem);			

			// ���Ʋ����������������ӽڵ�����ӹ�ϵ
			*newEffect = *pTemplate;

	//		newEffect->createSceneNode();
		}	

		assert (newEffect);

		mActiveEffectList.push_back(newEffect);
	
		return newEffect;
	}
	//---------------------------------------------------------------------
	Skill *EffectManager::getSkill( const Ogre::String &skillName )
	{
		SkillMap::iterator i = mSkillMap.find(skillName);

		if ( i != mSkillMap.end() )
		{
			return i->second;
		}
		else
		{
			return NULL;
		}
	}
	//-----------------------------------------------------------------------
    Effect * EffectManager::getTemplate(const Ogre::String& name)
    {
        EffectTemplateMap::iterator i = mEffectTemplateMap.find(name);
        if (i != mEffectTemplateMap.end())
        {
            return i->second;
        }
        else
        {
            return NULL;
        }
    }
	//---------------------------------------------------------------------
	EffectManager::ActiveEffectList::iterator EffectManager::destroyEffect( Effect *effect )
	{
		assert (effect);

        ActiveEffectList::iterator i = mActiveEffectList.begin();

        while ( i != mActiveEffectList.end() )
		{
			if (*i == effect)
			{
				delete *i;
				*i = NULL;
				i = mActiveEffectList.erase(i);
                break;
			}

            ++i;
		}

        return i;
	}
	//---------------------------------------------------------------------
	void EffectManager::removeEffect( Effect *effect, bool removeParentNode, bool removeNow )
	{
		assert (effect);

        effect->setRemoveParentNode(removeParentNode);

        if (removeNow)
        {
            for ( ActiveEffectList::iterator i = mActiveEffectList.begin();
                i != mActiveEffectList.end(); ++i )
            {
                if (*i == effect)
                {
                    /// �����е���ЧԪ����˯��
                    effect->removeAllElements();

                    if (effect->getRemoveParentNode())
                    {
                        effect->removeParentSceneNode();
                    }

                    addToFreeEffectMap(effect);
                    mActiveEffectList.erase(i);
                    return;
                }
            }
        }
        else
        {
            // �ر���Ч������ر����ӷ������Ȳ�����
            effect->shutdown();
            effect->setDelayRemove(true);
        }
	}
	//---------------------------------------------------------------------
	void EffectManager::parseEffectFile( Ogre::DataStreamPtr &stream )
	{
		addToLoadedScripts(stream->getName());

		String line;
        Effect *pEffect = NULL;
		Ogre::StringVector vecparams;
		Ogre::ParticleSystem* pSys = NULL;


        while(!stream->eof())
        {
            line = stream->getLine();
            ++mWrongLineNum;

			if (!(line.length() == 0 || line.substr(0,2) == "//"))
            {
                if (pEffect == 0 && pSys == 0)
                {      
					vecparams = Ogre::StringUtil::split(line, "\t ");
					//assert (vecparams[0] == "effect");
					//assert (vecparams.size() == 2);
					if(vecparams[0] == "particle" && vecparams.size() == 2)
					{
						// No current system
						// So first valid data should be a system name
						pSys = Ogre::ParticleSystemManager::getSingleton().createTemplate(vecparams[1], mGroupName);
						pSys->_notifyOrigin(stream->getName());
						// Skip to and over next {
						skipToNextOpenBrace(stream);

						continue;
					}
					else if (vecparams[0] != "effect" || vecparams.size() != 2)
					{
                        _logErrorInfo("Bad effect system name line", line, "EffectManager::parseEffectFile");

                        return;
					}

					// ������һ���յ�effect
					pEffect = createEffectTemplate(vecparams[1]);
					
                    skipToNextOpenBrace(stream);
                }				 
                else
                {
					if(pEffect)
					{
						if (line == "}")
						{
							// �Ѹ�effect�������ڵ��ļ������뵽map��
							addToEffectTemplateScriptFileMap(pEffect->getTemplateName(), stream->getName());

							// �����ؽ�����һ��effect
							pEffect = NULL;
						}
						// һ���µ�element
						else if (line.substr(0,7) == "element")
						{
							vecparams = Ogre::StringUtil::split(line, "\t ");
							if (vecparams.size() < 2)
							{
								_logErrorInfo("Bad effect system element line", line, "EffectManager::parseEffectFile");
								return;
							}
							skipToNextOpenBrace(stream);

							// ����element�Ĳ���
							parseNewElement(vecparams[1], stream, pEffect);
						}                    
						else
						{
							// ����effect�Ĳ���������еĻ�
							// Attribute
							parseEffectAttrib(line, pEffect);
						}
					}
					else if(pSys)
					{
						// Already in a system
						if (line == "}")
						{
							// �Ѹ�Particle�������ڵ��ļ������뵽map��
							addToParticleTemplateScriptFileMap(pSys->getName(), stream->getName());
							// Finished system
							pSys = 0;
						}
						else if (line.substr(0,7) == "emitter")
						{
							// new emitter
							// Get typename
							vecparams = Ogre::StringUtil::split(line, "\t ");
							if (vecparams.size() < 2)
							{
								// Oops, bad emitter
								Ogre::LogManager::getSingleton().logMessage("Bad particle system emitter line: '"
									+ line + "' in " + pSys->getName());
								skipToNextCloseBrace(stream);

							}
							bool hasEmitter = false;
							Ogre::ParticleSystemManager::ParticleEmitterFactoryIterator iterator = 
								Ogre::ParticleSystemManager::getSingleton().getEmitterFactoryIterator();
							while(iterator.hasMoreElements())
							{
								Ogre::String EmitterName = iterator.peekNextKey();
								{
									if (EmitterName == vecparams[1])
									{
										hasEmitter = true;
										break;
									}
									iterator.getNext();
								}
							}
							if (!hasEmitter)
							{
								// Oops, emitter doesn't exists.
								Ogre::LogManager::getSingleton().logMessage("Particle system emitter doesn't exists at line: '"
									+ line + "' in " + pSys->getName());
								skipToNextCloseBrace(stream);
							}
							else
							{
								skipToNextOpenBrace(stream);
								parseNewParticleEmitter(vecparams[1], stream, pSys);
							}
						}
						else if (line.substr(0,8) == "affector")
						{
							// new affector
							// Get typename
							vecparams = Ogre::StringUtil::split(line, "\t ");
							if (vecparams.size() < 2)
							{
								// Oops, bad affector
								Ogre::LogManager::getSingleton().logMessage("Bad particle system affector line: '"
									+ line + "' in " + pSys->getName());
								skipToNextCloseBrace(stream);

							}
							bool hasFacetory = false;
							Ogre::ParticleSystemManager::ParticleAffectorFactoryIterator iterator = 
								Ogre::ParticleSystemManager::getSingleton().getAffectorFactoryIterator();
							while(iterator.hasMoreElements())
							{
								Ogre::String factoryName = iterator.peekNextKey();
								if(factoryName == vecparams[1])
								{
									hasFacetory = true;
									break;
								}
								iterator.getNext();
							}

							if (!hasFacetory)
							{
								// Oops, affector doesn't exists.
								Ogre::LogManager::getSingleton().logMessage("Particle system affector doesn't exists at line: '"
									+ line + "' in " + pSys->getName());
								skipToNextCloseBrace(stream);
							}
							else
							{
								skipToNextOpenBrace(stream);
								parseNewParticleAffector(vecparams[1], stream, pSys);
							}
						}
						else
						{
							// Attribute
							parseParticleAttrib(line, pSys);
						}
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::parseEffectAttrib(const Ogre::String& line, Effect *effect)
	{
		// ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if ( vecparams.size() != 2 || false == effect->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad effect attribute line", line, "EffectManager::parseEffectAttrib");
		}
	}	
	//---------------------------------------------------------------------
	void EffectManager::parseSkillFile( Ogre::DataStreamPtr &stream )
	{
		addToLoadedSkillScripts(stream->getName());

		String line;
		Skill *pSkill = NULL;
	//	EffectInfo *pEffectInfo = NULL;
	//	AnimationRibbon *pRibbon = NULL;

		Ogre::StringVector vecparams;

		while(!stream->eof())
		{
			line = stream->getLine();
            ++mWrongLineNum;

			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{
				if (pSkill == NULL)
				{                   
					vecparams = Ogre::StringUtil::split(line, "\t ");

                    if (vecparams[0] != "skill" || vecparams.size() != 2)
                    {
                        _logErrorInfo("Wrong skill name line", line, "EffectManager::parseSkillFile");

                        continue;
                    }
			//		assert (vecparams[0] == "skill");
			//		assert (vecparams.size() == 2);

					// ������һ���յ�effect
					pSkill = createSkillTemplate(vecparams[1]);

					skipToNextOpenBrace(stream);
				}				 
				else
				{
					if (line == "}")
					{
						//// �����ؽ�����һ��effect
						//if (pEffectInfo)
						//{
						//	pSkill->mEffectInfos.push_back(pEffectInfo);
						//	pEffectInfo = NULL;
						//}

						//if (pRibbon)
						//{
						//	pSkill->mAnimationRibbons.push_back(pRibbon);
						//	pRibbon = NULL;
						//}
						// �Ѹ�effect�������ڵ��ļ������뵽map��
						addToSkillTemplateScriptFileMap(pSkill->getSkillName(), stream->getName());

						pSkill = NULL;
					}
					else if ( line == "AnimEffect" )
					{
						// ����anim effect info
						
						skipToNextOpenBrace(stream);
						parseAnimEffectInfo(stream, pSkill);
					}
					else if ( line == "Ribbon" )
					{
						skipToNextOpenBrace(stream);
						parseAnimRibbon(stream, pSkill);
					}
					else if ( line == "Sound" )
					{
						skipToNextOpenBrace(stream);
						parseAnimSound(stream, pSkill);
					}
                    else if ( line == "SceneLight" )
                    {
                        skipToNextOpenBrace(stream);
                        parseAnimSceneLightInfo(stream, pSkill);
                    }
					else if ( line == "AnimBulletFlow" )
					{
						skipToNextOpenBrace(stream);
						parseAnimBulletFlow(stream, pSkill);
					}
					else if ( line == "CameraShake" )
					{
						skipToNextOpenBrace(stream);
						parseAnimCameraShake(stream, pSkill);
					}
					// һ���µ�element
					else
					{						
						parseSkillAttrib(line, pSkill);
					}					
				}
			}
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::parseAnimEffectInfo(Ogre::DataStreamPtr &stream, Skill *skill)
	{
		assert (skill);

		AnimationEffectInfo *effectInfo = skill->addAnimationEffectInfo();

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
	void EffectManager::parseAnimBulletFlow(Ogre::DataStreamPtr &stream,Skill* skill)
	{
		assert (skill);

		AnimationBulletFlow *bulletFlow = skill->addAnimationBulletFlow();

		assert (bulletFlow);

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
					parseAnimBulletFlowAttrib(line, bulletFlow);
				}
			}
		}

	}
	void EffectManager::parseAnimBulletFlowAttrib(const String& line,AnimationBulletFlow* animationBulletFlow)
	{
		// ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (  vecparams.size() != 2 || false == animationBulletFlow->setParameter(vecparams[0], vecparams[1]) )
		{
			_logErrorInfo("Bad Anim BulletFlow attribute line", line, "EffectManager::parseAnimBulletFlowAttrib");
		}

	}
	//---------------------------------------------------------------------
	void EffectManager::parseAnimRibbon(Ogre::DataStreamPtr &stream, Skill *skill)
	{
		assert (skill);

		AnimationRibbon *ribbon = skill->addAnimationRibbon();

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
	//---------------------------------------------------------------------
	void EffectManager::parseAnimRibbonAttrib(const Ogre::String& line, AnimationRibbon *ribbon)
	{
		// ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (  vecparams.size() != 2 || false == ribbon->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad Anim ribbon attribute line", line, "EffectManager::parseAnimRibbonAttrib");
		}
    }
	void EffectManager::parseAnimCameraShake(Ogre::DataStreamPtr &stream, Skill *skill)
	{
		assert (skill);

		AnimationCameraShake *cameraShake = skill->addAnimationCameraShake();

		assert (cameraShake);

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
					parseAnimCameraShakeAttrib(line, cameraShake);
				}
			}
		}
	}
	void EffectManager::parseAnimCameraShakeAttrib(const Ogre::String& line, AnimationCameraShake *cameraShake)
	{
		// ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (  vecparams.size() != 2 || false == cameraShake->setParameter(vecparams[0], vecparams[1]) )
		{
			_logErrorInfo("Bad Anim camer shake attribute line", line, "EffectManager::parseAnimCameraShakeAttrib");
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::parseAnimSound(Ogre::DataStreamPtr &stream, Skill *skill)
	{
		assert (skill);

		AnimationSound *sound = skill->addAnimationSound();

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
	//---------------------------------------------------------------------
	void EffectManager::parseAnimSoundAttrib(const Ogre::String& line, AnimationSound *sound)
	{
		// ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		if (  vecparams.size() != 2 || false == sound->setParameter(vecparams[0], vecparams[1]) )
		{
            _logErrorInfo("Bad Anim sound attribute line", line, "EffectManager::parseAnimSoundAttrib");
		}
	}	
    //---------------------------------------------------------------------
    void EffectManager::parseAnimSceneLightInfo(Ogre::DataStreamPtr &stream, Skill *skill)
    {
        assert (skill);

        AnimationSceneLightInfo *sceneLightInfo = skill->addAnimationSceneLightInfo();

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
    //---------------------------------------------------------------------
    void EffectManager::parseAnimSceneLightInfoAttrib(const Ogre::String& line, AnimationSceneLightInfo *sceneLightInfo)
    {
        // ����element������
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

        if (vecparams.size() != 2)
        {
            _logErrorInfo("the number of parameters must be 2!", line, "EffectManager::parseAnimSceneLightInfoAttrib");
            return;
        }

        if ( false == sceneLightInfo->setParameter(vecparams[0], vecparams[1]) )
        {
            _logErrorInfo("Bad Anim Scene Light Info attribute line", line, "EffectManager::parseAnimSceneLightInfoAttrib");
        }
    }

	//-----------------------------------------------------------------------
	void EffectManager::parseNewParticleEmitter(const String& type, Ogre::DataStreamPtr& stream, Ogre::ParticleSystem* sys)
	{
		// Create new emitter
		Ogre::ParticleEmitter* pEmit = sys->addEmitter(type);
		// Parse emitter details
		Ogre::String line;

		while(!stream->eof())
		{
			line = stream->getLine();
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
					Ogre::StringUtil::toLowerCase(line);
					parseParticleEmitterAttrib(line, pEmit);
				}
			}
		}



	}
	//-----------------------------------------------------------------------
	void EffectManager::parseNewParticleAffector(const Ogre::String& type, Ogre::DataStreamPtr& stream, Ogre::ParticleSystem* sys)
	{
		// Create new affector
		Ogre::ParticleAffector* pAff = sys->addAffector(type);
		// Parse affector details
		Ogre::String line;

		while(!stream->eof())
		{
			line = stream->getLine();
			// Ignore comments & blanks
			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{
				if (line == "}")
				{
					// Finished affector
					break;
				}
				else
				{
					// Attribute
					Ogre::StringUtil::toLowerCase(line);
					parseParticleAffectorAttrib(line, pAff);
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void EffectManager::parseParticleAttrib(const Ogre::String& line, Ogre::ParticleSystem* sys)
	{
		// Split params on space
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		// Look up first param (command setting)
		if (!sys->setParameter(vecparams[0], vecparams[1]))
		{
			// Attribute not supported by particle system, try the renderer
			Ogre::ParticleSystemRenderer* renderer = sys->getRenderer();
			if (renderer)
			{
				if (!renderer->setParameter(vecparams[0], vecparams[1]))
				{
					_logErrorInfo("Bad particle system attribute line: '"
						, line , sys->getName() + " (tried renderer)");
				}
			}
			else
			{
				// BAD command. BAD!
				_logErrorInfo("Bad particle system attribute line: '"
					, line , sys->getName() +" (no renderer)");
			}
		}
	}
	//-----------------------------------------------------------------------
	void EffectManager::parseParticleEmitterAttrib(const Ogre::String& line, Ogre::ParticleEmitter* emit)
	{
		// Split params on first space
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		// Look up first param (command setting)
		if(vecparams.size() == 1)
		{
			if (!emit->setParameter(vecparams[0], ""))
			{
				// BAD command. BAD!
				_logErrorInfo("Bad particle emitter attribute line: '"
					, line , "' for emitter " + emit->getType());
			}
		}
		else
		{

			if (!emit->setParameter(vecparams[0], vecparams[1]))
			{
				// BAD command. BAD!
				_logErrorInfo("Bad particle emitter attribute line: '"
					, line , "' for emitter " + emit->getType());
			}
		}
		
	}
	//-----------------------------------------------------------------------
	void EffectManager::parseParticleAffectorAttrib(const Ogre::String& line, Ogre::ParticleAffector* aff)
	{
		// Split params on space
		Ogre::StringVector vecparams = Ogre::StringUtil::split(line, "\t ", 1);

		// Look up first param (command setting)
		if (!aff->setParameter(vecparams[0], vecparams[1]))
		{
			// BAD command. BAD!
			_logErrorInfo("Bad particle affector attribute line: '"
				, line , "' for affector " + aff->getType());
		}
	}

	//---------------------------------------------------------------------
	Effect * EffectManager::getFromFreeMap( const Ogre::String &templateName )
	{
#if USE_EFFECT_POOL
		FreeEffectMap::iterator i = mFreeEffectMap.find(templateName);

		if ( i != mFreeEffectMap.end() )
		{
			FreeEffectList *list = i->second;
			if (list->size() > 0 )
			{
				Effect *freeEffect = *(list->begin());
				list->erase( list->begin() );

				freeEffect->reborn();

    #if OUTPUT_DEBUG_INFO

				++succeed;
			//	Ogre::LogManager::getSingleton().logMessage("getFromFreeMap succeed : " + templateName);
    #endif

				return freeEffect;
			}
			else
			{
    #if OUTPUT_DEBUG_INFO
				++fail_nolist;
			//	Ogre::LogManager::getSingleton().logMessage("getFromFreeMap failed nolist : " + Ogre::StringConverter::toString(fail_nolist));

    #endif

				return NULL;
			}
		}
		else
		{
    #if OUTPUT_DEBUG_INFO
			++fail_nomap;
		//	Ogre::LogManager::getSingleton().logMessage("getFromFreeMap failed nomap : " + Ogre::StringConverter::toString(fail_nomap));
    #endif

			return NULL;
		}
#else
        return NULL;
#endif // #if USE_EFFECT_POOL
	}
	//---------------------------------------------------------------------
	void EffectManager::addToFreeEffectMap( Effect *effect )
	{
#if USE_EFFECT_POOL
		assert (effect);

		String templateName = effect->getTemplateName();

		FreeEffectMap::iterator i = mFreeEffectMap.find(templateName);

		if ( i != mFreeEffectMap.end() )
		{
			FreeEffectList *list = i->second;

			// �����ģ���effect������effct�ص����ƣ��ͰѶ�ͷ��effect���
			if (list->size() >= mMaxNumFreeEffectPerList)
			{
				Effect *effect = *(list->begin());

				assert (effect);

				delete effect;
				effect = NULL;

				list->pop_front();

    #if OUTPUT_DEBUG_INFO
				++large;
		//		Ogre::LogManager::getSingleton().logMessage("larger than mMaxNumFreeEffectPerList : " + Ogre::StringConverter::toString(large));
    #endif
			}

			(i->second)->push_back(effect);
		}
		else
		{
			FreeEffectList *list = new FreeEffectList;
			list->push_back(effect);
			std::pair<FreeEffectMap::iterator, bool> inserted =
				mFreeEffectMap.insert( FreeEffectMap::value_type(templateName, list) );

			assert(inserted.second);

    #if OUTPUT_DEBUG_INFO
			++newMap;
		//	Ogre::LogManager::getSingleton().logMessage("new freemap : " + Ogre::StringConverter::toString(newMap));
    #endif
		}

#else    
        delete effect;
#endif // #if USE_EFFECT_POOL
	}
	//---------------------------------------------------------------------
	void EffectManager::createController(void)
	{
		Ogre::ControllerManager &controllerManager = Ogre::ControllerManager::getSingleton();
		mController = controllerManager.createFrameTimePassthroughController(
			Ogre::ControllerValueRealPtr(new EffectControllerValue(this)));
	}
	//---------------------------------------------------------------------
	void EffectManager::updateActiveEffects(float time)
	{		
        ActiveEffectList::iterator i = mActiveEffectList.begin();
        while (i != mActiveEffectList.end())
        {
            Effect *effect = *i;

            assert (effect);

            if (effect->getDelayRemove())
            {
                if ( false == effect->existVisualElements() )
                {
                    effect->removeAllElements();

                    if (effect->getRemoveParentNode())
                    {
                        effect->removeParentSceneNode();
                    }

                    // ��������գ�ֱ��ɾ��
                    if (false == effect->mPutIntoFreePool)
                    {
                        i = destroyEffect(effect);
                    }
                    else
                    {
                        addToFreeEffectMap(effect);

                        i = mActiveEffectList.erase(i);
                    }
                }
                else
                    ++i;
            }
            else
            {
                effect->updateEffect(time);
                ++i;
            }
        }
	}
	//---------------------------------------------------------------------
	size_t EffectManager::getNumEffectTemplates(void)
	{
		return mEffectTemplateMap.size();
	}
	//---------------------------------------------------------------------
	EffectManager::EffectTemplateIterator 
		EffectManager::getEffectTemplateIterator(void)
	{
		return EffectTemplateIterator(
			mEffectTemplateMap.begin(), mEffectTemplateMap.end());
	}
	//---------------------------------------------------------------------
	EffectManager::SkillIterator 
		EffectManager::getSkillIterator(void)
	{
		return SkillIterator(
			mSkillMap.begin(), mSkillMap.end());
	}
	//---------------------------------------------------------------------
	EffectManager::ElementFactoryIterator 
		EffectManager::getElementFactoryIterator(void)
	{
		return ElementFactoryIterator(
			mElementFactoryMap.begin(), mElementFactoryMap.end());
	}
	//---------------------------------------------------------------------
	void EffectManager::addToEffectTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName )
	{
		assert ( false == templateName.empty() && false == fileName.empty() );

		std::pair<EffectTemplateScriptFileMap::iterator, bool> inserted = 
			mEffectTemplateScriptFileMap.insert( EffectTemplateScriptFileMap::value_type(templateName, fileName) );

		assert (inserted.second);
	}
	//---------------------------------------------------------------------
	void EffectManager::addToSkillTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName )
	{
		assert ( false == templateName.empty() && false == fileName.empty() );

		std::pair<SkillTemplateScriptFileMap::iterator, bool> inserted = 
			mSkillTemplateScriptFileMap.insert( SkillTemplateScriptFileMap::value_type(templateName, fileName) );

		assert (inserted.second);
	}
	//---------------------------------------------------------------------
	void EffectManager::addToParticleTemplateScriptFileMap( const Ogre::String &templateName, const Ogre::String &fileName )
	{
		assert ( false == templateName.empty() && false == fileName.empty() );

		std::pair<ParticleTemplateScriptFileMap::iterator, bool> inserted = 
			mParticleTemplateScriptFileMap.insert( ParticleTemplateScriptFileMap::value_type(templateName, fileName) );

		assert (inserted.second);
	}
	//---------------------------------------------------------------------
	void EffectManager::removeParticleTemplateScriptFileMap( const Ogre::String &templateName)
	{
		ParticleTemplateScriptFileMap::iterator it = mParticleTemplateScriptFileMap.find(templateName);
		if(it != mParticleTemplateScriptFileMap.end())
			mParticleTemplateScriptFileMap.erase(it);

	}
	//---------------------------------------------------------------------
	const Ogre::String & EffectManager::getFileNameByTemplateName( const Ogre::String &templateName )
	{
		EffectTemplateScriptFileMap::iterator i = mEffectTemplateScriptFileMap.find(templateName);

		assert ( i!= mEffectTemplateScriptFileMap.end() );

		return i->second;
	}
	//---------------------------------------------------------------------
	const Ogre::String & EffectManager::getFileNameBySkillTemplateName( const Ogre::String &templateName )
	{
		SkillTemplateScriptFileMap::iterator i = mSkillTemplateScriptFileMap.find(templateName);

		assert ( i!= mSkillTemplateScriptFileMap.end() );

		return i->second;
	}
	//---------------------------------------------------------------------
	const Ogre::String & EffectManager::getFileNameByParticleTemplateName( const Ogre::String &templateName )
	{
		ParticleTemplateScriptFileMap::iterator i = mParticleTemplateScriptFileMap.find(templateName);
      
		if( i!= mParticleTemplateScriptFileMap.end() )
		    return i->second;
		else
			return Ogre::StringUtil::BLANK;
	}
	//---------------------------------------------------------------------
	void EffectManager::getTemplatesFromScriptFile( const Ogre::String &fileName, Ogre::StringVector &templates )
	{
		for ( EffectTemplateScriptFileMap::iterator i = mEffectTemplateScriptFileMap.begin();
			i != mEffectTemplateScriptFileMap.end(); ++i )
		{
			if ( i->second == fileName )
			{
				templates.push_back(i->first);
			}
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::getSkillTemplatesFromScriptFile( const Ogre::String &fileName, Ogre::StringVector &templates )
	{
		for ( SkillTemplateScriptFileMap::iterator i = mSkillTemplateScriptFileMap.begin();
			i != mSkillTemplateScriptFileMap.end(); ++i )
		{
			if ( i->second == fileName )
			{
				templates.push_back(i->first);
			}
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::getParticleTemplatesFromScriptFile( const Ogre::String &fileName, Ogre::StringVector &templates )
	{
		for ( ParticleTemplateScriptFileMap::iterator i = mParticleTemplateScriptFileMap.begin();
			i != mParticleTemplateScriptFileMap.end(); ++i )
		{
			if ( i->second == fileName )
			{
				templates.push_back(i->first);
			}
		}
	}
	//---------------------------------------------------------------------
	Skill * EffectManager::createSkill(const Ogre::String &skillName)
	{
		Skill *skillTemplate = Fairy::EffectManager::getSingleton().getSkill(skillName);

		if ( NULL == skillTemplate )
			return NULL;
		else
		{
			Skill *newSkill = new Skill(skillName, mSystem);
			*newSkill = *skillTemplate;

			return newSkill;
		}
	}
	//---------------------------------------------------------------------
	void EffectManager::removeSkill(Skill *skill)
	{
		assert (skill);
		skill->shutdown();
		//m_destroySkillCallback->onDestroySkill(skill);
        delete skill;
	}
    //---------------------------------------------------------------------
    void EffectManager::_logErrorInfo(const Ogre::String& errorInfo, const Ogre::String& lineContent, 
        const Ogre::String& functionName)
    {
       Ogre::String errorDesc;

        errorDesc += "\n------------EffectManager Error Description------------------------";
        errorDesc += "\nfile : " + mParsingFileName;
        errorDesc += "\nline : " + Ogre::StringConverter::toString(mWrongLineNum);
        errorDesc += "\nline content : " + lineContent;
        errorDesc += "\nfunction : " + functionName;
        errorDesc += "\ndescription : " + errorInfo;
        errorDesc += "\n-------------------------------------------------------------------";

        Ogre::LogManager::getSingleton().logMessage(errorDesc);
    }
    //---------------------------------------------------------------------
    void EffectManager::clearAllFreeEffects(void)
    {
#if OUTPUT_DEBUG_INFO
        Ogre::LogManager::getSingleton().logMessage("Effect free map size : " + Ogre::StringConverter::toString(mFreeEffectMap.size()));
#endif

        // ɾ��free effect map
        for ( FreeEffectMap::iterator i = mFreeEffectMap.begin();
            i != mFreeEffectMap.end();	++i	)
        {
            if (i->second)
            {
                FreeEffectList *list = i->second;

                for ( FreeEffectList::iterator i = list->begin();
                    i != list->end(); ++i )
                {
                    delete (*i);
                    (*i) = NULL;
                }
                list->clear();
            }

            delete i->second;
            i->second = NULL;
        }
        mFreeEffectMap.clear();
    }
    //---------------------------------------------------------------------
    void EffectManager::setEffectLevel(EffectLevel level)
    {
        mEffectLevel = level;

        // ���effect pool�����е���Ч����Ϊ֮ǰ������������Ч�õĶ��ǲ�ͬ�����ڵļ���ģ����Բ�������
        clearAllFreeEffects();

        // �ִ����ЧҲ�����л���
        for ( ActiveEffectList::iterator i = mActiveEffectList.begin();
            i != mActiveEffectList.end();	++i	)
        {
            if (*i)
            {
                (*i)->mPutIntoFreePool = false;
            }
        }
    }
    //---------------------------------------------------------------------
    void EffectManager::_destroyEffectTemplates(void)
    {
        // ɾ��Effect template map
        for ( EffectTemplateMap::iterator i = mEffectTemplateMap.begin();
            i != mEffectTemplateMap.end();	++i	)
        {
            if (i->second)
            {
                delete i->second;
                i->second = NULL;
            }
        }
        mEffectTemplateMap.clear();
    }

	//---------------------------------------------------------------------
    void EffectManager::_destroySkillTemplates(void)
    {
        //// ɾ��skill map
        //for ( SkillMap::iterator i = mSkillMap.begin();
        //    i != mSkillMap.end();	++i )
        //{
        //    if (i->second)
        //    {				
        //        delete i->second;
        //        i->second = NULL;
        //    }
        //}
        //mSkillMap.clear();
    }

	void EffectManager::setDestroySkillCallback( DestroySkillCallback *cback )
	{
		m_destroySkillCallback  = cback;
	}
	DestroySkillCallback* EffectManager::getDestroySkillCallback() const
	{
		return m_destroySkillCallback;
	}

	
}