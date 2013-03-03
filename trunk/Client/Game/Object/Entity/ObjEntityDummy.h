
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

/**	�ն��� ���԰�������Ʒ
 */
class CObjEntityDummy : public CObjectEntity
{
public:

	CObjEntityDummy();
	virtual ~CObjEntityDummy();


	/** �õ����� */
	virtual EntityType GetType(VOID) const					{ return ETYPE_DUMMY; }

	/** ����Ⱦ�����ٽڵ� */
	virtual VOID Destroy(VOID);

	/** ����λ�� (��Ϸ��������) */
	virtual VOID SetPosition(const fVector3& vPos);
	/** ������ת�Ƕ� */
	virtual VOID SetOrientation(const fVector3& vRotate);
	
	/** �����Ƿ�ɼ� */
	virtual VOID SetVisible(BOOL bVisible);
	
	/** ����һ����Ч
	 */
	virtual VOID Attach_Effect(LPCTSTR szEffectName);
	virtual VOID Detach_Effect(void);
	
	/** ����һ��Ͷ������ */
	virtual VOID Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );
	virtual VOID Attach_ProjTexture(const char* effectName, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight);


	/** ���ܱ����ѡ�� */
	virtual BOOL GetRayQuery(VOID) const					{ return FALSE; }


protected:

	/// �����ڵ�
	Ogre::SceneNode*				m_pSceneNode;
	/// ����Ͷ����Ӱ
	std::vector<CObjEntityProjTex*>		m_vProjTex;
	/// ��������Ч
	Fairy::Effect*					m_pEffect;

};	// class


#endif	// _OBJECT_ENTITY_DUMMY_H