#include"stdafx.h"

#include "GCBankAddItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"
uint GCBankAddItemHandler::Execute( GCBankAddItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE	indexTo = pPacket->GetIndexTo();
			BYTE	indexFrom = pPacket->GetIndexFrom();

			if(pPacket->GetFromType() == GCBankAddItem::EQUIP_POS)
			{

			}
			else if(pPacket->GetFromType() == GCBankAddItem::BAG_POS)
			{
				tObject_Item*	pItemFrom = CDataPool::GetMe()->UserBag_GetItem(indexFrom);
				tObject_Item*	pItemTo = CDataPool::GetMe()->UserBank_GetItem(indexTo);

				switch(pPacket->GetOperateType())
				{
				case GCBankAddItem::OPERATE_MOVE:		// �ƶ����ո�
					{
						CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom);
						CDataPool::GetMe()->UserBag_SetItem(indexFrom, NULL, FALSE);
					}
					break;
				case GCBankAddItem::OPERATE_SPLICE:		// �ϲ�
					{
						//CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom);
						//CDataPool::GetMe()->UserBag_SetItem(indexFrom, NULL, FALSE);
					}
					break;
				case GCBankAddItem::OPERATE_SWAP:		// ����
					{
						CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom, FALSE);
						CDataPool::GetMe()->UserBag_SetItem(indexFrom, pItemTo, FALSE);
					}
					break;
				default:
					break;
				}
				CActionSystem::GetMe()->UserBank_Update();
				CActionSystem::GetMe()->UserBag_Update();

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCBankAddItemHandler_Info_Item_Save_To_Bank_Ok");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ�������гɹ�");

				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
			}
			else if(pPacket->GetFromType() == GCBankAddItem::ERROR_POS)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCBankAddItemHandler_Info_Item_Save_To_Bank_False");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ��������ʧ��");
			}

		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
