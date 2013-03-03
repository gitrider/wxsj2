#include "FairyTerrainLiquidObject.h"
#include "FairyTerrainLiquid.h"
#include "FairyPropertyHelper.h"
#include "FairyObjectFactory.h"
#include "FairySystem.h"
#include "FairySceneInfo.h"

#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreRenderTexture.h>
#include <OgreMaterialManager.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreCamera.h>
#include <OgrePlane.h>
#include <OgreLogManager.h>

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#endif

namespace Fairy {

    void ReflectRenderTargetListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        assert (mTerrainLiquid);
        mTerrainLiquid->setVisible(false);

        mCamera->enableReflection(*mPlane);
        mCamera->enableCustomNearClipPlane(*mPlane);
    }

    void ReflectRenderTargetListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        assert (mTerrainLiquid);
        mTerrainLiquid->setVisible(true);

        mCamera->disableReflection();
        mCamera->disableCustomNearClipPlane();
    }

    void RefractRenderTargetListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        assert (mTerrainLiquid);
        mTerrainLiquid->setVisible(false);

        mCamera->enableCustomNearClipPlane(*mPlane);
    }

    void RefractRenderTargetListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        assert (mTerrainLiquid);
        mTerrainLiquid->setVisible(true);

        mCamera->disableCustomNearClipPlane();
    }

    static const size_t TERRAIN_LIQUID_VERTEX_LIMIT = 32768;

    const Ogre::String TerrainLiquidObject::msType = "TerrainLiquid";
    const Ogre::String TerrainLiquidObject::msCategory = "StaticObject";

    // the name of the material that can reflect and refract the scene.
    const Ogre::String WaterReflectRefractMaterialName = "waterReflectRefractMat";
    const Ogre::String waterReflectRefractMatNamePrefixion = "# Fairy/waterReflectRefractMat/";

    enum TexUnit
    {
        NOISE,
        REFLECT,
        REFRACT,
        TEXUNITCONUT,
    };

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class TerrainLiquidObject::Factory : public ObjectFactory
    {
    public:
        const Ogre::String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new TerrainLiquidObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
		template <Ogre::ColourValue TerrainLiquidObject::*PtrToMember>
		struct ColourRGBMemberCommand : PropertyCommand
		{
			Variant doGet(const void* source) const
			{
				const TerrainLiquidObject* object = static_cast<const TerrainLiquidObject*>(source);
				return object->*PtrToMember;
			}

			Ogre::String doGetAsString(const void* source) const
			{
				const TerrainLiquidObject* object = static_cast<const TerrainLiquidObject*>(source);
				return ColourRGB2String(object->*PtrToMember);
			}

			void doSet(void* target, const Variant& value)
			{
				TerrainLiquidObject* object = static_cast<TerrainLiquidObject*>(target);
				object->*PtrToMember = VariantCast<Ogre::ColourValue>(value);
				object->updateRenderInstance();
			}

			void doSetAsString(void* target, const Ogre::String& value)
			{
				TerrainLiquidObject* object = static_cast<TerrainLiquidObject*>(target);
				object->*PtrToMember =String2ColourRGB(value);
				object->updateRenderInstance();
			}
		};

        typedef ObjectMemberPropertyCommand<TerrainLiquidObject,Ogre::String, &TerrainLiquidObject::mMaterialName> MaterialNameCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Vector3, &TerrainLiquidObject::mPosition> PositionCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, size_t, &TerrainLiquidObject::mSubdivision> SubdivisionCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Real, &TerrainLiquidObject::mTexcoordScale> TexcoordScaleCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Real, &TerrainLiquidObject::mTexcoordRotate> TexcoordRotateCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, bool, &TerrainLiquidObject::mDepthTexLayerEnabled> DepthTexLayerEnabledCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Real, &TerrainLiquidObject::mDepthTexLayerScale> DepthTexLayerScaleCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Real, &TerrainLiquidObject::mDepthTexLayerAdjust> DepthTexLayerAdjustCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject,Ogre::String, &TerrainLiquidObject::mProjectorName> ProjectorNameCmd;
        typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Real, &TerrainLiquidObject::mProjectorSize> ProjectorSizeCmd;
		typedef ColourRGBMemberCommand<&TerrainLiquidObject::mDiffuse> DiffuseCmd;
		typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Real, &TerrainLiquidObject::mAdjustDepth> DepthCmd;
		typedef ColourRGBMemberCommand<&TerrainLiquidObject::mMatSpecular> MatSpecularCmd;
		typedef ObjectMemberPropertyCommand<TerrainLiquidObject, Ogre::Real, &TerrainLiquidObject::mShininess> ShininessCmd;
    }

    //////////////////////////////////////////////////////////////////////////
    // TerrainLiquidObject class
    //////////////////////////////////////////////////////////////////////////
    TerrainLiquidObject::TerrainLiquidObject(void)
        : Object()
        , mSystem()
        , mTerrainLiquid()
        , mProjectionMaterial()
        , mProjectionCamera()
        , mMaterialName()
        , mPosition(Ogre::Vector3::ZERO)
        , mSubdivision(1)
        , mTexcoordScale(1)
        , mTexcoordRotate(0.0f)
        , mDepthTexLayerEnabled(false)
        , mDepthTexLayerScale(1.0f)
        , mDepthTexLayerAdjust(0.0f)
        , mProjectorName()
        , mProjectorSize(0.0f)
        , mReflectPlane(NULL)
        , mRefractPlane(NULL)
        , mReflectRenderTargetListener(NULL)
        , mRefractRenderTargetListener(NULL)
		, mDiffuse(Ogre::ColourValue::White)
		, mAdjustDepth(0)
		, mMatSpecular(Ogre::ColourValue::White)
		, mShininess(64.f)
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
        , mReflectTexture()
        , mRefractTexture()
#else
        , mReflectRenderTexture(NULL)
        , mRefractRenderTexture(NULL)
#endif
    {
        if (createPropertyDictionary("TerrainLiquid"))
        {
            static MaterialNameCmd materialNameCmd;
            defineProperty(
                "material",
                "The material name use to rendering this terrain liquid.",
                "MaterialName",
                &materialNameCmd,
                PF_PRIMARY);

            static PositionCmd positionCmd;
            defineProperty(
                "position",
                "The position of the terrain liquid.",
                "Position",
                &positionCmd,
                0);

            static SubdivisionCmd subdivisionCmd;
            defineProperty(
                "subdivision",
                "The subdivision of the terrain liquid grid.",
                "PositiveInteger",
                &subdivisionCmd,
                0);

            static TexcoordRotateCmd texcoordRotateCmd;
            defineProperty(
                "texture rotate",
                "The texture uv rotate of the terrain liquid.",
                "Ogre::Real",
                &texcoordRotateCmd,
                0);

            static TexcoordScaleCmd texcoordScaleCmd;
            defineProperty(
                "texture scale",
                "The texture uv factor of the terrain liquid.",
                "Ogre::Real",
                &texcoordScaleCmd,
                0);

            static DepthTexLayerEnabledCmd depthTexLayerEnabledCmd;
            defineProperty(
                "depth texture layer.enable",
                "Enable depth texture layer of the terrain liquid.",
                "Bool",
                &depthTexLayerEnabledCmd,
                0);

            static DepthTexLayerScaleCmd depthTexLayerScaleCmd;
            defineProperty(
                "depth texture layer.height scale",
                "The height scale factor to generate texcoords for depth texture layer.",
                "Ogre::Real",
                &depthTexLayerScaleCmd,
                0);

            static DepthTexLayerAdjustCmd depthTexLayerAdjustCmd;
            defineProperty(
                "depth texture layer.height adjust",
                "The height adjust to generate texcoords for depth texture layer.",
                "Ogre::Real",
                &depthTexLayerAdjustCmd,
                0);

            static ProjectorNameCmd projectorNameCmd;
            defineProperty(
                "projector name",
                "The object name use to project texture.",
                "String",
                &projectorNameCmd,
                0);

            static ProjectorSizeCmd projectorSizeCmd;
            defineProperty(
                "projector size",
                "The size used to scale the projected texture.",
                "Ogre::Real",
                &projectorSizeCmd,
                0);

			static DiffuseCmd diffuseCmd;
			defineProperty(
				"diffuse",
				"The diffuse of the terrain liquid.",
				"ColourRGB",
				&diffuseCmd,
				0);

			static DepthCmd depthCmd;
			defineProperty(
				"adjustDepth",
				"The adjust depth to set the alpha as 1.",
				"Ogre::Real",
				&depthCmd,
				0);

			static MatSpecularCmd matSpecularCmd;
			defineProperty(
				"specular",
				"The specular of the terrain liquid.",
				"ColourRGB",
				&matSpecularCmd,
				0);

			static ShininessCmd shininessCmd;
			defineProperty(
				"shininess",
				"The shininess of the terrain liquid.",
				"Ogre::Real",
				&shininessCmd,
				0);
        }
    }
    //-----------------------------------------------------------------------
    TerrainLiquidObject::~TerrainLiquidObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const Ogre::String& TerrainLiquidObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const Ogre::String& TerrainLiquidObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquidObject::createRenderInstance(System* system)
    {
        assert(system);
        mSystem = system;

        if (!mMaterialName.empty())
        {
            createTerrainLiquid();
        }
    }
    //-----------------------------------------------------------------------
    void TerrainLiquidObject::destroyRenderInstance(void)
    {
        if (mTerrainLiquid)
        {
            destroyTerrainLiquid();
        }
    }
    //-----------------------------------------------------------------------
    void TerrainLiquidObject::updateRenderInstance()
    {
        if (mTerrainLiquid)
        {
            destroyTerrainLiquid();
        }

        if (mSystem && !mMaterialName.empty())
        {
            createTerrainLiquid();
        }
    }
    //-----------------------------------------------------------------------
    void TerrainLiquidObject::createTerrainLiquid(void)
    {
        assert(!mTerrainLiquid);
        assert(mProjectionMaterial.isNull());
        assert(!mProjectionCamera);
        assert(mSystem);
        assert(!mMaterialName.empty());

        Ogre::SceneNode* parent = mSystem->getBaseSceneNode()->createChildSceneNode();
        assert(parent);

		mTerrainLiquid = new TerrainLiquid(mSystem, mPosition, mSubdivision, TERRAIN_LIQUID_VERTEX_LIMIT, mDiffuse, mAdjustDepth, mMatSpecular, mShininess);
        assert(mTerrainLiquid);

        parent->attachObject(mTerrainLiquid);

        mTerrainLiquid->setTexcoordScale(mTexcoordScale);
        mTerrainLiquid->setTexcoordRotate(Ogre::Radian(mTexcoordRotate));
        mTerrainLiquid->setDepthTexLayerSettings(
            mDepthTexLayerEnabled,
            mDepthTexLayerScale,
            mDepthTexLayerAdjust);

        _createRenderTexture();
        _setReflectPlane();

        _prepareProjector();

        if (!mProjectionMaterial.isNull())
            mTerrainLiquid->setMaterialName(mProjectionMaterial->getName());
        else
            mTerrainLiquid->setMaterialName(mMaterialName);
    }
    //-----------------------------------------------------------------------
    void TerrainLiquidObject::destroyTerrainLiquid(void)
    {
        _deleteRenderTextureRelative();

        assert(mTerrainLiquid);

        Ogre::SceneNode* parent = mTerrainLiquid->getParentSceneNode();
        assert(parent);

        if (!mProjectionMaterial.isNull())
        {
            Ogre::MaterialManager::getSingleton().remove(mProjectionMaterial->getHandle());
            mProjectionMaterial.setNull();
        }

        if (mProjectionCamera)
        {
            parent->getCreator()->destroyCamera(mProjectionCamera);
            mProjectionCamera = 0;
        }

        parent->getCreator()->destroySceneNode(parent);
		parent = NULL;

        delete mTerrainLiquid;
        mTerrainLiquid = NULL;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquidObject::_prepareProjector(void)
    {
        if (mProjectorName.empty() || !mProjectorSize)
            return;

        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(mMaterialName);
        if (material.isNull())
            return;

        bool hasProjector = false;
        {
            Ogre::Material::TechniqueIterator ti = material->getTechniqueIterator();
            while (ti.hasMoreElements())
            {
                Ogre::Technique* technique = ti.getNext();
                Ogre::Technique::PassIterator pi = technique->getPassIterator();
                while (pi.hasMoreElements())
                {
                    Ogre::Pass* pass = pi.getNext();
                    Ogre::Pass::TextureUnitStateIterator tusi = pass->getTextureUnitStateIterator();
                    while (tusi.hasMoreElements())
                    {
                        Ogre::TextureUnitState* tus = tusi.getNext();
                        if (Ogre::StringUtil::startsWith(tus->getName(), "Fairy/Projector", false))
                        {
                            hasProjector = true;
                        }
                    }
                }
            }
        }
        if (!hasProjector)
            return;

        ObjectPtr object = mSystem->getSceneInfo()->findObjectByName(mProjectorName);
        if (!object)
            return;

        Variant directionValue = object->getProperty("direction");
        if (directionValue.empty() || directionValue.type() != typeid(Ogre::Vector3))
            return;

        Ogre::Vector3 direction = VariantCast<Ogre::Vector3>(directionValue);
        direction.normalise();

       Ogre::String name = material->getName() + Ogre::StringConverter::toString((Ogre::ulong)this);

        mProjectionCamera = mTerrainLiquid->getParentSceneNode()->getCreator()->createCamera(name);
        mProjectionCamera->setAutoAspectRatio(false);
        mProjectionCamera->setAspectRatio(1.0f);
        mProjectionCamera->setFixedYawAxis(false);
        mProjectionCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        mProjectionCamera->setFOVy(Ogre::Degree(90));
        mProjectionCamera->setDirection(-direction);
        mProjectionCamera->setNearClipDistance(mProjectorSize / 2);
        mProjectionCamera->setFarClipDistance(mProjectorSize + 5000.0f);

        mTerrainLiquid->setProjectionCamera(mProjectionCamera);

        mProjectionMaterial = material->clone(name);
        {
            Ogre::Material::TechniqueIterator ti = mProjectionMaterial->getTechniqueIterator();
            while (ti.hasMoreElements())
            {
                Ogre::Technique* technique = ti.getNext();
                Ogre::Technique::PassIterator pi = technique->getPassIterator();
                while (pi.hasMoreElements())
                {
                    Ogre::Pass* pass = pi.getNext();
                    Ogre::Pass::TextureUnitStateIterator tusi = pass->getTextureUnitStateIterator();
                    while (tusi.hasMoreElements())
                    {
                        Ogre::TextureUnitState* tus = tusi.getNext();
                        if (Ogre::StringUtil::startsWith(tus->getName(), "Fairy/Projector", false))
                        {
                            tus->setProjectiveTexturing(true, mProjectionCamera);
                        }
                    }
                }
            }
        }
        mProjectionMaterial->load();
    }
    //-----------------------------------------------------------------------
    ObjectFactory* TerrainLiquidObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

    void TerrainLiquidObject::_createRenderTexture(void)
    {
        // only craete the texture when we want the water to reflect and refract
        if (mMaterialName.find(WaterReflectRefractMaterialName) !=Ogre::String::npos)
        {
            Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(WaterReflectRefractMaterialName);

            if (mat.isNull())
            {
                Ogre::LogManager::getSingleton().logMessage(
                    WaterReflectRefractMaterialName + "does not exist. Have you forgotten to define it in a "
                    ".material script?");
            }
            else
            {
                assert (mat->getTechnique(0)->getPass(0)->getNumTextureUnitStates() >= TEXUNITCONUT );

                // 构建新材质的名称，并判断这个材质是否存在
                Ogre::String newMatName = mTerrainLiquid->getName() + waterReflectRefractMatNamePrefixion + WaterReflectRefractMaterialName;

                Ogre::MaterialPtr newMat = Ogre::MaterialManager::getSingleton().getByName(newMatName);

                if (newMat.isNull() == false)
                {
                    Ogre::LogManager::getSingleton().logMessage(newMat->getName() + "exist.");
                }
                assert (newMat.isNull() == true);

                mat = mat->clone(newMatName);

                assert (false == mat.isNull());
                // create refraction texture
                if ( mat->getTechnique(0)->getPass(0)->getTextureUnitState( REFRACT )->getTextureName() == "<RefractTexInput>" )
                {
                    assert (mTerrainLiquid);
                    mRefractTextureName = mTerrainLiquid->getName() + waterReflectRefractMatNamePrefixion + "waterRefractTex";

                    Ogre::Viewport *v = NULL;

                    Ogre::Viewport* oldViewport = mSystem->getCamera()->getViewport();
                    Ogre::Real aspect = mSystem->getCamera()->getAspectRatio();

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
                    mRefractTexture = Ogre::TextureManager::getSingleton().createManual(
                        mRefractTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                        Ogre::TEX_TYPE_2D, 512, 512, 1, 0, Ogre::PF_X8R8G8B8, Ogre::TU_RENDERTARGET);
                    mRefractTexture->load();
                    {
                        v = mRefractTexture->getBuffer()->getRenderTarget()->addViewport( mSystem->getCamera() );
                    }
#else
                    mRefractRenderTexture = mSystem->getRoot()->getRenderSystem()->createRenderTexture( mRefractTextureName, 512, 512 );
                    {
                        v = mRefractRenderTexture->addViewport( mSystem->getCamera() );
                    }
#endif

                    mSystem->getCamera()->_notifyViewport(oldViewport);
                    mSystem->getCamera()->setAspectRatio(aspect);
                    assert (v);
                    mat->getTechnique(0)->getPass(0)->getTextureUnitState( REFRACT )->setTextureName(mRefractTextureName);
                    v->setOverlaysEnabled(false);
                }

                mRefractRenderTargetListener = new RefractRenderTargetListener;
                mRefractRenderTargetListener->setCamera( mSystem->getCamera() );

                mRefractRenderTargetListener->setTerrainLiquid(mTerrainLiquid);

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
                assert ( false == mRefractTexture.isNull() );
                mRefractTexture->getBuffer()->getRenderTarget()->addListener(mRefractRenderTargetListener);
#else
                assert (mRefractRenderTexture);
                assert ( mRefractRenderTexture ==
                    mSystem->getRoot()->getRenderSystem()->getRenderTarget(mRefractTextureName) );
                mRefractRenderTexture->addListener(mRefractRenderTargetListener);
#endif

                // create reflection texture
                if ( mat->getTechnique(0)->getPass(0)->getTextureUnitState( REFLECT )->getTextureName() == "<ReflectTexInput>" )
                {
                    assert (mTerrainLiquid);

                    mReflectTextureName = mTerrainLiquid->getName() + waterReflectRefractMatNamePrefixion + "waterReflectTex";

                    Ogre::Viewport *v = NULL;
                    Ogre::Viewport* oldViewport = mSystem->getCamera()->getViewport();
                    Ogre::Real aspect = mSystem->getCamera()->getAspectRatio();

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
                    mReflectTexture = Ogre::TextureManager::getSingleton().createManual(
                        mReflectTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                        Ogre::TEX_TYPE_2D, 512, 512, 1, 0, Ogre::PF_X8R8G8B8, Ogre::TU_RENDERTARGET);
                    mReflectTexture->load();
                    {
                        v = mReflectTexture->getBuffer()->getRenderTarget()->addViewport( mSystem->getCamera() );
                    }
#else
                    mReflectRenderTexture = mSystem->getRoot()->getRenderSystem()->createRenderTexture( mReflectTextureName, 512, 512 );
                    {
                        v = mReflectRenderTexture->addViewport( mSystem->getCamera() );
                    }
#endif

                    mSystem->getCamera()->_notifyViewport(oldViewport);
                    mSystem->getCamera()->setAspectRatio(aspect);
                    assert (v);
                    mat->getTechnique(0)->getPass(0)->getTextureUnitState( REFLECT )->setTextureName(mReflectTextureName);
                    v->setOverlaysEnabled(false);
                }

                mReflectRenderTargetListener = new ReflectRenderTargetListener;
                mReflectRenderTargetListener->setTerrainLiquid(mTerrainLiquid);
                mReflectRenderTargetListener->setCamera( mSystem->getCamera() );

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
                assert ( false == mReflectTexture.isNull() );
                mReflectTexture->getBuffer()->getRenderTarget()->addListener(mReflectRenderTargetListener);
#else
                assert (mReflectRenderTexture);
                assert ( mReflectRenderTexture ==
                    mSystem->getRoot()->getRenderSystem()->getRenderTarget(mReflectTextureName) );
                mReflectRenderTexture->addListener(mReflectRenderTargetListener);
#endif

                mMaterialName = newMatName;
            }
        }
    }

    void TerrainLiquidObject::_setReflectPlane()
    {
        if (mMaterialName.find(WaterReflectRefractMaterialName) !=Ogre::String::npos)
        {
            if ( mReflectPlane == NULL )
                mReflectPlane = new Ogre::Plane( Ogre::Vector3::UNIT_Y, mPosition.y );
            else
            {
                mReflectPlane->d = mPosition.y;
                mReflectPlane->normal = Ogre::Vector3::UNIT_Y;
            }

            assert (mReflectRenderTargetListener);
            mReflectRenderTargetListener->setPlane(mReflectPlane);

            if ( mRefractPlane == NULL )
                mRefractPlane = new Ogre::Plane( -Ogre::Vector3::UNIT_Y, -mPosition.y );
            else
            {
                mRefractPlane->d = -mPosition.y;
                mRefractPlane->normal = -Ogre::Vector3::UNIT_Y;
            }

            assert (mRefractRenderTargetListener);
            mRefractRenderTargetListener->setPlane(mRefractPlane);
        }
    }

    void TerrainLiquidObject::_deleteRenderTextureRelative(void)
    {
        if (mReflectRenderTargetListener)
        {
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
            mReflectTexture->getBuffer()->getRenderTarget()->removeListener(mReflectRenderTargetListener);
#else
            assert (mReflectRenderTexture);
            mReflectRenderTexture->removeListener(mReflectRenderTargetListener);
#endif

            delete mReflectRenderTargetListener;
            mReflectRenderTargetListener = NULL;
        }

        if (mRefractRenderTargetListener)
        {
#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
            mRefractTexture->getBuffer()->getRenderTarget()->removeListener(mRefractRenderTargetListener);
#else
            assert (mRefractRenderTexture);
            mRefractRenderTexture->removeListener(mRefractRenderTargetListener);
#endif

            delete mRefractRenderTargetListener;
            mRefractRenderTargetListener = NULL;
        }

        if (mReflectPlane)
        {
            delete mReflectPlane;
            mReflectPlane = NULL;
        }

        if (mRefractPlane)
        {
            delete mRefractPlane;
            mRefractPlane = NULL;
        }

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
        if ( false == mReflectTexture.isNull() )
        {
            assert( mReflectTexture->getBuffer()->getRenderTarget()->getNumViewports() == 1 );
            mReflectTexture->getBuffer()->getRenderTarget()->removeAllViewports();
            Ogre::TextureManager::getSingleton().remove(mReflectTexture->getName());
            mReflectTexture.setNull();
        }
#else
        if (mReflectRenderTexture)
        {
            assert ( mReflectRenderTexture->getNumViewports() == 1 );
            mReflectRenderTexture->removeAllViewports();
            mSystem->getRoot()->getRenderSystem()->destroyRenderTexture(mReflectTextureName);
            mReflectRenderTexture = NULL;
        }
#endif

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
        if ( false == mRefractTexture.isNull() )
        {
            assert( mRefractTexture->getBuffer()->getRenderTarget()->getNumViewports() == 1 );
            mRefractTexture->getBuffer()->getRenderTarget()->removeAllViewports();
            Ogre::TextureManager::getSingleton().remove(mRefractTexture->getName());
            mRefractTexture.setNull();
        }
#else
        if (mRefractRenderTexture)
        {
            assert ( mRefractRenderTexture->getNumViewports() == 1 );
            mRefractRenderTexture->removeAllViewports();
            mSystem->getRoot()->getRenderSystem()->destroyRenderTexture(mRefractTextureName);
            mRefractRenderTexture = NULL;
        }
#endif

        if (mMaterialName.find(waterReflectRefractMatNamePrefixion+WaterReflectRefractMaterialName) !=Ogre::String::npos)
        {
            Ogre::MaterialManager::getSingleton().remove(mMaterialName);
        }
    }
}
