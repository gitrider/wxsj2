#include "stdafx.h"
#include "ItemBoxPool.h"
#include "Obj_ItemBox.h"
#include "ItemTypes.h"
#include "Ini.h"






ItemBoxPool*	g_pItemBoxPool = NULL;


ItemBoxPool::ItemBoxPool():
m_nFreePosition(0)
{
	m_pItemBox = NULL ;
	m_pFreeItemID = NULL ;
}


ItemBoxPool::~ItemBoxPool()
{
	SAFE_DELETE_ARRAY( m_pItemBox ) ;
	SAFE_DELETE_ARRAY( m_pFreeItemID ) ;
}

BOOL	ItemBoxPool::Init()
{
	__ENTER_FUNCTION

	m_pItemBox	=	new	Obj_ItemBox[MAX_ITEM_BOX_POOL];
	Assert(m_pItemBox);
	for(INT i = 0;i<MAX_ITEM_BOX_POOL;i++)	m_pItemBox[i].SetPoolPos(i);

	m_pFreeItemID	= new INT[MAX_ITEM_BOX_POOL];
	Assert(m_pFreeItemID);

	for(INT i = 0;i<MAX_ITEM_BOX_POOL;i++)	m_pFreeItemID[i]	=	i;



	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}



Obj_ItemBox*	ItemBoxPool::NewItemBox()
{
	__ENTER_FUNCTION

		m_Lock.Lock();

	Assert(m_nFreePosition<=MAX_ITEM_BOX_POOL);
	Obj_ItemBox& ib = m_pItemBox[m_pFreeItemID[m_nFreePosition]];

	m_nFreePosition++;

	m_Lock.Unlock();

	return &ib;

	__LEAVE_FUNCTION

		m_Lock.Unlock();
		return NULL ;
}

VOID	ItemBoxPool::DelItemBox(uint iItemPos)
{
	__ENTER_FUNCTION

		m_Lock.Lock();

	Assert(	iItemPos>= 0 && iItemPos < MAX_ITEM_BOX_POOL);

	m_nFreePosition--;
	Assert(m_nFreePosition>=0);

	m_pFreeItemID[m_nFreePosition]	=	iItemPos;

	m_Lock.Unlock();
	return ;
	__LEAVE_FUNCTION
	m_Lock.Unlock();
}

VOID	ItemBoxPool::DeleteObj(Obj_ItemBox* pItem)
{
	__ENTER_FUNCTION

	m_Lock.Lock();
	uint iPoolPos = pItem->GetPoolPos();
	Assert(	iPoolPos>= 0 && iPoolPos < MAX_ITEM_BOX_POOL);
	m_nFreePosition--;
	Assert(m_nFreePosition>=0);
	m_pFreeItemID[m_nFreePosition]	= pItem->GetPoolPos();
	m_Lock.Unlock();
	return ;

	__LEAVE_FUNCTION
	m_Lock.Unlock();
}


