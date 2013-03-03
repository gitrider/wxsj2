
#include "stdafx.h"
#include "GCBankSwapItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "GIException.h"
uint GCBankSwapItemHandler::Execute( GCBankSwapItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE	indexTo = pPacket->GetIndexTo();
			BYTE	indexFrom = pPacket->GetIndexFrom();

			if(pPacket->GetFromType() == GCBankSwapItem::ERROR_POS
				|| pPacket->GetToType() == GCBankSwapItem::ERROR_POS
				)
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCBankSwapItemHandler_Info_Err_Command");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ�����");
				return PACKET_EXE_CONTINUE ;
			}


			switch(pPacket->GetFromType())
			{
			case GCBankSwapItem::EQUIP_POS:
				{
					switch(pPacket->GetToType())
					{
					case GCBankSwapItem::BAG_POS:
						{
						}
						break;
					case GCBankSwapItem::EQUIP_POS:
						{
						}
						break;
					case GCBankSwapItem::BANK_POS:
						{
						}
						break;
					default:
						break;
					}
				}
				break;
			case GCBankSwapItem::BAG_POS:
				{
					switch(pPacket->GetToType())
					{
					case GCBankSwapItem::BAG_POS:
						{
						}
						break;
					case GCBankSwapItem::EQUIP_POS:
						{
						}
						break;
					case GCBankSwapItem::BANK_POS:
						{
							tObject_Item*	pItemFrom = CDataPool::GetMe()->UserBag_GetItem(indexFrom);
							tObject_Item*	pItemTo = CDataPool::GetMe()->UserBank_GetItem(indexTo);

							AxTrace(0, 0, "BankSwapItem:: pItemFrom %s, indexFrom %d, GUID=%u", pItemFrom->GetName(), indexFrom, pItemFrom->GetGUID());
							AxTrace(0, 0, "BankSwapItem:: pItemTo %s, indexTo %d, GUID=%u", pItemTo->GetName(), indexTo, pItemTo->GetGUID());

							CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom, FALSE);
							CDataPool::GetMe()->UserBag_SetItem(indexFrom, pItemTo, FALSE);
							CActionSystem::GetMe()->UserBank_Update();
							CActionSystem::GetMe()->UserBag_Update();

							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GCBankSwapItemHandler_Info_Item_pag_EXchange_Bank_Ok");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������������Ʒ�����ɹ�");

							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
						}
						break;
					default:
						break;
					}
				}
				break;
			case GCBankSwapItem::BANK_POS:
				{
					switch(pPacket->GetToType())
					{
					case GCBankSwapItem::BAG_POS:
						{
							tObject_Item*	pItemFrom = CDataPool::GetMe()->UserBank_GetItem(indexFrom);
							tObject_Item*	pItemTo = CDataPool::GetMe()->UserBag_GetItem(indexTo);

							CDataPool::GetMe()->UserBag_SetItem(indexTo, pItemFrom, FALSE);
							CDataPool::GetMe()->UserBank_SetItem(indexFrom, pItemTo, FALSE);
							CActionSystem::GetMe()->UserBank_Update();
							CActionSystem::GetMe()->UserBag_Update();

							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GCBankSwapItemHandler_Info_Item_Bank_EXchange_Pag_Ok");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���е�������Ʒ�����ɹ�");

							CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
						}
						break;
					case GCBankSwapItem::EQUIP_POS:
						{
						}
						break;
					case GCBankSwapItem::BANK_POS:
						{
							tObject_Item*	pItemFrom = CDataPool::GetMe()->UserBank_GetItem(indexFrom);
							tObject_Item*	pItemTo = CDataPool::GetMe()->UserBank_GetItem(indexTo);

							CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom, FALSE);
							CDataPool::GetMe()->UserBank_SetItem(indexFrom, pItemTo, FALSE);
							CActionSystem::GetMe()->UserBank_Update();

							STRING strTemp = "";
							strTemp = NOCOLORMSGFUNC("GCBankSwapItemHandler_Info_Item_Bank_MoveTo_Bank_Ok");
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
							//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���е�������Ʒ��λ�ɹ�");

							CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
						}
						break;
					default:
						break;
					}
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
