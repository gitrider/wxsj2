#include "stdafx.h"
#include 		"Obj_ItemManager.h"
#include 		"WorldManager.h"
#include 		"ServerManager.h"
#include 		"Obj_ItemPool.h"
#include 		"Obj_Item.h"
#include 		"Scene.h"
#include		"GCDelObject.h"
#include		"ItemTable.h"
#include		"ItemRuler.h"
#include		"TimeManager.h"
#include		"SceneDropPosManager.h"


//默认Obj_ItemManager 管理的Obj_Item 个数
#define			DEFAULT_OBJ_ITEM_MANAGER_SIZE			1024 
//默认Obj_ItemManager HeartBeat 间隔
#define			OBJ_ITEMMANAGER_LOGIC_INTERVAL			500


Obj_ItemManager::Obj_ItemManager()
{
	__ENTER_FUNCTION

	m_TotalSize = DEFAULT_OBJ_ITEM_MANAGER_SIZE;

	m_Count		= 0;

	m_pObj_ItemID = new ObjID_t[DEFAULT_OBJ_ITEM_MANAGER_SIZE];
	Assert( m_pObj_ItemID ) ;

	memset(m_pObj_ItemID,-1,sizeof(ObjID_t)*DEFAULT_OBJ_ITEM_MANAGER_SIZE);

	__LEAVE_FUNCTION
}

Obj_ItemManager::~Obj_ItemManager()
{
	__ENTER_FUNCTION

	m_TotalSize			=	0;
	m_Count				=	0;

	SAFE_DELETE_ARRAY(m_pObj_ItemID);

	__LEAVE_FUNCTION
}

VOID	Obj_ItemManager::Init()
{
	__ENTER_FUNCTION

	m_TotalSize = DEFAULT_OBJ_ITEM_MANAGER_SIZE;
	m_Count		= 0;
	Assert( m_pObj_ItemID ) ;
	memset(m_pObj_ItemID,-1,sizeof(ObjID_t)*DEFAULT_OBJ_ITEM_MANAGER_SIZE);

	__LEAVE_FUNCTION
}

Obj_Item*	Obj_ItemManager::GenObj_Item()
{
	__ENTER_FUNCTION

	Obj_Item*	pIB		=	g_pObj_ItemPool->NewObj_Item();
	pIB->CleanUp();

	return pIB;

	__LEAVE_FUNCTION

	return NULL ;
}


Obj_Item*	Obj_ItemManager::GetObj_Item(ObjID_t ObjID)
{
	__ENTER_FUNCTION

	Obj_Item*	pItem	=	static_cast<Obj_Item*>(GetScene()->GetObjManager()->GetObj(ObjID)) ;
	Assert( pItem ) ;
	return pItem;

	__LEAVE_FUNCTION

		return NULL ;
}

BOOL	Obj_ItemManager::RemoveAllObj_Item()
{
	INT iCount = m_Count;

	for(int i = 0;i<iCount;i++)
	{
		Obj_Item* pObj_Item = GetObj_Item(m_pObj_ItemID[0]);

		if(pObj_Item)
		{
			pObj_Item->Recycle();
		}
	}

	return TRUE;
}

VOID	Obj_ItemManager::RecycleObj_Item(ObjID_t ObjID)
{
	__ENTER_FUNCTION

		Obj_Item*	pObj_Item	=	GetObj_Item(ObjID) ;
	Assert( pObj_Item ) ;

	pObj_Item->SetActiveFlag( FALSE );
	pObj_Item->CleanUp();

	GetScene()->DeleteObject( pObj_Item );

	__LEAVE_FUNCTION
}

BOOL	Obj_ItemManager::Add(Obj_Item* pObj_Item)
{
	__ENTER_FUNCTION

		ObjID_t	objId = pObj_Item->GetID();
	m_pObj_ItemID[m_Count] = objId;
	pObj_Item->SetObj_ItemID(m_Count);

	m_Count ++ ;

	Assert(m_Count<=MAX_OBJ_ITEM_POOL);

	if( m_Count >= m_TotalSize )
	{
		BOOL ret = Resize( DEFAULT_OBJ_ITEM_MANAGER_SIZE ) ;
		if( !ret )
		{
			Assert( FALSE ) ;
			return FALSE ;
		}
	}
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL	Obj_ItemManager::Resize(uint iSize)
{
	__ENTER_FUNCTION

		ObjID_t* pNew = new ObjID_t[m_TotalSize+iSize] ;
	if( !pNew )
	{
		Assert( pNew ) ;
		return FALSE ;
	}

	memcpy( pNew, m_pObj_ItemID, sizeof(ObjID_t)*m_TotalSize ) ;
	m_TotalSize = m_TotalSize+iSize ;

	SAFE_DELETE_ARRAY( m_pObj_ItemID ) ;

	m_pObj_ItemID = pNew ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL	Obj_ItemManager::Remove(ObjID_t ObjID)
{	
	__ENTER_FUNCTION

		Obj_Item*	pObj_Item	=	GetObj_Item(ObjID);

	Assert( pObj_Item ) ;

	uint index = (uint)(pObj_Item->GetObj_ItemID()) ;
	Assert( index<m_Count ) ;

	Assert( m_Count > 0 ) ;
	Assert( m_pObj_ItemID[index]==ObjID ) ;

	m_pObj_ItemID[index] = m_pObj_ItemID[m_Count-1] ;
	pObj_Item->SetObj_ItemID( INVALID_ID ) ;

	Obj_Item* pNewObj_Item = GetObj_Item(m_pObj_ItemID[index]);

	Assert( pNewObj_Item ) ;
	pNewObj_Item->SetObj_ItemID( index ) ;
	m_Count -- ;
	Assert( m_Count>=0 ) ;
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;

}

BOOL	Obj_ItemManager::HeartBeat(UINT uTime/* =0  */)
{

	__ENTER_FUNCTION

		if( !m_LogicTimer.IsSetTimer() )
		{
			m_LogicTimer.BeginTimer( OBJ_ITEMMANAGER_LOGIC_INTERVAL,uTime ) ;
			return TRUE ;
		}

		if( !m_LogicTimer.CountingTimer(uTime ) )
			return TRUE ;


		uint nCount = m_Count;

		for( uint i=0; i<nCount; i++ )
		{
			Obj_Item* pObj_Item = GetObj_Item(m_pObj_ItemID[i]);

			if( pObj_Item==NULL )
				continue ;

			_MY_TRY
			{
				pObj_Item->HeartBeat(uTime );
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}

		return TRUE ;

		__LEAVE_FUNCTION

			return FALSE ;
}


Obj_ItemContaner	Obj_ItemManager::CaculateItemDropFromMonster(uint iKillerLevel,
																 uint iMonsterID,
																 BOOL bTeam)
{
	Obj_ItemContaner IBContaner;
	__ENTER_FUNCTION
	
	Obj_ItemRuler ibr;

	ibr.CreateItemFromMonsterDrop(iMonsterID,
		iKillerLevel,
		bTeam,
		1.0f,
		IBContaner);	

	return IBContaner;
	__LEAVE_FUNCTION
		return IBContaner;
}


Obj_Item*	Obj_ItemManager::CreateMonsterDropObj_Item(WORLD_POS* pPos, _ITEM* pItem)
{
	__ENTER_FUNCTION
	Assert(pItem);
	Assert(!pItem->IsNullType());
	Obj_Item* pObj_Item = (Obj_Item*)(GetScene()->NewObject(Obj::OBJ_TYPE_DROP_ITEM));

	SceneDropPosManager* pDropPosMag = pObj_Item->getScene()->GetSceneDropPosManager();
	Assert(pDropPosMag);

	_OBJ_ITEM_INIT	initParam;
	initParam.m_uCreateTime		= g_pTimeManager->CurrentTime();
	initParam.m_uRecycleTime	= g_Config.m_ConfigInfo.m_DropBoxRecycleTime;
	initParam.m_Pos	=	*pPos;
	initParam.m_ItemType		=	ITYPE_MONSTERDROP_OBJ_ITEM;
	initParam.m_bRecycle		=	TRUE;
	pObj_Item->Init(&initParam);
	pObj_Item->SaveObj_Item(pItem);
	pObj_Item->EnablePickOwnerTime();

	pDropPosMag->SetDropPos(pPos->m_fX, pPos->m_fZ);
	return pObj_Item;

	__LEAVE_FUNCTION

		return NULL;
}

Obj_Item*	Obj_ItemManager::CreatePersonDropObj_Item(WORLD_POS* pPos, Item* pItem)
{
	__ENTER_FUNCTION
		Assert(pItem);
	Assert(!pItem->IsEmpty());
	Obj_Item* pObj_Item = (Obj_Item*)(GetScene()->NewObject(Obj::OBJ_TYPE_DROP_ITEM));
	SceneDropPosManager* pDropPosMag = pObj_Item->getScene()->GetSceneDropPosManager();
	Assert(pDropPosMag);

	_OBJ_ITEM_INIT	initParam;
	initParam.m_uCreateTime		= g_pTimeManager->CurrentTime();
	initParam.m_uRecycleTime	= g_Config.m_ConfigInfo.m_DropBoxRecycleTime;
	initParam.m_Pos	=	*pPos;
	initParam.m_ItemType		=	ITYPE_PERSONDROP_OBJ_ITEM;
	initParam.m_bRecycle		=	TRUE;
	pObj_Item->Init(&initParam);
	pItem->SaveValueTo(pObj_Item->GetObj_Item());
	pObj_Item->EnablePickOwnerTime();
	return pObj_Item;

	__LEAVE_FUNCTION

		return NULL;
}
//Obj_Item*	Obj_ItemManager::CreateGrowPointObj_Item(INT IBType)
//{
//	__ENTER_FUNCTION
//
//		Obj_Item* pIB = GenObj_Item();
//
//	Assert(ITYPE_DROPBOX!=IBType);
//	_OBJ_ITEM_INIT	initParam;
//	initParam.m_uCreateTime		= g_pTimeManager->CurrentTime();
//	initParam.m_uRecycleTime	= 3600000000;
//	initParam.m_ItemType	= IBType;
//	initParam.m_bRecycle		= FALSE;
//	pIB->Init(&initParam);
//
//
//	return pIB;
//
//	__LEAVE_FUNCTION
//
//		return NULL ;
//}



