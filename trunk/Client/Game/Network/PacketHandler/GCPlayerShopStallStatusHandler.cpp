#include "StdAfx.h"
#include "GCPlayerShopStallStatus.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Action\GMActionSystem.h"
#include "GIException.h"

UINT GCPlayerShopStallStatusHandler::Execute( GCPlayerShopStallStatus* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID	nShopID		=	pPacket->GetShopID();			//�̵�ID
			BYTE				nStallIndex	=	pPacket->GetStallIndex();		//��̨����
			BYTE				bStatus		=	pPacket->GetStatus();			//��̨״̬
			BYTE				FinalStallNum=	pPacket->GetFinalStallNum();	//��̨����

			CDataPool::GetMe()->PlayerShop_SetStallEnableByIndex( TRUE, nStallIndex, bStatus);
			CDataPool::GetMe()->PlayerShop_SetOpenStallNum(TRUE, FinalStallNum);
		}

		//֪ͨ�̵����
		CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
