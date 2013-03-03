// GCShopSoldListHandler.cpp
#include "stdafx.h"
#include "GCShopSoldList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCShopSoldListHandler::Execute( GCShopSoldList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//�Ѿ������˾�����
		//if(CDataPool::GetMe()->Booth_IsClose())
		//	return PACKET_EXE_CONTINUE ;

		INT nNum = pPacket->GetMerchadiseNum();
		if(nNum == 0)
		{//�յ�û����
			if(CDataPool::GetMe()->Booth_GetSoldNumber() != 0)
			{
				CDataPool::GetMe()->Booth_Sold_Clear();
			}
		}
		else
		{
			//���ԭ����Ʒ�б�
			CDataPool::GetMe()->Booth_Sold_Clear();

			CSoundSystemFMod::_PlayUISoundFunc(25+59);

			//��ӵ����ݳ���
			for(INT i=0; i<nNum; i++)
			{
				GCShopSoldList::_MERCHANDISE_ITEM* pNewItem = &(pPacket->GetMerchadiseList()[i]);
				tObject_Item* pItem = NULL;

				pItem = CObject_Item::NewItem( pNewItem->item_data.m_ItemIndex);
				if( pItem == NULL )
					continue;

				pItem->SetGUID(
					pNewItem->item_data.m_ItemGUID.m_World, 
					pNewItem->item_data.m_ItemGUID.m_Server, 
					pNewItem->item_data.m_ItemGUID.m_Serial);

				pItem->SetExtraInfo(&(pNewItem->item_data));

				KLAssert(pItem);
				
				//���ø���Ʒ�ڻ����ϵ�λ����Ϊ�ǻع���Ʒ����λ��Ӧ����200
				pItem->SetPosIndex(pNewItem->iIndex);

				//���ø���Ʒ��������ÿһ���������
				pItem->SetNumber(pNewItem->item_data.GetItemCount());

				//�ع���Ʒ��Ȼֻ��һ��
				pItem->SetMax(1);

				//�������ݳ�
				CDataPool::GetMe()->Booth_SetSoldItem(i, pItem);
			}
			//���ûع���Ʒ������
			CDataPool::GetMe()->Booth_SetSoldNumber(nNum);
		}
		
		//���µ�ActionSystem
		CActionSystem::GetMe()->Booth_Update();
		//֪ͨUI
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_BOOTH);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
