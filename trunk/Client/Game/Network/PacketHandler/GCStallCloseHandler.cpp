/*
������֪ͨ�ͻ��˹ر�̯λ
*/
#include "stdafx.h"
#include "GCStallClose.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "GIException.h"


uint GCStallCloseHandler::Execute( GCStallClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//ȡ�������ж�Ӧ��Ʒ������״̬
			//��Ҫ���������Ӧ��Ʒ������(ע�⣬��¡����ƷGUID��ͬ�����Կ�����������)
			for(INT i=0; i<STALL_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyStallBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyStallBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//ȥ������
						pBagItem->SetLock(FALSE);
					}
				}
			}


			//���̯λ
			CDataPool::GetMe()->MyStallBox_Clear();

			//�����Ϣ
			CDataPool::GetMe()->MyStallMsg_ClearUp();

			//����ui���ر�StallMessage����
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
			//uiˢ��
			CActionSystem::GetMe()->UserStallSale_Update();

			//ˢ�±�������
			CActionSystem::GetMe()->UserBag_Update();
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			// add by gh 2010/04/15 �ر�InputMoney
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_SALE);	
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
