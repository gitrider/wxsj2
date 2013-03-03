#ifndef _OPENBOXCONTAINER_CPP_
#define _OPENBOXCONTAINER_CPP_
//#include "stdafx.h"
#include "HumanOpenBox.h"

template< int OPEN_BOX_SIZE >
OpenBoxContainer<OPEN_BOX_SIZE>::OpenBoxContainer()
{
	if ( OPEN_BOX_SIZE == TMP_OPENBOX_CONTAINER_SIZE )
	{
		Init();
	}

}

template< int OPEN_BOX_SIZE >
OpenBoxContainer<OPEN_BOX_SIZE>::~OpenBoxContainer()
{
	if ( OPEN_BOX_SIZE == TMP_OPENBOX_CONTAINER_SIZE )
	{
		CleanUp();
	}

}
template< int OPEN_BOX_SIZE >
VOID OpenBoxContainer<OPEN_BOX_SIZE>::Init()
{
	__ENTER_FUNCTION

	//清空物品数据
	memset(m_ItemList, 0, sizeof(_ITEM)*OPEN_BOX_SIZE);

	//初始化物品container
	_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	OPEN_BOX_SIZE;
	m_Container.Init(&containInit);

	for(UINT i=0;i<OPEN_BOX_SIZE;i++)
	{
		_ITEM_INIT	itemInit(&m_ItemList[i]);
		m_Container.GetItem(i)->Init(&itemInit);
	}

	CleanUp();
	__LEAVE_FUNCTION
}

template< int OPEN_BOX_SIZE >
VOID OpenBoxContainer<OPEN_BOX_SIZE>::CleanUp()
{
	__ENTER_FUNCTION

	//清空物品数据
	memset(m_ItemList, 0, sizeof(_ITEM)*OPEN_BOX_SIZE);
	m_Container.CleanUp();
	SetState( FALSE );

	__LEAVE_FUNCTION
}



#endif
