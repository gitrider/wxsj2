#include "StdAfx.h"
#include "GCPlayerShopUpdatePartners.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Action\GMActionSystem.h"
#include "GIException.h"

UINT GCPlayerShopUpdatePartnersHandler::Execute( GCPlayerShopUpdatePartners* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID					nShopID			=	pPacket->GetShopID();		//�̵�ID
			BYTE								nPartnerNum		=	pPacket->GetPartnerNum();								//
			GCPlayerShopUpdatePartners::MsgPartner_t*		pPartnerList	=	pPacket->GetPartnerList();		//

			for(INT i = 0; i<nPartnerNum; i++)
			{
				CDataPool::GetMe()->PlayerShop_SetPartnerByIndex(i, pPartnerList[i].m_Guid, pPartnerList[i].m_szName);
			}
			CDataPool::GetMe()->PlayerShop_SetPartnerNum( nPartnerNum );
		}

		//֪ͨ�̵����
		CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
