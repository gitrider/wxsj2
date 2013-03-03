
/** ObjEntityDummy.h
 */

#ifndef _OBJECT_ENTITY_DUMMY_H
#define _OBJECT_ENTITY_DUMMY_H


#include "ObjectEntity.h"



namespace Fairy
{
	class Effect;
	class EffectElement;
};

class CObjEntityProjTex;

/**	空对象， 可以绑其他物品
 */
class CObjEntityDummy : public CObjectEntity
{
public:

	CObjEntityDummy();
	virtual ~CObjEntityDummy();


	/** 得到类型 */
	virtual EntityType GetType(VOID) const					{ return ETYPE_DUMMY; }

	/** 从渲染层销毁节点 */
	virtual VOID Destroy(VOID);

	/** 设置位置 (游戏世界坐标) */
	virtual VOID SetPosition(const fVector3& vPos);
	/** 设置旋转角度 */
	virtual VOID SetOrientation(const fVector3& vRotate);
	
	/** 设置是否可见 */
	virtual VOID SetVisible(BOOL bVisible);
	
	/** 附加一个特效
	 */
	virtual VOID Attach_Effect(LPCTSTR szEffectName);
	virtual VOID Detach_Effect(void);
	
	/** 附加一个投射纹理 */
	virtual VOID Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );
	virtual VOID Attach_ProjTexture(const char* effectName, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight);


	/** 不能被鼠标选中 */
	virtual BOOL GetRayQuery(VOID) const					{ return FALSE; }


protected:

	/// 场景节点
	Ogre::SceneNode*				m_pSceneNode;
	/// 附属投射阴影
	std::vector<CObjEntityProjTex*>		m_vProjTex;
	/// 附属的特效
	Fairy::Effect*					m_pEffect;

};	// class


#endif	// _OBJECT_ENTITY_DUMMY_H