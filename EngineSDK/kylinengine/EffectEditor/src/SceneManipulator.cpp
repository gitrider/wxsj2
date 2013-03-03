#include "SceneManipulator.h"

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreStringConverter.h>
#include <OgreRenderTargetListener.h>
#include <OgreException.h>
#include <OgreParticleSystem.h>
#include <OgreLogManager.h>
#include <OgreManualObject.h>

#include "Core/FairyXercesParser.h"
#include "Core/FairyXercesWriter.h"
#include "Core/FairyResources.h"
#include "Core/FrameStatsListener.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyEnviromentObject.h"
#include "Core/Terrain.h"
#include "Core/TerrainTile.h"
#include "Core/FairyUtils.h"
#include "EffectSystem/FairyEffect.h"
#include "WXOperatorManager.h"
#include "WXCreateObjectOperator.h"
#include "WXRenameObjectOperator.h"
#include "WXModifyObjectPropertyOperator.h"

#include "Manip.h"
#include "Action.h"
#include "CameraManips.h"
#include "CreateObjectAction.h"

#include "AddParticleSystemObjectPlugin.h"
#include "AddEffectObjectPlugin.h"
#include "AddLightObjectPlugin.h"

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/variate_generator.hpp>


//#define  __TEST_Projectors__
#ifdef __TEST_Projectors__
#include "Core/FairyProjector.h"
#endif

//~!@
//#include "Core/FairyLightObject.h"
//#include "Core/FairyFlags.h"
//
//#include <OgreLight.h>
//~!@

namespace Fairy {

	const float REAL_CAMERA_HEIGHT = 1367.0f;
    const float REAL_CAMERA_MIN_HEIGHT = 375.0f;
	const float REAL_CAMERA_ANGLE = 43.5f;

//////////////////////////////////////////////////////////////////////////

Manip*
SceneManipulator::getManip(const String& name) const
{
    Manips::const_iterator it = mManips.find(name);
    if (it == mManips.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Can't found manip of '" + name + "'.",
            "SceneManipulator::getManip");
    }

    return it->second;
}

void
SceneManipulator::registerManips(void)
{
    registerManip(new CameraPan(this));
    registerManip(new CameraRotate(this));
    registerManip(new CameraDragMove(this));
    registerManip(new CameraDragRotate(this));
    // TODO: register other usable manips
}

void
SceneManipulator::registerManip(Manip* manip)
{
    std::pair<Manips::iterator, bool> inserted =
        mManips.insert(Manips::value_type(manip->getName(), manip));
    if (!inserted.second)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
            "Manip with name '" + manip->getName() + "' already registered",
            "SceneManipulator::registerManip");
    }
}

void
SceneManipulator::destoryManips(void)
{
    for (Manips::const_iterator it = mManips.begin(); it != mManips.end(); ++it)
    {
        delete it->second;
    }
    mManips.clear();
}



//////////////////////////////////////////////////////////////////////////

Selection*
SceneManipulator::_getSelection(const String& type) const
{
    Selections::const_iterator it = mSelections.find(type);
    if (it == mSelections.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Can't found manip of '" + type + "'.",
            "SceneManipulator::getSelection");
    }

    return it->second;
}
//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::setActiveAction(const String& name)
{
    setActiveAction(name.empty() ? NULL : _getAction(name));
}

void SceneManipulator::setActiveAction(Action* action)
{
    if (mActiveAction != action)
    {
        if (mActiveAction)
            mActiveAction->setActive(false);
        mActiveAction = action;
        if (mActiveAction )
            mActiveAction->setActive(true);
    }
}

void 
SceneManipulator::registerAddObjectPlugins(void)
{
	// 添加各种物体指示器
	mAddObjectPlugins.push_back( new AddLightObjectPlugin(this) );
	mAddObjectPlugins.push_back( new AddParticleSystemObjectPlugin(this) );
	mAddObjectPlugins.push_back( new AddEffectObjectPlugin(this) );
}

void 
SceneManipulator::destroyAddObjectPlugins(void)
{
	for ( size_t i=0; i<mAddObjectPlugins.size(); ++i )
	{
		delete mAddObjectPlugins[i];
	}
}


Action*
SceneManipulator::_getAction(const String& name) const
{
    Actions::const_iterator it = mActions.find(name);
    if (it == mActions.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Can't found manip of '" + name + "'.",
            "SceneManipulator::getAction");
    }

    return it->second;
}

void
SceneManipulator::registerActions(void)
{
	registerAction(new CreateObjectAction(this));
    // TODO: register other usable selections
}

void
SceneManipulator::registerAction(Action* action)
{
    std::pair<Actions::iterator, bool> inserted =
        mActions.insert(Actions::value_type(action->getName(), action));
    if (!inserted.second)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
            "Action with name '" + action->getName() + "' already registered",
            "SceneManipulator::registerAction");
    }
}

void
SceneManipulator::destoryActions(void)
{
    for (Actions::const_iterator it = mActions.begin(); it != mActions.end(); ++it)
    {
        delete it->second;
    }
    mActions.clear();
}

//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::newScene()
{
    _preprocessScene();

	createGrid();
	createTerrain();
    _postprocessScene();

}



void
SceneManipulator::_preprocessScene(void)
{
    System::_preprocessScene();
}

void
SceneManipulator::_postprocessScene(void)
{
    _checkStdObjects(true);
	
	System::_postprocessScene();	

    // Setup camera, make sure we can see the scene
    moveCameraToUsableLocation();

    getOperatorManager()->reset();
    mSnapshot = getOperatorManager()->makeSnapshot();

#ifdef __TEST_Projectors__
    static bool firstTime = true;
    if (firstTime)
    {
        firstTime = false;

        Ogre::SceneNode* node;
        Projector* projector;

        node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
        projector = static_cast<Projector*>
            (mSceneManager->createMovableObject(node->getName(), ProjectorFactory::FACTORY_TYPE_NAME));
        projector->setTerrain(mTerrain);
        projector->setAspectRatio(1);
        projector->setShowFrustum(true);
        projector->setPatchMaterial("Fairy/ProjectorTest");
        node->attachObject(projector);
        node->setPosition(Ogre::Vector3(+250, 500, +250));
        node->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y, node->TS_WORLD);

        node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
        projector = static_cast<Projector*>
            (mSceneManager->createMovableObject(node->getName(), ProjectorFactory::FACTORY_TYPE_NAME));
        projector->setTerrain(mTerrain);
        projector->setAspectRatio(1);
        projector->setShowFrustum(true);
        projector->setPatchMaterial("Fairy/ProjectorTest");
        projector->setEnableClipPlanes(true);
        node->attachObject(projector);
        node->setPosition(Ogre::Vector3(-250, 500, +250));
        node->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y, node->TS_WORLD);

        node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
        projector = static_cast<Projector*>
            (mSceneManager->createMovableObject(node->getName(), ProjectorFactory::FACTORY_TYPE_NAME));
        projector->setTerrain(mTerrain);
        projector->setAspectRatio(1);
        projector->setShowFrustum(true);
        projector->setPatchMaterial("Fairy/ProjectorTest");
        projector->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        projector->setFOVy(Ogre::Math::ATan(2) * 2);
        node->attachObject(projector);
        node->setPosition(Ogre::Vector3(+250, 500, -250));
        node->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y, node->TS_WORLD);

        node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
        projector = static_cast<Projector*>
            (mSceneManager->createMovableObject(node->getName(), ProjectorFactory::FACTORY_TYPE_NAME));
        projector->setTerrain(mTerrain);
        projector->setAspectRatio(1);
        projector->setShowFrustum(true);
        projector->setPatchMaterial("Fairy/ProjectorTest");
        projector->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        projector->setFOVy(Ogre::Math::ATan(2) * 2);
        projector->setEnableClipPlanes(true);
        node->attachObject(projector);
        node->setPosition(Ogre::Vector3(-250, 500, -250));
        node->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y, node->TS_WORLD);
    }
#endif

	//~!@
	/*getSceneManager()->setVisibilityMask(Fairy::OVF_INDICATORS);
	Fairy::LightObject *o = static_cast<Fairy::LightObject *>
			(getSceneInfo()->findObjectByName("#FairyMainLightOne").get());
	o->getLight()->setVisibilityFlags(Fairy::OVF_INDICATORS);*/

	//~!@

#if 0
    static const String msLightmapMaker("Lightmap maker");
    Ogre::Camera* camera = 0;
    try
    {
        camera = mSceneManager->getCamera(msLightmapMaker);
    }
    catch (const Ogre::Exception& e)
    {
        if (e.getNumber() != e.ERR_ITEM_NOT_FOUND)
            throw;
    }

    if (!camera)
    {
        camera = mSceneManager->createCamera(msLightmapMaker);
        camera->setAutoAspectRatio(true);
        camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        camera->setFOVy(Ogre::Degree(90));

        Ogre::Quaternion orientation;
        orientation.FromAxes(Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y);
        camera->setOrientation(orientation);

        Ogre::Viewport* viewport = mRenderWindow->addViewport(camera, 1, 0, 0, 0.5, 0.5);
        viewport->setOverlaysEnabled(false);
        viewport->setSkiesEnabled(false);
        viewport->setShadowsEnabled(true);

        Ogre::RenderQueueInvocationSequence* rqis =
            mRoot->createRenderQueueInvocationSequence(msLightmapMaker);
        Ogre::RenderQueueInvocation* rqi =
            rqis->add(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1, "World Geometry");
    }

    mSceneManager->_updateSceneGraph(camera);
    Ogre::AxisAlignedBox aabb;
    Ogre::SceneManager::MovableObjectIterator itm =
        mSceneManager->getMovableObjectIterator(Ogre::EntityFactory::FACTORY_TYPE_NAME);
    while (itm.hasMoreElements())
    {
        Ogre::MovableObject* movable = itm.getNext();
        aabb.merge(movable->getWorldBoundingBox(true));
    }

    camera->setNearClipDistance(32 * getBaseScale() / 2);
    camera->setFarClipDistance(camera->getNearClipDistance() + aabb.getMaximum().y - aabb.getMinimum().y);
    camera->setPosition(0, camera->getNearClipDistance() + aabb.getMaximum().y, 0);

    mSceneManager->setShadowFarDistance(camera->getNearClipDistance() * Ogre::Math::Sqrt(1 + Ogre::Math::Sqr(camera->getAspectRatio())));
    mSceneManager->setShadowDirLightTextureOffset(camera->getPosition().y / mSceneManager->getShadowFarDistance());
#endif

    _fireSceneReset();
}
void SceneManipulator::_createTerrain(size_t width, size_t depth, size_t tileSize,
								   const String& baseTexture,
								   size_t seed, Real scale, Real height, Real h)
{
	//生成随机数
	std::auto_ptr<boost::minstd_rand> engine;
	if (seed)
		engine.reset(new boost::minstd_rand(seed));

	TerrainData* data = mSceneInfo->getTerrainData();

	data->mXSize = width;
	data->mZSize = depth;
	data->mTileSize = tileSize;
	data->mScale = Ogre::Vector3(scale,scale,scale);
	data->mPosition = Ogre::Vector3(- data->mScale.x * data->mXSize / 2,0,- data->mScale.z * data->mZSize / 2);
	data->_updateDerivedFields();

	data->mMaterialTemplates.clear();
	data->mMaterialTemplates["OneLayer"] = "Terrain/OneLayer";
	data->mMaterialTemplates["OneLayerLightmap"] = "Terrain/OneLayerLightmap";
	data->mMaterialTemplates["TwoLayer"] = "Terrain/TwoLayer";
	data->mMaterialTemplates["TwoLayerLightmap"] = "Terrain/TwoLayerLightmap";

	data->mPixmaps.clear();
	data->mTextures.clear();
	if (!baseTexture.empty())
	{
		data->mPixmaps.push_back(TerrainData::Pixmap(0));
		data->mTextures.push_back(baseTexture);
	}

	TerrainData::GridInfo gridInfo = { 0, 0, 0, 0, 0 };
	if (!data->mPixmaps.empty())
		gridInfo.layers[0].pixmapId = 1;
	std::fill(data->mGridInfos.begin(), data->mGridInfos.end(), gridInfo);

}
void
SceneManipulator::loadScene(const String& filename)
{
	static const String TEMP_GROUP_NAME = "#TEMP#";

	String baseName, path;
	Ogre::StringUtil::splitFilename(filename, baseName, path);

	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
	rgm.addResourceLocation(path, "FileSystem", TEMP_GROUP_NAME, false);

	loadSceneFromResource(mXMLParser, baseName, TEMP_GROUP_NAME);

	rgm.destroyResourceGroup(TEMP_GROUP_NAME);

}
void
SceneManipulator::resetScene(void)
{
    mSelectedObjects.clear();

    System::resetScene();
}

void
SceneManipulator::_checkStdObjects(bool autoCreate)
{
    ObjectPtr object;
    SceneInfo::ObjectsByTypeRange objects;

    objects = mSceneInfo->findObjectsByType("Enviroment");
    if (objects.first == objects.second)
    {
        if (autoCreate)
        {
            object = ObjectFactoryManager::getSingleton().createInstance("Enviroment");
			object->setName(FairyAutoEnviromentObjectName);
            mSceneInfo->addObject(object);
        }
    }

    objects = mSceneInfo->findObjectsByType("Light");
    if (objects.first == objects.second)
    {
        if (autoCreate)
        {
            object = ObjectFactoryManager::getSingleton().createInstance("Light");
			object->setName(FairyAutoMainLightOneObjectName);
            object->setPropertyAsString("type", "directional");
            object->setProperty("direction", Ogre::Vector3(-1, -1, -1));
            object->setProperty("diffuse", Ogre::ColourValue(0.6, 0.6, 0.6));
            object->setProperty("specular", Ogre::ColourValue(0.3, 0.3, 0.3));
            mSceneInfo->addObject(object);

            object = ObjectFactoryManager::getSingleton().createInstance("Light");
            object->setName(FairyAutoMainLightTwoObjectName);
            object->setPropertyAsString("type", "directional");
            object->setProperty("direction", Ogre::Vector3(+1, -1, -1));
            object->setProperty("diffuse", Ogre::ColourValue(0.1, 0.1, 0.1));
            object->setProperty("specular", Ogre::ColourValue(0.0, 0.0, 0.0));
            mSceneInfo->addObject(object);
        }
    }

    static const String gsSkyTypes[] =
    {
        "SkyBox",
        "SkyDome",
        "SkyPlane",
    };

    /*bool existsSky = false;
    for (size_t i = 0; i < sizeof(gsSkyTypes)/sizeof(*gsSkyTypes); ++i)
    {
	    objects = mSceneInfo->findObjectsByType(gsSkyTypes[i]);
	    if (objects.first != objects.second)
        {
            existsSky = true;
            break;
        }
    }

	if (!existsSky)
    {
		if (autoCreate)
		{
			object = ObjectFactoryManager::getSingleton().createInstance("SkyDome");
            object->setName(FairyAutoSkyDomeObjectName);
            object->setPropertyAsString("material", "CloudySky");   
			mSceneInfo->addObject(object);
		}
	}*/
}


bool
SceneManipulator::isModified(void) const
{
    return !getOperatorManager()->checkSnapshot(mSnapshot);
}

bool
SceneManipulator::isEditable(void) const
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

SceneManipulator::SceneManipulator(void)
    : mXMLParser(NULL)
    , mXMLWriter(NULL)
    , mOperatorManager(NULL)
    , mIndicatorRootSceneNode()
	, mGridNode(NULL)
	, m_baseNode(NULL)
	, m_axex(NULL)
	, mDummy(NULL)
	, mAxis_x(NULL)
	, mAxis_y(NULL)
	, mAxis_z(NULL)
	, mAxisNode_x(NULL)
	, mAxisNode_y(NULL)
	, mAxisNode_z(NULL)
    , mObjectsUseBoxCenter(false)
    , mAxisGizmoActive(false)
    , mIndicatorNodeVisible(true)
{
    mXMLParser = new XercesParser;
    mXMLWriter = new XercesWriter;

    mMoveFactor = 10;
    mRotateFactor = 360;
    mMoveSpeed = 30;
    mRotateSpeed = 36;
    mWheelSpeed = 2;
    mCameraHeightLocked = false;
    mShowStandardModel = false;
    mHeightAdjustSpeed = 3;
    mPositionAlignGranularity = 0;
    mRotationAlignGranularity = 0;
    mBrushType = Ogre::StringUtil::BLANK;
    mBrushXSize = 3;
    mBrushZSize = 3;

    // 默认的真实摄像机高度，角度
	mDefaultRealCameraAngle = REAL_CAMERA_ANGLE;
	mDefaultRealCameraHeight = REAL_CAMERA_HEIGHT;

    mRealCameraHeight = mDefaultRealCameraHeight;
    mRealCameraAngle = mDefaultRealCameraAngle;

    mRealLastCameraAngle = mRealCameraAngle;
	mUseRealCameraHeight = false;
	mUseRealCameraAngle = false;

    mActiveAction = NULL;

	mAffectRange = 0;
	mExponentValue = 0.0f;


	fillUnableCenterSelectObject();
	mSelectAllObjects = true;

}

SceneManipulator::~SceneManipulator()
{
    destoryActions();
    destoryManips();
    delete mOperatorManager;

    delete mXMLWriter;
    delete mXMLParser;


    if (mIndicatorRootSceneNode)
    {
        getSceneManager()->destroySceneNode(mIndicatorRootSceneNode);
		mIndicatorRootSceneNode = NULL;
    }

	destoryGrid();
	
}

//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::_loadResources(void)
{
    Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();

    // Declare editor special resources
/*    rgm.declareResource("pointLight.mesh", "Mesh", EDITOR_RESOURCE_GROUP_NAME);
    rgm.declareResource("spotLight.mesh", "Mesh", EDITOR_RESOURCE_GROUP_NAME);
    rgm.declareResource("directionalLight.mesh", "Mesh", EDITOR_RESOURCE_GROUP_NAME);*/
    rgm.declareResource("standardmodel.mesh", "Mesh", DEFAULT_RESOURCE_GROUP_NAME);

    System::_loadResources();

    // Load editor special resources
/*	rgm.loadResourceGroup(EDITOR_RESOURCE_GROUP_NAME);*/
}

void
SceneManipulator::_initScene(void)
{
    System::_initScene();

    mIndicatorRootSceneNode = getBaseSceneNode()->createChildSceneNode();
	mGridNode = getSceneManager()->getRootSceneNode()->createChildSceneNode("gridNode");
	mGridNode->setPosition(0,0,0);

	if (!m_axex && !mDummy && !m_baseNode)
	{
		m_baseNode = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		m_axex = m_baseNode->createChildSceneNode();

		mDummy = getSceneManager()->createEntity("MeshViewerCnDummy","axes.mesh");
		Ogre::Vector3 scale = Ogre::Vector3(2,2,2);
		m_axex->setScale(scale);

		Ogre::Vector3 position;
		position = Ogre::Vector3(0,0,0);

		m_axex->translate(position);
		m_axex->attachObject((Ogre::MovableObject*)mDummy);	

		// Create axis balls
		Ogre::MeshPtr sphereMesh = createCommonSphere("AxisEntity_x",1.5, 16, 16);
		// X
		m_pMaterialX = createPureColourMaterial(
			Ogre::ColourValue(1, 0, 0, 0.5)  );

		mAxis_x = getSceneManager()->createEntity("AxisEntity_x", sphereMesh->getName());
		m_pMaterialX->setDepthCheckEnabled(false);
		m_pMaterialX->setDepthWriteEnabled(false);
		m_pMaterialX->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

		//david-<<
		//mAxis_x->setNormaliseNormals(true);
		//david->>
		mAxis_x->setMaterialName(m_pMaterialX->getName());
		mAxisNode_x = m_axex->createChildSceneNode();
		mAxisNode_x->translate(Ogre::Vector3(13,0,0));
		mAxisNode_x->attachObject(mAxis_x);

		// Y
		m_pMaterialY = createPureColourMaterial(
			Ogre::ColourValue(0, 1, 0, 0.5)  );

		mAxis_y = getSceneManager()->createEntity("AxisEntity_y", sphereMesh->getName());
		m_pMaterialY->setDepthCheckEnabled(false);
		m_pMaterialY->setDepthWriteEnabled(false);
		m_pMaterialY->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

		//david-<<
		//mAxis_y->setNormaliseNormals(true);
		//david->>
		mAxis_y->setMaterialName(m_pMaterialY->getName());
		mAxisNode_y = m_axex->createChildSceneNode();
		mAxisNode_y->translate(Ogre::Vector3(0,13,0));
		mAxisNode_y->attachObject(mAxis_y);

		// Z
		m_pMaterialZ = createPureColourMaterial(
			Ogre::ColourValue(0, 0, 1, 0.5)  );

		mAxis_z = getSceneManager()->createEntity("AxisEntity_z", sphereMesh->getName());
		m_pMaterialZ->setDepthCheckEnabled(false);
		m_pMaterialZ->setDepthWriteEnabled(false);
		m_pMaterialZ->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);
		//david-<<
		//mAxis_z->setNormaliseNormals(true);
		//david->>
		mAxis_z->setMaterialName(m_pMaterialZ->getName());
		mAxisNode_z = m_axex->createChildSceneNode();
		mAxisNode_z->translate(Ogre::Vector3(0,0,13));
		mAxisNode_z->attachObject(mAxis_z);

		m_baseNode->setVisible(false);
	}

    mOperatorManager = new OperatorManager;

    registerManips();

    registerActions();

}

//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::moveCameraToUsableLocation(void)
{

	Ogre::Vector3 centre = Ogre::Vector3::ZERO;
	mCamera->setPosition( Ogre::Vector3(1,1,1) );
  //  mCamera->lookAt(data->getPositionAt(centre.x, centre.z - 20 * mBaseScale));
    mCamera->setOrientation(Ogre::Quaternion(Ogre::Degree(-mRealCameraAngle), Ogre::Vector3::UNIT_X));

    // 设置摄像机的fov为35
    mCamera->setFOVy(Ogre::Degree(35.0f));
}




//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::addObject(const ObjectPtr& object, SceneListener* exclude, bool disableUndo)
{
    mSceneInfo->addObject(object);
    _fireAddObject(object, exclude);

    if (!disableUndo)
    {
        CreateObjectOperator* op = new CreateObjectOperator(this);
        op->add(object, true);
        getOperatorManager()->addOperator(op);
    }
}

void
SceneManipulator::removeObject(const ObjectPtr& object, SceneListener* exclude, bool disableUndo)
{
    if (!disableUndo)
    {
        CreateObjectOperator* op = new CreateObjectOperator(this);
        op->add(object, false);
        getOperatorManager()->addOperator(op);
    }

    deselectObject(object, exclude);
    _fireRemoveObject(object, exclude);
    mSceneInfo->removeObject(object);
}

void
SceneManipulator::renameObject(const ObjectPtr& object, const String& newName, SceneListener* exclude, bool disableUndo)
{
    String oldName = object->getName();
    mSceneInfo->renameObject(object, newName);
    _fireRenameObject(object, oldName, exclude);

    if (!disableUndo)
    {
        RenameObjectOperator* op = new RenameObjectOperator(this);
        op->add(oldName, newName);
        getOperatorManager()->addOperator(op);
    }
}

void 
SceneManipulator::removeObject(const Ogre::String objName)
{
	const ObjectPtr &object = getSceneInfo()->findObjectByName(objName);
	assert (object->getName() == objName);

	_fireRemoveObject(object);
	mSceneInfo->removeObject(objName);
}

//////////////////////////////////////////////////////////////////////////

const SceneManipulator::ObjectSet&
SceneManipulator::getSelectedObjects(void) const
{
    return mSelectedObjects;
}

bool
SceneManipulator::hasSelectedObjects(void) const
{
    return !mSelectedObjects.empty();
}

bool
SceneManipulator::isObjectSelected(const ObjectPtr& object) const
{
    assert(object);

    return mSelectedObjects.find(object) != mSelectedObjects.end();
}

bool
SceneManipulator::selectObject(const ObjectPtr& object, SceneListener* exclude)
{
    assert(object);
    if (!object)
        return false;

    if (!mSelectedObjects.insert(object).second)
        return false;

    _fireSelectObject(object, exclude);
    return true;
}

bool
SceneManipulator::deselectObject(const ObjectPtr& object, SceneListener* exclude)
{
    assert(object);
    if (!object)
        return false;

    if (mSelectedObjects.erase(object) <= 0)
        return false;

    _fireDeselectObject(object, exclude);
    return true;
}

bool
SceneManipulator::clearSelectedObjects(SceneListener* exclude)
{
    if (mSelectedObjects.empty())
        return false;

    _fireDeselectAllObjects(exclude);
    mSelectedObjects.clear();
    return true;
}

bool
SceneManipulator::removeSelectedObjects(SceneListener* exclude, bool disableUndo)
{
    if (mSelectedObjects.empty())
        return false;

    _fireDeselectAllObjects(exclude);

    CreateObjectOperator* op = NULL;
    if (!disableUndo)
        op = new CreateObjectOperator(this);

    for (ObjectSet::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
    {
        const ObjectPtr& object = *it;
        if (op)
            op->add(object, false);
        _fireRemoveObject(object, exclude);
        mSceneInfo->removeObject(object);
    }

    if (op)
        getOperatorManager()->addOperator(op);

    mSelectedObjects.clear();
    return true;
}

bool 
SceneManipulator::addOjbects(const ObjectSet& objects, SceneListener* exclude, bool disableUndo)
{
    if (objects.empty())
        return false;

    CreateObjectOperator* op = NULL;
    if (!disableUndo)
        op = new CreateObjectOperator(this);

    for (ObjectSet::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        const ObjectPtr& object = *it;
        if (op)
            op->add(object, true);
        _fireAddObject(object, exclude);
        mSceneInfo->addObject(object);
    }

    if (op)
        getOperatorManager()->addOperator(op);

    return true;
}


Ogre::Quaternion 
SceneManipulator::_alignRotation(const Ogre::Quaternion& value)
{
    if(mRotationAlignGranularity > 0)
        return _alignRotation(value, mRotationAlignGranularity);

    return value;
}

void 
SceneManipulator::calcOffsetRadian(const Ogre::Degree& degree, const float& granularity,float& remainder)
{
    int  multipleCeil = Ogre::Math::Ceil(degree.valueDegrees() / (granularity / 2.0f));
    int  multipleFloor = Ogre::Math::Floor(degree.valueDegrees() /( granularity / 2.0f));
    int multiple = (multipleCeil % 2) ? multipleFloor : multipleCeil;
	//if( multiple == 0 ) multiple = 1;

    remainder = granularity / 2.0f * (float)(multiple) - degree.valueDegrees();
}

Ogre::Quaternion 
SceneManipulator::_alignRotation(const Ogre::Quaternion& value, Real granularity)
{
    Ogre::Quaternion r(value);
  
    Ogre::Matrix3 matRot;
    r.ToRotationMatrix(matRot);
    matRot.Orthonormalize();
    Ogre::Radian rfYAngle,rfPAngle,rfRAngle;
    
    float remainder;

    if(getActiveAction()->getParameter("%CurrentMode") == "RotateX")
    {
        matRot.ToEulerAnglesXZY(rfYAngle,rfPAngle,rfRAngle);
        Ogre::Degree degree(rfYAngle.valueDegrees());
          
        calcOffsetRadian(degree,granularity,remainder);
        Ogre::Quaternion q(Ogre::Degree(remainder),Ogre::Vector3::UNIT_X);
        r = q * r; 
    }
    else if(getActiveAction()->getParameter("%CurrentMode") == "RotateY")
    {
        matRot.ToEulerAnglesYXZ(rfYAngle,rfPAngle,rfRAngle);
        Ogre::Degree degree(rfYAngle.valueDegrees());
    
        calcOffsetRadian(degree,granularity,remainder);
        Ogre::Quaternion q(Ogre::Degree(remainder),Ogre::Vector3::UNIT_Y);
        r = q * r; 
    }
    else if(getActiveAction()->getParameter("%CurrentMode") == "RotateZ")
    {
        matRot.ToEulerAnglesZYX(rfYAngle,rfPAngle,rfRAngle);
        Ogre::Degree degree(rfYAngle.valueDegrees());

        calcOffsetRadian(degree,granularity,remainder);
        Ogre::Quaternion q(Ogre::Degree(remainder),Ogre::Vector3::UNIT_Z);
        r = q * r; 
    }

    return r;
}

void 
SceneManipulator::_snapSelectedObjectsRotation(Real granularity,int axis)
{
    if(granularity > 0)
    {
        std::auto_ptr<ModifyObjectPropertyOperator> op(new ModifyObjectPropertyOperator(this));

        for (ObjectSet::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
        {
            const ObjectPtr& object = *it;
            if (object->hasProperty("orientation"))
            {
                Ogre::Quaternion oldOrientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
                switch(axis) 
                {
                case 0:
                    getActiveAction()->setParameter("%CurrentMode" , "RotateX");
                	break;
                case 1:
                    getActiveAction()->setParameter("%CurrentMode" , "RotateY");
                	break;
                case 2:
                    getActiveAction()->setParameter("%CurrentMode" , "RotateZ");
                    break;
                }                
                Ogre::Quaternion newOrientation = _alignRotation(oldOrientation, granularity);
                if (newOrientation != oldOrientation)
                {
                    object->setProperty("orientation", newOrientation);
                    _fireObjectPropertyChanged(object, "Orientation");
                    op->add(object->getName(), "orientation", oldOrientation, newOrientation);
                }
            }
        }

        if (!op->empty())
            getOperatorManager()->addOperator(op.release());
    }
}


bool 
SceneManipulator::getUseBoxCenterSelection()
{
    return mObjectsUseBoxCenter;
}

void
SceneManipulator::setUseAxis(bool use)
{  
}

bool 
SceneManipulator::getUseAxis()
{
    return mAxisGizmoActive;
}

void 
SceneManipulator::setRealCameraAngle( const Real angle ) 
{ 
    mRealCameraAngle = angle;   

    /** dir为目标方向
        已知条件 : dir.y == -sin(alpha) ， 原有方向 curDir
                    /
                   /
                  /   
        alpha   \/angle
        -------------------------------
        现在要求出dir的x和z，
        (curDir.x/s)^2 + dir.y^2 + (curDir.y/s)^2 == 1
    */
    Ogre::Vector3 direction = mCamera->getDirection();
    Real v = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(angle));
    Real s = Ogre::Math::Sqrt((direction.x*direction.x + direction.z*direction.z) / (1-v*v));
    direction.x /= s;
    direction.z /= s;
    direction.y = -v;
    mCamera->setDirection(direction);
} 

void 
SceneManipulator::setRealCamera( const Real currentX, const Real currentY )
{
    mCamera->setPosition( currentX, 20, currentY ); 
}

void
SceneManipulator::parseStringAndCreateObject( const Ogre::String &infoString, Ogre::Vector3 &position )
{
    // 根据clipboard中的内容来构建一个临时的数据流，所有的解析工作是在这个数据流上做的
    Ogre::MemoryDataStream* stream = new Ogre::MemoryDataStream((void *)(infoString.c_str()),
        infoString.length(), false);

    // 用于临时保存要添加的物体
    Fairy::SceneManipulator::ObjectSet addedObjects;

    Ogre::String line;
    // 开始进行数据流的操作
    while(!stream->eof())
    {
        line = stream->getLine();

        // 根据字符串的开头来判断要拷贝的是什么物体
        // object
        if ( line.substr(0,6) == "object" )
        {                    
            Ogre::String objectType = line.substr(7,line.size());
            Fairy::ObjectPtr tempObjectPtr;
            tempObjectPtr = Fairy::ObjectFactoryManager::getSingleton().createInstance(objectType);

            // 排除 { 这一行
            stream->skipLine();

            Ogre::String valueLine;
            Ogre::StringVector vecparams;

            valueLine = stream->getLine();

            while ( valueLine != "}" )
            {
                // 参数是用＝来分隔的
                vecparams = Ogre::StringUtil::split(valueLine, "=");

                Ogre::String paraName, paraValue;                         

                paraName = vecparams[0];
                paraValue = vecparams[1];

                // 解析下一行
                valueLine = stream->getLine();

                // 物体的位置属性比较特殊，因为paste时物体应在鼠标的位置附近，所以要特殊处理
                if ( paraName == "position" )
                {
                    // 这里取出的位置只是该物体相对于这次选中的第一个物体的位置
                    Ogre::Vector3 relativePos = Ogre::StringConverter::parseVector3(paraValue);

                    // 加上鼠标与地面的交接处得到实际的位置
                    Ogre::Vector3 worldPos = relativePos + position;

                    // 如果该物体有接地点，还要加上接地点的位置
                    if (tempObjectPtr->hasProperty("grounding point"))
                    {
                        // Adjust position by grounding point
                        Ogre::Vector3 groundingPoint = Fairy::VariantCast<Ogre::Vector3>(tempObjectPtr->getProperty("grounding point"));
                        worldPos -= groundingPoint;
                    }
                    paraValue = Ogre::StringConverter::toString( worldPos );
                }

                // 设置物体的各项属性
                tempObjectPtr->setPropertyAsString(paraName, paraValue);
            }

            // 创建物体
            tempObjectPtr->createRenderInstance(this);
            // 设置物体的名称（自己起名，避免名字重复）
            tempObjectPtr->setName(getSceneInfo()->generateAutoName(tempObjectPtr));

            std::pair<Fairy::SceneManipulator::ObjectSet::iterator, bool> inserted =
                addedObjects.insert(tempObjectPtr);
            assert(inserted.second);
        }   
    }

    if (addedObjects.size() >0)
        // 整批地添加物体（为了能整批地undo）
        addOjbects(addedObjects);

    // 删除临时数据流
    delete stream;
}

void 
SceneManipulator::getSelectedObjectInfo( Ogre::String &infoString )
{
    const Fairy::SceneManipulator::ObjectSet &objects = this->getSelectedObjects();

    Fairy::SceneManipulator::ObjectSet::const_iterator i = objects.begin();

    /** 构建选中的物体的属性字符串
    字符串格式如下：
    /////////////////////

    object 物体类型
    {
    属性名=属性值
    }
    object 物体类型
    {
    属性名=属性值
    属性名=属性值
    ...
    }
    ////////////////////
    在粘贴时要根据这个格式来解析字符串，得到物体的信息
    */
    while ( i!=objects.end() )
    {
        const ObjectPtr& object = *i;
        Ogre::String objectType = object->getType();
        infoString += "object ";
        infoString += objectType.c_str();
        infoString += "\n{\n";

        Ogre::Vector3 firstObjectPos;
        // 物体的属性
        const Fairy::PropertyList& properties = object->getProperties();
        for (Fairy::PropertyList::const_iterator itProperty = properties.begin(); itProperty != properties.end(); ++itProperty)
        {
            const Ogre::String& name = itProperty->name;
            if (!(object->getPropertyFlags(name) & PF_DERIVED))
            {
                Ogre::String value = object->getPropertyAsString(name);

                // 拷贝时得到物体的位置是世界坐标，应转成相对于该组选中物体的相对坐标
                if ( name == "position" )
                {
                    if ( i == objects.begin() )
                    {
                        // 第一个物体的相对坐标为0
                        firstObjectPos = Ogre::StringConverter::parseVector3(value);
                        value = "0 0 0";
                    }
                    else
                    {
                        Ogre::Vector3 currentObjectPos = Ogre::StringConverter::parseVector3(value);
                        value = Ogre::StringConverter::toString( Ogre::Vector3(currentObjectPos-firstObjectPos) );
                    }
                }

                // 因为第一个物体的值为0，为默认值，所以要加上后面这个条件，第一个物体的位置才会被加上
                if (value != itProperty->defaultValue || name == "position")
                {
                    infoString += name.c_str();
                    infoString += "=";
                    infoString += value.c_str();
                    infoString += "\n"; 
                }
            }
        }

        // 一个物体的属性完了，换行
        infoString += "}\n";

        ++i;
    }    
}

String 
SceneManipulator::getTexGroupNameFromTextureName( String &textureName )
{
	// 如果该纹理名称没包含/，它就是直接放在brush根目录下的，一般不允许这样做，就算是这组纹理只有一张贴图
	// 也必须在brush文件夹下建一个新目录
	size_t pos = textureName.find_last_of('/');

	assert (pos != String::npos);
	
	return textureName.substr(0,pos);	
}


void 
SceneManipulator::reloadPaintInfo(void)
{


}

bool SceneManipulator::IsCenterSelectObject( Ogre::String objectType )
{
	UnableCentenSelectObject::const_iterator i = mUnableCentenSelectObject.begin();

	// 遍历不属于中心点选择的物体的类型列表，如果列表中没有，说明这个物体是属于可以中心点选择的
	while ( i != mUnableCentenSelectObject.end() )
	{
		if (objectType != *i)
			++i;
		else
			return false;
	}

	return true;
}

void SceneManipulator::fillUnableCenterSelectObject(void)
{
	mUnableCentenSelectObject.clear();
	mUnableCentenSelectObject.push_back("Light");
	mUnableCentenSelectObject.push_back("TerrainLiquid");
	mUnableCentenSelectObject.push_back("ParticleSystem");
}

void SceneManipulator::setIndicatorNodeVisible(bool visible)
{
    if (mIndicatorRootSceneNode)
    {
        mIndicatorRootSceneNode->setVisible(visible);
        mIndicatorNodeVisible = visible;
    }    
}

void SceneManipulator::setCameraPosition(const Ogre::Vector3& pos)
{
    // TODO: transform newPosition from world space to local space
    mCamera->setPosition(pos);

    if ( mUseRealCameraHeight )
        setRealCamera(pos.x, pos.z);

    _fireCameraPosChanged();

	SceneInfo::ObjectsByTypeRange objects;
	objects = mSceneInfo->findObjectsByType("Enviroment");
	if (objects.first != objects.second)
	{
		EnviromentObject* enviromentObject = static_cast<EnviromentObject*>(objects.first->get());
		enviromentObject->setWeatherParticleSystemPosition(pos);
	}
}
void SceneManipulator::createTerrain()
{
	_createTerrain(16,16,32,"丘陵山01.jpg",0,100,100);
}
void SceneManipulator::setTerrainVisible(bool visible)
{
	int iXSize = mTerrainData->getXSize();
	int iZSize = mTerrainData->getZSize();
	int iNumTilePerX = (iXSize - 1) / mTerrainData->mTileSize + 1;
	int iNumTilePerZ = (iZSize - 1) / mTerrainData->mTileSize + 1;
	for(int i = 0;i<iNumTilePerX;i++)
	{
		for(int j = 0;j<iNumTilePerZ;j++)
		{
			Fairy::TerrainTile* pTile = mTerrain->getTile(i,j);
			if(pTile)
				pTile->setVisible(visible);
		}
	}
}
void SceneManipulator::destoryGrid()
{
	if(mGridNode)
	{
		getSceneManager()->destroySceneNode(mGridNode);
		mGridNode = NULL;
	}
	getSceneManager()->destroyManualObject("gridObject");
	getSceneManager()->destroyManualObject("plusXAxis");
	getSceneManager()->destroyManualObject("negativeXAxis");
	getSceneManager()->destroyManualObject("plusYAxis");
	getSceneManager()->destroyManualObject("negativeYAxis");
	getSceneManager()->destroyManualObject("plusZAxis");
	getSceneManager()->destroyManualObject("negativeZAxis");
	getSceneManager()->destroySceneNode("axesNode");
	getSceneManager()->destroyEntity("axes");
}
void SceneManipulator::createGrid()
{
	Ogre::ManualObject* gridObject = getSceneManager()->createManualObject("gridObject");

	Ogre::MaterialPtr gridMaterial;
	gridMaterial = createPureColourMaterial(
		Ogre::ColourValue(0.6, 0.6, 0.6, 0.6)  );
	gridMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

	Ogre::MaterialPtr plusXAxisMaterial;
	plusXAxisMaterial = createPureColourMaterial(
		Ogre::ColourValue(0.8, 0, 0, 0.8)  );
	plusXAxisMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

	Ogre::MaterialPtr negativeXAxisMaterial;
	negativeXAxisMaterial = createPureColourMaterial(
		Ogre::ColourValue(0.6, 0, 0, 0.7)  );
	negativeXAxisMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);


	Ogre::MaterialPtr plusYAxisMaterial;
	plusYAxisMaterial = createPureColourMaterial(
		Ogre::ColourValue(0, 0.7, 0, 0.7)  );
	plusYAxisMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

	Ogre::MaterialPtr negativeYAxisMaterial;
	negativeYAxisMaterial = createPureColourMaterial(
		Ogre::ColourValue(0, 0.5, 0, 0.6)  );
	negativeYAxisMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);


	Ogre::MaterialPtr plusZAxisMaterial;
	plusZAxisMaterial = createPureColourMaterial(
		Ogre::ColourValue(0, 0, 0.8, 0.8)  );
	plusZAxisMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);

	Ogre::MaterialPtr negativeZAxisMaterial;
	negativeZAxisMaterial = createPureColourMaterial(
		Ogre::ColourValue(0, 0, 0.6, 0.7)  );
	negativeZAxisMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_COLOUR);



	Ogre::ManualObject* plusXAxis = getSceneManager()->createManualObject("plusXAxis");
	Ogre::ManualObject* negativeXAxis = getSceneManager()->createManualObject("negativeXAxis");
	Ogre::ManualObject* plusYAxis = getSceneManager()->createManualObject("plusYAxis");
	Ogre::ManualObject* negativeYAxis = getSceneManager()->createManualObject("negativeYAxis");
	Ogre::ManualObject* plusZAxis = getSceneManager()->createManualObject("plusZAxis");
	Ogre::ManualObject* negativeZAxis = getSceneManager()->createManualObject("negativeZAxis");

	//画网格

	gridObject->begin(gridMaterial->getName(),Ogre::RenderOperation::OT_LINE_LIST);

	int i = 0;
	int iXcoord = 0;
	int iZcoord = 0;
	
	int iGrids = 0;
	for(;i<10;i++)
	{
		iXcoord = -((iGrids+1)* 50);
		gridObject->position(iXcoord,0,-500);
		gridObject->index(i*2);
		gridObject->position(iXcoord,0, 500);
		gridObject->index(i*2+1);
		iGrids++;

	}
	iGrids = 0;
	for(;i<20;i++)
	{
		iXcoord = (iGrids+1)* 50;
		gridObject->position(iXcoord,0,-500);
		gridObject->index(i*2);
		gridObject->position(iXcoord,0, 500);
		gridObject->index(i*2+1);
		iGrids++;

	}
	iGrids = 0;
	for(;i<30;i++)
	{
		iZcoord = -((iGrids+1)* 50);
		gridObject->position(-500,0,iZcoord);
		gridObject->index(i*2);
		gridObject->position(500,0,iZcoord);
		gridObject->index(i*2+1);
		iGrids++;

	}
	iGrids = 0;
	for(;i<40;i++)
	{
		iZcoord = (iGrids+1)* 50;
		gridObject->position(-500,0,iZcoord);
		gridObject->index(i*2);
		gridObject->position(500,0,iZcoord);
		gridObject->index(i*2+1);
		iGrids++;

	}
	gridObject->end();

	plusXAxis->begin(plusXAxisMaterial->getName(),Ogre::RenderOperation::OT_LINE_LIST);
	plusXAxis->position(0,0,0);
	plusXAxis->index(0);
	plusXAxis->position(500,0,0);
	plusXAxis->index(1);
	plusXAxis->end();

	negativeXAxis->begin(negativeXAxisMaterial->getName(),Ogre::RenderOperation::OT_LINE_LIST);
	negativeXAxis->position(0,0,0);
	negativeXAxis->index(0);
	negativeXAxis->position(-500,0,0);
	negativeXAxis->index(1);
	negativeXAxis->end();

	plusYAxis->begin(plusYAxisMaterial->getName(),Ogre::RenderOperation::OT_LINE_LIST);
	plusYAxis->position(0,0,0);
	plusYAxis->index(0);
	plusYAxis->position(0,300,0);
	plusYAxis->index(1);
	plusYAxis->end();

	negativeYAxis->begin(negativeYAxisMaterial->getName(),Ogre::RenderOperation::OT_LINE_LIST);
	negativeYAxis->position(0,0,0);
	negativeYAxis->index(0);
	negativeYAxis->position(0,-300,0);
	negativeYAxis->index(1);
	negativeYAxis->end();

	plusZAxis->begin(plusZAxisMaterial->getName(),Ogre::RenderOperation::OT_LINE_LIST);
	plusZAxis->position(0,0,0);
	plusZAxis->index(0);
	plusZAxis->position(0,0,500);
	plusZAxis->index(1);
	plusZAxis->end();

	negativeZAxis->begin(negativeZAxisMaterial->getName(),Ogre::RenderOperation::OT_LINE_LIST);
	negativeZAxis->position(0,0,0);
	negativeZAxis->index(0);
	negativeZAxis->position(0,0,-500);
	negativeZAxis->index(1);
	negativeZAxis->end();

	Ogre::Entity* axes = mSceneManager->createEntity("axes","axes.mesh");
	Ogre::SceneNode * aixesNode = mGridNode->createChildSceneNode("axesNode");
	aixesNode->setScale(4,4,4);
	aixesNode->attachObject(axes);
	mGridNode->attachObject(gridObject);
	mGridNode->attachObject(plusXAxis);
	mGridNode->attachObject(negativeXAxis);
	mGridNode->attachObject(plusYAxis);
	mGridNode->attachObject(negativeYAxis);
	mGridNode->attachObject(plusZAxis);
	mGridNode->attachObject(negativeZAxis);
	mGridNode->setVisible(true);

	

}

void SceneManipulator::setLocatorVisible(bool visible)
{
	m_baseNode->setVisible(visible);
}
Ogre::SceneNode* SceneManipulator::getLocatorNode()
{
	return m_baseNode;
}
void SceneManipulator::_updateAxexInfo(Ogre::Vector3 pos, Ogre::Quaternion qRot)
{	
	m_axex->setPosition(Ogre::Vector3::ZERO);
	m_axex->setOrientation(Ogre::Quaternion::IDENTITY);
	m_axex->rotate( qRot ,Ogre::Node::TS_LOCAL);
	m_axex->translate(pos,Ogre::Node::TS_LOCAL);	
}

}