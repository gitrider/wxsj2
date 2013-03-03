
/** ����ӿ�
 */

#ifndef _ENGINE_INTERFACE_H
#define _ENGINE_INTERFACE_H


#include "GIGfxSystem.h"
#include "GIEventSystem.h"



struct EVENT;

class CCamera;
class CDebugPanel;
class CProcessBar;
class CHelpPanel;
class CFakeObjectEntityManager;
class CUIMapLight;
class TerrainGrid;


namespace Fairy 
{ 
	class System;
	class TerrainGrid;
	class PostFilterManager;
	class EnviromentObject;
};

namespace Ogre
{
	class RaySceneQuery;
};



enum ENUM_OBJ_TYPE
{
	NOONE_TYPE	= -1,	
	STATICENTITY_TYPE =1,						//1
	PARTICLE_TYPE = 1<<1,						//2
	TERRAINLIQUIDOBJ_TYPE= 1<<2,				//4
	EFFECTOBJ_TYPE= 1<<3 ,						//8
	BULLETOBJ_TYPE= 1<<4 ,						//16
	LOGICMODEL_TYPE= 1<<5,						//32 100000
	ALL_TYPE = (1<<6) -1,
};

class CEngineInterface : public tGfxSystem
{
public:

	CEngineInterface(VOID);
	virtual ~CEngineInterface(VOID);

	static CEngineInterface* GetMe(VOID)					{ return s_pMe; }


	/** ��ʼ����Ⱦ�� 
	*/
	virtual VOID Initial(VOID*);

	/** ������Ⱦ�� 
	*/
	virtual VOID Release(VOID);

	/** �߼���ѭ���� 
	*/
	virtual VOID Tick(VOID);


	/** ��Ⱦ���� */
	virtual VOID RenderFrame(VOID);
	/** ��ȾLoading���� */
	virtual VOID RenderLodingFrame(LPCTSTR szLoding);

	/** ��ӦWM_PAINT��Ϣ */
	virtual VOID OnPaint(VOID);
	/** ���ڴ�С�����ı� */
	virtual VOID OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam);
	/** ȡ����Ⱦ���� */
	virtual HWND GetWindow(VOID) const;
	/** ����ǰ��Ļ��ͼ���ļ��� */
	virtual BOOL PrintScreen(LPTSTR szReturnBuf, INT nBufSize);

	/** �����ػ��������ڿ��Ƹ�����ȾЧ���Ŀ��� */
	static VOID	WINAPI _OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);


	/** �õ�Fairy��Ⱦϵͳ */
	Fairy::System* GetFairySystem(VOID)						{ return m_pFairySystem; }
	
	/** ��Ϸ��������Ⱦ����֮��ķ������� */
	fVector3 GetSacle(VOID) const;

	// �������þɵ���ʾ���ã� ȫ����ʾ��
	// ��¼����֮�� ������Ϸ�У� �������������
	// void ResetOldShowSetting();


	/** ����Ƿ�Ҫ͸��
	 *	����Ϸ���ڵ�����Ľ����ȶ���ᱻ͸��
	 */
	virtual void CheckObjectTransparent();

public:

/** ����ת�����
 */

	/** ��һ������ת��Ϊ��һ������,ת���ɳɹ�����TRUE
	 *	��Դ����Ƿ���ʱ�򣬷���FALSE
	 */
	virtual BOOL Axis_Trans(AXIS_TYPE typeSource, const fVector3& fvSource, AXIS_TYPE typeTar, fVector3& fvTarget);

	/** ��������Ƿ�Ϸ� */
	virtual BOOL Axis_CheckValid(AXIS_TYPE typeSource, const fVector3& fvAxis);
	
public:

/** �������غ��� 
 */

	/** 
	 *	���õ�ǰ����� 
	 */
	virtual VOID Camera_SetCurrent(CAMERA_TYPE theType);

	/**
	 *	������UI�仯���µ�������۲��ı䶯
	 */
	virtual VOID Camera_SetOffset( OFFSET_TAG eOT, FLOAT fParallelOffset, FLOAT fVerticalOffset = 0.0f );

	/** 
	 *	�ƶ����������λ�õ�ĳһ��,һ����������ڵ�λ��(������ά����ϵ) 
	 */
	virtual VOID Camera_SetLookAt( const fVector3& fvPos );
	
	/** 
	 *	��������ӿ� [0.0f, 1.0f]  0-������� 1-��Զ����
	 */
	virtual VOID Camera_SetZoom(FLOAT fZoom);
	virtual VOID Camera_SetZoom(BOOL bUp,INT multiple);
	virtual FLOAT Camera_GetZoom(VOID) const;

	/** ����������� [0, 2��)�� ��z��ָ��ķ���Ϊ0�ȣ���ʱ��Ϊ������ 
	 */
	virtual VOID Camera_Direction(FLOAT fDir);
	virtual VOID Camera_AddDirection(FLOAT fMultiple);
	virtual FLOAT Camera_GetDirection(VOID) const;

	/** ȡ����Ļ���� */
	virtual VOID Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay);

	/** ��һ�� */
	virtual VOID Camera_VibrateOneTime();
	/** 
	 *	VibRange	y���������ƫ����
	 *	StepLen		�����ڸ���
	 *	Times		ʱ��
	*/
	virtual VOID Camera_VibrateOneTime(FLOAT fAmplitude, FLOAT fDuration, UINT fFrequency);


/** ����ѡ��, ����, �����õ���������ӿ�.
 */

	/** �����۾���λ�� */
	virtual VOID Camera_SetEyePos(const fVector3& fvPos);
	/** ���ù۲���λ�� */
	virtual VOID Camera_SetLookAtPos(const fVector3& fvPos);

public:

/** ������Ϣ
 */

	/** �������Deubg��Ϣ */
	virtual VOID PushDebugString(LPCTSTR szMsg, ...);
	/** ����Debug״̬��Ϣ */
	virtual VOID SetDebugString(LPCTSTR szName, LPCTSTR szValue);
	/** ����ѡ��������Ϣ */
	virtual	VOID SetHitObjText(LPCTSTR szHitObjText);


	static const CHAR CEngineInterface::sz32BitTexturesVar[];				// ��ɫ������������(16/32λ)
	static const CHAR CEngineInterface::szDefaultTextureFilteringVar[];		// ����(��/��/��)
	static const CHAR CEngineInterface::szLightmapQualityVar[];				// ������Ӱ(��/��/��)
	static const CHAR CEngineInterface::szAnimationQualityVar[];			// ��̬���嶯��(��/��)
	static const CHAR CEngineInterface::szPostFilterEnabledVar[];			// ȫ������Ч��(��/��)
	static const CHAR CEngineInterface::szFSAAVar[];						// �����(1/2/4)
	static const CHAR CEngineInterface::szShadowTechniqueVar[];				// ������Ӱ(��/��)
	static const CHAR CEngineInterface::szvsyncVar[];						// ��ֱͬ��(��/��)

public:

/** ������� 
 */

	/** Ԥ���س��� */
	virtual VOID			Scene_PreLoad(const STRING& theScenePreload);
	/** ���س��� */
	virtual VOID			Scene_Load(INT nID, const STRING& theSceneDefine);
	/**ж�س���*/
	virtual	VOID			Scene_UnLoad(void);
	/** ��ʾ�������� */
//	virtual VOID			Scene_ShowGridSwitch(INT nZoneSize);
	/** ��ʾ����ȫ����ȾЧ�� */
	virtual VOID			Scene_SetPostFilter(LPCTSTR szFilterName, BOOL state = TRUE);
	/** ����/�˳�����ȫ����ȾЧ�� */
	virtual VOID			Scene_SetPostFilter_Death(BOOL bEnter);
	/** ����Lightmap */
	virtual VOID			Scene_SetLightmapQuality(INT nQuality);
	/** ���û�����Ч(λ��) */
	virtual VOID			Scene_SetEnviromentInfo(const fVector3& fvPos);
	/** ���ò���32λ/16λת�� */
	virtual VOID			Scene_Enable32BitTextures(BOOL bSetting=TRUE);
	/** ���ò��������Ǹ�������3������������2������˫����1(Զ��������)�� */
	virtual VOID			Scene_SetTextureFiltering(INT nOptions);
	/** ���þ�̬���嶯�� */
	virtual VOID			Scene_SetAnimationQuality(INT nOptions);
	/** ����ȫ������Ч�� */
	virtual VOID			Scene_SetPostFilterEnabled(BOOL bSetting);
	/** ��������ʵʱ��Ӱ */
	virtual VOID			Scene_SetShadowTechnique(BOOL bSetting);
	/** ��ʾ���������л�*/
	virtual VOID Scene_ShowGridSwitch(INT nZoneSize);


	//-----------------------------------------------------------------------
	/*  �����Ͷ���������ʾ�Ͳ���ʾ ��Ϸ�����������¼��֣�
	LightObject::getFactory());
	StaticEntityObject::getFactory());
	ActorObject::getFactory());
	ModelObject::getFactory());
	ParticleSystemObject::getFactory()
	SkyBoxObject::getFactory()
	SkyDomeObject::getFactory()
	SkyPlaneObject::getFactory()
	EnviromentObject::getFactory()
	TerrainLiquidObject::getFactory()
	EffectObject::getFactory()
	BulletObject::getFactory()
	LogicModelObject::getFactory())
	RegionObject::getFactory()

	NB:  SetShowObjectByType(StaticEntityObject::getFactory()->getType(),OFF);--�رս��������ʾ
	*/



	virtual void SetShowObjectByType( LPCTSTR szName);


	/** 
		��ĳ������һ������ 
	 */
	virtual VOID Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos);
	
	//
	virtual VOID Debug_SetFakeCamera(int nType, float fValue);
	
	/** 
		��ʾ/����DebugPanel 
	 */
	virtual VOID Debug_ToggleShowDebugPanel(VOID);

	virtual VOID Debug_ToggleShowBoundingBox(VOID);
	
public:

/**================ ��������Ⱦ�������
 */

	/** 
		����һ������Ⱦʵ��  ETYPE_* 
	 */
	virtual tEntityNode* New_EntityObject(tEntityNode::EntityType type);

	/** 
		ɾ��һ����Ⱦ�� 
	 */
	VOID Destroy_EntityObject(tEntityNode* pNode);
	
	/** 
	 *	Ѱ�Һ����Ӵ���ʵ��
	 *	nX, nY - ���λ��
	 */
	virtual tEntityNode* Find_HitFairyObject(INT nX, INT nY);

public:

/**================ UI��ʾģ����� 
 */

	/** 
		����һ��Fake��Ⱦ���� 
	 */
	virtual VOID FakeObj_Create(LPCTSTR  szName, tEntityNode* pNode, tEntityNode* pParentNode, LPCTSTR szCameraName, 
		int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName);
	
	/** 
		����һ��Fake��Ⱦ����
	 */
	virtual VOID FakeObj_Destroy(LPCTSTR szObjName);
	
	/** 
		����/��ʾFake��Ⱦ���� 
	 */
	virtual VOID FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio);

	virtual bool FakeObj_SetAttach(tEntityNode* pNode, tEntityNode* pAttachNode);


protected:

	/// ���� Fairy��Ⱦϵͳ
	Fairy::System*				m_pFairySystem;

	/// ��ogre���������һ����װ,ʹ����ÿ���
	CCamera*					m_pCamera_Current;
	/// ����ѡ�����������
	CCamera*					m_pCamera_CharView;
	/// �����˳������ӽ��������
	CCamera*					m_pCamera_Scene;
	/// �����ƫ����
//	FLOAT						m_fParallelOffset;
//	FLOAT						m_fSaveParallelOffset;
//	FLOAT						m_fOldParallelOffset;
	FLOAT						m_fVerticalOffset;

	/// �ͻ������
	FLOAT						m_fFullClientWidth;
	BOOL						m_bFullWidth;
	FLOAT						m_fClientWidth;
	/// ����ͻ��������ߴ��Ӧ����Ϸ��λ��M��
	FLOAT						m_fUnitGameToScene;

	OFFSET_TAG					m_eOffsetTag;
	BOOL						m_bLeftOffset;
	BOOL						m_bRightOffset;

	/// ������ߺͳ�������
	Ogre::RaySceneQuery*		m_pRaySceneQuery;

	/// PostFilterϵͳ(ȫ����Ⱦ��)
	Fairy::PostFilterManager*	m_pPostFilterManager;

	/// ������Ч
	Fairy::EnviromentObject*	m_pEnviromentObject;

	//��������
	Fairy::TerrainGrid* m_pTerrainGrid;

	/// ������Ч
	BOOL						m_bDeathMode;
	INT							m_nDeachModePercent; //[0,100]

	/// ���԰�
	CDebugPanel*				m_pDebugPanel;

	/// FakeManager
	CFakeObjectEntityManager*	m_pFakeObjectManager;

	CUIMapLight* m_pMapLight;
	
protected:
	
	static CEngineInterface*	s_pMe;


	KL_DECLARE_DYNAMIC(CEngineInterface);

};	// class



class StringConvert
{
public:

	//utf<->mbcs
	static VOID		mbcs_to_utf8(const std::string& strIn, std::string& strOut);
	static VOID		utf8_to_mbcs(const std::string& strIn, std::string& strOut);

};



#endif	// _ENGINE_INTERFACE_H