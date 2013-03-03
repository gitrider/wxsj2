/*
被申请的客户端收到交易申请
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
			//加入交易等待对列
			ObjID_t Appid = pPacket->GetObjID();
			CDataPool::GetMe()->AddToAppList(Appid);
			
			/// ----------------------------------------------------------
			// 在窗口右侧显示交易提示图标  20100524LL
			CDataPool::GetMe()->SystemTip_AddEventInfo( STT_EXCHANGEINVITE, "收到一个交易请求" );

			// 更新提示图标
			CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );
			/// ----------------------------------------------------------

			//通知界面提示用户有人申请与你交易			
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCExchangeApplyHandler_Info_Receive_New_Exchange_Msg");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"收到一个新的交易请求！");
			CEventSystem::GetMe()->PushEvent(GE_RECEIVE_EXCHANGE_APPLY);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
