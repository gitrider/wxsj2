#include "StdAfx.h"
#include "GCItemInfo.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"

uint GCItemInfoHandler::Execute(GCItemInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	// ��Ʒ����ϸ��Ϣ
	//AxTrace(0, 2, "GCItemInfoHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		if( pPacket->getOpType() ==  Packets::GCItemInfo::OPERATOR_BAG )
		{
			CDataPool::GetMe()->UserBag_SetItemInfo(pPacket->getID(), pPacket->getIsNull(), pPacket->getItem());

			CActionSystem::GetMe()->UserBag_Update();
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED,pPacket->getID());

			// �õ���ϸ��Ϣ�� ����surpper tooltip.
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
		}
		else if( pPacket->getOpType() ==  Packets::GCItemInfo::OPERATOR_BUY_BACK )
		{
			// �ع������� ��������Ϣ����
		}
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}