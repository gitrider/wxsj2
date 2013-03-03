//���ݲ�����
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

const DString NEWLOCATORTIP			= "�½��󶨵�";
const DString PREFIX_NEW_LOCATOR	= "�µİ󶨵�_";
const DString DELLOCATORTIP			= "ɾ����ǰ�󶨵�";

const DString NEWEFFECTTIP			= "�½���Ч";
const DString PREFIX_NEW_EFFECT		= "�µ���Ч_";
const DString DELEFFECTTIP			= "ɾ����Ч";

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
		bool					stateEnabled;		//����״̬�Ƿ���Ч
		//Ogre::SceneNode*		node;				//mesh�ĳ����ڵ�
	}CUSTOM_PARAM;

public:
	void Build();

	bool IsBuild(){
		return m_bBuild;
	}

	void UpdateAllResource();													//����������Դ

	void UpdateObjResource();													//��������obj��Դ

	void UpdateAnimationListUI();												//���¶����б����

	void UpdateMeshAnimationListUI();

	void UpdateObjFileEditDlgUI2();												//�µĸ���obj�ļ��༭���溯��

	void OnInitObjTemplate();													//��ʼ��objģ��

	void OnInitMatTable();														//��ʼ�����ʱ�

	void OnInitEffectTable();													//��ʼ����Ч��

	void OnInitSkeltonTable();													//��ʼ��������

	void OnInitHintsTable();

	void OnInitMeshTable();

public:

	void SetAnimationSelector(AnimationSelector* pAnimationSelector){			//���ö�������ָ��
		m_pAnimationSelector = pAnimationSelector;
	}

	AnimationSelector* GetAnimationSelector(){									//��ȡ��������ָ��
		return m_pAnimationSelector;
	}

	ObjFileEditorPanel* GetObjFileEditorPanel(){										//��ȡobj�ļ��༭����ָ��
		return m_pObjFileEditorPanel;
	}

	void SetObjFileEditorPanel(ObjFileEditorPanel* pObjFileEditorPanel){				//����obj�ļ��༭����ָ��
		m_pObjFileEditorPanel = pObjFileEditorPanel;
	}

public:
	Fairy::SceneManipulator* m_pSceneManipulator;								//����������

	AnimationSelector* m_pAnimationSelector;									//��������ָ��

public:

	DStringList m_MatTable;														//���ʱ�
	std::map<DString,DString> m_MatFileList;									//�����ļ���

	DStringList m_ObjTable;														//obj�ļ���

	DStringList m_EffectTable;													//��Ч�ļ���

	DStringList m_SkeletonTable;												//�����ļ���

	DStringList m_MeshTable;													//ģ���ļ���

private:
	bool m_bBuild;																//������־

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