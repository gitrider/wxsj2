#include "StdAfx.h"
#include "GCPlayerShopMoney.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Action\GMActionSystem.h"
#include "GIException.h"

UINT GCPlayerShopMoneyHandler::Execute( GCPlayerShopMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID	nShopID	=	pPacket->GetShopID();		//�̵�ID
			BYTE	nType	=	pPacket->GetType();						//�浽��
			UINT	uAmount	=	pPacket->GetAmount();					//����
		
			if(nType == GCPlayerShopMoney::TYPE_BASE_MONEY)
			{
				CDataPool::GetMe()->PlayerShop_SetBaseMoney(TRUE, uAmount);
			}
			else if(nType == GCPlayerShopMoney::TYPE_PROFIT_MONEY)
			{
				CDataPool::GetMe()->PlayerShop_SetProfitMoney(TRUE, uAmount);
			}
		}

		//֪ͨ�̵����
		CEventSystem::GetMe()->PushEvent(GE_PS_SELF_ITEM_CHANGED);

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
