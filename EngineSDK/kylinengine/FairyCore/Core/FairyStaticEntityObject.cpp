#include "FairyStaticEntityObject.h"
#include "FairyObjectFactory.h"
#include "FairyObjectProxy.h"
#include "FairyResources.h"
#include "FairyResourceCollection.h"
#include "FairyUtils.h"
#include "FairyFlags.h"
#include "FairyTransparentor.h"
#include "FairySystem.h"
#include "OgreExt/OgreAutoAnimationEntity.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <OgreStaticGeometry.h>

#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreAny.h>
#include "FairyPropertyHelper.h"

namespace Fairy
{
    const Ogre::String StaticEntityObject::msType = "StaticEntity";
    const Ogre::String StaticEntityObject::msCategory = "StaticObject";
	Ogre::uint StaticEntityObject::ms_uGenPrefixCount = 0;

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class StaticEntityObject::Factory : public ObjectFactory
    {
    public:
        const Ogre::String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new StaticEntityObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct MeshNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getMeshName();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getMeshName();
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
               Ogre::String meshName = VariantCast<String>(value);
                object->setMeshName(meshName);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                object->setMeshName(value);
            }
        };

        struct AnimationLevelCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getAnimationLevel();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getAnimationLevel());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Ogre::Real animationLevel = VariantCast<Ogre::Real>(value);
                object->setAnimationLevel(animationLevel);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Ogre::Real animationLevel = Ogre::StringConverter::parseReal(value);
                object->setAnimationLevel(animationLevel);
            }
        };

        struct TransparencyCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getTransparency();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getTransparency());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Ogre::Real transparency = VariantCast<Ogre::Real>(value);
                object->setTransparency(transparency);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Ogre::Real transparency = Ogre::StringConverter::parseReal(value);
                object->setTransparency(transparency);
            }
        };

        struct CastShadowsCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getCastShadows();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getCastShadows());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = VariantCast<bool>(value);
                object->setCastShadows(castShadows);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = Ogre::StringConverter::parseBool(value);
                object->setCastShadows(castShadows);
            }
        };

        struct ReceiveShadowsCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getReceiveShadows();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getReceiveShadows());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = VariantCast<bool>(value);
                object->setReceiveShadows(castShadows);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = Ogre::StringConverter::parseBool(value);
                object->setReceiveShadows(castShadows);
            }
        };

        struct ReceiveDecalsCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getReceiveDecals();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getReceiveDecals());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool receiveDecals = VariantCast<bool>(value);
                object->setReceiveDecals(receiveDecals);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool receiveDecals = Ogre::StringConverter::parseBool(value);
                object->setReceiveDecals(receiveDecals);
            }
        };

		// 是否允许透明
		struct EnableTransparencyCmd : PropertyCommand
		{
			Variant doGet(const void* source) const
			{
				const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
				return object->getEnableTransparency();
			}

			Ogre::String doGetAsString(const void* source) const
			{
				const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
				return Ogre::StringConverter::toString(object->getEnableTransparency());
			}

			void doSet(void* target, const Variant& value)
			{
				StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
				bool receiveDecals = VariantCast<bool>(value);
				object->setEnableTransparency(receiveDecals);
			}

			void doSetAsString(void* target, const Ogre::String& value)
			{
				StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
				bool receiveDecals = Ogre::StringConverter::parseBool(value);
				object->setEnableTransparency(receiveDecals);
			}
		};

		struct ColourRGBMemberCommand : PropertyCommand
		{
			Variant doGet(const void* source) const
			{
				const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
				return object->getAdditionalDiffuse();
			}

			Ogre::String doGetAsString(const void* source) const
			{
				const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
				return ColourRGB2String(object->getAdditionalDiffuse());
			}

			void doSet(void* target, const Variant& value)
			{
				StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
				const Ogre::ColourValue diffuse = VariantCast<Ogre::ColourValue>(value);
				object->setAdditionalDiffuse(diffuse);
			}

			void doSetAsString(void* target, const Ogre::String& value)
			{
				StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
				const Ogre::ColourValue diffuse = String2ColourRGB(value);
				object->setAdditionalDiffuse(diffuse);
			}
		};

		// 是否允许透明
		struct DiffuseAddedCmd : PropertyCommand
		{
			Variant doGet(const void* source) const
			{
				const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
				return object->getDiffuseAdded();
			}

			Ogre::String doGetAsString(const void* source) const
			{
				const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
				return Ogre::StringConverter::toString(object->getDiffuseAdded());
			}

			void doSet(void* target, const Variant& value)
			{
				StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
				bool diffuseAdded = VariantCast<bool>(value);
				object->setDiffuseAdded(diffuseAdded);
			}

			void doSetAsString(void* target, const Ogre::String& value)
			{
				StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
				bool diffuseAdded = Ogre::StringConverter::parseBool(value);
				object->setDiffuseAdded(diffuseAdded);
			}
		};
        
		// 透明度增量（淡入淡出）
		float TRANPARENCY_FADE_INC = 0.01f;
	}

    //////////////////////////////////////////////////////////////////////////
    // StaticEntityObject class
    //////////////////////////////////////////////////////////////////////////


    StaticEntityObject::StaticEntityObject(void)
        : PlacementObject()
        , mSystem(NULL)
        , mProxy(NULL)
        , mEntity(NULL)
        , mMeshName()
        , mAnimationLevel(50)
		, mCurrentTransparency()
		, mDestTransparency()
        , mCastShadows(false)
        , mReceiveShadows(false)
        , mReceiveDecals(false)
		, mEnableTransparency(true)
        , mEntityTransparentor()
        , mAnimationType(AT_DISABLE)
		, mAdditionalDiffuse(Ogre::ColourValue::White)
		, mDiffuseAdded(false)
    {
		mUserAny = Ogre::Any();
		mClonedMaterials.clear();
		mPrefixName = "StaticEntityCloned/"+Ogre::StringConverter::toString(ms_uGenPrefixCount)+"/";
		ms_uGenPrefixCount++;

        if (createPropertyDictionary("StaticEntity"))
        {
            static MeshNameCmd meshNameCmd;
            defineProperty(
                "mesh name",
                "The mesh filename use to create this static entity.",
                "MeshName",
                &meshNameCmd,
                PF_PRIMARY);

            addBaseProperty();

            static AnimationLevelCmd animationLevelCmd;
            defineProperty(
                "animation level",
                "The animation level of this object.",
                "Ogre::Real",
                &animationLevelCmd,
                0);

            static TransparencyCmd transparencyCmd;
            defineProperty(
                "transparency",
                "Amount transparent of this object.",
                "Ogre::Real",
                &transparencyCmd,
                0);

            static CastShadowsCmd castShadowsCmd;
            defineProperty(
                "cast shadows",
                "Does this object cast shadows?",
                "Bool",
                &castShadowsCmd,
                0);

            static ReceiveShadowsCmd receiveShadowsCmd;
            defineProperty(
                "receive shadows",
                "Does this object receive shadows?",
                "Bool",
                &receiveShadowsCmd,
                0);

            static ReceiveDecalsCmd receiveDecalsCmd;
            defineProperty(
                "receive decals",
                "Does this object receive decals?",
                "Bool",
                &receiveDecalsCmd,
                0);

			static EnableTransparencyCmd enableTransparencyCmd;
			defineProperty(
				"enable transparency",
				"Does this object enable transparency?",
				"Bool",
				&enableTransparencyCmd,
				0);

			static ColourRGBMemberCommand additionalDiffuseCmd;
			defineProperty(
				"additional diffuse",
				"The additinal diffuse of the staticEntity object.",
				"ColourRGB",
				&additionalDiffuseCmd,
				0);

			static DiffuseAddedCmd diffuseAddedCmd;
			defineProperty(
				"diffuse added",
				"Does this object additional diffuse be added?",
				"Bool",
				&diffuseAddedCmd,
				0);
        }
    }
    //-----------------------------------------------------------------------
    StaticEntityObject::~StaticEntityObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const Ogre::String& StaticEntityObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const Ogre::String& StaticEntityObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::queryReferenceResources(ResourceCollection* resourceCollection)
    {
        if (mEntity)
        {
            resourceCollection->add("Mesh", mEntity->getMesh()->getName());

            if (getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME))
            {
                resourceCollection->add("Skeleton", mEntity->getMesh()->getSkeletonName());
            }
        }
        else
        {
            resourceCollection->add("Mesh", mMeshName);
        }
    }

	//david-<<
	//-----------------------------------------------------------------------
	void StaticEntityObject::setUserObject(Ogre::UserDefinedObject* obj)
	{
		mUserAny = Ogre::Any(obj);
		mEntity->setUserAny(Ogre::Any(mUserAny));
	}
	//-----------------------------------------------------------------------
	Ogre::UserDefinedObject* StaticEntityObject::getUserObject(void) const
	{
		return mUserAny.isEmpty() ? 0 : Ogre::any_cast<Ogre::UserDefinedObject*>(mUserAny);
	}
	//david->>
    //-----------------------------------------------------------------------
    void StaticEntityObject::queryBakableEntities(EntityList& entities)
    {
        assert(mEntity);

        if (!getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME))
        {
            entities.push_back(mEntity);
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::bakeStaticGeometry(Ogre::StaticGeometry* staticGeometry, const EntityList& entity)
    {
        assert(mEntity);
        assert(!entity.empty());
        assert(entity.front() == mEntity);
        assert(!getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME));

        staticGeometry->addSceneNode(mEntity->getParentSceneNode());
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mEntity);
        assert(!mProxy);

        PlacementObject::createRenderInstance(system);
        mSystem = system;

        mProxy = new ObjectProxy(shared_from_this());

        if (!mMeshName.empty())
        {
            createEntity();
        }
    }

	void StaticEntityObject::clearClonedMaterials()
	{
		for (MaterialMap::iterator iter = mClonedMaterials.begin(); iter != mClonedMaterials.end(); ++iter)
		{
			Ogre::String materialName = iter->first;

			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(materialName);
			if (!mat.isNull())
			{
				Ogre::MaterialManager::getSingleton().remove(mat->getHandle());
				mat.setNull();
			}
		}
		mClonedMaterials.clear();
	}

    //-----------------------------------------------------------------------
    void StaticEntityObject::destroyRenderInstance(void)
    {
        if (mEntity)
        {
            destroyEntity();
        }

        if (mProxy)
        {
            delete mProxy;
            mProxy = NULL;
        }

        mSystem = NULL;

        PlacementObject::destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setMeshName(const Ogre::String& meshName)
    {
        if (mMeshName != meshName)
        {
            if (mEntity)
            {
                destroyEntity();
            }

            mMeshName = meshName;

            if (mProxy && !mMeshName.empty())
            {
                createEntity();
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setAnimationLevel(Ogre::Real level)
    {
        mAnimationLevel = level;
        updateAnimationType();
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setTransparency(Ogre::Real transparency)
    {
        // Clamp transparency to range [0, 1]
        if (transparency < 0)
            transparency = 0;
        else if (transparency > 1)
            transparency = 1;

		if (mCurrentTransparency != transparency)
        {
            mDestTransparency = transparency;
            updateTransparency();
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setCastShadows(bool castShadows)
    {
        if (mCastShadows != castShadows)
        {
            mCastShadows = castShadows;

            if (mEntity)
            {
                mEntity->setCastShadows(castShadows);
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setReceiveShadows(bool receiveShadows)
    {
        if (mReceiveShadows != receiveShadows)
        {
            mReceiveShadows = receiveShadows;

            if (mEntity)
            {
                if (receiveShadows)
                {
                    mEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
                }
                else
                {
                    mEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAIN);
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setReceiveDecals(bool receiveDecals)
    {
        mReceiveDecals = receiveDecals;

        if (mEntity)
        {
            if (receiveDecals)
            {
                mEntity->addQueryFlags(OQF_DECAL_TARGET);
            }
            else
            {
                mEntity->removeQueryFlags(OQF_DECAL_TARGET);
            }
        }
    }
	//------------------------------------------------------
	void StaticEntityObject::setEnableTransparency(bool enableTransparency)
	{
		mEnableTransparency = enableTransparency;
	}
    //-----------------------------------------------------------------------

	void StaticEntityObject::setAdditionalDiffuse(Ogre::ColourValue diffuse)
	{
		mAdditionalDiffuse = diffuse;		

		if (mEntity)
		{
			if ((int)mClonedMaterials.size() == 0)
			{
				if (mAdditionalDiffuse == Ogre::ColourValue::White)
				{
					return;
				}
				cloneMaterialsManually();
			}

			for (MaterialMap::iterator iter = mClonedMaterials.begin(); iter != mClonedMaterials.end(); ++iter)
			{
				Ogre::String materialName = iter->first;
				Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().getByName(materialName);
				if (matPtr.isNull())
				{
					continue;
				}
				
				Ogre::String baseMaterialName = iter->second;
				Ogre::MaterialPtr baseMatPtr = Ogre::MaterialManager::getSingleton().getByName(baseMaterialName);
				if (baseMatPtr.isNull())
				{
					continue;
				}

				for (int techIdx = 0; techIdx < matPtr->getNumTechniques(); ++techIdx)
				{
					Ogre::Technique* technique = matPtr->getTechnique(techIdx);
					Ogre::Technique* baseTechnique = baseMatPtr->getTechnique(techIdx);

					for (int passIdx = 0; passIdx < technique->getNumPasses(); ++passIdx)
					{
						Ogre::Pass* pass = technique->getPass(passIdx);
						Ogre::Pass* basePass = baseTechnique->getPass(passIdx);

						Ogre::ColourValue oldDiffuse = basePass->getDiffuse();
						if (!mDiffuseAdded)
						{
							pass->setDiffuse( oldDiffuse*diffuse );
						}
						else
						{
							Ogre::ColourValue newColour = oldDiffuse+diffuse;
							newColour.saturate();
							pass->setDiffuse(newColour);
						}
					}
				}				
			}
		}

	}

	void StaticEntityObject::setDiffuseAdded(bool bAdded)
	{
		if (mDiffuseAdded == bAdded)
		{
			return;
		}

		mDiffuseAdded = bAdded;
		setAdditionalDiffuse(mAdditionalDiffuse);
	}

    void StaticEntityObject::createEntity(void)
    {
        assert(!mEntity);
        assert(mProxy && !mMeshName.empty());

        Ogre::SceneNode* parent = getSceneNode();
        assert(parent);
        Ogre::SceneManager* creator = parent->getCreator();
        assert(creator);

        // Load the mesh intelligently
        Ogre::MeshPtr mesh = loadMesh(
            mMeshName, Ogre::StringUtil::BLANK,
            Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);

        if (getAnimationSafely(mesh, AUTO_ANIMATION_NAME))
        {
            Ogre::NameValuePairList params;
            params["mesh"] = mesh->getName();
            Ogre::AutoAnimationEntity* autoAnimationEntity = static_cast<Ogre::AutoAnimationEntity*>(
                creator->createMovableObject(parent->getName(), Ogre::AutoAnimationEntityFactory::FACTORY_TYPE_NAME, &params));
            mEntity = autoAnimationEntity;
            convertEntityToHardwareSkinning(mEntity);
        }
        else
        {
            mEntity = creator->createEntity(parent->getName(), mesh->getName());
        }
		
		setAdditionalDiffuse(mAdditionalDiffuse);
		//david-<<
		setUserObject(mProxy);
		//david->>
		
        parent->attachObject(mEntity);
        mEntity->setCastShadows(mCastShadows);
        if (mReceiveDecals)
        {
            mEntity->addQueryFlags(OQF_DECAL_TARGET);
        }

        updateNormaliseNormals();
        updateTransparency();
        updateAnimationType();
    }

	void StaticEntityObject::cloneMaterialsManually()
	{
		if (mClonedMaterials.empty() && !getName().empty())
		{
			size_t mumSubEntity = mEntity->getNumSubEntities();

			for (size_t i = 0; i < mumSubEntity; ++i)
			{
				Ogre::SubEntity* subEntity =  mEntity->getSubEntity(i);
				Ogre::SubMesh* subMesh = subEntity->getSubMesh();
				Ogre::String materialName =  subMesh->getMaterialName();	
				Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().getByName(materialName);
				if (matPtr.isNull())
				{
					continue;
				}

				Ogre::String newMaterialName = mPrefixName+materialName;
				Ogre::MaterialPtr newMatPtr = Ogre::MaterialManager::getSingleton().getByName(newMaterialName);
				if (newMatPtr.isNull())
				{
					Ogre::MaterialPtr mClonedMaterial = matPtr->clone(newMaterialName);	
					mClonedMaterials.insert(MaterialMap::value_type(newMaterialName,materialName));
				}
				
				subEntity->setMaterialName(newMaterialName);				
			}			
		}
	}

    //-----------------------------------------------------------------------
    void StaticEntityObject::destroyEntity(void)
    {
        assert(mEntity);

        if (mEntityTransparentor)
        {
            delete mEntityTransparentor;
            mEntityTransparentor = NULL;
        }

		clearClonedMaterials();
		Ogre::SceneNode* parent = getSceneNode();
		parent->getCreator()->destroyEntity(mEntity);
        mEntity = NULL;
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::updateAnimationType(void)
    {
        if (mSystem && mEntity &&
            mEntity->getMovableType() == Ogre::AutoAnimationEntityFactory::FACTORY_TYPE_NAME)
        {
            AnimationType type = mSystem->_determineAnimationType(mAnimationLevel);
            if (mAnimationType != type)
            {
                mAnimationType = type;

                Ogre::AutoAnimationEntity* autoAnimationEntity = static_cast<Ogre::AutoAnimationEntity*>(mEntity);
                Ogre::AnimationState* as = autoAnimationEntity->getAnimationState(AUTO_ANIMATION_NAME);
                switch (mAnimationType)
                {
                case AT_DISABLE:
                    if (autoAnimationEntity->getAutoUpdateAnimationStateList().find(as) !=
                        autoAnimationEntity->getAutoUpdateAnimationStateList().end())
                    {
                        autoAnimationEntity->removeAutoUpdateAnimationState(AUTO_ANIMATION_NAME);
                    }
                    as->setEnabled(false);
                    break;

                case AT_FROZE:
                    if (autoAnimationEntity->getAutoUpdateAnimationStateList().find(as) !=
                        autoAnimationEntity->getAutoUpdateAnimationStateList().end())
                    {
                        autoAnimationEntity->removeAutoUpdateAnimationState(AUTO_ANIMATION_NAME);
                    }
                    else if (!as->getEnabled())
                    {
                        as->setTimePosition(Ogre::Math::UnitRandom() * as->getLength());
                        as->setEnabled(true);
                    }
                    break;

                case AT_FULL:
                    if (autoAnimationEntity->getAutoUpdateAnimationStateList().find(as) ==
                        autoAnimationEntity->getAutoUpdateAnimationStateList().end())
                    {
                        autoAnimationEntity->addAutoUpdateAnimationState(AUTO_ANIMATION_NAME,
                            as->getEnabled() ? as->getTimePosition() : - Ogre::Math::UnitRandom());
                    }
                    break;
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::updateNormaliseNormals(void)
    {
        assert(mEntity);
        assert(mEntity->getParentSceneNode());
		//david-<<
        //mEntity->setNormaliseNormals(
        //    !mEntity->getParentSceneNode()->_getDerivedScale().positionEquals(Ogre::Vector3::UNIT_SCALE));
		//david->>
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::updateTransparency(void)
    {
		if (NULL != mEntity)
		{
			// 淡出
			if(mCurrentTransparency > mDestTransparency)
			{
				mCurrentTransparency -= TRANPARENCY_FADE_INC;
				if(mCurrentTransparency < 0)
					mCurrentTransparency = 0;
			}
			// 淡入
			else
			{
				mCurrentTransparency += TRANPARENCY_FADE_INC;
				if(mCurrentTransparency > mDestTransparency)
					mCurrentTransparency = mDestTransparency;
			}

			if (NULL == mEntityTransparentor)
				mEntityTransparentor = new EntityTransparentor(mEntity);

			mEntityTransparentor->apply(mCurrentTransparency);
		}
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::processMessage(const Ogre::String& msg, const Variant& params)
    {
        if (msg == System::ANIMATION_QUALITY_CHANGED_MSG)
        {
            updateAnimationType();
        }

        PlacementObject::processMessage(msg, params);
    }
    //-----------------------------------------------------------------------
    bool StaticEntityObject::setProperty(const Ogre::String& name, const Variant& value)
    {
        if (!PlacementObject::setProperty(name, value))
            return false;

        if (mEntity && name == "scale")
        {
            updateNormaliseNormals();
        }

        return true;
    }
    //-----------------------------------------------------------------------
    bool StaticEntityObject::setPropertyAsString(const Ogre::String& name, const Ogre::String& value)
    {
        if (!PlacementObject::setPropertyAsString(name, value))
            return false;

        if (mEntity && name == "scale")
        {
            updateNormaliseNormals();
        }

        return true;
    }
    //-----------------------------------------------------------------------
    Variant StaticEntityObject::getProperty(const Ogre::String& name) const
    {
        if (name == "bounding box" && mEntity)
        {
            return mEntity->getBoundingBox();
        }

        return PlacementObject::getProperty(name);
    }
    //-----------------------------------------------------------------------
   Ogre::String StaticEntityObject::getPropertyAsString(const Ogre::String& name) const
    {
        if (name == "bounding box" && mEntity)
        {
            const Ogre::AxisAlignedBox& aabb = mEntity->getBoundingBox();
            return Ogre::StringConverter::toString(aabb.getMinimum()) + ", " + 
                   Ogre::StringConverter::toString(aabb.getMaximum());
        }

        return PlacementObject::getPropertyAsString(name);
    }
    //-----------------------------------------------------------------------
    ObjectFactory* StaticEntityObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
