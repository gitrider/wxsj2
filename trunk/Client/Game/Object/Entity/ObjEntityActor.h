
/**	ObjectEntityActor.h
 */

#ifndef _OBJECT_ENTITY_ACTOR_H
#define _OBJECT_ENTITY_ACTOR_H


#include "ObjectEntity.h"



namespace Fairy
{
    class LogicModel;
};

class DObject_NT;
class CObj_ProjTex;

/** ��ɫʵ��
 */
class CObjEntityActor : public CObjectEntity
{
public:

	CObjEntityActor();
	virtual ~CObjEntityActor();


	/** �õ����� */
	virtual EntityType GetType(VOID) const					{ return ETYPE_ACTOR; }

	/** ����Ⱦ�����ٽڵ� */
	virtual VOID Destroy(VOID);

	/** �����Ƿ�ɼ� */
	virtual VOID SetVisible(BOOL bVisible);

	/** ����һ��Ͷ������ 
	 */
	virtual VOID Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );
	virtual void Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator);
	virtual void Detach_Object(tEntityNode *pObject);

	/** ���ý�ɫģ����Դ�� */
	virtual VOID Actor_SetFile(LPCTSTR szActorFile);
	/** ���ý�ɫģ�͵����� */
	virtual VOID Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue);
	/** �õ���ɫģ�͵����� */
	virtual VOID Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue);
	
	/** ȡ������ģ���ϵ�ĳ�ؼ���λ�� */
	virtual VOID Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition);
	
	/** ȡ��"ͷ��״̬��"����Ļ�ϵ�λ��,�������FALSE����ʾ����Ļ֮��,����û�иõ� */
	virtual BOOL Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1);
	
	/** ������ʽ
	 *	bAnim		- TRUE(���ö���)/FALSE(������ʽ)
	 *	szSkillName	- ������������ʽ��
	 *	bLoop		- �Ƿ�ѭ��
	 *	fFuseParam	- ���������ںϲ��� 
	 */
	virtual BOOL Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam, INT nFlag, INT nObjType, INT nLogicTag);
	virtual VOID Actor_SetHairColor(UINT HairColor);
	virtual Fairy::Skill* GetCurrentSkill();
	virtual VOID StopCurrentAction();

	virtual VOID Actor_PlayAnimation(LPCTSTR szAnimationName, FLOAT fStartTime, FLOAT fWeight);

	virtual VOID Actor_ChangeActionLoop(bool bLoop);
	
	/** �л������Ĳ����ٶ�
	 *	fRate		- ���ű���
	 */
	virtual VOID Actor_ChangeActionRate(FLOAT fRate);
	
	/** ����VisibleFlag */
	virtual VOID Actor_SetUIVisibleFlag(VOID);
	
	/** ����ȱʡ���� */
	virtual VOID Actor_SetDefaultAnim(LPCTSTR szAnimName);
	
	/** ����͸���� */
	virtual VOID Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime);
	
	/** ����������Ч��Ŀ��� */
	virtual VOID Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition);
	
	/** �����Ƿ�������Hover״̬ */
	virtual VOID Actor_SetMouseHover(BOOL bHover);
	
	/** ���ý�ɫ��������֪ͨ */
	virtual void SetAnimationEndEvent(FUNC_ONANIMATIONENDEX pFunc, DWORD dwParam, INT n);
	/** ���ý�ɫ�������Խ���֪ͨ */
	virtual void SetAnimationCanBreakEvent(FUNC_ONSKILLBREAKTIME pFunc, DWORD dwParam);
	/** ���ý�ɫ���������֪ͨ */
	virtual void SetAnimationHitEvent(FUNC_ONSKILLHITTIME pFunc, DWORD dwParam);
	/** ���ý�ɫ���������֪ͨ */
	virtual void SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	
	/** ����λ�� (��Ϸ��������) */
	virtual VOID SetPosition(const fVector3& vPos);

    virtual VOID SetOrientation(const fVector3& vRotate);
	
	/** ������Ч */
	virtual VOID SetLeftWeaponEffect (LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color);
	virtual VOID SetRightWeaponEffect(LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color);

	virtual VOID ClearWeaponEffect ( eWEAPATTR eWeaponAttr );


	/** �ı������Ķ���
	 *	
	 *	szWeaponAttrType	�󶨵����ͣ�Left�� right�� Shield��
	 *	lpszActionName		������
	 *	bLoop				�����Ƿ�ѭ������
	 *	fFuseParam			�����ں�ʱ��
	 */
	virtual VOID ChangeWeaponAction( eWEAPATTR szWeaponAttrType, LPCSTR lpszActionName, BOOL bLoop, FLOAT fFuseParam );
	virtual VOID ChangeWeaponActionSpeed( eWEAPATTR szWeaponAttrType, FLOAT fRate );


	/** ��Ч��� 
	 */
	virtual UINT AddEffect( LPCSTR effectName, LPCSTR locatorName );
	virtual VOID DelEffect( UINT effect );
	virtual VOID DelAllEffect( VOID );
	virtual VOID	AddAreaEffect(LPCSTR templateName);
	virtual bool	RemoveAreaEffect(LPCSTR templateName);

	Fairy::LogicModel* GetActorImpl( VOID )					{ return mModel; }

	/** ��ȡ�������ͣ��Ա������ͬ������ */
	virtual int GetTerrainSoundType();
	/** �������Ų��� */
	virtual void SetScaleFactor(float, float, float);
	virtual VOID SetGfxPosition(const fVector3& vPos);


protected:

    Fairy::LogicModel*		mModel;
	
	Fairy::LogicModel*		m_leftWeaponModel;
	UINT					m_leftWeaponEffect;

	Fairy::LogicModel*		m_rightWeaponModel;
	UINT					m_rightWeaponEffect;

	BOOL					m_bVisible;

};	// class


#endif	// _OBJECT_ENTITY_ACTOR_H