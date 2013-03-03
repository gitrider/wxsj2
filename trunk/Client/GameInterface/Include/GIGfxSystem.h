
/** ��Ⱦ���ӿ�
 */

#pragma once


#include "GINode.h"
#include "GIMath.h"


namespace Fairy
{
	class Skill;
}


namespace Fairy
{
	struct TransformInfo;
	typedef std::vector<TransformInfo> TransformInfos;
}

typedef bool (*FUNC_ONANIMATIONEND)(LPCTSTR, DWORD);
typedef bool (*FUNC_ONANIMATIONENDEX)(LPCTSTR, DWORD, INT);
typedef bool (*FUNC_ONSKILLBREAKTIME)(LPCTSTR, DWORD, WORD, WORD);
typedef bool (*FUNC_ONSKILLHITTIME)(LPCTSTR, DWORD, WORD, WORD);



/** ��Ⱦ��ʵ��
 */
class tEntityNode
{
public:

	enum EntityType
	{
		ETYPE_DUMMY,	/// ������,ֻ��SceneNode��������Ⱦ,���Ը�����Ч����ͶӰ����
		ETYPE_MESH,		/// ��̬����(.mesh)
		ETYPE_MODEL,	/// ��̬ģ��(.model)
		ETYPE_ACTOR,	/// ��ɫ(DObject)
						//...
	};

	/// �����󶨵�����
	enum eWEAPATTR
	{
		WEAP_NONE,		// ��
		WEAP_RIGHT,		// ����
		WEAP_LEFT,		// ����
		WEAP_LEFT_SHIELD,	// �����
	};


public:

	//-------------------------------------------------------
	//�õ�����
	virtual EntityType	GetType(VOID) const		= 0;
	//����Ⱦ�����ٽڵ�
	virtual VOID		Destroy(VOID)			= 0;
	
	//���ø�������
	virtual VOID	SetData(UINT dwData)		= 0;
	virtual UINT	GetData(VOID) const			= 0;

	//-------------------------------------------------------
	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos)		= 0;
	//������ת�Ƕ�
	virtual VOID	SetOrientation(const fVector3& vRotate)	= 0;
	virtual VOID SetGfxPosition(const fVector3& vPos) =0;
	//ֱ������Fairy����
	virtual VOID	SetFairyProperty(const char* szPropertyName, const char* szPropertyValue) = 0;

	virtual VOID	SetLeftWeaponEffect (LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color) = 0;
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color) = 0;

	virtual VOID ClearWeaponEffect ( eWEAPATTR eWeaponAttr ) = 0;


	/** �ı������Ķ���
	*	
	*	szWeaponAttrType			�󶨵����ͣ�Left�� right�� Shield��
	*	lpszActionName				������
	*	bLoop									�����Ƿ�ѭ������
	*	fFuseParam						�����ں�ʱ��
	*/
	virtual VOID	ChangeWeaponAction( eWEAPATTR szWeaponAttrType, LPCSTR lpszActionName, BOOL bLoop, FLOAT fFuseParam )	= 0;
	virtual VOID	ChangeWeaponActionSpeed( eWEAPATTR szWeaponAttrType, FLOAT fRate )	= 0;

	//�����Ƿ�ɼ�
	virtual VOID	SetVisible(BOOL bVisible)				= 0;

	//�����Ƿ��ܱ����ѡ��
	virtual VOID	SetRayQuery(BOOL bQuery)				= 0;
	virtual BOOL	GetRayQuery(VOID) const					= 0;


	/// �������ѡ�����ȼ�������Խ�����ȼ�Խ��
	enum RAYQUERY_LEVEL
	{
		RL_UNKNOWN		= 0,

		RL_ITEMBOX				= 1,		/// �����			1
		RL_PLAYER_DEADBODY		= 2,		/// ���ʬ��		2
		RL_CREATURE				= 3,		/// NPC & Monster	3
		RL_TRIPPEROBJ			= 4,		/// ��Դ			4
		RL_PLAYEROTHER			= 5,		/// ���			5
		RL_PLAYERMYSLEF			= 6,		/// �Լ�			6
		RL_PET					= 7,		/// ����			7
		RL_CREATURE_DEADBODY	= 8,		/// ����ʬ��		8
		RL_PLATFORM				= 9,		/// ����ƽ̨		9
	};

	virtual VOID			RayQuery_SetLevel(RAYQUERY_LEVEL nLevel)	= 0;
	virtual RAYQUERY_LEVEL	RayQuery_GetLevel(VOID) const				= 0;

	//-------------------------------------------------------
	//����һ����Ч
	virtual VOID	Attach_Effect(LPCTSTR szEffectName)		= 0;
	virtual VOID	Detach_Effect(void)						= 0;

	// �°渽��һ����Ч
	virtual UINT			AddEffect( LPCSTR effectName, LPCSTR locatorName )	= 0;
	virtual VOID			DelEffect( UINT effect )							= 0;
	virtual VOID			DelAllEffect( VOID )								= 0;
	virtual void			AddAreaEffect(LPCSTR templateName) =0;
	virtual bool			RemoveAreaEffect(LPCSTR templateName) =0;
	//����һ��Ͷ������
	enum PROJTEX_TYPE
	{
		SELECT_RING,		// ѡ��
		SHADOW_BLOB,		// ��Ӱ��
		REACHABLE,			// ���Ե����Ŀ�ĵ�
		UNREACHABLE,		// �޷������Ŀ�ĵ�
		AURA_RUNE,			// �������÷�Χ��
		BLOOD_STAIN_1,		// Ѫ�� 1
		BLOOD_STAIN_2,		// Ѫ�� 2
		BLOOD_STAIN_3,		// Ѫ�� 3
		BLOOD_STAIN_4,		// Ѫ�� 4
							//...

		SELF_DEFINE_EFFECT,	//�Զ������������
	};
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f) = 0;
	virtual VOID Attach_ProjTexture(const char* effectName, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight) =0;
	virtual void	Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator)	{	}
	virtual void	Detach_Object(tEntityNode *pObject)	{	}

	//-------------------------------------------------------
	//��̬������
	//����MeshFile
	virtual VOID	StaObj_SetMeshFile(LPCTSTR szMeshFile)	= 0;

	//-------------------------------------------------------
	//��ɫ��
	//���ý�ɫģ����Դ��
	virtual VOID	Actor_SetFile(LPCTSTR szActorFile)	= 0;
	//���ý�ɫģ�͵�����
	virtual VOID	Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue) = 0;
	//�õ���ɫģ�͵�����
	virtual VOID	Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue) = 0;
	//ȡ������ģ���ϵ�ĳ�ؼ���λ��
	virtual VOID	Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition) = 0;
	//ȡ��"ͷ��״̬��"����Ļ�ϵ�λ��,�������FALSE����ʾ����Ļ֮��,����û�иõ�
	virtual BOOL	Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1) = 0;
	//������ʽ
	// bAnim		- TRUE(���ö���)/FALSE(������ʽ)
	// szSkillName	- ������������ʽ��
	// bLoop		- �Ƿ�ѭ��
	// fFuseParam	- ���������ںϲ��� 
	virtual BOOL	Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam=0.3, INT nFlag = 3, INT nObjType = 0, INT nLogicTag = 0) {return FALSE;}
	virtual VOID	Actor_SetHairColor(UINT HairColor) = 0;
	virtual Fairy::Skill* GetCurrentSkill(){return NULL;}
	virtual VOID StopCurrentAction(){}
	virtual VOID Actor_ChangeActionLoop(bool bLoop) =0;
	// �л������Ĳ����ٶ�
	// fRate		- ���ű���
	virtual VOID	Actor_ChangeActionRate(FLOAT fRate) = 0;
	//����������ͬʱ������������
	virtual VOID	Actor_PlayAnimation(LPCTSTR szAnimationName, FLOAT fStartTime=0.0f, FLOAT fWeight=1.0f) = 0;
	//����ȱʡ����
	virtual VOID	Actor_SetDefaultAnim(LPCTSTR szAnimName) = 0;
	//����UI VisibleFlag
	virtual VOID	Actor_SetUIVisibleFlag(VOID) = 0;
	//����͸����
	virtual VOID	Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime) = 0;
	// ����������Ч��Ŀ���
	virtual VOID	Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition) = 0;
	//�����Ƿ�������Hover״̬
	virtual VOID	Actor_SetMouseHover(BOOL bHover) = 0;

	//���ý�ɫ��������֪ͨ
	virtual void	SetAnimationEndEvent(FUNC_ONANIMATIONENDEX pFunc, DWORD dwParam, INT)	{	}
	//���ý�ɫ�������Խ���֪ͨ
	virtual void	SetAnimationCanBreakEvent(FUNC_ONSKILLBREAKTIME pFunc, DWORD dwParam)	{	}
	//���ý�ɫ���������֪ͨ
	virtual void	SetAnimationHitEvent(FUNC_ONSKILLHITTIME pFunc, DWORD dwParam)	{	}
	//���ý�ɫ���������֪ͨ
	virtual void	SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)	{	}

public:

	virtual int		GetTerrainSoundType()	{ return -1; };
	virtual void	SetScaleFactor(float, float, float)	{ }

};


/** ͼ�����ϵͳ
 */
class KYLIN_API tGfxSystem : public tNode
{
public:

	//----------------------------------------------------------------
	//	ϵͳ��غ���

	/// ��ʼ����Ⱦ��
	virtual VOID		Initial(VOID*)		= 0;
	/// ������Ⱦ��
	virtual VOID		Release(VOID)		= 0;
	/// ��Ⱦ����
	virtual VOID		RenderFrame(VOID)	= 0;
	/// ��ȾLoading����
	virtual VOID		RenderLodingFrame(LPCTSTR szLoding) = 0;
	/// ��ӦWM_PAINT��Ϣ
	virtual VOID		OnPaint(VOID)		= 0;
	/// ���ڴ�С�����ı�
	virtual VOID		OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)	= 0;
	/// ȡ����Ⱦ����
	virtual HWND		GetWindow(VOID)	const				= 0;
	//����ǰ��Ļ��ͼ���ļ���
	virtual BOOL		PrintScreen(LPTSTR szReturnBuf, INT nBufSize) = 0;

	//--- for debug
	/// ����Deubg��Ϣ
	virtual VOID		PushDebugString(LPCTSTR szMsg, ...)	= 0;
	/// ����Debug״̬��Ϣ
	virtual VOID		SetDebugString(LPCTSTR szName, LPCTSTR szValue) = 0;
	/// ����ѡ��������Ϣ
	virtual	VOID		SetHitObjText(LPCTSTR szHitObjText)	= 0;
	//--- for debug

	virtual void CheckObjectTransparent() = 0;

	virtual void SetShowObjectByType(LPCTSTR szName) =0;

public:

	//----------------------------------------------------------------
	//	����ת�����
	enum AXIS_TYPE 
	{
		AX_GAME,		//��Ϸ�������꣬����Ϊ��λ
		AX_PLAN,		//��Ϸ�������꣬����Ϊ��λ,����������Y����
		AX_GFX,			//��Ⱦϵͳ�ж������꣬һ��������Ϊ��λ
		AX_SCREEN,		//��Ļ�ϵ��������꣬�������λ��,����
	};

	// ��һ������ת��Ϊ��һ������,ת���ɳɹ�����TRUE
	// ��Դ����Ƿ���ʱ�򣬷���FALSE
	virtual BOOL		Axis_Trans(	AXIS_TYPE typeSource, const fVector3& fvSource,
									AXIS_TYPE typeTar,			fVector3& fvTarget) = 0;

public:

	//----------------------------------------------------------------
	//	�������غ���
	enum CAMERA_TYPE
	{
		CHAR_VIEW,
		SCENE
	};
	//���õ�ǰ�����
	virtual VOID		Camera_SetCurrent(CAMERA_TYPE theType)	= 0;

	enum OFFSET_TAG
	{
		OT_LEFT = 0,
		OT_RIGHT,

		OT_NONE,
	};
	/// ����ƫ����
	virtual VOID		Camera_SetOffset( OFFSET_TAG eOT, FLOAT fParallelOffset, FLOAT fVerticalOffset = 0.0f ) = 0;

	/// �ƶ����������λ�õ�ĳһ��,һ����������ڵ�λ��(�����ά����ϵ)
	virtual VOID		Camera_SetLookAt(const fVector3& fvPos) = 0;
	
	/// ��������ӿ� [0.0f, 1.0f]  0-������� 1-��Զ����
	virtual VOID		Camera_SetZoom(FLOAT fZoom)				= 0;
	virtual VOID		Camera_SetZoom(BOOL bUp,INT multiple)	= 0;
	virtual FLOAT		Camera_GetZoom(VOID) const				= 0;

	/// ����������� [0, 2��)�� ��z��ָ��ķ���Ϊ0�ȣ���ʱ��Ϊ������
	virtual VOID		Camera_Direction(FLOAT fDir)			= 0;
	virtual VOID		Camera_AddDirection(FLOAT fMultiple)	= 0;
	virtual FLOAT		Camera_GetDirection(VOID) const			= 0;

	// ȡ����Ļ����
	virtual VOID		Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay) = 0;

	// ��һ��
	virtual VOID		Camera_VibrateOneTime() = 0;
	virtual VOID		Camera_VibrateOneTime(FLOAT VibRange, FLOAT StepLen, UINT Times) = 0;

	//-----------------------------------------------------------------------
	//
	// ����ѡ��, ����, �����õ���������ӿ�.
	//

	// �����۾���λ��
	virtual VOID		Camera_SetEyePos(const fVector3& fvPos) = 0;
	// ���ù۲���λ��
	virtual VOID		Camera_SetLookAtPos(const fVector3& fvPos) = 0;


public:

	//----------------------------------------------------------------
	//Ԥ���س���
	virtual VOID			Scene_PreLoad(const STRING& theScenePreload) = 0;
	//���س���
	virtual VOID			Scene_Load(INT nID, const STRING& theSceneDefine)	= 0;
	//ж�س���
	virtual	VOID			Scene_UnLoad(void) =0;
	//��ʾ��������
//	virtual VOID			Scene_ShowGridSwitch(INT nZoneSize)			= 0;
	//��ʾ����ȫ����ȾЧ��
	virtual VOID			Scene_SetPostFilter(LPCTSTR szFilterName, BOOL state = TRUE)	= 0;
	//����LightMap
	virtual VOID			Scene_SetLightmapQuality(INT nQuality)		= 0;
	//����/�˳�����ȫ����ȾЧ��
	virtual VOID			Scene_SetPostFilter_Death(BOOL bEnter)		= 0;
	//���û�����Ч(λ��)
	virtual VOID			Scene_SetEnviromentInfo(const fVector3& fvPos)	= 0;
	//���ò���32λ/16λת��
	virtual VOID			Scene_Enable32BitTextures(BOOL bSetting=TRUE)	= 0;
	//���ò��������Ǹ�������3������������2������˫����1(Զ��������)��
	virtual VOID			Scene_SetTextureFiltering(INT nOptions)			= 0;
	//���þ�̬���嶯��
	virtual VOID			Scene_SetAnimationQuality(INT nOptions)			= 0;
	//����ȫ������Ч��
	virtual VOID			Scene_SetPostFilterEnabled(BOOL bSetting)		= 0;
	//��������ʵʱ��Ӱ
	virtual VOID			Scene_SetShadowTechnique(BOOL bSetting)			= 0;
	//��ʾ���������л�
	virtual VOID			Scene_ShowGridSwitch(INT nZoneSize) =0;

	//--- for debug
	//��ĳ������һ������
	virtual VOID			Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos) = 0;
	//
	virtual VOID			Debug_SetFakeCamera(int nType, float fValue) = 0;
	//��ʾ/����DebugPanel
	virtual VOID			Debug_ToggleShowDebugPanel(VOID) = 0;
	virtual VOID			Debug_ToggleShowBoundingBox(VOID) = 0;

	//--- for debug

	//----------------------------------------------------------------
	//	��������Ⱦ�������
public:

	/// ����һ������Ⱦʵ��  ETYPE_*
	virtual tEntityNode*	New_EntityObject(tEntityNode::EntityType type)	= 0;
	///Ѱ�Һ����Ӵ���ʵ��
	virtual tEntityNode*	Find_HitFairyObject(INT nX, INT nY)				= 0;

	//----------------------------------------------------------------
	//UI��ʾģ�����
public:

	//����һ��Fake��Ⱦ����
	virtual VOID			FakeObj_Create(LPCTSTR  szName, tEntityNode* pNode, tEntityNode* pParentNode, LPCTSTR szCameraName, 
		int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName) = 0;
	//����һ��Fake��Ⱦ����
	virtual VOID			FakeObj_Destroy(LPCTSTR szObjName) = 0;
	//����/��ʾFake��Ⱦ����
	virtual VOID			FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio) = 0;
	virtual bool FakeObj_SetAttach(tEntityNode* pNode, tEntityNode* pAttachNode) =0;

	KL_DECLARE_DYNAMIC(tGfxSystem);

};