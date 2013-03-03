/*
�ӵ�������֪ͨ,��ʾȷ����ť
*/
#include "stdafx.h"
#include "GCExchangeSynchConfirmII.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "GIException.h"

uint GCExchangeSynchConfirmIIHandler::Execute( GCExchangeSynchConfirmII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{//��ʼ��˫�����׺У��򿪽���
			BYTE IsEnable = pPacket->GetIsEnable();

			if(IsEnable)
			{
				CDataPool::GetMe()->MyExBox_SetConfirm(TRUE);
			}
			else
			{
				CDataPool::GetMe()->MyExBox_SetConfirm(FALSE);
			}
			//ˢ�½���
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
