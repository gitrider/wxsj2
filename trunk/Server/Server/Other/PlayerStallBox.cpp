/************************************************************************/
/*
��������:	2006��2��13��
����ʱ��:	11:54
�ļ�����:	PlayerStallBox.h
�ļ�·��:	d:\Prj\Server\Server\Other\PlayerStallBox.cpp
������:		sunyu

�ļ����ܣ�	����̵��һ����̨
�޸ļ�¼��
*/
/************************************************************************/

#include "stdafx.h"
#include "PlayerStallBox.h"
#include "PlayerShop.h"
VOID PlayerStallBox::Init(StallBoxDB_t* pStallBoxDB, PlayerShop* pShop)
{
	__ENTER_FUNCTION

	if(pStallBoxDB == NULL)
		return;
	m_pStallBoxDB			= pStallBoxDB;

	//���ӵ�����ʱ����
	m_StallBoxRT.m_pShop	=	pShop;

	//��ʼ��container
	_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	STALL_BOX_SIZE;
	m_StallBoxRT.m_Container.Init(&containInit);

	for(INT i=0;i<STALL_BOX_SIZE;i++)
	{
		_ITEM_INIT	itemInit(&m_pStallBoxDB->m_ItemList[i]);
		m_StallBoxRT.m_Container.GetItem(i)->Init(&itemInit);
		m_StallBoxRT.m_Serial[i] = 0;
	}

	//��ʼ��container
	_ITEMCONTAINER_INIT	petcontainInit;
	petcontainInit.m_nContainerSize	=	STALL_PET_BOX_SIZE;
	m_StallBoxRT.m_PetContainer.Init(&petcontainInit);

	for(INT i=0;i<STALL_PET_BOX_SIZE;i++)
	{
		_ITEM_INIT	petInit(&m_pStallBoxDB->m_PetList[i]);
		m_StallBoxRT.m_PetContainer.GetItem(i)->Init(&petInit);
		m_StallBoxRT.m_Serial[i] = 0;
	}

	__LEAVE_FUNCTION
}

VOID PlayerStallBox::CleanUp()
{
	__ENTER_FUNCTION
	
	m_pStallBoxDB->CleanUp();
	for(INT i=0;i<STALL_BOX_SIZE;i++)
	{
		m_StallBoxRT.m_Serial[i] = 0;
	}


	__LEAVE_FUNCTION
}


BOOL PlayerStallBox::CanSale(UINT uIndex)
{
	__ENTER_FUNCTION

	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		return m_pStallBoxDB->m_bItemIsOnSale[uIndex];
	}
	__LEAVE_FUNCTION

	return FALSE;
}
VOID PlayerStallBox::SetCanSale(UINT uIndex, BOOL bCan)
{
	__ENTER_FUNCTION

	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_pStallBoxDB->m_bItemIsOnSale[uIndex] = bCan;
		m_StallBoxRT.m_pShop->DataChanged();
	}
	__LEAVE_FUNCTION

}

UINT PlayerStallBox::GetPriceByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		return m_pStallBoxDB->m_ItemPrice[uIndex];
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}

VOID PlayerStallBox::SetPriceByIndex(UINT uIndex, UINT nPrice)
{
	__ENTER_FUNCTION

	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_pStallBoxDB->m_ItemPrice[uIndex] = nPrice;
		m_StallBoxRT.m_pShop->DataChanged();
	}
	__LEAVE_FUNCTION
}

BYTE PlayerStallBox::GetSerialByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		 return m_StallBoxRT.m_Serial[uIndex];
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}

VOID PlayerStallBox::SetSerialByIndex(UINT uIndex, BYTE uSerial)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_StallBoxRT.m_Serial[uIndex] = uSerial;
	}
	__LEAVE_FUNCTION
}

BYTE PlayerStallBox::IncSerialByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_StallBoxRT.m_Serial[uIndex]++;
		return m_StallBoxRT.m_Serial[uIndex];
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}

VOID PlayerStallBox::SetStallStatus(const STATUS_STALL stallstatus)
{
	m_pStallBoxDB->m_StallStatus = (BYTE)stallstatus;
	m_StallBoxRT.m_pShop->DataChanged();
}

VOID PlayerStallBox::SetStallType(const TYPE_STALL stalltype)
{
	m_pStallBoxDB->m_StallType = (BYTE)stalltype;
	m_StallBoxRT.m_pShop->DataChanged();
}
