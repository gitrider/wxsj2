//数据操作类
#ifndef __DATAMANIPULATOR_H__
#define __DATAMANIPULATOR_H__

#include <wx/filename.h>
#include <wx/msgdlg.h>


#include "SceneManipulator.h"

#include "AnimationSelector.h"
#include "ObjFileEditDlg.h"
#include "ObjFileEditorPanel.h"
#include "res/ObjFileEditorPanelHelper.h"

#include "OgreSkeletonManager.h"
#include "OgreSkeleton.h"

class wxButton;

namespace Fairy {

typedef	Ogre::String				DString;
typedef	std::vector<DString>		DStringList;

const DString NEWLOCATORTIP			= "新建绑定点";
const DString PREFIX_NEW_LOCATOR	= "新的绑定点_";
const DString DELLOCATORTIP			= "删除当前绑定点";

const DString NEWEFFECTTIP			= "新建特效";
const DString PREFIX_NEW_EFFECT		= "新的特效_";
const DString DELEFFECTTIP			= "删除特效";

class CDataManipulator
{
public:
	static CDataManipulator* m_DataManipulatr;
	static Ogre::SceneNode* m_baseNode;
	static	Ogre::SceneNode* m_axex;
	static	Ogre::Entity*	 mDummy;
	static Ogre::SceneNode* CDataManipulator::mAxisNode_x;
	static Ogre::SceneNode* CDataManipulator::mAxisNode_y;
	static Ogre::SceneNode* CDataManipulator::mAxisNode_z;
	static	Ogre::Entity*	 mAxis_x;
	static	Ogre::Entity*	 mAxis_y;
	static	Ogre::Entity*	 mAxis_z;
	static CDataManipulator* GetDataManipulator(){
		return m_DataManipulatr;
	}
public:
	CDataManipulator(SceneManipulator* sceneManipulatorid);

	~CDataManipulator(void);
public:
	enum HintType
	{
		HT_MESH = 1,
		HT_MATERIAL,
		HT_SLOT,
	};

	typedef struct _CUSTOM_PARAM{
		Ogre::AnimationState*	animState;
		bool					stateEnabled;		//动画状态是否有效
		//Ogre::SceneNode*		node;				//mesh的场景节点
	}CUSTOM_PARAM;

public:
	void Build();

	bool IsBuild(){
		return m_bBuild;
	}

	void UpdateAllResource();													//更新所有资源

	void UpdateObjResource();													//更新所有obj资源

	void UpdateAnimationListUI();												//更新动画列表界面

	void UpdateMeshAnimationListUI();

	void UpdateObjFileEditDlgUI2();												//新的更新obj文件编辑界面函数

	void OnInitObjTemplate();													//初始化obj模板

	void OnInitMatTable();														//初始化材质表

	void OnInitEffectTable();													//初始化特效表

	void OnInitSkeltonTable();													//初始化骨骼表

	void OnInitHintsTable();

	void OnInitMeshTable();

public:

	void SetAnimationSelector(AnimationSelector* pAnimationSelector){			//设置动画界面指针
		m_pAnimationSelector = pAnimationSelector;
	}

	AnimationSelector* GetAnimationSelector(){									//获取动画界面指针
		return m_pAnimationSelector;
	}

	ObjFileEditorPanel* GetObjFileEditorPanel(){										//获取obj文件编辑界面指针
		return m_pObjFileEditorPanel;
	}

	void SetObjFileEditorPanel(ObjFileEditorPanel* pObjFileEditorPanel){				//设置obj文件编辑界面指针
		m_pObjFileEditorPanel = pObjFileEditorPanel;
	}

public:
	Fairy::SceneManipulator* m_pSceneManipulator;								//场景管理器

	AnimationSelector* m_pAnimationSelector;									//动画界面指针

public:

	DStringList m_MatTable;														//材质表
	std::map<DString,DString> m_MatFileList;									//材质文件表

	DStringList m_ObjTable;														//obj文件表

	DStringList m_EffectTable;													//特效文件表

	DStringList m_SkeletonTable;												//骨骼文件表

	DStringList m_MeshTable;													//模型文件表

private:
	bool m_bBuild;																//创建标志

	typedef std::map<DString,DStringList> SkeAniLinkList;
	SkeAniLinkList m_skeAniLinkList;

	Ogre::Entity* m_IndicatorEntity;

	bool m_bObjModified;

public:

	DStringList m_skeltonFileList;
	DString m_CurObjFile;														// Current obj file 
	DString m_CurMeshFile;														// Current mesh file

	void LoadMeshRes(Ogre::String meshName);

	void LoadObjTemplateRes(Ogre::String templatename);
	bool SaveToObj(Ogre::String fileName);

	Fairy::LogicModel* m_pObjTemplate;
	Fairy::ObjectPtr	m_pMeshObject;

	ObjFileEditorPanel* m_pObjFileEditorPanel;

	Fairy::LogicModel::EffectHandle m_curEffectHandle;
	
	typedef std::map<Ogre::String, Ogre::String> BoneList;
	BoneList m_curBoneList;

	Ogre::SceneNode* m_pObjSceneNode;

	Ogre::MaterialPtr m_pMaterialX;
	Ogre::MaterialPtr m_pMaterialY;
	Ogre::MaterialPtr m_pMaterialZ;

	bool mShowBoundingBox;
	bool mShowWorldBoundingBox;
	Ogre::WireBoundingBox* mBoxIndicator;
	Ogre::SceneNode* mBoxSceneNode;

	bool mSaveDefaultSkeleton;

	// For mesh animation
	Ogre::Animation*		mAnim;				
	Ogre::AnimationState*	mAnimState;	
	CUSTOM_PARAM*			mAnimParam;


public:
	void UpdateAttributeInfo(DString attibuteName);
	bool ChangeAttributeValue(DString attributeValue);
	void UpdateEntityInfo(DString entityName);
	void UpdateMaterialInfo(DString materialName);
	void UpdateSlotInfo(DString slotName, DString locatorName);
	void UpdateLocatorInfo(DString locatorName);
	bool ChangeLocatorInfo(DString locator, DString bone, Ogre::Vector3 pos, Ogre::Quaternion rot);

	void _updateAxexInfo();

	bool _updateBoneList();
	bool _updateSkeletonList();
	bool _updateSkeAniList();

	bool _updateEffectList();
	void UpdateEffectInfo(DString effectName);
	void changeEffectInfo( const DString& name, const DString& effectName, const DString& locatorName, bool translateAll, const Ogre::ColourValue& colour);
	bool changeEffectName( const DString& oldName, const DString& newName);

	bool deleteCurEffect();
	bool addEffectInfo(const DString& effectName);

	bool changeEntityMeshAndMat(const DString &name,const DString &mesh, const DString & material);
	bool changeMaterialEntiryAndMat(const DString &name,const DString &entity, const DString & material);
	bool changeLocatorSlot(const DString & locatorName, const DString &slotName, const DString &objName, const DString &attrib );
	bool changeLocatorName(const DString & oldLocatorName, const DString& newLocatorName);
	int changeAttributeName(const DString & oldAttributeName, const DString& newAttributeName);
	bool updateUIAfterByAttributeName(const DString & oldAttributeName, const DString& newAttributeName, int hint);
	void _updateCurLocatorTrans(Ogre::Vector3 deltaPos, Ogre::Quaternion deltaRot, bool trans = true);
	void _updateCurLocatorRot(Ogre::Quaternion deltaRot);
	bool deleteCurLocator();
	void changeSlotsEffect(const DString& effect, const DString& locatorName);
	DString isSelSlotUsed(const DString& locatorName, const DString& slotName);

	bool switchSlot(const Ogre::String & oldLocatorName, const Ogre::String & newLocatorName, const Ogre::String &slotName);

	DString _formatValidLocatorName();
	DString _formatValidEffectName();

	void updateAnimationInfo();
	bool changeAnimation(const Ogre::String& aniName, Ogre::Real delay);
	bool changeMeshAnimation(const Ogre::String& aniName);

	void _updateAxisByBone(const Ogre::String & boneName);

	// Helpers
	bool _addSkeleton(const Ogre::String &SkeletonName, bool bCheckSkeMatching = true);
	bool _delSkeleton(const Ogre::String &SkeletonName);
	Ogre::Skeleton::BoneIterator _getBone(const Ogre::String &SkeletonName);
	LogicModelManager::AnimationSkeletonLinks * _getCurAniLinks();
	bool _IsSkeletonMatching(const Ogre::String & baseSkeleton, const Ogre::String &newSkeleton);

	bool isModified(){ return m_bObjModified;}
	void setModified(bool bModified){ m_bObjModified = bModified; }

	bool isCurObjHasEntity();
	DString _findMaterialFromEntity(const DString & entityName);

	void switchBoundingBox(bool bWorld = true);
	void showWorldBoundingBox(bool bShow = true);

	void setSaveDefaultSkeleton(bool flag);

};
}

inline Fairy::CDataManipulator* GetDataManipulator()
{
	return Fairy::CDataManipulator::GetDataManipulator();
}

#endif