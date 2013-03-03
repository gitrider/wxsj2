/*
�յ�����Ϣ,���������Ѿ�����,˫���򿪽��׺н���
*/
#include "stdafx.h"
#include "GCExchangeEstablishI.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"


uint GCExchangeEstablishIHandler::Execute( GCExchangeEstablishI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{//��ʼ��˫�����׺У��򿪽���
			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear(FALSE);

			if(CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID()) == NULL)
			{
				return PACKET_EXE_CONTINUE ;				
			}
			INT nObjId = CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID())->GetID();

			//���õ�ǰ�뱾�˽��׵����ID
			CDataPool::GetMe()->SetExchangObjID(pPacket->GetObjID());

			//������ʱ��������Ʒ�Ľ��濪�ţ���ô��Ҫǿ�йرգ�����ͬʱ��������������
			if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Synthesize"))
			{
				CEventSystem::GetMe()->PushEvent(GE_CLOSE_SKILL_BOOK);
			}

			//�򿪽���
			CEventSystem::GetMe()->PushEvent(GE_OPEN_EXCHANGE_FRAME,nObjId);

			//����
			CActionSystem::GetMe()->UserExchangeSelf_Update();
			CActionSystem::GetMe()->UserExchangeOther_Update();

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
