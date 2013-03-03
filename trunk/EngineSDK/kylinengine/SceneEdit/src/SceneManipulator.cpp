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
#include <OgreVector3.h>

#include "Core/FairyXercesParser.h"
#include "Core/FairyXercesWriter.h"

#include "Core/FairyResources.h"
#include "Core/FrameStatsListener.h"
#include "Core/FairySceneInfo.h"
#include "Core/Terrain.h"

#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyEnviromentObject.h"
#include "Core/TerrainTypeInfos.h"

#include "Core/FairyStaticEntityObject.h"

#include "EffectSystem/FairyEffect.h"

#include "WXOperatorManager.h"
#include "WXCreateObjectOperator.h"
#include "WXRenameObjectOperator.h"
#include "WXModifyObjectPropertyOperator.h"

#include "Manip.h"
#include "HitIndicator.h"
#include "Selection.h"
#include "Action.h"

#include "CameraManips.h"
#include "IntersectPoint.h"
#include "JunctionPoints.h"
#include "IntersectGrids.h"
#include "StandardModelIndicator.h"

#include "TerrainSelections.h"
#include "BrushShapes.h"
#include "DragHeightAction.h"
#include "RaiseHeightAction.h"
#include "LowerHeightAction.h"
#include "SmoothHeightAction.h"
#include "TrueSmoothHeightAction.h"
#include "LevelOffHeightAction.h"
#include "SetTerrainHeightAction.h"
#include "SimplePaintAction.h"
#include "TerrainLayerOneEraserAction.h"
#include "GridInfoFlagModifyAction.h"
#include "TerrainSelectionAction.h"
#include "DragTerrainColourAction.h"

#include "CreateObjectAction.h"
#include "CreateComplexObjectAction.h"
#include "ManipObjectAction.h"
#include "AutoTexPaintAction.h"
#include "FixPaintAction.h"

#include "Regioneditaction.h"		// ������������༭
#include "Monstereditaction.h"		// ����༭action
#include "Growpointeditaction.h"	// ������༭
#include "Npcpatrolaction.h"		// npcѲ��·�߱༭
#include "PathFindAction.h"		// npcѲ��·�߱༭
#include "Eventareaeditaction.h"	// �¼�����༭
#include "Stallinfoaction.h"		// ̯λ�༭
#include "Soundeditaction.h"		// �����༭
#include "DatuPointEditAction.h"


// listener
#include "AddLightObjectPlugin.h"
#include "AddParticleSystemObjectPlugin.h"
#include "AddLiquidObjectPlugin.h"
#include "AddEffectObjectPlugin.h"
#include "AddStaticEntityObjectPlugin.h"

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/variate_generator.hpp>

#include "FractalTerrain.h"
#include "TerrainGrid.h"

#include "TerrainPaintXMLHandler.h"

#include "Core/FairySystem.h"
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

HitIndicator*
SceneManipulator::getHitIndicator(const String& name) const
{
    HitIndicators::const_iterator it = mHitIndicators.find(name);
    if (it == mHitIndicators.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Can't found manip of '" + name + "'.",
            "SceneManipulator::getHitIndicator");
    }

    return it->second;
}

void
SceneManipulator::registerHitIndicators(void)
{
    registerHitIndicator(new IntersectPoint(this));
    registerHitIndicator(new JunctionPoints(this));
    registerHitIndicator(new IntersectGrids(this));
    registerHitIndicator(new FlipDiagonalGrids(this));
    registerHitIndicator(new DisableDecalGrids(this));
    registerHitIndicator(new TerrainSelectionGrids(this));
    registerHitIndicator(new StandardModelIndicator(this));
    // TODO: register other usable hit indicators
}

void
SceneManipulator::registerHitIndicator(HitIndicator* hitIndicator)
{
    std::pair<HitIndicators::iterator, bool> inserted =
        mHitIndicators.insert(HitIndicators::value_type(hitIndicator->getName(), hitIndicator));
    if (!inserted.second)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
            "Hit indicator with name '" + hitIndicator->getName() + "' already registered",
            "SceneManipulator::registerHitIndicator");
    }
}

void
SceneManipulator::destoryHitIndicators(void)
{
    for (HitIndicators::const_iterator it = mHitIndicators.begin(); it != mHitIndicators.end(); ++it)
    {
        delete it->second;
    }
    mHitIndicators.clear();
}

//////////////////////////////////////////////////////////////////////////

BrushShape*
SceneManipulator::_getBrushShape(const String& type) const
{
    BrushShapes::const_iterator it = mBrushShapes.find(type);
    if (it == mBrushShapes.end())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Can't found manip of '" + type + "'.",
            "SceneManipulator::getBrushShape");
    }

    return it->second;
}

void
SceneManipulator::registerBrushShapes(void)
{
    registerBrushShape(new BoxBrushShape);
    registerBrushShape(new EllipseBrushShape);
    // TODO: register other usable brush shapes
}

void
SceneManipulator::registerBrushShape(BrushShape* hitIndicator)
{
    std::pair<BrushShapes::iterator, bool> inserted =
        mBrushShapes.insert(BrushShapes::value_type(hitIndicator->getType(), hitIndicator));
    if (!inserted.second)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
            "Brush shape with type '" + hitIndicator->getType() + "' already registered",
            "SceneManipulator::registerBrushShape");
    }
}

void
SceneManipulator::destoryBrushShapes(void)
{
    for (BrushShapes::const_iterator it = mBrushShapes.begin(); it != mBrushShapes.end(); ++it)
    {
        delete it->second;
    }
    mBrushShapes.clear();
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

void
SceneManipulator::registerSelections(void)
{
    registerSelection(new JunctionSelection(getTerrain(),this));
    registerSelection(new GridSelection(getTerrain(),this));
    // TODO: register other usable selections
}

void
SceneManipulator::registerSelection(Selection* selection)
{
    std::pair<Selections::iterator, bool> inserted =
        mSelections.insert(Selections::value_type(selection->getType(), selection));
    if (!inserted.second)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
            "Selection with type '" + selection->getType() + "' already registered",
            "SceneManipulator::registerSelection");
    }
}

void
SceneManipulator::destorySelections(void)
{
    for (Selections::const_iterator it = mSelections.begin(); it != mSelections.end(); ++it)
    {
        delete it->second;
    }
    mSelections.clear();
}

void 
SceneManipulator::registerAddObjectPlugins(void)
{
    // ��Ӹ�������ָʾ��
    mAddObjectPlugins.push_back( new AddLightObjectPlugin(this) );
    mAddObjectPlugins.push_back( new AddParticleSystemObjectPlugin(this) );
	mAddObjectPlugins.push_back( new AddLiquidObjectPlugin(this) );
	mAddObjectPlugins.push_back( new AddEffectObjectPlugin(this) );
	mAddObjectPlugins.push_back( new AddStaticEntityObjectPlugin(this));
}

void 
SceneManipulator::destroyAddObjectPlugins(void)
{
    for ( size_t i=0; i<mAddObjectPlugins.size(); ++i )
    {
        delete mAddObjectPlugins[i];
    }
}


//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::setActiveAction(const String& name)
{
	// ����ǰΪManipObjectAction״̬�������ѡ��
	if((NULL != mActiveAction) && (mActiveAction->getName() == "ManipObjectAction"))
	{
		clearSelectedObjects();
	}

    setActiveAction(name.empty() ? NULL : _getAction(name));
}

void SceneManipulator::setActiveAction(Action* action)
{
    if (mActiveAction != action)
    {
        if (mActiveAction)
            mActiveAction->setActive(false);
        mActiveAction = action;
        if (mActiveAction && mHitPointShown)
            mActiveAction->setActive(true);
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
    registerAction(new DragHeightAction(this));
    registerAction(new RaiseHeightAction(this));
    registerAction(new LowerHeightAction(this));
    registerAction(new SmoothHeightAction(this));
    registerAction(new TrueSmoothHeightAction(this));
    registerAction(new LevelOffHeightAction(this));
    registerAction(new SetTerrainHeightAction(this));
    registerAction(new SimplePaintAction(this));
    registerAction(new TerrainLayerOneEraserAction(this));
    registerAction(new AutoTexPaintAction(this));
	registerAction(new FixPaintAction(this));
    registerAction(new CreateObjectAction(this));
	registerAction(new CreateComplexObjectAction(this));
    registerAction(new ManipObjectAction(this));
    registerAction(new FlipDiagonalAction(this));
    registerAction(new DisableDecalAction(this));
    registerAction(new TerrainSelectionAction(this));
	registerAction(new CRegionEditAction(this));		// ע�᲻����������༭action
	registerAction(new CMonsterEditaction(this));		// ע�����༭action
	registerAction(new CGrowPointEditAction(this));		// ע��������༭action
	registerAction(new CNpcPatrolAction(this));			// ע��npcѲ��·�߱༭action
	registerAction(new CPathFindAction(this));			// ע��Ѱ·�༭action
	registerAction(new CEventAreaEditAction(this));		// ע���¼�����༭action
	registerAction(new CStallInfoAction(this));			// ע��̯λ�༭action
	registerAction(new SoundEditAction(this));			// ע�������༭action
    registerAction(new DatuPointEditAction(this));			// ��ͼ��action
	registerAction(new DragTerrainColourAction(this));

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
SceneManipulator::newScene(Ogre::String& algorithm, size_t width, size_t depth, size_t tileSize,
                           const Ogre::String& baseTexture,
                           size_t seed,size_t scale, Ogre::Real height, Ogre::Real h)
{
    _preprocessScene();

    _createTerrain(algorithm,width,depth,tileSize,baseTexture,seed,scale,height);

    _postprocessScene();

}

//������ַ���Ӧ�ú�NewSceneDialog�е��ַ�������strTbls����һ��
void SceneManipulator::initFuncMap()
{
    funcMap.insert(FuncMap::value_type("Diamond Square",generateFractalTerrain));
    funcMap.insert(FuncMap::value_type("Fluid Simulation",fluidGenerate));
    funcMap.insert(FuncMap::value_type("Box Filtering",BoxFilterHeightMap));
    funcMap.insert(FuncMap::value_type("Hill Generate",generateHillTerrain));
    funcMap.insert(FuncMap::value_type("Frac SynthPass",fracSynthPass));
} 

void SceneManipulator::fillFuncParam(FuncParam* param,int seed,Ogre::Real scale)
{
    assert(mTerrainData);

    memset(param,0,sizeof(param));
    param->heightMap = &mTerrainData->mHeightmap[0];
    param->xsize = mTerrainData->mXSize;
    param->zsize = mTerrainData->mZSize;
    param->seed = seed;
    param->scale = scale;

}

void
SceneManipulator::_createTerrain(Ogre::String& algorithm,size_t width, size_t depth, size_t tileSize,
                                 const String& baseTexture,
                                 size_t seed, Real scale, Real height, Real h)
{
    //���������
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

    //���ҵ���Ӧ�ĸ߶�ͼ���ɺ���
    algorithmFunc funcGenerateHeightMap ;
    FuncMap::const_iterator iter = funcMap.find(algorithm) ;
    if(iter != funcMap.end())
    {
        funcGenerateHeightMap = iter->second;
    }
    else
    {
        iter = funcMap.begin();
        funcGenerateHeightMap = iter->second;
    }

    FuncParam param;
    fillFuncParam(&param,seed,height);
    funcGenerateHeightMap(&param);
 
    if (engine.get() && data->mPixmaps.size() > 1)
    {
        boost::variate_generator<boost::minstd_rand&, boost::uniform_smallint<> >
            generator(*engine, boost::uniform_smallint<>(1, data->mPixmaps.size()));
        for (int z = 0; z < data->mZSize; ++z)
        {
            for (int x = 0; x < data->mXSize; ++x)
            {
                data->mGridInfos[z * data->mXSize + x].layers[0].pixmapId = generator();
            }
        }
    }
    else
    {
        TerrainData::GridInfo gridInfo = { 0, 0, 0, 0, 0 };
        if (!data->mPixmaps.empty())
            gridInfo.layers[0].pixmapId = 1;
        std::fill(data->mGridInfos.begin(), data->mGridInfos.end(), gridInfo);
    }
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

    // Give hit indicators a chance to adjust shapes
    for (HitIndicators::const_iterator it = mHitIndicators.begin(); it != mHitIndicators.end(); ++it)
    {
        it->second->reshape();
    }

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

void
SceneManipulator::resetScene(void)
{
    mSelectedObjects.clear();

    // Reset selections
    for (Selections::const_iterator it = mSelections.begin(); it != mSelections.end(); ++it)
    {
        it->second->reset();
    }

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

    bool existsSky = false;
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
	}
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

    // ����terrain type info
    mTerrainTypeInfos->setTerrainData(mTerrainData);
    mTerrainTypeInfos->updateTerrainTypeInfos();
}

void
SceneManipulator::saveScene(const String &filename)
{
    if (getTerrainData()->_optimize())
    {
        getTerrain()->buildGeometry(getBaseSceneNode(), isEditable());
    }
    mSceneInfo->save(mXMLWriter, filename);

    mSnapshot = getOperatorManager()->makeSnapshot();
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

    , mHitPointShown(true)
    , mObjectsUseBoxCenter(false)
    , mAxisGizmoActive(false)
	, mTerrainGrid(NULL)
    , mIndicatorNodeVisible(true)
	, mClearObjectAdditionalDiffuse(false)
	, mCameraPerspective(NULL)
	, mCameraOrthographic(NULL)
{
    mXMLParser = new XercesParser;
    mXMLWriter = new XercesWriter;

    mMoveFactor = 10;
    mRotateFactor = 360;
    mMoveSpeed = 30;
    mRotateSpeed = 36;
    mWheelSpeed = 1;
    mCameraHeightLocked = false;
    mShowStandardModel = false;
    mHeightAdjustSpeed = 3;
    mPositionAlignGranularity = 0;
    mRotationAlignGranularity = 0;
    mBrushType = Ogre::StringUtil::BLANK;
    mBrushXSize = 3;
    mBrushZSize = 3;

    // Ĭ�ϵ���ʵ������߶ȣ��Ƕ�
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

	mTerrainPaintInfoContainer = new TerrainPaintInfoContainer;

	fillUnableCenterSelectObject();
	mSelectAllObjects = true;

    mTerrainTypeInfos = new TerrainTypeInfos;
	mDiffuseColour = Ogre::ColourValue::White;
	mSelectableType.clear();
}

SceneManipulator::~SceneManipulator()
{
    delete mTerrainTypeInfos;

    destoryActions();
    destorySelections();
    destoryBrushShapes();
    destoryManips();
    destoryHitIndicators();

    destroyAddObjectPlugins();

    delete mOperatorManager;

    delete mXMLWriter;
    delete mXMLParser;

	if(mTerrainGrid)
		delete mTerrainGrid;

	delete mTerrainPaintInfoContainer;

    // ����preview image
    clearPreviewImage();

    if (mIndicatorRootSceneNode)
    {
        getSceneManager()->destroySceneNode(mIndicatorRootSceneNode);
		mIndicatorRootSceneNode = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////
void
SceneManipulator::_createCamera(void)
{
	Fairy::System::_createCamera();
}

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

    mOperatorManager = new OperatorManager;

    registerHitIndicators();
    registerManips();
    registerBrushShapes();
    registerSelections();
    registerActions();

    registerAddObjectPlugins();

    // Set default brush
    setBrush("BoxBrushShape", 3, 3);

    initFuncMap();

    // Load scene template
/*    loadSceneFromResource(mXMLParser, "WXSceneTemplate.xml", GRAMMAR_RESOURCE_GROUP_NAME);*/
	// load and parse the texture info 
    // ���ﲻ�����������ļ��Ľ�������Ϊ��reloadPaintInfoʱ�����һ�Σ������ظ�����
   // loadTerrainPaintInfo(mXMLParser, "FairyTexInfo.xml", BRUSH_RESOURCE_GROUP_NAME);

    // �����������fovΪ35
    mCamera->setFOVy(Ogre::Degree(35.0f));

  //  Ogre::ParticleSystem::setDefaultIterationInterval(0.033f);

    // ��ȡ������Ϣ�����ļ�
    try
    {
        mTerrainTypeInfos->parseTerrainTypeConfigFile("WXTerrainType.cfg");
    }
    catch(Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());
    }

}

//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::moveCameraToUsableLocation(void)
{
	// Switch to the perspective camera first
	if (mCamera->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
	{
		changeCameraType();
	}

    // Move camera, make sure we can see the scene
    TerrainData* data = getTerrainData();
    Ogre::Vector3 centre = data->mCentre;
    mCamera->setPosition( centre.x, mRealCameraHeight + data->getHeightAt(centre.x, centre.z), centre.z );
  //  mCamera->lookAt(data->getPositionAt(centre.x, centre.z - 20 * mBaseScale));
    mCamera->setOrientation(Ogre::Quaternion(Ogre::Degree(-mRealCameraAngle), Ogre::Vector3::UNIT_X));

    // �����������fovΪ35
    mCamera->setFOVy(Ogre::Degree(35.0f));
}

//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::showHitPoint(bool show)
{
    mHitPointShown = show;

    if (!show)
    {
        // Hide all hit indicators
        for (HitIndicators::const_iterator it = mHitIndicators.begin(); it != mHitIndicators.end(); ++it)
        {
            it->second->hide();
        }
    }

    // Active/deactive current action
    if (mActiveAction)
    {
        mActiveAction->setActive(show);
    }
}

//////////////////////////////////////////////////////////////////////////

void
SceneManipulator::setBrush(const String& type, size_t xsize, size_t zsize)
{
    BrushShape* brushShape = _getBrushShape(type);
    if (!brushShape || xsize <= 0 || zsize <= 0)
    {
        Ogre::StringUtil::StrStreamType str;
        str << "Invalidate brush settings."
            << " Type: " << type
            << " XSize: " << xsize
            << " ZSize: " << zsize;
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, str.str(), "SceneManipulator::setBrush");
    }

    mBrushType = type;
    mBrushXSize = xsize;
    mBrushZSize = zsize;
}

void
SceneManipulator::_buildSelection(Selection* selection, Real xcentre, Real zcentre)
{
    assert(selection);

    if (mBrushType.empty())
    {
        selection->reset();
    }
    else
    {
        BrushShape* brushShape = _getBrushShape(mBrushType);
        assert(brushShape);
        brushShape->build(*selection, xcentre, zcentre, mBrushXSize, mBrushZSize, this);
    }
}

void
SceneManipulator::_buildSelection(const String& selectionType, Real xcentre, Real zcentre)
{
    _buildSelection(_getSelection(selectionType), xcentre, zcentre);
}

//////////////////////////////////////////////////////////////////////////

bool
SceneManipulator::makeTerrainHitInfo(HitInfo& hitInfo, Real winx, Real winy)
{
    hitInfo.position = getCamera()->getPosition();
    hitInfo.orientation = getCamera()->getOrientation();
    hitInfo.ray = getWindowToViewportRay(winx, winy);
    hitInfo.hitted = getTerrainIntersects(hitInfo.ray, hitInfo.hitPosition, &hitInfo.hitNormal);
    return hitInfo.hitted;
}

bool
SceneManipulator::makeTerrainHitInfo(HitInfo& hitInfo, const Point& pt)
{
    return makeTerrainHitInfo(hitInfo, pt.x, pt.y);
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

	if (!isObjectSelectable(object))
	{
		return false;
	}

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

bool SceneManipulator::isObjectSelectable(const ObjectPtr& object)
{
	if (object.get() == NULL)
	{
		return true;
	}

	if (!object->hasProperty("position"))
	{
		return true;
	}

	if (object->getName() == Fairy::FairyAutoMainLightOneObjectName 
		||object->getName() == Fairy::FairyAutoMainLightTwoObjectName)
	{
		return true;
	}
	
	return mSelectableType.find(object->getType()) != mSelectableType.end();
}

Ogre::Vector3
SceneManipulator::_alignPosition(const Ogre::Vector3& value)
{
    return _alignPosition(value, mPositionAlignGranularity);
}

Ogre::Vector3
SceneManipulator::_alignPosition(const Ogre::Vector3& value, Real granularity)
{
    Ogre::Vector3 r(value);

    if (granularity > 0)
    {
        r.x = Ogre::Math::Floor(r.x * getTerrainData()->mInvScale.x / granularity + 0.5) *
            getTerrainData()->mScale.x * granularity;
        r.z = Ogre::Math::Floor(r.z * getTerrainData()->mInvScale.z / granularity + 0.5) *
            getTerrainData()->mScale.z * granularity;
    }

    return r;
}

void
SceneManipulator::_snapSelectedObjectsPosition(Real granularity)
{
    if (granularity > 0)
    {
        std::auto_ptr<ModifyObjectPropertyOperator> op(new ModifyObjectPropertyOperator(this));

        for (ObjectSet::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
        {
            const ObjectPtr& object = *it;
            if (object->hasProperty("position"))
            {
                Ogre::Vector3 oldPosition = VariantCast<Ogre::Vector3>(object->getProperty("position"));
                Ogre::Vector3 newPosition = _alignPosition(oldPosition, granularity);
                if (newPosition != oldPosition)
                {
                    Ogre::Vector3 groundingPoint = Ogre::Vector3::ZERO;
                    if (object->hasProperty("grounding point"))
                        groundingPoint = VariantCast<Ogre::Vector3>(object->getProperty("grounding point"));
                    // Adjust height relative to terrain
                    newPosition.y +=
                        getTerrainData()->getHeightAt(newPosition.x + groundingPoint.x, newPosition.z + groundingPoint.z) -
                        getTerrainData()->getHeightAt(oldPosition.x + groundingPoint.x, oldPosition.z + groundingPoint.z);
                    object->setProperty("position", newPosition);
                    _fireObjectPropertyChanged(object, "position");
                    op->add(object->getName(), "position", oldPosition, newPosition);
                }
            }
        }

        if (!op->empty())
            getOperatorManager()->addOperator(op.release());
    }
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

void
SceneManipulator::setUseBoxCenterSelection(int x,int y,bool use)
{
    if(getActiveAction() && getActiveAction()->getName() == "ManipObjectAction")
    {
        mObjectsUseBoxCenter = use;
        ManipObjectAction* action = (ManipObjectAction*) getActiveAction();
        if(!mObjectsUseBoxCenter)
			action->clearBoxCenterManager();
        else
        {
            ManipObjectAction* action = (ManipObjectAction*) getActiveAction();
			action->clearBoxCenterManager();
            action->addBoxCenterMovable(Point(x,y));
        }
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
    if(getActiveAction() && getActiveAction()->getName() == "ManipObjectAction")
    { 
        mAxisGizmoActive = use;
        ManipObjectAction* action = (ManipObjectAction*) getActiveAction();
        if(!mAxisGizmoActive)
            action->hideAxis();
        else
            action->createAxis(getSceneManager(),getCamera(),getViewport());
    }    
}

bool 
SceneManipulator::getUseAxis()
{
    return mAxisGizmoActive;
}

TerrainGrid* 
SceneManipulator::getTerrainGrid()
{
	if(!mTerrainGrid)
	{
		mTerrainGrid = new TerrainGrid(getSceneManager(),this);
	}

	return mTerrainGrid;
}


void 
SceneManipulator::setRealCameraAngle( const Real angle ) 
{ 
    mRealCameraAngle = angle;   

    /** dirΪĿ�귽��
        ��֪���� : dir.y == -sin(alpha) �� ԭ�з��� curDir
                    /
                   /
                  /   
        alpha   \/angle
        -------------------------------
        ����Ҫ���dir��x��z��
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
    // ȡ����ǰ�����������εĸ߶�
    TerrainData* data = getTerrainData();
    mCamera->setPosition( currentX, mRealCameraHeight + data->getHeightAt(currentX,currentY), currentY ); 
}

void 
SceneManipulator::AddTransparentTexture( const String &textureName, bool transparent )
{
    assert( !textureName.empty() );

   // TransparentTextures::const_iterator i = mTransparentTextures.find(textureName);

    // ����ǰ�ȼ���Ƿ�������Ѿ����ڣ�������ǣ�˵�����¼��������
   // if ( i==mTransparentTextures.end() )
  //  {
        std::pair<TransparentTextures::iterator, bool> inserted =
            mTransparentTextures.insert( TransparentTextures::value_type(textureName,transparent) );

        assert( inserted.second );
 //   }    
}

bool 
SceneManipulator::IsTransparentTexture( const String &textureName )
{
	assert( !textureName.empty() );
	TransparentTextures::const_iterator i = mTransparentTextures.find(textureName);


	// ����Ҳ�����˵���������û���жϹ����Ͷ�ȡ�����ж�
	if ( i==mTransparentTextures.end() )
	{
		const TextureInfo &info = mTerrainPaintInfoContainer->findTextureInfoByTexName(textureName);

		Ogre::Image image;
		image.load(textureName, BRUSH_RESOURCE_GROUP_NAME);
		bool transparent = image.getHasAlpha() && info.textureType != InnerTex;

		AddTransparentTexture(textureName, transparent);

		return transparent;
	}
	else
		return i->second;    
}

void
SceneManipulator::parseStringAndCreateObject( const Ogre::String &infoString, Ogre::Vector3 &position )
{
    // ����clipboard�е�����������һ����ʱ�������������еĽ��������������������������
    Ogre::MemoryDataStream* stream = new Ogre::MemoryDataStream((void *)(infoString.c_str()),
        infoString.length(), false);

    // ������ʱ����Ҫ��ӵ�����
    Fairy::SceneManipulator::ObjectSet addedObjects;

    Ogre::String line;
    // ��ʼ�����������Ĳ���
    while(!stream->eof())
    {
        line = stream->getLine();

        // �����ַ����Ŀ�ͷ���ж�Ҫ��������ʲô����
        // object
        if ( line.substr(0,6) == "object" )
        {                    
            Ogre::String objectType = line.substr(7,line.size());
            Fairy::ObjectPtr tempObjectPtr;
            tempObjectPtr = Fairy::ObjectFactoryManager::getSingleton().createInstance(objectType);

            // �ų� { ��һ��
            stream->skipLine();

            Ogre::String valueLine;
            Ogre::StringVector vecparams;

            valueLine = stream->getLine();

            while ( valueLine != "}" )
            {
                // �������ã����ָ���
                vecparams = Ogre::StringUtil::split(valueLine, "=");

                Ogre::String paraName, paraValue;                         

                paraName = vecparams[0];
                paraValue = vecparams[1];

                // ������һ��
                valueLine = stream->getLine();

                // �����λ�����ԱȽ����⣬��Ϊpasteʱ����Ӧ������λ�ø���������Ҫ���⴦��
                if ( paraName == "position" )
                {
                    // ����ȡ����λ��ֻ�Ǹ�������������ѡ�еĵ�һ�������λ��
                    Ogre::Vector3 relativePos = Ogre::StringConverter::parseVector3(paraValue);

                    // ������������Ľ��Ӵ��õ�ʵ�ʵ�λ��
                    Ogre::Vector3 worldPos = relativePos + position;

                    // ����������нӵص㣬��Ҫ���Ͻӵص��λ��
                    if (tempObjectPtr->hasProperty("grounding point"))
                    {
                        // Adjust position by grounding point
                        Ogre::Vector3 groundingPoint = Fairy::VariantCast<Ogre::Vector3>(tempObjectPtr->getProperty("grounding point"));
                        worldPos -= groundingPoint;
                    }
                    paraValue = Ogre::StringConverter::toString( worldPos );
                }

                // ��������ĸ�������
                tempObjectPtr->setPropertyAsString(paraName, paraValue);
            }

            // ��������
            tempObjectPtr->createRenderInstance(this);
            // ������������ƣ��Լ����������������ظ���
            tempObjectPtr->setName(getSceneInfo()->generateAutoName(tempObjectPtr));

            std::pair<Fairy::SceneManipulator::ObjectSet::iterator, bool> inserted =
                addedObjects.insert(tempObjectPtr);
            assert(inserted.second);
        }   
    }

    if (addedObjects.size() >0)
        // ������������壨Ϊ����������undo��
        addOjbects(addedObjects);

    // ɾ����ʱ������
    delete stream;
}

void 
SceneManipulator::getSelectedObjectInfo( Ogre::String &infoString )
{
    const Fairy::SceneManipulator::ObjectSet &objects = this->getSelectedObjects();

    Fairy::SceneManipulator::ObjectSet::const_iterator i = objects.begin();

    /** ����ѡ�е�����������ַ���
    �ַ�����ʽ���£�
    /////////////////////

    object ��������
    {
    ������=����ֵ
    }
    object ��������
    {
    ������=����ֵ
    ������=����ֵ
    ...
    }
    ////////////////////
    ��ճ��ʱҪ���������ʽ�������ַ������õ��������Ϣ
    */
    while ( i!=objects.end() )
    {
        const ObjectPtr& object = *i;
        Ogre::String objectType = object->getType();
        infoString += "object ";
        infoString += objectType.c_str();
        infoString += "\n{\n";

        Ogre::Vector3 firstObjectPos;
        // ���������
        const Fairy::PropertyList& properties = object->getProperties();
        for (Fairy::PropertyList::const_iterator itProperty = properties.begin(); itProperty != properties.end(); ++itProperty)
        {
            const Ogre::String& name = itProperty->name;
            if (!(object->getPropertyFlags(name) & PF_DERIVED))
            {
                Ogre::String value = object->getPropertyAsString(name);

                // ����ʱ�õ������λ�����������꣬Ӧת������ڸ���ѡ��������������
                if ( name == "position" )
                {
                    if ( i == objects.begin() )
                    {
                        // ��һ��������������Ϊ0
                        firstObjectPos = Ogre::StringConverter::parseVector3(value);
                        value = "0 0 0";
                    }
                    else
                    {
                        Ogre::Vector3 currentObjectPos = Ogre::StringConverter::parseVector3(value);
                        value = Ogre::StringConverter::toString( Ogre::Vector3(currentObjectPos-firstObjectPos) );
                    }
                }

                // ��Ϊ��һ�������ֵΪ0��ΪĬ��ֵ������Ҫ���Ϻ��������������һ�������λ�òŻᱻ����
                if (value != itProperty->defaultValue || name == "position")
                {
                    infoString += name.c_str();
                    infoString += "=";
                    infoString += value.c_str();
                    infoString += "\n"; 
                }
            }
        }

        // һ��������������ˣ�����
        infoString += "}\n";

        ++i;
    }    
}

//size_t 
//SceneManipulator::findGridPaintInfo( String texGroupName )
//{
//	// ����������
//	mResultGridPaintInfos.clear();
//	
//	// �������ε����и���
//	TerrainData *terrainData = getTerrainData();
//	assert (terrainData);
//
//	int xsize = terrainData->getXSize();
//	int zsize = terrainData->getZSize();
//
//	for ( int x=0; x<xsize; ++x )
//		for( int z=0; z<zsize; ++z )
//		{
//			const TerrainData::GridInfo &gridInfo = terrainData->getGridInfo(x,z);
//
//			String layerGroupName;	// ���浱ǰ�����ĸ������õ����������������
//			String layerTexName;    // ���浱ǰ�����ĸ������õ��������������
//			int id = 0;				// ���浱ǰ����ĳһ�������id
//
//			// ��鵱ǰ��ĵ�0������
//			id = gridInfo.layers[0].pixmapId;
//
//			// ���idС��1��˵��������ӵ���һ�㻹û��Ϳ������ֱ������
//			if (id<1)
//				continue;
//
//			TerrainData::Pixmap pixmap = terrainData->mPixmaps[id-1];
//			layerTexName = terrainData->mTextures[ pixmap.textureId ];
//
//			// ����������������֪�������������
//			layerGroupName = getTexGroupNameFromTextureName(layerTexName);
//
//			// �������������������������Ҫ�滻������������һ���ģ�˵����һ�����һ���������Ҫ���滻
//			if ( layerGroupName == texGroupName )
//			{
//				// ����������������֪���������
//				TexType texType = getTexTypeFromTextureName(layerTexName);
//				assert (texType>=InnerTex && texType<Other);
//
//				int orientation = gridInfo.layers[0].orientation;
//
//				mResultGridPaintInfos.push_back(GridPaintInfo( orientation, texType, 0, x, z,
//					pixmap.left, pixmap.top, pixmap.right, pixmap.bottom ));
//			}
//
//			// ��鵱ǰ��ĵ�1������
//			id = gridInfo.layers[1].pixmapId;
//			if (id<1)
//				continue;
//
//			pixmap = terrainData->mPixmaps[id-1];
//
//			layerTexName = terrainData->mTextures[ pixmap.textureId ];
//			layerGroupName = getTexGroupNameFromTextureName(layerTexName);
//
//			if ( layerGroupName == texGroupName )
//			{
//				TexType texType = getTexTypeFromTextureName(layerTexName);
//				assert (texType>=InnerTex && texType<Other);
//
//				int orientation = gridInfo.layers[1].orientation;
//				mResultGridPaintInfos.push_back(GridPaintInfo( orientation, texType, 1, x, z,
//					pixmap.left, pixmap.top, pixmap.right, pixmap.bottom ));
//			}
//		}
//
//	return mResultGridPaintInfos.size();
//}

String 
SceneManipulator::getTexGroupNameFromTextureName( String &textureName )
{
	// �������������û����/��������ֱ�ӷ���brush��Ŀ¼�µģ�һ�㲻��������������������������ֻ��һ����ͼ
	// Ҳ������brush�ļ����½�һ����Ŀ¼
	size_t pos = textureName.find_last_of('/');

	assert (pos != String::npos);
	
	return textureName.substr(0,pos);	
}

TexType 
SceneManipulator::getTexTypeFromTextureName( String &textureName )
{
	if ( textureName.find("��Ե") != String::npos )
	{
		return EdgeTex;
	}
	else if ( textureName.find("�ڽ�") != String::npos )
	{
		return InnerCornerTex;
	}
	else if ( textureName.find("���") != String::npos )
	{
		return OuterCornerTex;
	}
	else if ( textureName.find("�Խ�") != String::npos )
	{
		return DuijiaoTex;
	}
	else
	{
		return InnerTex;
	}
}

/////////////////////////////////////////////////////////
void 
SceneManipulator::setPaintPixmapInfo( PaintPixmap &pixmap, const TextureInfo &texInfo )
{
    const Ogre::Image *image = getPreviewImage(texInfo.ownerTextureName);

    pixmap.textureName = texInfo.ownerTextureName;

    // �������������
    uint ownerTexHeight = image->getHeight();
    uint ownerTexWidht = image->getWidth();

    pixmap.left = (float)(TexTileSize*texInfo.leftCorner) / (float)ownerTexWidht;
    pixmap.top = (float)(TexTileSize*texInfo.topCorner) / (float)ownerTexHeight;
    pixmap.bottom = (float)(TexTileSize*(texInfo.topCorner + texInfo.height)) / (float)ownerTexHeight;
    pixmap.right = (float)(TexTileSize*(texInfo.leftCorner + texInfo.width)) / (float)ownerTexWidht;
}

void 
SceneManipulator::reloadPaintInfo(void)
{

  loadTerrainPaintInfo(mXMLParser, "FairyTexInfo.xml", BRUSH_RESOURCE_GROUP_NAME);

}

void 
SceneManipulator::loadTerrainPaintInfo(XMLParser* parser, const String& name, const String& groupName)
{
    TerrainPaintXMLHandler handler(mTerrainPaintInfoContainer);

    handler.clearTextureInfos();

    // parse the texture info XML file
    parser->parseXMLFile(handler, name, groupName,
        "FairyTexInfo.xsd", GRAMMAR_RESOURCE_GROUP_NAME);

    const String& errorInfo = handler.getErrorInfoString();

    if ( false == errorInfo.empty() )
    {
        Ogre::LogManager::getSingleton().logMessage(
            "Load terrain paint info file wrong\n" + errorInfo + 
            "SceneManipulator::loadTerrainPaintInfo");
    }
}

const TextureInfos&
SceneManipulator::getTextureInfos( const Ogre::String &brushName )
{
    // ����������ȡ�����������Ϣ
    const TextureInfoMap &textureInfoMap = mTerrainPaintInfoContainer->getTextureInfoMap();

    TextureInfoMap::const_iterator it = textureInfoMap.find(brushName);

    assert ( it != textureInfoMap.end() );

    return it->second;
}

TextureInfo 
SceneManipulator::getTextureInfo(const Ogre::String &brushName, const Ogre::String &textureName)
{
    // ��������������������ȡ������Ϣ
    const TextureInfos resultInfos = getTextureInfos(brushName);

    for ( size_t i=0; i<resultInfos.size(); ++i )
    {
        if (resultInfos[i].textureName == textureName)
            return resultInfos[i];
    }

    // ���ߵ�����϶�����
    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
        "Can't find the texture info for brush '" + brushName + "' with texture name '" + textureName + "'",
        "SceneManipulator::getTextureInfo");
}

Ogre::Image * 
SceneManipulator::getPreviewImage( const Ogre::String &textureName )
{
    PreviewImageMap::iterator it = mPreviewImageMap.find(textureName);

    if ( it == mPreviewImageMap.end() )
    {
        Ogre::Image *image = new Ogre::Image;
        image->load(textureName,"Brushes");

        assert (image);

        std::pair< PreviewImageMap::iterator, bool > inserted = 
            mPreviewImageMap.insert(PreviewImageMap::value_type(textureName,image));

        assert (inserted.second);

        return image;
    }
    else
    {
        assert (it->second);
        return it->second;
    }
}

void SceneManipulator::clearPreviewImage(void)
{
    PreviewImageMap::iterator it = mPreviewImageMap.begin();

    while ( it != mPreviewImageMap.end() )
    {
        delete it->second;
        ++it;
    }

    mPreviewImageMap.clear();
}

bool SceneManipulator::IsCenterSelectObject( Ogre::String objectType )
{
	UnableCentenSelectObject::const_iterator i = mUnableCentenSelectObject.begin();

	// �������������ĵ�ѡ�������������б�����б���û�У�˵��������������ڿ������ĵ�ѡ���
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

bool SceneManipulator::eraseTerrainPixmapByTextureName(const String &texName)
{
    if (texName.empty())
        return false;

    // �������ε����и���
    TerrainData *terrainData = getTerrainData();
    assert (terrainData);

    int xsize = terrainData->getXSize();
    int zsize = terrainData->getZSize();

    int minx, maxx, minz, maxz;

    minx = maxx = minz = maxz = -1;

    bool bFirstGrid = false;

    for ( int x=0; x<xsize; ++x )
    {
        for( int z=0; z<zsize; ++z )
        {
            const TerrainData::GridInfo &gridInfo = terrainData->getGridInfo(x,z);

            String layerTexName;    // ���浱ǰ�����ĸ������õ��������������
            int id = 0;				// ���浱ǰ����ĳһ�������id

            // ��鵱ǰ��ĵ�0������
            id = gridInfo.layers[0].pixmapId;

            // ���idС��1��˵��������ӵ���һ�㻹û��Ϳ������ֱ������
            if (id<1)
                continue;

            TerrainData::Pixmap pixmap = terrainData->mPixmaps[id-1];
            layerTexName = terrainData->mTextures[ pixmap.textureId ];


            // �������������������������Ҫ�滻������������һ���ģ�˵����һ�����һ���������Ҫ���滻
            if ( layerTexName == texName )
            {
                TerrainData::GridInfo info = gridInfo;
                info.layers[0].pixmapId = 0;
                info.layers[0].orientation = 0;

                info.layers[1].pixmapId = 0;
                info.layers[1].orientation = 0;

                terrainData->setGridInfo(x, z, info);

                if ( false == bFirstGrid )
                {
                    minx = maxx = x;
                    minz = maxz = z;
                    bFirstGrid = true;
                }
                else
                {
                    if (minx > x)
                        minx = x;
                    else if (maxx < x)
                        maxx = x;
                    if (minz > z)
                        minz = z;
                    else if (maxz < z)
                        maxz = z;
                }

                continue;
            }

            // ��鵱ǰ��ĵ�1������
            id = gridInfo.layers[1].pixmapId;
            if (id<1)
                continue;

            pixmap = terrainData->mPixmaps[id-1];

            layerTexName = terrainData->mTextures[ pixmap.textureId ];

            if ( layerTexName == texName )
            {
                TerrainData::GridInfo info = gridInfo;

                info.layers[1].pixmapId = 0;
                info.layers[1].orientation = 0;

                terrainData->setGridInfo(x, z, info);

                if ( false == bFirstGrid )
                {
                    minx = maxx = x;
                    minz = maxz = z;
                    bFirstGrid = true;
                }
                else
                {
                    if (minx > x)
                        minx = x;
                    else if (maxx < x)
                        maxx = x;
                    if (minz > z)
                        minz = z;
                    else if (maxz < z)
                        maxz = z;
                }
            }
        }		
    }

    // ���һ�����Ӷ�û�У�˵�����������û���������
    if ( false == bFirstGrid )
        return false;

    if (minx < 0) minx = 0;
    if (minz < 0) minz = 0;
    if (maxx > getTerrainData()->mXSize-1)
        maxx = getTerrainData()->mXSize-1;
    if (maxz > getTerrainData()->mZSize-1)
        maxz = getTerrainData()->mZSize-1;
    if (minx <= maxx && minz <= maxz)
    {
        getTerrain()->notifyGridInfoModified(minx, minz, maxx+1, maxz+1);
    }

    return true;
}

void SceneManipulator::setIndicatorNodeVisible(bool visible, bool cascade)
{
    if (mIndicatorRootSceneNode)
    {
        mIndicatorRootSceneNode->setVisible(visible, cascade);
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

void SceneManipulator::updateAllObjectDiffuseByGrid()
{
	const SceneInfo::Objects& objects = mSceneInfo->getObjects();
	for (SceneInfo::Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Ogre::String type = (*it)->getType();
		if(Fairy::StaticEntityObject::msType == type)
		{
			Fairy::StaticEntityObject* entity = static_cast<Fairy::StaticEntityObject*>((*it).get());

			Ogre::Vector3 pos = Fairy::VariantCast<Ogre::Vector3>(entity->getProperty("position"));
			std::pair<int, int> baseGrid = mTerrainData->getGrid(pos.x, pos.z);
			Ogre::ColourValue diffuse = mTerrainData->getDiffuseInfo(baseGrid.first, baseGrid.second);
			Point point(baseGrid.first, baseGrid.second);
			if(mDiffuseApplyInfoMap.find(point) != mDiffuseApplyInfoMap.end())
			{
				entity->setAdditionalDiffuse(diffuse);
			}
		}
	}
}

void SceneManipulator::_loadDiffuseInfo(const Ogre::String& filename)
{
	mTerrainData->mDiffuseInfoMap.clear();

	if (filename.empty())
	{
		return;
	}

	_readDiffuseInfo(filename);
}

bool SceneManipulator::_readDiffuseInfo( const Ogre::String& fileName)
{
	FILE *fp = NULL;
	int icount ;
	char* buf;
	char* pPointCur;
	mTerrainData->mDiffuseInfoMap.clear();
	fopen_s(&fp, fileName.c_str(), "rb");
	if (fp == NULL)
		return false;

	fread(&icount, sizeof(int), 1, fp );
	if(icount==0)
	{
		fclose(fp);
		fp = NULL;
		return false;
	}

	fseek( fp, 0, SEEK_END );
	int length = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	buf = new char[length+1];
	memset(buf, 0, length);
	buf[length]=0;
	pPointCur = buf;

	if ( fread( buf, length, 1, fp ) != 1 ) {
		//LogManager::getSingleton().logMessage("file import error.");
		fclose(fp);
		fp = NULL;

		delete [] buf;
		buf =0;
	
		return false;
	}

	fclose(fp);
	fp = NULL;

	int index = 4;
	pPointCur +=4;
	for(int i=0;i<icount;i++)
	{
		Point UV;
		Ogre::ColourValue color;
		static unsigned short* sTmp = NULL;

		sTmp = static_cast<unsigned short*>(static_cast<void*>(pPointCur));
		UV.x = *sTmp;
		pPointCur +=2;
		sTmp = static_cast<unsigned short*>(static_cast<void*>(pPointCur));
		UV.y = *sTmp;
		pPointCur +=2;

		static unsigned char cTmp;
		cTmp =  (unsigned char) *pPointCur++;
		color.r = (float) cTmp/256.0f;
		cTmp =  (unsigned char) *pPointCur++;
		color.g = (float) cTmp/256.0f;
		cTmp =  (unsigned char) *pPointCur++;
		color.b = (float) cTmp/256.0f;
		/*cTmp =  (unsigned char) *pPointCur++;
		color.a = (float) cTmp/256.0f;*/
		//fread(&UV, sizeof(Point), 1, fp );
		//fread(&color, sizeof(Ogre::ColourValue), 1, fp );
		mTerrainData->mDiffuseInfoMap.insert(std::pair<Point, Ogre::ColourValue>(UV,color) );
	}

	if (buf != 0)
	{
		delete [] buf;
		buf =0;
	}
	
	return true;

}

void SceneManipulator::_loadDiffuseApplyInfo(const Ogre::String& filename)
{
	mDiffuseApplyInfoMap.clear();

	if (filename.empty())
	{
		return;
	}

	_readDiffuseApplyInfo(filename);
}

bool SceneManipulator::_readDiffuseApplyInfo(const Ogre::String& fileName)
{
	FILE *fp = NULL;
	int icount ;
	char* buf;
	char* pPointCur;
	mDiffuseApplyInfoMap.clear();
	fopen_s(&fp, fileName.c_str(), "rb");
	if (fp == NULL)
		return false;

	fread(&icount, sizeof(int), 1, fp );
	if(icount==0)
	{
		fclose(fp);
		fp = NULL;
		return false;
	}

	fseek( fp, 0, SEEK_END );
	int length = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	buf = new char[length+1];
	memset(buf, 0, length);
	buf[length]=0;
	pPointCur = buf;

	if ( fread( buf, length, 1, fp ) != 1 ) {
		//LogManager::getSingleton().logMessage("file import error.");
		fclose(fp);
		fp = NULL;

		delete [] buf;
		buf =0;
		return false;
	}

	fclose(fp);
	fp = NULL;

	int index = 4;
	pPointCur +=4;
	for(int i=0;i<icount;i++)
	{
		Point UV;
		static unsigned short* sTmp = NULL;

		sTmp = static_cast<unsigned short*>(static_cast<void*>(pPointCur));
		UV.x = *sTmp;
		pPointCur +=2;
		sTmp = static_cast<unsigned short*>(static_cast<void*>(pPointCur));
		UV.y = *sTmp;
		pPointCur +=2;		

		mDiffuseApplyInfoMap.insert(std::pair<Point, bool>(UV,true) );
	}

	if (buf != 0)
	{
		delete [] buf;
		buf =0;
	}

	return true;
}

void SceneManipulator::setPerspectiveCameraAsDefault(void)	
{   

}

void SceneManipulator::setOrthographicCameraAsDefault(void)	
{   

}

void SceneManipulator::changeCameraType(void)
{
    HitInfo terrainHitInfo;
	// ȡ��Ļ���ĵ���ε�λ�ã�����������˵�ľ�����Ϊ���ü�����
	Point pt(mViewport->getActualWidth() / 2,mViewport->getActualHeight() / 2);


	// ��������޽�����ֱ�ӷ���
	if(!makeTerrainHitInfo(terrainHitInfo, pt))
	{
		return;
	}

    // ͸��->�����ı任����
	static Ogre::Real baseNearClip = mCamera->getNearClipDistance();

    if (mCamera->getProjectionType() == Ogre::PT_PERSPECTIVE)
    {
		mCamera->setProjectionType( Ogre::PT_ORTHOGRAPHIC);
	    // ����3����ĸ��£� [6/9/2010 �¾���]
	    // 1.λ�õĸ��£���ʹ��x,zֵ��ʹ��ԭ�и߶�ֵy
	    // 2.����ĸ��£���ʹ����y����ת��ֵ
	    // 3.��������ĸ��£��������ü����й�

		Ogre::Real distance = mCamera->getRealPosition().y - terrainHitInfo.hitPosition.y;
		mCamera->setPosition(terrainHitInfo.hitPosition.x, terrainHitInfo.hitPosition.y + 2.5 * distance, terrainHitInfo.hitPosition.z);

		mCamera->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X));
		mCamera->setNearClipDistance(distance);
	}
	// ����->͸�ӵı任����
	else
	{
		mCamera->setProjectionType( Ogre::PT_PERSPECTIVE);

		Ogre::Vector3 vPoint = terrainHitInfo.hitPosition;
		Ogre::Real height =  mCamera->getRealPosition().y - 1.5*mCamera->getNearClipDistance(); 
		Ogre::Vector3 direction = - mCamera->getRealDirection();
		direction.normalise();
		Ogre::Ray rCameraRay = Ogre::Ray(vPoint, direction);
		// ������ߵĳ��ȣ����ݴ�������������λ��
		Ogre::Real length = Ogre::Math::Abs((height - vPoint.y) / direction.y);
		Ogre::Vector3 vTargetPoint= rCameraRay.getPoint(length);
		mCamera->setPosition(vTargetPoint);
		mCamera->setNearClipDistance(baseNearClip);
	}
}

void SceneManipulator::createObjectArray(int count,Ogre::Real size, Ogre::Vector3 axis)
{
	const Fairy::SceneManipulator::ObjectSet &objects = getSelectedObjects();
	Fairy::SceneManipulator::ObjectSet::const_iterator i = objects.begin();
	if (i != objects.end())
	{
		const Fairy::ObjectPtr& object = *i;
		
		Ogre::Vector3 length =size*axis;

		Ogre::Quaternion rot = Ogre::Quaternion::IDENTITY;
		if (object->hasProperty("orientation"))
		{
			rot = Ogre::StringConverter::parseQuaternion(object->getPropertyAsString("orientation"));
		}

		Fairy::SceneManipulator::ObjectSet addedObjects;

		int factor = count>0?1:-1;
		for (int j = 0; j<std::abs(count); ++j)
		{
			Fairy::ObjectPtr tempObjectPtr = Fairy::ObjectFactoryManager::getSingleton().createInstance(object->getType());
			const Fairy::PropertyList& properties = object->getProperties();
			for (Fairy::PropertyList::const_iterator itProperty = properties.begin(); itProperty != properties.end(); ++itProperty)
			{
				const Ogre::String& name = itProperty->name;
				if (!(object->getPropertyFlags(name) & PF_DERIVED))
				{
					Ogre::String value = object->getPropertyAsString(name);

					if ( name == "position" )
					{
						Ogre::Vector3 basePos = Ogre::StringConverter::parseVector3(value);
						Ogre::Vector3 adjustPos = basePos+rot*(length*(j+1)*factor);
						value = Ogre::StringConverter::toString(adjustPos);							 
					}
					tempObjectPtr->setPropertyAsString(name, value);
				}
			}				

			tempObjectPtr->createRenderInstance(this);
			tempObjectPtr->setName(getSceneInfo()->generateAutoName(tempObjectPtr));

			std::pair<Fairy::SceneManipulator::ObjectSet::iterator, bool> inserted =
				addedObjects.insert(tempObjectPtr);
			assert(inserted.second);
		}	

		if (addedObjects.size() >0)
		{
			addOjbects(addedObjects);		
		}
	}
}

}
