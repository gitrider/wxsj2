
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

/** 对象实体数据
 */
class CObjectEntity : public tEntityNode
{
public:

	CObjectEntity();
	virtual ~CObjectEntity();


	/** 从渲染层销毁节点 */
	virtual VOID Destroy(VOID);

	/** 设置附加数据
	 */
	virtual VOID SetData(UINT nData)									{ m_nData = nData; }
	virtual UINT GetData(VOID) const									{ return m_nData; }

	/** 设置位置(游戏世界坐标) */
	virtual VOID SetPosition(const fVector3& vPos)						{}
	/** 设置旋转角度 */
	virtual VOID SetOrientation(const fVector3& vRotate)				{}
	//设置位置 (渲染系统坐标)
	virtual VOID	SetGfxPosition(const fVector3& vPos) {}
	
	/** 直接设置Fairy属性
	 */
	virtual VOID SetFairyProperty(const char* szPropertyName, const char* szPropertyValue);

	/** 设置是否可见 */
	virtual VOID SetVisible(BOOL bVisible)								{}

	/** 设置是否能被鼠标选中
	 */
	virtual VOID SetRayQuery(BOOL bQuery)								{ m_bRayQuery = bQuery; }
	virtual BOOL GetRayQuery(VOID) const								{ return m_bRayQuery; }

	/** 鼠标选中优先级
	 */
	virtual VOID RayQuery_SetLevel(RAYQUERY_LEVEL nLevel)				{ m_nRayQueryLevel = nLevel; }
	virtual RAYQUERY_LEVEL RayQuery_GetLevel(VOID) const				{ return m_nRayQueryLevel; }

	/** 附加一个特效
	 */
	virtual VOID Attach_Effect(LPCTSTR szEffectName)					{}
	virtual VOID Detach_Effect(void)									{}

	virtual UINT AddEffect(LPCSTR effectName, LPCSTR locatorName)		{ return NULL; }
	virtual VOID DelEffect( UINT effect ){}
	virtual VOID DelAllEffect( VOID ){}
	virtual VOID	AddAreaEffect(LPCSTR templateName) {}
	virtual bool	RemoveAreaEffect(LPCSTR templateName) { return false; }


	virtual VOID SetSkillBulletData(VOID* pData){}

	/** 设置武器特效
	 */
	virtual VOID SetLeftWeaponEffect (LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color) {}
	virtual VOID SetRightWeaponEffect(LPCTSTR szEffectName, LPCSTR szLocatorName, UINT color) {}

	virtual VOID ClearWeaponEffect ( eWEAPATTR eWeaponAttr ) {};


	/** 改变武器的动作
	*	
	*	szWeaponAttrType			绑定点类型（Left， right， Shield）
	*	lpszActionName				动作名
	*	bLoop									动作是否循环播放
	*	fFuseParam						动作融合时间
	*/
	virtual VOID ChangeWeaponAction( eWEAPATTR szWeaponAttrType, LPCSTR lpszActionName, BOOL bLoop, FLOAT fFuseParam )	{};
	virtual VOID	ChangeWeaponActionSpeed( eWEAPATTR szWeaponAttrType, FLOAT fRate ) {};


	/** 附加一个投射纹理 */
	virtual VOID Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f)		{}
	virtual VOID Attach_ProjTexture(const char* effectName, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight) {}

	/** 设置MeshFile */
	virtual VOID StaObj_SetMeshFile(LPCTSTR szMeshFile)						{}
	/** 设置角色模型资源名 */
	virtual VOID Actor_SetFile(LPCTSTR szActorFile)							{}
	/** 设置角色模型的属性 */
	virtual VOID Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue)	{}
	/** 得到角色模型的属性 */
	virtual VOID Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue)	{}
	/** 取得任务模型上的某关键点位置 */
	virtual VOID Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition)		{}

	/** 取得"头顶状态点"在屏幕上的位置,如果返回FALSE，表示在屏幕之外,或者没有该点 */
	virtual BOOL Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1)		{ return FALSE; }
	
	/** 进入招式
	 *	bAnim		- TRUE(调用动作)/FALSE(调用招式)
	 *	szSkillName	- 动作名或者招式名
	 *	bLoop		- 是否循环
	 *	fFuseParam	- 骨骼动画融合参数
	 */
	virtual BOOL Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam=0.3, INT nFlag = 3, INT nObjType = 0, INT nLogicTag = 0){return FALSE;}
	virtual VOID	Actor_PlayAnimation(LPCTSTR szAnimationName, FLOAT fStartTime=0.0f, FLOAT fWeight=1.0f) {};
	virtual VOID Actor_SetHairColor(UINT HairColor)													{}
	virtual Fairy::Skill* GetCurrentSkill(){return NULL;}
	virtual VOID StopCurrentAction(){}
	
	/**	切换动画的播放速度
	 *	fRate		- 缩放比率
	 */
	virtual VOID Actor_ChangeActionRate(FLOAT fRate)									{}
	/** 设置缺省动作 */
	virtual VOID Actor_SetDefaultAnim(LPCTSTR szAnimName)								{}
	
	/** 设置VisibleFlag */
	virtual VOID Actor_SetUIVisibleFlag(VOID)											{}
	
	/** 设置透明度 */
	virtual VOID Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime)					{}
	
	/** 设置连线特效的目标点 */
	virtual VOID Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition)	{}
	
	/** 设置是否出于鼠标Hover状态 */
	virtual VOID Actor_SetMouseHover(BOOL bHover)										{} 

		virtual VOID	Actor_ChangeActionLoop(bool bLoop){}


protected:

	/// 
	UINT				m_nData;
	/// 鼠标选中优先级,数越大越优先选中
	RAYQUERY_LEVEL		m_nRayQueryLevel;


	/// 引入FairyObject
	Fairy::ObjectPtr	m_pFairyObj;
	/// 是否能被鼠标选中
	BOOL				m_bRayQuery;

};	// class


#endif	// _OBJECT_ENTITY_H