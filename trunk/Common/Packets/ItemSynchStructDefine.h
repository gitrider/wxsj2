/************************************************************************/
/*
��������:	2006��3��2��
����ʱ��:	11:54
�ļ�����:	ItemSynchStructDefine.h
������:		sunyu

�ļ����ܣ�	�������п����õ�����Ʒ��Ϣ�ṹ
*/
/************************************************************************/

#ifndef __ITEMSYNCHSTRUCTDEFINE_H__
#define __ITEMSYNCHSTRUCTDEFINE_H__

#include "Type.h"
namespace ITEM_SYNCH_STRUCT
{
	//CG
	
	//�ӱ������ƶ���Ʒ������̵꣬�Զ�Ѱ��λ��
	struct CGAutoMoveItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		BYTE				m_nShopSerial;						//�̵����к�
		CGAutoMoveItemFromBagToPlayerShop_t()
		{
			m_nStallIndex = 0;
			m_nShopSerial = 0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMoveItemFromBagToPlayerShop_t);
		}
	};

	//�ӱ������ƶ���Ʒ������̵꣬�Զ�Ѱ��λ��
	struct CGAutoMovePetFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		BYTE				m_nShopSerial;						//�̵����к�
		CGAutoMovePetFromBagToPlayerShop_t()
		{
			m_nStallIndex = 0;
			m_nShopSerial = 0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMovePetFromBagToPlayerShop_t);
		}
	};

	//������̵��ƶ���Ʒ���������Զ�Ѱ��λ��
	struct CGAutoMoveItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		CGAutoMoveItemFromPlayerShopToBag_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMoveItemFromPlayerShopToBag_t);
		}
	};

	//������̵��ƶ���Ʒ���������Զ�Ѱ��λ��
	struct CGAutoMovePetFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		CGAutoMovePetFromPlayerShopToBag_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMovePetFromPlayerShopToBag_t);
		}
	};

	//�ӱ����ƶ���Ʒ������̵ָ꣬��λ��
	struct CGManuMoveItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		CGManuMoveItemFromBagToPlayerShop_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGManuMoveItemFromBagToPlayerShop_t);
		}
	};

	//������̵��ƶ���Ʒ��������ָ��λ��
	struct CGManuMoveItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		CGManuMoveItemFromPlayerShopToBag_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGManuMoveItemFromPlayerShopToBag_t);
		}
	};
    
	//������̵��ƶ���Ʒ������̵ָ꣬��λ��
	struct CGManuMoveItemFromPlayerShopToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerialSource;					//Դλ�����к�
		UINT				m_uSerialDest;						//Ŀ��λ�����к�
		BYTE				m_nShopSerial;						//�̵����к�
		CGManuMoveItemFromPlayerShopToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerialSource		=	0;
			m_uSerialDest		=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGManuMoveItemFromPlayerShopToPlayerShop_t);
		}
	};

	//GC
	//�ӱ����ƶ���Ʒ������̵�
	struct GCMoveItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		GCMoveItemFromBagToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMoveItemFromBagToPlayerShop_t);
		}
	};

	//�ӱ����ƶ���Ʒ������̵�
	struct GCMovePetFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		GCMovePetFromBagToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMovePetFromBagToPlayerShop_t);
		}
	};

	//������̵��ƶ���Ʒ��������ָ��λ��
	struct GCMoveItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		BYTE				m_Flag;								//��־
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		GCMoveItemFromPlayerShopToBag_t()
		{
			m_nStallIndex		=	0;
			m_Flag				=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMoveItemFromPlayerShopToBag_t);
		}
	};

	//������̵��ƶ���Ʒ��������ָ��λ��
	struct GCMovePetFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		BYTE				m_Flag;								//��־
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		GCMovePetFromPlayerShopToBag_t()
		{
			m_nStallIndex		=	0;
			m_Flag				=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMovePetFromPlayerShopToBag_t);
		}
	};

	//������̵��ƶ���Ʒ������̵�
	struct GCMoveItemFromPlayerShopToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerialSource;					//��Ʒ���к�
		UINT				m_uSerialDest;						//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�

		GCMoveItemFromPlayerShopToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerialSource		=	0;
			m_uSerialDest		=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMoveItemFromPlayerShopToPlayerShop_t);
		}
	};

	//�ӱ���������̵꽻����Ʒ
	struct GCExchangeItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		GCExchangeItemFromBagToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCExchangeItemFromBagToPlayerShop_t);
		}
	};

	//������̵꽻����Ʒ������
	struct GCExchangeItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerial;							//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�
		GCExchangeItemFromPlayerShopToBag_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCExchangeItemFromPlayerShopToBag_t);
		}
	};

	//������̵꽻����Ʒ������̵�
	struct GCExchangeItemFromPlayerShopToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//�̵�GUID
		BYTE				m_nStallIndex;						//��̨����
		UINT				m_uSerialSource;					//��Ʒ���к�
		UINT				m_uSerialDest;						//��Ʒ���к�
		BYTE				m_nShopSerial;						//�̵����к�

		GCExchangeItemFromPlayerShopToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerialSource		=	0;
			m_uSerialDest		=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCExchangeItemFromPlayerShopToPlayerShop_t);
		}
	};

	//ItemList
	struct GCPlayerShopItemListForSelf_t
	{
		enum
		{
			FOR_BUYER = 1,
			FOR_MANAGER,
		};
		BYTE					m_bFlag;								//��־
		_PLAYERSHOP_GUID		m_ShopGuid;								//�̵�GUID
		CHAR					m_ShopName[MAX_SHOP_NAME_SIZE];			//�̵���
		CHAR					m_ShopDesc[MAX_SHOP_DESC_SIZE];			//�̵�����
		GUID_t					m_OwnerGuid;							//����GUID
		CHAR					m_OwnerName[MAX_CHARACTER_NAME];		//������
		UINT					m_uBaseMoney;							//����
		UINT					m_uProfitMoney;							//Ӯ����
		BYTE					m_aStallIsOpen[MAX_STALL_NUM_PER_SHOP];	//�˹�̨�Ƿ���
		BYTE					m_bIsItemList;
		BYTE					m_bUIFlag;								//UI��Ҫ��һ����־
		BYTE					m_bIsSaleOut;							//�Ƿ����̳�״̬
		UINT					m_uSaleOutPrice;						//�̳���
		BYTE					m_Serial;								//�̵����к�
		BYTE					m_ExRecListNum;							//���׼�¼����
		BYTE					m_MaRecListNum;							//�����¼����
		BYTE					m_bStallNum;							//̯λ����
		BYTE					m_bType;								//�̵�����
		FLOAT					m_ComFactor;							//��ҵָ��
	
		GCPlayerShopItemListForSelf_t()
		{
			m_bFlag					=	0;
			m_uBaseMoney			=	0;
			m_uProfitMoney			=	0;
			m_OwnerGuid				=	0;
			m_bIsItemList			=	0;
			m_bUIFlag				=	0;
			m_bIsSaleOut			=	0;
			m_uSaleOutPrice			=	0;
			m_Serial				=	0;
			m_ExRecListNum			=	0;
			m_MaRecListNum			=	0;
			m_bStallNum				=	0;
			m_bType					=	0;
			m_ComFactor				=	0;
			memset(m_ShopName, 0, MAX_SHOP_NAME_SIZE);
			memset(m_ShopDesc, 0, MAX_SHOP_DESC_SIZE);
			memset(m_OwnerName, 0, MAX_CHARACTER_NAME);
			memset(m_aStallIsOpen, 0, MAX_STALL_NUM_PER_SHOP);
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListForSelf_t);
		}
	};

    struct GCPlayerShopItemListEachItemForSelf_t
	{
		BYTE	m_nStallIndex;
		UINT	m_ItemPrice;
		BYTE	m_ItemSerial;
		BYTE	m_IsMine;
		BYTE	m_IsOnSale;

		GCPlayerShopItemListEachItemForSelf_t()
		{
			m_nStallIndex = 0;
			m_ItemPrice = 0;
			m_ItemSerial = 0;
			m_IsMine = 0;
			m_IsOnSale = 0;
		}
		UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListEachItemForSelf_t);

		}
	};

	struct GCPlayerShopItemListEachSerialForSelf_t
	{
		BYTE	m_nStallIndex;
		BYTE	m_ItemSerial;

		GCPlayerShopItemListEachSerialForSelf_t()
		{
			m_nStallIndex	= 0;
			m_ItemSerial	= 0;
		}
		UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListEachSerialForSelf_t);

		}
	};

	struct GCPlayerShopPetListForSelf_t
	{
		BYTE					m_bUIFlag;						//UI��Ҫ��һ����־
	
		GCPlayerShopPetListForSelf_t()
		{
			m_bUIFlag		= 0;
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopPetListForSelf_t);
		}
	};

	struct GCPlayerShopItemListForOther_t
	{
		BYTE					m_bFlag;						//��־1.�Ǳ��˽��� 2.���˽���
		_PLAYERSHOP_GUID		m_ShopGuid;						//�̵�GUID
		CHAR					m_ShopName[MAX_SHOP_NAME_SIZE];	//�̵���
		CHAR					m_ShopDesc[MAX_SHOP_DESC_SIZE];	//����
		GUID_t					m_OwnerGuid;					//����GUID
		CHAR					m_OwnerName[MAX_CHARACTER_NAME];//������
		BYTE					m_aStallIsOpen[MAX_STALL_NUM_PER_SHOP];	//�˹�̨�Ƿ���
		BYTE					m_bIsItemList;
		BYTE					m_bUIFlag;						//UI��Ҫ��һ����־
		BYTE					m_bIsSaleOut;					//�˵��Ƿ��Ѿ��̳�
		UINT					m_uBaseMoney;					//����
		UINT					m_uProfitMoney;					//Ӯ����
		UINT					m_uSaleOutPrice;				//�̳��۸�
		BYTE					m_Serial;						//�̵����к�
		BYTE					m_bStallNum;					//ӵ�еĹ�̨��

		GCPlayerShopItemListForOther_t()
		{
			m_bFlag				= 0;
			m_OwnerGuid			= 0;
			m_bIsItemList		= 0;
			m_bUIFlag			= 0;
			m_bIsSaleOut		= 0;
			m_uBaseMoney		= 0;
			m_uProfitMoney		= 0;
			m_uSaleOutPrice		= 0;
			m_Serial			= 0;
			m_bStallNum		= 0;
			memset(m_ShopName, 0, MAX_SHOP_NAME_SIZE);
			memset(m_ShopDesc, 0, MAX_SHOP_DESC_SIZE);
			memset(m_OwnerName, 0, MAX_CHARACTER_NAME);
			memset(m_aStallIsOpen, 0, MAX_STALL_NUM_PER_SHOP);
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListForOther_t);
		}
	};

	struct GCPlayerShopItemListEachItemForOther_t
	{
		BYTE				m_nStallIndex;				//��̨����
		UINT				m_uPrice;					//��Ʒ�۸�
		BYTE				m_uSerial;					//��Ʒ���к�
		BYTE				m_bIsMine;					//�Ƿ����Լ�

		GCPlayerShopItemListEachItemForOther_t()
		{
			m_nStallIndex	=	0;				//��̨����
			m_uPrice		=	0;				//��Ʒ�۸�
			m_uSerial		=	0;				//��Ʒ���к�
			m_bIsMine		=	0;				//�Ƿ����Լ�
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListEachItemForOther_t);
		}
	};

	struct GCPlayerShopPetListForOther_t
	{
		BYTE					m_bUIFlag;				//UI  ��Ҫ��һ����־
	
		GCPlayerShopPetListForOther_t()
		{
			m_bUIFlag		= 0;
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopPetListForOther_t);
		}
	};




}

#endif
