/*
̯����̯����Ӷ���
*/

#include "stdafx.h"

#include "GCStallAddItem.h"
#include "GCStallError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"

uint GCStallAddItemHandler::Execute( GCStallAddItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_ITEM_GUID		ItemGuid	=	pPacket->GetObjGUID();
			PET_GUID_t		PetGuid		=	pPacket->GetPetGUID();
			UINT			nPrice		=	pPacket->GetPrice();
			BYTE			ToIndex		=	pPacket->GetToIndex();
			BYTE			FromType	=	pPacket->GetFromType();
			UINT			nSerial		=	pPacket->GetSerial();

			switch(FromType)
			{
			case STALL_MSG::POS_BAG :
				{
					//ת��GUID
					tObject_Item::ITEM_GUID tempitemGuid;
					tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
					tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
					tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

					//�����е�λ��
					UINT	IndexInBag = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(tempitemGuid.m_idUnion);
					if(IndexInBag<0)
						return PACKET_EXE_CONTINUE;

                    tObject_Item* pStallItem = CDataPool::GetMe()->UserBag_GetItem(IndexInBag);
					//���������ڵĶ�Ӧ����Ʒ
					if(pStallItem)
					{
						pStallItem->SetLock(TRUE);
					}

					//��¡��Ʒ
					tObject_Item* pNewItem  = CObject_Item::NewItem( pStallItem->GetIdTable());
					((CObject_Item*)pNewItem)->Clone((CObject_Item*)pStallItem);

					CDataPool::GetMe()->MyStallBox_SetItem(ToIndex, pNewItem);
					CDataPool::GetMe()->MyStallBox_SetItemPrice(ToIndex, nPrice);
					CDataPool::GetMe()->MyStallBox_SetItemSerial(ToIndex, nSerial);

					//uiˢ��
					CActionSystem::GetMe()->UserStallSale_Update();
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);

					//�򿪳����б����
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_LIST,0);

					//���±���
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			case STALL_MSG::POS_EQUIP :
				{
				}
				break;
			case STALL_MSG::POS_PET :
				{//���Գ�����
					//INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
					////��ס���λ�õĳ���
					//SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(IndexInBag);
					//CDataPool::GetMe()->MyStallBox_SetPet(ToIndex, pPetData, FALSE);

					//CDataPool::GetMe()->MyStallBox_SetPetSerial(ToIndex, nSerial);
					//CDataPool::GetMe()->MyStallBox_SetPetPrice(ToIndex, nPrice);

					////ˢ�½���,��ס�������еĳ���
					//CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);
					//CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_LIST);
					
				}
				break;
			default:
				break;
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
