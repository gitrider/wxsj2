#ifndef _EditDObject_NT_H_
#define _EditDObject_NT_H_

#define MODELSYSTEM_STATIC_LINK
#include "Modelsystem\SkeletonMeshUtil.h"
#include "Modelsystem\SkeletonMesh.h"


////////////////////////////////////////////////////////////////////////////////////
//
// �༭��ʹ��.
//

// object ѡ��ʱʹ�õĲ��ʡ�
typedef			std::vector<Ogre::MaterialPtr> MATERIAL_SELECT_VECTOR;

// object ԭ���Ĳ���.
typedef			std::vector<Ogre::MaterialPtr> MATERIAL_OLD_VECTOR;


namespace Fairy	{
	class System;
};

class CEditDobject_NT : public DObject_NT
{
public:
	CEditDobject_NT( Fairy::System *system );
	~CEditDobject_NT(void);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// �༭��ʹ��.
	//
	

	// 
	public:

	// ����ģ�͵�͸����
	// 0 -- ��ȫ͸����
	// 1 -- ��͸����
	void SetTransparence(float Transparence = 1.0);

	// ����ģ�͵Ļ����ɫ
	void SetBlendColor(float fRed = 0, float fGreen = 0, float fBlue = 0);

	// object ѡ��ʱʹ�õĲ��ʡ�
	MATERIAL_SELECT_VECTOR m_materialSelVector;

	// object ԭ���Ĳ���.
	MATERIAL_OLD_VECTOR    m_materilaOldVector;

	// ����ѡ�е������ɫ.
	void SetSelectLook(Ogre::ColourValue color);

	// ���������������ɫ.
	void SetUnSelLook();

	// ��ղ��� 
	void ClearMaterial();

	//
	// �༭��ʹ��.
	//
	////////////////////////////////////////////////////////////////////////////////////

};

#endif
