/************************************************************************/
/*
	��������:	2006��2��13��
	����ʱ��:	11:54
	�ļ�����:	PlayerStallBox.h
	�ļ�·��:	d:\Prj\Server\Server\Other\PlayerStallBox.h
	������:		sunyu

	�ļ����ܣ�	����̵��һ����̨
	�޸ļ�¼��
*/
/************************************************************************/

#ifndef _PLAYER_STALL_BOX_H_
#define _PLAYER_STALL_BOX_H_

#include "PlayerShopDefine.h"
#include "ItemContainer.h"
/*
��������ÿ����̨�Ľṹ.
*/
class PlayerShop;
using namespace PLAYER_SHOP;
class PlayerStallBox
{
public:
	//����ʱ�ṹ
	struct StallBoxRT_t
	{
		ItemContainer	m_Container;							//ITEM_CONTAINER
		ItemContainer	m_PetContainer;							//PET_CONTAINER
		BYTE			m_Serial[STALL_BOX_SIZE];				//���к�����
		PlayerShop*		m_pShop;
	};
public:
	PlayerStallBox()
	{
		m_pStallBoxDB = NULL;
	};
	~PlayerStallBox()
	{

	};
public:
	//-------------------------------------------------------------------------------------------------
	//��ʼ��,�������̵�Ĺ�̨
	VOID Init(StallBoxDB_t* pStallBoxDB, PlayerShop* pShop);

	VOID CleanUp();
public:
	//-------------------------------------------------------------------------------------------------
	//����
	STATUS_STALL	GetStallStatus(){return (STATUS_STALL)m_pStallBoxDB->m_StallStatus;}
	VOID			SetStallStatus(const STATUS_STALL stallstatus);

	TYPE_STALL		GetStallType(){return (TYPE_STALL)m_pStallBoxDB->m_StallType;}
	VOID			SetStallType(const TYPE_STALL stalltype);

	BOOL			CanSale(UINT uIndex);
	VOID			SetCanSale(UINT uIndex, BOOL bCan);

	UINT			GetPriceByIndex(UINT uIndex);
	VOID			SetPriceByIndex(UINT uIndex, UINT nPrice);

	BYTE			GetSerialByIndex(UINT uIndex);
	VOID			SetSerialByIndex(UINT uIndex, BYTE uSerial);
	BYTE			IncSerialByIndex(UINT uIndex);

	ItemContainer*	GetContainer()
	{
		return &m_StallBoxRT.m_Container;
	}

	ItemContainer*	GetPetContainer()
	{
		return &m_StallBoxRT.m_PetContainer;
	}

private:
	//-------------------------------------------------------------------------------------------------
	//��Ҫ�洢���ݿ����Ϣ
	StallBoxDB_t*	m_pStallBoxDB;
	//-------------------------------------------------------------------------------------------------
	//����ʱ��Ϣ
	StallBoxRT_t	m_StallBoxRT;
};
#endif