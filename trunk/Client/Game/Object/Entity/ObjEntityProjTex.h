
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
	������FairyObj�ϵ�Ͷ������,����ѡ��,��Ӱ�ߵ�,��ӡ��	
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

	/// ���ǻ�������
	BOOL						m_bShow;
	/// ���Ӹ߶�
	FLOAT						m_fAddHeight;
	//���浱ǰ����Ч����
	STRING				m_effectName;

};	// class