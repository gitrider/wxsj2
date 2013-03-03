#include "FairyModelObject.h"
#include "FairyObjectFactory.h"
#include "FairyObjectProxy.h"
#include "FairyModel.h"
#include "FairySystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace Fairy
{
    const Ogre::String ModelObject::msType = "Model";
    const Ogre::String ModelObject::msCategory = "Model";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class ModelObject::Factory : public ObjectFactory
    {
    public:
        const Ogre::String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new ModelObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct ModelNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const ModelObject* object = static_cast<const ModelObject*>(source);
                return object->getModelName();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const ModelObject* object = static_cast<const ModelObject*>(source);
                return object->getModelName();
            }

            void doSet(void* target, const Variant& value)
            {
                ModelObject* object = static_cast<ModelObject*>(target);
               Ogre::String modelName = VariantCast<String>(value);
                object->setModelName(modelName);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                ModelObject* object = static_cast<ModelObject*>(target);
                object->setModelName(value);
            }
        };

        struct AnimationLevelCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const ModelObject* object = static_cast<const ModelObject*>(source);
                return object->getAnimationLevel();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const ModelObject* object = static_cast<const ModelObject*>(source);
                return Ogre::StringConverter::toString(object->getAnimationLevel());
            }

            void doSet(void* target, const Variant& value)
            {
                ModelObject* object = static_cast<ModelObject*>(target);
                Ogre::Real animationLevel = VariantCast<Ogre::Real>(value);
                object->setAnimationLevel(animationLevel);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                ModelObject* object = static_cast<ModelObject*>(target);
                Ogre::Real animationLevel = Ogre::StringConverter::parseReal(value);
                object->setAnimationLevel(animationLevel);
            }
        };

        struct CastShadowsCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const ModelObject* object = static_cast<const ModelObject*>(source);
                return object->getCastShadows();
            }

           Ogre::String doGetAsString(const void* source) const
            {
                const ModelObject* object = static_cast<const ModelObject*>(source);
                return Ogre::StringConverter::toString(object->getCastShadows());
            }

            void doSet(void* target, const Variant& value)
            {
                ModelObject* object = static_cast<ModelObject*>(target);
                bool castShadows = VariantCast<bool>(value);
                object->setCastShadows(castShadows);
            }

            void doSetAsString(void* target, const Ogre::String& value)
            {
                ModelObject* object = static_cast<ModelObject*>(target);
                bool castShadows = Ogre::StringConverter::parseBool(value);
                object->setCastShadows(castShadows);
            }
        };

    }

    //////////////////////////////////////////////////////////////////////////
    // ModelObject class
    //////////////////////////////////////////////////////////////////////////


    ModelObject::ModelObject(void)
        : PlacementObject()
        , mProxy(NULL)
        , mModel(NULL)
        , mModelName()
        , mAnimationLevel(50)
        , mCastShadows(false)
        , mSystem(NULL)
    {
        if (createPropertyDictionary("Model"))
        {
            static ModelNameCmd modelNameCmd;
            defineProperty(
                "model name",
                "The model filename use to create this model.",
                "ModelName",
                &modelNameCmd,
                PF_PRIMARY);

            addBaseProperty();

            static AnimationLevelCmd animationLevelCmd;
            defineProperty(
                "animation level",
                "The animation level of this object.",
                "Ogre::Real",
                &animationLevelCmd,
                0);

            static CastShadowsCmd castShadowsCmd;
            defineProperty(
                "cast shadows",
                "Does this object cast shadows?",
                "Bool",
                &castShadowsCmd,
                0);
        }
    }
    //-----------------------------------------------------------------------
    ModelObject::~ModelObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const Ogre::String& ModelObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const Ogre::String& ModelObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void ModelObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mModel);
        assert(!mProxy);

        PlacementObject::createRenderInstance(system);
        mSystem = system;

        mProxy = new ObjectProxy(shared_from_this());

        if (!mModelName.empty())
        {
            createModel();
        }
    }
    //-----------------------------------------------------------------------
    void ModelObject::destroyRenderInstance(void)
    {
        if (mModel)
        {
            destroyModel();
        }

        if (mProxy)
        {
            delete mProxy;
            mProxy = NULL;
        }

        PlacementObject::destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void ModelObject::setModelName(const Ogre::String& modelName)
    {
        if (mModelName != modelName)
        {
            if (mModel)
            {
                destroyModel();
            }

            mModelName = modelName;

            if (mProxy && !mModelName.empty())
            {
                createModel();
            }
        }
    }
    //-----------------------------------------------------------------------
    void ModelObject::setAnimationLevel(Ogre::Real level)
    {
        mAnimationLevel = level;
        if (mModel)
        {
            mModel->setAnimationType(mSystem->_determineAnimationType(mAnimationLevel));
        }
    }
    //-----------------------------------------------------------------------
    void ModelObject::setCastShadows(bool castShadows)
    {
        mCastShadows = castShadows;
        if (mModel)
        {
            mModel->setCastShadows(mCastShadows);
        }
    }
    //-----------------------------------------------------------------------
    void ModelObject::createModel(void)
    {
        assert(!mModel);
        assert(mProxy && !mModelName.empty());

        Ogre::SceneNode* parent = getSceneNode();
        assert(parent);

        mModel = new Model(parent, mModelName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mModel->setUserObject(mProxy);

        mModel->setAnimationType(mSystem->_determineAnimationType(mAnimationLevel));
        mModel->setCastShadows(mCastShadows);
    }
    //-----------------------------------------------------------------------
    void ModelObject::destroyModel(void)
    {
        assert(mModel);

        mModel->setUserObject(NULL);
        delete mModel;
        mModel = NULL;
        mSystem = NULL;
    }
    //-----------------------------------------------------------------------
    void ModelObject::processMessage(const Ogre::String& msg, const Variant& params)
    {
        if (msg == System::ANIMATION_QUALITY_CHANGED_MSG)
        {
            if (mModel)
            {
                mModel->setAnimationType(mSystem->_determineAnimationType(mAnimationLevel));
            }
        }

        PlacementObject::processMessage(msg, params);
    }
    //-----------------------------------------------------------------------
    Variant ModelObject::getProperty(const Ogre::String& name) const
    {
        if (name == "bounding box" && mModel)
        {
            return mModel->getBoundingBox();
        }

        return PlacementObject::getProperty(name);
    }
    //-----------------------------------------------------------------------
   Ogre::String ModelObject::getPropertyAsString(const Ogre::String& name) const
    {
        if (name == "bounding box" && mModel)
        {
            const Ogre::AxisAlignedBox& aabb = mModel->getBoundingBox();
            return Ogre::StringConverter::toString(aabb.getMinimum()) + ", " + 
                   Ogre::StringConverter::toString(aabb.getMaximum());
        }

        return PlacementObject::getPropertyAsString(name);
    }
    //-----------------------------------------------------------------------
    ObjectFactory* ModelObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
