#ifndef __SCENEMANIPULATOR_H__
#define __SCENEMANIPULATOR_H__

#include "Core/FairySystem.h"
#include "WXSceneListener.h"

#include <OgreString.h>

namespace Fairy {

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class XMLParser;
class XMLWriter;
class OperatorManager;
class FrameStatsListener;
class SceneInfo;
class Manip;
class Action;
class Selection;
struct FuncParam;
class EnviromentObject;


const Ogre::String FairyAutoEnviromentObjectName = "#Enviroment";
const Ogre::String FairyAutoSkyDomeObjectName = "#SkyDome";
const Ogre::String FairyAutoMainLightOneObjectName = "#MainLightOne";
const Ogre::String FairyAutoMainLightTwoObjectName = "#MainLightTwo";

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

/** An scene manipulator
@remarks
@par
*/
class SceneManipulator : public System, public SceneListenerManager
{
public:
	void loadScene(const String& filename);
    void resetScene(void);
    void newScene(const String& texName);

    bool isModified(void) const;
    bool isEditable(void) const;

    // -------------------------------------------
    // Accessor
    // -------------------------------------------

    XMLParser* getXMLParser(void) const             { return mXMLParser; }
    XMLWriter* getXMLWriter(void) const             { return mXMLWriter; }

    OperatorManager* getOperatorManager(void) const { return mOperatorManager; }

    /** Gets the indicator scene node.
    @remark
        All editor special renderables, movables, scene nodes MUST be attach under this node
    */
    Ogre::SceneNode* getIndicatorRootSceneNode(void) const  { return mIndicatorRootSceneNode; }

    // -------------------------------------------
    // Helper
    // -------------------------------------------

    void moveCameraToUsableLocation(void);

    // -------------------------------------------
    // Objects accessor
    // -------------------------------------------

    void addObject(const ObjectPtr& object, SceneListener* exclude = 0, bool disableUndo = false);
    void removeObject(const ObjectPtr& object, SceneListener* exclude = 0, bool disableUndo = false);

    void renameObject(const ObjectPtr& object, const String& newName, SceneListener* exclude = 0, bool disableUndo = false);

	void removeObject(const Ogre::String objName);

    typedef std::set<ObjectPtr> ObjectSet;

    const ObjectSet& getSelectedObjects(void) const;
    bool hasSelectedObjects(void) const;
    bool isObjectSelected(const ObjectPtr& object) const;
    bool selectObject(const ObjectPtr& object, SceneListener* exclude = 0);
    bool deselectObject(const ObjectPtr& object, SceneListener* exclude = 0);
    bool clearSelectedObjects(SceneListener* exclude = 0);
    bool removeSelectedObjects(SceneListener* exclude = 0, bool disableUndo = false);
    // ���������object�����ڿ���ճ��
    bool addOjbects(const ObjectSet& objects, SceneListener* exclude = 0, bool disableUndo = false);
    // -------------------------------------------
    // Mouse manip
    // -------------------------------------------

    Manip* getManip(const String& name) const;

    Selection* _getSelection(const String& type) const;

    Action* _getAction(const String& name) const;

    void setActiveAction(const String& name);
    void setActiveAction(Action* action);
    Action* getActiveAction(void) const { return mActiveAction; }

    Real getBaseScale(void) const       { return mBaseScale; }

    void setMoveFactor(Real value)      { mMoveFactor = value; }
    Real getMoveFactor(void) const      { return mMoveFactor; }
    Real _getMoveFactor(void) const     { return mMoveFactor * mBaseScale; }

    void setRotateFactor(Real value)    { mRotateFactor = value; }
    Real getRotateFactor(void) const    { return mRotateFactor; }

    void setMoveSpeed(Real value)       { mMoveSpeed = value; }
    Real getMoveSpeed(void) const       { return mMoveSpeed; }
    Real _getMoveSpeed(void) const      { return mMoveSpeed * mBaseScale; }

    void setRotateSpeed(Real value)     { mRotateSpeed = value; }
    Real getRotateSpeed(void) const     { return mRotateSpeed; }

    void setWheelSpeed(Real value)      { mWheelSpeed = value; }
    Real getWheelSpeed(void) const      { return mWheelSpeed; }
    Real _getWheelSpeed(void) const     { return mWheelSpeed * mBaseScale; }

    void setCameraHeightLocked(bool value)  { mCameraHeightLocked = value; }
    bool getCameraHeightLocked(void) const  { return mCameraHeightLocked; }

    void setShowStandardModel(bool value)   { mShowStandardModel = value; }
    bool getShowStandardModel(void) const   { return mShowStandardModel; }

    void setHeightAdjustSpeed(Real value)   { mHeightAdjustSpeed = value; }
    Real getHeightAdjustSpeed(void) const   { return mHeightAdjustSpeed; }
    Real _getHeightAdjustSpeed(void) const  { return mHeightAdjustSpeed * mBaseScale; }

    Real getPositionAlignGranularity(void) const    { return mPositionAlignGranularity; }
    Real getRotationAlignGranularity(void) const    { return mRotationAlignGranularity; }
    void setPositionAlignGranularity(Real value)    { mPositionAlignGranularity = value; }
    void setRotationAlignGranularity(Real value)    { mRotationAlignGranularity = value; }
    Ogre::Quaternion _alignRotation(const Ogre::Quaternion& value);
    Ogre::Quaternion _alignRotation(const Ogre::Quaternion& value, Real granularity);
    void _snapSelectedObjectsRotation(Real granularity,int axis);
    void calcOffsetRadian(const Ogre::Degree& degree, const float& granularity,float& remainder);

    void setBrush(const String& type, size_t xsize, size_t zsize);
    const String& getBrushType(void) const  { return mBrushType; }
    size_t getBrushXSize(void) const        { return mBrushXSize; }
    size_t getBrushZSize(void) const        { return mBrushZSize; }
    void setBrushXSize(size_t size)         { mBrushXSize = size; }
    void setBrushZSize(size_t size)         { mBrushZSize = size; }
    void _buildSelection(Selection* selection, Real xcentre, Real zcentre);
    void _buildSelection(const String& selectionType, Real xcentre, Real zcentre);

    /// ��ʵ�������ص����ú���
	void setDefaultRealCameraHeight( const Real height ) { mDefaultRealCameraHeight = height; }
	void setDefaultRealCameraAngle( const Real angle ) { mDefaultRealCameraAngle = angle; }
	float getDefaultRealCameraHeight(void) { return mDefaultRealCameraHeight; }
	float getDefaultRealCameraAngle(void) { return mDefaultRealCameraAngle; }

    void setRealCameraHeight( const Real height ) { mRealCameraHeight = height; }
    void setRealCameraAngle( const Real angle );

	void setUseRealCameraHeight( bool use )	{	mUseRealCameraHeight = use;	}
	void setUseRealCameraAngle( bool use )	{	mUseRealCameraAngle = use;	}

    float getRealCameraHeight(void) { return mRealCameraHeight; }
    float getRealCameraAngle(void) { return mRealCameraAngle; }
	bool getUseRealCameraHeight(void) { return mUseRealCameraHeight;	}
	bool getUseRealCameraAngle(void)	{	return mUseRealCameraAngle;	}

    /// ���ƶ��������action��������ʵ�������
    void setRealCamera( const Real currentX, const Real currentY );


    /// ��ȡ��ǰѡ�е���������ԣ������ַ���������clipboard��
    void getSelectedObjectInfo( Ogre::String &infoString );

    /// ���ݴ�clipboard�еõ��ı����������Ե��ַ�������paste����
    void parseStringAndCreateObject( const Ogre::String &infoString, Ogre::Vector3 &position );


    Ogre::Image * getPreviewImage( const Ogre::String &textureName );

    void reloadPaintInfo(void);  

	/// ���øı���θ߶ȵ�Ӱ�췶Χ
	void setJunctionWeightAffectRange( int range )	{	mAffectRange = range;	}
	int getJunctionWeightAffectRange(void) {	return mAffectRange; }

	/// ���øı���θ߶�ʱ���õ�kֵ
	void setJunctionWeightExpValue( float exp ) {	mExponentValue = exp; }
	float getJunctionWeightExpValue(void)	{	return mExponentValue;	}

	/// ������ѡ�������������ҳ���ǰ�����Ƿ��õ����������������õ���������Ĵ���
	size_t findGridPaintInfo( String texGroupName );

	String getTexGroupNameFromTextureName( String &textureName );

	//const GridPaintInfoList &getResultPaintInfoList(void)
	//{
	//	return mResultGridPaintInfos;
	//}

	void setSelectAllObjects( bool set = true )	{	mSelectAllObjects = set;	}
	bool getSelectAllObjects(void)	{	return mSelectAllObjects;	}

	bool IsCenterSelectObject( Ogre::String objectType );

	void fillUnableCenterSelectObject(void);	



    /// ����ָʾ���ڵ�Ŀɼ���
    void setIndicatorNodeVisible(bool visible);
    bool getIndicatorNodeVisible(void)
    {
        return mIndicatorNodeVisible;
    }

    /// �����������λ��
    /** ����Ŀ�ĵ����꣬����ݲ�ͬ������д����統ǰ������Ƿ������߶ȣ�
    */
    void setCameraPosition(const Ogre::Vector3& pos);

	void createGrid();
	void createTerrain(const String& tex);
	void setTerrainVisible(bool visible);

	void setGridVisible(bool visible);

public:

    SceneManipulator(void);
    ~SceneManipulator();

protected:

    void registerManips(void);
    void registerManip(Manip* manip);
    void destoryManips(void);

    void registerActions(void);
    void registerAction(Action* action);
    void destoryActions(void);

    void checkStdObjects(bool autoCreate = true);

	void registerAddObjectPlugins(void);
	void destroyAddObjectPlugins(void);

protected:

    // Overrides

    void _loadResources(void);
    void _initScene(void);
    void _preprocessScene(void);
    void _postprocessScene(void);

	void _createTerrain(size_t width, size_t depth, size_t tileSize = 32,
		const String& baseTexture = Ogre::StringUtil::BLANK,
		size_t seed = 0, Real scale = 100, Real height = 100, Real h = 0.85f);

    // Editor special helpers

    void _checkStdObjects(bool autoCreate = true);

protected:

    // XML load/save
    XMLParser* mXMLParser;
    XMLWriter* mXMLWriter;

    // Configure

    Real mMoveFactor;
    Real mRotateFactor;
    Real mMoveSpeed;
    Real mRotateSpeed;
    Real mWheelSpeed;
    bool mCameraHeightLocked;
    Real mHeightAdjustSpeed;
    Real mPositionAlignGranularity;
    Real mRotationAlignGranularity;
    bool mShowStandardModel;
    String mBrushType;
    size_t mBrushXSize;
    size_t mBrushZSize;

    /// ��ʵ������ĸ߶ȣ��Ƕ�
    Real mRealCameraHeight;
    Real mRealCameraAngle;
    Real mRealLastCameraAngle;

	Real mDefaultRealCameraAngle;
	Real mDefaultRealCameraHeight;

    /// �Ƿ�ʹ����ʵ�����
	bool mUseRealCameraHeight;
	bool mUseRealCameraAngle;

	/// ������λ�ˢȨ������Ĳ���
	int mAffectRange;
	float mExponentValue;
    // Manips

    typedef std::map<String, Manip*> Manips;
    Manips mManips;


    typedef std::map<String, Selection*> Selections;
    Selections mSelections;

    typedef std::map<String, Action*> Actions;
    Actions mActions;
    Action* mActiveAction;

    // Undo/redo manage

    typedef size_t Timestamp;
    typedef std::map<String, Timestamp> Snapshot;

    OperatorManager* mOperatorManager;
    Snapshot mSnapshot;

    Ogre::SceneNode* mIndicatorRootSceneNode;

    ObjectSet mSelectedObjects;


    // �����в��ɼ��������ָʾ������Դ�ȣ�
    typedef std::vector<Fairy::SceneListener *> AddObjectPlugins;
    AddObjectPlugins mAddObjectPlugins;

    // ͸�������б������alphaͨ�������������
    typedef std::map<String, bool> TransparentTextures;
    TransparentTextures mTransparentTextures;

    bool mObjectsUseBoxCenter;
    bool mAxisGizmoActive;



	/// �����п���ѡ��ģ������������ĵ�ѡ��������б�
	typedef std::list<Ogre::String> UnableCentenSelectObject;
	UnableCentenSelectObject mUnableCentenSelectObject;

	/// ��ǰ��ѡ��״̬���Ƿ�ѡ����������ĵ����ѡ������壩
	bool mSelectAllObjects;

    bool mIndicatorNodeVisible;

	Ogre::SceneNode*  mGridNode;

	bool mTerrainVisible;
	bool mGridVisible;

public:

    //createScene functables
    typedef void (*algorithmFunc) (FuncParam* param);
    typedef std::map<Ogre::String,algorithmFunc> FuncMap;
    FuncMap funcMap;

    bool getUseBoxCenterSelection();
    bool getUseAxis();
    void setUseAxis(bool use);

	Ogre::Vector3 _alignPosition(const Ogre::Vector3& value);
	Ogre::Vector3 _alignPosition(const Ogre::Vector3& value, Real granularity);

	bool getTerrainVisible(){return mTerrainVisible;}
	bool getGridVisible(){return mGridVisible;}
};


}

#endif // __SceneManipulator_H__
