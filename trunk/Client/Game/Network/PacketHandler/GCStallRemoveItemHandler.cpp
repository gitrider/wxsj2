/*
֪ͨ�ͻ���ɾ����̯λ�е�ĳ����Ʒ
*/

#include "stdafx.h"
#include "GCStallRemoveItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"

#include "GIException.h"

uint GCStallRemoveItemHandler::Execute( GCStallRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_ITEM_GUID	ItemGuid	=	pPacket->GetObjGUID();
			PET_GUID_t	PetGuid		=	pPacket->GetPetGUID();
			UINT		Serial		=	pPacket->GetSerial();
			BYTE		ToType		=	pPacket->GetToType();

			switch(ToType)
			{
			case STALL_MSG::POS_BAG:
				{
					//ת��GUID
					tObject_Item::ITEM_GUID tempitemGuid;
					tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
					tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
					tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

					//̯λ�е�λ��
					UINT	IndexInStall = CDataPool::GetMe()->MyStallBox_GetItemIndexByGUID(tempitemGuid.m_idUnion);
					if(IndexInStall<0)
						return PACKET_EXE_CONTINUE;
					
					//�����е�λ��
					UINT	IndexInBag = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(tempitemGuid.m_idUnion);
					if(IndexInBag<0)
						return PACKET_EXE_CONTINUE;

					//��������е���Ʒ����
					if(CDataPool::GetMe()->UserBag_GetItem(IndexInBag))
					{
						CDataPool::GetMe()->UserBag_GetItem(IndexInBag)->SetLock(FALSE);
					}

					//ɾ��̯λ�д�λ���ϵĶ���
					CDataPool::GetMe()->MyStallBox_SetItem(IndexInStall, NULL);

					//��λ�����кŸı�
					CDataPool::GetMe()->MyStallBox_SetItemSerial(IndexInStall, Serial);

					//���½���
					CActionSystem::GetMe()->UserStallSale_Update();
					//����ActionSystem
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);
					//���±���
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			case STALL_MSG::POS_EQUIP:
				{
				}
				break;
			case STALL_MSG::POS_PET:
				{
					INT IndexInStallBox = CDataPool::GetMe()->MyStallBox_GetPetByGuid(PetGuid);
					CDataPool::GetMe()->MyStallBox_SetPet(IndexInStallBox, NULL, FALSE);
					CDataPool::GetMe()->MyStallBox_SetPetSerial(IndexInStallBox, Serial);
					INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
					//�������λ�õĳ���
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_LIST);
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
