#include "StdAfx.h"
#include "GCPlayerShopOnSale.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Action\GMActionSystem.h"
#include "GIException.h"

UINT GCPlayerShopOnSaleHandler::Execute( GCPlayerShopOnSale* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID nShopID			=	pPacket->GetShopID();			//�̵�ID
			BYTE		nStallIndex		=	pPacket->GetStallIndex();		//��̨ID
			_ITEM_GUID	ItemGuid		=	pPacket->GetObjGUID();			//��ƷGUID
			PET_GUID_t	PetGuid			=	pPacket->GetPetGuid();			//����Guid
			UINT		nSerial			=	pPacket->GetSerial();			//��ǰ���к�
			BYTE		bIsOnSale		=	pPacket->GetIsOnSale();			//�Ƿ��ϼ�
			UINT		uPrice			=	pPacket->GetPrice();			//��Ʒ�۸�

			INT			nType			=	0;//0��ʾ������1��ʾ������Ʒ
			if(PetGuid.IsNull())
			{
				//��ʾ�����Ǵ�����Ʒ
				nType = 1;
			}


			if(nType == 1)
			{
				//ת��GUID
				tObject_Item::ITEM_GUID tempitemGuid;
				tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
				tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
				tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

				//���̵����ҵ�����Ʒ
				INT	nIndexInShop = CDataPool::GetMe()->PlayerShop_GetItemIndexByGUID(TRUE, nStallIndex, tempitemGuid.m_idUnion);
				if(nIndexInShop<0)
				{
					Assert(0);
					return PACKET_EXE_CONTINUE ;
				}
				if(bIsOnSale)
				{//�ϼ�
					CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nIndexInShop, nSerial);
					CDataPool::GetMe()->PlayerShop_SetItemPrice(TRUE, nStallIndex, nIndexInShop, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndexInShop, TRUE);
				}
				else
				{
					CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nIndexInShop, nSerial);
					CDataPool::GetMe()->PlayerShop_SetItemPrice(TRUE, nStallIndex, nIndexInShop, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndexInShop, FALSE);
				}

				CActionSystem::GetMe()->UserPS_Self_Update();
				CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
			}
			else
			{
				//���̵����ҵ��ó���
				INT	nIndex = CDataPool::GetMe()->PlayerShop_GetPetByGuid(TRUE, nStallIndex, PetGuid);
				if(nIndex<0)
				{
					Assert(0);
					return PACKET_EXE_CONTINUE ;
				}
				if(bIsOnSale)
				{//�ϼ�
					CDataPool::GetMe()->PlayerShop_SetPetSerial(TRUE, nStallIndex, nIndex, nSerial);
					CDataPool::GetMe()->PlayerShop_SetPetPrice(TRUE, nStallIndex, nIndex, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndex, TRUE);
				}
				else
				{
					CDataPool::GetMe()->PlayerShop_SetPetSerial(TRUE, nStallIndex, nIndex, nSerial);
					CDataPool::GetMe()->PlayerShop_SetPetPrice(TRUE, nStallIndex, nIndex, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndex, FALSE);
				}

				CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
