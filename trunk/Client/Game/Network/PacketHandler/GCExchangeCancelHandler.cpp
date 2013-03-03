/*
�յ�����Ϣ�������˵Ľ��׺������Ѿ������
*/

#include "stdafx.h"
#include "GCExchangeCancel.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Logic\Item\Obj_Item.h"

#include "GIException.h"
uint GCExchangeCancelHandler::Execute( GCExchangeCancel* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//��Ҫ���������Ӧ��Ʒ������(ע�⣬��¡����ƷGUID��ͬ�����Կ�����������)
			for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyExBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyExBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//ȥ������
						pBagItem->SetLock(FALSE);
					}
				}
			}

			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear();

			//����Action
			CActionSystem::GetMe()->UserExchangeOther_Update();
			CActionSystem::GetMe()->UserExchangeSelf_Update();
			CActionSystem::GetMe()->UserBag_Update();

			//���±���
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			//�رս���
			CEventSystem::GetMe()->PushEvent(GE_SUCCEED_EXCHANGE_CLOSE);
			//�����Ѿ���ȡ��

			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCExchangeCancelHandler_Info_Cancel_Exchange");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����Ѿ���ȡ��");
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
