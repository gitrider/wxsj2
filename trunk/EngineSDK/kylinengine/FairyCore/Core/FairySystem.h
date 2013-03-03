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

	// �����쳣��Ϣ��Ŀǰֻ���涪ʧ��object����
	struct ExceptionInfo
	{	
		Ogre::String objectName;

		ExceptionInfo( const Ogre::String objName )
		: objectName(objName)
		{
		}
	};

	typedef std::vector<ExceptionInfo> ExceptionInfoList;
	// ÿ���쳣��Ϣ���Զ�Ӧ���object����Ϊ�����ڳ����ж��ʹ����ͬ��mesh
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

    /** ����ϵͳ�����崴���ȼ�
    @param createLevel ���崴���ȼ���Ĭ��ֵΪ50������ȡ<=50��������
    @remarks
    ���������е������Ƿ���Ҫ����������ʾ���Ļ��� ��
    ÿ���ɰڷ��ڳ����е����壨WXPlacementObject������һ��mCreateLevel����ʾ
    �������Ĵ����ȼ�������ǰϵͳ��Ҳ��һ��mSystemCreateLevel������ؿ�������
    �Ĵ�����ֻ�����������ּ��������ڵ���50�������¶�ȡ����ʱ���������Żᱻ������
    ���������mSystemCreateLevel�򵥸������mCreateLevel�������´򿪳�������ô
    �����ϴ�������������ͻ���ʱ�����أ�visibleΪfalse����
    ÿ�������mCreateLevel��Ĭ��ֵΪ50��ϵͳ��mSystemCreateLevelĬ��ֵҲΪ50��Ҳ����
    ˵���Ƕ�Ϊ���ֵ��Ĭ������¶��ᴴ���������塣
    �����������������ǿ��Խ���������崴���ȼ������ò�ͬ�Ļ����������������ȼ�������
    ����������������A��B��C�����ǵ�mCreateLevel�ֱ���0��10��50��
    mSystemCreateLevel��ϵͳ������Ҳ���Զ�Ϊ3���ȼ����ֱ�Ϊ0��40��50��
    ������ǰ�mSystemCreateLevel����Ϊ��ͼ�����ֵΪ0����ʱ��ֻ��C����ʾ������
    ������ǰ�mSystemCreateLevel����Ϊ�м�����ֵΪ40����ʱB��C������ʾ������
    ���mSystemCreateLevel������߼�����ֵΪ50����ô���е����嶼����ʾ������
    ��Ȼ��Ҳ���������������壬ֻ���mSystemCreateLevel����Ϊ������
    */
    virtual void setCreateLevel(Ogre::Real createLevel);

    /// ��ȡ��ǰϵͳ���崴���ȼ�
    virtual Ogre::Real getCreateLevel(void) const;

    /** ���ݴ�������崴���ȼ��͵�ǰϵͳ�����崴���ȼ����������Ƿ񴴽�����ʾ������
    @return ���Ϊtrue��˵����Ҫ����������
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

    /// ��ȡ���������Ϣ
    TerrainTypeInfos* getTerrainTypeInfos(void)
    {
        return mTerrainTypeInfos;
    }

	void makeObjectTransparent( const Ogre::Vector3 &start, const Ogre::Vector3 &end, float nNewTrans, float nDelay, IObjectFilter *pFilter );

	void clearTransparencyObjects();


/* �汾����
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

	//�ڵ�����İ�͸�������б�
	std::set<Fairy::Object*>    m_setTransparencyObjects;
};

}

#endif 
