#include "stdafx.h"
#include "Obj_ItemPool.h"
#include "Obj_Item.h"
#include "ItemTypes.h"
#include "Ini.h"


Obj_ItemPool*	g_pObj_ItemPool = NULL;


Obj_ItemPool::Obj_ItemPool():
m_nFreePosition(0)
{
	m_pObj_Item = NULL ;
	m_pFreeObj_ItemID = NULL ;
}


Obj_ItemPool::~Obj_ItemPool()
{
	SAFE_DELETE_ARRAY( m_pObj_Item ) ;
	SAFE_DELETE_ARRAY( m_pFreeObj_ItemID ) ;
}

BOOL	Obj_ItemPool::Init()
{
	__ENTER_FUNCTION

	m_pObj_Item	=	new	Obj_Item[MAX_OBJ_ITEM_POOL];
	Assert(m_pObj_Item);
	for(INT i = 0;i<MAX_OBJ_ITEM_POOL;i++)	m_pObj_Item[i].SetPoolPos(i);

	m_pFreeObj_ItemID	= new INT[MAX_OBJ_ITEM_POOL];
	Assert(m_pFreeObj_ItemID);

	for(INT i = 0;i<MAX_OBJ_ITEM_POOL;i++)	m_pFreeObj_ItemID[i]	=	i;


	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}



Obj_Item*	Obj_ItemPool::NewObj_Item()
{
	__ENTER_FUNCTION

	m_Lock.Lock();

	Assert(m_nFreePosition<=MAX_ITEM_BOX_POOL);
	Obj_Item& ib = m_pObj_Item[m_pFreeObj_ItemID[m_nFreePosition]];
	m_nFreePosition++;

	m_Lock.Unlock();

	return &ib;

	__LEAVE_FUNCTION

		m_Lock.Unlock();
	return NULL ;
}

VOID	Obj_ItemPool::DelObj_Item(uint iItemPos)
{
	__ENTER_FUNCTION

	m_Lock.Lock();

	Assert(	iItemPos>= 0 && iItemPos < MAX_OBJ_ITEM_POOL);

	m_nFreePosition--;
	Assert(m_nFreePosition>=0);

	m_pFreeObj_ItemID[m_nFreePosition]	=	iItemPos;
	
	
	

	m_Lock.Unlock();
	return ;
	__LEAVE_FUNCTION
		m_Lock.Unlock();
}

VOID	Obj_ItemPool::DeleteObj(Obj_Item* pItem)
{
	__ENTER_FUNCTION

		m_Lock.Lock();
	uint iPoolPos = pItem->GetPoolPos();
	Assert(	iPoolPos>= 0 && iPoolPos < MAX_OBJ_ITEM_POOL);
	m_nFreePosition--;
	Assert(m_nFreePosition>=0);
	m_pFreeObj_ItemID[m_nFreePosition]	= pItem->GetPoolPos();
	m_Lock.Unlock();
	return ;

	__LEAVE_FUNCTION
		m_Lock.Unlock();
}