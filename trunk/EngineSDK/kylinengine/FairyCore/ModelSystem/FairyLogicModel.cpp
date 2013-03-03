/********************************************************************
filename:   WXLogicModel.cpp

purpose:    LogicModel���������滻ԭ��DObject�ģ�����Ҫ��������Ⱦ����
�ͻ��˵�һ���Ӻϲ㣬������Ⱦ�㣬ʵ�ֿͻ��˶Գ�����
��̬�����һЩ�����绻װ������Ч�ȣ��������ǿͻ��˿��Ժ�
�����ʵ�ָ���Ч���������������Ⱦ��Ĵ��롣
*********************************************************************/

#include "FairyLogicModel.h"

#include "Core/FairySystem.h"
#include "Core/FairyUtils.h"
#include "Core/TerrainTypeInfos.h"
#include "Core/FairyResources.h"
#include "Core/FairyFlags.h"

#include "EffectSystem/FairyEffectManager.h"
#include "EffectSystem/FairySkill.h"
#include "EffectSystem/FairyAnimationRibbon.h"
#include "EffectSystem/FairyAnimationEffectInfo.h"
#include "EffectSystem/FairyAnimationSound.h"
#include "EffectSystem/FairyProjectorElement.h"
#include "EffectSystem/FairyAnimationSceneLightInfo.h"
#include "EffectSystem/FairyMainSceneLight.h"
#include "EffectSystem/FairyBulletFlowSystemManager.h"
#include "EffectSystem/FairyBulletFlowSystem.h"
#include "EffectSystem/FairyAnimationBulletFlow.h"
#include <OgreLogManager.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <OgreSkeletonInstance.h>
#include <OgreBone.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreMaterialManager.h>
#include <OgreCamera.h>
#include <OgreAxisAlignedBox.h>
#include <OgreGpuProgramManager.h>
#include <OgreAnimation.h>
#include <OgreSkeletonManager.h>
#include <OgreStringVector.h>
#include <ScorpioFabric.h>
#include <ScorpioCollision.h>

/// ���define EDITOR����execute�оͻ�ִ�б༭������Ҫ����Ϊ�����ݻ�����������λ�õȣ�
#define EDITOR
/*#undef  EDITOR*/

namespace Fairy	{

    OnPlaySound LogicModel::mOnPlaySound = NULL;
	OnStopSound LogicModel::mOnStopSound = NULL;
    GetHeightInWorld LogicModel::mGetHeightInWorld = NULL;

	String LogicModel::MESH_HINT = "Mesh";
	String LogicModel::MATERIAL_HINT = "Material";
	String LogicModel::SLMT_CREATE_HINT = "SlotCreate";

	String LogicModel::MOVEMENT_TYPE_ATTRIBUTE = "MovementType";
   Ogre::String LogicModel::DEFAULT_ANIM_NAME = "DefaultAnimation";
   Ogre::String LogicModel::BOUNDING_BOX = "BoundingBox";
   Ogre::String LogicModel::SCALE_FACTOR = "Scale";

   Ogre::String LogicModel::HAIR_MESH_NAME = "HairMesh";

    //////////////////////////////////////////////////////////////////////////
    //-----------------------------------------------------------------------
    const Ogre::ColourValue& _getColourFromMaterial(const Ogre::MaterialPtr& mat)
    {
        assert (false == mat.isNull());

        const Ogre::ColourValue realZero(0.0,0.0,0.0,1.0);

        for (unsigned short techIt = 0; techIt < mat->getNumTechniques(); ++techIt)
        {
            Ogre::Technique* technique = mat->getTechnique(techIt);

            for (unsigned short passIt = 0; passIt < technique->getNumPasses(); ++passIt)
            {
                Ogre::Pass* pass = technique->getPass(passIt);

                const Ogre::ColourValue& passColour = pass->getSelfIllumination();

                if (passColour != realZero &&
                    passColour != Ogre::ColourValue::ZERO)
                {
                    // ���ص�һ����Ϊ0���Է�����ɫ
                    return passColour;
                }
            }
        }

        // Ĭ�Ϸ��ص��Ǻ�ɫ
        return Ogre::ColourValue::Black;
    }

    LogicModel::ModelMaterialInstance::ModelMaterialInstance(const Ogre::MaterialPtr& origin) :
    mOrigin(), mDerived()
    {
        // Generate material name
        Ogre::String newName;

        // Avoid name collision
        do {
            newName = origin->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
        } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

        Ogre::AliasTextureNamePairList nameList;
        bool depthWrite = false;
        bool setName = false;
        bool alphaTest = false;

        bool colourOpAdd = false;

        Ogre::Material::TechniqueIterator dti = origin->getTechniqueIterator();
        while (dti.hasMoreElements())
        {
            Ogre::Technique* dt = dti.getNext();
            Ogre::Technique::PassIterator dpi = dt->getPassIterator();
            while (dpi.hasMoreElements())
            {
                Ogre::Pass* dp = dpi.getNext();

                if (dp->getDepthWriteEnabled())
                    depthWrite = true;

                if (dp->getAlphaRejectFunction() != Ogre::CMPF_ALWAYS_PASS)
                    alphaTest = true;

                Ogre::Pass::TextureUnitStateIterator dti = dp->getTextureUnitStateIterator();

                while (dti.hasMoreElements())
                {
                    Ogre::TextureUnitState* dt = dti.getNext();

                    if (dt->getTextureType() == Ogre::TEX_TYPE_2D && !setName)
                    {                        
                        nameList.insert(Ogre::AliasTextureNamePairList::value_type("<baseTexture>", dt->getTextureName()));

                        setName = true;
                    }

                    // �ж�ԭ�����Ƿ��õ���add���ӷ���
                    if (dt->getColourBlendMode().operation == Ogre::LBX_ADD)
                        colourOpAdd = true;
                }                
            }
        }

        mDerived = Ogre::MaterialManager::getSingleton().create(newName, origin->getGroup());

        // ��cloneһ��originMat����ΪmOrigin��������entity�е�ԭ���ʾ���Զ���ᱻ�Ķ�
        do {
            newName = origin->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
        } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

        mOrigin = origin->clone(newName);

        // Clone material
        Ogre::MaterialPtr baseTransparencyMat;

        if (depthWrite && alphaTest)
            baseTransparencyMat = Ogre::MaterialManager::getSingleton().getByName("Transparent/AlphaTemplate");
        else
            baseTransparencyMat = Ogre::MaterialManager::getSingleton().getByName("Transparent/NoAlphaTemplate");

        if (baseTransparencyMat.isNull())
        {
            Ogre::LogManager::getSingleton().logMessage("can't find the transparent material! "
                "LogicModel::ModelMaterialInstance::ModelMaterialInstance");

            baseTransparencyMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");

            if (baseTransparencyMat.isNull())
            {
                // ���ҵ�effect������
                OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                    "can't find the BaseWhite material!",
                    "LogicModel::ModelMaterialInstance::ModelMaterialInstance");
            }
        }

        baseTransparencyMat->copyDetailsTo(mDerived);

        mDerived->applyTextureAliases(nameList);

        // ����Transparent/AlphaTemplate�����п��ܻ�disable lighting����������Ӧ�ð�lighting��
        mDerived->setLightingEnabled(true);

        mDerived->setSelfIllumination( _getColourFromMaterial(origin) );

        // ͷ�����ʵ���������
        if (colourOpAdd)
        {
            mDerived->getTechnique(0)->getPass(1)->getTextureUnitState(0)->setColourOperation(Ogre::LBO_ADD);
            mDerived->setDiffuse(Ogre::ColourValue::Black);
            mDerived->setAmbient(Ogre::ColourValue::Black);
        }

        mDerived->compile();
    }
    //-----------------------------------------------------------------------
    LogicModel::ModelMaterialInstance::~ModelMaterialInstance()
    {
        if (!mDerived.isNull())
        {
            Ogre::MaterialManager *mm = Ogre::MaterialManager::getSingletonPtr();
            if (mm)
            {
                mm->remove(mDerived->getHandle());
            }
            mDerived.setNull();
        }

        if (!mOrigin.isNull())
        {
            Ogre::MaterialManager *mm = Ogre::MaterialManager::getSingletonPtr();
            if (mm)
            {
                mm->remove(mOrigin->getHandle());
            }
            mOrigin.setNull();
        }
    }
    //////////////////////////////////////////////////////////////////////////

    size_t LogicModel::EntityTransparentor::addMaterial(const Ogre::MaterialPtr& material)
    {
        size_t index = 0;
        MaterialInstanceList::const_iterator it, itend;
        itend = mMaterialInstances.end();
        for (it = mMaterialInstances.begin(); it != itend; ++it, ++index)
        {
            ModelMaterialInstance* materialInstance = *it;
            if (materialInstance->mOrigin->getName() == material->getName())
                return index;
        }

        mMaterialInstances.push_back(new ModelMaterialInstance(material));
        return index;
    }
    //-----------------------------------------------------------------------
    LogicModel::EntityTransparentor::EntityTransparentor(void)
        : mEntity(NULL)
    {
    }
    //-----------------------------------------------------------------------
    LogicModel::EntityTransparentor::EntityTransparentor(Ogre::Entity* entity)
        : mEntity(NULL)
    {
        init(entity);
    }
    //-----------------------------------------------------------------------
    LogicModel::EntityTransparentor::~EntityTransparentor()
    {
        clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::EntityTransparentor::clear(void)
    {
        MaterialInstanceList::const_iterator it, itend;
        itend = mMaterialInstances.end();
        for (it = mMaterialInstances.begin(); it != itend; ++it)
        {
            ModelMaterialInstance* materialInstance = *it;
            delete materialInstance;
        }
        mMaterialInstances.clear();
        mSubEntityMaterialIndices.clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::EntityTransparentor::init(Ogre::Entity* entity)
    {
        clear();

        mEntity = entity;

        size_t numSubEntities = mEntity->getNumSubEntities();
        mSubEntityMaterialIndices.resize(numSubEntities);
        for (Ogre::uint i = 0; i < numSubEntities; ++i)
        {
            Ogre::SubEntity* subEntity = mEntity->getSubEntity(i);
            const Ogre::MaterialPtr& material = subEntity->getMaterial();
            size_t index = addMaterial(material);
            mSubEntityMaterialIndices[i] = index;
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::EntityTransparentor::apply(Ogre::Real transparency)
    {
        assert(mEntity);

        size_t numSubEntities = mEntity->getNumSubEntities();
        if (transparency >= 1)
        {
            // Complete transparency, just as invisible
            mEntity->setVisible(false);
        }
        else if (transparency <= 0)
        {
            // Complete opaque, just as origin
            mEntity->setVisible(true);
            for (Ogre::uint i = 0; i < numSubEntities; ++i)
            {
                Ogre::SubEntity* subEntity = mEntity->getSubEntity(i);
                subEntity->setMaterialName(mMaterialInstances[mSubEntityMaterialIndices[i]]->mOrigin->getName());
            }
        }
        else
        {
            // Derive transparency material first
            MaterialInstanceList::const_iterator it, itend;
            itend = mMaterialInstances.end();
            for (it = mMaterialInstances.begin(); it != itend; ++it)
            {
                ModelMaterialInstance* materialInstance = *it;
                applyTransparency(materialInstance->mOrigin, materialInstance->mDerived, transparency);
            }

            // Applies derived transparency material to sub-entities.
            mEntity->setVisible(true);
            for (Ogre::uint i = 0; i < numSubEntities; ++i)
            {
                Ogre::SubEntity* subEntity = mEntity->getSubEntity(i);
                subEntity->setMaterialName(mMaterialInstances[mSubEntityMaterialIndices[i]]->mDerived->getName());
            }
        }
    }
    //-----------------------------------------------------------------------
    bool LogicModel::EntityTransparentor::applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Ogre::Real transparency)
    {
        using namespace Ogre;

        assert(0 <= transparency && transparency <= 1);

        Ogre::Real opacity = 1 - transparency;
        bool applied = false;

        Material::TechniqueIterator dti = derived->getTechniqueIterator();
        while (dti.hasMoreElements())
        {
            Technique* dt = dti.getNext();
            Technique::PassIterator dpi = dt->getPassIterator();
            while (dpi.hasMoreElements())
            {
                Pass* dp = dpi.getNext();
                SceneBlendFactor bfSrc = dp->getSourceBlendFactor();
                SceneBlendFactor bfDst = dp->getDestBlendFactor();
                if (bfSrc == SBF_SOURCE_ALPHA && bfDst == SBF_ONE_MINUS_SOURCE_ALPHA)
                {
                    applied = true;
                    
                    const ColourValue& diffuse = dp->getDiffuse();
                    dp->setDiffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
                    switch (dp->getAlphaRejectFunction())
                    {
                    case CMPF_ALWAYS_FAIL:
                    case CMPF_ALWAYS_PASS:
                        break;
                    case CMPF_LESS:
                    case CMPF_LESS_EQUAL:
                    case CMPF_EQUAL:
                    case CMPF_NOT_EQUAL:
                    case CMPF_GREATER_EQUAL:
                    case CMPF_GREATER:
                        dp->setAlphaRejectValue(static_cast<uchar>(128 * opacity));
                        break;
                    }
                }                
            }
        }

        if (applied)
        {
            // Must re-compile
            derived->compile();
        }

        return applied;
    }
    //////////////////////////////////////////////////////////////////////////
    //-----------------------------------------------------------------------
    LogicModel::FabricTransparentor::FabricTransparentor(Ogre::Fabric* fabric)
        : mFabric(fabric)
        , mMaterialInstance(NULL)
    {
        init();
    }
    //-----------------------------------------------------------------------
    LogicModel::FabricTransparentor::~FabricTransparentor()
    {
        clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::FabricTransparentor::clear(void)
    {
        delete mMaterialInstance;
    }
    //-----------------------------------------------------------------------
    void LogicModel::FabricTransparentor::init(void)
    {
        clear();

        mMaterialInstance = new ModelMaterialInstance(Ogre::MaterialManager::getSingleton().getByName(mFabric->getMaterialName()));
    }
    //-----------------------------------------------------------------------
    void LogicModel::FabricTransparentor::apply(Ogre::Real transparency)
    {
        assert(mFabric);

        if (transparency >= 1)
        {
            // Complete transparency, just as invisible
            mFabric->setVisible(false);
        }
        else if (transparency <= 0)
        {
            // Complete opaque, just as origin
            mFabric->setVisible(true);
            mFabric->setMaterialName(mMaterialInstance->mOrigin->getName());
        }
        else
        {
            applyTransparency(mMaterialInstance->mOrigin, mMaterialInstance->mDerived, transparency);

            // Applies derived transparency material to sub-entities.
            mFabric->setVisible(true);
            mFabric->setMaterialName(mMaterialInstance->mDerived->getName());
        }
    }
    //-----------------------------------------------------------------------
    bool LogicModel::FabricTransparentor::applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Ogre::Real transparency)
    {
        using namespace Ogre;

        assert(0 <= transparency && transparency <= 1);

        Ogre::Real opacity = 1 - transparency;
        bool applied = false;

        Material::TechniqueIterator dti = derived->getTechniqueIterator();
        while (dti.hasMoreElements())
        {
            Technique* dt = dti.getNext();
            Technique::PassIterator dpi = dt->getPassIterator();
            while (dpi.hasMoreElements())
            {
                Pass* dp = dpi.getNext();
                SceneBlendFactor bfSrc = dp->getSourceBlendFactor();
                SceneBlendFactor bfDst = dp->getDestBlendFactor();
                if (bfSrc == SBF_SOURCE_ALPHA && bfDst == SBF_ONE_MINUS_SOURCE_ALPHA)
                {
                    applied = true;

                    const ColourValue& diffuse = dp->getDiffuse();
                    dp->setDiffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
                    switch (dp->getAlphaRejectFunction())
                    {
                    case CMPF_ALWAYS_FAIL:
                    case CMPF_ALWAYS_PASS:
                        break;
                    case CMPF_LESS:
                    case CMPF_LESS_EQUAL:
                    case CMPF_EQUAL:
                    case CMPF_NOT_EQUAL:
                    case CMPF_GREATER_EQUAL:
                    case CMPF_GREATER:
                        dp->setAlphaRejectValue(static_cast<uchar>(128 * opacity));
                        break;
                    }
                }                
            }
        }

        if (applied)
        {
            // Must re-compile
            derived->compile();
        }

        return applied;
    }
    //////////////////////////////////////////////////////////////////////////
    
	LogicModel::LogicModel( const Ogre::String& name, System *system ) :
	mName(name),
	mSystem(system),
	mModelMainNode(NULL),
    mParentNode(NULL),
	mSkeletonEntity(NULL),
	mCreatedEntityCount(0),
	mSkeleton(NULL),
    mFullBoundingBox(NULL),
	mIsPrepareBoundBox(false),
	mCurrentAnimationState(NULL),
	mFadeOutAnimationState(NULL),
	mGlobalAnimationRate(1.0f),
	mFadeOutDelay(0.0f),
	mCurrentAnimationName(""),
	mAnimationLoop(false),
	mEffectHandleCount(0),
	mCurrentSkill(NULL),
	mOnAnimationFinish(NULL),
	mOnSkillBreakTime(NULL),
	mOnSkillHitTime(NULL),
    mOnSkillShakeTime(NULL),
    mOnGetEffectName(NULL),
    mCallbackFuncInfo(0),
	mAnimEditing(false),
	mAnimTimePos(0.0f),
	mChildModel(false),
	mMainNodeNeedUpdate(false),
	mMovementType(MT_NORMAL),
    mModelPosition(Ogre::Vector3::ZERO),
    mModelOrientation(Ogre::Quaternion::IDENTITY),
    mCurrentTransparency(0.0f),
    mDestTransparency(0.0f),
    mNeedUpdateTransparency(false),
    mTransparencyTime(0.0f),
    mDefaultAnimationName(""),//վ��
    mHairColour(Ogre::ColourValue::Black),
    mShadowCastable(false),
    mAttachParent(NULL),
    mSelected(false),
    mExternalBoundingBox(NULL),
    mExternalBoundingBoxValue(""),
    mExternalScaleFactor(Ogre::Vector3::UNIT_SCALE),
    mColliderSet(NULL),
    mTerrainHeight(Ogre::Math::POS_INFINITY),
    mVisibleFlag(OVF_DEFAULT),
    mEnableWalkingEffect(true),
    mIsActive(true),
    mVisible(true),
    mFrontBackOrientation(Ogre::Quaternion::IDENTITY),
    mLeftRightOrientation(Ogre::Quaternion::IDENTITY),
	mlinks(NULL)
	{
		mUserAny = Ogre::Any();

		mAttribMap.clear();
		mEntityMap.clear();
		mLocatorMap.clear();
		mEffectMap.clear();
		mAttachedModelOriginSceneNodes.clear();
        mModelProjectors.clear();
        mShadowUncastableMeshs.clear();
        mMaterialMap.clear();
        mFabricInfoMap.clear();
        mFabricColliderMap.clear();
        mCreatedEffectList.clear();
		mEffectHandleMap.clear();
		mAreaEffects.clear();
	}
	//-----------------------------------------------------------------------
	LogicModel::~LogicModel()
	{
        if (mExternalBoundingBox)
            delete mExternalBoundingBox;

        if (mFullBoundingBox)
            delete mFullBoundingBox;

        removeAllFabrics();

        _destroyColliders();

        // ��ɾ�������Լ����ϵ�model���������ȣ�
        mLocatorMap.clear();

        mShadowUncastableMeshs.clear();

		mAttribMap.clear();

        _delAllProjector();

		_delAllAreaEffect();

		destroyEntities();

        _delSkeletonEntity();

		destroySceneNode();

		delAllEffect();

		delCurrentSkill();
	}
	//-----------------------------------------------------------------------
	void reset(void)
	{

	}
	//-----------------------------------------------------------------------
	bool LogicModel::addAttribute( const Ogre::String &name, const Ogre::String &value, const Ogre::String &hint )
	{
		AttribMap::iterator i = mAttribMap.find(name);

		if ( i != mAttribMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' already exists! " +
				"LogicModel::addAttribute " + mName );

			return false;
		}

		std::pair<AttribMap::iterator, bool> inserted = 
			mAttribMap.insert( AttribMap::value_type( name, AttribValue(value, hint) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' inserted failed! " +
				"LogicModel::addAttribute " + mName );

			return false;
		}

        // �����������͵�����
        _dealWithExtraAttributes(name, value);

		return true;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::changeAttribute( const Ogre::String &name, const Ogre::String &value )
	{
		AttribMap::iterator i = mAttribMap.find(name);

		if ( i == mAttribMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' doesn't exists! " +
				"LogicModel::changeAttribute " + mName );

			return false;
		}

		AttribValue& tempValue = i->second;

		// �����ǰ����ֵ����Ҫ���õ�ֵ��ֱ�ӷ���
		if (tempValue.mAttribValue == value)
			return true;

		// �ж�hint
		if ( tempValue.mHint == MESH_HINT )
		{
			if ( changeEntity(name, value) )
			{
				tempValue.mAttribValue = value;
				return true;
			}
		}
		else if ( tempValue.mHint == SLMT_CREATE_HINT )
		{
			if ( changeSlotAttrib(name, value) )
			{
				tempValue.mAttribValue = value;
				return true;
			}
		}	
		else if ( tempValue.mHint == MATERIAL_HINT )
		{
			if ( changeMaterial(name, value) )
			{
				tempValue.mAttribValue = value;
				return true;
			}
		}
		else
		{
            // �����������͵�����
            _dealWithExtraAttributes(name, value);
		}

		return false;
	}
    //-----------------------------------------------------------------------
	bool LogicModel::changeAttributeHints(const Ogre::String &name,const Ogre::String &hints)
	{
		AttribMap::iterator i = mAttribMap.find(name);

		if ( i == mAttribMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' doesn't exists! " +
				"LogicModel::changeAttributeHints " + mName );

			return false;
		}

		AttribValue& tempValue = i->second;

		// �����ǰ����ֵ����Ҫ���õ�ֵ��ֱ�ӷ���
		if (tempValue.mHint == hints)
			return true;

		mAttribMap[name].mHint = hints;

		return true;
	}
    //-----------------------------------------------------------------------
    bool LogicModel::getAttribute( const Ogre::String& name,Ogre::String& value)
    {
        AttribMap::iterator i = mAttribMap.find(name);

        if ( i == mAttribMap.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' doesn't exists! " +
                "LogicModel::changeAttribute " + mName );

            return false;
        }

        value = i->second.mAttribValue;

        return true;
    }
    //-----------------------------------------------------------------------
    Variant LogicModel::getAttribute( const Ogre::String& attributeName )
    {
        AttribMap::iterator i = mAttribMap.find(attributeName);

        if ( i == mAttribMap.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + attributeName + "' doesn't exists! " +
                "LogicModel::changeAttribute " + mName );

            return Variant();
        }

        AttribValue& tempValue = i->second;

        // �ж�hint
        if ( tempValue.mHint == SLMT_CREATE_HINT )
        {
            for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
                itLocator != mLocatorMap.end(); ++itLocator )
            {
                LocatorValue& tempLocatorValue = itLocator->second;

                SlotMap::iterator itSlot = tempLocatorValue.mSlotMap.find(attributeName);

                if ( itSlot != tempLocatorValue.mSlotMap.end() )
                {
                    SlotValue& tempSlotValue = itSlot->second;

                    return tempSlotValue.mModel;
                }			
            }
        }
        
        return Variant();
    }
    //-----------------------------------------------------------------------
    bool LogicModel::hasAttribute( const Ogre::String& name )
    {
        AttribMap::iterator i = mAttribMap.find(name);

        return ( i != mAttribMap.end() );
    }
	//-----------------------------------------------------------------------
	bool LogicModel::addEntity( const Ogre::String &name, const Ogre::String &meshName, const Ogre::String &matName )
	{
		EntityMap::iterator i = mEntityMap.find(name);

		if ( i != mEntityMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + name + "' already exists! " +
				"LogicModel::addEntity " + mName );

			return false;
		}

		std::pair<EntityMap::iterator, bool> inserted = 
			mEntityMap.insert( EntityMap::value_type( name, EntityValue(meshName, matName) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + name + "' inserted failed! " +
				"LogicModel::addEntity " + mName );

			return false;
		}

		return true;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::changeEntity( const Ogre::String &name, const Ogre::String &value )
	{
		EntityMap::iterator i = mEntityMap.find(name);

		if ( i == mEntityMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + name + "' dosen't exists! " +
				"LogicModel::changeEntity " + mName );

			return false;
		}

		EntityValue& tempValue = i->second;

        // ɾ��ԭ�е�transparentor
        if (tempValue.mTransparentor)
        {
            delete tempValue.mTransparentor;
            tempValue.mTransparentor = NULL;
        }

		// ɾ��ԭ�е�entity
		if (tempValue.mEntity)
		{
            // ɾ��clone����material
            _clearEntityMaterials(tempValue);

			_destroyEntityImpl(tempValue.mEntity);
            tempValue.mEntity = NULL;
		}

        if (false == value.empty())
        {
            tempValue.mEntity = _createEntityImpl(value);

            if (tempValue.mEntityColourChanged)
                _setEntityColour(tempValue, tempValue.mEntityColour);

            if (mCurrentTransparency > 0.0f)
            {
                tempValue.mTransparentor = new EntityTransparentor(tempValue.mEntity);
                tempValue.mTransparentor->apply(mCurrentTransparency);
            }
        }

        // �����obj�ļ���û�����Χ�еĴ�С���͸���entity������Χ��
        if (NULL == mExternalBoundingBox)
            _updateBoundingBox();

    	return true;
	}

	/**add by zhuwenchao 2010.1.28
	//��ȡָ��ʵ����(���༭��AVATAR��װ����ʹ��)
	//@param hints ��Ҫ��ȡ�Ĳ��ֵ�ʵ������
	 */
	Ogre::String LogicModel::GetEntityName(const Ogre::String &hints)
	{
		if(hints.empty())
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model hints with name '" + hints + "' dosen't exists! " +
				"LogicModel::GetEntityName " + hints );

			return false;

		}
		EntityMap::iterator it = mEntityMap.begin();
		while(it != mEntityMap.end())
		{
			Ogre::StringVector vec = Ogre::StringUtil::split(it->first,"_");
			if(hints.compare(vec[0]) == 0)
				return it->first;
			it++;
		}
		return "";
	}

	//-----------------------------------------------------------------------
	bool LogicModel::addMaterial( const Ogre::String &name, const Ogre::String &entityEntry, const Ogre::String &matName )
	{
		MaterialMap::iterator i = mMaterialMap.find(name);

		if ( i != mMaterialMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' already exists! " +
				"LogicModel::addMaterial " + mName );

			return false;
		}

		std::pair<MaterialMap::iterator, bool> inserted = 
			mMaterialMap.insert( MaterialMap::value_type( name, MaterialValue(entityEntry, matName) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' inserted failed! " +
				"LogicModel::addMaterial " + mName );

			return false;
		}

		return true;
	}
    //-----------------------------------------------------------------------
	bool LogicModel::changeMaterial( const Ogre::String &name, const Ogre::String &value )
	{
		MaterialMap::iterator i = mMaterialMap.find(name);

		if ( i == mMaterialMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' dosen't exists! " +
				"LogicModel::changeMaterial " + mName );

			return false;
		}

		MaterialValue& tempValue = i->second;

		if ( _setEntityMaterial(tempValue.mEntityEntry, value) )
		{
			tempValue.mMaterialName = value;

			return true;
		}
		else
			return false;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::addLocator( const Ogre::String &name, const Ogre::String &boneName, 
		const Ogre::Vector3 &pos, const Ogre::Quaternion &orientation, bool transferable, bool translateFirst)
	{
		LocatorMap::iterator i = mLocatorMap.find(name);

		if ( i != mLocatorMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + name + "' already exists! " +
				"LogicModel::addLocator " + mName );

			return false;
		}

		std::pair<LocatorMap::iterator, bool> inserted = 
			mLocatorMap.insert( LocatorMap::value_type( name, 
            LocatorValue(boneName, pos, orientation, transferable, translateFirst ) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + name + "' inserted failed! " +
				"LogicModel::addLocator " + mName );

			return false;
		}

		return true;
	}
    //-----------------------------------------------------------------------
	bool LogicModel::DelLocator(const Ogre::String &name)
	{
		LocatorMap::iterator i = mLocatorMap.find(name);

		if ( i == mLocatorMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + name + "' not exists! " +
				"LogicModel::DelLocator " + mName );

			return false;
		}

		mLocatorMap.erase(i);

		return true;
	}
    //-----------------------------------------------------------------------
    bool LogicModel::isLocatorTransferable(const Ogre::String& name)
    {
        LocatorMap::iterator i = mLocatorMap.find(name);

        if (i != mLocatorMap.end())
        {
            return i->second.mTransferable;
        }
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::isLocatorExist(const Ogre::String& locator)
    {
        LocatorMap::iterator i = mLocatorMap.find(locator);

        if (i != mLocatorMap.end())
        {
            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::getLocatorInfo(const Ogre::String& locator, LocatorValue& locatorInfo)
    {
        LocatorMap::const_iterator i = mLocatorMap.find(locator);

        if (i != mLocatorMap.end())
        {
            locatorInfo = i->second;
            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::setLocatorInfo(const Ogre::String& locator, const LocatorValue& info)
    {
        LocatorMap::iterator i = mLocatorMap.find(locator);

        if (i != mLocatorMap.end())
        {
            i->second = info;
            return true;
        }
        else
            return false;
    }
	//-----------------------------------------------------------------------
	bool LogicModel::addSlot( const Ogre::String &locatorName, const Ogre::String &slotName, const Ogre::String &modelName, const Ogre::String &attrib )
	{
		LocatorMap::iterator i = mLocatorMap.find(locatorName);

		if ( i == mLocatorMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
				"LogicModel::addSlot " + mName );

			return false;
		}

		SlotMap &slotMap = i->second.mSlotMap;
		std::pair<SlotMap::iterator, bool> inserted = 
			slotMap.insert( SlotMap::value_type( slotName, SlotValue(modelName) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Slot with name '" + slotName + "' inserted failed! " +
				"LogicModel::addSlot " + mName );

			return false;
		}

		return true;
	}  
	//-----------------------------------------------------------------------
	bool LogicModel::changeSlotAttrib( const Ogre::String &name, const Ogre::String &value )
	{
		for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
			itLocator != mLocatorMap.end(); ++itLocator )
		{
			LocatorValue& tempLocatorValue = itLocator->second;

			SlotMap::iterator itSlot = tempLocatorValue.mSlotMap.find(name);

			if ( itSlot != tempLocatorValue.mSlotMap.end() )
			{
				SlotValue& tempSlotValue = itSlot->second;

				// ���value��""����ɾ��ԭ�е�����
				if ( value.empty() )
				{
					if (tempSlotValue.mModel)
						LogicModelManager::getSingleton().destroyLogicModel(tempSlotValue.mModel);

					tempSlotValue.mModel = NULL;
					tempSlotValue.mModelName = value;
					
					return true;
				}
				else
				{
                    tempSlotValue.mModelName = value;

					return _createModelInSlot(tempSlotValue, tempLocatorValue);
				}				
			}			
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::addEffectInfo( const Ogre::String& name, const Ogre::String& effectName,
        const Ogre::String& locatorName, bool translateAll, const Ogre::ColourValue& colour )
	{
		EffectMap::iterator i = mEffectMap.find(name);

		if ( i != mEffectMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Effect with name '" + name + "' already exists! " +
				"LogicModel::addEffectInfo " + mName );

			return false;
		}

		std::pair<EffectMap::iterator, bool> inserted = 
			mEffectMap.insert( EffectMap::value_type( name, EffectValue(effectName, locatorName, translateAll, colour) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Effect with name '" + name + "' inserted failed! " +
				"LogicModel::addEffectInfo " + mName );

			return false;
		}

		return true;
	}
    //-----------------------------------------------------------------------
    bool LogicModel::addShadowUncastable(const Ogre::String& meshName)
    {
        ShadowUncastableMeshs::iterator i = mShadowUncastableMeshs.begin();

        // �����Ƿ���������
        while (i != mShadowUncastableMeshs.end())
        {
            if (*i == meshName)
                break;

            ++i;
        }

        if ( i != mShadowUncastableMeshs.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model ShadowUncastable with name '" + 
                meshName + "' already exists!  " + "LogicModel::addShadowUncastable " + mName );

            return false;
        }

        mShadowUncastableMeshs.push_back(meshName);

        return true;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::isShadowUncastable(const Ogre::String& meshName)
    {
        ShadowUncastableMeshs::iterator i = mShadowUncastableMeshs.begin();

        // �����Ƿ���������
        while (i != mShadowUncastableMeshs.end())
        {
            if (*i == meshName)
                return true;

            ++i;
        }

        return false;
    }
	//-----------------------------------------------------------------------
	/*LogicModel& LogicModel::operator = (const LogicModel& rhs)
	{


		return *this;
	}*/
	//-----------------------------------------------------------------------
	void LogicModel::_createEntity(void)
	{
		// ����entity�б���������Ӧ��entity
		for ( EntityMap::iterator i = mEntityMap.begin();
			i != mEntityMap.end(); ++i )
		{
			Ogre::Entity* workingEntity = NULL;

			EntityValue& tempValue = i->second;
			
			String meshName = tempValue.mMeshName;
			String materialName = tempValue.mMaterialName;
			// add by zhuwenchao 2010.1.19 ��������ȡ��.mesh�ļ��Ƿ����ж�
			// ����ļ������ڣ��ͱ���������
			try
			{
				Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(meshName, DEFAULT_RESOURCE_GROUP_NAME);			
			}
			catch(const Ogre::Exception& e)
			{
				Ogre::LogManager::getSingleton().logMessage(e.getDescription());
				continue;
			}
			// ���xml�ļ���mesh�ļ�����Ϊ��
            if (false == meshName.empty())
            {
                tempValue.mEntity = _createEntityImpl(meshName);

                if (tempValue.mEntity && false == materialName.empty())
                {
                    _setEntityMaterial(i->first, materialName);

                    if (tempValue.mEntityColourChanged)
                        _setEntityColour(tempValue, tempValue.mEntityColour);

                    if (mCurrentTransparency > 0.0f)
                    {
                        tempValue.mTransparentor = new EntityTransparentor(tempValue.mEntity);
                        tempValue.mTransparentor->apply(mCurrentTransparency);
                    }
                }
            }			
		}

        // �����obj�ļ���û�����Χ�еĴ�С���͸���entity������Χ��
        if (NULL == mExternalBoundingBox)
            _updateBoundingBox();
	}
	//-----------------------------------------------------------------------
	void LogicModel::destroyEntities(void)
	{
		for ( EntityMap::iterator i = mEntityMap.begin();
			i != mEntityMap.end(); ++i )
		{
            if (i->second.mTransparentor)
            {
                delete i->second.mTransparentor;
                i->second.mTransparentor = NULL;
            }

            // ɾ��clone����material
            _clearEntityMaterials(i->second);

			Ogre::Entity* workingEntity = i->second.mEntity;

			if (workingEntity)
			{
				String entityName = workingEntity->getName();

                Ogre::SceneNode* parentNode = workingEntity->getParentSceneNode();

                if (parentNode)
                {
                    parentNode->detachObject(entityName);
                }

				mSystem->getSceneManager()->destroyEntity(entityName);
			}
		}
		mEntityMap.clear();
	}
	//-----------------------------------------------------------------------
	void LogicModel::setUserAny(const Ogre::Any& anything)
	{
		mUserAny = anything;

		for ( EntityMap::iterator i = mEntityMap.begin();
			i != mEntityMap.end(); ++i )
		{
			if (i->second.mEntity)
				i->second.mEntity->setUserAny(anything);
		}

		// locator�ϵ�modelҲҪ����
		for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
			itLocator != mLocatorMap.end(); ++itLocator )
		{
			LocatorValue& tempValue = itLocator->second;

			for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
				itSlot != tempValue.mSlotMap.end(); ++itSlot )
			{
				SlotValue& tempSlot = itSlot->second;

				if (tempSlot.mModel)
					tempSlot.mModel->setUserAny(anything);
			}
		}
	}
	//-----------------------------------------------------------------------
	const Ogre::Any& LogicModel::getUserAny(void) const
	{
		return mUserAny;
	}
	//-----------------------------------------------------------------------
	void LogicModel::setUserObject(Ogre::UserDefinedObject* obj)
	{
		setUserAny(Ogre::Any(obj));
	}
	//-----------------------------------------------------------------------
	Ogre::UserDefinedObject* LogicModel::getUserObject(void) const
	{
		return mUserAny.isEmpty() ? 0 : Ogre::any_cast<Ogre::UserDefinedObject*>(mUserAny);
	}
	//-----------------------------------------------------------------------
	void LogicModel::setParentSceneNode( Ogre::Node* parentNode )
	{
		if(mModelMainNode && mModelMainNode->getParentSceneNode())
			mModelMainNode->getParentSceneNode()->removeChild(mModelMainNode->getName());

		if(parentNode)
		{
			parentNode->addChild(mModelMainNode);

		}

        mParentNode = parentNode;
	}
	//-----------------------------------------------------------------------
	void LogicModel::destroySceneNode(void)
	{
		if (mModelMainNode)
		{
			if (mModelMainNode->getParentSceneNode())
			{
				mModelMainNode->getParentSceneNode()->removeChild(mModelMainNode->getName());
			}
            mModelMainNode->removeAndDestroyAllChildren();
			mModelMainNode->getCreator()->destroySceneNode(mModelMainNode->getName());
            mModelMainNode = NULL;
		}
	}
	//-----------------------------------------------------------------------
	const Ogre::AxisAlignedBox& LogicModel::getBoundingBox(void)
	{
        if (mExternalBoundingBox)
            return *mExternalBoundingBox;
        else
        {
            return _getFullBoundingBox();
        }
	}
	//-----------------------------------------------------------------------
	void LogicModel::_destroyEntityImpl(Ogre::Entity* entity)
	{
		assert (entity);

		String entityName = entity->getName();

  //      if (entityName == mSkeletonEntity->getName())
  //      {
  //          mSkeletonEntity = NULL;
  //          mSkeleton = NULL;
  //      }

		entity->getParentSceneNode()->detachObject(entityName);

		mSystem->getSceneManager()->destroyEntity(entityName);		
	}
	//-----------------------------------------------------------------------
	Ogre::Entity* LogicModel::_createEntityImpl( const Ogre::String& meshName )
	{
		Ogre::Entity* resultEntity = NULL;

        loadMesh(
            meshName, Ogre::StringUtil::BLANK,
            Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);

		String entityName = mModelMainNode->getName() + "_Entity" + "_" + meshName +
			Ogre::StringConverter::toString(mCreatedEntityCount++);

        static bool suppressSkeletalAnimation = true;
		//david-<<
		//resultEntity = mSystem->getSceneManager()
        //    ->createEntity(entityName, meshName, suppressSkeletalAnimation);
		resultEntity = mSystem->getSceneManager()
		    ->createEntity(entityName, meshName);
		//david->>

		resultEntity->setUserAny(getUserAny());

        static bool initialised = false;
        static bool changeRenderQueue = false;
        if (!initialised)
        {
            initialised = true;
            const char* s = getenv("Fairy_Hack");
            changeRenderQueue = s && (strstr(s, "X-Ray") != 0);
        }
        if (changeRenderQueue)
        {
            resultEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_6);
        }
      
        // ����entityΪ��ǰ��visible flag
        resultEntity->setVisibilityFlags(mVisibleFlag);
        // �ֶ�����visible������octree manager��bug������Ҫ�ֶ����ã�
        resultEntity->setVisible(mVisible);

		//david-<<
        // ������Ҫ����normalize normal
        //resultEntity->setNormaliseNormals(
        //    !mExternalScaleFactor.positionEquals(Ogre::Vector3::UNIT_SCALE));
		//david->.

		mModelMainNode->attachObject(resultEntity);

		if (!resultEntity->getMesh()->getSkeleton().isNull()) 
		{
            // ���master enttiyΪ�գ�˵����ǰ������ǵ�һ���й�����entity��������master entity
            if (NULL == mSkeletonEntity)
            {
                const Ogre::MeshPtr& originMesh = resultEntity->getMesh();
                assert (false == originMesh.isNull());
                const Ogre::SkeletonPtr& originSke = originMesh->getSkeleton();
                assert (false == originSke.isNull());

                _createSkeletonEntity(originSke);
            }

            assert (mSkeletonEntity);

            // ����ڹ������ʱ�����쳣����¼����ص���Ϣ
            try
            {
                resultEntity->shareSkeletonInstanceWith(mSkeletonEntity);
            }
            catch (const Ogre::Exception& e)
            {
                Ogre::StringUtil::StrStreamType errorString;

                errorString << "LogicModel::_createEntityImpl --> shareSkeletonInstanceWith\n";
                errorString << "new created entity : " << resultEntity->getMesh()->getName() << std::endl;
                errorString << "skeleton entity : " << mSkeletonEntity->getMesh()->getName() << std::endl;

                Ogre::LogManager::getSingleton().logMessage(errorString.str());

                if (mSystem->getThrowExceptionIfResourceIncorrect())
                {
                    if(resultEntity)
					{
						mModelMainNode->getCreator()->destroyEntity(resultEntity);
						resultEntity = NULL;
					}
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Resource was incorrectness due incaution producer. "
                        "Full error description: " + e.getFullDescription(),
                        "LogicModel::_createEntityImpl");
                }
            }
		}

        // share skeleton֮��Ŵ���hardware skin material����ʱ���ܱ�֤entity�е�mSkeletonInstance����ֵ��
        _convertEntityToCharacterMaterial(resultEntity);
        // bool succeed = convertEntityToHardwareSkinning(resultEntity);

		if(mSkeletonEntity && !mSkeleton)
        {
			mSkeleton = mSkeletonEntity->getSkeleton();
            // �ı���ģʽ
            mSkeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
        }

        // �����ǰmodel������Ϊ����ͶӰ����Ҫ��������
        resultEntity->setCastShadows(mShadowCastable && false == isShadowUncastable(meshName));

		return resultEntity;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createMainSceneNode(void)
	{
		mModelMainNode = mSystem->getSceneManager()->getRootSceneNode()->createChildSceneNode();

        mParentNode = mModelMainNode->getParent();

        // ������������
        mModelMainNode->setScale(mExternalScaleFactor);
	}
	//-----------------------------------------------------------------------
	void LogicModel::initModel(void)
	{
		_createMainSceneNode();

        // ����external bounding box
        if (false == mExternalBoundingBoxValue.empty())
        {
            _createBoundingBoxFromExternal();
        }

		_createEntity();

		_setEntityMaterials();

		_createLocatorNode();

		_createEffects();

        // ���ų�ʼ����
        //if (false == mDefaultAnimationName.empty())
        //{
        //    createSkill(mDefaultAnimationName, true, true);
        //}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createLocatorNode(void)
	{
		for ( LocatorMap::iterator i = mLocatorMap.begin();
			i != mLocatorMap.end(); ++i )
		{
			LocatorValue& tempValue = i->second;

            // �����Ȳ�����locator�����scene node���ȵ���Ҫʱ�Ŵ���
		//	assert (NULL == tempValue.mLocatorNode);
		//	tempValue.mLocatorNode = 
        //        _createLocatorSceneNode(tempValue.mOffsetPos, tempValue.mOffsetOrientation);

            // ������ʼ��model
            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (false == tempSlot.mModelName.empty())
                    _createModelInSlot(tempSlot, tempValue);
            }
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::execute(Ogre::Real delta)
	{	
        // ���ɼ��Ĳ��Ҳ���ui�е�obj�����ø���
        if(!mVisible && mVisibleFlag != OVF_GUI_ELEMENTS)
            return;

		if (mMainNodeNeedUpdate)
		{
			// ���λ�û���ת�����˱仯������Ҫ��ʱ����main node
			mModelMainNode->_update(true, false);

			// ���ݵ�ǰ��������ת����
			if ( (mMovementType != MT_NORMAL) && mGetHeightInWorld )
				_rotate();

			mMainNodeNeedUpdate = false;
		}

		// ���²���Ķ����б�
        _updatePlayingAnimations(delta);

        // ����������
		_animationBlend(delta);

        // ����͸����
        if (mNeedUpdateTransparency)
            updateTransparency(delta);

		if(mCurrentAnimationState)
		{
			Ogre::Real totalen = 0.0f;
            Ogre::Real oldTime = 0.0f;
            Ogre::Real currTime = 0.0f;

			totalen = mCurrentAnimationState->getLength();

            // �����������ĳ��Ȳ�Ϊ0����������addTime�����������ȷ��oldTime��currTime
            if ( false == Ogre::Math::RealEqual(0.0f, totalen) )
            {
                oldTime = mCurrentAnimationState->getTimePosition() / totalen;

#ifdef EDITOR
                // �����ǰ���Ǳ༭״̬������ѭ������
                if ( false == mAnimEditing )
                {
                    mCurrentAnimationState->addTime(delta * mGlobalAnimationRate);
                }
                else
                {
                    mCurrentAnimationState->setTimePosition( mAnimTimePos * totalen );
                }
#else
                mCurrentAnimationState->addTime(delta * mGlobalAnimationRate);

#endif
                // Make sure the skeletal animation of the entities up-to-date, so all bones,
                // tag-points transform will up-to-date on demand
                static bool old = false;
                if (old)
                    mSkeleton->setAnimationState(*mSkeletonEntity->getAllAnimationStates());
				//wangling-<<
                else
				{
					mSkeletonEntity->_updateSkeletalAnimation();
				}
				//david->>

                currTime = mCurrentAnimationState->getTimePosition() / totalen;
            }

			bool animationConitnue = _handleAnimationFinish(oldTime, currTime);

		
            
			if (animationConitnue)
			{
				// ����bone locator��λ��
				_updateLocatorPos();

				if ( mCurrentSkill )
				{					
					_handleSkillHitTime(oldTime, currTime);
					_handleSkillBreakTime(oldTime, currTime);	
                    _handleSkillShakeTime(oldTime, currTime);

					if (mCurrentSkill && mVisible)
					{
						_createAnimationEffect(oldTime, currTime);
						_updateAttachedAnimationEffect(oldTime, currTime);
						_createAnimationCameraShake(oldTime, currTime);
						_updateAnimationCameraShake(delta);
						_createAnimationRibbon(oldTime, currTime);
						_updateAnimationRibbon(delta);  
						_createAnimationBulletFlow(oldTime,currTime);
						_updateAnimationBulletFlow(oldTime,currTime);
						_updateAnimationSound(oldTime, currTime);

                        _updateAnimationLight(oldTime, currTime);
					}
				}
			}
		}

        _updateFabricColliders();

        // ���¹���model�ϵ�effect
        _updateAllEffects();

        _updateModelProjectors();

		_updateAreaEffects();

		_executeLocatorModel(delta);

        // ����ͨ��attachModel�ϵ�model
        _executeAttachedModel(delta);
	}
	//-----------------------------------------------------------------------
	void LogicModel::setVisibleFlag(Ogre::uint32 flags)
	{
        if (mVisibleFlag != flags)
        {
            mVisibleFlag = flags;

            // ���������entity
            for ( EntityMap::iterator itEntity = mEntityMap.begin();
                itEntity != mEntityMap.end(); ++itEntity )
            {
                Ogre::Entity* pEntity = itEntity->second.mEntity;
                if (pEntity)
                    pEntity->setVisibilityFlags(flags);
            }

            // ͨ��addEffect�����ȥ��effectҲҪ����visible flag����������Ч��
            CreatedEffectList::iterator i = mCreatedEffectList.begin();

            while ( i != mCreatedEffectList.end() )
            {
                Fairy::Effect* effect = (*i).mEffect;

                if (effect)
                {
                    effect->setVisibleFlag(flags);
                }

                ++i;
            }		

            // ����fabric��visible flag
            FabricInfoMap::iterator fabricIterator = mFabricInfoMap.begin();

            while ( fabricIterator != mFabricInfoMap.end() )
            {
                fabricIterator->second.mFabric->setVisibilityFlags(flags);

                ++fabricIterator;
            }

            // locator�ϵ�modelҲҪ����
            for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
                itLocator != mLocatorMap.end(); ++itLocator )
            {
                LocatorValue& tempValue = itLocator->second;

                for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                    itSlot != tempValue.mSlotMap.end(); ++itSlot )
                {
                    SlotValue& tempSlot = itSlot->second;

                    if (tempSlot.mModel)
                        tempSlot.mModel->setVisibleFlag(flags);
                }
            }
        }		
	}
	//-----------------------------------------------------------------------
	bool LogicModel::_changeAnimation(const Ogre::String& animationName, Ogre::Real delay)
	{
		if (mFadeOutAnimationState)
		{
			mFadeOutAnimationState->setWeight(0);
			mFadeOutAnimationState->setEnabled(false);
		}

		mFadeOutAnimationState = mCurrentAnimationState;
		mFadeOutDelay = delay;

		if(animationName.empty())
		{
			mCurrentAnimationState = NULL;
			mCurrentAnimationName = "";
		}
		else
		{
			try{

                // �п�����Ϊ��Դ��ԭ��ȱ�������skeleton�ļ����������entityΪ��
                if (mSkeletonEntity)
                {
				    mCurrentAnimationState = _getAnimationState(animationName);

                    mSkeleton->getAnimation(animationName)->setInterpolationMode( 
                    LogicModelManager::getSingleton().getAnimationInterpolationMode() );
                }
                else
                {
                    Ogre::LogManager::getSingleton().logMessage( "Logic Model : " + mName + 
                        " Skeleton Entity is NULL, Maybe the skeleton file is lost!" +
                        "LogicModel::_changeAnimation");
                }

			}
			catch (const Ogre::Exception& e)
			{
                Ogre::LogManager::getSingleton().logMessage("Model : " + mName + '\n' + "_getAnimationState Failed!");
				mCurrentAnimationState = NULL;

                if (mSystem->getThrowExceptionIfResourceIncorrect())//��Ӳ�׽��ɣ������������ڣ�
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Resource was incorrectness due incaution producer. "
                        "Full error description: " + e.getFullDescription(),
                        "LogicModel::_changeAnimation");
                }
			}
			if(mCurrentAnimationState)
			{
				if (mFadeOutAnimationState == mCurrentAnimationState)
					mFadeOutAnimationState = NULL;

				mCurrentAnimationState->setEnabled(true);
				mCurrentAnimationState->setTimePosition(0.0f);
				if(delay>0.0f)//yangfei temp modify(�ⲿ�ִ���3d�����һ������)
				{
					mCurrentAnimationState->setWeight(mFadeOutAnimationState ? 0 : 1);
				}
				else
				{
					mCurrentAnimationState->setWeight(1);
				}
				mCurrentAnimationName = animationName;

                // ��ʱע�͵�������棬��Ϊ�������ǵ�������������ֻ��һ֡
                //float animLength = mCurrentAnimationState->getLength();

                //if ( animLength <= 0.0f )
                //{
                //    Ogre::LogManager::getSingleton().logMessage( mName + " The length of animation '"
                //        + mCurrentAnimationName + "' <= zero '" + 
                //        Ogre::StringConverter::toString(animLength) + "' " +
                //        "LogicModel::_changeAnimation");
                //}

				return true;
			}
			else
			{
				mCurrentAnimationName = "";
				return false;
			}
		}
		return true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateLocatorPos(void)
	{
		for ( LocatorMap::iterator i = mLocatorMap.begin();
			i != mLocatorMap.end(); ++i )
		{
			LocatorValue& tempValue = i->second;

            // ֻ���´��������ģ��Ѿ�ʹ�õģ�locator node
            if (tempValue.mLocatorNode)
            {
			    if (false == tempValue.mBoneName.empty())
			    {
				    Ogre::Bone* bone = NULL;
				    try
				    {
					    bone = mSkeleton->getBone(tempValue.mBoneName);
				    }
				    catch (const Ogre::Exception& e)
				    {
					    Ogre::LogManager::getSingleton().logMessage("LogicModel::_updateLocatorPos " + mName + e.getDescription());

                        if (mSystem->getThrowExceptionIfResourceIncorrect())
                        {
                            OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                                "Resource was incorrectness due incaution producer. "
                                "Full error description: " + e.getFullDescription(),
                                "LogicModel::_updateLocatorPos");
                        }

                        continue;
				    }

				    assert (bone);

				    Ogre::SceneNode* locatorNode = tempValue.mLocatorNode;
				    assert (locatorNode);
					//wangling-<<
				    locatorNode->setPosition( bone->_getDerivedPosition() );
				    locatorNode->setOrientation( bone->_getDerivedOrientation() );
					//wangling->>

                    if (tempValue.mTranslateFirst)
                    {
                        locatorNode->translate( tempValue.mOffsetPos, Ogre::Node::TS_LOCAL );
                        locatorNode->rotate( tempValue.mOffsetOrientation, Ogre::Node::TS_LOCAL );
                    }
                    else
                    {
                        locatorNode->rotate( tempValue.mOffsetOrientation, Ogre::Node::TS_LOCAL );
                        locatorNode->translate( tempValue.mOffsetPos, Ogre::Node::TS_LOCAL );
                    }

                    // �����ﲢ������locatornode��update
             //       locatorNode->_update(true,false);
			    }
            }
		}

		// ȫ��locator node��������positon֮���ٵ������ǹ�ͬ�ĸ��ڵ���update
        if (mLocatorMap.size())
            mModelMainNode->_update(true,false);
	}
	//-----------------------------------------------------------------------
	void LogicModel::setAnimationLoop(bool loop)
	{
		mAnimationLoop = loop;

		if (mCurrentAnimationState)
		{
			mCurrentAnimationState->setLoop(loop);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_animationBlend(Ogre::Real delta)
	{
		if (mFadeOutAnimationState && mCurrentAnimationState)
		{
			if (mFadeOutDelay <= delta)
			{
				mFadeOutAnimationState->setWeight(0);
				mFadeOutAnimationState->setEnabled(false);
				mCurrentAnimationState->setWeight(1);
				mFadeOutAnimationState = NULL;
			}
			else
			{
				Ogre::Real weight = mCurrentAnimationState->getWeight();
				weight = weight + delta * (1 - weight) / mFadeOutDelay;

				mCurrentAnimationState->setWeight(weight);
				mFadeOutAnimationState->setWeight(1 - weight);

				mFadeOutAnimationState->addTime(delta * mGlobalAnimationRate);
			}

			mFadeOutDelay -= delta;
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_executeLocatorModel(Ogre::Real delta)
	{
		for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
			itLocator != mLocatorMap.end(); ++itLocator )
		{
			LocatorValue& tempValue = itLocator->second;

			for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
				itSlot != tempValue.mSlotMap.end(); ++itSlot )
			{
				SlotValue& tempSlot = itSlot->second;

				if (tempSlot.mModel)
					tempSlot.mModel->execute(delta);
			}
		}
	}
	//-----------------------------------------------------------------------
	LogicModel::EffectHandle LogicModel::addEffect( const Ogre::String& effectName, const Ogre::String& locatorName,
        LogicModel::GetTransformInfoType transformType, const Ogre::ColourValue& colour )
	{	
		// �ҵ����locator��scene node
		LocatorMap::iterator i = mLocatorMap.find(locatorName);

		if ( i == mLocatorMap.end() )
		{
		//	Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
		//		"LogicModel::addEffect " + mName );

            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
                "LogicModel::addEffect " + mName + " " + effectName,
                "LogicModel::addEffect");

			return 0;
		}
		else
		{
			Effect *effect = EffectManager::getSingleton().createEffect(effectName);

			if (effect)
			{
				effect->createSceneNode();

                // �ڴ�����ʱ��������Ϊһ����Զ�ĵط�����Ϊ�п����޳��ˣ�����ִ��
                // ���model��execute��effect��λ�õ���Ϣ���ᱻ���£��������������
                // �����п���һ��û������λ�õ�effect
                TransformInfo info;
                info.mPosition = Ogre::Vector3(0,-10000,0);

                effect->setTransformInfo(info);

                // ����effectΪ��ǰ��visible flag
                effect->setVisibleFlag(mVisibleFlag);
                effect->setVisible(mVisible);

				// ֻ���ر�ָ������ɫ������Ҫ���õ�effect��
                if (colour != Ogre::ColourValue::White)
                    effect->setColour(colour);

                mCreatedEffectList.push_back( CreatedEffectInfo(effect, locatorName, transformType, ++mEffectHandleCount) );

				return mEffectHandleCount;
			}
			else
				return 0;
		}		
	}
	//-----------------------------------------------------------------------
	LogicModel::CreatedEffectList::iterator LogicModel::delEffect( EffectHandle handle )
	{
        assert (handle > 0);

        CreatedEffectList::iterator i = mCreatedEffectList.begin();

        // ��λeffect
        while ( i != mCreatedEffectList.end() )
        {
            if ( (*i).mEffectHandle == handle )
            {
                break;
            }

            ++i;
        }

		// Remove handle for effecthandelmap
		for(EffectHandleMap::iterator handleIter = mEffectHandleMap.begin(); handleIter != mEffectHandleMap.end(); ++handleIter)
		{
			if (handleIter->second == handle)
			{
				mEffectHandleMap.erase(handleIter);
				break;
			}
		}

		if ( i != mCreatedEffectList.end() )
		{
			EffectManager::getSingleton().removeEffect(((*i).mEffect),false);

            // ȥ������remove��effect������
            (*i).mEffect = NULL;

            // erase���������¸�iterator
			return mCreatedEffectList.erase(i);
		}
		else
        {
            // ���ҵ�effect������
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "the handle " + Ogre::StringConverter::toString(handle) + " is wrong!",
                "LogicModel::delEffect");
        }
	}
    //-----------------------------------------------------------------------
    LogicModel::CreatedEffectList::iterator 
        LogicModel::delEffect( LogicModel::CreatedEffectList::iterator effectIterator )
    {
        assert ( effectIterator != mCreatedEffectList.end() );
		
		// Remove handle for effecthandelmap
		for(EffectHandleMap::iterator handleIter = mEffectHandleMap.begin(); handleIter != mEffectHandleMap.end(); ++handleIter)
		{
			if (handleIter->second == (*effectIterator).mEffectHandle)
			{
				mEffectHandleMap.erase(handleIter);
				break;
			}
		}

        EffectManager::getSingleton().removeEffect(((*effectIterator).mEffect),false);

        // ȥ������remove��effect������
        (*effectIterator).mEffect = NULL;

        // erase���������¸�iterator
        return mCreatedEffectList.erase(effectIterator);
    }
	//-----------------------------------------------------------------------
	void LogicModel::delAllEffect(void)
	{
		for ( CreatedEffectList::iterator i = mCreatedEffectList.begin();
			i != mCreatedEffectList.end(); ++i )
		{		
			Effect* effect = (*i).mEffect;
			if (effect)
			{
				EffectManager::getSingleton().removeEffect(effect,false);

                // ȥ������remove��effect������
                (*i).mEffect = NULL;
			}
		}
		mCreatedEffectList.clear();
		mEffectHandleMap.clear();
	}
    //-----------------------------------------------------------------------
    Effect* LogicModel::getEffect(LogicModel::EffectHandle handle)
    {
        assert (handle > 0);

        CreatedEffectList::iterator i = mCreatedEffectList.begin();

        // ��λeffect
        while ( i != mCreatedEffectList.end() )
        {
            if ( (*i).mEffectHandle == handle )
            {
                break;
            }

            ++i;
        }

        if ( i != mCreatedEffectList.end() )
        {
            return (*i).mEffect;
        }
        else
        {
            // ���ҵ�effect������
            /*OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "the handle " + Ogre::StringConverter::toString(handle) + " is wrong!",
                "LogicModel::getEffect");*/
            return NULL;
        }
    }
	//-----------------------------------------------------------------------
	bool LogicModel::getLocatorWorldTransform(const Ogre::String& name, TransformInfo& info, const Ogre::Vector3& offsetPos,
		const Ogre::Quaternion& offsetRotation, LogicModel::GetTransformInfoType type )
	{
		Ogre::Matrix4 mtx;
		Ogre::Matrix3 mat(Ogre::Matrix3::IDENTITY);

		LocatorMap::iterator it = mLocatorMap.find(name);

		if (it == mLocatorMap.end())
			return false;
		else
		{
			LocatorValue& tempValue = it->second;

			Ogre::SceneNode* locatorNode = _getLocatorSceneNode(tempValue);

			switch (type)
			{
			case GTIT_ALL:
				//wangling-<<
                info.mRotation = locatorNode->_getDerivedOrientation() * offsetRotation;
                info.mPosition = locatorNode->_getDerivedOrientation() * ( locatorNode->_getDerivedScale() * offsetPos )
                    + locatorNode->_getDerivedPosition();
				//wangling->>

				break;

			case GTIT_POSITION:
				//wangling-<<
                info.mPosition = locatorNode->_getDerivedOrientation() * ( locatorNode->_getDerivedScale() * offsetPos )
                   + locatorNode->_getDerivedPosition();
				//wangling->>

				break;

			case GTIT_ORIENTATION:
				//wangling-<<
                info.mRotation = locatorNode->_getDerivedOrientation() * offsetRotation;
				//wangling->>

				break;

			default:
				return false;
			}

			return true;
		}		
	}
	//-----------------------------------------------------------------------
	bool LogicModel::getBoneWorldTransform(const Ogre::String& name, TransformInfo& info, const Ogre::Vector3& offsetPos,
		const Ogre::Quaternion& offsetRotation, LogicModel::GetTransformInfoType type )
	{
		assert (mSkeleton);

		Ogre::Bone* bone = NULL;

		try
		{
			bone = mSkeleton->getBone(name);
		}
		catch (const Ogre::Exception& e)
		{
			Ogre::LogManager::getSingleton().logMessage("LogicModel::getBoneWorldTransform " + mName + e.getDescription());

            /*if (mSystem->getThrowExceptionIfResourceIncorrect())//�󶨵�û�ҵ���������Ҳû������֣�
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                    "Resource was incorrectness due incaution producer. "
                    "Full error description: " + e.getFullDescription(),
                    "LogicModel::getBoneWorldTransform");
            }*/

			return false;
		}

		Ogre::Matrix3 mat(Ogre::Matrix3::IDENTITY);

		assert (bone);
		
		switch (type)
		{
		case GTIT_ALL:

			info.mRotation = mModelMainNode->_getDerivedOrientation() * bone->_getDerivedOrientation() * offsetRotation;

			info.mRotation.ToRotationMatrix(mat);

			info.mPosition = mModelMainNode->_getFullTransform() * bone->_getDerivedPosition() + mat * offsetPos;

			break;

		case GTIT_POSITION:

			info.mPosition = mModelMainNode->_getFullTransform() * bone->_getDerivedPosition() + offsetPos;

			break;

		case GTIT_ORIENTATION:

			info.mRotation = mModelMainNode->_getDerivedOrientation() * bone->_getDerivedOrientation() * offsetRotation;

			break;

		default:
			return false;
		}	

		return true;		
	}
    //-----------------------------------------------------------------------
    bool LogicModel::getLocatorWorldTransform(const Ogre::String& name, Ogre::Matrix4& mtx)
    {
        LocatorMap::iterator it = mLocatorMap.find(name);

        if (it == mLocatorMap.end())
            return false;
        else
        {
            LocatorValue& tempValue = it->second;

            Ogre::SceneNode* locatorNode = _getLocatorSceneNode(tempValue);

            mtx = locatorNode->_getFullTransform();

            return true;
        }
    }
	void LogicModel::stopCurrentAction()
	{
		setCurrentAnimTime(1.0f);
	}
	//-----------------------------------------------------------------------
	void LogicModel::createSkill(const Ogre::String& skillName, bool loop, bool anim, Real delayTime)
	{
		delCurrentSkill();
		try
		{
			if ( false == anim )
			{
				mCurrentSkill = EffectManager::getSingleton().createSkill(skillName);

				if (NULL == mCurrentSkill)
				{
					_changeAnimation(skillName, delayTime);
				}
				else
				{
					_changeAnimation(mCurrentSkill->getAnimationName(), delayTime);
				}
			}
			else
			{
				_changeAnimation(skillName, delayTime);
			}												
		}
		catch ( Ogre::Exception& e)
		{
			if (e.getNumber() ==  Ogre::Exception::ERR_RT_ASSERTION_FAILED)
			{
				 Ogre::LogManager::getSingleton().logMessage("Animation : " + skillName + '\n' + "_getAnimationState Failed!");
			}
			throw;
		}
		setAnimationLoop(loop);
	}
	//-----------------------------------------------------------------------
	void LogicModel::delCurrentSkill(void)
	{
		if (mCurrentSkill)
		{
			// todo
			_muteCurrentSkillSound();
			EffectManager::getSingleton().removeSkill(mCurrentSkill);
			mCurrentSkill = NULL;
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_muteCurrentSkillSound(void)
	{
		if (mCurrentSkill)
		{
			if (mAnimationLoop && mOnStopSound)
			{
				for ( Ogre::ushort i=0; i<mCurrentSkill->getNumAnimationSounds(); ++i )
				{
					AnimationSound *sound = mCurrentSkill->getAnimationSound(i);
					assert (sound);
					if (sound->mSoundHandle != -1)
					{
						mOnStopSound(sound->mSoundHandle);
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	bool LogicModel::_handleAnimationFinish(Ogre::Real oldTime, Ogre::Real currTime)
	{
		if (currTime <= oldTime && false == mAnimationLoop)
		{
			if ( mOnAnimationFinish )
			{
				mOnAnimationFinish(mCurrentAnimationName.c_str(), mCallbackFuncInfo,0);

				return false;
			}
		}

		return true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_handleSkillHitTime(Ogre::Real oldTime, Ogre::Real currTime)
	{
		if (mCurrentSkill)
		{
            unsigned short hitTimeCount = mCurrentSkill->getNumHitTimes();
			for (Ogre::ushort i=0; i<hitTimeCount; ++i)
			{
				Ogre::Real hitTime = mCurrentSkill->getHitTime(i);

				if (hitTime >= 0.0f && hitTime <= 1.0f)
				{
					if((currTime >= oldTime && hitTime >= oldTime && hitTime < currTime) || 
						(currTime < oldTime && (hitTime >= oldTime || hitTime < currTime)))
					{
						if (mOnSkillHitTime)
							mOnSkillHitTime(mCurrentAnimationName.c_str(), mCallbackFuncInfo, i, hitTimeCount);

						if ( mCurrentSkill == NULL )
							break;
					}
				}
			}
		}		
	}
	//-----------------------------------------------------------------------
	void LogicModel::_handleSkillBreakTime(Ogre::Real oldTime, Ogre::Real currTime)
	{
		if (mCurrentSkill)
		{
            unsigned short breakTimeCount = mCurrentSkill->getNumBreakTimes();
			for (Ogre::ushort i=0; i<breakTimeCount; ++i)
			{
				Ogre::Real breakTime = mCurrentSkill->getBreakTime(i);

				if (breakTime >= 0.0f && breakTime <= 1.0f)
				{
					if((currTime >= oldTime && breakTime >= oldTime && breakTime < currTime) || 
						(currTime < oldTime && (breakTime >= oldTime || breakTime < currTime)))
					{
						if (mOnSkillBreakTime)
							mOnSkillBreakTime(mCurrentAnimationName.c_str(), mCallbackFuncInfo, i, breakTimeCount);

						if ( mCurrentSkill == NULL )
							break;
					}
				}
			}
		}
	}
    //-----------------------------------------------------------------------
    void LogicModel::_handleSkillShakeTime(Ogre::Real oldTime, Ogre::Real currTime)
    {
        if (mCurrentSkill)
        {
            for (Ogre::ushort i=0; i<mCurrentSkill->getNumShakeTimes(); ++i)
            {
                Ogre::Real shakeTime = mCurrentSkill->getShakeTime(i);

                if (shakeTime >= 0.0f && shakeTime <= 1.0f)
                {
                    if((currTime >= oldTime && shakeTime >= oldTime && shakeTime < currTime) || 
                        (currTime < oldTime && (shakeTime >= oldTime || shakeTime < currTime)))
                    {
                        if (mOnSkillShakeTime)
                            mOnSkillShakeTime(mCurrentAnimationName.c_str(), mCallbackFuncInfo);
                    }
                }
            }
        }
    }
	//-----------------------------------------------------------------------
	void LogicModel::_createAnimationEffect(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationEffectInfos(); ++i )
		{
			AnimationEffectInfo *effectInfo = mCurrentSkill->getAnimationEffectInfo(i);
			assert (effectInfo);

			Ogre::Real time = effectInfo->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				Effect *effect = effectInfo->getEffect();

                // ��ȡ�ű��е���Ч����
                const Ogre::String& effectTemplateName = effectInfo->getEffectTemplateName();

               Ogre::String realEffectTemplate;

                // �����ģ����Ч����
                if ( effectTemplateName.find("<") !=Ogre::String::npos )
                {
                    if ( false == mEnableWalkingEffect ||
                        false == _getTemplateEffectName( realEffectTemplate, effectTemplateName, mTerrainHeight ) )
                        continue;
                }
                else
                {
                    realEffectTemplate = effectTemplateName;

                    mTerrainHeight = Ogre::Math::POS_INFINITY;
                }

                //////////////////////////////////////////////////////////////////////////
               Ogre::String attachPoint = effectInfo->getAttachPoint();

                TransformInfo info;

                // �ȴӰ󶨵��ң����û���ٴӹ�ͷ������
                if ( getLocatorWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
                    effectInfo->getOffsetRotation() ) )
                {
                }
                else if ( getBoneWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
                    effectInfo->getOffsetRotation() ) )
                {
                }

                if (mTerrainHeight < Ogre::Math::POS_INFINITY && info.mPosition.y >= mTerrainHeight)
                    return;
                //////////////////////////////////////////////////////////////////////////

#ifdef EDITOR
                if (effect)
                {
                    // �ȰѾɵ�ɾ�����ڴ����µ�
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

#else

                if (effect)
                {
                    // ���������ѭ����(�����ߵ������������϶���loop��)�����ҵ�ǰskill��Ҫ��ÿ�ν��ж���ʱ������һ������Ч
                    if (mCurrentSkill->getRepeatEffect())
                    {
                        // �ȰѾɵ�ɾ�����ڴ����µ�
                        EffectManager::getSingleton().removeEffect(effect, false, false);

                        effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
                        effect->createSceneNode();
                        effectInfo->setEffect(effect);
                    }
                }
                else
                {
                    effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
                    effect->createSceneNode();
                    effectInfo->setEffect(effect);
                }
#endif

				if ( false == effectInfo->getAttach() )
				{
                    // ������Ҫ�����߶�����
                    if (mTerrainHeight < Ogre::Math::POS_INFINITY)
                    {
                        info.mPosition.y = mTerrainHeight;
                    }

					// ��effect�����
					effect->setTransformInfo(info);
				}					
			}
		}
	}

	//-----------------------------------------------------------------------
	void LogicModel::_updateAttachedAnimationEffect(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationEffectInfos(); ++i )
		{
			AnimationEffectInfo *effectInfo = mCurrentSkill->getAnimationEffectInfo(i);
			assert (effectInfo);

			Effect *effect = effectInfo->getEffect();

			if (effect && effectInfo->getAttach())
			{
                // ��ȡ�ű��е���Ч����
                const Ogre::String& effectTemplateName = effectInfo->getEffectTemplateName();

               Ogre::String realEffectTemplate;

                // �����ģ����Ч����
                if ( effectTemplateName.find("<") !=Ogre::String::npos )
                {
                    if ( false == mEnableWalkingEffect ||
                        false == _getTemplateEffectName( realEffectTemplate, effectTemplateName, mTerrainHeight ) )
                        continue;
                }
                else
                {
                    realEffectTemplate = effectTemplateName;

                    mTerrainHeight = Ogre::Math::POS_INFINITY;
                }

				String attachPoint = effectInfo->getAttachPoint();

				TransformInfo info;

				// �ȴӰ󶨵��ң����û���ٴӹ�ͷ������
				if ( getLocatorWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
					effectInfo->getOffsetRotation() ) )
				{
				}
				else if ( getBoneWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
					effectInfo->getOffsetRotation() ) )
				{
				}
    
                // ������Ҫ�����߶�����
                if (mTerrainHeight < Ogre::Math::POS_INFINITY)
                {
                    // �����ǰ���µ��λ�õ��ڵ�����Ϣ�߶ȣ�˵������ˮ�У��Ͱ�ˮ����Чλ����Ϊ������Ϣ�߶ȣ�Ҳ����ˮ��߶�
                    if (info.mPosition.y < mTerrainHeight)
                        info.mPosition.y = mTerrainHeight;
                    else
                    {
                        // �����˵ĸ߶ȸ��ڵ�����Ϣ�߶ȣ�˵������ˮ�����ϣ���������ˮ���ϵ����ϣ�������ֱ�Ӱ���Чɾ��
                        EffectManager::getSingleton().removeEffect(effect, false, true);

                        // ������Ч��Ϣ�����Ч����ΪNULL����Ȼ�п��ܻ����Ժ󻹵������effect�������effect�Ѿ���addToFreeListʱ
                        // ���ܱ�delete�ˣ�������VC��R6025����
                        effectInfo->setEffect(NULL);

                        continue;
                    }
                }

				// ��effect�����
				effect->setTransformInfo(info);
			}
		}
	}
	void LogicModel::_createAnimationCameraShake(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationCameraShake(); ++i )
		{
			AnimationCameraShake *cameraShake = mCurrentSkill->getAnimationCameraShake(i);
			assert (cameraShake);

			Ogre::Real time = cameraShake->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				cameraShake->createInstance();
			}
		}

	}
	void LogicModel::_updateAnimationCameraShake(Ogre::Real delta)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationCameraShake(); ++i )
		{
			AnimationCameraShake *cameraShake = mCurrentSkill->getAnimationCameraShake(i);
			assert (cameraShake);

			cameraShake->updateInstance(delta);
			
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createAnimationRibbon(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationRibbons(); ++i )
		{
			AnimationRibbon *ribbon = mCurrentSkill->getAnimationRibbon(i);
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
	void LogicModel::_updateAnimationRibbon(Ogre::Real delta)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationRibbons(); ++i )
		{
			AnimationRibbon *ribbon = mCurrentSkill->getAnimationRibbon(i);
			assert (ribbon);

			String attachPoint = ribbon->getAttachPoint();

			TransformInfo info;

			// �ȴӰ󶨵��ң����û���ٴӹ�ͷ������
			if ( getLocatorWorldTransform( attachPoint, info, ribbon->getOffsetPos(), 
				ribbon->getOffsetRotation() ) )
			{
			}
			else if ( getBoneWorldTransform( attachPoint, info, ribbon->getOffsetPos(), 
				ribbon->getOffsetRotation() ) )
			{
			}

			// ��effect�����
			ribbon->updateRenderInstance(delta, info);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateAnimationSound(Ogre::Real oldTime, Ogre::Real currTime)
	{
		if (mOnPlaySound)
		{
			// sound
			for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationSounds(); ++i )
			{					
				AnimationSound *sound = mCurrentSkill->getAnimationSound(i);
				assert (sound);

				if (false == sound->mPlayed)
				{
					Ogre::Real time = sound->getAttachTime();

					if((currTime >= oldTime && time >= oldTime && time < currTime) || 
						(currTime < oldTime && (time >= oldTime || time < currTime)))
					{
						Ogre::Matrix4 mtx;

						mtx = mModelMainNode->_getFullTransform();

						Ogre::Real pos[3] = { mtx.getTrans().x, mtx.getTrans().y, mtx.getTrans().z };

						sound->mSoundHandle = mOnPlaySound(sound->getSoundName().c_str(), pos, mAnimationLoop);

						sound->mPlayed = true;
					}
				}	
			}
		}
	}

			//-----------------------------------------------------------------------
	void LogicModel::_createAnimationBulletFlow(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationBulletFlows(); ++i )
		{
			AnimationBulletFlow *pAnimationBulletFlow = mCurrentSkill->getAnimationBulletFlow(i);
			assert (pAnimationBulletFlow);

			Ogre::Real time = pAnimationBulletFlow->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				Ogre::String BulletFlowSystemName = pAnimationBulletFlow->getBulletFlowTemplateName();
				BulletFlowSystem *pBulletFlowSystem = BulletFlowSystemManager::getSingleton().getBulletFlowSystemTemplate(BulletFlowSystemName);
				if(pBulletFlowSystem)
				{

					Ogre::String attachPoint = pAnimationBulletFlow->getAttachPoint();
					TransformInfo info;
					// �ȴӰ󶨵��ң����û���ٴӹ�ͷ������
					if ( getLocatorWorldTransform(attachPoint , info, pAnimationBulletFlow->getOffsetPos(), 
						pAnimationBulletFlow->getOffsetRotation() ) )
					{
					}
					else if ( getBoneWorldTransform( attachPoint, info, pAnimationBulletFlow->getOffsetPos(), 
						pAnimationBulletFlow->getOffsetRotation() ) )
					{
					}
					if(mCurrentSkill->m_userData)
					{
						BulletFlowSystemManager::getSingleton().getCreateBulletFlowCallback()->onCreateBulletFlow(BulletFlowSystemName,
							info,mCurrentSkill->m_userData);
					}
					
				}
			}
		}
	}
	void LogicModel::_updateAnimationBulletFlow(Ogre::Real oldTime, Ogre::Real currTime)
	{
		for ( Ogre::ushort i = 0; i < mCurrentSkill->getNumAnimationBulletFlows(); ++i )
		{
			AnimationBulletFlow *pAnimationBulletFlow = mCurrentSkill->getAnimationBulletFlow(i);
			assert (pAnimationBulletFlow);

			Ogre::Real time = pAnimationBulletFlow->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				Ogre::String BulletFlowSystemName = pAnimationBulletFlow->getBulletFlowTemplateName();
				BulletFlowSystem *pBulletFlowSystem = BulletFlowSystemManager::getSingleton().getBulletFlowSystemTemplate(BulletFlowSystemName);
				if(pBulletFlowSystem)
				{
					//pBulletFlowSystem->update(
				}
			}
		}
	}
    //-----------------------------------------------------------------------
    void LogicModel::_updateAnimationLight(Ogre::Real oldTime, Ogre::Real currTime)
    {
        MainSceneLight* light = EffectManager::getSingleton().getMainSceneLight();

        assert (light);

        unsigned short lightInfoNum = mCurrentSkill->getNumAnimationSceneLightInfos();

        // �����ǰ�����߲���������ڣ��Ͳ��ı�ƹ�
        if (lightInfoNum > 0 && false == isCameraVisible())
        {
            return;
        }

        for ( Ogre::ushort i = 0; i < lightInfoNum; ++i )
        {
            AnimationSceneLightInfo *lightInfo = mCurrentSkill->getAnimationSceneLightInfo(i);
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
	//-----------------------------------------------------------------------
	bool LogicModel::attachModel(const Ogre::String& locatorName, LogicModel* model)
	{
		assert (model);

		LocatorMap::iterator itLocator = mLocatorMap.find(locatorName);

		if (itLocator == mLocatorMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
				"LogicModel::attachModel " + mName );

			return false;
		}

		AttachedModelOriginSceneNodes::iterator it = mAttachedModelOriginSceneNodes.find(model);

		if ( it != mAttachedModelOriginSceneNodes.end() )
			it->second = mParentNode;
		else
		{
			std::pair<AttachedModelOriginSceneNodes::iterator, bool> inserted = 
				mAttachedModelOriginSceneNodes.insert( AttachedModelOriginSceneNodes::value_type
				( model, mParentNode ) );

			assert (inserted.second);
		}		

        Ogre::SceneNode* locatorNode = _getLocatorSceneNode(itLocator->second);

		model->setParentSceneNode(locatorNode);

        // ���ҽӵ�modelҪ����λ�ã��������ܱ�֤���ҽӵ�locator��λ����
        model->resetPosition();

        // �����obj�ļ���û�����Χ�еĴ�С���͸���entity������Χ��
        if (NULL == mExternalBoundingBox)
            _updateBoundingBox();
		
        // ���߱��ҽ�����˭attach����
        model->notifyAttach(this);

		return true;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::detachModel( LogicModel* model )
	{
		assert (model);

		AttachedModelOriginSceneNodes::iterator it = mAttachedModelOriginSceneNodes.find(model);

		if ( it == mAttachedModelOriginSceneNodes.end() )
			return false;

		model->setParentSceneNode(it->second);

		mAttachedModelOriginSceneNodes.erase(it);

        // detach�����Թҽ���ΪNULL
        model->notifyAttach(NULL);

		return true;
	}
	//-----------------------------------------------------------------------
	Ogre::ushort LogicModel::getNumBones(void)
	{
		if (mSkeleton)
			return mSkeleton->getNumBones();
		else
			return 0;
	}
	//-----------------------------------------------------------------------
	const Ogre::String& LogicModel::getBoneName( Ogre::ushort index )
	{
		assert (mSkeleton);
		Ogre::Bone *bone = mSkeleton->getBone(index);
		assert (bone);
		return bone->getName();
	}
	const Ogre::Bone* LogicModel::getBone( Ogre::ushort index )
	{
		assert (mSkeleton);
		Ogre::Bone *bone = NULL;
		bone = mSkeleton->getBone(index);
		return bone;
	}
	const Ogre::Bone* LogicModel::getBone( const Ogre::String& name )
	{
		assert (mSkeleton);
		Ogre::Bone *bone = NULL;
		bone = mSkeleton->getBone(name);
		return bone;
	}
	//-----------------------------------------------------------------------
	Ogre::Real LogicModel::getCurrentAnimationLength(void)
	{
		if(mCurrentAnimationState)
		{
			return mCurrentAnimationState->getLength();
		}
		else
			return 0.0f;
	}
	//-----------------------------------------------------------------------
	Ogre::ushort LogicModel::getSkeletonAnimationCount(void)
	{
		if (mSkeleton)
			return mSkeleton->getNumAnimations();
		else
			return 0;
	}
	//-----------------------------------------------------------------------
	Ogre::Animation* LogicModel::getSkeletonAnimation( Ogre::ushort index )
	{
		if (mSkeleton)
			return mSkeleton->getAnimation(index);
		else
			return NULL;
	}
	//-----------------------------------------------------------------------
	void LogicModel::getLocatorNames( Ogre::StringVector& names )
	{
		for ( LocatorMap::iterator i = mLocatorMap.begin();
			i != mLocatorMap.end(); ++i )
		{
			names.push_back(i->first);
		}
	}
	//-----------------------------------------------------------------------
	bool LogicModel::_setEntityMaterial(const Ogre::String& entityEntry, const Ogre::String& matName)
	{
        // �������һ�����ַ�����Ҳreturn true����Ϊ�ͻ�����ж��װ��ʱ���material����""
        if (matName.empty())
            return true;
        else
        {
            EntityMap::iterator itEntity = mEntityMap.find(entityEntry);

            if ( itEntity == mEntityMap.end() )
            {
                Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + entityEntry + "' dosen't exists! " +
                    "LogicModel::_setEntityMaterial " + mName );

                return false;
            }

            EntityValue& tempEntityValue = itEntity->second;

            Ogre::Entity* entity = tempEntityValue.mEntity;
            assert (entity);

            // ���������";"���ָ���
            if (matName.find(";") !=Ogre::String::npos)
            {
                Ogre::StringVector matNames = Ogre::StringUtil::split(matName, ";");

                assert (matName.size() > 1);

                for( Ogre::uint i=0; i<entity->getNumSubEntities(); ++i )
                {
                    Ogre::SubEntity* subEntity = entity->getSubEntity(i);
                    assert (subEntity);

                    // �������Ե�material����ʹ�ã�û�еĻ�ʹ�õ�һ��material
                    Ogre::String subMatName;

                    if (i < matNames.size())
                    {
                        subMatName = matNames[i];
                    }
                    else
                    {
                        subMatName = matNames[0];
                    }

                    // �ȼ���Ƿ����������
                    const Ogre::MaterialPtr subMat = Ogre::MaterialManager::getSingleton().getByName(subMatName);

                    if ( false == subMat.isNull() )
                        subEntity->setMaterialName( subMatName );
                }
            }
            else
            {
                // �ȼ���Ƿ����������
                const Ogre::MaterialPtr entityMat = Ogre::MaterialManager::getSingleton().getByName(matName);

                if ( false == entityMat.isNull() )
                    entity->setMaterialName(matName);
            }

            // ���ò���֮��ҲҪ�ٴε��ã���Ϊentity�еĲ����Ѿ��仯��
            _convertEntityToCharacterMaterial(entity);

            if (tempEntityValue.mEntityColourChanged)
                _setEntityColour(tempEntityValue, tempEntityValue.mEntityColour);

            if (mCurrentTransparency > 0.0f)
            {
                if (tempEntityValue.mTransparentor)
                    delete tempEntityValue.mTransparentor;

                tempEntityValue.mTransparentor = new EntityTransparentor(tempEntityValue.mEntity);
                tempEntityValue.mTransparentor->apply(mCurrentTransparency);
            }


            return true;
        }		
	}
	//-----------------------------------------------------------------------
	void LogicModel::_setEntityMaterials(void)
	{
		for ( MaterialMap::iterator i = mMaterialMap.begin();
			i != mMaterialMap.end(); ++i )
		{
			MaterialValue& tempValue = i->second;			

			_setEntityMaterial(tempValue.mEntityEntry, tempValue.mMaterialName);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createEffects()
	{
		for ( EffectMap::iterator i = mEffectMap.begin();
			i != mEffectMap.end(); ++i )
		{
			EffectValue& tempValue = i->second;

			EffectHandle handle = 0;

            if (tempValue.mTranslateAll)
			    handle = addEffect(tempValue.mEffectName, tempValue.mLocator, GTIT_ALL, tempValue.mEffectColour);
            else
                handle = addEffect(tempValue.mEffectName, tempValue.mLocator, GTIT_POSITION, tempValue.mEffectColour);

			if (handle != 0)
			{
				mEffectHandleMap.insert(EffectHandleMap::value_type(i->first, handle));
			}
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateAllEffects(void)
	{
		CreatedEffectList::iterator i = mCreatedEffectList.begin();

		while ( i != mCreatedEffectList.end() )
		{
			Fairy::Effect* effect = (*i).mEffect;

			if (effect)
			{
				if ( false == effect->isAlive() )
				{
					i = delEffect(i);

					continue;
				}		
				else
				{
					TransformInfo info;

                    const Ogre::String& locatorName = (*i).mLocator;

                    // �ж��Ƿ���ڱ�attach��״̬�£�����ǣ�����locator�ǿ�ת�Ƶģ���Ҫ��ȡ
                    // attachParent��locatorλ���ˣ�������󣬽��¹�Ȧ�����Ч��Ҫת�Ƶ����
                    // ���¹�Ȧ�㣨�ڱ���������������������İ�effect�ҽӵ���Ľ��¹�Ȧ���ϣ�

                    if ( mAttachParent // ����һ��LogicModel��attach
                        && isLocatorTransferable(locatorName) // ��locator�ǿ���ת�Ƶ�
                        && mAttachParent->isLocatorTransferable(locatorName) // attachParent�������locator������Ҳ�ǿ�ת�Ƶ� 
                        )
                    {
                        // ��ȡattachParent��locator��λ��
                        mAttachParent->getLocatorWorldTransform( locatorName, info, Ogre::Vector3::ZERO,
                        Ogre::Quaternion::IDENTITY, (*i).mTransformType );
                    }
                    else
                    {
                        // ��ȡ�����locatorλ��
                        getLocatorWorldTransform( locatorName, info, Ogre::Vector3::ZERO,
						    Ogre::Quaternion::IDENTITY, (*i).mTransformType );	
                    }

					// ��effect�����
					effect->setTransformInfo(info);
				}			
			}

			++i;
		}		
	}
	//-----------------------------------------------------------------------
	void LogicModel::setPosition(Ogre::Vector3& pos)
	{
		mModelPosition = pos;
		mModelMainNode->setPosition(mModelPosition);
		mMainNodeNeedUpdate = true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::setOrientation(Ogre::Quaternion& orientation)
	{
		mModelOrientation = orientation;
		mModelMainNode->setOrientation(mModelOrientation);
		mMainNodeNeedUpdate = true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateBoundingBox(void)
	{
        if (!mFullBoundingBox)
            mFullBoundingBox = new Ogre::AxisAlignedBox;

        mFullBoundingBox->setNull();

        // ������ǰ���е�entity
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {
            Ogre::Entity* workingEntity = i->second.mEntity;

            if (workingEntity)
            {
                mFullBoundingBox->merge( workingEntity->getBoundingBox() );
            }
        }

        // �������е�attach model
        AttachedModelOriginSceneNodes::const_iterator it = 
            mAttachedModelOriginSceneNodes.begin();

        while (it != mAttachedModelOriginSceneNodes.end())
        {
            if (it->first)
                mFullBoundingBox->merge( it->first->_getFullBoundingBox() );

            ++it;
        }		
	}
	//-----------------------------------------------------------------------
	void LogicModel::calXAxisRotation(float deltaX, float deltaZ, float deltaHeight)
	{
		float deltaLength = Ogre::Math::Sqrt( deltaZ * deltaZ + deltaX * deltaX );
		float angle = deltaHeight / deltaLength;
		Ogre::Radian radian = Ogre::Math::ATan( angle );

		Ogre::Vector3 axis = mModelMainNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_X;
		mFrontBackOrientation.FromAngleAxis( radian, axis );
	}
	//-----------------------------------------------------------------------
	void LogicModel::calZAxisRotation(float deltaX, float deltaZ, float deltaHeight)
	{
		float deltaLength = Ogre::Math::Sqrt( deltaZ * deltaZ + deltaX * deltaX );
		float angle = deltaHeight / deltaLength;
		Ogre::Radian radian = Ogre::Math::ATan( angle );

		Ogre::Vector3 axis = mModelMainNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		mLeftRightOrientation.FromAngleAxis( radian, axis );
	}
	//-----------------------------------------------------------------------
	void LogicModel::_rotate(void)
	{
		if (mModelMainNode)
		{
#define MAX_DELTA_HEIGHT 550.0f

			if (mMovementType == MT_ROTATE_X_AXIS ||
				mMovementType == MT_ROTATE_X_Z_AXIS)
			{
				TransformInfo frontPointInfo;

				bool succeedFront = getLocatorWorldTransform( "FrontPoint", frontPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				TransformInfo backPointInfo;

				bool succeedBack = getLocatorWorldTransform( "BackPoint", backPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				if (succeedFront && succeedBack)
				{
					// ��ȡ�ĸ���ĵ��θ߶�
					float lfy = 0.0f;
                    _getWorldHeight(frontPointInfo.mPosition.x, frontPointInfo.mPosition.z, lfy);
					float lby = 0.0f;
                    _getWorldHeight(backPointInfo.mPosition.x, backPointInfo.mPosition.z, lby);

					float deltaHeight = lfy-lby;

                    // ���ǰ�����̫�󣬾�ֱ������
                    if (Ogre::Math::Abs(deltaHeight) > MAX_DELTA_HEIGHT)
                    {
                        Ogre::LogManager::getSingleton().logMessage("_ratate() Larged than MAX_DELTA_HEIGHT : "
                            + Ogre::StringConverter::toString(deltaHeight));
                        return;
                    }

					float dz = frontPointInfo.mPosition.z - backPointInfo.mPosition.z;
					float dx = frontPointInfo.mPosition.x - backPointInfo.mPosition.x;
					calXAxisRotation(dx, dz, deltaHeight);
				}	
			}

			if (mMovementType == MT_ROTATE_X_Z_AXIS)
			{
				TransformInfo leftPointInfo;

				bool succeedLeft = getLocatorWorldTransform( "LeftPoint", leftPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				TransformInfo rightPointInfo;

				bool succeedRight = getLocatorWorldTransform( "RightPoint", rightPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				if (succeedLeft && succeedRight)
				{
					// ��ȡ�ĸ���ĵ��θ߶�
					float lfy = 0.0f;
                    _getWorldHeight(leftPointInfo.mPosition.x, leftPointInfo.mPosition.z, lfy);
					float lby = 0.0f;
                    _getWorldHeight(rightPointInfo.mPosition.x, rightPointInfo.mPosition.z, lby);

					float deltaHeight = lfy-lby;

                    // ������ҵ���̫�󣬾�ֱ������
                    if (Ogre::Math::Abs(deltaHeight) > MAX_DELTA_HEIGHT)
                        return;

					float dz = leftPointInfo.mPosition.z - rightPointInfo.mPosition.z;
					float dx = leftPointInfo.mPosition.x - rightPointInfo.mPosition.x;
					calZAxisRotation(dx, dz, deltaHeight);
				}	
			}			

		//	Ogre::Quaternion initOri = mModelMainNode->getInitialOrientation();
			mModelMainNode->setOrientation(mLeftRightOrientation * mFrontBackOrientation * mModelOrientation);
		}	
	}
	//-----------------------------------------------------------------------
	void LogicModel::_setMovementType( const Ogre::String& value )
	{
		if ( value == "Normal" )
			mMovementType = MT_NORMAL;
		else if ( value == "RotateX" )
			mMovementType = MT_ROTATE_X_AXIS;
		else if ( value == "RotateXZ" )
			mMovementType = MT_ROTATE_X_Z_AXIS;
		else
		{
			Ogre::LogManager::getSingleton().logMessage( "Wrong Movement Type '" + value + "' ! " +
				"LogicModel::_setMovementType " + mName );
		}
	}
    //-----------------------------------------------------------------------
    void LogicModel::resetPosition(void)
    {
        mModelPosition = Ogre::Vector3::ZERO;
        mModelOrientation = Ogre::Quaternion::IDENTITY;

        mModelMainNode->setPosition(mModelPosition);
        mModelMainNode->setOrientation(mModelOrientation);
    }
    //-----------------------------------------------------------------------
    void LogicModel::setVisible(bool visible)
    { 
        if (mModelMainNode)
        {
            mModelMainNode->setVisible(visible);

            Ogre::Node* parentNode = mModelMainNode->getParent();

            // ��ǰҪת�ɿɼ�
            if (visible)
            {
                // ֮ǰremovechild�ˣ�Ҳ����˵��ʱmVisibleΪfalse
                if (parentNode == NULL)
                {
                    mParentNode->addChild(mModelMainNode);
                }
                else
                {
                    if (mParentNode != parentNode)
                    {
                        Ogre::LogManager::getSingleton().logMessage("setVisible error 1");
                    }
                }
            }
            else
            {
                if (parentNode != NULL)
                {
                    if (mParentNode != parentNode)
                    {
                        Ogre::LogManager::getSingleton().logMessage("setVisible error 2");
                    }

                    parentNode->removeChild(mModelMainNode);
                }
                else
                {
                    if (mVisible != visible)
                    {
                        Ogre::LogManager::getSingleton().logMessage("setVisible error 3");
                    }
                }
            }
        }

        mVisible = visible;

        // ͨ��addEffect��ӵ���ЧҲҪ����
        CreatedEffectList::iterator i = mCreatedEffectList.begin();

        while ( i != mCreatedEffectList.end() )
        {
            Fairy::Effect* effect = (*i).mEffect;

            if (effect)
            {
                effect->setVisible(visible);
            }

            ++i;
        }

        // locator�ϵ�modelҲҪ����
        for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
            itLocator != mLocatorMap.end(); ++itLocator )
        {
            LocatorValue& tempValue = itLocator->second;

            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (tempSlot.mModel)
                    tempSlot.mModel->setVisible(visible);
            }
        }

        // ��������������ˣ�����ǰ��ִ�е�skillҲҪɾ������Ȼ���ڳ���������skill����Ч
        if (!visible)
		{
			delCurrentSkill();

			// ����projectorֻ�в���ʾ��ʱ�򣬲�����
			for ( ModelProjectors::iterator i = mModelProjectors.begin();
				i != mModelProjectors.end(); ++i )
			{
				(*i)->mProjectorEffect->setVisible(visible);
			}
		}
    }
    //-----------------------------------------------------------------------
    void LogicModel::addProjector(ModelProjectorType type, const Ogre::String& projectorEffectName,
        bool show, float projectorSize)
    {
        assert (false == projectorEffectName.empty());

        ModelProjector* modelProjector = NULL;

        // ������
        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            if ((*i)->mType == type)
            {
                modelProjector = *i;
                break;
            }
        }

        // �����ǰ���������������͵�projector
        if (modelProjector)
        {
            // �����Ч���Ʋ�һ�����͸�����Ч
            if (projectorEffectName != modelProjector->mProjectorName)
            {
                EffectManager::getSingleton().removeEffect(modelProjector->mProjectorEffect, false);

                modelProjector->mProjectorEffect = EffectManager::getSingleton().createEffect(projectorEffectName);

                /** projector���ڲ�ֱ�ӹ���mMainNode�ϣ����Ǵ����Լ���һ���ڵ㣬Ȼ����execute�е���
                    _updateModelProjectors��������Щ�ڵ��λ�ã�������Ϊ�˲���main node��Ӱ�죬����
                    LogicModel::setVisible�ж�main node����setVisible������Ӱ�쵽���projector�ڵ�
                */
                modelProjector->mProjectorNode = modelProjector->mProjectorEffect->createSceneNode();
          //      modelProjector->mProjectorEffect->getBasicNode()->setInheritOrientation(false);

                for (unsigned short i=0; i<modelProjector->mProjectorEffect->getNumElements(); ++i)
                {
                    EffectElement *element = modelProjector->mProjectorEffect->getElement(i);

                    if (element->getType() == "Projector")
                        static_cast<ProjectorElement*>
                        (element)->setProjectionSize(projectorSize);
                }


                modelProjector->mProjectorName = projectorEffectName;

                assert (modelProjector->mProjectorEffect);
            }
        }
        else
        {
            modelProjector = new ModelProjector(projectorEffectName, type);
            modelProjector->mProjectorEffect = EffectManager::getSingleton().createEffect(projectorEffectName);
            modelProjector->mProjectorNode = modelProjector->mProjectorEffect->createSceneNode();

    //        modelProjector->mProjectorEffect->createSceneNode(mModelMainNode);
    //        modelProjector->mProjectorEffect->getBasicNode()->setInheritOrientation(false);

            for (unsigned short i=0; i<modelProjector->mProjectorEffect->getNumElements(); ++i)
            {
                EffectElement *element = modelProjector->mProjectorEffect->getElement(i);

                if (element->getType() == "Projector")
                    static_cast<ProjectorElement*>
                    (element)->setProjectionSize(projectorSize);
            }

            assert (modelProjector->mProjectorEffect);

            mModelProjectors.push_back(modelProjector);
        }

        assert (modelProjector);

        modelProjector->mProjectorEffect->setVisible(show && mVisible);
    }
    //-----------------------------------------------------------------------
    bool LogicModel::removeProjector(ModelProjectorType type, const Ogre::String& projectorEffectName)
    {
        ModelProjector* modelProjector = NULL;

        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            if ((*i)->mType == type && (*i)->mProjectorName == projectorEffectName)
            {
                EffectManager::getSingleton().removeEffect((*i)->mProjectorEffect, false);

                delete *i;
                *i = NULL;

                mModelProjectors.erase(i);

                return true;
            }
        }

        return false;
    }

	void LogicModel::addAreaEffect(const Ogre::String& templateName)
	{
		float height;
		Ogre::String realEffectTemplate;
		if ( false == mEnableWalkingEffect)
			return;

		bool hasEffectInfo = _getTemplateEffectName( realEffectTemplate, templateName, height );

		//Fairy::Effect* targetEffect = NULL;
		//Ogre::SceneNode* targetNode = NULL;
        for ( AreaEffectMap::iterator i = mAreaEffects.begin();
            i != mAreaEffects.end(); ++i )
        {
			AreaEffect& areaEffect = *i;
			if (areaEffect.mTemplateName == templateName)
			{
				if (areaEffect.mEffect)
				{
					if (areaEffect.mEffect->getTemplateName() == realEffectTemplate)
					{
						return;
					}
					else
					{
						EffectManager::getSingleton().removeEffect(areaEffect.mEffect,false);
						if (hasEffectInfo)
						{
							areaEffect.mEffect = EffectManager::getSingleton().createEffect(realEffectTemplate);
							areaEffect.mBaseNode = areaEffect.mEffect->createSceneNode();
						}
						else
						{
							areaEffect.mEffect = NULL;
							areaEffect.mBaseNode = NULL;
						}					
						return;
					}
				}
				else
				{
					if (hasEffectInfo)
					{
						Fairy::Effect* targetEffect = EffectManager::getSingleton().createEffect(realEffectTemplate);
						Ogre::SceneNode* targetNode = targetEffect->createSceneNode();    
					}	
					return;			
				}
			}			
		}

        // �����ǰ����û���������͵�Ч��
		if (!hasEffectInfo)
		{
			AreaEffect newAreaEffect(NULL, templateName, NULL);
			mAreaEffects.push_back(newAreaEffect); 	
		}
		else
		{
			Fairy::Effect* targetEffect = EffectManager::getSingleton().createEffect(realEffectTemplate);
			Ogre::SceneNode* targetNode = targetEffect->createSceneNode();          

			assert (targetEffect);
			assert (targetNode);
		
			//Ogre::Vector3 basePos = targetNode->getPosition();
			//targetNode->setPosition(basePos.x, height, basePos.z);

			AreaEffect newAreaEffect(targetEffect, templateName, targetNode);
			mAreaEffects.push_back(newAreaEffect); 	
		}		
	}
	bool LogicModel::removeAreaEffect(const Ogre::String& templateName)
	{
		for ( AreaEffectMap::iterator i = mAreaEffects.begin();
			i != mAreaEffects.end(); ++i )
		{
			AreaEffect areaEffect = *i;
			if (areaEffect.mTemplateName == templateName)
			{
				if (areaEffect.mEffect)
				{
					EffectManager::getSingleton().removeEffect(areaEffect.mEffect,false);
				}			
				mAreaEffects.erase(i);
				return true;
			}
		}

		return false;
	}
    //-----------------------------------------------------------------------
    void LogicModel::_delAllProjector(void)
    {
        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            EffectManager::getSingleton().removeEffect((*i)->mProjectorEffect, false);

            delete *i;
            *i = NULL;
        }

        mModelProjectors.clear();
    }

	void LogicModel::_delAllAreaEffect(void)
	{
		for ( AreaEffectMap::iterator i = mAreaEffects.begin();
			i != mAreaEffects.end(); ++i )
		{
			AreaEffect areaEffect = *i;
			if (areaEffect.mEffect)
			{
				EffectManager::getSingleton().removeEffect(areaEffect.mEffect,false);
			}			
		}

		mAreaEffects.clear();
	}
    //-----------------------------------------------------------------------
    void LogicModel::_updateModelProjectors(void)
    {
        // ���µ�ǰ�����������ϵ�projector��λ��
        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            Ogre::SceneNode* projectorNode = (*i)->mProjectorNode;
            if (projectorNode)
                // �����model��ǰ�������������õ�projector��
                projectorNode->setPosition(mModelMainNode->_getDerivedPosition());
        }
    }

	void LogicModel::_updateAreaEffects(void)
	{
		for ( AreaEffectMap::iterator i = mAreaEffects.begin();
			i != mAreaEffects.end(); ++i )
		{
			AreaEffect& areaEffect = *i;			
			
			if(	!mEnableWalkingEffect||!mVisible||mAttachParent)
			{
				if (areaEffect.mEffect)
				{
					areaEffect.mEffect->setVisible(false);
				}
				continue;
			}
			
			float height;
			Ogre::String realEffectTemplate;
			if(false == _getTemplateEffectName( realEffectTemplate, areaEffect.mTemplateName, height ))
			{
				if (areaEffect.mEffect)
				{
					areaEffect.mEffect->setVisible(false);
				}
				continue;
			}			
			
			if(areaEffect.mEffect && (realEffectTemplate != areaEffect.mEffect->getTemplateName()))
			{
				// update the effect automatically
				EffectManager::getSingleton().removeEffect(areaEffect.mEffect,false);
		
				areaEffect.mEffect = EffectManager::getSingleton().createEffect(realEffectTemplate);
				areaEffect.mBaseNode = areaEffect.mEffect->createSceneNode();
			}
			else if (!areaEffect.mEffect)
			{
				areaEffect.mEffect = EffectManager::getSingleton().createEffect(realEffectTemplate);
				areaEffect.mBaseNode = areaEffect.mEffect->createSceneNode();
			}

			// Update the pos and rot of area effects
			Ogre::Vector3 mainPos = mModelMainNode->_getDerivedPosition();

			TransformInfo info;				
			info.mRotation = mModelMainNode->_getDerivedOrientation();

			if (height >= Ogre::Math::POS_INFINITY)
				height = mainPos.y;

			info.mPosition = Ogre::Vector3(mainPos.x , height , mainPos.z);
			areaEffect.mEffect->setTransformInfo(info);
			
			areaEffect.mEffect->setVisible(true);
		}
	}

    //-----------------------------------------------------------------------
    void LogicModel::setTransparent(Ogre::Real time, Ogre::Real transparency)
    {
        // ���ʱ��Ϊ0������������͸����
        if ( Ogre::Math::RealEqual(time,0.0f) )
        {
            mCurrentTransparency = mDestTransparency = transparency;
            _updateAllEntitiesTransparency();
        }
        else
        {
            // Clamp transparency to range [0, 1]
            if (transparency < 0)
                transparency = 0;
            else if (transparency > 1)
                transparency = 1;

            if (false == Ogre::Math::RealEqual(mDestTransparency,transparency))
            {              
                mDestTransparency = transparency;
                mTransparencyTime = time;
                mNeedUpdateTransparency = true;
                mDeltaTransparency = mDestTransparency - mCurrentTransparency;

                // updateTransparency();
            }
        }
        // locator�ϵ�modelҲҪ����
        for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
            itLocator != mLocatorMap.end(); ++itLocator )
        {
            LocatorValue& tempValue = itLocator->second;

            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (tempSlot.mModel)
                    tempSlot.mModel->setTransparent(time, transparency);
            }
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::updateTransparency(Ogre::Real deltaTime)
    {
        assert (mTransparencyTime > 0.0f);

        Ogre::Real transparencyInc = deltaTime / mTransparencyTime * mDeltaTransparency;

        mCurrentTransparency += transparencyInc;        

        if ( (mDeltaTransparency >= 0.0f) && (mCurrentTransparency >= mDestTransparency) ||
            (mDeltaTransparency < 0.0f) && (mCurrentTransparency < mDestTransparency) )
        {
            mNeedUpdateTransparency = false;
            mCurrentTransparency = mDestTransparency;
        }

        _updateAllEntitiesTransparency();
    }
    //-----------------------------------------------------------------------
    bool LogicModel::_createModelInSlot(SlotValue& slotValue, LocatorValue& locatorValue)
    {
        if (slotValue.mModelName.empty())
            return false;

        LogicModel* tempModel = LogicModelManager::getSingleton().createLogicModel(slotValue.mModelName);

        if (tempModel)
        {
            // ɾ��ԭ���ҵ�model
            if (slotValue.mModel)
                LogicModelManager::getSingleton().destroyLogicModel(slotValue.mModel);

            slotValue.mModel = tempModel;

            Ogre::SceneNode* locatorNode = _getLocatorSceneNode(locatorValue);

            tempModel->setParentSceneNode( locatorNode );
            tempModel->setChildModel(true);

            tempModel->setTransparencyInfo(mCurrentTransparency, mDestTransparency,
                Ogre::Math::Abs(mDestTransparency - mCurrentTransparency) * mTransparencyTime);

            // ����Ϊ�ҽ��ϵ������visible flag
            tempModel->setVisibleFlag(mVisibleFlag);
            tempModel->setVisible(mVisible);

            tempModel->setScaleFactor(mExternalScaleFactor);

            return true;
        }

        return false;
    }
    //-----------------------------------------------------------------------
    void LogicModel::_dealWithExtraAttributes(const Ogre::String& name, const Ogre::String& value)
    {
        // �����������͵�����
        if (name == MOVEMENT_TYPE_ATTRIBUTE)
        {
            // �����ƶ�����
            _setMovementType(value);
        }
        else if (name == DEFAULT_ANIM_NAME)
        {
            _setDefaultAnimationName(value);
        }
        else if (name == BOUNDING_BOX)
        {
            mExternalBoundingBoxValue = value;
           // _createBoundingBoxFromExternal(value);
        }
        else if (name == SCALE_FACTOR)
        {
            _setScaleFactor(value);
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::setHairColour(const Ogre::ColourValue& colour)
    {
        EntityMap::iterator i = mEntityMap.find(HAIR_MESH_NAME);

        if (i != mEntityMap.end())
        {
            // ��ȡͷ��entity
            Ogre::Entity* hairEntity = i->second.mEntity;

            if (hairEntity)
            {
                // ������ɫ����
                mHairColour = colour;

                _setEntityColour(i->second, colour);
            }
        }
    }
    //-----------------------------------------------------------------------
    Ogre::Entity* LogicModel::_getEntityByName(const Ogre::String& name)
    {
        EntityMap::iterator i = mEntityMap.find(name);

        if ( i != mEntityMap.end() )
        {
            return i->second.mEntity;
        }
        else
            return NULL;
    }
    //-----------------------------------------------------------------------
    void LogicModel::setShadowCastable(bool castable)
    {
        if (mShadowCastable != castable)
        {
            mShadowCastable = castable;

            // �Ե�ǰ���е�entity����������
            for ( EntityMap::iterator i = mEntityMap.begin();
                i != mEntityMap.end(); ++i )
            {                
                const Ogre::String& meshName = i->second.mMeshName;

                // �����ǰ���mesh�ǹ涨Ϊ����ͶӰ�ģ���������һ��
                if ( isShadowUncastable(meshName) )
                    continue;

                Ogre::Entity* workingEntity = i->second.mEntity;
                if (workingEntity)
                {
                    workingEntity->setCastShadows(mShadowCastable);
                }
            }
        }        
    }
    //-----------------------------------------------------------------------
    void LogicModel::_createSkeletonEntity(const Ogre::SkeletonPtr& skeleton)
    {
        mSkeletonEntity = LogicModelManager::getSingleton()._createSkeletalEntity(skeleton);

        // ����skeleton entityҲ��Ҫ�����ⲿ���ݣ���Ϊ��Ҳ������ѯ
     //   mSkeletonEntity->setUserAny(getUserAny());
        mSkeletonEntity->setQueryFlags(0);

        mModelMainNode->attachObject(mSkeletonEntity);
    }
    //-----------------------------------------------------------------------
    void LogicModel::_delSkeletonEntity(void)
    {
        if (mSkeletonEntity)
        {
            // Detach from parent scene node
            Ogre::SceneNode* sn = mSkeletonEntity->getParentSceneNode();
            if (sn && sn == mSkeletonEntity->getParentNode())
            {
                sn->detachObject(mSkeletonEntity);
            }

            // The manager will take care with skeletal entity caching
            LogicModelManager::getSingleton()._destroySkeletalEntity(mSkeletonEntity);
            mSkeletonEntity = 0;
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::setSelected(bool selected)
    {
        if (mSelected != selected)
        {
            mSelected = selected;

            if (mSelected)
                setBodyColour(Ogre::ColourValue::White);
            else
                resetBodyColour();
        }        
    }
    //-----------------------------------------------------------------------
    void LogicModel::setBodyColour( const Ogre::ColourValue& colour )
    {
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {  
            // ���ı�ͷ������ɫ
            if (i->first == HAIR_MESH_NAME)
                continue;

            EntityValue& tempValue = i->second;

            _setEntityColour(tempValue, colour);
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::_setEntityColour( EntityValue& entityValue, const Ogre::ColourValue& entityColour )
    {
        entityValue.mEntityColour = entityColour;
        entityValue.mEntityColourChanged = true;

        Ogre::Entity* workingEntity = entityValue.mEntity;

        if (workingEntity)
        {
            for (unsigned int subEntityI = 0;
                subEntityI < workingEntity->getNumSubEntities(); ++subEntityI)
            {
                Ogre::SubEntity* subEntity = workingEntity->getSubEntity(subEntityI);

                assert (subEntity);

                if (entityValue.mClonedMaterials.size() < subEntityI + 1)
                {
                    const Ogre::MaterialPtr& originMat = subEntity->getMaterial();

                    // ������ǰ����ɫֵ
                    entityValue.mEntityOriginColour = _getColourFromMaterial(originMat);

                    Ogre::String newName;

                    // Avoid name collision
                    do {
                        newName = originMat->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
                    } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

                    Ogre::MaterialPtr newMat = originMat->clone(newName);

                    newMat->setSelfIllumination(entityValue.mEntityColour);

                    // ��subEntity���ò���
                    subEntity->setMaterialName(newName);

                    // ��cloneһ��originMat����ΪmOrigin��������entity�е�ԭ���ʾ���Զ���ᱻ�Ķ�
                    do {
                        newName = originMat->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
                    } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

                    Ogre::MaterialPtr oldMat = originMat->clone(newName);

                    // ������ʱ�������飬����������
                    entityValue.mClonedMaterials.push_back(newMat);
                    entityValue.mOriginMaterialNames.push_back(oldMat->getName());
                }
                else
                {
                    Ogre::MaterialPtr originMat = subEntity->getMaterial();

                    originMat->setSelfIllumination(entityValue.mEntityColour);
                }                    
            }

            // ����͸�����е�origin material
            EntityTransparentor* transparentor = entityValue.mTransparentor;
         
            if (transparentor)
            {
                // ����origin material
                for (unsigned int subEntityIndex = 0; subEntityIndex < workingEntity->getNumSubEntities();
                    ++subEntityIndex)
                {
                    Ogre::SubEntity* pSubEntity = workingEntity->getSubEntity(subEntityIndex);

                    assert (pSubEntity);

                    ModelMaterialInstance* materialInstance = transparentor->mMaterialInstances[transparentor->mSubEntityMaterialIndices[subEntityIndex]];

                    if (materialInstance)
                    {
                //        const Ogre::String& subEntityMatName = pSubEntity->getMaterialName();

                //        const Ogre::String& transparentorMatName = materialInstance->mOrigin->getName();

                        materialInstance->mDerived->setSelfIllumination(entityValue.mEntityColour);
                        materialInstance->mOrigin->setSelfIllumination(entityValue.mEntityColour);

                        /*if (subEntityMatName != transparentorMatName)
                        {
                            const Ogre::MaterialPtr& subEntityMat = pSubEntity->getMaterial();
                            materialInstance->mOrigin = subEntityMat;
                        }*/
                    }
                }
            }
        }

    }
    //-----------------------------------------------------------------------
    void LogicModel::_clearEntityMaterials( EntityValue& entityValue )
    {
        // ɾ��clone����material
        for (size_t cloneMatI = 0; cloneMatI < entityValue.mClonedMaterials.size();
            ++cloneMatI)
        {
            std::vector< Ogre::MaterialPtr >& materials = entityValue.mClonedMaterials;
            Ogre::MaterialPtr& mat = materials[cloneMatI];

            if (false == mat.isNull())
            {
                Ogre::MaterialManager::getSingleton().remove(mat->getHandle());
                mat.setNull();
            }
        }
        entityValue.mClonedMaterials.clear();

        for (size_t cloneMatI = 0; cloneMatI < entityValue.mOriginMaterialNames.size();
            ++cloneMatI)
        {
            Ogre::MaterialManager::getSingleton().remove(entityValue.mOriginMaterialNames[cloneMatI]);
        }
        entityValue.mOriginMaterialNames.clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::resetBodyColour(void)
    {
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {  
            // ���ı�ͷ������ɫ
            if (i->first == HAIR_MESH_NAME)
                continue;

            EntityValue& tempValue = i->second;

            tempValue.mEntityColour = Ogre::ColourValue::Black;
            tempValue.mEntityColourChanged = false;

            _setEntityColour(tempValue, tempValue.mEntityOriginColour);
        //    _resetEntityMaterial(tempValue);
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::_resetEntityMaterial( EntityValue& entityValue )
    {
        Ogre::Entity* workingEntity = entityValue.mEntity;

        if (workingEntity)
        {
            for (unsigned int subEntityI = 0;
                subEntityI < workingEntity->getNumSubEntities(); ++subEntityI)
            {
                Ogre::SubEntity* subEntity = workingEntity->getSubEntity(subEntityI);

                assert (subEntity);

                subEntity->setMaterialName( entityValue.mOriginMaterialNames[subEntityI] );                
            }
        }

        _clearEntityMaterials(entityValue);
    }
    //-----------------------------------------------------------------------
    void LogicModel::_updateAllEntitiesTransparency(void)
    {
        // ���������entity
        for ( EntityMap::iterator itEntity = mEntityMap.begin();
            itEntity != mEntityMap.end(); ++itEntity )
        {
            Ogre::Entity* pEntity = itEntity->second.mEntity;
            EntityTransparentor* transparentor = itEntity->second.mTransparentor;

            if (pEntity)
            {
                if (transparentor)
                {
                    transparentor->apply(mCurrentTransparency);

         //           if (mCurrentTransparency <= 0)
         //           {
         //               delete itEntity->second.mTransparentor;
         //               itEntity->second.mTransparentor = 0;
         //           }
                }
            //    else if (mDestTransparency > 0)
                else
                {
                    itEntity->second.mTransparentor = new EntityTransparentor(pEntity);

                    itEntity->second.mTransparentor->apply(mCurrentTransparency);
                }
            }           
        }

        // ���µ�ǰ���еĲ��ϵ�͸����
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.begin();

        while ( fabricIterator != mFabricInfoMap.end() )
        {
            Ogre::Fabric* fabric = fabricIterator->second.mFabric;
            FabricTransparentor* transparentor = fabricIterator->second.mTransparentor;

            if (fabric)
            {
                if (transparentor)
                {
                    transparentor->apply(mCurrentTransparency);
                }
                else
                {
                    fabricIterator->second.mTransparentor = new FabricTransparentor(fabric);
                    fabricIterator->second.mTransparentor->apply(mCurrentTransparency);
                }
            }  

            ++fabricIterator;
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::setTransparencyInfo(Ogre::Real currTransparency, Ogre::Real destTransparency, Ogre::Real transparentTime)
    {
        if ( mCurrentTransparency != currTransparency ||
            mDestTransparency != destTransparency )
        {
            mCurrentTransparency = currTransparency;
            mDestTransparency = destTransparency;

            if (Ogre::Math::RealEqual(transparentTime, 0.0f))
                _updateAllEntitiesTransparency();
            else
            {
                mTransparencyTime = transparentTime;
                // ������Ҫ�����flag����Ϊtrue����Ȼ��execute�в�update transparency
                mNeedUpdateTransparency = true;

                mDeltaTransparency = mDestTransparency - mCurrentTransparency;
            }
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::_createBoundingBoxFromExternal(void)
    {
        Ogre::StringVector values = Ogre::StringUtil::split(mExternalBoundingBoxValue, " ");

        if (values.size() != 3)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "the bounding box parameter count must be 3!",
                "LogicModel::_createBoundingBoxFromExternal"); 

            return;
        }

        Ogre::Real x = Ogre::StringConverter::parseReal(values[0]);
        Ogre::Real y = Ogre::StringConverter::parseReal(values[1]);
        Ogre::Real z = Ogre::StringConverter::parseReal(values[2]);

        Ogre::Vector3 minPoint;
        minPoint.x = -x / 2;
        minPoint.z = -z / 2;
        minPoint.y = 0.0f;

        Ogre::Vector3 maxPoint;
        maxPoint.x = x / 2;
        maxPoint.z = z / 2;
        maxPoint.y = y;

        mExternalBoundingBox = new Ogre::AxisAlignedBox(minPoint, maxPoint);
    }
    //-----------------------------------------------------------------------
    bool LogicModel::rayIntersect(const Ogre::Ray& ray)
    {
		// Add additional translation to bounding box
		// �Ƿ���Ҫƫ���д�ȷ������ע��
	/*	Ogre::Vector3 additionPos = Ogre::Vector3::ZERO;
		Ogre::Matrix4 addTrans = Ogre::Matrix4::IDENTITY;
		if (mSkeletonEntity && mSkeleton)
		{
			Ogre::Bone* rootBone = mSkeleton->getRootBone();
			if (rootBone)
			{
				additionPos = rootBone->getPosition()-rootBone->getInitialPosition();
				addTrans.setTrans(additionPos);
			}
		}		*/

        if (mExternalBoundingBox)
        {
            // �õ�ǰscene node��transform��ǰָ����boundingbox����Ϊ��ǰ��boundingbox���Ǿֲ�����ϵ�ģ�
            Ogre::AxisAlignedBox intersectedBox(mExternalBoundingBox->getMinimum(), mExternalBoundingBox->getMaximum());

            //intersectedBox.transformAffine(mModelMainNode->_getFullTransform()*addTrans);
			intersectedBox.transformAffine(mModelMainNode->_getFullTransform());

            // ��ת�����boundingbox��ray���м��
            return Ogre::Math::intersects(ray, intersectedBox).first;
        }
        else if (mFullBoundingBox)
        {
            // �õ�ǰscene node��transform��ǰָ����boundingbox����Ϊ��ǰ��boundingbox���Ǿֲ�����ϵ�ģ�
            Ogre::AxisAlignedBox intersectedBox(mFullBoundingBox->getMinimum(), mFullBoundingBox->getMaximum());

            //intersectedBox.transformAffine(mModelMainNode->_getFullTransform()*addTrans);
			intersectedBox.transformAffine(mModelMainNode->_getFullTransform());

            // ��ת�����boundingbox��ray���м��
            return Ogre::Math::intersects(ray, intersectedBox).first;
        }        
        else
            return false;
    }
    //-----------------------------------------------------------------------
    void LogicModel::_setScaleFactor(const Ogre::String& value)
    {
        Ogre::StringVector values = Ogre::StringUtil::split(value, " ");

        if (values.size() != 3)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "the scale factor parameter count must be 3!",
                "LogicModel::_setScaleFactor"); 

            return;
        }

        setScaleFactor(Ogre::StringConverter::parseVector3(value));
    }
    //-----------------------------------------------------------------------
    bool LogicModel::_getTemplateEffectName(Ogre::String& effectName, const Ogre::String& templateName, float& height)
    {
        Fairy::TerrainTypeInfos* typeInfos = mSystem->getTerrainTypeInfos();
        if (!typeInfos)
            return false;

        Ogre::Vector3 modelPos = mModelMainNode->_getDerivedPosition();

        typeInfos->getEffectInfo(modelPos.x, modelPos.z, templateName, effectName, height);

        if (effectName.empty())
            return false;
        else
            return true;
    }
    //-----------------------------------------------------------------------
    Ogre::AnimationState* LogicModel::_getAnimationState(const Ogre::String& animName)
    {
        // ���жϵ�ǰskeleton��û�����animation
        // ���ﲻ���ж�mSkeletonEntity->getAllAnimationStates()��Ϊ�գ���Ϊ�ܽ���
        // ���������˵��mSkeletonEntity��Ϊ�գ����Կ϶��Ǵ�������
        if (!mSkeletonEntity->getAllAnimationStates()->hasAnimationState(animName))
        {
            if ( false == mSkeletonEntity->getSkeleton()->hasAnimation(animName) )        
            {
                // ���û�У��ʹ�animation skeleton link�л�ȡҪ��ȡ��skeleton������
                mSkeletonEntity->getSkeleton()->addLinkedSkeletonAnimationSource(
                    LogicModelManager::getSingleton().getSkeletonFromAnimationName(mName, animName) );
            }
            mSkeletonEntity->refreshAvailableAnimationState();
        }

        return mSkeletonEntity->getAnimationState(animName);
    }
    //---------------------------------------------------------------------
    void LogicModel::_getWorldHeight(float x, float z, float& y)
    {
        try
        {
            mGetHeightInWorld(x,z,y);
        }
        catch (...)
        {
            Ogre::StringUtil::StrStreamType errorString;

            errorString << "Get height in world callback exception!\n";
            errorString << "Model Name : " << mName << '\n';
            errorString << "x : " << Ogre::StringConverter::toString(x) << '\n';
            errorString << "z : " << Ogre::StringConverter::toString(z) << '\n';
            errorString << "Function Name : " << "LogicModel::_getWorldHeight";

            Ogre::LogManager::getSingleton().logMessage(errorString.str());

            throw;
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_executeAttachedModel(Ogre::Real delta)
    {
        AttachedModelOriginSceneNodes::const_iterator it = 
            mAttachedModelOriginSceneNodes.begin();

        while (it != mAttachedModelOriginSceneNodes.end())
        {
            if (it->first)
                it->first->execute(delta);

            ++it;
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::addFabric(
        const Ogre::String& fabricName,
        int width, int height,
        Ogre::Real mass, Ogre::Real naturalLength,
        Ogre::Real structuralStiffness,
        Ogre::Real shearStiffness,
        Ogre::Real flexionStiffness,
        Ogre::Real deformationRate,
        const Ogre::Vector3& windVelocity,
        const Ogre::String& materialName,
        const Ogre::String& headLocatorName,
        const Ogre::String& tailLocatorName)
    {
        // ��������û��ͬ����fabric����
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.find(fabricName);

        // ����У���ɾ��֮ǰ������fabric
        if (fabricIterator != mFabricInfoMap.end())
        {
            removeFabric(fabricName);
        }

        // �Ȳ���Ҫ�ҽӵ�locator�Ƿ����
        LocatorValue headLocatorValue, tailLocatorValue;

        if ( getLocatorInfo(headLocatorName, headLocatorValue) &&
             getLocatorInfo(tailLocatorName, tailLocatorValue) )
        {
            Ogre::Fabric* fabric = static_cast<Ogre::Fabric*>(
                mSystem->getSceneManager()->createMovableObject(mModelMainNode->getName() + "_" + fabricName,
                ScorpioFabricFactory_TypeName) );

            // ����Ϊ��ǰ��visible flag
            fabric->setVisibilityFlags(mVisibleFlag);

            mModelMainNode->attachObject(fabric);

            Ogre::SceneNode* headLocatorNode = _getLocatorSceneNode(headLocatorValue);
            Ogre::SceneNode* tailLocatorNode = _getLocatorSceneNode(tailLocatorValue);

            setLocatorInfo(headLocatorName, headLocatorValue);
            setLocatorInfo(tailLocatorName, tailLocatorValue);

            Ogre::Vector3 headLocatorpos = headLocatorNode->_getDerivedPosition();
            //Ogre::Vector3 tailLocatorpos = tailLocatorValue.mLocatorNode->_getDerivedPosition();

            //Ogre::Vector3 xAxis = (tailLocatorpos - headLocatorpos).normalisedCopy();

            //Ogre::Vector3 zAxis = xAxis.crossProduct(Ogre::Vector3::UNIT_Y);

            //Ogre::Quaternion originOri(xAxis, Ogre::Vector3::UNIT_Y, zAxis);

            fabric->setPhysicsToDisplayScale(100);

            // Setup maximise movement tracking for numric stable
            fabric->setMaxTrackingDistance(100);
            fabric->setMaxTrackingAngle(Ogre::Degree(30));

            fabric->createRectanglePiece(
                width, height,
                mass, naturalLength,
                structuralStiffness, shearStiffness, flexionStiffness,
                deformationRate,
                true, false, headLocatorpos / 100/*, originOri*/);

            fabric->trackParticle(0, headLocatorNode);
            fabric->trackParticle(width - 1, tailLocatorNode);

            fabric->setMaterialName(materialName);
            fabric->setTimeSteps(0.05f, 0.02f, 0.05f);
            fabric->setWindVelocity(windVelocity);

            fabric->fastForward(1, 0.03);

            // �����ǰ��δ��������ײ���ļ��ϣ��ʹ�������������obj�ļ��ж��������collider
            if (NULL == mColliderSet)
                _initFabricColliders();

            if (mColliderSet)
                fabric->setColliderSet(mColliderSet);

            FabricInfo fabricInfo(fabric);

            // �����ǰ����͸��״̬,�����ò��ϵ�͸����
            if (mCurrentTransparency > 0.0f)
            {
                fabricInfo.mTransparentor = new FabricTransparentor(fabricInfo.mFabric);
                fabricInfo.mTransparentor->apply(mCurrentTransparency);
            }

            mFabricInfoMap.insert( FabricInfoMap::value_type(fabricName, fabricInfo) );
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::removeFabric(const Ogre::String& fabricName)
    {
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.find(fabricName);

        // ����У���ɾ��֮ǰ������fabric
        if (fabricIterator != mFabricInfoMap.end())
        {
            mModelMainNode->detachObject(fabricIterator->second.mFabric);

            mSystem->getSceneManager()->destroyMovableObject(fabricIterator->second.mFabric);

            delete fabricIterator->second.mTransparentor;

            mFabricInfoMap.erase(fabricIterator);
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::removeAllFabrics(void)
    {
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.begin();

        while ( fabricIterator != mFabricInfoMap.end() )
        {
            mModelMainNode->detachObject(fabricIterator->second.mFabric);

            mSystem->getSceneManager()->destroyMovableObject(fabricIterator->second.mFabric);

            delete fabricIterator->second.mTransparentor;

            ++fabricIterator;
        }

        mFabricInfoMap.clear();
    }
    //---------------------------------------------------------------------
    void LogicModel::_destroyColliders(void)
    {
        if (mColliderSet)
        {
            FabricColliderMap::iterator i = mFabricColliderMap.begin();

            while ( i != mFabricColliderMap.end() )
            {
                FabricCollider& collider = i->second;

                if (collider.mCollider)
                {
                    LogicModelManager::getSingleton().getCollisionManager()->destroyCollider(collider.mCollider); 
                }  

                ++ i;
            }

            LogicModelManager::getSingleton().getCollisionManager()->destroyColliderSet(mColliderSet);
        }
    }
    //---------------------------------------------------------------------
    bool LogicModel::addFabricCollider( const Ogre::String& colliderName, const Ogre::String& locatorName,
        FabricColliderType colliderType, const Ogre::String& colliderInfo )
    {
        FabricColliderMap::iterator i = mFabricColliderMap.find(colliderName);

        if ( i != mFabricColliderMap.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Fabric Collider with name '"
                + colliderName + "' already exists! " +
                "LogicModel::addFabricCollider " + mName );

            return false;
        }

        std::pair<FabricColliderMap::iterator, bool> inserted = 
            mFabricColliderMap.insert( FabricColliderMap::value_type( 
            colliderName, FabricCollider(locatorName, colliderType, colliderInfo) ) );

        if (false == inserted.second)
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Fabric Collider with name '"
                + colliderName + "' inserted failed! " +
                "LogicModel::addFabricCollider " + mName );

            return false;
        }

        return true;
    }
    //---------------------------------------------------------------------
    void LogicModel::removeFabricCollider( const Ogre::String& colliderName )
    {
        if (mColliderSet)
        {
            if (colliderName == "TerrainCollider")
                mColliderSet->removeCollider( LogicModelManager::getSingleton().getTerrainCollider() );
            else
            {
                FabricColliderMap::const_iterator i = mFabricColliderMap.find(colliderName);

                if ( i != mFabricColliderMap.end() )
                {
                    const FabricCollider& collider = i->second;
                    
                    if (collider.mCollider)
                    	mColliderSet->removeCollider(collider.mCollider);
                }
            }
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_initFabricColliders(void)
    {
        Ogre::CollisionManager* cm = LogicModelManager::getSingleton().getCollisionManager();

        if (cm)
        {
            mColliderSet = cm->createColliderSet();

            FabricColliderMap::iterator i = mFabricColliderMap.begin();

            while ( i != mFabricColliderMap.end() )
            {
                FabricCollider& collider = i->second;

                ++ i;

                // ��ȡlocatorָ��
                LocatorValue locatorValue;
                if ( getLocatorInfo(collider.mLocatorName, locatorValue) )
                {
                    collider.mLocatorNode = _getLocatorSceneNode(locatorValue);

					// �Ѹı���locator���������û�ȥ
                    setLocatorInfo(collider.mLocatorName, locatorValue);
                }
                else
                    continue;

                switch (collider.mColliderType)
                {
                case FCT_BOX:
                    {
                        collider.mCollider = cm->createBoxCollider( Ogre::StringConverter::parseVector3(collider.mColliderInfo) );
                        break;
                    }
                case FCT_SPHERE:
                    {
                        collider.mCollider = cm->createSphereCollider( Ogre::StringConverter::parseReal(collider.mColliderInfo) );
                        break;
                    }
                case FCT_PLANE:
                    {
                        collider.mCollider = cm->createPlaneCollider();
                        break;
                    }
                default:
                    {
                        continue;
                        break;
                    }
                }

                mColliderSet->addCollider(collider.mCollider);
            }

            // ��ӵ���collider
            mColliderSet->addCollider( LogicModelManager::getSingleton().getTerrainCollider() );
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_updateFabricColliders(void)
    {
        FabricColliderMap::iterator i = mFabricColliderMap.begin();

        while ( i != mFabricColliderMap.end() )
        {
            FabricCollider& collider = i->second;

            if (collider.mCollider && collider.mLocatorNode)
            {
                collider.mCollider->setPosition( collider.mLocatorNode->_getDerivedPosition() / 100 );

                // �����ƽ�����ͣ�������collider��rotation
                if (collider.mColliderType == FCT_PLANE)
                {
                    // ����plane�ķ���Ϊlocator�ķ�����
                    const Ogre::Quaternion& ori = collider.mLocatorNode->_getDerivedOrientation();

					// �ټ���һ����б�ĽǶ�,0.25�����13��
                    Ogre::Quaternion angle(Ogre::Quaternion::IDENTITY);

                    angle.FromAngleAxis(Ogre::Radian(0.25), Ogre::Vector3::NEGATIVE_UNIT_X);

                    collider.mCollider->setRotation( ori * angle );
                }
            }  

            ++ i;
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::enableAnimationState(bool enable)
    {
        if (mCurrentAnimationState && mCurrentAnimationState->getEnabled() != enable)
            mCurrentAnimationState->setEnabled(enable);

        if (mFadeOutAnimationState && mFadeOutAnimationState->getEnabled() != enable)
            mFadeOutAnimationState->setEnabled(enable);
    }
    //---------------------------------------------------------------------
    Ogre::SceneNode* LogicModel::_createLocatorSceneNode(const Ogre::Vector3& offsetPos, 
        const Ogre::Quaternion& offsetOri)
    {
        static Ogre::uint count = 0;
        Ogre::StringUtil::StrStreamType str;
        str << "Locator_" << std::setw(5) << std::setfill('0') << count++ << std::ends;
        Ogre::SceneNode* locatorNode = mModelMainNode->createChildSceneNode(str.str());
        locatorNode->translate( offsetPos );
        locatorNode->setOrientation( offsetOri );

        return locatorNode;
    }
    //---------------------------------------------------------------------
    Ogre::SceneNode* LogicModel::_getLocatorSceneNode(LocatorValue& locatorValue)
    {
        if ( !(locatorValue.mLocatorNode) )
        {
            locatorValue.mLocatorNode =
                _createLocatorSceneNode(locatorValue.mOffsetPos, locatorValue.mOffsetOrientation);
        }

        return locatorValue.mLocatorNode;
    }
    //---------------------------------------------------------------------
    bool LogicModel::isCameraVisible(void)
    {
        bool visible = true;

        if ( mFullBoundingBox && false == isChildModel() && false == isAttached() && 
            getVisibleFlag() == OVF_DEFAULT )
        {
#if 0
            // ʹ���ڲ������bounding box
            Ogre::AxisAlignedBox intersectedBox(mFullBoundingBox->getMinimum(), mFullBoundingBox->getMaximum());

            intersectedBox.transformAffine(mModelMainNode->_getFullTransform());

            visible = mSystem->getCamera()->isVisible(intersectedBox);

#else

            // ʹ��scene node��_update�м����bounding box
            visible = mSystem->getCamera()->isVisible( mModelMainNode->_getWorldAABB());
#endif
        }

        return visible;
    }
    //---------------------------------------------------------------------
    Ogre::StringVector LogicModel::getOriginMeshNames(void)
    {
        Ogre::StringVector nameVector;

        EntityMap::iterator i = mEntityMap.begin();

        while ( i != mEntityMap.end() )
        {
            const EntityValue& entity = i->second;

            if (false == entity.mMeshName.empty())
                nameVector.push_back(entity.mMeshName);

            ++i;
        }

        return nameVector;
    }
    //---------------------------------------------------------------------
    void LogicModel::playAnimation(const Ogre::String& animName, Ogre::Real startTime, Ogre::Real weight)
    {
        // ����б����Ƿ����������������еĻ����Ѿɵ�ɾ����
        PlayingAnimationList::iterator animIterator = mPlayingAnimationList.begin();

        while (animIterator != mPlayingAnimationList.end())
        {
            Ogre::AnimationState* state = *animIterator;

            assert (state);

            if (state->getAnimationName() == animName)
            {
                state->setEnabled(false);
                mPlayingAnimationList.erase(animIterator);
				//  ɾ�����
				if (startTime<0 && weight < 0)
				{
					return;
				}
                break;
            }

            ++animIterator;
        }

        Ogre::AnimationState* state = NULL;

        try
        {
            // �п�����Ϊ��Դ��ԭ��ȱ�������skeleton�ļ����������entityΪ��
            if (mSkeletonEntity)
            {
                state = _getAnimationState(animName);
                mSkeleton->getAnimation( animName )->setInterpolationMode( 
                    LogicModelManager::getSingleton().getAnimationInterpolationMode() );
            }
            else
            {
                Ogre::LogManager::getSingleton().logMessage( "Logic Model : " + mName + 
                    " Skeleton Entity is NULL, Maybe the skeleton file is lost!" +
                    "LogicModel::playAnimation");
                return;
            }

        }
        catch (const Ogre::Exception& e)
        {
            Ogre::LogManager::getSingleton().logMessage(e.getFullDescription() + "LogicModel::playAnimation!");

            if (mSystem->getThrowExceptionIfResourceIncorrect())//Ŀǰ�޵��ú���
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                    "Resource was incorrectness due incaution producer. "
                    "Full error description: " + e.getFullDescription(),
                    "LogicModel::playAnimation");
            }

            return;
        }

        if(state)
        {         
            state->setEnabled(true);
            state->setLoop(false);
            state->setTimePosition(startTime);
            state->setWeight(weight);

            mPlayingAnimationList.push_back(state);
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_updatePlayingAnimations(Ogre::Real deltaTime)
    {
        PlayingAnimationList::iterator animIterator = mPlayingAnimationList.begin();

        while (animIterator != mPlayingAnimationList.end())
        {
            Ogre::AnimationState* state = *animIterator;

            assert (state);

            // �����������Ѿ�������ϣ��Ͱ������б���ɾ��
            if (state->getTimePosition() >= state->getLength())
            {
                state->setEnabled(false);
                animIterator = mPlayingAnimationList.erase(animIterator);
            }
            else
            {
                // ���򣬼���ִ���������
                state->addTime(deltaTime * mGlobalAnimationRate);
                ++animIterator;
            }
        }
    }
    //---------------------------------------------------------------------
    bool LogicModel::_convertEntityToCharacterMaterial(Ogre::Entity* entity)
    {
            return false;//LM�޸�

        // ����һ��static����,����֤_checkForHardwareSkinningMaterialSupportֻ�ᱻ��һ��
        static bool materialSupport = _checkForHardwareSkinningMaterialSupport();

        if (!materialSupport)
            return false;

        // ����entity�Ƿ������
        if (entity->isHardwareAnimationEnabled())
            return false;

        if (!entity->hasSkeleton() || entity->hasVertexAnimation())
            return false;

        // Ŀǰֻ��һ��program��hlsl��
        const Ogre::String postfix = "_hlsl";

        // ��entity�е�ÿ��sub entity�����м�⣬ֻ��ÿ��sub entity������Ҫ�����ת��
        std::vector<Ogre::MaterialPtr> derivedMaterials;
        for (unsigned short i = 0; i < entity->getNumSubEntities(); ++i)
        {
            Ogre::SubEntity* se = entity->getSubEntity(i);
            Ogre::SubMesh* sm = se->getSubMesh();

            Ogre::MaterialPtr origin = Ogre::MaterialManager::getSingleton().getByName(se->getMaterialName());
            if (origin.isNull())
                return false;

            if (origin->getNumTechniques() != 1)
                return false;

            Ogre::Technique* technique = origin->getTechnique(0);
            if (technique->getNumPasses() != 1)
                return false;

            Ogre::Pass* pass = technique->getPass(0);
            if (pass->hasVertexProgram())
            {
                if (!pass->getVertexProgram()->isSkeletalAnimationIncluded())
                    return false;
                if (!pass->getVertexProgram()->isSupported())
                    return false;

                derivedMaterials.push_back(origin);
                continue;
            }

            // ֻ�в�����specular������ֵ�ģ���ʹ��specular
            bool useSpecular = true;

            if (pass->getSpecular() == Ogre::ColourValue::Black)
                useSpecular = false;

            if (pass->getNumTextureUnitStates() != 1)
                return false;

            const Ogre::VertexData* vertexData;
            const Ogre::Mesh::IndexMap* indexMap;
            if (sm->useSharedVertices)
            {
                vertexData = sm->parent->sharedVertexData;
                indexMap = &sm->parent->sharedBlendIndexToBoneIndexMap;
            }
            else
            {
                vertexData = sm->vertexData;
                indexMap = &sm->blendIndexToBoneIndexMap;
            }

            const Ogre::VertexElement* blendWeights =
                vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_BLEND_WEIGHTS);
            if (!blendWeights)
                return false;

            size_t numWeightsPerVertex = Ogre::VertexElement::getTypeCount(blendWeights->getType());
            size_t numWorldMatrices = indexMap->size();

            // Ĭ��ʹ�õ���vs 1.1���shader
           Ogre::String vsVersion = "_VS_1_1";

            // ���ĳ��sub entity�Ĺ�ͷ����������25
            if (numWorldMatrices > 25)
            {
                // ���ҵ�ǰ����֧��vs 2.0���Ǿ�ʹ��vs 2.0��shader
                if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0") && numWorldMatrices < 75 )
                    vsVersion = "_VS_2_0";
                else
                    return false;
            }
            
           Ogre::String prefix = "Character_HardwareSkinning";

            static const Ogre::String weightNames[] = { "_OneWeight", "_TwoWeight", "_ThreeWeight", "_FourWeight" };
            const Ogre::String& weightName = weightNames[numWeightsPerVertex-1];
            prefix += weightName + vsVersion;

            Ogre::MaterialPtr derived = _deriveHardwareSkinningMaterial(origin, prefix, postfix, useSpecular);
            derivedMaterials.push_back(derived);
        }

        // Change the materials
        for (unsigned short i = 0; i < entity->getNumSubEntities(); ++i)
        {
            Ogre::SubEntity* se = entity->getSubEntity(i);
            se->setMaterialName(derivedMaterials[i]->getName());
        }

        return true;
    }
    //---------------------------------------------------------------------
    Ogre::MaterialPtr LogicModel::_deriveHardwareSkinningMaterial(const Ogre::MaterialPtr& origin,
        const Ogre::String& prefix, const Ogre::String& postfix, bool useSpecular)
    {
       Ogre::String derivedMaterialName = origin->getName() +
            " : " + prefix + "_";

        Ogre::MaterialPtr derived = Ogre::MaterialManager::getSingleton().getByName(derivedMaterialName);

        if (derived.isNull())
        {
            derived = origin->clone(derivedMaterialName);

            Ogre::Technique* newTechnique = derived->createTechnique();
            Ogre::Technique* technique = derived->getTechnique(0);
            *newTechnique = *technique;

           Ogre::String baseName = prefix;

            Ogre::Pass* pass = technique->getPass(0);
            
            if (useSpecular)
            {
                pass->setVertexProgram(baseName + "_PixelSpecular" + postfix);

                if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") )
                    pass->setFragmentProgram("Character_HardwareSkinning_PixelSpecular_PS_2_0_hlsl");
            }
            else
            {
                pass->setVertexProgram(baseName + postfix);

                if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_1_1") )
                    pass->setFragmentProgram("Character_HardwareSkinning_PS_1_1_hlsl");
            }

            pass->setShadowCasterVertexProgram(baseName + "_AmbientOnly" + postfix);
            pass->getShadowCasterVertexProgramParameters()->setNamedAutoConstant("ambient", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
            pass->setShadowReceiverVertexProgram(baseName + "_Unlit" + postfix);
        }

        LogicModelManager::getSingleton().addToMaterialPool(derived->getHandle());

        return derived;
    }
    //---------------------------------------------------------------------
    bool LogicModel::_checkForHardwareSkinningMaterialSupport(void)
    {
        // �����һ��program�����.program�Ƿ���ȷ����
        Ogre::GpuProgramPtr vp =
            Ogre::GpuProgramManager::getSingleton().getByName("Character_HardwareSkinning_OneWeight_VS_1_1_PixelSpecular_hlsl");
        if ( vp.isNull() )
            return false;
        else
        {
            vp->load();

            if ( false == vp->isSupported() )
                return false;
        }

        return true;
    }
    //---------------------------------------------------------------------
    float LogicModel::getAnimationLength(const Ogre::String& animName) const
    {
       Ogre::String realAnimName;

        Skill *skillTemplate = Fairy::EffectManager::getSingleton().getSkill(animName);

        if (skillTemplate)
        {
            realAnimName = skillTemplate->getAnimationName();
        }
        else
        {
            realAnimName = animName;
        }

        if (mSkeletonEntity)
        {
            Ogre::Animation* animation = NULL;

            if (mSkeletonEntity->getSkeleton()->hasAnimation(realAnimName))
            {
                animation = mSkeletonEntity->getSkeleton()->getAnimation(realAnimName);

                return animation->getLength();
            }
            else
            {
                try
                {
                    Ogre::SkeletonPtr skeleton = Ogre::SkeletonManager::getSingleton().load( 
                        LogicModelManager::getSingleton().getSkeletonFromAnimationName(mName, realAnimName), 
                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                    animation = skeleton->getAnimation(realAnimName);

                    return animation->getLength();
                }
                catch (const Ogre::Exception& e)
                {
                    Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());

                    if (mSystem->getThrowExceptionIfResourceIncorrect())//Ŀǰ�޵��ú���
                    {
                        OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                            "Resource was incorrectness due incaution producer. "
                            "Full error description: " + e.getFullDescription(),
                            "LogicModel::getAnimationLength");
                    }

                    return -1.0f;
                }
            }
        }
        else
            return -1.0f;
    }
    //---------------------------------------------------------------------
    void LogicModel::setCurrentAnimTime(Ogre::Real range)
    {
        assert (range >= 0.0f && range <= 1.0f);

        if (mCurrentAnimationState)
        {
            mCurrentAnimationState->setTimePosition( mCurrentAnimationState->getLength() * range );
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::setScaleFactor(const Ogre::Vector3& scaleFactor)
    {
        if (!mExternalScaleFactor.positionEquals(scaleFactor))
        {
            mExternalScaleFactor = scaleFactor;

            // child model�ǲ���Ҫ�������������õ��ڵ��еģ���Ϊ�������ӻᴫ����ȥ
            if (false == isChildModel() && mModelMainNode)
                mModelMainNode->setScale(mExternalScaleFactor);

            setEntityNormalizeNormal(!mExternalScaleFactor.positionEquals(Ogre::Vector3::UNIT_SCALE));
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::setEntityNormalizeNormal(bool normal)
    {
        // ���������entity
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {
            Ogre::Entity* workingEntity = i->second.mEntity;
			//david-<<
            //if (workingEntity)
            //{
            //    workingEntity->setNormaliseNormals(normal);
            //}
			//david->>
        }

        // ����locator�ϵ�model
        for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
            itLocator != mLocatorMap.end(); ++itLocator )
        {
            LocatorValue& tempValue = itLocator->second;

            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (tempSlot.mModel)
                    tempSlot.mModel->setEntityNormalizeNormal(normal);
            }
        }
    }

	int LogicModel::_getTerrainSoundType()
	{
		return 1;
		//switch(soundType)
		//{
		//// ��ͨ����
		//case GENERAL_TER:
		//		id = 0;
		//		break;
		//// �ݵ�
		//case GRASS_TER:
		//		id = 1;
		//		break;
		//// ����
		//case GROUND_TER:
		//		id = 2;
		//		break;
		//// ѩ��
		//case SNOW_TER:
		//		id = 3;
		//		break;
		//// ˮ
		//case WATER_TER:
		//		id = 4;
		//		break;
		//// ʯͷ
		//case ROCK_TER:
		//		id = 5;
		//		break;
		//}
	}

}
