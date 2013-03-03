
/**	ObjectEntity.h
 */

#ifndef _OBJECT_ENTITY_H
#define _OBJECT_ENTITY_H


#include "GIGfxSystem.h"
#include "Core/FairyObject.h"

#include <OgreVector3.h>
#include "EffectSystem/FairySkill.h"



namespace Ogre
{
	class SceneNode;
}

class CObj_ProjTex;

/** ����ʵ������
 */
class CObjectEntity : public tEntityNode
{
public:

	CObjectEntity();
	virtual ~CObjectEntity();


	/** ����Ⱦ�����ٽڵ� */
	virtual VOID Destroy(VOID);

	/** ���ø�������
	 */
	virtual VOID SetData(UINT nData)									{ m_nData = nData; }
	virtual UINT GetData(VOID) const									{ return m_nData; }

	/** ����λ��(��Ϸ��������) */
	virtual VOID SetPosition(const fVector3& vPos)						{}
	/** ������ת�Ƕ� */
	virtual VOID SetOrientation(const fVector3& vRotate)				{}
	//����λ�� (��Ⱦϵͳ����)
	virtual VOID	SetGfxPosition(const fVector3& vPos) {}
	
	/** ֱ������Fairy����
	 */
	virtual VOID SetFairyProperty(const char* szPropertyName, const char* szPropertyValue);

	/** �����Ƿ�ɼ� */
	virtual VOID SetVisible(BOOL bVisible)								{}

	/** �����Ƿ��ܱ����ѡ��
	 */
	virtual VOID SetRayQuery(BOOL bQuery)								{ m_bRayQuery = bQuery; }
	virtual BOOL GetRayQuery(VOID) const								{ return m_bRayQuery; }

	/** ���ѡ�����ȼ�
	 */
	virtual VOID RayQuery_SetLevel(RAYQUERY_LEVEL nLevel)				{ m_nRayQueryLevel = nLevel; }
	virtual RAYQUERY_LEVEL RayQuery_GetLevel(VOID) const				{ return m_nRayQueryLevel; }

	/** ����һ����Ч
	 */
	virtual VOID Attach_Effect(LPCTSTR szEffectName)					{}
	virtual VOID Detach_Effect(void)									{}

	virtual UINT AddEffect(LPCSTR effectName, LPCSTR locatorName)		{ return NULL; }
	virtual VOID DelEffect( UINT effect ){}
	virtual VOID DelAllEffect( VOID ){}
	virtual VOID	AddAreaEffect(LPCSTR templateName) {}
	virtual bool	RemoveAreaEffect(LPCSTR templateName) { return false; }


	virtual VOID SetSkillBulletData(VOID* pData){}

	/** ����������Ч
	 */
	virtual VOID SetLeftWeaponEffect (LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color) {}
	virtual VOID SetRightWeaponEffect(LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color) {}

	virtual VOID ClearWeaponEffect ( eWEAPATTR eWeaponAttr ) {};


	/** �ı������Ķ���
	*	
	*	szWeaponAttrType			�󶨵����ͣ�Left�� right�� Shield��
	*	lpszActionName				������
	*	bLoop									�����Ƿ�ѭ������
	*	fFuseParam						�����ں�ʱ��
	*/
	virtual VOID ChangeWeaponAction( eWEAPATTR szWeaponAttrType, LPCSTR lpszActionName, BOOL bLoop, FLOAT fFuseParam )	{};
	virtual VOID	ChangeWeaponActionSpeed( eWEAPATTR szWeaponAttrType, FLOAT fRate ) {};


	/** ����һ��Ͷ������ */
	virtual VOID Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f)		{}
	virtual VOID Attach_ProjTexture(const char* effectName, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight) {}

	/** ����MeshFile */
	virtual VOID StaObj_SetMeshFile(LPCTSTR szMeshFile)						{}
	/** ���ý�ɫģ����Դ�� */
	virtual VOID Actor_SetFile(LPCTSTR szActorFile)							{}
	/** ���ý�ɫģ�͵����� */
	virtual VOID Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue)	{}
	/** �õ���ɫģ�͵����� */
	virtual VOID Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue)	{}
	/** ȡ������ģ���ϵ�ĳ�ؼ���λ�� */
	virtual VOID Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition)		{}

	/** ȡ��"ͷ��״̬��"����Ļ�ϵ�λ��,�������FALSE����ʾ����Ļ֮��,����û�иõ� */
	virtual BOOL Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1)		{ return FALSE; }
	
	/** ������ʽ
	 *	bAnim		- TRUE(���ö���)/FALSE(������ʽ)
	 *	szSkillName	- ������������ʽ��
	 *	bLoop		- �Ƿ�ѭ��
	 *	fFuseParam	- ���������ںϲ���
	 */
	virtual BOOL Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam=0.3, INT nFlag = 3, INT nObjType = 0, INT nLogicTag = 0){return FALSE;}
	virtual VOID	Actor_PlayAnimation(LPCTSTR szAnimationName, FLOAT fStartTime=0.0f, FLOAT fWeight=1.0f) {};
	virtual VOID Actor_SetHairColor(UINT HairColor)													{}
	virtual Fairy::Skill* GetCurrentSkill(){return NULL;}
	virtual VOID StopCurrentAction(){}
	
	/**	�л������Ĳ����ٶ�
	 *	fRate		- ���ű���
	 */
	virtual VOID Actor_ChangeActionRate(FLOAT fRate)									{}
	/** ����ȱʡ���� */
	virtual VOID Actor_SetDefaultAnim(LPCTSTR szAnimName)								{}
	
	/** ����VisibleFlag */
	virtual VOID Actor_SetUIVisibleFlag(VOID)											{}
	
	/** ����͸���� */
	virtual VOID Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime)					{}
	
	/** ����������Ч��Ŀ��� */
	virtual VOID Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition)	{}
	
	/** �����Ƿ�������Hover״̬ */
	virtual VOID Actor_SetMouseHover(BOOL bHover)										{} 

		virtual VOID	Actor_ChangeActionLoop(bool bLoop){}


protected:

	/// 
	UINT				m_nData;
	/// ���ѡ�����ȼ�,��Խ��Խ����ѡ��
	RAYQUERY_LEVEL		m_nRayQueryLevel;


	/// ����FairyObject
	Fairy::ObjectPtr	m_pFairyObj;
	/// �Ƿ��ܱ����ѡ��
	BOOL				m_bRayQuery;

};	// class


#endif	// _OBJECT_ENTITY_H