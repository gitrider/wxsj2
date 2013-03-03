
/** ObjEntityProjTex.h
 */

#pragma once


#include "GIGfxSystem.h"



namespace Ogre
{
	class SceneNode;
}
namespace Fairy
{
    class Effect;
}
class CObject;

/**
	附属在FairyObj上的投射纹理,包括选择环,阴影斑点,脚印等	
 */
class CObjEntityProjTex
{
public:

	CObjEntityProjTex(tEntityNode::PROJTEX_TYPE type, Ogre::SceneNode* pParentNode, FLOAT fAddHeight=170.0f);
	CObjEntityProjTex(const CHAR* effectName, Ogre::SceneNode* pParentNode, FLOAT fAddHeight=170.0f);
	virtual ~CObjEntityProjTex();

	tEntityNode::PROJTEX_TYPE	GetType(VOID) const						{ return m_theType; }
	VOID						Show(BOOL bShow);
	VOID						SetAddHeight(FLOAT fAddHeight = 1.7f);
	FLOAT						GetAddHeight(void) const				{ return m_fAddHeight; }
	void						ChangeEffect(const char* effectName, Ogre::SceneNode* pParentNode, FLOAT fAddHeight=170.0f);


protected:

	tEntityNode::PROJTEX_TYPE	m_theType;

    Fairy::Effect*				mProjectorEffect;

	/// 先是或者隐藏
	BOOL						m_bShow;
	/// 附加高度
	FLOAT						m_fAddHeight;
	//保存当前的特效名称
	STRING				m_effectName;

};	// class