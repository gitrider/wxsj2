
#include "stdafx.h"
#include "GCStallItemPrice.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"

uint GCStallItemPriceHandler::Execute( GCStallItemPrice* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_ITEM_GUID		ItemGuid	=	pPacket->GetObjGUID();
			PET_GUID_t		PetGuid		=	pPacket->GetPetGUID();
			UINT			nPrice		=	pPacket->GetPrice();
			UINT			nSerial		=	pPacket->GetSerial();

			if(PetGuid.IsNull())
			{//��Ʒ�۸���趨
				//ת��GUID
				tObject_Item::ITEM_GUID tempitemGuid;
				tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
				tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
				tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

				//�����е�λ��
				INT	unIndexInStall = CDataPool::GetMe()->MyStallBox_GetItemIndexByGUID(tempitemGuid.m_idUnion);
				if(unIndexInStall<0)
					return PACKET_EXE_CONTINUE;

				CDataPool::GetMe()->MyStallBox_SetItemPrice(unIndexInStall,nPrice);
				CDataPool::GetMe()->MyStallBox_SetItemSerial(unIndexInStall,nSerial);

				//������Ʒ�۸�
				CActionSystem::GetMe()->UserStallSale_Update();
				//֪ͨ����
				CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,(INT)unIndexInStall);
			}
			else
			{//����۸���趨
				INT	IndexInStall = CDataPool::GetMe()->MyStallBox_GetPetByGuid(PetGuid);
				if(IndexInStall<0)
					return PACKET_EXE_CONTINUE;

				CDataPool::GetMe()->MyStallBox_SetPetPrice(IndexInStall, nPrice);
				CDataPool::GetMe()->MyStallBox_SetPetSerial(IndexInStall, nSerial);
				//ˢ�½���

				CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,(INT)IndexInStall);
			}
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
