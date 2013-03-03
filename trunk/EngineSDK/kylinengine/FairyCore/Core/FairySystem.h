#ifndef __FAIRYSYSTEM_H__
#define __FAIRYSYSTEM_H__

#include "FairyPrerequisites.h"
#include "FairyObject.h"
#include <OgreString.h>
#include <OgreVector4.h>
#include <OgreColourValue.h>
#include <OgreCommon.h>
#include <map>
#include <set>
#include "gpuprogramhelper.h"
#include "OgreSingleton.h"
#include "Core/FairyObjectProxy.h"
namespace Ogre {
    class MovableObjectFactory;
	class TexCoordBillboardParticleRendererFactory;
	class MeshParticleRendererFactory;
	class MeshRotationAffectorFactory;
    class MeshAnimationAffectorFactory;
    class RibbonRendererFactory;
	class PakFileSystemArchiveFactory;
}

namespace Fairy {

	class IObjectFilter
	{
	public:
		virtual bool	Filter( const ObjectPtr &obj )	=	0;
	};

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------
enum eSystemInitFlag
{
	E_FORCE_LOAD_ALL_RESOURCE = 1,	
};


class XMLParser;

class CollisionModelManager;
class ObjectFactoryManager;
class PostFilterManager;
class EffectManager;
class BulletSystemManager;
class BulletFlowSystemManager;

class FrameStatsListener;
class SceneInfo;
class Terrain;
class TerrainData;
class TerrainTile;

class LogicModelManager;

class TerrainTypeInfos;
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class System //: public Ogre::Singleton<System>
{
public:

	// 保存异常信息，目前只保存丢失的object名称
	struct ExceptionInfo
	{	
		Ogre::String objectName;

		ExceptionInfo( const Ogre::String objName )
		: objectName(objName)
		{
		}
	};

	typedef std::vector<ExceptionInfo> ExceptionInfoList;
	// 每个异常信息可以对应多个object，因为可能在场景中多次使用了同个mesh
	typedef std::map<Ogre::String, ExceptionInfoList> ExceptionInfoMap;

    static const Ogre::String ANIMATION_QUALITY_CHANGED_MSG;
    static const Ogre::String CREATE_LEVEL_CHANGED_MSG;

public:
    // Initialise phase one
    System(void);
    virtual ~System();

    // Initialise phase two
    virtual void init(
        const Ogre::String& resourceFileName = "Resources.cfg",
        const Ogre::String& pluginFileName = "Plugins.cfg",
        const Ogre::String& configFileName = "Fairy.cfg",
        const Ogre::String& logFileName = "Fairy.log");


    // Initialise phase three
	virtual void setup(Ogre::ulong windowHandle, const Ogre::NameValuePairList* extraParams);

public:

    // Scene maintenance
    virtual bool isEditable(void) const;
    virtual bool isEmpty(void) const;
    virtual void resetScene(void);
    virtual void loadSceneFromResource(XMLParser* parser, const Ogre::String& name, const Ogre::String& groupName);

/*
	void setSetupInitFlag( unsigned int n )	{mSystemSetupInitFlag=n;}
	unsigned int getSetupInitFlag()			{return mSystemSetupInitFlag;}
*/
    // -------------------------------------------
    // Configure
    // -------------------------------------------
    virtual void setAnimationQuality(Ogre::Real animationQuality);
    virtual Ogre::Real getAnimationQuality(void) const;

    virtual AnimationType _determineAnimationType(Ogre::Real level);

    /** 设置系统的物体创建等级
    @param createLevel 物体创建等级，默认值为50，可以取<=50的任意数
    @remarks
    决定场景中的物体是否需要创建（或显示）的机制 ：
    每个可摆放在场景中的物体（WXPlacementObject）都有一个mCreateLevel来表示
    这个物体的创建等级，而当前系统中也有一个mSystemCreateLevel来总体地控制物体
    的创建，只有这两个数字加起来大于等于50，在重新读取场景时，这个物体才会被创建。
    如果设置了mSystemCreateLevel或单个物体的mCreateLevel而不重新打开场景，那么
    不符合创建条件的物体就会暂时的隐藏（visible为false）。
    每个物体的mCreateLevel的默认值为50，系统的mSystemCreateLevel默认值也为50，也就是
    说它们都为最大值，默认情况下都会创建所有物体。
    根据以上描述，我们可以建立多个物体创建等级来适用不同的机器，下面以三个等级举例。
    场景中有三个物体A，B，C，它们的mCreateLevel分别是0，10，50，
    mSystemCreateLevel在系统设置中也可以定为3个等级，分别为0，40，50，
    如果我们把mSystemCreateLevel设置为最低级，数值为0，这时就只有C会显示出来，
    如果我们把mSystemCreateLevel设置为中级，数值为40，这时B和C都能显示出来，
    如果mSystemCreateLevel调到最高级，数值为50，那么所有的物体都会显示出来。
    当然你也可以隐藏所有物体，只需把mSystemCreateLevel设置为负数。
    */
    virtual void setCreateLevel(Ogre::Real createLevel);

    /// 获取当前系统物体创建等级
    virtual Ogre::Real getCreateLevel(void) const;

    /** 根据传入的物体创建等级和当前系统的物体创建等级，来决定是否创建或显示该物体
    @return 如果为true，说明需要创建该物体
    */
    virtual bool _determineCreateLevel(Ogre::Real level);

    /** Bake static entities to static geometry.
    */
    virtual void bakeStaticGeometries(size_t maxMemoryUsage);

    /** Clear all baked static geometry.
    */
    virtual void clearStaticGeometries(void);

    // Render window maintenance
    virtual bool renderOneFrame(void);
    virtual void updateWindow(void);
    virtual void resizeWindow(size_t width, size_t height);

    // -------------------------------------------
    // General helper
    // -------------------------------------------

    /** Gets a world space ray as cast from the camera through a viewport position.
    @param winx, winy The x and y position at which the ray should intersect the viewport, 
        in window coordinates
    */
    virtual Ogre::Ray getWindowToViewportRay(Ogre::Real winx, Ogre::Real winy) const;

    /** Gets a world space ray as cast from the camera through a viewport position.
    @param pt The x and y position at which the ray should intersect the viewport, 
        in window coordinates
    */
    virtual Ogre::Ray getWindowToViewportRay(const Point& pt) const;

    virtual bool getTerrainIntersects(const Ogre::Ray& ray, Ogre::Vector3& position, Ogre::Vector3* normal = 0, bool allowOutside = false) const;
    virtual bool getTerrainIntersects(Ogre::Real winx, Ogre::Real winy, Ogre::Vector3& position, Ogre::Vector3* normal = 0, bool allowOutside = false) const;
    virtual bool getTerrainIntersects(const Point& pt, Ogre::Vector3& position, Ogre::Vector3* normal = 0, bool allowOutside = false) const;

    // -------------------------------------------
    // Accessor
    // -------------------------------------------

    CollisionModelManager* getCollisionModelManager(void) const { return mCollisionModelManager; }
    ObjectFactoryManager* getObjectFactoryManager(void) const   { return mObjectFactoryManager; }
    PostFilterManager* getPostFilterManager(void) const         { return mPostFilterManager; }

    Ogre::Root* getRoot(void) const                 { return mRoot; }
    Ogre::RenderWindow* getRenderWindow(void) const { return mRenderWindow; }
    Ogre::SceneManager* getSceneManager(void) const { return mSceneManager; }

    /** Returns the Ogre::SceneNode that all fairy object render instance will attach to.
    @remark
        All fairy scene objects attach to this scene node, so if you want to hide or render
        another set of objects, just detach this scene node from scene root node.
        This make render sub-scene more easy.
    */
    Ogre::SceneNode* getBaseSceneNode(void) const   { return mBaseSceneNode; }

    /** Returns the Ogre::SceneNode that all static fairy object render instance will attach to.
    @remark
        This scene node is a child of base scene node, and it is intend for STATIC/STILL object only.
    */
    Ogre::SceneNode* getStaticSceneNode(void) const { return mStaticSceneNode; }

    Ogre::Camera* getCamera(void) const             { return mCamera; }
    Ogre::Viewport* getViewport(void) const         { return mViewport; }

    FrameStatsListener* getFrameStatsListener(void) const   { return mFrameStatsListener; }

    Terrain* getTerrain(void) const                 { return mTerrain; }
    TerrainData* getTerrainData(void) const         { return mTerrainData; }
    SceneInfo* getSceneInfo(void) const             { return mSceneInfo; }

    Ogre::Real getBaseScale(void) const                   { return mBaseScale; }

	Ogre::ColourValue getBackgroundColour() const {return mBackgrounColour;}
	void setBackgroundColour(const Ogre::ColourValue& colour);

	// scene loading exception handle
	const System::ExceptionInfoMap & getExceptionInfoMap(void)	{	return mExceptionInfoMap;	}
	bool hasExceptionWhenLoadScene(void)	{	return mHasExceptionWhenLoadScene;	}
	void addExceptionInfo( const Ogre::String exceptionDesc, const System::ExceptionInfo &info );	
	void clearExceptionInfo(void);

    void setThrowExceptionIfResourceIncorrect(bool enable)  { mThrowExceptionIfResourceIncorrect = enable; }
    bool getThrowExceptionIfResourceIncorrect(void) const   { return mThrowExceptionIfResourceIncorrect; }

    void setDisableIncrementalBuildScene(bool disable)  { mDisableIncrementalBuildScene = disable; }
    bool getDisableIncrementalBuildScene(void) const    { return mDisableIncrementalBuildScene; }

	EffectManager *getEffectManager(void)	{	return mEffectManager;	}

//laim	BulletSystemManager* getBulletSystemManager() {return m_bulletSystemManager;}

    /// 获取地形类别信息
    TerrainTypeInfos* getTerrainTypeInfos(void)
    {
        return mTerrainTypeInfos;
    }

	void makeObjectTransparent( const Ogre::Vector3 &start, const Ogre::Vector3 &end, float nNewTrans, float nDelay, IObjectFilter *pFilter );

	void clearTransparencyObjects();


/* 版本差异
//// terrain shader param
	void setTerrainBrightness( float nNewBrightness );
	void setTerrainFocusColour( const Ogre::ColourValue &clrNew );
	void setTerrainRenderableCustomParams( Ogre::Renderable *pR );

	void makeObjectTransparent( const Ogre::Vector3 &start, const Ogre::Vector3 &end, float nNewTrans, float nDelay, IObjectFilter *pFilter );
	void clearObjectTransparent();
	bool traceToNearTerrainPos( const Ogre::Ray &ray, float nDistance , Ogre::Vector3 &vPos );
	void setWindDir( const Ogre::Vector3 &dir );
	void setWindForce( const float &nForce );
	void setTerrainLightmapThrehold( float nNewLightmapThrehold );
	void setTerrainSpeculareExtend( const Ogre::Vector3 &clr );
	void requestEffectOnRenderable( TerrainTile *pTT );

	void setTerrainVisible( bool bV );
///

	void setRenderSystemSchemaName( const String &sSchema );
	const String &getRenderSystemSchemaName();
*/
protected:

    // Initialise phase two helpers

    virtual void _registerExtraPlugins(void);

    virtual void _registerMovableObjectFactories(void);

    virtual void _setupResources(const Ogre::String& resourceFileName);
    virtual void _registerObjectFactories(void);

    // Initialise phase three helpers

	virtual void _createRenderWindow(Ogre::ulong windowHandle, const Ogre::NameValuePairList* extraParams);
    virtual void _createSceneManager(void);
    virtual void _createCamera(void);
    virtual void _createViewports(void);
    virtual void _createResourceListener(void);
    virtual void _loadResources(void);
    virtual void _initScene(void);

    // Scene creating/loading helpers

    virtual void _preprocessScene(void);
    virtual void _postprocessScene(void);
    virtual void _adjustCamera(void);	

protected:

    CollisionModelManager* mCollisionModelManager;
    ObjectFactoryManager* mObjectFactoryManager;
    PostFilterManager* mPostFilterManager;

    typedef std::list<Ogre::MovableObjectFactory*> MovableObjectFactoryList;
    MovableObjectFactoryList mMovableObjectFactoryList;

    Ogre::Root* mRoot;
    Ogre::RenderWindow* mRenderWindow;
    Ogre::SceneManager* mSceneManager;
    Ogre::SceneNode* mBaseSceneNode;
    Ogre::SceneNode* mStaticSceneNode;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;

	Ogre::ColourValue mBackgrounColour;

    FrameStatsListener* mFrameStatsListener;

    Ogre::Real mAnimationQuality;
    Ogre::Real mSystemCreateLevel;

    Terrain* mTerrain;
    TerrainData* mTerrainData;
    SceneInfo* mSceneInfo;
    std::list<Ogre::StaticGeometry*> mStaticGeometries;
    Ogre::Real mBaseScale;

	ExceptionInfoMap mExceptionInfoMap;
	bool mHasExceptionWhenLoadScene;
    bool mThrowExceptionIfResourceIncorrect;
    bool mDisableIncrementalBuildScene;

	EffectManager *mEffectManager;

	BulletSystemManager* mBulletSystemManager;
	BulletFlowSystemManager* mBulletFlowSystemManager;

	Ogre::TexCoordBillboardParticleRendererFactory *mTexCoordBillboardParticleRendererFactory;
	Ogre::MeshParticleRendererFactory *mMeshParticleRendererFactory;
	Ogre::MeshRotationAffectorFactory *mMeshRotationAffectorFactory;
    Ogre::MeshAnimationAffectorFactory *mMeshAnimationAffectorFactory;
    Ogre::RibbonRendererFactory *mRibbonRendererFactory;

	LogicModelManager *mLogicModelManager;

    TerrainTypeInfos* mTerrainTypeInfos;

	Ogre::PakFileSystemArchiveFactory *mWXFileSystemArchiveFactory;

	//遮挡人物的半透明物体列表
	std::set<Fairy::Object*>    m_setTransparencyObjects;
};

}

#endif 
