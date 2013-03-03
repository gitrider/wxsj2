//GCSplitItemResult.h

#include "StdAfx.h"
#include "GCSplitItemResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"



uint GCSplitItemResultHandler::Execute( GCSplitItemResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//��������ɺ�Ľ����ʾ
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//�ɹ����
		if( GCSplitItemResult::RESULT_SUCCEED == pPacket->getResult())
		{
			//��Ҫ������ֵ���Ʒ�Ͳ�����ɵ���Ʒ
			INT nCon = pPacket->getContainer();
			switch( nCon )
			{
			case GCSplitItemResult::BAG_CON:
				{
					// �����µ���Ʒ
					WORD wBagIndex = pPacket->getConIndex();
					_ITEM* pItem = pPacket->getItem();

					CObject_Item* pItemObj = CObject_Item::NewItem(pItem->m_ItemIndex);
					if(!pItemObj) 
						return PACKET_EXE_CONTINUE;

					pItemObj->SetGUID(
						pItem->m_ItemGUID.m_World, 
						pItem->m_ItemGUID.m_Server,
						pItem->m_ItemGUID.m_Serial);

					pItemObj->SetExtraInfo(pItem);

					CDataPool::GetMe()->UserBag_SetItem(wBagIndex, pItemObj );
					CActionSystem::GetMe()->UserBag_Update();
			
					//�޸ı���ֵ���Ʒ
					
				}
				break;
			case GCSplitItemResult::BANK_CON:
				{
					WORD wBankIndex = pPacket->getConIndex();
					_ITEM* pItem = pPacket->getItem();

					CObject_Item* pItemObj = CObject_Item::NewItem(pItem->m_ItemIndex);
					if(!pItemObj) 
						return PACKET_EXE_CONTINUE;

					pItemObj->SetGUID(
						pItem->m_ItemGUID.m_World, 
						pItem->m_ItemGUID.m_Server,
						pItem->m_ItemGUID.m_Serial);

					pItemObj->SetExtraInfo(pItem);

					CDataPool::GetMe()->UserBank_SetItem(wBankIndex, pItemObj );
					CActionSystem::GetMe()->UserBank_Update();

					//�޸ı���ֵ���Ʒ
					
				}
				break;
			default:
				break;
			}
		}
		//���ʧ�ܣ�û�пո������ɲ�ֺ����Ʒ
		else if( GCSplitItemResult::RESULT_FALSE_NOGRID == pPacket->getResult() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCSplititemResultHandler_Info_Can_Not_Splite");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����������������ɲ��");
		}
		//���ʧ�ܣ���ֹ����ϵ�ԭ��
		else
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCSplititemResultHandler_Info_Splite_Err");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ʧ��");
		}

		//����������Ʒ������
		INT nTypeOwner	= CDataPool::GetMe()->Split_GetItemConta();
		INT nItemPos	= CDataPool::GetMe()->Split_GetItemPos();
		switch( nTypeOwner ) 
		{
		case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
			{
				tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

				if(pItem)
				{
					pItem->SetLock(FALSE);
				}
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			}
			break;
		case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
			{
				tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

				if(pItem)
				{
					pItem->SetLock(FALSE);
				}
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
			}
			break;
		default:
			break;
		}
		CDataPool::GetMe()->Split_SetItemConta(-1);
		CDataPool::GetMe()->Split_SetItemPos(-1);
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
