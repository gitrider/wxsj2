#include "StdAfx.h"
#include "GCNewPortal.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\TripperObj\GMTripperObj_Transport.h"
#include "Object\ObjectCommandDef.h"
#include "GCCharJump.h"

#include "..\..\World\WorldManager.h"
#include "GIException.h"

uint GCNewPortalHandler::Execute( GCNewPortal* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewPortalHandler::Execute");
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		//���λ���Ƿ�Ϸ�
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCNewPortalHandler");
		}

		//����ItemBox
		CTripperObject_Transport* pTransport = (CTripperObject_Transport*)CObjectManager::GetMe()->NewObject( "CTripperObject_Transport", pPacket->getObjID());

		pTransport->Initial(NULL);	

		//����λ��
		pTransport->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
