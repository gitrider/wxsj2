
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


	/** 得到类型 */
	virtual EntityType GetType(VOID) const					{ return ETYPE_MODEL; }
	
	/** 设置是否可见 */
	virtual VOID SetVisible(BOOL bVisible);

	/** 设置MeshFile */
	virtual VOID StaObj_SetMeshFile(LPCTSTR szMeshFile);


	/** 设置位置 (游戏世界坐标) */
	virtual VOID SetPosition(const fVector3& vPos);
	/** 设置旋转角度 */
	virtual VOID SetOrientation(const fVector3& vRotate);


};	// class


#endif	// _OBJECT_ENTITY_MODEL_H