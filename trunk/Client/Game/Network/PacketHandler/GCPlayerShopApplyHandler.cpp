#include "StdAfx.h"
#include "Network\NetManager.h"
#include "GCPlayerShopApply.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "GIException.h"
#include "CGPlayerShopEstablish.h"
#include "Object\Manager\ObjectManager.h"

UINT GCPlayerShopApplyHandler::Execute( GCPlayerShopApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{

			FLOAT ComFactor	=	pPacket->GetComFactor();	//��ǰ��ҵָ��
			UINT  Cost		=	pPacket->GetCost();			//������
			BYTE  nType		=	pPacket->GetType();			//�ɿ�������
			ObjID_t NpcId	=	pPacket->GetNpcId();		//Npc Id

			INT ObjectID = (CObjectManager::GetMe()->FindServerObject(NpcId))->GetID();

			//�յ������Ϣ��һ���������ͨ��
			CDataPool::GetMe()->PlayerShop_SetCommercialFactor(ComFactor);
			CDataPool::GetMe()->PlayerShop_SetCost(Cost);
			CDataPool::GetMe()->PlayerShop_SetType(nType);
			CDataPool::GetMe()->PlayerShop_SetNpcID(ObjectID);

			//��̯λȷ������
			CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_CREATESHOP, ObjectID);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
