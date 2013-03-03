#include"stdafx.h"

#include "GCBankRemoveItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"
uint GCBankRemoveItemHandler::Execute( GCBankRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE	indexTo = pPacket->GetIndexTo();
			BYTE	indexFrom = pPacket->GetIndexFrom();

			if(pPacket->GetToType() == GCBankRemoveItem::EQUIP_POS)
			{

			}
			else if(pPacket->GetToType() == GCBankRemoveItem::BAG_POS)
			{
				switch( pPacket->GetOperateType() )
				{
				case GCBankRemoveItem::OPERATE_MOVE:		// �ƶ���Ʒ���ո�
					{
						tObject_Item*	pItem = CDataPool::GetMe()->UserBank_GetItem(indexFrom);
						CDataPool::GetMe()->UserBag_SetItem(indexTo, pItem);
						CDataPool::GetMe()->UserBank_SetItem(indexFrom, NULL, FALSE);
						CActionSystem::GetMe()->UserBank_Update();
						CActionSystem::GetMe()->UserBag_Update();

						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GCBankRemoveItemhandler_Info_Load_Item_Ok");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒȡ�����гɹ�");


						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
					}
					break;
				case GCBankRemoveItem::OPERATE_SPLICE:		// ������Ʒ
					{
						
					}
					break;
				case GCBankRemoveItem::OPERATE_SPLIT:		// �����Ʒ
					{
						
					}
					break;
				case GCBankRemoveItem::OPERATE_SWAP:		// ������Ʒ
					{
						tObject_Item*	pItem  = CDataPool::GetMe()->UserBank_GetItem(indexFrom);
						tObject_Item*	pItem1 = CDataPool::GetMe()->UserBag_GetItem(indexTo);
						CDataPool::GetMe()->UserBag_SetItem(indexTo, pItem, FALSE);
						CDataPool::GetMe()->UserBank_SetItem(indexFrom, pItem1, FALSE);
						CActionSystem::GetMe()->UserBank_Update();
						CActionSystem::GetMe()->UserBag_Update();

						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GCBankRemoveItemhandler_Info_Exchange_Item_Bank_pag_Ok");
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
						//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���кͱ���������Ʒ�ɹ�");

						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
					}
					break;
				default:
					break;
				}

			}
			else if(pPacket->GetToType() == GCBankRemoveItem::ERROR_POS)
			{
				
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCBankRemoveItemhandler_Info_Move_Item_Fail");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�ƶ���Ʒʧ��");
			}
		}


		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
