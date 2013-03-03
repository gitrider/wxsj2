
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

/** 角色实体
 */
class CObjEntityActor : public CObjectEntity
{
public:

	CObjEntityActor();
	virtual ~CObjEntityActor();


	/** 得到类型 */
	virtual EntityType GetType(VOID) const					{ return ETYPE_ACTOR; }

	/** 从渲染层销毁节点 */
	virtual VOID Destroy(VOID);

	/** 设置是否可见 */
	virtual VOID SetVisible(BOOL bVisible);

	/** 附加一个投射纹理 
	 */
	virtual VOID Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );
	virtual void Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator);
	virtual void Detach_Object(tEntityNode *pObject);

	/** 设置角色模型资源名 */
	virtual VOID Actor_SetFile(LPCTSTR szActorFile);
	/** 设置角色模型的属性 */
	virtual VOID Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue);
	/** 得到角色模型的属性 */
	virtual VOID Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue);
	
	/** 取得任务模型上的某关键点位置 */
	virtual VOID Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition);
	
	/** 取得"头顶状态点"在屏幕上的位置,如果返回FALSE，表示在屏幕之外,或者没有该点 */
	virtual BOOL Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1);
	
	/** 进入招式
	 *	bAnim		- TRUE(调用动作)/FALSE(调用招式)
	 *	szSkillName	- 动作名或者招式名
	 *	bLoop		- 是否循环
	 *	fFuseParam	- 骨骼动画融合参数 
	 */
	virtual BOOL Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam, INT nFlag, INT nObjType, INT nLogicTag);
	virtual VOID Actor_SetHairColor(UINT HairColor);
	virtual Fairy::Skill* GetCurrentSkill();
	virtual VOID StopCurrentAction();

	virtual VOID Actor_PlayAnimation(LPCTSTR szAnimationName, FLOAT fStartTime, FLOAT fWeight);

	virtual VOID Actor_ChangeActionLoop(bool bLoop);
	
	/** 切换动画的播放速度
	 *	fRate		- 缩放比率
	 */
	virtual VOID Actor_ChangeActionRate(FLOAT fRate);
	
	/** 设置VisibleFlag */
	virtual VOID Actor_SetUIVisibleFlag(VOID);
	
	/** 设置缺省动作 */
	virtual VOID Actor_SetDefaultAnim(LPCTSTR szAnimName);
	
	/** 设置透明度 */
	virtual VOID Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime);
	
	/** 设置连线特效的目标点 */
	virtual VOID Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition);
	
	/** 设置是否出于鼠标Hover状态 */
	virtual VOID Actor_SetMouseHover(BOOL bHover);
	
	/** 设置角色动画结束通知 */
	virtual void SetAnimationEndEvent(FUNC_ONANIMATIONENDEX pFunc, DWORD dwParam, INT n);
	/** 设置角色动画可以结束通知 */
	virtual void SetAnimationCanBreakEvent(FUNC_ONSKILLBREAKTIME pFunc, DWORD dwParam);
	/** 设置角色动画打击点通知 */
	virtual void SetAnimationHitEvent(FUNC_ONSKILLHITTIME pFunc, DWORD dwParam);
	/** 设置角色动画打击点通知 */
	virtual void SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	
	/** 设置位置 (游戏世界坐标) */
	virtual VOID SetPosition(const fVector3& vPos);

    virtual VOID SetOrientation(const fVector3& vRotate);
	
	/** 武器特效 */
	virtual VOID SetLeftWeaponEffect (LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color);
	virtual VOID SetRightWeaponEffect(LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color);

	virtual VOID ClearWeaponEffect ( eWEAPATTR eWeaponAttr );


	/** 改变武器的动作
	 *	
	 *	szWeaponAttrType	绑定点类型（Left， right， Shield）
	 *	lpszActionName		动作名
	 *	bLoop				动作是否循环播放
	 *	fFuseParam			动作融合时间
	 */
	virtual VOID ChangeWeaponAction( eWEAPATTR szWeaponAttrType, LPCSTR lpszActionName, BOOL bLoop, FLOAT fFuseParam );
	virtual VOID ChangeWeaponActionSpeed( eWEAPATTR szWeaponAttrType, FLOAT fRate );


	/** 特效相关 
	 */
	virtual UINT AddEffect( LPCSTR effectName, LPCSTR locatorName );
	virtual VOID DelEffect( UINT effect );
	virtual VOID DelAllEffect( VOID );
	virtual VOID	AddAreaEffect(LPCSTR templateName);
	virtual bool	RemoveAreaEffect(LPCSTR templateName);

	Fairy::LogicModel* GetActorImpl( VOID )					{ return mModel; }

	/** 获取地面类型，以便产生不同的声音 */
	virtual int GetTerrainSoundType();
	/** 设置缩放参数 */
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