#include "FairySystem.h"

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>
#include <OgreStringConverter.h>
#include <OgreVector2.h>
#include <OgreException.h>
#include <OgreParticleSystemManager.h>
#include <OgreArchiveManager.h>
#include <OgreTextureManager.h>
#include <OgreMeshManager.h>
#include <OgreLogManager.h>
#include <OgreRenderQueueListener.h>

#include "FrameStatsListener.h"

#include "FairyFlags.h"
#include "FairyUtils.h"
#include "FairyResources.h"
#include "FairySceneInfo.h"
#include "Terrain.h"

#include "FairyCollisionModelManager.h"
#include "FairyObjectFactoryManager.h"
#include "FairyObjectFactory.h"
#include "FairyLightObject.h"
#include "FairyStaticEntityObject.h"
#include "FairyActorObject.h"
#include "FairyModelObject.h"
#include "FairyParticleSystemObject.h"
#include "FairySkyBoxObject.h"
#include "FairySkyDomeObject.h"
#include "FairySkyPlaneObject.h"
#include "FairyEnviromentObject.h"
#include "FairyTerrainLiquidObject.h"
#include "FairyEffectObject.h"
#include "FairyBulletObject.h"
#include "FairyLogicModelObject.h"
#include "TerrainTypeInfos.h"
#include "Core/FairyResources.h"
#include "PostFilter/FairyPostFilterManager.h"
#include "EffectSystem/FairyEffectManager.h"
#include "EffectSystem/FairyBulletFlowSystemManager.h"
#include "EffectSystem/FairyBulletSystemManager.h"
#include "EffectSystem/FairyMainSceneLight.h"

#include "ModelSystem/FairyLogicModelManager.h"

#include "OgreExt/OgreAutoAnimationEntity.h"
#include "OgreExt/OgreTexCoordBillboardParticleRenderer.h"
#include "OgreExt/OgreMeshParticleRenderer.h"
#include "OgreExt/OgreMeshRotationAffectorFactory.h"
#include "OgreExt/OgreMeshAnimationAffectorFactory.h"
#include "OgreExt/OgreRibbonRenderer.h"

#include "FairyProjector.h"
#include "ResourceSystem/FairyPakFileSystem.h"

namespace Fairy {

const Ogre::String System::ANIMATION_QUALITY_CHANGED_MSG("AnimationQualityChanged");
const Ogre::String System::CREATE_LEVEL_CHANGED_MSG("CreateLevelChanged");

//////////////////////////////////////////////////////////////////////////
// Initialise phase one
//////////////////////////////////////////////////////////////////////////
System::System(void)
    : mCollisionModelManager(NULL)
    , mObjectFactoryManager(NULL)
    , mPostFilterManager(NULL)

    , mMovableObjectFactoryList()

    , mRoot(NULL)
    , mRenderWindow(NULL)
    , mSceneManager(NULL)
    , mBaseSceneNode(NULL)
    , mStaticSceneNode(NULL)
    , mCamera(NULL)
    , mViewport(NULL)

    , mFrameStatsListener(NULL)

    , mAnimationQuality(50)
    , mSystemCreateLevel(50)

    , mTerrain(NULL)
    , mTerrainData(NULL)
    , mSceneInfo(NULL)
    , mStaticGeometries()
    , mBaseScale(1)
	, mBackgrounColour(0,0,0,1)
    , mHasExceptionWhenLoadScene(false)
    , mThrowExceptionIfResourceIncorrect(true)
    , mDisableIncrementalBuildScene(false)

	, mEffectManager(NULL)
	, mBulletSystemManager(NULL)
	, mBulletFlowSystemManager(NULL)
	, mTexCoordBillboardParticleRendererFactory(NULL)
	, mMeshParticleRendererFactory(NULL)
	, mMeshRotationAffectorFactory(NULL)
    , mMeshAnimationAffectorFactory(NULL)
	, mLogicModelManager(NULL)
    , mTerrainTypeInfos(NULL)
    , mRibbonRendererFactory(NULL)
{
}
//-----------------------------------------------------------------------
System::~System()
{
    delete mTerrainTypeInfos;

    if (mRenderWindow)
        mRenderWindow->setActive(false);

    delete mPostFilterManager;

    delete mFrameStatsListener;	

    clearStaticGeometries();
    delete mSceneInfo;

    if (mCamera && mSceneManager)
        mSceneManager->destroyCamera(mCamera->getName());

	m_setTransparencyObjects.clear();

    if (mStaticSceneNode)
    {
        mStaticSceneNode->getCreator()->destroySceneNode(mStaticSceneNode);
		mStaticSceneNode = NULL;
    }

    if (mLogicModelManager)
    {
        delete mLogicModelManager;
        mLogicModelManager = NULL;
    }

	delete mEffectManager;

	delete mBulletSystemManager;
	delete mBulletFlowSystemManager;

    if (mBaseSceneNode)
    {
        mBaseSceneNode->getCreator()->destroySceneNode(mBaseSceneNode);
		mBaseSceneNode = NULL;
    }

    delete mObjectFactoryManager;
    delete mCollisionModelManager;
	
    delete mRoot;

    for (MovableObjectFactoryList::const_iterator it = mMovableObjectFactoryList.begin(); it != mMovableObjectFactoryList.end(); ++it)
    {
        Ogre::MovableObjectFactory* factory = *it;
        delete factory;
    }

	if (mTexCoordBillboardParticleRendererFactory)
	{
		OGRE_DELETE mTexCoordBillboardParticleRendererFactory;
		mTexCoordBillboardParticleRendererFactory = NULL;
	}

	if (mMeshParticleRendererFactory)
	{
		OGRE_DELETE mMeshParticleRendererFactory;
		mMeshParticleRendererFactory = NULL;
	}

	if (mMeshRotationAffectorFactory)
	{
		OGRE_DELETE mMeshRotationAffectorFactory;
		mMeshRotationAffectorFactory = NULL;
	}	

    if (mMeshAnimationAffectorFactory)
    {
        OGRE_DELETE mMeshAnimationAffectorFactory;
        mMeshAnimationAffectorFactory = NULL;
    }

    if (mRibbonRendererFactory)
    {
        OGRE_DELETE mRibbonRendererFactory;
        mRibbonRendererFactory = NULL;
    }
}
//////////////////////////////////////////////////////////////////////////
// Initialise phase two
//////////////////////////////////////////////////////////////////////////
void
System::init(const Ogre::String& resourceFileName,
             const Ogre::String& pluginFileName,
             const Ogre::String& configFileName,
             const Ogre::String& logFileName)
{
    assert(!Ogre::Root::getSingletonPtr());

    mRoot = new Ogre::Root(pluginFileName, configFileName, logFileName);

	mWXFileSystemArchiveFactory = new Ogre::PakFileSystemArchiveFactory();
	Ogre::ArchiveManager::getSingleton().addArchiveFactory( mWXFileSystemArchiveFactory );

	//mRoot->loadInnerPlugins();

	//david-<<
    // Enable automatically build edge lists for all loading meshes.
    //Ogre::MeshManager::getSingleton().setEnableAutoBuildAllMeshesEdgeLists(true);
	//david->>

    // Setup default object query and visibility mask
    Ogre::MovableObject::setDefaultQueryFlags(OQF_DEFAULT);
    Ogre::MovableObject::setDefaultVisibilityFlags(OVF_DEFAULT);

    assert(!CollisionModelManager::getSingletonPtr());
    mCollisionModelManager = new CollisionModelManager;

    _registerExtraPlugins();

    if (!resourceFileName.empty())
    {
        _setupResources(resourceFileName);
    }

    assert(!ObjectFactoryManager::getSingletonPtr());
    mObjectFactoryManager = new ObjectFactoryManager;
    _registerObjectFactories();
}

void
System::makeObjectTransparent( const Ogre::Vector3 &start, const Ogre::Vector3 &end, float nNewTrans, float nDelay, IObjectFilter *pFilter )
{
	Ogre::Vector3 vDirction = (start-end);
	Ogre::Real fDistance = vDirction.length();
	Ogre::Ray ray = Ogre::Ray(end,vDirction);
	Ogre::RaySceneQuery* raySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
	std::set<Fairy::Object*> setCurrentTransparentObjects;

	raySceneQuery->setRay(ray);
	const Ogre::RaySceneQueryResult& queryResult = raySceneQuery->execute();
	for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
	{
		Ogre::MovableObject* pMovable = it->movable;
		Ogre::Vector3 vObjectPos = pMovable->getParentNode()->getPosition();
		Ogre::Vector3 vObjectDir = (vObjectPos-end);
		Fairy::ObjectPtr pObject = Fairy::getObjectFromMovable(pMovable);

		if(pObject)
		{  
			// 当满足以下条件时设置透明: 1.是StaticEntity类型; 2.相交; 3.允许透明
			if(pObject->getType() == "StaticEntity" && vObjectDir.dotProduct(vDirction) > 0 && static_cast<Fairy::StaticEntityObject*>(pObject.get())->getEnableTransparency())
			{
				Fairy::StaticEntityObject* pModel = static_cast<Fairy::StaticEntityObject*>(pObject.get());
				pModel->setTransparency(0.5f);
				// 删除原有的Object，放到临时的Set中
				m_setTransparencyObjects.erase(pObject.get());
				setCurrentTransparentObjects.insert(pObject.get());
			}
		}
	}

	std::set<Fairy::Object*>::iterator iter = m_setTransparencyObjects.begin();
	std::set<Fairy::Object*>::iterator tempIter;

	while (iter != m_setTransparencyObjects.end())
	{
		tempIter = iter;
		++iter;
		// 恢复没有遮挡的Object的透明度为0
		Fairy::StaticEntityObject* pModel = static_cast<Fairy::StaticEntityObject*>(*tempIter);
		pModel->setTransparency(0);
		// 删除透明度为0的Object
		if (pModel->getTransparency()==0)
		{
			m_setTransparencyObjects.erase(tempIter);
		}
	}

	// 将遮挡的Object放到m_setTransparencyObjects
	for (std::set<Fairy::Object*>::iterator it = setCurrentTransparentObjects.begin(); it != setCurrentTransparentObjects.end(); ++it)
	{
        m_setTransparencyObjects.insert(*it);
	}

	if(raySceneQuery)
	{
		getSceneManager()->destroyQuery(raySceneQuery);
		raySceneQuery = NULL;
	}
}
//-----------------------------------------------------------------------

void System::clearTransparencyObjects()
{
	m_setTransparencyObjects.clear();
}
void
System::_registerExtraPlugins(void)
{
    _registerMovableObjectFactories();
    for (MovableObjectFactoryList::const_iterator it = mMovableObjectFactoryList.begin(); it != mMovableObjectFactoryList.end(); ++it)
    {
        Ogre::MovableObjectFactory* factory = *it;
        mRoot->addMovableObjectFactory(factory);
    }

	mTexCoordBillboardParticleRendererFactory = OGRE_NEW Ogre::TexCoordBillboardParticleRendererFactory;
	Ogre::ParticleSystemManager::getSingleton().addRendererFactory(mTexCoordBillboardParticleRendererFactory);

	mMeshParticleRendererFactory = OGRE_NEW Ogre::MeshParticleRendererFactory;
	Ogre::ParticleSystemManager::getSingleton().addRendererFactory(mMeshParticleRendererFactory);

	mMeshRotationAffectorFactory = OGRE_NEW Ogre::MeshRotationAffectorFactory();
	Ogre::ParticleSystemManager::getSingleton().addAffectorFactory(mMeshRotationAffectorFactory);

    mMeshAnimationAffectorFactory = OGRE_NEW Ogre::MeshAnimationAffectorFactory();
    Ogre::ParticleSystemManager::getSingleton().addAffectorFactory(mMeshAnimationAffectorFactory);

    mRibbonRendererFactory = OGRE_NEW Ogre::RibbonRendererFactory;
    Ogre::ParticleSystemManager::getSingleton().addRendererFactory(mRibbonRendererFactory);
}
//-----------------------------------------------------------------------
void
System::_registerMovableObjectFactories(void)
{
    mMovableObjectFactoryList.push_back(new Ogre::AutoAnimationEntityFactory);
    mMovableObjectFactoryList.push_back(new ProjectorFactory);
    // TODO: new movable object factory register here
}
//-----------------------------------------------------------------------
void
System::_setupResources(const Ogre::String& resourceFileName)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(resourceFileName);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        const Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::const_iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            bool recursive = false;
            if (archName[0] == '*')
            {
                archName.erase(0, 1);
                Ogre::StringUtil::trim(archName);
                recursive = true;
            }
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName, recursive);
        }
    }
}
//-----------------------------------------------------------------------
void
System::_registerObjectFactories(void)
{
    mObjectFactoryManager->registerFactory(LightObject::getFactory());
    mObjectFactoryManager->registerFactory(StaticEntityObject::getFactory());
    mObjectFactoryManager->registerFactory(ActorObject::getFactory());
    mObjectFactoryManager->registerFactory(ModelObject::getFactory());
    mObjectFactoryManager->registerFactory(ParticleSystemObject::getFactory());
    mObjectFactoryManager->registerFactory(SkyBoxObject::getFactory());
    mObjectFactoryManager->registerFactory(SkyDomeObject::getFactory());
    mObjectFactoryManager->registerFactory(SkyPlaneObject::getFactory());
    mObjectFactoryManager->registerFactory(EnviromentObject::getFactory());
    mObjectFactoryManager->registerFactory(TerrainLiquidObject::getFactory());
    mObjectFactoryManager->registerFactory(EffectObject::getFactory());
	mObjectFactoryManager->registerFactory(BulletObject::getFactory());
    mObjectFactoryManager->registerFactory(LogicModelObject::getFactory());
	mObjectFactoryManager->registerFactory(RegionObject::getFactory());

    // TODO: register object factories here
}
//////////////////////////////////////////////////////////////////////////
// Initialise phase three
//////////////////////////////////////////////////////////////////////////
void
System::setup(Ogre::ulong windowHandle, const Ogre::NameValuePairList* extraParams)
{
    assert(mRoot);

    _createRenderWindow(windowHandle, extraParams);
    _createSceneManager();
    _createCamera();
    _createViewports();

	mEffectManager = new EffectManager(this);

	mBulletSystemManager = new BulletSystemManager(this);
	mBulletFlowSystemManager = new BulletFlowSystemManager(this);

	mLogicModelManager = new LogicModelManager(this);

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    _createResourceListener();

    // Load resources
    _loadResources();

    _initScene();
}
//-----------------------------------------------------------------------
void
System::_createRenderWindow(Ogre::ulong windowHandle, const Ogre::NameValuePairList* extraParams)
{
    mRoot->initialise(false, Ogre::StringUtil::BLANK);

    Ogre::NameValuePairList miscParams;
    if (extraParams)
    {
        miscParams = *extraParams;
    }

    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(windowHandle);
    mRenderWindow = mRoot->createRenderWindow(Ogre::StringUtil::BLANK, 0, 0, false, &miscParams);
    mRenderWindow->setActive(true);
}
//-----------------------------------------------------------------------
class FairyRenderQueueListener : public Ogre::RenderQueueListener
{
protected:
    Ogre::Rectangle2D* mFullScreenQuad;

public:
    FairyRenderQueueListener(void)
        : mFullScreenQuad()
    {
    }

    void renderQueueStarted(uint8 queueGroupId, const Ogre::String& invocation,
        bool& skipThisInvocation)
    {
        using namespace Ogre;

        if (queueGroupId == RENDER_QUEUE_6)
        {
            RenderSystem* rs = Root::getSingleton().getRenderSystem();
            rs->clearFrameBuffer(FBT_STENCIL);
            rs->setStencilCheckEnabled(true);
            rs->setStencilBufferParams(
                CMPF_ALWAYS_PASS,
                0xFFFFFFFF, 0xFFFFFFFF,
                SOP_KEEP,
                SOP_INCREMENT,
                SOP_REPLACE,
                false);
        }
    }

    void renderQueueEnded(uint8 queueGroupId, const Ogre::String& invocation,
        bool& repeatThisInvocation)
    {
        using namespace Ogre;

        if (queueGroupId == RENDER_QUEUE_6)
        {
            MaterialPtr material;
            // Also init full screen quad while we're at it
            if (!mFullScreenQuad)
            {
                mFullScreenQuad = new Rectangle2D();
                mFullScreenQuad->setCorners(-1,1,1,-1);
                material = mFullScreenQuad->getMaterial()->clone(StringConverter::toString((Ogre::ulong)mFullScreenQuad));
                mFullScreenQuad->setMaterial(material->getName());
                Pass* pass = material->getTechnique(0)->getPass(0);
                pass->setDepthCheckEnabled(false);
                pass->setDepthWriteEnabled(false);
                pass->setFog(true, FOG_NONE);
                pass->setLightingEnabled(true);
                pass->setAmbient(ColourValue::ZERO);
                pass->setDiffuse(ColourValue::ZERO);
                pass->setSpecular(ColourValue::ZERO);
                pass->setSelfIllumination(0, 1, 0);
            }
            else
            {
                material = mFullScreenQuad->getMaterial();
            }
            Pass* pass = material->getTechnique(0)->getPass(0);

            SceneManager* sm = Root::getSingleton()._getCurrentSceneManager();
            RenderSystem* rs = Root::getSingleton().getRenderSystem();

            rs->setStencilBufferParams(
                CMPF_EQUAL,
                0xFFFFFFFF, 0xFFFFFFFF,
                SOP_KEEP,
                SOP_INVERT,
                SOP_INVERT,
                false);
            pass->setColourWriteEnabled(false);
            sm->_injectRenderWithPass(pass, mFullScreenQuad, false);

            rs->setStencilBufferParams(
                CMPF_NOT_EQUAL,
                0, 0xFFFFFFFF,
                SOP_KEEP,
                SOP_KEEP,
                SOP_KEEP,
                false);
            pass->setColourWriteEnabled(true);
            sm->_injectRenderWithPass(pass, mFullScreenQuad, false);

            rs->setStencilCheckEnabled(false);
            rs->setStencilBufferParams();
        }
    }
};
void
System::_createSceneManager(void)
{
    mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "FairyMainScene");

    const char* s = getenv("Fairy_Hack");
    if (s)
    {
        if (strstr(s, "X-Ray") != 0)
        {
            static FairyRenderQueueListener sFairyRenderQueueListener;
            mSceneManager->addRenderQueueListener(&sFairyRenderQueueListener);
        }
    }

    Ogre::RenderQueue* rq = mSceneManager->getRenderQueue();

	//david-<<
    // Disable shadows for all future groups
    //rq->setEnableShadowsForUnregisteredGroup(false);
	//david->>

    // Disable shadow for all registered render queue group
    Ogre::RenderQueue::QueueGroupIterator it = rq->_getQueueGroupIterator();
    while (it.hasMoreElements())
    {
        Ogre::uint8 qid = it.peekNextKey();
        Ogre::RenderQueueGroup* group = it.getNext();
        group->setShadowsEnabled(false);
    }

    // Only enable shadows for world geometry at the moment
    rq->getQueueGroup(mSceneManager->getWorldGeometryRenderQueue())->setShadowsEnabled(true);
}
//-----------------------------------------------------------------------
void
System::_createCamera(void)
{
    // Create the camera
    mCamera = mSceneManager->createCamera("MainCamera");

    // Set camera near/far clip
    Ogre::Real s = 1.0f;
    mCamera->setNearClipDistance(0.25f * s);//LM修改
    mCamera->setFarClipDistance(150.0f * s);
    // 45 degree fov
    mCamera->setFOVy(Ogre::Degree(45));
    // Auto aspect ratio
    mCamera->setAutoAspectRatio(true);
}
//-----------------------------------------------------------------------
void
System::_createViewports(void)
{
    // Create one viewport, entire window
    mViewport = mRenderWindow->addViewport(mCamera);
    mViewport->setBackgroundColour(mBackgrounColour);

    if (!mCamera->getAutoAspectRatio())
    {
        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(
            Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
    }
}
//-----------------------------------------------------------------------
void
System::_createResourceListener(void)
{
	// We need to pre-initialise the 'Bootstrap' group so we can use
	// the basic contents in the loading screen
	//david-<<
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
        Fairy::BOOTSTRAP_RESOURCE_GROUP_NAME);
	//david->>

    // TODO: Create any resource listeners (for loading screens)
}
//-----------------------------------------------------------------------
void
System::_loadResources(void)
{
	// Initialise, parse scripts etc
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-----------------------------------------------------------------------
void
System::_initScene(void)
{
    mSceneManager->addSpecialCaseRenderQueue(FairySpecialCaseRenderQueue);
    mSceneManager->setVisibilityMask(OVF_DEFAULT);

    mFrameStatsListener = new FrameStatsListener(mRenderWindow);

	mBaseSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode("#MainSceneNode");
    mStaticSceneNode = mBaseSceneNode->createChildSceneNode("#StaticSceneNode");

    mPostFilterManager = new PostFilterManager;

    mSceneInfo = new SceneInfo;
    mTerrain = mSceneInfo->getTerrain();
    assert(mTerrain);
    mTerrainData = mSceneInfo->getTerrainData();
    assert(mTerrainData);

    mTerrainTypeInfos = new TerrainTypeInfos;

    // 读取地形信息配置文件
    try
    {
        mTerrainTypeInfos->parseTerrainTypeConfigFile("FairyTerrainType.cfg");
    }
    catch(Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());
    }
}
//////////////////////////////////////////////////////////////////////////
// Scene maintenance
//////////////////////////////////////////////////////////////////////////
bool
System::isEditable(void) const
{
    return false;
}
//-----------------------------------------------------------------------
bool
System::isEmpty(void) const
{
    if (!getTerrainData()->isEmpty())
        return false;

    return getSceneInfo()->getObjects().empty();
}
//-----------------------------------------------------------------------
void
System::resetScene(void)
{
    clearStaticGeometries();

    mSceneInfo->reset();

    // Destroy all pass maps
    static bool destroyPassMaps = true;
    mSceneManager->getRenderQueue()->clear(destroyPassMaps);

    // Release all automatic temporary buffers and free all unused temporary buffers.
    if (Ogre::HardwareBufferManager::getSingletonPtr())
    {
        Ogre::HardwareBufferManager::getSingleton()._releaseBufferCopies(true);
    }
}
//-----------------------------------------------------------------------
void
System::loadSceneFromResource(XMLParser* parser, const Ogre::String& name, const Ogre::String& groupName)
{
    _preprocessScene();

    mSceneInfo->load(parser, name, groupName, isEditable());

    _postprocessScene();
}
//-----------------------------------------------------------------------
void
System::_preprocessScene(void)
{
    resetScene();
}
//-----------------------------------------------------------------------
void
System::_postprocessScene(void)
{
    mBaseScale = Ogre::Math::Sqrt(getTerrainData()->mScale.x * getTerrainData()->mScale.z);

    // Adjust the camera to fit to current scene
    _adjustCamera();

	clearExceptionInfo();

    //bakeStaticGeometries(0);

    // 生成terrain type info
    mTerrainTypeInfos->setTerrainData(mTerrainData);
    mTerrainTypeInfos->updateTerrainTypeInfos();

    // Create render instances
    mSceneInfo->initialise(this);
    
    // 告诉特效系统当前场景的灯光信息
    EffectManager::getSingleton().getMainSceneLight()->updateLightInfo();
}
//-----------------------------------------------------------------------
void
System::bakeStaticGeometries(size_t maxMemoryUsage)
{
    clearStaticGeometries();

    // HACK

    Ogre::StaticGeometry* sg = mSceneManager->createStaticGeometry("Fairy/StaticGeometry/0");
    mStaticGeometries.push_back(sg);
    sg->setRegionDimensions(Ogre::Vector3(2000, 2000, 2000));

    const SceneInfo::Objects& objects = mSceneInfo->getObjects();
    for (SceneInfo::Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        const ObjectPtr& object = *it;
        EntityList entities;
        object->queryBakableEntities(entities);
        if (!entities.empty())
        {
            object->bakeStaticGeometry(sg, entities);
        }
    }

    sg->build();
}
//-----------------------------------------------------------------------
void
System::clearStaticGeometries(void)
{
    std::list<Ogre::StaticGeometry*>::const_iterator it;
    for (it = mStaticGeometries.begin(); it != mStaticGeometries.end(); ++it)
    {
        Ogre::StaticGeometry* sg = *it;
        getSceneManager()->destroyStaticGeometry(sg->getName());
    }

    mStaticGeometries.clear();
}
//-----------------------------------------------------------------------
void
System::_adjustCamera(void)
{
    // Set camera near/far clip base on terrain scale
    mCamera->setNearClipDistance(100.0f);//(0.25f * mBaseScale);//LM修改
    mCamera->setFarClipDistance(10000.f);//(150.0f * mBaseScale);
    // 45 degree fov
    mCamera->setFOVy(Ogre::Degree(115));
}
//////////////////////////////////////////////////////////////////////////
// Render window maintenance
//////////////////////////////////////////////////////////////////////////
bool
System::renderOneFrame(void)
{
    if (!Ogre::Root::getSingletonPtr() || !Ogre::Root::getSingleton().getRenderSystem())
        return false;

    return Ogre::Root::getSingleton().renderOneFrame();
}
//-----------------------------------------------------------------------
void
System::updateWindow(void)
{
    if (mRenderWindow)
    {
        mRenderWindow->update();
    }
}
//-----------------------------------------------------------------------
void
System::resizeWindow(size_t width, size_t height)
{
    if (mRenderWindow && mPostFilterManager)
    {
        if (width != mRenderWindow->getWidth() || height != mRenderWindow->getHeight())
        {
            mRenderWindow->windowMovedOrResized();

            // notify post filter manager
            mPostFilterManager->_notifyViewportSizeChanged(mViewport);
        }
    }
}
//////////////////////////////////////////////////////////////////////////
// General helper
//////////////////////////////////////////////////////////////////////////
Ogre::Ray
System::getWindowToViewportRay(Ogre::Real winx, Ogre::Real winy) const
{
    return mCamera->getCameraToViewportRay(
        (winx - mViewport->getActualLeft()) / mViewport->getActualWidth(),
        (winy - mViewport->getActualTop()) / mViewport->getActualHeight());
}
//-----------------------------------------------------------------------
Ogre::Ray
System::getWindowToViewportRay(const Point& pt) const
{
    return getWindowToViewportRay(pt.x, pt.y);
}
//-----------------------------------------------------------------------
bool
System::getTerrainIntersects(const Ogre::Ray& ray, Ogre::Vector3& position, Ogre::Vector3* normal, bool allowOutside) const
{
    return getTerrainData()->getIntersects(ray, position, normal, allowOutside);
}
//-----------------------------------------------------------------------
bool
System::getTerrainIntersects(Ogre::Real winx, Ogre::Real winy, Ogre::Vector3& position, Ogre::Vector3* normal, bool allowOutside) const
{
    return getTerrainIntersects(getWindowToViewportRay(winx, winy), position, normal, allowOutside);
}
//-----------------------------------------------------------------------
bool
System::getTerrainIntersects(const Point& pt, Ogre::Vector3& position, Ogre::Vector3* normal, bool allowOutside) const
{
    return getTerrainIntersects(pt.x, pt.y, position, normal, allowOutside);
}

void 
System::clearExceptionInfo(void)
{
	mHasExceptionWhenLoadScene = false;
	mExceptionInfoMap.clear();
}

void 
System::addExceptionInfo( const Ogre::String exceptionDesc, const System::ExceptionInfo &info )
{
	// 如果没找到这个异常描述，就表示是一种新的mesh的丢失
	ExceptionInfoMap::iterator i = mExceptionInfoMap.find(exceptionDesc);

	if ( i == mExceptionInfoMap.end() )
	{
		ExceptionInfoList list;
		list.push_back(info);
		mExceptionInfoMap.insert( ExceptionInfoMap::value_type(exceptionDesc, list) );
	}
	else
	{
		assert ( (i->second).size() > 0 );
		(i->second).push_back(info);
	}
	// find out if the mesh has been in the exception list

	mHasExceptionWhenLoadScene = true;
}
//-----------------------------------------------------------------------
void
System::setAnimationQuality(Ogre::Real animationQuality)
{
    if (mAnimationQuality != animationQuality)
    {
        mAnimationQuality = animationQuality;
        getSceneInfo()->broadcastMessage(ANIMATION_QUALITY_CHANGED_MSG, animationQuality);
    }
}
//-----------------------------------------------------------------------
Ogre::Real
System::getAnimationQuality(void) const
{
    return mAnimationQuality;
}
//-----------------------------------------------------------------------
AnimationType
System::_determineAnimationType(Ogre::Real level)
{
    level += mAnimationQuality;
    if (level >= 100)
        return AT_FULL;
    else if (level >= 50)
        return AT_FROZE;
    else
        return AT_DISABLE;
}
//-----------------------------------------------------------------------
void
System::setCreateLevel(Ogre::Real createLevel)
{
    if (mSystemCreateLevel != createLevel)
    {
        mSystemCreateLevel = createLevel;
        getSceneInfo()->broadcastMessage(CREATE_LEVEL_CHANGED_MSG, createLevel);
    }
}
//-----------------------------------------------------------------------
Ogre::Real
System::getCreateLevel(void) const
{
    return mSystemCreateLevel;
}
//-----------------------------------------------------------------------
bool
System::_determineCreateLevel(Ogre::Real level)
{    
    level += mSystemCreateLevel;
    if (level >= 50)
        return true;
    else
        return false;
}
void System::setBackgroundColour(const Ogre::ColourValue& colour)
{
	mBackgrounColour = colour;
	mViewport->setBackgroundColour(colour);
}
}
