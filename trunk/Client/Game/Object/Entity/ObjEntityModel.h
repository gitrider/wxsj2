
/** FairyObject_StaticMesh			*
 */

#ifndef _OBJECT_ENTITY_MODEL_H
#define _OBJECT_ENTITY_MODEL_H


#include "ObjectEntity.h"



class CObjEntityModel : public CObjectEntity
{
public:

	CObjEntityModel();
	virtual ~CObjEntityModel();


	/** �õ����� */
	virtual EntityType GetType(VOID) const					{ return ETYPE_MODEL; }
	
	/** �����Ƿ�ɼ� */
	virtual VOID SetVisible(BOOL bVisible);

	/** ����MeshFile */
	virtual VOID StaObj_SetMeshFile(LPCTSTR szMeshFile);


	/** ����λ�� (��Ϸ��������) */
	virtual VOID SetPosition(const fVector3& vPos);
	/** ������ת�Ƕ� */
	virtual VOID SetOrientation(const fVector3& vRotate);


};	// class


#endif	// _OBJECT_ENTITY_MODEL_H