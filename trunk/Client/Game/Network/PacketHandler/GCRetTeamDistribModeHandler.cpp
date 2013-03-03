#include "StdAfx.h"
#include "gcretteamdistribmode.h"
#include "DataPool/GMUIDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Event\GMEventSystem.h"


uint	GCRetTeamDistribModeHandler::Execute(GCRetTeamDistribMode *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE nMode = pPacket->getMode();
			if (nMode < ITEM_DISTRIB_NUM)
			{
				if(CUIDataPool::GetMe()->GetTeamOrGroup())
				{
					CUIDataPool::GetMe()->GetTeamOrGroup()->ChangeMode(nMode);
					
					//�����Ϣ����ʵʱ���Ķ�Ա������Ϣ���µ���ʾ��Ϣ
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CHANGE_MODE, nMode );
					//������ӷ���NEW_DEBUGMESSAGE�����ڴ�������ʾ�ı���Ʒ������Ϣ
					switch(nMode)
					{
						case ITEM_FREE_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒʰȡģʽ����Ϊ������ʰȡģʽ");
							}
							break;
						case ITEM_TEAMLEADER_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒʰȡģʽ����Ϊ���ӳ�ʰȡģʽ");
							}
							break;
						case ITEM_INTURNS_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒʰȡģʽ����Ϊ������ʰȡģʽ");
							}
							break;
						case ITEM_SEPARATE_DISTRIB:
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒʰȡģʽ����Ϊ���ֱ�ʰȡģʽ");
							}
							break;
					}
				}
			}
		}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}