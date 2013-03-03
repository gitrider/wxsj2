
#include "FairyBulletSystem.h"
#include "FairyAnimationEffectInfo.h"
#include "FairyAnimationRibbon.h"
#include "FairyAnimationSceneLightInfo.h"
#include "FairyAnimationBulletFlow.h"
#include "FairyAnimationCameraShake.h"
#include "FairyAnimationSound.h"
#include "FairyAnimationLine.h"
#include "FairyMainSceneLight.h"
#include "FairyEffectManager.h"
#include "Core/FairySystem.h"
#include "FairyEffect.h"
#include <OgreStringConverter.h>
#include <OgreSceneManager.h>
namespace Fairy
{


	//float Q_rsqrt( float number );


	OnPlaySound BulletSystem::mOnPlaySound = NULL;
	OnStopSound BulletSystem::mOnStopSound = NULL;


	BulletSystem::BulletSystem(const Ogre::String& templateName,System* system):
		m_system(system)
		,m_isCreated(false)
		,m_fOldTime(0.0f)
		,m_fDelta(0.0f)
		,m_fCumulateTime(0.0f)
		,m_vTargetPoint(Ogre::Vector3::ZERO)
		,m_fCumulateDistance(0.0f)
		,m_vRotation(Ogre::Vector3::ZERO)
		,m_bAlreadyHit(false)
		,m_bActionAgain(false)
		,m_vCasterPoint(Ogre::Vector3::ZERO)
		,m_accelerationWorld(Ogre::Vector3::ZERO)
		,m_sceneNode(NULL)
		,m_clusterIndex(0)
		,m_TargetModel(NULL)		
		,m_CasterModel(NULL)
		,m_templateName(templateName)
	{
		initParameters();
	}
	BulletSystem::~BulletSystem()
	{
		removeAllAnimationEffectInfos();
		removeAllAnimationRibbons();
		removeAllAnimationSounds();
        removeAllAnimationSceneLightInfos();
		removeAllAnimationCameraShakes();
		mBulletOperators.clear();
		if(m_sceneNode)
		{
			m_sceneNode->getCreator()->destroySceneNode(m_sceneNode);
			m_sceneNode=NULL;
		}
	}

	void BulletSystem::initParameters(void)
	{
		if (createParamDictionary("BulletSystem"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();
			
		}
	}


	const Ogre::Vector3& BulletSystem::getPosition() const
	{
		if(m_sceneNode)
			return m_sceneNode->getPosition();
		return Ogre::Vector3::ZERO;
	}
	void BulletSystem::setPosition(const Ogre::Vector3& val)
	{
		if(m_sceneNode)
			m_sceneNode->setPosition(val);
	}

	const Ogre::Quaternion& BulletSystem::getOrientation() const
	{
		if(m_sceneNode)
			return m_sceneNode->getOrientation();
		return Ogre::Quaternion::IDENTITY;
	}

	void BulletSystem::setOrientation(const Ogre::Quaternion& val)
	{
		if(m_sceneNode)
			m_sceneNode->setOrientation(val);
	}

	const Ogre::Vector3& BulletSystem::getVelocityWorld() const
	{
		return m_velocityWorld;
	}
	void BulletSystem::setVelocityWorld(const Ogre::Vector3& val)
	{
		m_velocityWorld = val;
	}
	const Ogre::Vector3& BulletSystem::getAccelerationWorld() const
	{
		return m_accelerationWorld;
	}
	void BulletSystem::setAccelerationWorld(const Ogre::Vector3& val)
	{
		m_accelerationWorld = val;
	}
	unsigned int BulletSystem::getClusterIndex() const
	{
		return m_clusterIndex;
	}
	void BulletSystem::setClusterIndex(unsigned int val)
	{
		m_clusterIndex = val;
	}

	//---------------------------------------------------------------------
	 void BulletSystem::createSceneNode(void)
	{
		// if the scene node isn't null, the effect is exist(it is recreated from free list), so
		// we don't need to create the scene node.
		if ( m_sceneNode )
			return;

		assert (m_system);
		Ogre::SceneNode *baseNode = m_system->getBaseSceneNode();
		assert (baseNode);

		m_sceneNode = baseNode->createChildSceneNode();

		assert (m_sceneNode);

		//dumpSceneNode(mBasicNode);

		// create the scene node of the element
		for ( AnimationEffectInfos::iterator i = mAnimationEffectInfos.begin();
			i != mAnimationEffectInfos.end();	++i	)
		{
			if ((*i))
			{
				Effect* pEffect = (*i)->getEffect();
				if(pEffect)
					pEffect->createSceneNode(m_sceneNode);
			}
		}

	}


	void BulletSystem::createSceneNode( Ogre::SceneNode *parentNode )
	{
		assert (parentNode);


		if (m_sceneNode)
		{
			//assert (mBasicNode->getParent() == NULL);
            // 如果已经挂在别的节点上了，就先解挂
            // 如果这个effect是从freelist中得到的，而且他上次创建的是用
            // createSceneNode（不带参数，例如LogicModel中的addEffect），
            // 当removeEffect时，并不会接触它与父节点的关系，所以这时他
            // 是有parent的，这时就先要接触它与父节点的关系
            if (m_sceneNode->getParent())
            {
                m_sceneNode->getParent()->removeChild(m_sceneNode);
            }
			parentNode->addChild(m_sceneNode);
		}
		else
		{
			m_sceneNode = parentNode->createChildSceneNode();

			for ( AnimationEffectInfos::iterator i = mAnimationEffectInfos.begin();
				i != mAnimationEffectInfos.end();	++i	)
			{
				if ((*i))
				{
					Effect* pEffect = (*i)->getEffect();
					if(pEffect)
						pEffect->createSceneNode(m_sceneNode);
				}
			}
		}

		assert (m_sceneNode);
	}
	void BulletSystem::removeSceneNode()
	{
		if ( !m_sceneNode )
			return;

		if (m_sceneNode)
		{
            if (m_sceneNode->getParent())
            {
                m_sceneNode->getParent()->removeChild(m_sceneNode);
            }
		}
	}
	void BulletSystem::setTransformInfo(const TransformInfo& info)
	{
		m_transInfo.mPosition = info.mPosition;
		m_transInfo.mRotation = info.mRotation;
	}
	const TransformInfo& BulletSystem::getTransformInfo()
	{
		return m_transInfo;
	}
	void BulletSystem::update(Real time)
	{
		if(!m_isCreated)
			return;
		float currentTime = m_fOldTime + time;
		m_fCumulateTime += time;
		m_fDelta = time;

		TransformInfo caster_info;	

		// 先从绑定点找，如果没有再从骨头名称找
		if ( m_CasterModel->getLocatorWorldTransform( m_AttachPointName, caster_info,m_OffsetPos, 
			m_OffsetRoation) )
		{
		}
		else if ( m_CasterModel->getBoneWorldTransform( m_AttachPointName, caster_info,m_OffsetPos, 
			m_OffsetRoation) )		
		{
		}

		TransformInfo target_info;		// 先从绑定点找，如果没有再从骨头名称找	
		if ( m_TargetModel->getLocatorWorldTransform( m_TargetPointName, target_info) )		
		{
		}
		else if ( m_TargetModel->getBoneWorldTransform( m_TargetPointName, target_info) )
		{
		}

		m_vTargetPoint = target_info.mPosition;
		m_vCasterPoint = caster_info.mPosition;


		_createAnimationEffect(m_fOldTime,currentTime);
		_updateAttachedAnimationEffect(m_fOldTime,currentTime);
		_createAnimationCameraShake(m_fOldTime,currentTime);
		_updateAnimationCameraShake(time);
		_createAnimationRibbon(m_fOldTime,currentTime);
		_updateAnimationRibbon(m_fOldTime,currentTime);
		_updateAnimationSound(m_fOldTime,currentTime);
		_updateAnimationLight(m_fOldTime,currentTime);
		m_fOldTime = currentTime;

	}
		//---------------------------------------------------------------------
	BulletSystem & BulletSystem::operator = (const BulletSystem &rhs)
	{
		removeAllAnimationEffectInfos();

		// Copy effect infos
		unsigned int i;
		for(i = 0; i < rhs.getNumAnimationEffectInfos(); ++i)
		{
			AnimationEffectInfo *rhsAEI = rhs.getAnimationEffectInfo(i);
			AnimationEffectInfo *newAEI = addAnimationEffectInfo();

			assert ( rhsAEI && newAEI );

		//	rhsAEI->copyParametersTo(newAEI);
            rhsAEI->copyParameters(*newAEI);
		}

		removeAllAnimationRibbons();

		// copy ribbons
		for(i = 0; i < rhs.getNumAnimationRibbons(); ++i)
		{
			AnimationRibbon *rhsAEI = rhs.getAnimationRibbon(i);
			AnimationRibbon *newAEI = addAnimationRibbon();

			assert ( rhsAEI && newAEI );

            rhsAEI->copyParameters(*newAEI);
		}

		removeAllAnimationSounds();

		// copy sounds
		for(i = 0; i < rhs.getNumAnimationSounds(); ++i)
		{
			AnimationSound *rhsAEI = rhs.getAnimationSound(i);
			AnimationSound *newAEI = addAnimationSound();

			assert ( rhsAEI && newAEI );

            rhsAEI->copyParameters(*newAEI);
		}

        removeAllAnimationSceneLightInfos();

        // Copy effect infos
        for(i = 0; i < rhs.getNumAnimationSceneLightInfos(); ++i)
        {
            AnimationSceneLightInfo *rhsAEI = rhs.getAnimationSceneLightInfo(i);
            AnimationSceneLightInfo *newAEI = addAnimationSceneLightInfo();

            assert ( rhsAEI && newAEI );

            rhsAEI->copyParameters(*newAEI);
        }
        rhs.copyParameters(this);

		removeAllAnimationCameraShakes();

		// Copy effect infos
		for(i = 0; i < rhs.getNumAnimationCameraShake(); ++i)
		{
			AnimationCameraShake *rhsAEI = rhs.getAnimationCameraShake(i);
			AnimationCameraShake *newAEI = addAnimationCameraShake();

			assert ( rhsAEI && newAEI );

			rhsAEI->copyParameters(*newAEI);
		}
		rhs.copyParameters(this);

		return *this;
	}
	//---------------------------------------------------------------------
	AnimationEffectInfo* BulletSystem::addAnimationEffectInfo(void)
	{
		AnimationEffectInfo *effectInfo = new AnimationEffectInfo();
		assert (effectInfo);

		mAnimationEffectInfos.push_back(effectInfo);

		return effectInfo;
	}
	//---------------------------------------------------------------------
	AnimationEffectInfo* BulletSystem::getAnimationEffectInfo(unsigned short index) const
	{
		assert(index < mAnimationEffectInfos.size() && "Animation Effect Info index out of bounds!");
		return mAnimationEffectInfos[index];
	}
	//---------------------------------------------------------------------
	unsigned short BulletSystem::getNumAnimationEffectInfos(void) const
	{
		return static_cast<unsigned short>( mAnimationEffectInfos.size() );
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAnimationEffectInfo(unsigned short index)
	{
		assert(index < mAnimationEffectInfos.size() && "Animation Effect Info index out of bounds!");
		AnimationEffectInfos::iterator ai = mAnimationEffectInfos.begin() + index;

		if (*ai)
		{
			delete *ai;
			*ai = NULL;
		}

		mAnimationEffectInfos.erase(ai);
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAllAnimationEffectInfos(void)
	{
		/*for (unsigned short i=0; i<getNumAnimationEffectInfos(); ++i )
		{
			removeAnimationEffectInfo(i);
		}
		mAnimationEffectInfos.clear();*/
		for ( AnimationEffectInfos::iterator i = mAnimationEffectInfos.begin(); i != mAnimationEffectInfos.end(); ++i )
		{

			if (*i)
			{
				delete *i;
			}
		}
		mAnimationEffectInfos.clear();
	}
	//---------------------------------------------------------------------
	AnimationRibbon* BulletSystem::addAnimationRibbon(void)
	{
		AnimationRibbon *ribbon = new AnimationRibbon(m_system);
		assert (ribbon);

		mAnimationRibbons.push_back(ribbon);

		return ribbon;
	}
	//---------------------------------------------------------------------
	AnimationRibbon* BulletSystem::getAnimationRibbon(unsigned short index) const
	{
		assert(index < mAnimationRibbons.size() && "Animation ribbon index out of bounds!");
		return mAnimationRibbons[index];
	}
	//---------------------------------------------------------------------
	unsigned short BulletSystem::getNumAnimationRibbons(void) const
	{
		return static_cast<unsigned short>( mAnimationRibbons.size() );
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAnimationRibbon(unsigned short index)
	{
		assert(index < mAnimationRibbons.size() && "Animation ribbon index out of bounds!");
		AnimationRibbons::iterator ai = mAnimationRibbons.begin() + index;

		if (*ai)
		{
			delete *ai;
			*ai = NULL;
		}

		mAnimationRibbons.erase(ai);
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAllAnimationRibbons(void)
	{
		/*for (unsigned short i=0; i<getNumAnimationRibbons(); ++i )
		{
			removeAnimationRibbon(i);
		}*/
		for ( AnimationRibbons::iterator i = mAnimationRibbons.begin(); i != mAnimationRibbons.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mAnimationRibbons.clear();
	}
	//---------------------------------------------------------------------
	AnimationCameraShake* BulletSystem::addAnimationCameraShake(void)
	{
		AnimationCameraShake *cameraShake = new AnimationCameraShake(m_system);
		assert (cameraShake);

		mAnimationCameraShakes.push_back(cameraShake);

		return cameraShake;
	}
	//---------------------------------------------------------------------
	AnimationCameraShake* BulletSystem::getAnimationCameraShake(unsigned short index) const
	{
		assert(index < mAnimationCameraShakes.size() && "Animation camera shakes index out of bounds!");
		return mAnimationCameraShakes[index];
	}
	//---------------------------------------------------------------------
	unsigned short BulletSystem::getNumAnimationCameraShake(void) const
	{
		return static_cast<unsigned short>( mAnimationCameraShakes.size() );
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAnimationCameraShake(unsigned short index)
	{
		assert(index < mAnimationCameraShakes.size() && "Animation camera shakes index out of bounds!");
		AnimationCameraShakes::iterator bi = mAnimationCameraShakes.begin() + index;

		if (*bi)
		{
			delete *bi;
			*bi = NULL;
		}

		mAnimationCameraShakes.erase(bi);
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAllAnimationCameraShakes(void)
	{
		for ( AnimationCameraShakes::iterator i = mAnimationCameraShakes.begin(); i != mAnimationCameraShakes.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mAnimationCameraShakes.clear();
	}
	//---------------------------------------------------------------------
	AnimationSound* BulletSystem::addAnimationSound(void)
	{
		AnimationSound *sound = new AnimationSound();
		assert (sound);

		mAnimationSounds.push_back(sound);

		return sound;
	}
	//---------------------------------------------------------------------
	AnimationSound* BulletSystem::getAnimationSound(unsigned short index) const
	{
		assert(index < mAnimationSounds.size() && "Animation sound index out of bounds!");
		return mAnimationSounds[index];
	}
	//---------------------------------------------------------------------
	unsigned short BulletSystem::getNumAnimationSounds(void) const
	{
		return static_cast<unsigned short>( mAnimationSounds.size() );
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAnimationSound(unsigned short index)
	{
		assert(index < mAnimationSounds.size() && "Animation sound index out of bounds!");
		AnimationSounds::iterator ai = mAnimationSounds.begin() + index;

		if (*ai)
		{
			delete *ai;
			*ai = NULL;
		}

		mAnimationSounds.erase(ai);
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAllAnimationSounds(void)
	{		
		for ( AnimationSounds::iterator i = mAnimationSounds.begin(); i != mAnimationSounds.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mAnimationSounds.clear();
	}
    //---------------------------------------------------------------------
    AnimationSceneLightInfo* BulletSystem::addAnimationSceneLightInfo(void)
    {
        AnimationSceneLightInfo *effectInfo = new AnimationSceneLightInfo();
        assert (effectInfo);

        mAnimationSceneLightInfos.push_back(effectInfo);

        return effectInfo;
    }
    //---------------------------------------------------------------------
    AnimationSceneLightInfo* BulletSystem::getAnimationSceneLightInfo(unsigned short index) const
    {
        assert(index < mAnimationSceneLightInfos.size() && "Animation scene light Info index out of bounds!");
        return mAnimationSceneLightInfos[index];
    }
    //---------------------------------------------------------------------
    unsigned short BulletSystem::getNumAnimationSceneLightInfos(void) const
    {
        return static_cast<unsigned short>( mAnimationSceneLightInfos.size() );
    }
    //---------------------------------------------------------------------
    void BulletSystem::removeAnimationSceneLightInfo(unsigned short index)
    {
        assert(index < mAnimationSceneLightInfos.size() && "Animation scene light Info index out of bounds!");
        AnimationSceneLightInfos::iterator ai = mAnimationSceneLightInfos.begin() + index;

        if (*ai)
        {
            delete *ai;
            *ai = NULL;
        }

        mAnimationSceneLightInfos.erase(ai);
    }
    //---------------------------------------------------------------------
    void BulletSystem::removeAllAnimationSceneLightInfos(void)
    {
        for ( AnimationSceneLightInfos::iterator i = mAnimationSceneLightInfos.begin(); i != mAnimationSceneLightInfos.end(); ++i )
        {
            if (*i)
            {
                delete *i;
                *i = NULL;
            }
        }
        mAnimationSceneLightInfos.clear();
    }

	//---------------------------------------------------------------------
	AnimationLine* BulletSystem::addAnimationLine(void)
	{
		AnimationLine* line = new AnimationLine(m_system);
		assert (line);

		mAnimationLines.push_back(line);

		return line;
	}
	//---------------------------------------------------------------------
	AnimationLine* BulletSystem::getAnimationLine(unsigned short index) const
	{
		assert(index < mAnimationLines.size() && "Animation line Info index out of bounds!");
		return mAnimationLines[index];
	}
	//---------------------------------------------------------------------
	unsigned short BulletSystem::getNumAnimationLines(void) const
	{
		return static_cast<unsigned short>( mAnimationLines.size() );
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAnimationLine(unsigned short index)
	{
		assert(index < mAnimationLines.size() && "Animation line index out of bounds!");
		AnimationLines::iterator ai = mAnimationLines.begin() + index;

		if (*ai)
		{
			delete *ai;
			*ai = NULL;
		}

		mAnimationLines.erase(ai);
	}
	//---------------------------------------------------------------------
	void BulletSystem::removeAllAnimationLines(void)
	{
		for ( AnimationLines::iterator i = mAnimationLines.begin(); i != mAnimationLines.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mAnimationLines.clear();
	}


	void BulletSystem::addBulletOperator(BulletOperator* pOperator)
	{
		mBulletOperators.push_back(pOperator);
	}
	std::vector<BulletOperator*>& BulletSystem::getBulletOperators()
	{
		return mBulletOperators;
	}
	bool BulletSystem::findBulletOperator(BulletOperator* pOperator)
	{
		bool hasOperator = false;
		BulletOperators::iterator it = mBulletOperators.begin();
		while(it != mBulletOperators.end())
		{
			if(pOperator == *it)
			{
				hasOperator = true;
				break;
			}
			it++;
		}
		return hasOperator;
	}
	void BulletSystem::copyParameters(BulletSystem* newInfo) const
    {
        newInfo->m_templateName = m_templateName;
    }

	void BulletSystem::_createAnimationEffect(Ogre::Real oldTime,Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < getNumAnimationEffectInfos(); ++i )
		{
			AnimationEffectInfo *effectInfo = getAnimationEffectInfo(i);
			assert (effectInfo);

			Ogre::Real time = effectInfo->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				Effect *effect = effectInfo->getEffect();

                // 获取脚本中的特效名称
                const Ogre::String& effectTemplateName = effectInfo->getEffectTemplateName();

               Ogre::String realEffectTemplate;

                realEffectTemplate = effectTemplateName;


                //////////////////////////////////////////////////////////////////////////
               Ogre::String attachPoint = effectInfo->getAttachPoint();

                TransformInfo info;
				info = m_transInfo;

				if (effect)
				{
					// 先把旧的删除，在创建新的
					EffectManager::getSingleton().removeEffect(effect, false, false);

					effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
					effect->createSceneNode();
					effectInfo->setEffect(effect);
				}
				else
				{
					effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
					effect->createSceneNode();
					effectInfo->setEffect(effect);
				}

				if ( false == effectInfo->getAttach() )
				{
					// 根据需要调整高度数据
					/*if (mTerrainHeight < Ogre::Math::POS_INFINITY)
					{
						info.mPosition.y = mTerrainHeight;
					}*/

					// 给effect传入点
					effect->setTransformInfo(info);
				}
					
			}
		}
	}

	void BulletSystem::_updateAttachedAnimationEffect(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < getNumAnimationEffectInfos(); ++i )
		{
			AnimationEffectInfo *effectInfo = getAnimationEffectInfo(i);
			assert (effectInfo);
			TransformInfo info;
			info = m_transInfo;

			Effect *effect = effectInfo->getEffect();

			if (effect && effectInfo->getAttach())
			{
                // 获取脚本中的特效名称
                const Ogre::String& effectTemplateName = effectInfo->getEffectTemplateName();
		

				// 给effect传入点
				effect->setTransformInfo(info);
			}
		}
	}

	void BulletSystem::_createAnimationRibbon(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < getNumAnimationRibbons(); ++i )
		{
			AnimationRibbon *ribbon = getAnimationRibbon(i);
			assert (ribbon);

			Ogre::Real time = ribbon->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				ribbon->createRenderInstance();
			}
		}
	}
		//-----------------------------------------------------------------------
	void BulletSystem::_updateAnimationRibbon(Ogre::Real oldTime, Ogre::Real currTime)
	{
		Ogre::Real delta = currTime - oldTime;
		if(delta < 0.001)
			return;
		for ( Ogre::ushort i = 0; i < getNumAnimationRibbons(); ++i )
		{
			AnimationRibbon *ribbon = getAnimationRibbon(i);
			assert (ribbon);

			String attachPoint = ribbon->getAttachPoint();

			TransformInfo info;

			// 给effect传入点
			ribbon->updateRenderInstance(delta, m_transInfo);
		}
	}
	void BulletSystem::_createAnimationCameraShake(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < getNumAnimationCameraShake(); ++i )
		{
			AnimationCameraShake *cameraShake = getAnimationCameraShake(i);
			assert (cameraShake);

			Ogre::Real time = cameraShake->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				cameraShake->createInstance();
			}
		}

	}
	void BulletSystem::_updateAnimationCameraShake(Ogre::Real delta)
	{
		for ( Ogre::ushort i = 0; i < getNumAnimationCameraShake(); ++i )
		{
			AnimationCameraShake *cameraShake = getAnimationCameraShake(i);
			assert (cameraShake);

			cameraShake->updateInstance(delta);

		}
	}
	//-----------------------------------------------------------------------
	void BulletSystem::_updateAnimationSound(Ogre::Real oldTime, Ogre::Real currTime)
	{
		if (mOnPlaySound)
		{
		// sound
			for ( Ogre::ushort i = 0; i < getNumAnimationSounds(); ++i )
			{					
				AnimationSound *sound = getAnimationSound(i);
				assert (sound);

				if (false == sound->mPlayed)
				{
					Ogre::Real time = sound->getAttachTime();

					if((currTime >= oldTime && time >= oldTime && time < currTime) || 
						(currTime < oldTime && (time >= oldTime || time < currTime)))
					{
						float pos[3];
						pos[0] = m_transInfo.mPosition.x;
						pos[1] = m_transInfo.mPosition.y;
						pos[2] = m_transInfo.mPosition.z;
						sound->mSoundHandle = mOnPlaySound(sound->getSoundName().c_str(),pos, false);

						sound->mPlayed = true;
					}
				}	
			}
		}

	}
	//-----------------------------------------------------------------------
    void BulletSystem::_updateAnimationLight(Ogre::Real oldTime, Ogre::Real currTime)
    {
        MainSceneLight* light = EffectManager::getSingleton().getMainSceneLight();

        assert (light);

        unsigned short lightInfoNum = getNumAnimationSceneLightInfos();

        for ( Ogre::ushort i = 0; i < lightInfoNum; ++i )
        {
            AnimationSceneLightInfo *lightInfo = getAnimationSceneLightInfo(i);
            assert (lightInfo);

            Ogre::Real time = lightInfo->getAttachTime();

            if((currTime >= oldTime && time >= oldTime && time < currTime) || 
                (currTime < oldTime && (time >= oldTime || time < currTime)))
            {
                LightModifyPhase modifyPhase = light->getLightModifyPhase();

                if (modifyPhase == LMP_IDLE)
                {
                    light->setFadeTime( lightInfo->getFadeInTime(), lightInfo->getFadeOutTime() );

                    light->setLastTime(lightInfo->getLastTime());

                    light->setColourValue(lightInfo->getDestColour());
                }
                
                if (modifyPhase == LMP_LAST)
                    light->addLastTime(lightInfo->getLastTime());
            }
        }
    }
	void BulletSystem::_updateAnimationLine(Ogre::Real oldTime,Ogre::Real currTime)	{
		Ogre::Real delta = currTime - oldTime;
		if(delta < 0.001)
			return;		
		for ( Ogre::ushort i = 0; i < getNumAnimationLines(); ++i )
		{			
			AnimationLine *line = getAnimationLine(i);
			assert (line);			
			String attachPoint = line->getAttachPoint();			
			TransformInfo info;		
			// 给effect传入点
			line->updateRenderInstance(delta, m_transInfo);		
		}	
	}	

	void BulletSystem::shutdown(void)
	{
		for ( AnimationEffectInfos::iterator i = mAnimationEffectInfos.begin(); i != mAnimationEffectInfos.end(); ++i )
		{
			AnimationEffectInfo* info = *i;
			Effect* effect = info->getEffect();

			if (effect)
			{
				effect->shutdown();
			}
		}
	}
	const Real BulletSystem::getHeightToTerrain()
	{
		Real fHeight = 0.0f;
		Fairy::TerrainData* pTerrainData = m_system->getTerrainData();
		if(pTerrainData)
		{
			fHeight = m_transInfo.mPosition.y - pTerrainData->getHeightAt(m_transInfo.mPosition.x,m_transInfo.mPosition.z);
		}
		return fHeight;
	}

	void BulletSystem::Initial(LogicModel* targetModel,LogicModel* casterModel,const Ogre::String attachPoint,
		const Ogre::String targetPoint,const Ogre::Vector3	ffsetPos,const Ogre::Quaternion	offsetRotation)	
	{
		if(m_isCreated)
			return;

		createSceneNode();
		TransformInfo caster_info;

		// 先从绑定点找，如果没有再从骨头名称找
		if ( casterModel->getLocatorWorldTransform( attachPoint, caster_info,ffsetPos, 
			offsetRotation) )
		{
		}
		else if ( casterModel->getBoneWorldTransform( attachPoint, caster_info,ffsetPos, 
			offsetRotation) )		
		{
		}

		TransformInfo target_info;		// 先从绑定点找，如果没有再从骨头名称找	
		if ( targetModel->getLocatorWorldTransform( targetPoint, target_info) )		
		{
		}
		else if ( targetModel->getBoneWorldTransform( targetPoint, target_info) )
		{
		}

		setTransformInfo(caster_info);

		m_sceneNode->setPosition(caster_info.mPosition);
		m_sceneNode->setOrientation(caster_info.mRotation);

		m_vTargetPoint	= target_info.mPosition;
		m_vCurrentPoint = caster_info.mPosition;
		m_vCasterPoint = caster_info.mPosition;

		m_TargetModel = targetModel;
		m_TargetPointName = targetPoint;
		m_CasterModel = casterModel;
		m_AttachPointName = attachPoint;	
		m_OffsetPos = ffsetPos;		
		m_OffsetRoation = offsetRotation;		
		m_bAlreadyHit		= false;
		m_fCumulateTime		= 0.0f;
		m_fCumulateDistance = 0.0f;
		m_fDelta			= 0.0f;

		m_isCreated = true;
	}


	Real BulletSystem::getArriveDistance() const
	{
		return m_vTargetPoint.distance(m_transInfo.mPosition);
	}


}