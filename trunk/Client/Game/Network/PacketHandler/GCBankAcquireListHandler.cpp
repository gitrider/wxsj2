#include"stdafx.h"

#include "GCBankAcquireList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"

uint GCBankAcquireListHandler::Execute( GCBankAcquireList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//���ԭ����Ʒ�б�
			CDataPool::GetMe()->UserBank_Clear();

			//���������Ǯ��
			INT	Money = pPacket->GetBankMoney();
			CDataPool::GetMe()->UserBank_SetBankMoney(Money);

			//��ǰ���еĴ�С
			INT	BankSize = pPacket->GetBankSize();
			CDataPool::GetMe()->UserBank_SetBankEndIndex(BankSize);

			GCBankAcquireList::_BANK_ITEM*	ItemList = pPacket->GetBankItemList();
			INT nNum = pPacket->GetItemNum();
			INT bankpos = 0;
			tObject_Item* pItem = NULL;

			//��ӵ����ݳ���
			for(INT i=0; i<nNum; i++)
			{
				bankpos = ItemList[i].bankindex;

				//remove
			/*	if(ItemList[i].isBlueEquip)
				{
					pItem = CObject_Item::NewItem( ItemList[i].item_data.m_ItemIndex);
					KLAssert(pItem);
					pItem->SetGUID(
						ItemList[i].item_data.m_ItemGUID.m_World, 
						ItemList[i].item_data.m_ItemGUID.m_Server,
						ItemList[i].item_data.m_ItemGUID.m_Serial);
					CDataPool::GetMe()->UserBank_SetItem(bankpos, pItem);
					CDataPool::GetMe()->UserBank_SetItemExtraInfo(bankpos, FALSE, &ItemList[i].item_data);
				}
				else*/
				//{
					pItem = CObject_Item::NewItem( ItemList[i].item_data.m_ItemIndex);
					KLAssert(pItem);
					CDataPool::GetMe()->UserBank_SetItem(bankpos, pItem);
				//}
				pItem->SetPosIndex(bankpos);
				pItem->SetNumber(ItemList[i].byNumber);
			}
			
			CActionSystem::GetMe()->UserBank_Update();
			INT nBankNpcId = CDataPool::GetMe()->UserBank_GetNpcId();
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_BANK,nBankNpcId);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
