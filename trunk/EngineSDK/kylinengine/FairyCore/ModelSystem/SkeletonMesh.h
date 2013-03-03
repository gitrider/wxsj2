#ifndef _SKELETONMESH_H_
#define _SKELETONMESH_H_

#include "ModelTypes.h"

//ogre ͷ�ļ�����
#include <OgrePrerequisites.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreAxisAlignedBox.h>
#include <OgreAny.h>

// fairy effect header
#include "EffectSystem/FairyEffect.h"
#include "EffectSystem/FairyEffectManager.h"
#include "OgreExt/OgreUserDefinedObject.h"
namespace Fairy	{
	class System;
};

// ����Ӱ���ʶ
//Attribute hint define
#define AH_MESH							(0x01)
#define AH_ANIMATION					(0x02)
#define AH_LOCATOR						(0x04)
#define AH_MATERIAL						(0x08)				//add by zhuwenchao 2010.1.21 ��һ��������
#define AH_ALL							(AH_MESH|AH_ANIMATION|AH_LOCATOR)

//����Ԥ����


typedef			Ogre::String				DString;
typedef			std::vector<DString>		DStringList;
typedef			unsigned long				dword;

typedef unsigned int EffectHandle;

struct FairyEffectInfo
{
	FairyEffectInfo() :	mEffect(NULL), mLocator("")
	{
	}

	Fairy::Effect *mEffect;
	Ogre::String mLocator;
};

typedef std::map<EffectHandle, FairyEffectInfo> FairyEffectMap;

//���ߺ���������ʼ
void				  MeshLog(bool bShowDialog, const char* format, ...);
typedef bool          (*_OnAnimationEnd)(const char* pAnimationName, unsigned long info);

typedef bool          (*_OnSkillHitTime)(const char* pAnimationName, unsigned long info);
typedef bool          (*_OnSkillBreakTime)(const char* pAnimationName, unsigned long info);
typedef bool          (*_OnShakeTime)(const char* pAnimationName, unsigned long info);

// ��������ָ��
typedef int (__stdcall* OnPlaySound)(const char* szSoundFile, float* fvPos, bool isLoop);
typedef void (__stdcall* OnStopSound)(int handle);

enum ObjEntityIndex
{
	Face,
	Hair,
	Cap,
	MainBody,
	Foot,
	Arm,
	Shoulder,

	EntityCount,
};

enum GetTransformInfoType
{
	GTIT_POSITION,
	GTIT_ORIENTATION,
	GTIT_ALL,
};

// object���ͣ���Ҫ������object�����߷�ʽ
enum ObjectMovementType
{
	OT_NORMAL, // ��ͨ���ͣ��������������object�ķ�λ
	OT_ROTATE_X_AXIS,	// ֻ��ǰ��ҡ��
	OT_ROTATE_X_Z_AXIS,	// ����ǰ������ҡ��
};

class DObject_NT;

// �ҽ���Ϣ
struct AttachInfo
{
	AttachInfo(DString &point, DObject_NT *object) :
	mAttachPoint(point), mAttachObject(object)
	{
	}

	AttachInfo() :
	mAttachPoint(""), mAttachObject(NULL)
	{
	}

	// ���ҽӵĵ������
	DString mAttachPoint;
	// ���ҽӵ�����
	DObject_NT *mAttachObject;
};

typedef std::list<AttachInfo> AttachInfos;

// ���ҽ��ϵ�����ԭ����scene node
typedef std::map<DObject_NT *, Ogre::SceneNode *> AttachedObjectOriginSceneNodes;

struct ObjEntity
{
	ObjEntity() : matName(""), pEntity(NULL)	{	}
	DString				matName;
	Ogre::Entity*		pEntity;
};

class SkeletonMeshUtil;

class MODEL_API	DObject_NT
{
protected:	

	FairyEffectMap mFairyEffectMap;

	friend	class SkeletonMeshUtil;

	Fairy::Skill *mCurrentSkill;

public:
	//���캯��
	DObject_NT( Fairy::System *system );

	
	virtual ~DObject_NT();
	
	void							Init();

	void							ReSet();

	// ���ó�������Ŀɼ���־
	void							setVisibleFlag(unsigned int nVisibleFlag);

    void                            setUserAny(const Ogre::Any& anything);
    const Ogre::Any&                getUserAny(void) const;
    void                            setUserObject(Ogre::UserDefinedObject* obj);
    Ogre::UserDefinedObject*        getUserObject(void) const;

	//���Դ�����
	void							SetGroupName(DString StrGroupName) {	m_GroupName = StrGroupName;	}

	void							SetObjFileName(DString StrObjFileName);
	DString							GetObjFileName(){return m_ObjFile;};

	void							SetObjName(DString StrObjName);
	DString							GetObjName(){return m_ObjName;};

	//�����ڵ㷽��
	void							SetObjectNode(Ogre::SceneNode* pNode){ m_pObjNode = pNode;};
	Ogre::SceneNode*							GetObjectNode(){return m_pObjNode;};


	bool							isChildObject(){return m_isChildObject;};
	void							SetIsChildObject(bool isChild){m_isChildObject = isChild;};

	void							SetAttachedVisibleEntity(Ogre::Entity* pEntity){m_pAttachedVisibleEntity = pEntity;};
	Ogre::Entity*							GetAttachedVisibleEntity(){return m_pAttachedVisibleEntity;};


	void							SetGlobalAnimationRate(float fRate);

	void                            CreateSkill(const DString &skillName, bool loop = true,
										bool anim = false, Ogre::Real delayTime = 0.0f);

	void                            DelCurrentSkill(void);

	Fairy::Skill *					GetCurrentSkill(void)
	{	
		return mCurrentSkill;
	}

	float                           GetCurrentAnimationLength(void);

    virtual bool					Execute(Ogre::Real delta);

    bool							ChangeAction(const DString& actioname, Ogre::Real delay = 0);

	bool							ChangeAttributes(const DStringList& attribnames, const DStringList& attribvalues);

	bool							SetParentNode(Ogre::SceneNode* parentnode);

	EffectHandle					AddEffect(const Ogre::String &effectName, const Ogre::String &locatorName);
	std::pair<bool, FairyEffectMap::iterator>		DelEffect( EffectHandle handle );
	void                            DelAllEffect(void);

	bool							SetAnimationEndEventListener(_OnAnimationEnd func, unsigned long info);
	bool                            SetSkillHitTimeListener(_OnSkillHitTime func, unsigned long info);
	bool                            SetSkillBreakTimeListener(_OnSkillBreakTime func, unsigned long info);
	bool                            SetShakeTimeListener(_OnShakeTime func, unsigned long info);
	//public edit functions
	bool							UpdateObjAttribute(dword hints);//update object attribute and clear the attribute's dirty state
	long 							GetObjAttributeCount();
	bool 							GetObjAttribute(const DString& attribname, DString& attribvalue, dword& hints);
	bool 							GetObjAttribute(long idx, DString& attribname, DString& attribvalue, dword& hints);
	bool 							AddObjAttribute(const DString& name, const DString& value, const DString& hints);
	bool 							SetObjAttribute(const DString& name, const DString& value, const DString& hints);
	bool 							DelObjAttribute(const DString& name);
	DString							Hint2String(dword hints);

	long 							GetObjEntityCount();
	bool 							GetObjEntity(long idx, DString& name, DString& objfile, DString& material, DString& condition);
	bool 							SetObjEntity(long idx, const DString& material, const DString& condition);
	bool 							SetObjEntity(const DString& name, const DString& material, const DString& condition);

	bool 							GetObjAllAnimName(DStringList& anl);
	DStringList&					GetObjAllAnimName();

	long 							GetObjLocatorCount();
	bool 							GetObjLocatorName(long idx, DString& locname);
	long 							GetObjLocatorSlotCount(long idx);
	bool 							GetObjLocatorSlot(long idx, long slotidx, DString& slotname, DString& objfile, DStringList& attribs);
	DString&						GetObjLocatorSlotName(long idx,long slotidx);
	DString&						GetObjLocatorSlotObjectFile(long idx,long slotidx);
	DStringList&					GetObjLocatorSlotAttribs(long idx,long slotidx);

	bool                            IsLocatorExist(const DString &locName);

	bool 							AddObjLocatorSlot(long idx, const DString& slotname, const DString& objfile, const DStringList& attribs);
	bool 							SetObjLocatorSlot(long idx, const DString& slotname, const DString& objfile, const DStringList& attribs);
	bool 							DelObjLocatorSlot(long idx, const DString& slotname);

	bool 							GetObjLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx);

	bool 							GetLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx);

	static void						SetSceneMgr(Ogre::SceneManager* pSceneMgr);
	static Ogre::SceneManager*		GetSceneMgr();
	
	Ogre::SceneNode*				getSceneNode(void) const {	return m_pObjNode;}

    const Ogre::AxisAlignedBox&     getBoundingBox(void) const;

	/// ����Ĭ�϶�������ѭ�������������еĶ������ö���Ϊѭ�����ţ�
	void SetDefaultAnimation(const DString& animName)
	{
		m_DefaultAnimName = animName;
	}

	void SetAnimationLoop(bool loop);
	
	bool GetAnimationLoop(void);

	static void SetPlaySoundFuncton(OnPlaySound func)
	{
		m_fOnPlaySound = func;
	}

	static void SetStopSoundFunction(OnStopSound func)
	{
		m_fOnStopSound = func;
	}

	static OnPlaySound GetPlaySoundFuncton(void)
	{
		return m_fOnPlaySound;
	}

	static OnStopSound GetStopSoundFunction(void)
	{
		return m_fOnStopSound;
	}

	const DString & getCurrentActionName(void)
	{
		return m_CurrActionName;
	}

	// ��ָ����λ�ùҽ�����DObject��ע�⣬��������ڱ��ҽ�֮ǰ����Ҫ�ȴ�������
	void AttachObjects( AttachInfos &infos );
	// ��ָ����DObject�ӵ�ǰDObject�Ͻӹң�������ҵ�֮ǰ�ĸ��ڵ���
	void DetachObjects( AttachInfos &infos );

	// ������ݵ�������ת�ĽǶ�
	void CalZAxisRotation(float deltaX, float deltaZ, float deltaHeight);
	void CalXAxisRotation(float deltaX, float deltaZ, float deltaHeight);

	ObjectMovementType GetObjectMovementType(void)
	{
		return mObjectMovementType;
	}

	bool							_GetLocatorWorldTransform(const DString& name, Fairy::TransformInfo &info, const Ogre::Vector3 &offsetPos = Ogre::Vector3::ZERO,
		const Ogre::Quaternion &offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );
	bool 							_GetBoneWorldTransform(const DString& name, Fairy::TransformInfo &info, const Ogre::Vector3 &offsetPos = Ogre::Vector3::ZERO,
		const Ogre::Quaternion &offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );
	//{=====================================================================================================
	//add by zhuwenchao 2010.1.5
	void UpdateAnimationList(DString meshname);					// ����ָ��DObject�Ķ����б�
	void UpdateLinkSkeAnimationList(DStringList inifile);		// ���¹�������Ķ����б�
	DStringList& GetObjAllAnimNameEx();							// ��ȡ���������б�(���й�������������)
	void UpdateSkelBoneMap(DString Skelname);					// ���¹������ͷ������
	//=====================================================================================================}


protected://type define
	//Attributes
	
	
	struct	_AttribValue
	{
		DString				Value;
		dword				Hints;
		bool				IsChanged;
	};
	typedef		std::map<DString, _AttribValue>		_AttribMap;


	//EntityList
	struct	_EntityInfo
	{
		DString				Name;
		DString				File;
		DString				Material;
		DString				Condition;
		Ogre::Entity*		pEntity;
	};
	typedef		std::vector<_EntityInfo>		_EntityList;
	
	//Locators
	struct	_Slot
	{
		DString				Name;
		DString				ObjFile;
		DStringList			InitAttribs;
		DObject_NT*			pObject;
	};
	typedef std::vector<_Slot> _SlotList;
	
	
	struct	_Locator
	{
        _Locator()
            : OffRot(Ogre::Quaternion::IDENTITY)
        {
        }

		DString				Name;
		DString				BoneName;
		Ogre::Vector3		OffPos;
		Ogre::Quaternion	OffRot;
		Ogre::SceneNode*	pLocatorNode;
		_SlotList			SlotList;
	};
	typedef		std::vector<_Locator>	_LocatorList;	

protected://attributes
	static Ogre::SceneManager*		s_pSceneMgr;

    Ogre::Any                       mUserAny;

	DString							m_GroupName;
	DString							m_ObjFile;
	DString							m_ObjName;

	Ogre::SceneNode*				m_pObjNode;
	Ogre::Entity*					m_pObjFirstSkeletonEntity;

	Ogre::SkeletonInstance*			m_pObjSkeleton;
	Ogre::SkeletonInstance*			m_pObjParentSkeleton;

	Ogre::AnimationState*			m_pAnimationState;
    Ogre::AnimationState*			m_fadeOutAnimationState;
    Ogre::Real		                m_fadeOutDelay;
	
	DString							m_CurrActionName;

	_AttribMap						m_AttribMap;
	_EntityList						m_EntityList;
	_LocatorList					m_LocatorList;

	//���������¼��ص�����
	_OnAnimationEnd					m_fOnAnimationEnd;

	_OnSkillBreakTime               m_fOnSkillBreakTime;
	_OnSkillHitTime                 m_fOnSkillHitTime;
	_OnShakeTime                    m_fOnShakeTime;

	unsigned long					m_ReserveInfo;

	//�Ƿ���Object
	bool							m_isChildObject;

	Ogre::Entity*					m_pAttachedVisibleEntity;

    //����BoundingBox
    mutable Ogre::AxisAlignedBox    m_FullBoundingBox;
    
    //�Ƿ����BoundingBox
    mutable bool                    m_isPrepareBoundBox;

	//ȫ�ֵ�Animation play rate
	float							m_fGlobalAnimationRate;

	/// Ĭ�϶���������
	DString                       m_DefaultAnimName;

	bool                           m_bAnimationLoop;

	// �����ĺ���ָ��
	static OnPlaySound                   m_fOnPlaySound;
	static OnStopSound                   m_fOnStopSound;

	ObjEntity mObjEntities[EntityCount];

	unsigned int mEntityNameCount;
	unsigned int mEffectHandleCount;

	// ��dobject�����ͣ���ͬ�������ڸ���ʱ���в�ͬ����Ϊ
	ObjectMovementType mObjectMovementType;

	AttachedObjectOriginSceneNodes mAttachedObjectOriginSceneNodes;
	
	// ����ϵͳָ�룬����ȡ��TerrainData��ָ��
	Fairy::System *mSystem;	

	// ǰ���ŵص�������Orientation
	Ogre::Quaternion mFrontBackOrientation;
	// �����ŵص�������Orientation
	Ogre::Quaternion mLeftRightOrientation;
public:
	//{=====================================================================================================
	//add by zhuwenchao 2010.1.5
	DStringList	m_SkeFileNameVector;													//�����ļ������б�

	typedef DStringList AnimationListIterator;						
	AnimationListIterator m_AniList;													//�������б�(һ��ʵ������ж���)

	typedef std::map<DString,AnimationListIterator> AnimationSkeletonLinksMap;
	AnimationSkeletonLinksMap m_LinkMap;												//�����붯��������

	typedef DStringList BoneListIterator;												//��ͷ�ļ������б�	
	BoneListIterator m_BoneList;

	typedef std::map<DString,BoneListIterator> SkelBoneLinks;							//�������ͷ������
	SkelBoneLinks SkelBoneLinksmap;
	// Ĭ�ϵĹ�������
	DString m_DefaultSkeletonName;

	//=====================================================================================================}

protected://functions
	bool 							_EvalCondition(const DString& condition);
	bool 							_EvalAction(const DString& action);
	
	//������صĲ�������
	bool 							_AddAttrib(const DString& name, const DString& value, const DString& hints);
	bool 							_SetAttrib(const DString& name, const DString& value, const DString& hints);
	bool 							_GetAttrib(const DString& name, DString& value, dword& hints, bool& ischanged);
	bool 							_DelAttrib(const DString& name);
	bool 							_SetAttribValue(const DString& name, const DString& value, dword* hints = NULL);
	long 							_GetAttribCount();
	bool 							_GetAttrib(long idx, DString& name, DString& value, dword &hints, bool& ischanged);
	bool 							_ClearAttribState();
	static DString					_Hint2Str(dword hints);
	static dword					_Str2Hint(const DString& hints);

	//ʵ����صĲ�������
	bool 							_AddEntity(const DString& name, const DString& entfile, const DString& mat, const DString& condition);
	bool 							_LoadEntity();
	Ogre::Entity*					_FindVisibleEntity();
	bool							_LoadChildEntity();

	bool 							_SetEntity(const DString& name, const DString& mat, const DString& condition);
	bool 							_UpdateEntitys();
	void							_ReAttachChildObjects();
		

	bool 							_DestroyEntitys();		////ֻ��ɾ�������ʱ�����
	long 							_GetEntityCount();
	bool 							_GetEntity(long idx, DString& name, DString& entfile, DString& mat, DString& condition);
	bool 							_SetEntity(long idx, const DString& mat, const DString& condition);

	void							_SetEntityMat(Ogre::Entity* pEntity,const DString& mat);

	//�󶨵���Ϣ��صĲ�������
	bool 							_AddLocator(const DString& name, const DString& bonename, const Ogre::Vector3& offpos, const Ogre::Quaternion& offrot);
	bool							_AddChildLocator(const DString& name, const DString& bonename, const Ogre::Vector3& offpos, const Ogre::Quaternion& offrot,Ogre::SkeletonInstance* pParentSkeleton,Ogre::Bone* ParentBone);
	bool 							_GetLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx);
	bool 							_AddSlot(const DString& locatorname, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_SetSlot(const DString& locatorname, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_DelSlot(const DString& locatorname, const DString& slotname);
	bool 							_UpdateLocators();		//���������Ըı�ʱ����locator list

	bool							_UpdateLocatorPos();
	bool							_UpdateLocatorPos(const DString& locatorname,const Ogre::Vector3& position,const Ogre::Quaternion& quaternion);	//add by zhuwenchao 2010.1.12 ����һ�����¹ҵ�λ�õĺ���

	bool							_AttachEntitysToBone(Ogre::Bone* pBone);
	
	bool 							_DestroyLocators();	//ֻ��ɾ�������ʱ�����
	long 							_GetLocatorCount();
	bool 							_GetLocatorName(long idx, DString& locname);
	long 							_GetSlotCount(long idx);
	bool 							_GetSlot(long idx, long slotidx, DString& slotname, DString& objfile, DStringList& attribs);
	bool 							_AddSlot(long idx, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_SetSlot(long idx, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_DelSlot(long idx, const DString& slotname);

	Ogre::Entity * _LoadEntity( const DString& meshName );
	void _SetEntity( const DString& name, const DString& value );

	void _DeleteEntities(void);

	void _MuteCurrentSkillSound(void);

	// ���ø�Object����������
	void _SetObjectMovementType( const DString& typeStr );

	// ������Ҫ��ת����
	void _Rotate(void);

	// ���¹ҽ���ȥ��fairy effect
	void _updateFairyEffects(void);
};

#endif