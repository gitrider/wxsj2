/*
������Ŀͻ����յ���������
*/
#include "stdafx.h"
#include "GCExchangeApplyI.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "GIException.h"

uint GCExchangeApplyIHandler::Execute( GCExchangeApplyI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//���뽻�׵ȴ�����
			ObjID_t Appid = pPacket->GetObjID();
			CDataPool::GetMe()->AddToAppList(Appid);
			
			/// ----------------------------------------------------------
			// �ڴ����Ҳ���ʾ������ʾͼ��  20100524LL
			CDataPool::GetMe()->SystemTip_AddEventInfo( STT_EXCHANGEINVITE, "�յ�һ����������" );

			// ������ʾͼ��
			CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );
			/// ----------------------------------------------------------

			//֪ͨ������ʾ�û������������㽻��			
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCExchangeApplyHandler_Info_Receive_New_Exchange_Msg");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�յ�һ���µĽ�������");
			CEventSystem::GetMe()->PushEvent(GE_RECEIVE_EXCHANGE_APPLY);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
