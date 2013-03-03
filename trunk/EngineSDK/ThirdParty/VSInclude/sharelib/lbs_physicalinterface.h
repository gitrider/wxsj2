#ifndef __LBS_PHYSICAL_INTERFACE_H__
#define __LBS_PHYSICAL_INTERFACE_H__

#include "fi_messagestruct.h"

class LBS_SplitSpace;
struct FI_CollisionResultData;
struct LOM_BaseObject;
class FI_Mesh;
class FI_OctTree;
struct FI_CollisionResultData;

/// Enable Collision with water 
///Water

/// Collision object interface
struct LOM_ICollisionObject
{
	virtual void	SetCollisionCtrlType( int nType )	=	0;
	virtual int		GetCollisionCtrlType()				=	0;
	virtual int		GetCollisionType()		=	0;
	virtual FI_GID_T		GetCollisionObjectID()	=	0;
	virtual void	SetCollsionType( int nType )		=	0;
	virtual void	SetCollisionID( FI_GID_T &idCol )	=	0;
	virtual void	UpdateMatrix( irr::core::matrix4 &m )	=	0;
};

/// Collision manager interface
struct LOM_ICollisionManager
{
	virtual void	SetCollisionSpace( LBS_SplitSpace* pSpace )	=	0;
	virtual void	EnableCollisionSpace(bool bE)				=	0;
	virtual bool	IsEnableCollisionSpace()					=	0;

	virtual void	UpdateCollObjByObject(LOM_BaseObject *pObj) =	0;
	virtual bool	DeleteCollisionObject( FI_GID_T &id )	=	0;
	virtual	LOM_ICollisionObject*		FindCollsionObject( FI_GID_T &idObj )	=	0;

	virtual void	SetCollisionDisableType( int nType1, int nType2 )	=	0;

	virtual bool	DoCollisionDefault( LOM_BaseObject *pObj, int nTimePass, FI_CollisionResultData &result, FI_GID_T &idColl  )	=	0;
	virtual std::vector<FI_GID_T>	GetAllIntersectObjectID( LOM_BaseObject *pObj, int nTimePass  )			=	0;

	virtual LOM_ICollisionObject*		CreateObjectBoxCollisionObject( LOM_BaseObject *pObj )		=	0;
	virtual LOM_ICollisionObject*		CreateObjectMeshCollisionObject( LOM_BaseObject *pObj, FI_Mesh *pMesh )		=	0;
	virtual LOM_ICollisionObject*		CreateObjectSubMeshCollisionObject( LOM_BaseObject *pObj, FI_Mesh *pMesh )		=	0;
	virtual LOM_ICollisionObject*		CreateObjectOctTreeMeshCollisionObject( LOM_BaseObject *pObj, FI_Mesh *pMesh, FI_OctTree *pOctTree )	=	0;
	virtual LOM_ICollisionObject*		CreateWaterCollisionObject( LOM_BaseObject *pObj )	=	0;
	virtual LOM_ICollisionObject*		CreateHeightmapCollisionObject( LOM_BaseObject *pObj )	=	0;
};

#endif

