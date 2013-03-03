#include "stdafx.h"

#include "GCStallOpen.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"

uint GCStallOpenHandler::Execute( GCStallOpen* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

		ObjID_t	OwnerObjId	=	pPacket->GetObjID();
		BYTE	ItemNum		=	pPacket->GetMerchadiseNum();

		//���ԭ����Ʒ�б�
		if(OwnerObjId == CObjectManager::GetMe()->GetMySelf()->GetServerID())
		{//���Լ���̯λ

			//���̯λ
			CDataPool::GetMe()->MyStallBox_Clear();

			//��ʼ���Լ�̯λ
			CDataPool::GetMe()->MyStallBox_SetIsOpen(TRUE);//����̯��ID
			CDataPool::GetMe()->MyStallBox_SetObjID(OwnerObjId);
			CDataPool::GetMe()->MyStallBox_SetStallName(pPacket->GetShopName()); //����̯λ��
			//CDataPool::GetMe()->MyStallBox_SetOwnerName(pChar->); //����̯����
			CDataPool::GetMe()->MyStallBox_SetFirstPage(pPacket->GetFirstPage());//������ҳ
			CDataPool::GetMe()->MyStallBox_SetOwnerGUID(pPacket->GetGUID());
			CDataPool::GetMe()->MyStallBox_SetDefaultPage(pPacket->GetFirstPage());

			//ÿ����Ʒ��ӵ����ݳ���
			for(INT i=0; i<ItemNum; i++)
			{

				GCStallOpen::_STALL_ITEM* pNewItem = &(pPacket->GetMerchadiseList()[i]);

				if(pNewItem->bIsPet == TRUE)
				{//�Լ�������ĳ���
					if(pNewItem->PetGuid.IsNull())
					{//�˳���GUID��Ч��ֻ�����к�ˢ����
						CDataPool::GetMe()->MyStallBox_SetPet(pNewItem->nIndex, NULL, FALSE);
						CDataPool::GetMe()->MyStallBox_SetPetPrice(pNewItem->nIndex, pNewItem->nPrice);
						CDataPool::GetMe()->MyStallBox_SetPetSerial(pNewItem->nIndex, pNewItem->nSerial);
					}
					else
					{//��������к�һ��ˢ����
						//INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(pNewItem->PetGuid);
						////��ס���λ�õĳ���
						//SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(IndexInBag);
						////��֤��������ǲ��Ǳ���ס��
						//Assert(pPetData->m_nIsPresent == 2);
						//CDataPool::GetMe()->MyStallBox_SetPet(pNewItem->nIndex, pPetData, FALSE);
						//CDataPool::GetMe()->MyStallBox_SetPetPrice(pNewItem->nIndex, pNewItem->nPrice);
						//CDataPool::GetMe()->MyStallBox_SetPetSerial(pNewItem->nIndex, pNewItem->nSerial);
					}
				}
				else
				{
					if(pNewItem->item.IsNullType())
					{//û����Ʒֻ�����к�ˢ����
						CDataPool::GetMe()->MyStallBox_SetItem(pNewItem->nIndex, NULL);
						CDataPool::GetMe()->MyStallBox_SetItemPrice(pNewItem->nIndex, pNewItem->nPrice);//����̯λ��Ʒ�۸�
						CDataPool::GetMe()->MyStallBox_SetItemSerial(pNewItem->nIndex, pNewItem->nSerial);//����̯λ��Ʒ���к�
					}
					else
					{//��Ʒ�����к�һ��ˢ����
						//������Ʒʵ��
						tObject_Item* pItem = CObject_Item::NewItem( pNewItem->item.m_ItemIndex);
						if(!pItem) continue;

						pItem->SetGUID(
							pNewItem->item.m_ItemGUID.m_World, 
							pNewItem->item.m_ItemGUID.m_Server, 
							pNewItem->item.m_ItemGUID.m_Serial);
						KLAssert(pItem);

						//�ŵ����ܵ��ƶ�λ����
						CDataPool::GetMe()->MyStallBox_SetItem(pNewItem->nIndex, pItem);
						CDataPool::GetMe()->MyStallBox_SetItemExtraInfo(pNewItem->nIndex, FALSE, &(pNewItem->item));
						CDataPool::GetMe()->MyStallBox_SetItemPrice(pNewItem->nIndex, pNewItem->nPrice);//����̯λ��Ʒ�۸�
						CDataPool::GetMe()->MyStallBox_SetItemSerial(pNewItem->nIndex, pNewItem->nSerial);//����̯λ��Ʒ���к�
					}
				}
			}

			// ���ʹ��Լ�̯λ���¼�
			CActionSystem::GetMe()->UserStallSale_Update();
			CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_SALE);
			//�򿪳����б����
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST,0);
		}
		else
		{//���˵�̯λ
			//���Ŀ��̯λ
			CDataPool::GetMe()->OtStallBox_Clear(TRUE);

			//��ʼ��Ŀ��̯λ
			CDataPool::GetMe()->OtStallBox_SetIsOpen(TRUE);//����̯��ID
			CDataPool::GetMe()->OtStallBox_SetObjID(OwnerObjId);
			CDataPool::GetMe()->OtStallBox_SetStallName(pPacket->GetShopName()); //����̯λ��
			//CDataPool::GetMe()->MyStallBox_SetOwnerName(pPacket->Get); //����̯����
			CDataPool::GetMe()->OtStallBox_SetFirstPage(pPacket->GetFirstPage());//������ҳ
			CDataPool::GetMe()->OtStallBox_SetOwnerGUID(pPacket->GetGUID());

			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(OwnerObjId);

			LPCSTR szName = ((CObject_Character*)pObj)->GetCharacterData()->Get_Name();
			CDataPool::GetMe()->OtStallBox_SetOwnerName(szName);
			CDataPool::GetMe()->OtStallBox_SetDefaultPage(pPacket->GetFirstPage());
			
			//ÿ����Ʒ��ӵ����ݳ���
			for(INT i=0; i<ItemNum; i++)
			{
				GCStallOpen::_STALL_ITEM* pNewItem = &(pPacket->GetMerchadiseList()[i]);
				//������Ʒʵ��
				tObject_Item* pItem = CObject_Item::NewItem( pNewItem->item.m_ItemIndex);
				if(!pItem) continue;

				pItem->SetGUID(
					pNewItem->item.m_ItemGUID.m_World, 
					pNewItem->item.m_ItemGUID.m_Server, 
					pNewItem->item.m_ItemGUID.m_Serial);
				KLAssert(pItem);

				//�ŵ����ܵ��ƶ�λ����
				CDataPool::GetMe()->OtStallBox_SetItem(pNewItem->nIndex, pItem);
				CDataPool::GetMe()->OtStallBox_SetItemExtraInfo(pNewItem->nIndex, FALSE, &(pNewItem->item));
				CDataPool::GetMe()->OtStallBox_SetItemPrice(pNewItem->nIndex, pNewItem->nPrice);//����̯λ��Ʒ�۸�
				CDataPool::GetMe()->OtStallBox_SetItemSerial(pNewItem->nIndex, pNewItem->nSerial);//����̯λ��Ʒ���к�
			}

			// ���ʹ򿪱���̯λ���¼�
			CActionSystem::GetMe()->UserStallBuy_Update();

			INT nObjId = CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID())->GetID();
			CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_BUY,nObjId);
		}
	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
